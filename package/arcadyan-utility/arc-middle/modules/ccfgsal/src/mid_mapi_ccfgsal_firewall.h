/*
 * Arcadyan middle layer definition for Firewall operations
 *
 * Copyright 2010, Arcadyan Corporation
 * All Rights Reserved.
 *
 */

#ifndef _mid_mapi_ccfg_firewall_h_
#define _mid_mapi_ccfg_firewall_h_

#include "mid_mapi_ccfg.h"
#define CBAC_URL
#define CBAC_SCH
#define CBAC_DOS

//const variable
#define TRUE			1
#define FALSE			0


#define MAX_MAC_FILTER_ENTRY			32

#define NEED_SRC_MAC_FILTER						// is need source mac filter (original MAC filter implement in extended rule)

#define MAX_SERVICE_POOL_SIZE			32		// Maximum service pool size
#define MAX_POLICY_NUM					40		// maximum policy number

#define POLICY_COUNTER_OFFSET2			500		// policy counter offset value 2 (allocate some policy number for other policy type)
#define POLICY_COUNTER_UP				65534U	// policy counter upper boundary
//
//	// type of group rule (return matching rule type of firewall filtering)
#define CBAC_STANDARD_TYPE				1		// match standard rule type
#define CBAC_EXTEND_TYPE				2		// match extended rule type

// application port define
	// tcp port
#define CBAC_FTP_SERV					21		// FTP tcp control port
#define CBAC_TELNET_SERV				23		// telnet tcp port
#define CBAC_SMTP_SERV					25		// SMTP tcp port
#define CBAC_HTTP_SERV1					80		// Http tcp port 1
#define CBAC_POP3_SERV					110		// POP3 tcp port
#define CBAC_AUTH_SERV					113		// authentication service
#define CBAC_NEWS_SERV					119		// News tcp port
#define CBAC_NBIOS_NS					137		// netbios name service
#define CBAC_NBIOS_SS					139		// netbios session service
#define CBAC_HTTPS_SERV					443		// http SSL tcp port
#define CBAC_NBIOS_DS					445		// microsoft nbios data session (Samba, SMB)
#define CBAC_RIP_SERV1					520		// RIP tcp port
#define CBAC_REALPLAYER_SERV			554		// real player tcp port
#define CBAC_WHITE_BOARD_SERV			1503	// NetMeeting white board tcp port
#define CBAC_H323_SERV					1720	// H.323 tcp setup port
#define CBAC_PPTP_SERV					1723	// PPTP tcp control port
#define CBAC_MS_MEDIAPLAYER_SERV		1755	// MS media player tcp port
#define CBAC_MSN_MSG_SERV				1863	// MSN messenger tcp port
#define CBAC_HTTP_SERV2					3128	// Http tcp port 2
#define CBAC_YAHOO_MSG_SERV				5050	// Yahoo messenger tcp port
#define CBAC_AOL_SERV1					5190	// AOL ICQ tcp control port
#define CBAC_XWIN_SERV1					6000	// Linux XWin Server
#define CBAC_XWIN_SERV2					6001	// Linux XWin Server
#define CBAC_NAPSTER_SERV				6699	// napster tcp port
#define CBAC_HTTP_SERV3					8000	// Http tcp port 3
#define CBAC_HTTP_SERV4					8001	// Http tcp port 4
#define CBAC_HTTP_SERV5					8080	// Http tcp port 5
	// udp port
