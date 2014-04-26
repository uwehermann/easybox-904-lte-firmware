/*
 * Arcadyan middle layer header file of APIs of WLAN configuration
 *
 * Copyright 2011, Arcadyan Corporation
 * All Rights Reserved.
 *
 */

#ifndef _mid_mapi_ccfg_wlan_h_
#define _mid_mapi_ccfg_wlan_h_

#include "mid_mapi_ccfg.h"

#define MAPI_CCFG_WLAN_CONCURRENT_DUAL_BAND_SUPPORT

#ifdef MAPI_CCFG_WLAN_CONCURRENT_DUAL_BAND_SUPPORT
#define MAPI_CCFG_WLAN_PROFILE_SUPPORT
#define MAPI_CCFG_WLAN_WPS_MUTUALLY_EXCLUSIVE // WPS only enable in One Profile
#endif

#define MAPI_CCFG_WLAN_IP4_LEN                   16
#define MAPI_CCFG_WLAN_MAC_LEN                   20

#define MAPI_CCFG_WLAN_MAX_BAND_NUM             2
#define MAPI_CCFG_WLAN_BAND0                    0
#define MAPI_CCFG_WLAN_BAND1                    1
#define MAPI_CCFG_WLAN_BAND_24GHZ               MAPI_CCFG_WLAN_BAND0
#define MAPI_CCFG_WLAN_BAND_5GHZ                MAPI_CCFG_WLAN_BAND1
#define MAPI_CCFG_WLAN_MAX_SSID_NUM             2


// AP
#define MAPI_CCFG_WLAN_DISABLE                  0
#define MAPI_CCFG_WLAN_ENABLE                   1
#define MAPI_CCFG_WLAN_BY_TIMER                 2

#define MAPI_CCFG_WLAN_11B_MASK                 0x1
#define MAPI_CCFG_WLAN_11G_MASK                 0x2
#define MAPI_CCFG_WLAN_11A_MASK                 0x4
#define MAPI_CCFG_WLAN_11N_MASK                 0x8

#define MAPI_CCFG_WLAN_AUTO_CHANNEL             0

#define MAPI_CCFG_WLAN_BW_20MHZ                 0
#define MAPI_CCFG_WLAN_BW_20MHZ40MHZ            1
#define MAPI_CCFG_WLAN_EXT_CHANNEL_UPPER        0
#define MAPI_CCFG_WLAN_EXT_CHANNEL_LOWER        1

#define MAPI_CCFG_WLAN_SSID_MAX_LEN             64



//WPS
#define MAPI_CCFG_WLAN_WPS_UNCONFIGURED         0
#define MAPI_CCFG_WLAN_WPS_CONFIGURED           1

#define MAPI_CCFG_WLAN_WPS_PIN_MAX_LEN          9

#define MAPI_CCFG_WLAN_WPS_AUTHORIZED_MAC_MAX_NUM   5

//WDS
#define MAPI_CCFG_WLAN_MAX_WDS                  4

//WMM
#define MAPI_CCFG_WLAN_WMM_AC_NUM               4
#define MAPI_CCFG_WLAN_WMM_AC_BK                0
#define MAPI_CCFG_WLAN_WMM_AC_BE                1
#define MAPI_CCFG_WLAN_WMM_AC_VI                2
#define MAPI_CCFG_WLAN_WMM_AC_VO                3
#define MAPI_CCFG_WLAN_WMM_MAX_LEN				64

//802.1X
#define MAPI_CCFG_WLAN_1X_SECRET_KEY_MAX_LEN    64
#define MAPI_CCFG_WLAN_1X_NAS_ID_MAX_LEN        64

//security
#define MAPI_CCFG_WLAN_SEC_OS                   0
#define MAPI_CCFG_WLAN_SEC_WEP                  1
#define MAPI_CCFG_WLAN_SEC_WPA                  2
#define MAPI_CCFG_WLAN_SEC_WPAWPA2              3
#define MAPI_CCFG_WLAN_SEC_WPA2                 4

