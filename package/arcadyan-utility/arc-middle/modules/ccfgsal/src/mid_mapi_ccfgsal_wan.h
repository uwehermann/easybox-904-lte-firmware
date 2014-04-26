/*
 * Arcadyan middle layer header file of APIs of WAN configuration
 *
 * Copyright 2010, Arcadyan Corporation
 * All Rights Reserved.
 *
 */

#ifndef _mid_mapi_ccfg_wan_h_
#define _mid_mapi_ccfg_wan_h_

#include "mid_mapi_ccfg.h"

#define MAPI_CCFG_WAN_IDBASE_ADSL               0
#define MAPI_CCFG_WAN_IDBASE_VDSL               50
#define MAPI_CCFG_WAN_IDBASE_ETHERNET           100
#define MAPI_CCFG_WAN_IDBASE_3G                 200
#define MAPI_CCFG_WAN_IDBASE_LTE                300

#define MAPI_CCFG_WAN_UNKNOWN_VALUE            -1
//#define MAPI_CCFG_WAN_EMPTY_VALUE              -2

#define MAPI_CCFG_WAN_DFT_MAX_WAN_NUM           3

//wan physical types
#define MAPI_CCFG_WAN_TYPE_NUM                  6
#define MAPI_CCFG_WAN_TYPE_DSL_AUTO             0
#define MAPI_CCFG_WAN_TYPE_ADSL                 1
#define MAPI_CCFG_WAN_TYPE_ETHERNET             2
#define MAPI_CCFG_WAN_TYPE_VDSL                 3
#define MAPI_CCFG_WAN_TYPE_3G                   4
#define MAPI_CCFG_WAN_TYPE_LTE                  5
#define MAPI_CCFG_WAN_TYPE_CURR                 MAPI_CCFG_WAN_TYPE_NUM /* special usage to ignore wan type */
#define MAPI_CCFG_WAN_TYPE_DFT                  MAPI_CCFG_WAN_TYPE_DSL_AUTO /* special usage to default wan type */

#define MAPI_CCFG_WAN_SECTION_LEN               MID_CCFG_MAX_SECTION_NAME_LEN
#define MAPI_CCFG_WAN_IFNAME_LEN                16

#define MAPI_CCFG_WAN_ADMIN_NUM                 2
#define MAPI_CCFG_WAN_ADMIN_DISABLE             0
#define MAPI_CCFG_WAN_ADMIN_ENABLE              1
//notice: must update MAPI_CCFG_WAN_ADMIN[] accordingly if changing definitions

#define MAPI_CCFG_WAN_PROTO_NUM                 5
#define MAPI_CCFG_WAN_PROTO_STATIC              0
#define MAPI_CCFG_WAN_PROTO_DHCP                1
#define MAPI_CCFG_WAN_PROTO_PPPOE               2
#define MAPI_CCFG_WAN_PROTO_PPPOA               3
#define MAPI_CCFG_WAN_PROTO_NONE                4
//notice: must update MAPI_CCFG_WAN_PROTO[] accordingly if changing definitions

#define MAPI_CCFG_WAN_IP4_LEN                   16
#define MAPI_CCFG_WAN_DNS_LEN                   128
#define MAPI_CCFG_WAN_USERNAME_LEN              128
#define MAPI_CCFG_WAN_PASSWORD_LEN              128
#define MAPI_CCFG_WAN_NCP_LEN                   16
#define MAPI_CCFG_WAN_DISCONNECT_ACTIVE         0
#define MAPI_CCFG_WAN_DISCONNECT_PASSIVE        1
#define MAPI_CCFG_WAN_DISCONNECTTIME_LEN        8
#define MAPI_CCFG_WAN_MAC_LEN                   20
#define MAPI_CCFG_WAN_DHCPCLIENTID_LEN          128