#define CBAC_ECHO_SERV					7		// UDP echo service
#define CBAC_DISCARD_SERV				9		// UDP discard port
#define CBAC_CHARGEN_SERV				19		// character generator service
#define CBAC_DNS_SERV					53		// DNS service
#define CBAC_DHCP_SERV					67		// DHCP server service
#define CBAC_DHCP_CLIENT				68		// DHCP client service
#define CBAC_TFTP_SERV					69		// TFTP udp port
#define CBAC_NT_RPC_SERV				135		// NT RPC service
#define CBAC_SNMP_SERV1					161		// SNMP service
#define CBAC_SNMP_SERV2					162		// SNMP trap service
#define CBAC_IKE_SERV					500		// IKE udp port
#define CBAC_RIP_SERV2					520		// RIP udp port
#define CBAC_L2TP_SERV					1701	// L2TP udp control port
#define CBAC_ICQ_SERV					4000	// Mirabilis ICQ udp control port
#define CBAC_SIP_SERV					5060	// SIP udp port
#define CBAC_AOL_SERV2					5190	// AIM service udp port

// inspect function define
#define INSPECT_N_LOW					0		// valid protocol assign lower bound

#define INSPECT_FRAG_N					0		// fragment inspect function mask index
#define INSPECT_TCP_N					1		// tcp protocol inspect function mask index
#define INSPECT_UDP_N					2		// udp protocol inspect function mask index
#define INSPECT_FTP_N					3		// ftp protocol inspect function mask index
#define INSPECT_H323_N					4		// h.323 protocol inspect function mask index
#define INSPECT_SMTP_N					5		// smtp protocol inspect function mask index
#define INSPECT_HTTP_N					6		// http(with URL) protocol inspect function mask index
#define INSPECT_L2TP_N					7		// L2TP protocol inspect function mask index
#define INSPECT_POP3_N					8		// POP3 protocol inspect function mask index
#define INSPECT_NEWS_N					9		// News protocol inspect function mask index
#define INSPECT_TELNET_N				10		// Telnet protocol inspect function mask index
#define INSPECT_HTTPS_N					11		// Http-SSL protocol inspect function mask index
#define INSPECT_ICQ_N					12		// ICQ service inspect function mask index
#define INSPECT_AOL_N					13		// AOL service inspect function mask index
//#define INSPECT_MSN_MESS_N			14		// MSN Messenger service inspect function mask index
#define INSPECT_SIP_N					14		// SIP service inspect function mask index
#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
#define INSPECT_USER_DEF2_N				15		// user define protocol 2 inspect function mask index
#else
#define INSPECT_YAHOO_MESS_N			15		// Yahoo Messenger service inspect function mask index
#endif // NEED_USER_DEF2_IN_POLICY
#define INSPECT_MS_MEDIA_N				16		// MS media player service inspect function mask index
#define INSPECT_REAL_PLAY_N				17		// real player service inspect function mask index
#define INSPECT_NAPSTER_N				18		// Napster service inspect function mask index
#define INSPECT_PPTP_N					19		// PPTP protocol inspect function mask index
#define INSPECT_TFTP_N					20		// TFTP protocol inspect function mask index
#define INSPECT_HTTP_ONLY_N				21		// http protocol inspect function mask index
#define INSPECT_H323_DAT_N				22		// h.323 protocol data channel inspect function mask index
#define INSPECT_DNS_N					23		// DNS protocol inspect function mask index
#define INSPECT_SNMP_N					24		// SNMP protocol inspect function mask index
#define INSPECT_RIP_N					25		// RIP protocol inspect function mask index
#define INSPECT_XWIN_N					26		// XWin protocol inspect function mask index
#define INSPECT_RPC_NT_N				27		// NT RPC service inspect function mask index
#define INSPECT_AUTH_N					28		// authentication service inspect function mask index
#define INSPECT_NBIOS_N					29		// netbios service inspect function mask index
#ifdef _SURFCONTROL
#define INSPECT_CPA_N					30		// CPA protocol inspect function mask index
#elif defined(_PARECONTROL)
#define INSPECT_PCTL_N					30		// Belkin parent control inspect function mask index
#else
#define INSPECT_tmp3_N					30		// tmp protocol inspect function mask index
#endif // _SURFCONTROL
#define INSPECT_USER_DEF_N				31		// user define protocol inspect function mask index

#define INSPECT_N_UP					31		// valid protocol assign upper bound

	// inspect function mask define