#define MAPI_CCFG_WLAN_AUTHEN_NO                0
#define MAPI_CCFG_WLAN_AUTHEN_1X                1
#define MAPI_CCFG_WLAN_AUTHEN_PSK               2

#define MAPI_CCFG_WLAN_CYPHER_TKIP              1
#define MAPI_CCFG_WLAN_CYPHER_AES               2

#define MAPI_CCFG_WLAN_WPA_PASSPHRASE_PSK       0
#define MAPI_CCFG_WLAN_WPA_HEX_PSK              1
#define MAPI_CCFG_WLAN_WPA_PASSPHRASE_KEY_LEN   64
#define MAPI_CCFG_WLAN_WPA_HEX_KEY_LEN          65

#define MAPI_CCFG_WLAN_WEP_MAX_KEYLEN 			33

//ACL
#define MAPI_CCFG_WLAN_ACL_NONE					0
#define MAPI_CCFG_WLAN_ACL_ALLOW                1
#define MAPI_CCFG_WLAN_ACL_DENY                 2
#define MAPI_CCFG_WLAN_ACL_MAX_MAC_NUM          32


// Timer
#define MAPI_CCFG_WLAN_TIMER_MAX_RULE_NUM       5
#define MAPI_CCFG_WLAN_TIMER_MAX_LEN			32

//Profile
#ifdef MAPI_CCFG_WLAN_PROFILE_SUPPORT
#define MAPI_CCFG_WLAN_PROFILE_MAX_NUM          2

#define MAPI_CCFG_WLAN_PROFILE_COVER_24GHZ      1
#define MAPI_CCFG_WLAN_PROFILE_COVER_5GHZ       2
#define MAPI_CCFG_WLAN_PROFILE_COVER_DUAL       (MAPI_CCFG_WLAN_PROFILE_COVER_24GHZ|MAPI_CCFG_WLAN_PROFILE_COVER_5GHZ)
#endif

typedef struct mapi_ccfg_wlan_global {
    unsigned char   supported_band_num;                                     //1 or 2
    unsigned char   band0_enable;
    unsigned char   band0_max_ap_num;                                       //max number of SSIDs at band0
#ifdef MAPI_CCFG_WLAN_CONCURRENT_DUAL_BAND_SUPPORT
    unsigned char   band1_enable;
    unsigned char   band1_max_ap_num;
#endif
    //unsigned char   disable_lan_wlan_access;

    //WPS
    unsigned char   wps_version;
    unsigned char   wps_band0_enable;
    unsigned char   wps_band0_configured;                                   //0 for unconfigured; 1 for cionfigured
    unsigned char   wps_band0_pin_len;                                      //8 or 4
    unsigned char   wps_band0_pincode[MAPI_CCFG_WLAN_WPS_PIN_MAX_LEN];
    unsigned char   wps_band0_authorized_mac[MAPI_CCFG_WLAN_WPS_AUTHORIZED_MAC_MAX_NUM][MAPI_CCFG_WLAN_MAC_LEN];    //the mac should be in format xx:xx:xx:xx:xx:xx
#ifdef MAPI_CCFG_WLAN_CONCURRENT_DUAL_BAND_SUPPORT
    unsigned char   wps_band1_enable;
    unsigned char   wps_band1_configured;
    unsigned char   wps_band1_pin_len;
    unsigned char   wps_band1_pincode[MAPI_CCFG_WLAN_WPS_PIN_MAX_LEN];
    unsigned char   wps_band1_authorized_mac[MAPI_CCFG_WLAN_WPS_AUTHORIZED_MAC_MAX_NUM][MAPI_CCFG_WLAN_MAC_LEN];    //the mac should be in format xx:xx:xx:xx:xx:xx
#endif
	unsigned char 	button_act;
} mapi_ccfg_wlan_global_t;

