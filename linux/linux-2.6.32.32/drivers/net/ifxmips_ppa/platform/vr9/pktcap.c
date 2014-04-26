/*
 * netfilter module for userspace bridged Ethernet frames logging daemons
 *
 *	Authors:
 *	Bart De Schuymer <bdschuym@pandora.be>
 *	Harald Welte <laforge@netfilter.org>
 *
 *  November, 2004
 *
 * Based on ipt_ULOG.c, which is
 * (C) 2000-2002 by Harald Welte <laforge@netfilter.org>
 *
 * This module accepts two parameters: 
 * 
 * nlbufsiz:
 *   The parameter specifies how big the buffer for each netlink multicast
 * group is. e.g. If you say nlbufsiz=8192, up to eight kb of packets will
 * get accumulated in the kernel until they are sent to userspace. It is
 * NOT possible to allocate more than 128kB, and it is strongly discouraged,
 * because atomically allocating 128kB inside the network rx softirq is not
 * reliable. Please also keep in mind that this buffer size is allocated for
 * each nlgroup you are using, so the total kernel memory usage increases
 * by that factor.
 *
 * flushtimeout:
 *   Specify, after how many hundredths of a second the queue should be
 *   flushed even if it is not full yet.
 *
 */


#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/socket.h>
#include <linux/skbuff.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/netlink.h>
#include <linux/netdevice.h>
#include <linux/module.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_ulog.h>
#include <net/sock.h>
/*#include "../br_private.h"*/

#define	NETLINK_PKTCAP		30