#define MAPI_CCFG_WAN_DFT_ONDEMAND              0
#define MAPI_CCFG_WAN_DFT_IDLETIMEOUT           20
#define MAPI_CCFG_WAN_DFT_LCP_ECHO_INTERVAL     15
#define MAPI_CCFG_WAN_DFT_LCP_ECHO_FAIL         3

#define MAPI_CCFG_WAN_MAX_VLAN_TAGS             1

#define MAPI_CCFG_WAN_NAT_OFF                   0
#define MAPI_CCFG_WAN_NAT_ON                    1
#define MAPI_CCFG_WAN_NAT_CONDITIONAL           2

#define MAPI_CCFG_WAN_DISABLE_IP6               0
#define MAPI_CCFG_WAN_NATIVE_IP6                1
#define MAPI_CCFG_WAN_MANUAL_IP6_TUNNEL         2   //configured 6in4
#define MAPI_CCFG_WAN_6TO4_IP6_TUNNEL           3
#define MAPI_CCFG_WAN_6RD_IP6_TUNNEL            4

#define MAPI_CCFG_WAN_MAX_IP6_PFX_LEN           44

typedef struct mapi_ccfg_wan_global {
    int             wan_type;
    int             active_wan_type;
    int             ppp_pass_thru;
    int             max_wan_num;
    char            dns1[MAPI_CCFG_WAN_IP4_LEN];
    char            dns2[MAPI_CCFG_WAN_IP4_LEN];    
} mapi_ccfg_wan_global_t;

typedef struct mapi_ccfg_wan {
    int             wan_type;
    int             wan_idx;                                        //0-based
    char            section[MAPI_CCFG_WAN_SECTION_LEN];
    int             adminstate;
    int             proto;
    int             defaultroute;
    char            ifname[MAPI_CCFG_WAN_IFNAME_LEN];
    char            ip4addr[MAPI_CCFG_WAN_IP4_LEN];
    char            ip4mask[MAPI_CCFG_WAN_IP4_LEN];
    char            ip4gateway[MAPI_CCFG_WAN_IP4_LEN];
    char            dns[MAPI_CCFG_WAN_DNS_LEN];
    int             mtu;
    char            username[MAPI_CCFG_WAN_USERNAME_LEN];
    char            password[MAPI_CCFG_WAN_PASSWORD_LEN];
    int             on_demand;
    int             idletime;
    int             auto_reconnect;
    int             lcp_echo_interval;
    int             lcp_echo_fail;
    char            ncp[MAPI_CCFG_WAN_NCP_LEN];
    int             disconnecttype;
    char            disconnecttime[MAPI_CCFG_WAN_DISCONNECTTIME_LEN];
    char            macaddr[MAPI_CCFG_WAN_MAC_LEN];
    int             brvlan_idx;                                     //bridged vlan idx, 1-base
    int             vlan_tagged;                                    //0: no VLAN tag; 1: with 1 VLAN tag; 2: with 2 VLAN tag
    unsigned short  vlan_1p[MAPI_CCFG_WAN_MAX_VLAN_TAGS];
    unsigned short  vlan_id[MAPI_CCFG_WAN_MAX_VLAN_TAGS];

    char            clientid[MAPI_CCFG_WAN_DHCPCLIENTID_LEN];
    int             nat;
    
    char            ip6_enable;
    char            accept_ra;
    //char            send_rs;
    unsigned short  rs_interval;
    char            ip6_tunnel_pfx[MAPI_CCFG_WAN_MAX_IP6_PFX_LEN];  //e.g., 2002::/16
    char            ip6_tunnel_peer[MAPI_CCFG_WAN_IP4_LEN];         //tunnel peer's ipv4 address, e.g., for 6rd it's the border relay's ipv4 address
    char            ip6_tunnel_6rd_manual;
    char            ip6_tunnel_6rd_ip4masklen;
    char            dhcp6c;
} mapi_ccfg_wan_t;