//802.1X configuration
typedef struct mapi_ccfg_wlan_1x {
    unsigned char   srv_ip[MAPI_CCFG_WLAN_IP4_LEN];                         //RADIUS server ip, the ip should be in format x.x.x.x
    unsigned short  srv_port;                                               //RADIUS server port
    unsigned char   secret_key[MAPI_CCFG_WLAN_1X_SECRET_KEY_MAX_LEN];
    unsigned char   nas_id[MAPI_CCFG_WLAN_1X_NAS_ID_MAX_LEN];
    unsigned long   idle_session_timeout;                                   //max session idle time, in ms
    unsigned long   re_auth_period;                                         //re-authentication period, in ms
	unsigned short  quiet_period;                                           //the time period that system would keep in quiet
		                                                                    //after authentication failure of a source MAC, in ms
} mapi_ccfg_wlan_1x_t;

//per-ap(SSID) basic configuration
typedef struct mapi_ccfg_wlan_ap {
    unsigned char   band;                                                   //MAPI_CCFG_WLAN_BAND_24GHZ / MAPI_CCFG_WLAN_BAND_5GHZ
    unsigned char   ap_idx;                                                 //SSID index at the band (0-based)
    unsigned char   enable;                                                 //MAPI_CCFG_WLAN_DISABLE / MAPI_CCFG_WLAN_ENABLE / MAPI_CCFG_WLAN_BY_TIMER
    unsigned char   mode_mask;                                              //bitmask composed of MAPI_CCFG_WLAN_11B_MASK, MAPI_CCFG_WLAN_11G_MASK, ...
    unsigned char   protect;                                                //0 to disable; 1 to enable
    unsigned short  country_code;
    unsigned char   channel;                                                //MAPI_CCFG_WLAN_AUTO_CHANNEL or 1-13
    unsigned char   bandwidth;                                              //MAPI_CCFG_WLAN_BW_20MHZ/MAPI_CCFG_WLAN_BW_40MHZ/...
    unsigned char   ht_ext_channel;                                         //MAPI_CCFG_WLAN_EXT_CHANNEL_UPPER/MAPI_CCFG_WLAN_EXT_CHANNEL_LOWER
    unsigned char   ssid[MAPI_CCFG_WLAN_SSID_MAX_LEN];
    unsigned char   ssid_hidden;
	unsigned char	isolate_wlan_eb;										//enable/disable to separate IP from LAN and WLAN

    //WMM
    unsigned char   wmm_capable;

} mapi_ccfg_wlan_ap_t;

//per-ap(SSID) WDS configuration
typedef struct mapi_ccfg_wlan_wds {
    unsigned char   band;                                                   //MAPI_CCFG_WLAN_BAND_24GHZ / MAPI_CCFG_WLAN_BAND_5GHZ
    unsigned char   ap_idx;                                                 //SSID index at the band (0-based)
    unsigned char   wds_enable;
    unsigned char   wds_peer_eb[MAPI_CCFG_WLAN_MAX_WDS];
    unsigned char   wds_peer_mac[MAPI_CCFG_WLAN_MAX_WDS][MAPI_CCFG_WLAN_MAC_LEN];   //the mac should be in format xx:xx:xx:xx:xx:xx
    unsigned char   wds_peer_ssid[MAPI_CCFG_WLAN_MAX_WDS][MAPI_CCFG_WLAN_SSID_MAX_LEN];
} mapi_ccfg_wlan_wds_t;