#define INSPECT_FRAG		((unsigned long)0x01L)		// fragment service inspect
#define INSPECT_TCP			((unsigned long)0x02L)		// tcp protocol inspect
#define INSPECT_UDP			((unsigned long)0x04L)		// udp protocol inspect
#define INSPECT_FTP			((unsigned long)0x08L)		// ftp protocol inspect
#define INSPECT_H323		((unsigned long)0x10L)		// h.323 protocol inspect
#define INSPECT_SMTP		((unsigned long)0x20L)		// smtp protocol inspect
#define INSPECT_HTTP		((unsigned long)0x40L)		// http protocol inspect (with URL)
#define INSPECT_L2TP		((unsigned long)0x80L)		// L2TP protocol inspect
#define INSPECT_POP3		((unsigned long)0x100L)		// POP3 protocol inspect
#define INSPECT_NEWS		((unsigned long)0x200L)		// News protocol inspect
#define INSPECT_TELNET		((unsigned long)0x400L)		// Telnet protocol inspect
#define INSPECT_HTTPS		((unsigned long)0x800L)		// Http-SSL protocol inspect
#define INSPECT_ICQ			((unsigned long)0x1000L)	// ICQ service inspect
#define INSPECT_AOL			((unsigned long)0x2000L)	// AOL service inspect
//#define INSPECT_MSN_MESS	((unsigned long)0x4000L)	// MSN Messenger service inspect
#define INSPECT_SIP			((unsigned long)0x4000L)	// SIP service inspect
#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
#define INSPECT_USER_DEF2	((unsigned long)0x8000L)	// user define protocol 2 inspect
#else
#define INSPECT_YAHOO_MESS	((unsigned long)0x8000L)	// Yahoo Messenger service inspect
#endif // NEED_USER_DEF2_IN_POLICY
#define INSPECT_MS_MEDIA	((unsigned long)0x10000L)	// MS media player service inspect
#define INSPECT_REAL_PLAY	((unsigned long)0x20000L)	// real player service inspect
#define INSPECT_NAPSTER		((unsigned long)0x40000L)	// Napster service inspect
#define INSPECT_PPTP		((unsigned long)0x80000L)	// PPTP service inspect
#define INSPECT_TFTP		((unsigned long)0x100000L)	// TFTP protocol inspect
#define INSPECT_HTTP_ONLY	((unsigned long)0x200000L)	// http protocol inspect
#define INSPECT_H323_DAT	((unsigned long)0x400000L)	// h.323 protocol data channel inspect
#define INSPECT_DNS			((unsigned long)0x800000L)	// DNS protocol inspect
#define INSPECT_SNMP		((unsigned long)0x1000000L)	// SNMP protocol inspect
#define INSPECT_RIP			((unsigned long)0x2000000L)	// RIP protocol inspect
#define INSPECT_XWIN		((unsigned long)0x4000000L)	// XWin protocol inspect
#define INSPECT_RPC_NT		((unsigned long)0x8000000L)	// NT RPC service inspect
#define INSPECT_AUTH		((unsigned long)0x10000000L)// authentication service inspect
#define INSPECT_NBIOS		((unsigned long)0x20000000L)// netbios service inspect
#ifdef _SURFCONTROL
#define INSPECT_CPA			((unsigned long)0x40000000L)// CPA protocol inspect
#elif defined(_PARECONTROL)
#define INSPECT_PCTL		((unsigned long)0x40000000L)// Belkin parent control protocol inspect
#else
#define INSPECT_tmp3		((unsigned long)0x40000000L)// tmp protocol inspect
#endif // _SURFCONTROL
#define INSPECT_USER_DEF	((unsigned long)0x80000000L)// user define protocol inspect


//// flag define of schedule function variable
#define CBAC_SCHE_FUNC_OFF			0
#define CBAC_SCHE_FUNC_ON			1
#define CBAC_SCHE_FUNC_EVERY		2
#define CBAC_SCHE_FUNC_DAY			3
//
//

