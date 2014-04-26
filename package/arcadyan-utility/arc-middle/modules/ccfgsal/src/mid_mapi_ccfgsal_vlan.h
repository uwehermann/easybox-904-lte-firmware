/*
 * Arcadyan middle layer header file of APIs of VLAN configuration
 *
 * Copyright 2010, Arcadyan Corporation
 * All Rights Reserved.
 *
 */

#ifndef _mid_mapi_ccfg_vlan_h_
#define _mid_mapi_ccfg_vlan_h_

#include "mid_mapi_ccfg.h"

#define MAPI_CCFG_VLAN_STATIC_DHCP
#define MAPI_CCFG_VLAN_ALIAS_SUBNET_CONFIG

#define MAPI_CCFG_VLAN_MAX_BR_LAN_NUM           4
#define MAPI_CCFG_VLAN_SECTION_NAME             "vlan"
#define MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM         4
#define MAPI_CCFG_VLAN_DFT_LAN_PORT_NUM         4                           //lan port numbers
#define MAPI_CCFG_VLAN_DFT_SSID_NUM             1                           //wlan port numbers
#define MAPI_CCFG_VLAN_LOGICAL_PORT_NUM         8                           //not include cpu port
#define MAPI_CCFG_VLAN_LOGICAL_PORT_MAP_STR_LEN 64
#define MAPI_CCFG_VLAN_NAME_STR_LEN             64

#define MAPI_CCFG_VLAN_IP4_LEN                  16
#define MAPI_CCFG_VLAN_DHCP_LEASE_LEN           16
#define MAPI_CCFG_VLAN_DOMAINNAME_LEN           64
#define MAPI_CCFG_VLAN_DHCP_OPT43_LEN           512
#define MAPI_CCFG_VLAN_DHCP_OPT60_LEN           64
#define MAPI_CCFG_VLAN_DHCP_OPT240_LEN          64

#ifdef MAPI_CCFG_VLAN_STATIC_DHCP
#define MAPI_CCFG_VLAN_DHCP_MAX_STATIC_ENTRY    10
#define MAPI_CCFG_VLAN_MAC_LEN                  20
#endif

#define MAPI_CCFG_VLAN_DISABLE                  0
#define MAPI_CCFG_VLAN_ENABLE                   1

typedef struct mapi_ccfg_vlan_global {
    int             enable;
    int             max_vlan_num;
    int             active_vlan_num;                                        //number of active vlan
    int             logical_port_num;
    int             logical_cpu_port;                                       //0-based
    char            port_phyconf[MAPI_CCFG_VLAN_LOGICAL_PORT_MAP_STR_LEN];
    int             lan_port[MAPI_CCFG_VLAN_DFT_LAN_PORT_NUM];              //location/index of lan ports (logical) in port_phyconf[] (0-based)
    int             wan_port;                                               //location/index wan port location in port_phyconf[] (0-based)
    int             wan_lan_share;
    int             port_mask[MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM];             //bit mask presenting the LAN ports belonging to each VLAN
    int             ssid_mask[MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM];             //bit mask presenting the WLAN ports(SSIDs) belonging to each VLAN
    int             ethsw_vlanid_to_cpu[MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM];   //the internal vlan id between ethernet switch and CPU 
    int             bind_to_brlan_id[MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM];      //index (1-based) of br-lan to which the interfaces in this vlan is binded
} mapi_ccfg_vlan_global_t;
    
typedef struct mapi_ccfg_vlan {
    int             vlan_idx;                                               //1-based
    //char            vlan_name[MAPI_CCFG_VLAN_NAME_STR_LEN];                 //user-defined readable name/description
    //int             port_mask;                                              //bit mask presenting the LAN ports belonging to this VLAN
    //int             ssid_mask;                                              //bit mask presenting the WLAN ports(SSIDs) belonging to this VLAN
    int             brwan_mask;                                             //bit mask presenting the WAN interfaces bridging with this VLAN
    int             bind_rtwan;                                             //index of the routing WAN interface this VLAN binds to (Note: 0-based wan idx)
    int             vlan_tagged;
    unsigned short  vlan_1p;
    unsigned short  vlan_id;
    char            ip4addr[MAPI_CCFG_VLAN_IP4_LEN];
    char            ip4mask[MAPI_CCFG_VLAN_IP4_LEN];
    int             dhcp4s_eb;
    char            dhcp4pool_s[MAPI_CCFG_VLAN_IP4_LEN];
    char            dhcp4pool_e[MAPI_CCFG_VLAN_IP4_LEN];
    char            dhcp4lease[MAPI_CCFG_VLAN_DHCP_LEASE_LEN];
    char            domainname[MAPI_CCFG_VLAN_DOMAINNAME_LEN];
    char            dhcp4_opt43_vendor_specific[MAPI_CCFG_VLAN_DHCP_OPT43_LEN];
    char            dhcp4_opt60_client_vendor_id[MAPI_CCFG_VLAN_DHCP_OPT60_LEN];
    char            dhcp4_opt240[MAPI_CCFG_VLAN_DHCP_OPT240_LEN];

#ifdef MAPI_CCFG_VLAN_STATIC_DHCP
    char            static_entry_eb[MAPI_CCFG_VLAN_DHCP_MAX_STATIC_ENTRY];
    char            static_ip4[MAPI_CCFG_VLAN_DHCP_MAX_STATIC_ENTRY][MAPI_CCFG_VLAN_IP4_LEN];
    char            static_mac[MAPI_CCFG_VLAN_DHCP_MAX_STATIC_ENTRY][MAPI_CCFG_VLAN_MAC_LEN];
#endif

#ifdef MAPI_CCFG_VLAN_ALIAS_SUBNET_CONFIG
    //alias subnet, usually identified by dhcp option 60
    char            alias_ip4addr[MAPI_CCFG_VLAN_IP4_LEN];
    char            alias_ip4mask[MAPI_CCFG_VLAN_IP4_LEN];
    char            alias_dhcp4pool_s[MAPI_CCFG_VLAN_IP4_LEN];
    char            alias_dhcp4pool_e[MAPI_CCFG_VLAN_IP4_LEN];    
#endif
} mapi_ccfg_vlan_t;


#ifdef __cplusplus
extern "C" {
#endif


extern int mapi_ccfg_get_vlan_global(int tid, mapi_ccfg_vlan_global_t* p_vlan_glb, int factory_dft);
extern int mapi_ccfg_get_vlan(int tid, mapi_ccfg_vlan_t* p_vlan_cfg, int factory_dft);
extern int mapi_ccfg_set_vlan(int tid, mapi_ccfg_vlan_t* p_vlan_cfg);
extern int mapi_ccfg_enable_vlan_global(int tid, int enable);

//extern char mapi_vlan_portmap[MAPI_CCFG_VLAN_DFT_LAN_PORT_NUM+1];


#ifdef __cplusplus
}
#endif


#endif // _mid_mapi_ccfg_vlan_h_