//per-ap(SSID) security configuration
typedef struct mapi_ccfg_wlan_sec {
    unsigned char   band;                                                   //MAPI_CCFG_WLAN_BAND_24GHZ / MAPI_CCFG_WLAN_BAND_5GHZ
    unsigned char   ap_idx;                                                 //SSID index at the band (0-based)

    unsigned char   sec_mode;                                               //MAPI_CCFG_WLAN_SEC_OS/MAPI_CCFG_WLAN_SEC_WEP/MAPI_CCFG_WLAN_SEC_WPA/...
    unsigned char   authen_mode;                                            //MAPI_CCFG_WLAN_AUTHEN_NO/MAPI_CCFG_WLAN_AUTHEN_PSK/MAPI_CCFG_WLAN_AUTHEN_1X

    //WEP
    unsigned char   wep_key_len;
    unsigned char   wep_dft_key_id;
    unsigned char   wep_key[4][MAPI_CCFG_WLAN_WEP_MAX_KEYLEN];                                      //for WEP-128 (128 + 24-bit IV = 152), in HEX
    //WPA/WPA2
    unsigned char   wpa_cypher_suite;                                       //it's a bitmask composed of MAPI_CCFG_WLAN_CYPHER_TKIP and MAPI_CCFG_WLAN_CYPHER_AES
    unsigned char   wpa2_cypher_suite;
    unsigned char   psk_key_input;                                          //MAPI_CCFG_WLAN_WPA_PASSPHRASE_PSK/MAPI_CCFG_WLAN_WPA_HEX_PSK
    unsigned char   psk_passphrase[MAPI_CCFG_WLAN_WPA_PASSPHRASE_KEY_LEN];
    unsigned char   psk_256key[MAPI_CCFG_WLAN_WPA_HEX_KEY_LEN];
    unsigned long   group_rekey_period;                                     //period to re-key group-key
    unsigned char   strict_update;                                          //1 to enable strict group key update
} mapi_ccfg_wlan_sec_t;

//per-ap(SSID) WMM configuration
typedef struct mapi_ccfg_wlan_wmm {
    unsigned char   band;                                                   //MAPI_CCFG_WLAN_BAND_24GHZ / MAPI_CCFG_WLAN_BAND_5GHZ
    unsigned char   ap_idx;                                                 //SSID index at the band (0-based)

    unsigned char  ApCWmin[MAPI_CCFG_WLAN_WMM_AC_NUM];
    unsigned char  ApCWmax[MAPI_CCFG_WLAN_WMM_AC_NUM];
    unsigned char  ApAIFSN[MAPI_CCFG_WLAN_WMM_AC_NUM];
    unsigned short ApTxop[MAPI_CCFG_WLAN_WMM_AC_NUM];
    unsigned char  ApACM[MAPI_CCFG_WLAN_WMM_AC_NUM];

    unsigned char  BSSCWmin[MAPI_CCFG_WLAN_WMM_AC_NUM];
    unsigned char  BSSCWmax[MAPI_CCFG_WLAN_WMM_AC_NUM];
    unsigned char  BSSAIFSN[MAPI_CCFG_WLAN_WMM_AC_NUM];
    unsigned short BSSTxop[MAPI_CCFG_WLAN_WMM_AC_NUM];
    unsigned char  BSSACM[MAPI_CCFG_WLAN_WMM_AC_NUM];    
    
} mapi_ccfg_wlan_wmm_t;

//per-ap(SSID) ACL
typedef struct mapi_ccfg_wlan_acl {
    unsigned char   band;                                                   //MAPI_CCFG_WLAN_BAND_24GHZ / MAPI_CCFG_WLAN_BAND_5GHZ
    unsigned char   ap_idx;                                                 //SSID index at the band (0-based)

    unsigned char   enable;
    unsigned char   action;                                                 //MAPI_CCFG_WLAN_ACL_ALLOW/MAPI_CCFG_WLAN_ACL_DENY for matched MAC
    unsigned char   mac[MAPI_CCFG_WLAN_ACL_MAX_MAC_NUM][MAPI_CCFG_WLAN_MAC_LEN];    //the mac should be in format xx:xx:xx:xx:xx:xx
} mapi_ccfg_wlan_acl_t;

// timer for wireless enable/disable
typedef struct mapi_ccfg_wlan_timer_rule{
    unsigned char   weekday_mask;                                           //bit 0: Sunday;
                                                                            //bit 1: Monday; ...
    unsigned char   start_hour;                                             //0-23
    unsigned char   start_min;                                              //0-59
    unsigned char   end_hour;                                               //0-23
    unsigned char   end_min;                                                //0-59    
} mapi_ccfg_wlan_timer_rule_t;