struct scheduleTime_s {							// [1]~[7]: sunday~saturday,  [0]: everyday
	char stTime_h[8];							// start time hour (0~24, 0 is woldcard, [0] is everyday)
	char stTime_m[8];							// start time min (0~60, 0 is woldcard, [0] is everyday)
	char edTime_h[8];							// end time hour (0~25, 0 is woldcard, [0] is everyday)
	char edTime_m[8];							// end time min (0~60, 0 is woldcard, [0] is everyday)
};

extern short getPolicyNum(short grpID, unsigned short *plyID, unsigned short *priority);

#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
extern int getPolicyStatus(short grpID, unsigned short plyID, char *name, unsigned char *ip, short *schId, char *log, unsigned long *service, short *uProtocol, unsigned short *uDPort, short *uProtocol2, unsigned short *uDPort2);
extern int getPolicyStatusDetail(int tid, short grpID, unsigned short plyID, char *name, unsigned char *ip, unsigned char *mac, short *schId, char *log, unsigned long *service, char *action, short *uProtocol, unsigned short *uDPort, short *uProtocol2, unsigned short *uDPort2, char *func);
#else
int getPolicyStatus(short grpID, unsigned short plyID, char *name, unsigned char *ip, short *schId, char *log, unsigned long *service, short *uProtocol, unsigned short *uDPort);
int getPolicyStatusDetail(int tid, short grpID, unsigned short plyID, char *name, unsigned char *ip, unsigned char *mac, short *schId, char *log, unsigned long *service, char *action, short *uProtocol, unsigned short *uDPort, char *func);
#endif // NEED_USER_DEF2_IN_POLICY

extern int getScheduleRule(short schIdx, char *func, char *name, char *comm, struct scheduleTime_s *time);

#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
extern int setPolicyByAp(short grpID, char *name, char *nameOld, unsigned short idOld, unsigned char *ip, short schId, char log, unsigned long service, short uProtocol, unsigned short *uDPort, short uProtocol2, unsigned short *uDPort2);
#else
extern int setPolicyByAp(short grpID, char *name, char *nameOld, unsigned short idOld, unsigned char *ip, short schId, char log, unsigned long service, short uProtocol, unsigned short *uDPort);
#endif // NEED_USER_DEF2_IN_POLICY
extern char getCBACFuncSpec(signed char *SpiDos, signed char *ping, signed char *rip, signed char *dmz);
extern int setExtPolicy(short grpID, unsigned short plyID, unsigned char sip_lo, unsigned char sip_hi, unsigned short sPort_lo, unsigned short sPort_hi, unsigned char *sMac, char protoType);
extern int setFrameFilterPolicyDetail(short grpID, unsigned short plyID, char action, unsigned char *sMac, unsigned short eTypeLo, unsigned short eTypeHi);	// newFeature,  ## 1.04.49xx
extern int setFrameFilterPolicy(short grpID, unsigned short plyID, char action, unsigned char *sMac);
extern int getFrameFilterPolicyDetail(short grpID, unsigned short plyID, unsigned char *sMac, unsigned short *eTypeLo, unsigned short *eTypeHi);	// newFeature,  ## 1.04.49xx
extern int getFrameFilterPolicy(short grpID, unsigned short plyID, unsigned char *sMac);

