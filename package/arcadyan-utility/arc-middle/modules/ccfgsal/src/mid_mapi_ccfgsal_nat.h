/*
 * Arcadyan middle layer definition for NAT operations
 *
 * Copyright 2010, Arcadyan Corporation
 * All Rights Reserved.
 *
 */

#ifndef _mid_mapi_ccfg_nat_h_
#define _mid_mapi_ccfg_nat_h_

#include "mid_mapi_ccfg.h"

#define NAT_STR_LEN                 40	/*20*/
#define NAT_IP_LEN                  16
#define NAT_GROUP_NAME_LEN          20

#define ICMP_PROTO      1
#define IGMP_PROTO      2
#define TCP_PROTO       6
#define UDP_PROTO      17
//#ifdef _PPTP
#define GRE_PROTO      47       //added by arthur
//#endif // _PPTP

#ifdef _IPSEC
#define IP_PROTO	     4 //IP in IP (encasulation)
#endif
#define AH_PROTO		51
#define ESP_PROTO		50


#define PAT_PORT_BASE				55000
#define TCP_PORT_BASE				PAT_PORT_BASE

#define GENERAL_TCP_PORT			1000
#define SPECIAL_TCP_PORT			400
#define GENERAL_NTCP_PORT			400
#define SPECIAL_NTCP_PORT			400

#define MAX_TCP_PORT				(GENERAL_TCP_PORT + SPECIAL_TCP_PORT)
#define NTCP_PORT_BASE				(TCP_PORT_BASE + MAX_TCP_PORT)
#define MAX_NTCP_PORT				(GENERAL_NTCP_PORT + SPECIAL_NTCP_PORT)
#define MAX_PAT_PORT				(MAX_TCP_PORT + MAX_NTCP_PORT)


#if !defined(_NUM_SIP_ACCOUNT)
#define	_NUM_SIP_ACCOUNT	10
#endif
#define PORT_RANGE_GROUP_MAX	5
#define IP_RANGE_GROUP_MAX		1

struct IPRange_t{
	unsigned long ip;
	unsigned short count;
	char dummy[2];
}IPRange;
struct PortRange_t{
	short Protocol;
	unsigned short b_port;
	unsigned short e_port;
};
typedef enum _PORTRANGE_SRU_IDX
{
	PORTRANGE_SRU_PROTO,
	PORTRANGE_SRU_BPORT,
	PORTRANGE_SRU_EPORT,
} PORTRANGE_SRU_IDX ;
typedef enum _IPRANGE_SRU_IDX
{
	IPRANGE_SRU_IPF,
	IPRANGE_SRU_IPT,
} IPRANGE_SRU_IDX ;
typedef struct ServicePortRange_t{
	unsigned char 		enable;
	char 				service_name[16];
	struct PortRange_t 	port_range;
} ServicePortRange;

#define SERVICE_NAME_SIP			"SIP"
#define SERVICE_NAME_SIP_RTP		"SIP RTP"
#define SERVICE_NAME_SIP_T38		"SIP T38"
#define SERVICE_NAME_TR69			"TR69"
#define SERVICE_NAME_HTTP_MGMT		"HTTP MGMT"
#define SERVICE_NAME_NCIDD			"NCIDD"

typedef enum _SERVICE_PORT_IDX
{
	SERVICE_PORT_SIP = 0,   // SIP Listen Port
	SERVICE_PORT_SIP_N = SERVICE_PORT_SIP+_NUM_SIP_ACCOUNT-1,
	SERVICE_PORT_SIP_RTP,
	SERVICE_PORT_SIP_T38,
	SERVICE_PORT_TR69,
	SERVICE_PORT_HTTP_MGMT,
	SERVICE_PORT_NCIDD,

	SERVICE_PORT_NUM
} SERVICE_PORT_IDX ;

struct PubServ_t {                                           /* Virtual Server Mapping */
    char group_name[NAT_GROUP_NAME_LEN];
    unsigned char group_member_num;
    struct PortRange_t Global_Port[PORT_RANGE_GROUP_MAX];    /* pseudo port(host order) */
    short Protocol;                                          /* protocol */
    struct IPRange_t Global_IP[IP_RANGE_GROUP_MAX];          /* network order */
    unsigned long Private_IP;                                /* LAN Host ip address */
    struct PortRange_t Private_Port[PORT_RANGE_GROUP_MAX];   /* LAN Host port number(host order) */
    short Netno;                                             /* Interface number */
    char Service_Name[NAT_STR_LEN];                              /* service name */

    // for UPnP compatible ------------ pert, 2002/06/20
    unsigned long leaseTime;      /* this entry life time. 0 for static entry. sec. */
    int remoteIP[4];        /* limited remote IP, [0]=0 for unlimited */
    unsigned char enable;         /* dynamic enable/disable static entry. TRUE/FALSE */
                                  /* useless for dynamic entry */
//    char dummy1;
//    short dummy2;
    // -------------------------------------------------
};                                      /* Protocol = 0 for free entry */



struct AddrMapping_t {
	int func;
	int ifno;
	unsigned long srcipb;
	unsigned long dstipb;
	unsigned long dstipe;
};

typedef enum _NAT_RET
{
	NAT_RET_SUCCESS,
	NAT_RET_FAIL,
} NAT_RET ;

struct Zone_t {
	int ifno;
	char g_addr[17];
	char l_addr[17];
};

extern int mapi_ccfgsal_nat_get_NATStatus(int tid);
extern void mapi_ccfgsal_nat_set_NATStatus(int tid, int isEnable, char** ifname, int sizeif);
extern void mapi_ccfgsal_nat_get_Conf2PubServ(int tid, struct PubServ_t *pub_serv, int idx);
extern void mapi_ccfgsal_nat_set_PubServ2Conf(int tid, int idx, struct PubServ_t *pub_serv);
extern void mapi_ccfgsal_nat_get_AddressMapping(int tid, int idx, struct AddrMapping_t *addr_mapping);
extern void mapi_ccfgsal_nat_set_AddressMapping(int tid, int idx, struct AddrMapping_t *addr_mapping);
extern void mapi_ccfgsal_nat_get_DMZ(int tid, int idx, struct Zone_t *zone);
extern void mapi_ccfgsal_nat_add_DMZ(int tid, int idx, struct Zone_t zone);
extern int mapi_ccfgsal_nat_del_DMZ(int tid, int idx);
long mapi_ccfgsal_nat_portConfig(int tid, struct PortRange_t *ports, int numPort);
long mapi_ccfgsal_nat_findFreePort(int tid, int stPort, int edPort);
long mapi_ccfgsal_nat_findRandFreePort(int tid, int stPort, int edPort);
//////////////////////////HTTP redirect function
void mapi_ccfgsal_nat_redirectEnable(char* hostIP, int isSSLSupported);
void mapi_ccfgsal_nat_redirectDisable(char* hostIP, int isSSLSupported);
void mapi_ccfgsal_nat_dnat(int action, char* proto, char* srcIP, int srcPort, char* dstIP, int dstPort);
//////////////////////////UPnP function
void mapi_ccfgsal_nat_AddUPnPClient(char* proto, char* intf, int sport, char* dhostIP, int dport);
void mapi_ccfgsal_nat_DelUPnPClient(char* proto, char* intf, int sport, char* dhostIP, int dport);

#endif // _mid_mapi_ccfg_nat_h_