typedef struct mapi_ccfg_wlan_timer {
    unsigned char   band;                                                   //MAPI_CCFG_WLAN_BAND_24GHZ / MAPI_CCFG_WLAN_BAND_5GHZ
    unsigned char   ap_idx;                                                 //SSID index at the band (0-based)

    unsigned char   enable;
    unsigned char   action;                                                 //MAPI_CCFG_WLAN_DISABLE/MAPI_CCFG_WLAN_ENABLE when a rule is matched
    unsigned char   rule_num;
    mapi_ccfg_wlan_timer_rule_t   rule[MAPI_CCFG_WLAN_TIMER_MAX_RULE_NUM];
} mapi_ccfg_wlan_timer_t;


//per-ap(SSID) WPS
typedef struct mapi_ccfg_wlan_wps {
    unsigned char   band;                                                   //MAPI_CCFG_WLAN_BAND_24GHZ / MAPI_CCFG_WLAN_BAND_5GHZ
    unsigned char   ap_idx;                                                 //SSID index at the band (0-based)

    unsigned char   enable;
    unsigned char   configured;                                   //0 for unconfigured; 1 for cionfigured
    unsigned char   pin_len;                                      //8 or 4
    unsigned char   pincode[MAPI_CCFG_WLAN_WPS_PIN_MAX_LEN];
    unsigned char   authorized_mac[MAPI_CCFG_WLAN_WPS_AUTHORIZED_MAC_MAX_NUM][MAPI_CCFG_WLAN_MAC_LEN];    //the mac should be in format xx:xx:xx:xx:xx:xx
} mapi_ccfg_wlan_wps_t;

#ifdef MAPI_CCFG_WLAN_PROFILE_SUPPORT
typedef struct mapi_ccfg_wlan_profile {
    unsigned char   profile_idx;                                            //0-based
    unsigned char   enable;                                                 //MAPI_CCFG_WLAN_DISABLE/MAPI_CCFG_WLAN_ENABLE
    unsigned char   cover_bands;                                            //possible values: 0, MAPI_CCFG_WLAN_PROFILE_COVER_24GHZ, MAPI_CCFG_WLAN_PROFILE_COVER_5GHZ, or (MAPI_CCFG_WLAN_PROFILE_COVER_24GHZ|MAPI_CCFG_WLAN_PROFILE_COVER_5GHZ)
    unsigned char   ssid_hidden;
	unsigned char   ssid[MAPI_CCFG_WLAN_SSID_MAX_LEN];
	unsigned char	isolate_wlan_eb;
} mapi_ccfg_wlan_profile_t;
#endif


typedef struct mapi_ccfg_wlan_device_dft {
 unsigned char   pincode[MAPI_CCFG_WLAN_WPS_PIN_MAX_LEN];
 unsigned char   psk_passphrase[MAPI_CCFG_WLAN_WPA_PASSPHRASE_KEY_LEN];
} mapi_ccfg_wlan_device_dft_t;