//=====================config.h
	// functionality define
  #ifdef CBAC_SP
	#define CBAC_INSPECT_IPSPOOF			// ip spoofing detect module
  #endif // CBAC_SP
  #ifdef CBAC_FR
	#define CBAC_INSPECT_FRAGMENT			// fragmentation inspection module
  #endif // CBAC_FR
  #ifdef CBAC_TCP
	#define CBAC_INSPECT_TCP				// tcp inspection module
  #endif // CBAC_TCP
  #ifdef CBAC_UDP
	#define CBAC_INSPECT_UDP				// udp inspection module
  #endif // CBAC_UDP
  #ifdef CBAC_DOS
	#define CBAC_INSPECT_DOS				// some denial of service patern detect module
  #endif // CBAC_DOS
  #ifdef CBAC_SCH
	#define CBAC_RULE_SCHEDULE				// access control rule schedule function
  #endif // CBAC_SCH
  #ifdef CBAC_URL
	#define CBAC_URL_BLOCKING				// application layer Http URL blocking
  #endif // CBAC_URL
  #ifdef CBAC_FRAME
	#define CBAC_FRAME_FILTER				// frame filter module
  #endif // CBAC_FRAME

	// buffer define
	#define MAX_CBAC_STANDARD_RULE_NUM		1				// standard ACL rule pool size
	#define MAX_CBAC_EXTEND_RULE_NUM		250				// extended ACL rule pool size
#ifdef CBAC_CLASSIFY
	//#define MAX_CBAC_CLASSIFY_RULE_NUM		4				// classify rule pool size
	#define MAX_CBAC_CLASSIFY_RULE_NUM		33				// classify rule pool size
#endif // CBAC_CLASSIFY
#ifdef CBAC_FRAME_FILTER
	#define MAX_CBAC_FRAME_RULE_NUM			32				// frame filter rule pool size
#endif // CBAC_FRAME_FILTER
	#define HALF_GRP_RULE_NUM				125				// half of rules number in one access list
	#define MAX_CBAC_GROUP_NUM				4				// accss list pool size
	#define MAX_PORT_NUM					10				// port range pair number in one extended ACL
	#define CBAC_RULE_NAME_LEN				16				// inspection rule name buffer length
	#define MAX_SCHEDULE_RULE_NUM			10				// maximun schedule rule number
	#define MAX_SHC_RULE_COMMENT			20				// schedule rule comment length
	#define MAX_URL_NAME_LEN				128				// maximun URL address string length
	#define MAX_URL_NUM						30				// maximun number of URL string

	// action define (return action of access control)
	#define CBAC_DENY_ACT					0				// deny packet pass through
	#define CBAC_PERMIT_ACT					1				// allow packet pass through
	#define CBAC_COUNT_ACT					2				// counting pass through packet number
	#define CBAC_IPSEC_AH_ACT				3				// identify IPSec-AH packet
	#define CBAC_IPSEC_ESP_ACT				4				// identify IPSec-ESP packet
	#define CBAC_L2TP_ACT					5
#ifndef _CLASSIFY
#ifdef CBAC_CLASSIFY
	#define CBAC_1ST_FORWD                  6
	#define CBAC_2ND_FORWD                  7
#ifdef MULTI_PPPOE
    #define CLSFY_TO_WAN1                   1
    #define CLSFY_TO_WAN2                   2
    #define CLSFY_TO_WAN3                   3
    #define CLSFY_TO_WAN4                   4
    #define CLSFY_TO_DEFAULT                0
#endif // MULTI_PPPOE
#endif
#endif

	#define CBAC_NULL_ACT					10				// identify L2TP packet