#define PRINTR(format, args...) do { if (net_ratelimit()) \
                                printk(format , ## args); } while (0)

static unsigned int nlbufsiz = NLMSG_GOODSIZE;
static unsigned int flushtimeout = 10;

typedef struct {
	unsigned int qlen;		/* number of nlmsgs' in the skb */
	struct nlmsghdr *lastnlh;	/* netlink header of last msg in skb */
	struct sk_buff *skb;		/* the pre-allocated skb */
	struct timer_list timer;	/* the timer function */
	spinlock_t lock;		/* the per-queue lock */
} ebt_ulog_buff_t;

static ebt_ulog_buff_t ulog_buffers[EBT_ULOG_MAXNLGROUPS];
static struct sock *ebtulognl;

#define	EBT_ULOG_MAX_PORT		12
#define	EBT_ULOG_MAX_PVC		8
#define	EBT_ULOG_MIN_PKT_LEN	20
#define	EBT_ULOG_PORT_ETH0		0
#define	EBT_ULOG_PORT_ETH1		1
#define	EBT_ULOG_PORT_WLAN		3
#define	EBT_ULOG_PORT_IPOA		6
#define	EBT_ULOG_PORT_DSL		7

typedef struct {
	short			vpi;
	unsigned short	vci;
} ebt_ulog_pvc;

typedef struct {
	unsigned long	rxCnt;
	unsigned long	rxCpy;
	unsigned long	txCnt;
	unsigned long	txCpy;
} ebt_ulog_cnt;

static int				pktcap_enable;
static unsigned short	pktcap_rxportlist;
static unsigned short	pktcap_txportlist;
static ebt_ulog_pvc		pktcap_rxpvclist[EBT_ULOG_MAX_PVC];
static ebt_ulog_pvc		pktcap_txpvclist[EBT_ULOG_MAX_PVC];
static int				pktcap_pkt_len_min = EBT_ULOG_MIN_PKT_LEN;
static ebt_ulog_cnt		pktcap_portcnt[EBT_ULOG_MAX_PORT];

/* send one ulog_buff_t to userspace */
static void ulog_send(unsigned int nlgroup)
{
	ebt_ulog_buff_t *ub = &ulog_buffers[nlgroup];

	if (timer_pending(&ub->timer))
		del_timer(&ub->timer);

	if (!ub->skb)
		return;

	/* last nlmsg needs NLMSG_DONE */
	if (ub->qlen > 1)
		ub->lastnlh->nlmsg_type = NLMSG_DONE;

	NETLINK_CB(ub->skb).dst_group = nlgroup + 1;
	netlink_broadcast(ebtulognl, ub->skb, 0, nlgroup + 1, GFP_ATOMIC);

	ub->qlen = 0;
	ub->skb = NULL;
}

/* timer function to flush queue in flushtimeout time */
static void ulog_timer(unsigned long data)
{
	spin_lock_bh(&ulog_buffers[data].lock);
	if (ulog_buffers[data].skb)
		ulog_send(data);
	spin_unlock_bh(&ulog_buffers[data].lock);
}

static struct sk_buff *ulog_alloc_skb(unsigned int size)
{
	struct sk_buff *skb;
	unsigned int n;

	n = max(size, nlbufsiz);
	skb = alloc_skb(n, GFP_ATOMIC);
	if (!skb) {
		PRINTR(KERN_ERR "ebt_ulog: can't alloc whole buffer "
		       "of size %ub!\n", n);
		if (n > size) {
			/* try to allocate only as much as we need for
			 * current packet */
			skb = alloc_skb(size, GFP_ATOMIC);
			if (!skb)
				PRINTR(KERN_ERR "ebt_ulog: can't even allocate "
				       "buffer of size %ub\n", size);
		}
	}

	return skb;
}

static char* ebt_ulog_ifname( int port, short vpi, int vci )
{
	static char* ifnames[] = { "eth0", "eth1", "ipoa/pppoa", "dsl", "wlan", "na" };

	switch (port) {
	  case EBT_ULOG_PORT_ETH0:	return ifnames[0];
	  case EBT_ULOG_PORT_ETH1:	return ifnames[1];
	  case EBT_ULOG_PORT_WLAN:	return ifnames[4];
	  case EBT_ULOG_PORT_IPOA:	return ifnames[2];
	  case EBT_ULOG_PORT_DSL:	return ifnames[3];
	  default:					return ifnames[5];
	}
}

static void ebt_ulog_packet(unsigned int hooknr, const struct sk_buff *skb, int off,
   int dir, int port, short vpi, int vci,
   const struct ebt_ulog_info *uloginfo, const char *prefix)
{
	ebt_ulog_packet_msg_t*	pm;
	size_t					size, copy_len;
	struct nlmsghdr*		nlh;
	unsigned int			group = uloginfo->nlgroup;
	ebt_ulog_buff_t*		ub = &ulog_buffers[group];
	spinlock_t*				lock = &ub->lock;

	if ((uloginfo->cprange == 0) ||
	    (uloginfo->cprange > skb->len - off))
		copy_len = skb->len - off;
	else
		copy_len = uloginfo->cprange;

	size = NLMSG_SPACE(sizeof(*pm) + copy_len);
	if (size > nlbufsiz) {
		PRINTR("ebt_ulog: Size %Zd needed, but nlbufsiz=%d\n",
		       size, nlbufsiz);
		return;
	}

	spin_lock_bh(lock);

	if (!ub->skb) {
		if (!(ub->skb = ulog_alloc_skb(size)))
			goto alloc_failure;
	} else if (size > skb_tailroom(ub->skb)) {
		ulog_send(group);

		if (!(ub->skb = ulog_alloc_skb(size)))
			goto alloc_failure;
	}

	nlh = NLMSG_PUT(ub->skb, 0, ub->qlen, 0,
	                size - NLMSG_ALIGN(sizeof(*nlh)));
	ub->qlen++;

	pm = NLMSG_DATA(nlh);

	/* Fill in the ulog data */
	pm->version = EBT_ULOG_VERSION;
	do_gettimeofday(&pm->stamp);
	if (ub->qlen == 1)
		__net_timestamp(ub->skb);
	pm->data_len = copy_len;
	pm->mark = skb->mark;
	pm->hook = hooknr;
	if (uloginfo->prefix != NULL)
		strcpy(pm->prefix, uloginfo->prefix);
	else
		*(pm->prefix) = '\0';

	if (dir == 0) { /*ingress*/
		strcpy(pm->physindev, ebt_ulog_ifname(port,vpi,vci) );
		strcpy(pm->indev, pm->physindev);
	} else
		pm->indev[0] = pm->physindev[0] = '\0';

	if (dir == 1) {
		/* If out exists, then out is a bridge port */
		strcpy(pm->physoutdev, ebt_ulog_ifname(port,vpi,vci) );
		strcpy(pm->outdev, pm->physoutdev);
	} else
		pm->outdev[0] = pm->physoutdev[0] = '\0';

	if (skb_copy_bits(skb, off, pm->data, copy_len) < 0)
		BUG();

	if (ub->qlen > 1)
		ub->lastnlh->nlmsg_flags |= NLM_F_MULTI;

	ub->lastnlh = nlh;

	if (ub->qlen >= uloginfo->qthreshold)
		ulog_send(group);
	else if (!timer_pending(&ub->timer)) {
		ub->timer.expires = jiffies + flushtimeout * HZ / 1000;
		add_timer(&ub->timer);
	}

unlock:
	spin_unlock_bh(lock);

	return;

nlmsg_failure:
	printk(KERN_CRIT "ebt_ulog: error during NLMSG_PUT. This should "
	       "not happen, please report to author.\n");
	goto unlock;
alloc_failure:
	goto unlock;
}

static int ebt_packet_loggable( int dir, int port, short vpi, int vci)
{
	int				cnt;
	int				portlist;
	ebt_ulog_pvc*	ppvclist;

	if (pktcap_enable == 0)
		return 0;

	if ( dir == 0 )
	{
		portlist = pktcap_rxportlist;
		ppvclist = pktcap_rxpvclist;
	}
	else if ( dir == 1 )
	{
		portlist = pktcap_txportlist;
		ppvclist = pktcap_txpvclist;
	}
	else
	{
		return 0;
	}

	if ( (portlist & (0x1<<port)) == 0x0 )
		return 0;

	if ( port == EBT_ULOG_PORT_DSL )
	{
		for ( cnt = 0; cnt < EBT_ULOG_MAX_PVC; cnt++ )
		{
			if (ppvclist[cnt].vpi == 0 && ppvclist[cnt].vci == 0)
				return 1;
			if (ppvclist[cnt].vpi == vpi && ppvclist[cnt].vci == vci)
				return 1;
		}
		return 0;
	}
	else
		return 1;
}

/* this function is registered with the netfilter core
 *  skb - packet buffer
 *  off - offset of head of packet
 *  dir - packet direction, 0 - rx/ingress, 1 - tx/egress
 *  port - port number, 0 - eth0, 1 - eth1, 6 - IPoA,PPPoA, 7 - IPoE,PPPoE,PTM, 3 - WLAN
 */
void ebt_log_packet( const struct sk_buff *skb, int off, int dir, int port, short vpi, int vci)
{
	struct ebt_ulog_info loginfo;

	if (!skb || (int)(skb->len-off) < pktcap_pkt_len_min)
		return;

	if ( port < EBT_ULOG_MAX_PORT )
	{
		if ( dir == 0 )
			pktcap_portcnt[ port ].rxCnt++;
		else
			pktcap_portcnt[ port ].txCnt++;
	}

	if (!ebt_packet_loggable(dir,port,vpi,vci))
		return;

	if ( port < EBT_ULOG_MAX_PORT )
	{
		if ( dir == 0 )
			pktcap_portcnt[ port ].rxCpy++;
		else
			pktcap_portcnt[ port ].txCpy++;
	}

	loginfo.nlgroup = EBT_ULOG_DEFAULT_NLGROUP;
	loginfo.cprange = 0;
	loginfo.qthreshold = EBT_ULOG_DEFAULT_QTHRESHOLD;
	loginfo.prefix[0] = '\0';

	ebt_ulog_packet( 0, skb, off, dir, port, vpi, vci, &loginfo, "WAN_CAP");
}

EXPORT_SYMBOL(ebt_log_packet);

static void proc_write_pktcap_reset( void )
{
	int	cnt;

	pktcap_enable = 0;

	pktcap_rxportlist = 0x0;
	pktcap_txportlist = 0x0;

	for (cnt = 0; cnt < EBT_ULOG_MAX_PVC; cnt++ ) {
		pktcap_rxpvclist[cnt].vpi = -1;
		pktcap_rxpvclist[cnt].vci = 0;
		pktcap_txpvclist[cnt].vpi = -1;
		pktcap_txpvclist[cnt].vci = 0;
	}

	pktcap_pkt_len_min = EBT_ULOG_MIN_PKT_LEN;
}

static void proc_write_pktcap_help( void )
{
	printk(	"Packet capturing:\n"
			"    help                  - show this help message\n"
			"    reset                 - reset packet capturing function to default settings\n"
			"    clear                 - clear counters\n"
			"    enable                - enable packet capturing function\n"
			"    disable               - disable packet capturing function\n"
			"    on  <port> <dir>      - enable <port> <dir> packet capturing\n"
			"    off <port> <dir>      - disable <port> <dir> packet capturing\n"
			"    add <vpi> <vci> <dir> - add ADSL <vpi> <vci> <dir> into packet capturing\n"
			"    del <vpi> <vci> <dir> - remove ADSL <vpi> <vci> <dir> from packet capturing\n"
			"    set <pkt_min_len>     - filter out those packets whose lengthes are less than <pkt_min_len>\n"
			"  <port>                  - eth0, eth1, wlan, ipoa, dsl, or ID 0~15\n"
			"  <dir>                   - rx, tx\n"
			"  <vpi>                   - 0 - 255\n"
			"  <vci>                   - 0 - 65535\n"
			"  <pkt_min_len>           - 0 - 65535, default %d\n"
			, EBT_ULOG_MIN_PKT_LEN
		);
}

static long arc_simple_strtol( const char *cp, char **endp, unsigned int base )
{
	while (*cp==' ' || *cp=='\t')
		cp++;

	if(*cp=='-')
		return -simple_strtoul(cp+1,endp,base);
	return simple_strtoul(cp,endp,base);
}

static int proc_write_pktcap( struct file*	file, const char* buffer, unsigned long	count, void* data )
{
	char*			pPtr;
	char			sBuf[128];
	int				portbitmap;
	long 			portid;
	short			vpi;
	unsigned short 	vci;
	ebt_ulog_pvc*	ppvclist;
	int				tmp, en, cnt;

	sBuf[0] = sBuf[sizeof(sBuf)-1] = '\0';

	/* trim the tailing space, tab and LF/CR*/
	if ( count > 0 )
	{
		if (count >= sizeof(sBuf))
			count = sizeof(sBuf) - 1;

		if (copy_from_user(sBuf, buffer, count))
			return count;

		pPtr = (char*)sBuf + count - 1;

		for (; *pPtr==' ' || *pPtr=='\t' || *pPtr=='\n' || *pPtr=='\r'; pPtr++)
		{
			*pPtr = '\0';
		}
	}

	/* reset */
	if ( strnicmp( sBuf, "reset", sizeof("reset")-1 ) == 0 )
	{
		proc_write_pktcap_reset();
		return count;
	}

	/* clear */
	if ( strnicmp( sBuf, "clear", sizeof("clear")-1 ) == 0 )
	{
		memset( pktcap_portcnt, 0, sizeof(pktcap_portcnt) );
		return count;
	}

	/* enable */
	if ( strnicmp( sBuf, "enable", sizeof("enable")-1 ) == 0 )
	{
		pktcap_enable = 1;
		return count;
	}

	/* disable */
	if ( strnicmp( sBuf, "disable", sizeof("disable")-1 ) == 0 )
	{
		pktcap_enable = 0;
		return count;
	}

	/* on / off */
	if ( strnicmp( sBuf, "on", sizeof("on")-1 ) == 0 || strnicmp( sBuf, "off", sizeof("off")-1 ) == 0 )
	{
		if (strnicmp( sBuf, "on", sizeof("on")-1 ) == 0) {
			en = 1;
			for (pPtr=sBuf+sizeof("on"); *pPtr==' ' || *pPtr=='\t'; pPtr++) { }
		} else {
			en = 0;
			for (pPtr=sBuf+sizeof("off"); *pPtr==' ' || *pPtr=='\t'; pPtr++) { }
		}
		/* <port> */
		if (strnicmp( pPtr, "eth0", sizeof("eth0")-1 ) == 0) {
			portbitmap = (0x1<<EBT_ULOG_PORT_ETH0);
			pPtr += sizeof("eth0");
		}
		else if (strnicmp( pPtr, "eth1", sizeof("eth1")-1 ) == 0) {
			portbitmap = (0x1<<EBT_ULOG_PORT_ETH1);
			pPtr += sizeof("eth1");
		}
		else if (strnicmp( pPtr, "wlan", sizeof("wlan")-1 ) == 0) {
			portbitmap = (0x1<<EBT_ULOG_PORT_WLAN);
			pPtr += sizeof("wlan");
		}
		else if (strnicmp( pPtr, "ipoa", sizeof("ipoa")-1 ) == 0) {
			portbitmap = (0x1<<EBT_ULOG_PORT_IPOA);
			pPtr += sizeof("ipoa");
		}
		else if (strnicmp( pPtr, "dsl", sizeof("dsl")-1 ) == 0) {
			portbitmap = (0x1<<EBT_ULOG_PORT_DSL);
			pPtr += sizeof("dsl");
		}
		else {
			portid = arc_simple_strtol( pPtr, &pPtr, 0 );
			if ( portid < 0 || portid > 15 ) {
				proc_write_pktcap_help();
				return count;
			}
			portbitmap = (0x1<<portid);
		}
		/* <dir> */
		for (; *pPtr==' ' || *pPtr=='\t'; pPtr++) { }
		if (strnicmp( pPtr, "rx", sizeof("rx")-1 ) == 0) {
			if (en)
				pktcap_rxportlist |= portbitmap;
			else
				pktcap_rxportlist &= ~portbitmap;
			return count;
		}
		else if (strnicmp( pPtr, "tx", sizeof("tx")-1 ) == 0) {
			if (en)
				pktcap_txportlist |= portbitmap;
			else
				pktcap_txportlist &= ~portbitmap;
			return count;
		}
		else {
			proc_write_pktcap_help();
			return count;
		}
		return count;
	}

	/* add / del*/
	if ( strnicmp( sBuf, "add", sizeof("add")-1 ) == 0 || strnicmp( sBuf, "del", sizeof("del")-1 ) == 0 )
	{
		if (strnicmp( sBuf, "add", sizeof("add")-1 ) == 0) {
			en = 1;
			for (pPtr=sBuf+sizeof("add"); *pPtr==' ' || *pPtr=='\t'; pPtr++) { }
		} else {
			en = 0;
			for (pPtr=sBuf+sizeof("del"); *pPtr==' ' || *pPtr=='\t'; pPtr++) { }
		}
		/* <vpi> */
		for (; *pPtr==' ' || *pPtr=='\t'; pPtr++) { }
		if (*pPtr == '\0') {
			proc_write_pktcap_help();
			return count;
		}
	    tmp = (int)simple_strtol(pPtr, &pPtr, 0);
		if (tmp < 0 || tmp > 255) {
			proc_write_pktcap_help();
			return count;
		}
		vpi = (short)tmp;
		/* <vci> */
		for (; *pPtr==' ' || *pPtr=='\t'; pPtr++) { }
		if (*pPtr == '\0') {
			proc_write_pktcap_help();
			return count;
		}
	    tmp = (int)simple_strtol(pPtr, &pPtr, 0);
		if (tmp < 0 || tmp > 65535) {
			proc_write_pktcap_help();
			return count;
		}
		vci = (unsigned short)tmp;
		/* <dir> */
		for (; *pPtr==' ' || *pPtr=='\t'; pPtr++) { }
		if (strnicmp( pPtr, "rx", sizeof("rx")-1 ) == 0)
			ppvclist = pktcap_rxpvclist;
		else if (strnicmp( pPtr, "tx", sizeof("tx")-1 ) == 0)
			ppvclist = pktcap_txpvclist;
		else {
			proc_write_pktcap_help();
			return count;
		}
		/* check */
		if (en == 1) { /* add */
			for ( cnt = 0; cnt < EBT_ULOG_MAX_PVC; cnt++ ) {
				if (ppvclist[cnt].vpi == vpi && ppvclist[cnt].vci == vci) {
					printk( "PVC exists\n" );
					return count;
				}
			}
			for ( cnt = 0; cnt < EBT_ULOG_MAX_PVC; cnt++ ) {
				if (ppvclist[cnt].vpi == -1) {
					ppvclist[cnt].vpi = vpi;
					ppvclist[cnt].vci = vci;
					return count;
				}
			}
			printk( "no available PVC entry\n" );
			return count;
		}
		else {
			for ( cnt = 0; cnt < EBT_ULOG_MAX_PVC; cnt++ ) {
				if (ppvclist[cnt].vpi == vpi && ppvclist[cnt].vci == vci) {
					ppvclist[cnt].vpi = -1;
					return count;
				}
			}
			if ( cnt == EBT_ULOG_MAX_PVC ) {
				printk( "PVC doesn not exist\n" );
				return count;
			}
		}
		return count;
	}

	/* set*/
	if ( strnicmp( sBuf, "set", sizeof("set")-1 ) == 0 )
	{
		for (pPtr=sBuf+sizeof("set"); *pPtr==' ' || *pPtr=='\t'; pPtr++) { }
		if (*pPtr == '\0') {
			proc_write_pktcap_help();
			return count;
		}
	    tmp = (int)simple_strtol(pPtr, &pPtr, 0);
		if (tmp < 0 || tmp > 65535) {
			proc_write_pktcap_help();
			return count;
		}
		pktcap_pkt_len_min = tmp;
		return count;
	}

	proc_write_pktcap_help();

	return count;
}

static int proc_read_pktcap(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
	int	cnt;
	int	len = 0;

	len += sprintf( buf+len, "packet capturing: %s\n", (pktcap_enable ? "enable" : "disable") );

	len += sprintf( buf+len, "rx portlist:" );
	for ( cnt = 0; cnt < EBT_ULOG_MAX_PORT; cnt++ ) {
		if ( (pktcap_rxportlist & (0x1<<cnt)) == 0x0 )
			continue;
		len += sprintf( buf+len, " %d", cnt );
		if (cnt == EBT_ULOG_PORT_ETH0)
			len += sprintf( buf+len, "(eth0)" );
		else if (cnt == EBT_ULOG_PORT_ETH1)
			len += sprintf( buf+len, "(eth1)" );
		else if (cnt == EBT_ULOG_PORT_WLAN)
			len += sprintf( buf+len, "(wlan)" );
		else if (cnt == EBT_ULOG_PORT_IPOA)
			len += sprintf( buf+len, "(ipoa)" );
		else if (cnt == EBT_ULOG_PORT_DSL)
			len += sprintf( buf+len, "(dsl)" );
	}
	len += sprintf( buf+len, "\n" );
	for ( cnt = 0; cnt < EBT_ULOG_MAX_PVC; cnt++ )
	{
		if (pktcap_rxpvclist[cnt].vpi == -1)
			continue;
		len += sprintf( buf+len, "rx pvc %d: %d/%d\n", cnt, pktcap_rxpvclist[cnt].vpi, pktcap_rxpvclist[cnt].vci );
	}

	len += sprintf( buf+len, "tx portlist:" );
	for ( cnt = 0; cnt < EBT_ULOG_MAX_PORT; cnt++ ) {
		if ( (pktcap_txportlist & (0x1<<cnt)) == 0x0 )
			continue;
		len += sprintf( buf+len, " %d", cnt );
		if (cnt == EBT_ULOG_PORT_ETH0)
			len += sprintf( buf+len, "(eth0)" );
		else if (cnt == EBT_ULOG_PORT_ETH1)
			len += sprintf( buf+len, "(eth1)" );
		else if (cnt == EBT_ULOG_PORT_WLAN)
			len += sprintf( buf+len, "(wlan)" );
		else if (cnt == EBT_ULOG_PORT_IPOA)
			len += sprintf( buf+len, "(ipoa)" );
		else if (cnt == EBT_ULOG_PORT_DSL)
			len += sprintf( buf+len, "(dsl)" );
	}
	len += sprintf( buf+len, "\n" );
	for ( cnt = 0; cnt < EBT_ULOG_MAX_PVC; cnt++ )
	{
		if (pktcap_txpvclist[cnt].vpi == -1)
			continue;
		len += sprintf( buf+len, "tx pvc %d: %d/%d\n", cnt, pktcap_txpvclist[cnt].vpi, pktcap_txpvclist[cnt].vci );
	}

	len += sprintf( buf+len, "minimum packet length: %d\n", pktcap_pkt_len_min );

	for ( cnt = 0; cnt < EBT_ULOG_MAX_PORT; cnt++ )
	{
		len += sprintf( buf+len, "rxCnt[%02d]: %lu\n", cnt, pktcap_portcnt[cnt].rxCnt );
		len += sprintf( buf+len, "rxCpy[%02d]: %lu\n", cnt, pktcap_portcnt[cnt].rxCpy );
		len += sprintf( buf+len, "txCnt[%02d]: %lu\n", cnt, pktcap_portcnt[cnt].txCnt );
		len += sprintf( buf+len, "txCpy[%02d]: %lu\n", cnt, pktcap_portcnt[cnt].txCpy );
	}

	return len;
}

static int ebt_ulog_init(void)
{
	struct proc_dir_entry*	proc_file_conf;
	int i, ret = 0;

	if (nlbufsiz >= 128*1024) {
		printk(KERN_NOTICE "ebt_ulog: Netlink buffer has to be <= 128kB,"
		       " please try a smaller nlbufsiz parameter.\n");
		return -EINVAL;
	}

	/* initialize ulog_buffers */
	for (i = 0; i < EBT_ULOG_MAXNLGROUPS; i++) {
		init_timer(&ulog_buffers[i].timer);
		ulog_buffers[i].timer.function = ulog_timer;
		ulog_buffers[i].timer.data = i;
		spin_lock_init(&ulog_buffers[i].lock);
	}

	ebtulognl = netlink_kernel_create(&init_net, NETLINK_PKTCAP,
					  EBT_ULOG_MAXNLGROUPS, NULL, NULL,
					  THIS_MODULE);
	if (!ebtulognl)
		ret = -ENOMEM;

  #ifdef CONFIG_ARC_PPP_ULOG
  {	extern void dev_ppp_proc_set_nl_sock( struct sock* new_nl_sock );
	dev_ppp_proc_set_nl_sock( ebtulognl );
  }
  #endif /* CONFIG_ARC_PPP_ULOG */

	proc_write_pktcap_reset();

//	proc_mkdir("eth", NULL);
	proc_file_conf = create_proc_entry("eth/pktcap", 0666, NULL);
	if (proc_file_conf == NULL) {
		printk( "error creating entry in /proc/eth/pktcap\n" );
	} else {
		proc_file_conf->write_proc = proc_write_pktcap;
		proc_file_conf->read_proc  = proc_read_pktcap;
	}

	memset( pktcap_portcnt, 0, sizeof(pktcap_portcnt) );

	return ret;
}

static void ebt_ulog_fini(void)
{
	ebt_ulog_buff_t *ub;
	int i;

  #ifdef CONFIG_ARC_PPP_ULOG
  {	extern void dev_ppp_proc_set_nl_sock( struct sock* new_nl_sock );
	dev_ppp_proc_set_nl_sock( NULL );
  }
  #endif /* CONFIG_ARC_PPP_ULOG */

    remove_proc_entry("eth/pktcap", NULL);

	for (i = 0; i < EBT_ULOG_MAXNLGROUPS; i++) {
		ub = &ulog_buffers[i];
		if (timer_pending(&ub->timer))
			del_timer(&ub->timer);
		spin_lock_bh(&ub->lock);
		if (ub->skb) {
			kfree_skb(ub->skb);
			ub->skb = NULL;
		}
		spin_unlock_bh(&ub->lock);
	}
	sock_release(ebtulognl->sk_socket);
}