#ifdef __cplusplus
extern "C" {
#endif


extern int mapi_ccfg_get_wlan_global(int tid, mapi_ccfg_wlan_global_t* p_wlan_glb, int factory_dft);
extern int mapi_ccfg_set_wlan_global(int tid, mapi_ccfg_wlan_global_t* p_wlan_glb);
extern int mapi_ccfg_get_wlan_1x(int tid, mapi_ccfg_wlan_1x_t* p_wlan_1x_cfg, int factory_dft);
extern int mapi_ccfg_set_wlan_1x(int tid, mapi_ccfg_wlan_1x_t* p_wlan_1x_cfg);

//note: for per-ap configuration APIs, "band" and "ap_idx" must be specified in the input data structure
extern int mapi_ccfg_get_wlan_ap(int tid, mapi_ccfg_wlan_ap_t* p_wlan_ap_cfg, int factory_dft);
extern int mapi_ccfg_set_wlan_ap(int tid, mapi_ccfg_wlan_ap_t* p_wlan_ap_cfg);
extern int mapi_ccfg_get_wlan_wds(int tid, mapi_ccfg_wlan_wds_t* p_wlan_wds_cfg, int factory_dft);
extern int mapi_ccfg_set_wlan_wds(int tid, mapi_ccfg_wlan_wds_t* p_wlan_wds_cfg);
extern int mapi_ccfg_get_wlan_sec(int tid, mapi_ccfg_wlan_sec_t* p_wlan_sec_cfg, int factory_dft);
extern int mapi_ccfg_set_wlan_sec(int tid, mapi_ccfg_wlan_sec_t* p_wlan_sec_cfg);
extern int mapi_ccfg_get_wlan_wmm(int tid, mapi_ccfg_wlan_wmm_t* p_wlan_wmm_cfg, int factory_dft);
extern int mapi_ccfg_set_wlan_wmm(int tid, mapi_ccfg_wlan_wmm_t* p_wlan_wmm_cfg);
extern int mapi_ccfg_get_wlan_acl(int tid, mapi_ccfg_wlan_acl_t* p_wlan_acl_cfg, int factory_dft);
extern int mapi_ccfg_set_wlan_acl(int tid, mapi_ccfg_wlan_acl_t* p_wlan_acl_cfg);
extern int mapi_ccfg_get_wlan_wps(int tid, mapi_ccfg_wlan_wps_t* p_wlan_wps_cfg, int factory_dft);
extern int mapi_ccfg_set_wlan_wps(int tid, mapi_ccfg_wlan_wps_t* p_wlan_wps_cfg);
extern int mapi_ccfg_get_wlan_timer(int tid, mapi_ccfg_wlan_timer_t* p_wlan_timer_cfg, int factory_dft);
extern int mapi_ccfg_set_wlan_timer(int tid, mapi_ccfg_wlan_timer_t* p_wlan_timer_cfg);

#ifdef MAPI_CCFG_WLAN_PROFILE_SUPPORT
extern int mapi_ccfg_get_wlan_profile(int tid, mapi_ccfg_wlan_profile_t* p_wlan_prof_cfg, int factory_dft);
extern int mapi_ccfg_set_wlan_profile(int tid, mapi_ccfg_wlan_profile_t* p_wlan_prof_cfg);
extern int mapi_ccfg_get_wlan_prof_sec(int tid, int profile_idx, mapi_ccfg_wlan_sec_t* p_wlan_sec_cfg, int factory_dft);
extern int mapi_ccfg_set_wlan_prof_sec(int tid, int profile_idx, mapi_ccfg_wlan_sec_t* p_wlan_sec_cfg);
extern int mapi_ccfg_get_wlan_prof_wps(int tid, int profile_idx, mapi_ccfg_wlan_wps_t* p_wlan_wps_cfg, int factory_dft);
extern int mapi_ccfg_set_wlan_prof_wps(int tid, int profile_idx, mapi_ccfg_wlan_wps_t* p_wlan_wps_cfg);
//extern int mapi_ccfg_get_wlan_prof_timer(int tid, int profile_idx, mapi_ccfg_wlan_timer_t* p_wlan_timer_cfg, int factory_dft);
//extern int mapi_ccfg_set_wlan_prof_timer(int tid, int profile_idx, mapi_ccfg_wlan_timer_t* p_wlan_timer_cfg);
extern int mapi_ccfg_get_wlan_prof_acl(int tid, int profile_idx, mapi_ccfg_wlan_acl_t* p_wlan_acl_cfg, int factory_dft);
extern int mapi_ccfg_set_wlan_prof_acl(int tid, int profile_idx, mapi_ccfg_wlan_acl_t* p_wlan_acl_cfg);
extern int mapi_ccfg_set_wlan_prof_wep_random_key(int tid, int profile_idx, unsigned char *p_passphrase ,mapi_ccfg_wlan_sec_t* p_wlan_sec_cfg);

#endif

extern int mapi_ccfg_set_wlan_wps_default_pin(int tid );

extern int mapi_ccfg_get_wlan_device_default(int tid, mapi_ccfg_wlan_device_dft_t* p_wlan_device_dft);


#ifdef __cplusplus
}
#endif


#endif // _mid_mapi_ccfg_wlan_h_