#define MAPI_CCFG_WAN_PVC_ENCAP_NUM             2
#define MAPI_CCFG_WAN_PVC_ENCAP_VCMUX           0
#define MAPI_CCFG_WAN_PVC_ENCAP_LLC             1
//notice: must update MAPI_CCFG_WAN_PVC_ENCAP[] accordingly if changing definitions

#define MAPI_CCFG_WAN_PVC_QOS_NUM               5
#define MAPI_CCFG_WAN_PVC_QOS_CBR               0
#define MAPI_CCFG_WAN_PVC_QOS_VBRnrt            1
#define MAPI_CCFG_WAN_PVC_QOS_UBR               2
#define MAPI_CCFG_WAN_PVC_QOS_VBRrt             3
#define MAPI_CCFG_WAN_PVC_QOS_UBRp              4
//notice: must update MAPI_CCFG_WAN_PVC_QOS[] accordingly if changing definitions

typedef struct mapi_ccfg_pvc {
    int             unit;                                        //0-based
    int             vci;
    int             vpi;
    int             encaps;
    int             qos;
    int             pcr;	/* synonym for max_pcr */
    int             scr;
    int             mbs;
} mapi_ccfg_pvc_t;

#define MAPI_CCFG_WAN_APN_PLMN_LEN              64
#define MAPI_CCFG_WAN_APN_APN_LEN               64
#define MAPI_CCFG_WAN_APN_PIN_LEN               16

#define MAPI_CCFG_WAN_APN_DEFAULT_BAND          20
#define MAPI_CCFG_WAN_APN_DEFAULT_DLCHANNEL     0

typedef struct mapi_ccfg_apn {
    int             unit;                                        //0-based
    int             band;
    int             dl_channel;
    char            plmn[MAPI_CCFG_WAN_APN_PLMN_LEN];
    char            apn[MAPI_CCFG_WAN_APN_APN_LEN];
    char            pin[MAPI_CCFG_WAN_APN_PIN_LEN];
} mapi_ccfg_apn_t;


#ifdef __cplusplus
extern "C" {
#endif


extern int mapi_ccfg_get_wan_real_type( int tid, int wan_type );
extern int mapi_ccfg_get_wan_real_idx( int tid, int wan_type, int wan_idx );
extern int mapi_ccfg_get_wan_idx_base( int tid, int wan_type );
extern int mapi_ccfg_get_wan_global(int tid, mapi_ccfg_wan_global_t* p_wan_glb, int factory_dft);
extern int mapi_ccfg_set_wan_global(int tid, mapi_ccfg_wan_global_t* p_wan_glb);
extern int mapi_ccfg_get_wan(int tid, mapi_ccfg_wan_t* p_wan_cfg, int factory_dft);
extern int mapi_ccfg_set_wan(int tid, mapi_ccfg_wan_t* p_wan_cfg);
extern int mapi_ccfg_get_pvc(int tid, mapi_ccfg_pvc_t* p_pvc_cfg, int factory_dft);
extern int mapi_ccfg_set_pvc(int tid, mapi_ccfg_pvc_t* p_pvc_cfg);
extern int mapi_ccfg_get_apn(int tid, mapi_ccfg_apn_t* p_apn_cfg, int factory_dft);
extern int mapi_ccfg_set_apn(int tid, mapi_ccfg_apn_t* p_apn_cfg);
extern int mapi_ccfg_enable_wan(int tid, int wan_type, int wan_idx, int adminstate);

extern const char *MAPI_CCFG_WAN_ADMIN[MAPI_CCFG_WAN_ADMIN_NUM];
extern const char *MAPI_CCFG_WAN_PROTO[MAPI_CCFG_WAN_PROTO_NUM];
extern const char *MAPI_CCFG_WAN_PVC_ENCAP[MAPI_CCFG_WAN_PVC_ENCAP_NUM];
extern const char *MAPI_CCFG_WAN_PVC_QOS[MAPI_CCFG_WAN_PVC_QOS_NUM];


#ifdef __cplusplus
}
#endif


#endif // _mid_mapi_ccfg_wan_h_