//#define IPPROTO_IP              0               /* dummy for IP */
//#define IPPROTO_ICMP            1               /* control message protocol */
//#define IPPROTO_IGMP            2
//#define IPPROTO_TCP             6               /* tcp */
//#define IPPROTO_UDP             17              /* user datagram protocol */
//#define IPPROTO_RSVP            46
	// protocol define (filtering IP protocol field)
	#define CBAC_IP_PROTO					IPPROTO_IP		// IP packet
	#define CBAC_TCP_PROTO					IPPROTO_TCP		// TCP packet
	#define CBAC_UDP_PROTO					IPPROTO_UDP		// UDP packet
	#define CBAC_ICMP_PROTO					IPPROTO_ICMP	// ICMP packet
	#define CBAC_GRE_PROTO					GRE_PROTO		// PPTP GRE packet
	#define CBAC_AH_PROTO					AH_PROTO		// IPSec AH packet
	#define CBAC_ESP_PROTO					ESP_PROTO		// IPSec ESP packet

	// mail log mask
	#define CBAC_LOG_DoS					0x01			// DoS log mask
	#define CBAC_LOG_PING					0x02			// deny ping log mask
	#define CBAC_LOG_RIP					0x04			// RIP defect log mask

	// wan default service mask
	#define CBAC_W_I_TELNET_M				0x01
	#define CBAC_W_I_WWW_M					0x02
	#define CBAC_W_I_SNMP_M					0x04
	#define CBAC_W_I_RIP_M					0x08
	#define CBAC_W_I_DHCP_CLT_M				0x10
	#define CBAC_W_I_PPTP_M					0x20
	#define CBAC_W_I_IKE_M					0x40
	#define CBAC_W_I_H323_M					0x80

	#define CBAC_W_O_NBIOS_M				0x10000

	#define CBAC_W_IDENT_M					0x1000000


#ifdef CBAC_FRAME_FILTER
	#define MAX_CBAC_FRAME_CONTENT			8				// frame filter content size

#endif // CBAC_FRAME_FILTER

#ifdef CBAC_CLASSIFY
	#define	MAX_MAC_ENTRY_NUM				1
	#define MAX_MAC_NUM_PER_RULE			5				// maximum SMAC number per classify rule
	#define POLICYR_CLSFY_GROUP_IDX         4
	#define WAN_CLSFY_GROUP_IDX             3
#ifdef MULTI_PPPOE
	#define MULTI_PPPOE_BACKUP_SESSION_MASK			0x80 // | MULTI_PPPOE_BACKUP_SESSION_MASK
	#define MULTI_PPPOE_BACKUP_SESSION_UNMASK			(~0x80)/*0x7f*/ // & MULTI_PPPOE_BACKUP_SESSION_UNMASK
	#define USED_MULTI_PPPOE_RULE_MASK			0x40 // | USED_MULTI_PPPOE_RULE_MASK
	#define USED_MULTI_PPPOE_RULE_UNMASK			(~0x40) // & USED_MULTI_PPPOE_RULE_MASK
	#define EMPTY_MULTI_PPPOE_RULE_MASK			(~0x40) // & EMPTY_MULTI_PPPOE_RULE_MASK
	#define MULTI_PPPOE_INTERFACE_MASK			(~(MULTI_PPPOE_BACKUP_SESSION_MASK | USED_MULTI_PPPOE_RULE_MASK)) // set interface bit to 1
	#define MULTI_PPPOE_INTERFACE_UNMASK		(MULTI_PPPOE_BACKUP_SESSION_MASK | USED_MULTI_PPPOE_RULE_MASK) // set interface bit to 0

	#define DEFAULT_MULTI_PPPOE_RULE_INDEX	0
	#define BACKUP_SESSION_SHIFT_BIT	7 // right shift
	#define USED_MULTI_PPPOE_RULE_SHIFT_BIT	6 // right shift
#endif // MULTI_PPPOE
	unsigned char	clsfyMacPool[MAX_MAC_ENTRY_NUM][6];


	// domain name setting for multip session
	#define MultiSession_Classify_Num	(MAX_CBAC_CLASSIFY_RULE_NUM-1)
	#define MultiSession_Classify_DomainName_Num	1
	// MultiSession_DomainName[0] <--> ruleClsfy[1]
	// MultiSession_DomainName[1] <--> ruleClsfy[2]
	// MultiSession_DomainName[2] <--> ruleClsfy[3]

#endif //CBAC_CLASSIFY





//=====================cbac.h
struct service_s {
	char name[CBAC_RULE_NAME_LEN];				// pre-define service name
	unsigned short dstPort[MAX_PORT_NUM];		// destination port number
	unsigned char proto;						// IP protocol (CBAC_IP_PROTO/CBAC_ICMP_PROTO/CBAC_TCP_PROTO/
												// CBAC_UDP_PROTO)
	unsigned long mask;							// service mask (0: for empty slot)
};



// frame filter content arrange
#define CBAC_FRAME_SMAC				0
#define CBAC_FRAME_ETHER			6
#define CBAC_FRAME_TOS				8
#define CBAC_FRAME_ID				10
#define CBAC_FRAME_FRAG				12
#define CBAC_FRAME_PROTOCOL			14
#define CBAC_FRAME_SIP				15
#define CBAC_FRAME_DIP				19
#define CBAC_FRAME_SPORT			23
#define CBAC_FRAME_DPORT			25
#define CBAC_FRAME_FLAGS			27
//-------------end cglobal.h --------------------
//unsigned short policyCounter1, policyCounter2;			// unique id for policy identify

extern int Check_IP(char *str);

//external api for other module
extern char mapi_ccfg_firewall_update_ExtRuleSrcSubnet(int tid, long ruleID, unsigned char *oldSubnet, unsigned char *newSubnet);
extern char mapi_ccfgsal_firewall_set_CBACFunc(int tid, signed char inspect, signed char dos, signed char ping, signed char rip, signed char dmz);
extern char mapi_ccfgsal_firewall_get_URLKeyword(int tid, char *kword, short idx);
extern char mapi_ccfgsal_firewall_set_URLKeyword(int tid, char *kword, short idx);
extern int mapi_ccfgsal_firewall_get_ExtPolicy(int tid, short grpID, unsigned short plyID, unsigned char *sip_lo, unsigned char *sip_hi, unsigned short *sPort_lo, unsigned short *sPort_hi, unsigned char *sMac, char *protoType);
extern int mapi_ccfgsal_firewall_set_ExtPolicy(int tid, short grpID, unsigned short plyID, unsigned char sip_lo, unsigned char sip_hi, unsigned short sPort_lo, unsigned short sPort_hi, unsigned char *sMac, char protoType);
#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
extern int mapi_ccfgsal_firewall_set_PolicyByAp(int tid, short grpID, char *name, char *nameOld, unsigned short idOld, unsigned char *ip, short schId, char log, unsigned long service, short uProtocol, unsigned short *uDPort, short uProtocol2, unsigned short *uDPort2);
#else
extern int mapi_ccfgsal_firewall_set_PolicyByAp(int tid,short grpID, char *name, char *nameOld, unsigned short idOld, unsigned char *ip, short schId, char log, unsigned long service, short uProtocol, unsigned short *uDPort);
#endif // NEED_USER_DEF2_IN_POLICY
extern int mapi_ccfgsal_firewall_del_PolicyById(int tid, short grpID, unsigned short plyID);
extern char mapi_ccfgsal_firewall_get_CBACFuncSpec(int tid, signed char *SpiDos, signed char *ping, signed char *rip, signed char *dmz);
extern int mapi_ccfgsal_firewall_get_ScheduleRule(int tid, short schIdx, char *func, char *name, char *comm, struct scheduleTime_s *time);
#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
extern int mapi_ccfgsal_firewall_get_PolicyStatus(int tid, short grpID, unsigned short plyID, char *name, unsigned char *ip, short *schId, char *log, unsigned long *service, short *uProtocol, unsigned short *uDPort, short *uProtocol2, unsigned short *uDPort2);
#else
extern int mapi_ccfgsal_firewall_get_PolicyStatus(int tid, short grpID, unsigned short plyID, char *name, unsigned char *ip, short *schId, char *log, unsigned long *service, short *uProtocol, unsigned short *uDPort);
#endif // NEED_USER_DEF2_IN_POLICY
extern short mapi_ccfgsal_firewall_get_PolicyNum(int tid, short grpID, unsigned short *plyID, unsigned short *priority);
#endif // _mid_mapi_ccfg_firewall_h_
