/*
 * Arcadyan middle layer header file of APIs of VoIP configuration
 *
 * Copyright 2011, Arcadyan Corporation
 * All Rights Reserved.
 *
 */

#ifndef _mid_mapi_ccfg_voip_h_
#define _mid_mapi_ccfg_voip_h_

#include "mid_mapi_ccfg.h"

#define MAPI_CCFG_VOICE_UMTS_SUPPORT

//some SIP configuration fields maybe correlated, define them here
//#define MAPI_CCFG_VOICE_PROXY_REGISTRAR_SIPDOMAIN_CORRELATED
//#define MAPI_CCFG_VOICE_PROXYPORT_REGISTRARPORT_CORRELATED

#define MAPI_CCFG_VOIP_MAX_SIP_ACCOUNT_NUM              10
#define MAPI_CCFG_VOIP_MAX_ISDN_NUM                     10
#ifdef MAPI_CCFG_VOICE_UMTS_SUPPORT
#define MAPI_CCFG_VOIP_MAX_TOTAL_ACCOUNT_NUM            (MAPI_CCFG_VOIP_MAX_SIP_ACCOUNT_NUM+MAPI_CCFG_VOIP_MAX_ISDN_NUM+2)  //sip+isdn+pstn+umts
#else
#define MAPI_CCFG_VOIP_MAX_TOTAL_ACCOUNT_NUM            (MAPI_CCFG_VOIP_MAX_SIP_ACCOUNT_NUM+MAPI_CCFG_VOIP_MAX_ISDN_NUM+1)  //sip+isdn+pstn
#endif

#define MAPI_CCFG_VOIP_MAX_DIAL_RULE_NUM                10  //not include number of emergency rules

#define MAPI_CCFG_VOIP_DFT_RTP_MIN_PORT                 5002
#define MAPI_CCFG_VOIP_DFT_RTP_MAX_PORT                 5012

#define MAPI_CCFG_VOIP_DFT_HKFLUSH_MIN                  80
#define MAPI_CCFG_VOIP_DFT_HKFLUSH_MAX                  900

#define MAPI_CCFG_VOIP_DFT_PMTU_ADJ_SZ                  596

typedef enum MAPI_CCFG_VOIP_CODEC_TYPE_E
{
	MAPI_CCFG_VOIP_CODEC_TYPE_NONE		= 0,
	MAPI_CCFG_VOIP_CODEC_TYPE_G711_64_U,
	MAPI_CCFG_VOIP_CODEC_TYPE_G711_64_A,
	MAPI_CCFG_VOIP_CODEC_TYPE_G726_40_A,
	MAPI_CCFG_VOIP_CODEC_TYPE_G726_32_A,
	MAPI_CCFG_VOIP_CODEC_TYPE_G726_24_A,
	MAPI_CCFG_VOIP_CODEC_TYPE_G726_16_A,
	MAPI_CCFG_VOIP_CODEC_TYPE_G729,
	MAPI_CCFG_VOIP_CODEC_TYPE_G722,
	MAPI_CCFG_VOIP_CODEC_TYPE_G723,
	MAPI_CCFG_VOIP_CODEC_TYPE_G728_16,
  #ifdef MAPI_CCFG_VOICE_UMTS_SUPPORT
	MAPI_CCFG_VOIP_CODEC_TYPE_LINE_16_8,
  #endif
	MAPI_CCFG_VOIP_CODEC_TYPE_T38,
	MAPI_CCFG_VOIP_CODEC_TYPE_TELEVT,
	MAPI_CCFG_VOIP_CODEC_TYPE_NBR
} MAPI_CCFG_VOIP_CODEC_TYPE_T;

typedef enum MAPI_CCFG_VOIP_CODEC_PKT_PERIOD_E
{
	MAPI_CCFG_VOIP_CODEC_PKT_PERIOD_20 = 0,
	MAPI_CCFG_VOIP_CODEC_PKT_PERIOD_10_20_30 = 1,
	MAPI_CCFG_VOIP_CODEC_PKT_PERIOD_5_TO_40 = 2,
	MAPI_CCFG_VOIP_CODEC_PKT_PERIOD_50_TO_10_20_30 = 3,
} MAPI_CCFG_VOIP_CODEC_PKT_PERIOD_T;

#if 0
  #define MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM     12
#else
  #define MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM     MAPI_CCFG_VOIP_CODEC_MAX
#endif

#define MAPI_CCFG_VOIP_DFT_RTP_FRAME_TIME               20

#define MAPI_CCFG_VOIP_DFT_SIP_ACC_MAX                  10
#define MAPI_CCFG_VOIP_DFT_LINE_MAX                     10
#define MAPI_CCFG_VOIP_DFT_SESS_LINE_MAX                2
#define MAPI_CCFG_VOIP_DFT_SESS_TOTAL_MAX               4
#define MAPI_CCFG_VOIP_DFT_SIGNAL_PROTO                 "SIP/2.0"
#define MAPI_CCFG_VOIP_DFT_REGIONS                      "JP,US,FR,DE,ES,TW,CA,IT,BE"
#define MAPI_CCFG_VOIP_DFT_URI_SCHEMES                  "sip"
#define MAPI_CCFG_VOIP_DFT_LINE_REGION                  "DE"
#define MAPI_CCFG_VOIP_DFT_SIP_ORG                      ""
#define MAPI_CCFG_VOIP_DFT_RTP_DSCP_MARK                46	// DSCP: EF
#define MAPI_CCFG_VOIP_DFT_RTP_EVT_PAYLOAD_TYPE         101

typedef enum
{
	MAPI_CCFG_VOIP_CAP_LIST_1_DSCP_COUPLED				= 0x0001,
	MAPI_CCFG_VOIP_CAP_LIST_1_ETHERNET_TAGGING_COUPLED	= 0x0002,
	MAPI_CCFG_VOIP_CAP_LIST_1_PSTN_SOFT_SWITCH_OVER		= 0x0004,
	MAPI_CCFG_VOIP_CAP_LIST_1_FAX_T38					= 0x0008,
	MAPI_CCFG_VOIP_CAP_LIST_1_FAX_PASS_THRU				= 0x0010,
	MAPI_CCFG_VOIP_CAP_LIST_1_MODEM_PASS_THRU			= 0x0020,
	MAPI_CCFG_VOIP_CAP_LIST_1_TONE_GEN					= 0x0040,
	MAPI_CCFG_VOIP_CAP_LIST_1_RING_GEN					= 0x0080,
	MAPI_CCFG_VOIP_CAP_LIST_1_NUMBERING_PLAN			= 0x0100,
	MAPI_CCFG_VOIP_CAP_LIST_1_BUTTON_MAP				= 0x0200,
	MAPI_CCFG_VOIP_CAP_LIST_1_SIP_RESPONSE_MAP			= 0x0400,
	MAPI_CCFG_VOIP_DFT_CAP_LIST_1						= (	  MAPI_CCFG_VOIP_CAP_LIST_1_FAX_T38
															| MAPI_CCFG_VOIP_CAP_LIST_1_FAX_PASS_THRU
															| MAPI_CCFG_VOIP_CAP_LIST_1_MODEM_PASS_THRU
														  )
} MAPI_CCFG_VOIP_CAP_LIST_1_T;

typedef enum
{
	MAPI_CCFG_VOIP_SIP_ROLE_USER_AGENT = 0,
	MAPI_CCFG_VOIP_SIP_ROLE_B2B_USER_AGENT = 1,
	MAPI_CCFG_VOIP_SIP_ROLE_OUTBOUND_PROXY = 2,
	MAPI_CCFG_VOIP_DFT_SIP_ROLE = MAPI_CCFG_VOIP_SIP_ROLE_USER_AGENT,
} MAPI_CCFG_VOIP_SIP_ROLE_T;

typedef enum
{
	MAPI_CCFG_VOIP_SIP_EXT_REFER = 0x01,
	MAPI_CCFG_VOIP_SIP_EXT_INFO = 0x02,
	MAPI_CCFG_VOIP_DFT_SIP_EXT = ( MAPI_CCFG_VOIP_SIP_EXT_REFER | MAPI_CCFG_VOIP_SIP_EXT_INFO )
} MAPI_CCFG_VOIP_SIP_EXT_T;

typedef enum
{
	MAPI_CCFG_VOIP_SIP_LINE_PASS_THRU_DIABLE = 0,
	MAPI_CCFG_VOIP_SIP_LINE_PASS_THRU_AUTO = 1,
	MAPI_CCFG_VOIP_SIP_LINE_PASS_THRU_FORCE = 2,
	MAPI_CCFG_VOIP_DFT_SIP_LINE_FAX_PASS_THRU = MAPI_CCFG_VOIP_SIP_LINE_PASS_THRU_AUTO,
	MAPI_CCFG_VOIP_DFT_SIP_LINE_MODEM_PASS_THRU = MAPI_CCFG_VOIP_SIP_LINE_PASS_THRU_AUTO,
} MAPI_CCFG_VOIP_SIP_LINE_PASS_THRU_T;

typedef enum
{
	MAPI_CCFG_VOIP_SIP_TRANSPORT_UDP = 0,
	MAPI_CCFG_VOIP_SIP_TRANSPORT_TCP = 1,
	MAPI_CCFG_VOIP_SIP_TRANSPORT_TLS = 2,
	MAPI_CCFG_VOIP_SIP_TRANSPORT_SCTP = 3,
	MAPI_CCFG_VOIP_DFT_SIP_PROXY_TRANSPORT = MAPI_CCFG_VOIP_SIP_TRANSPORT_UDP,
	MAPI_CCFG_VOIP_DFT_SIP_REGISTRAR_TRANSPORT = MAPI_CCFG_VOIP_SIP_TRANSPORT_UDP,
	MAPI_CCFG_VOIP_DFT_SIP_USER_AGENT_TRANSPORT = MAPI_CCFG_VOIP_SIP_TRANSPORT_UDP,
} MAPI_CCFG_VOIP_SIP_TRANSPORT_T;

typedef enum
{
	MAPI_CCFG_VOIP_FAX_T38_RATE_2400 = 0,
	MAPI_CCFG_VOIP_FAX_T38_RATE_4800 = 1,
	MAPI_CCFG_VOIP_FAX_T38_RATE_7200 = 2,
	MAPI_CCFG_VOIP_FAX_T38_RATE_9600 = 3,
	MAPI_CCFG_VOIP_FAX_T38_RATE_12000 = 4,
	MAPI_CCFG_VOIP_FAX_T38_RATE_14400 = 5,
	MAPI_CCFG_VOIP_FAX_T38_RATE_33600 = 6,
	MAPI_CCFG_VOIP_DFT_FAX_T38_RATE = MAPI_CCFG_VOIP_FAX_T38_RATE_14400,
} MAPI_CCFG_VOIP_FAX_T38_RATE_T;

typedef struct mapi_ccfg_voip_codec {
    unsigned char   codec;                                                      //enum in MAPI_CCFG_VOIP_CODEC_TYPE_T
    unsigned char   selected;                                                   //to specify whether this codec is selected or not (so, items which are supported but not selected should be in the available codec list)
} mapi_ccfg_voip_codec_t;

typedef struct mapi_ccfg_voip_codec_info {
	unsigned char   idx;                                                        //index
	unsigned char   codec;                                                      //enum in MAPI_CCFG_VOIP_CODEC_TYPE_T
	unsigned char   pkt_period;                                                 //enum in MAPI_CCFG_VOIP_CODEC_PKT_PERIOD_T
	unsigned char   silence_suppression;                                        //boolean
	unsigned long   bit_rate;                                                   //to specify whether this codec is selected or not (so, items which are supported but not selected should be in the available codec list)
} mapi_ccfg_voip_codec_info_t;

#define	MAPI_CCFG_VOIP_CODEC_INFO_ALL	\
					  { 1, MAPI_CCFG_VOIP_CODEC_TYPE_G711_64_U, MAPI_CCFG_VOIP_CODEC_PKT_PERIOD_20, 1, 65536L }	\
					, { 2, MAPI_CCFG_VOIP_CODEC_TYPE_G711_64_A, MAPI_CCFG_VOIP_CODEC_PKT_PERIOD_20, 1, 65536L }	\
					, { 3, MAPI_CCFG_VOIP_CODEC_TYPE_G729,      MAPI_CCFG_VOIP_CODEC_PKT_PERIOD_20, 1, 8192L  }	\
					, { 4, MAPI_CCFG_VOIP_CODEC_TYPE_G722,      MAPI_CCFG_VOIP_CODEC_PKT_PERIOD_20, 1, 6451L  }	\
					, { 5, MAPI_CCFG_VOIP_CODEC_TYPE_G726_16_A, MAPI_CCFG_VOIP_CODEC_PKT_PERIOD_20, 1, 16384L }	\
					, { 6, MAPI_CCFG_VOIP_CODEC_TYPE_G726_24_A, MAPI_CCFG_VOIP_CODEC_PKT_PERIOD_20, 1, 24576L }	\
					, { 7, MAPI_CCFG_VOIP_CODEC_TYPE_G726_32_A, MAPI_CCFG_VOIP_CODEC_PKT_PERIOD_20, 1, 32768L }	\
					, { 8, MAPI_CCFG_VOIP_CODEC_TYPE_G726_40_A, MAPI_CCFG_VOIP_CODEC_PKT_PERIOD_20, 1, 40960L }

#define	MAPI_CCFG_VOIP_CODEC_MAX				8
#define	MAPI_CCFG_VOIP_CODEC_IS_VALID( codec )	( (codec) >= 1 && (codec) <= MAPI_CCFG_VOIP_CODEC_MAX )

//global
typedef struct mapi_ccfg_voip_global {
    unsigned char   max_sip_line_num;                   //if no corresponding parameter in system config, it's MAPI_CCFG_VOIP_MAX_SIP_ACCOUNT_NUM and read-only
    unsigned char   max_isdn_msn_num;                   //if no corresponding parameter in system config, it's MAPI_CCFG_VOIP_MAX_ISDN_NUM and read-only
    unsigned char   max_dial_rule_num;                  //if no corresponding parameter in system config, it's MAPI_CCFG_VOIP_MAX_DIAL_RULE_NUM and read-only
    unsigned char   phone_num;                          //e.g., 3 for the case of 2 FXS + ISDN bus

    unsigned char   arcor_customer;
    unsigned char   t38_eb;
    unsigned char   pmtu_adjust_eb;
    unsigned char   bw_mn_eb;

    unsigned short  rtp_port_min;
    unsigned short  rtp_port_max;
    unsigned short  hook_flash_min;                     //in msec
    unsigned short  hook_flash_max;
    unsigned short  pmtu_adjust_sz;
    unsigned short  rtp_frame_ms;                       //rtp frame time, in ms
    unsigned char   fxo_control;

	/*** constant items ***/
	unsigned char   sip_acc_num;
	unsigned char   sip_acc_max;
	unsigned char   line_max;
	unsigned char   sess_line_max;
	unsigned char   sess_total_max;
	char			signal_proto[ sizeof(MAPI_CCFG_VOIP_DFT_SIGNAL_PROTO) ];
	char			regions[ sizeof(MAPI_CCFG_VOIP_DFT_REGIONS) ];
	unsigned short  capability_list_1;					//enum in MAPI_CCFG_VOIP_CAP_LIST_1_T, bitwise-OR
	unsigned char   sip_role;							//enum in MAPI_CCFG_VOIP_SIP_ROLE_T
	unsigned char   sip_ext;							//enum in MAPI_CCFG_VOIP_SIP_EXT_T, bitwise-OR
	char			uri_schemes[ sizeof(MAPI_CCFG_VOIP_DFT_URI_SCHEMES) ];
	unsigned char   rtp_dscp_mark;
	unsigned char   rtp_evt_payload_type;
	unsigned char   fax_t38_rate;						//enum in MAPI_CCFG_VOIP_FAX_T38_RATE_T
	/***********************/

    //unsigned char   all_codec_list[MAPI_CCFG_VOIP_MAX_SUPPORTED_CODEC_NUM];
} mapi_ccfg_voip_global_t;

#define MAPI_CCFG_VOIP_DISPLAY_NAME_MAX_LEN             48
#define MAPI_CCFG_VOIP_USER_ID_MAX_LEN                  48
#define MAPI_CCFG_VOIP_PASSWD_MAX_LEN                   24
#define MAPI_CCFG_VOIP_REALM_MAX_LEN                    64
#define MAPI_CCFG_VOIP_AUTH_ID_MAX_LEN                  40
#define MAPI_CCFG_VOIP_FQDN_MAX_LEN                     64
#define MAPI_CCFG_VOIP_URI_MAX_LEN                      (MAPI_CCFG_VOIP_USER_ID_MAX_LEN+MAPI_CCFG_VOIP_FQDN_MAX_LEN+3)

#define MAPI_CCFG_VOIP_DTMF_INBAND                      0
#define MAPI_CCFG_VOIP_DTMF_OUTBAND                     1
#define MAPI_CCFG_VOIP_DTMF_SIPINFO                     2

#define MAPI_CCFG_VOIP_DFT_SIP_PORT                     5060
#define MAPI_CCFG_VOIP_DFT_REGISTRAR_EXPIRE             3600	// in seconds

#define MAPI_CCFG_VOIP_DFT_STUN_ENABLE                  1
#define MAPI_CCFG_VOIP_DFT_STUN_SERVER                  ""
#define MAPI_CCFG_VOIP_DFT_PSTN_FAIL_OVER               1

//SIP account (per-account)
typedef struct mapi_ccfg_voip_sip_line {
    unsigned char   line_idx;                                                   //0-based
    unsigned char   enable;
    char            display_name[MAPI_CCFG_VOIP_DISPLAY_NAME_MAX_LEN];          //display name
    char            user_id[MAPI_CCFG_VOIP_USER_ID_MAX_LEN];                    //user id (phone number)
    char            userid_area[MAPI_CCFG_VOIP_USER_ID_MAX_LEN];                //user id (phone number)
    char            userid_local[MAPI_CCFG_VOIP_USER_ID_MAX_LEN];               //user id (phone number)
    char            passwd[MAPI_CCFG_VOIP_PASSWD_MAX_LEN];
    char            realm[MAPI_CCFG_VOIP_REALM_MAX_LEN];
    unsigned char   use_auth_id;                                                //0 or 1
    char            auth_id[MAPI_CCFG_VOIP_AUTH_ID_MAX_LEN];
    //unsigned char   area_code_len;                                              //length of area-code in user_id
    unsigned char   listen_wan;
    unsigned short  listen_port;
    char            sipdomain[MAPI_CCFG_VOIP_FQDN_MAX_LEN];                                                 //index of WAN interface, 0-based
    char            registrar[MAPI_CCFG_VOIP_FQDN_MAX_LEN];
    unsigned short  registrar_port;
    unsigned short  registrar_expire;                                           //in seconds
    char            proxy[MAPI_CCFG_VOIP_FQDN_MAX_LEN];                         //outbound proxy
    unsigned short  proxy_port;
    unsigned char   use_ob_proxy;                                               //set 1 to use outbound proxy
    char            ob_proxy[MAPI_CCFG_VOIP_FQDN_MAX_LEN];                      //outbound proxy
    unsigned short  ob_proxy_port;
    unsigned char   dtmf_tx_method;                                             //MAPI_CCFG_VOIP_DTMF_INBAND/MAPI_CCFG_VOIP_DTMF_OUTBAND/...
    unsigned char   fax_passthru_codec;                                         //1: g.711a; 2: g.711u
    unsigned char   use_dnssrv;
    unsigned char   use_fetch_binding;
    char            uri[MAPI_CCFG_VOIP_URI_MAX_LEN];
    unsigned char   codec_config;
    unsigned char   selected_codec_list[MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM];     //the selected codecs, sorted by their priority
	/*** constant items ***/
	char			region[ sizeof(MAPI_CCFG_VOIP_DFT_LINE_REGION) ];
	unsigned char	stun_eb;
	char			stun_server[ sizeof(MAPI_CCFG_VOIP_DFT_STUN_SERVER) ];
	unsigned char	pstn_fail_over;
	unsigned char	fax_pass_thru;
	unsigned char	modem_pass_thru;
	unsigned char	proxy_transport;                                            //enum in MAPI_CCFG_VOIP_SIP_TRANSPORT_T
	unsigned char	registrar_transport;                                        //enum in MAPI_CCFG_VOIP_SIP_TRANSPORT_T
	unsigned char	user_agent_transport;                                       //enum in MAPI_CCFG_VOIP_SIP_TRANSPORT_T
	char			organization[ sizeof(MAPI_CCFG_VOIP_DFT_SIP_ORG) ];
    unsigned short  registrar_period;                                           //in seconds, always equal to registrar_period
	/***********************/
} mapi_ccfg_voip_sip_line_t;

#define MAPI_CCFG_VOIP_ISDN_MAX_NUMBER                  11
#define MAPI_CCFG_VOIP_AREA_CODE_MAX_LEN                10
#define MAPI_CCFG_VOIP_MSN_MAX_LEN                      32
#define MAPI_CCFG_VOIP_ISDN_EXTENSIONS					4

//ISDN MSN numbers (all msn numbers)
typedef struct mapi_ccfg_voip_isdn {
    unsigned char   AddISDNNumberAllowed;
    char            area_code[MAPI_CCFG_VOIP_AREA_CODE_MAX_LEN];
    char            msn_number[MAPI_CCFG_VOIP_ISDN_MAX_NUMBER][MAPI_CCFG_VOIP_MSN_MAX_LEN];
    char            cfu_number[MAPI_CCFG_VOIP_ISDN_MAX_NUMBER][MAPI_CCFG_VOIP_MSN_MAX_LEN];
    char            cfb_number[MAPI_CCFG_VOIP_ISDN_MAX_NUMBER][MAPI_CCFG_VOIP_MSN_MAX_LEN];
    char            cfnr_number[MAPI_CCFG_VOIP_ISDN_MAX_NUMBER][MAPI_CCFG_VOIP_MSN_MAX_LEN];
    char            msn_index[MAPI_CCFG_VOIP_ISDN_EXTENSIONS][MAPI_CCFG_VOIP_ISDN_MAX_NUMBER];
    char            rcv_all[MAPI_CCFG_VOIP_ISDN_EXTENSIONS];
    int             auto_detect_off;
    int             point2point_enable;
} mapi_ccfg_voip_isdn_t;

#ifdef MAPI_CCFG_VOICE_UMTS_SUPPORT
//UMTS
typedef struct mapi_ccfg_voip_umts {
    char            area_code[MAPI_CCFG_VOIP_AREA_CODE_MAX_LEN];
    char            msn_number[MAPI_CCFG_VOIP_MSN_MAX_LEN];
} mapi_ccfg_voip_umts_t;
#endif

#define MAPI_CCFG_VOIP_PHONE_NUM                        3
#define MAPI_CCFG_VOIP_PHONE_F                          0
#define MAPI_CCFG_VOIP_PHONE_U                          1
#define MAPI_CCFG_VOIP_PHONE_ISDN                       2

//the definitions of the below account index bases mainly follow those in Supertask version
#define MAPI_CCFG_VOIP_CONNECT_SIP_IDX_BASE             0
#define MAPI_CCFG_VOIP_CONNECT_ISDN_IDX_BASE            80
#define MAPI_CCFG_VOIP_CONNECT_PSTN_IDX_BASE            90
#define MAPI_CCFG_VOIP_CONNECT_MOBILE_IDX_BASE          92

//in the original design, line types are identified by their account index, e.g., account index in 80-89 is ISDN MSN
//(see the definitions above), however UI/TR069 programmers may not be aware of this.
//so I try to use (line type, logic_idx) tuple to identify account/lines used in extension in/out configurations
//here are definition of "line type"
#define MAPI_CCFG_VOIP_CONNECT_SIP_ACCOUNT              0
#define MAPI_CCFG_VOIP_CONNECT_ISDN_MSN                 1
#define MAPI_CCFG_VOIP_CONNECT_PSTN_NUM                 2
#define MAPI_CCFG_VOIP_CONNECT_MOBILE_NUM               3
//special types
#define MAPI_CCFG_VOIP_CONNECT_ISDN_ALL                 91                      //to represent ISDN when no MSN number is specified
#define MAPI_CCFG_VOIP_CONNECT_FXO_BACKUP               96                      //mainly used in extension out, to specify the case: try SIP accounts first, then FXO if SIP failed
#define MAPI_CCFG_VOIP_CONNECT_FXO_FIRST                97                      //mainly used in extension out, to specify the case: try FXO first, then SIP accounts
#define MAPI_CCFG_VOIP_CONNECT_FXO_ONLY                 98                      //mainly used in extension out, to specify the case: FXO only
#define MAPI_CCFG_VOIP_CONNECT_NONE                     99

#define MAPI_CCFG_VOIP_DFT_ECHO_CANCEL_MS               32

typedef struct mapi_ccfg_voip_extension {
    unsigned char   type;                                                       //MAPI_CCFG_VOIP_CONNECT_SIP_ACCOUNT/MAPI_CCFG_VOIP_CONNECT_ISDN_MSN/...
    unsigned char   logic_idx;                                                  //0-based
} mapi_ccfg_voip_extension_t;

#define MAPI_CCFG_VOIP_DFT_WMI_ENABLE                   0
#define MAPI_CCFG_VOIP_DFT_DND_ENABLE                   0

typedef enum MAPI_CCFG_TEL_PORT_COUNTRY_E
{
	MAPI_CCFG_TEL_PORT_COUNTRY_JAPAN = 0,
	MAPI_CCFG_TEL_PORT_COUNTRY_USA,
	MAPI_CCFG_TEL_PORT_COUNTRY_FRANCE,
	MAPI_CCFG_TEL_PORT_COUNTRY_GERMANY,
	MAPI_CCFG_TEL_PORT_COUNTRY_SPAIN,
	MAPI_CCFG_TEL_PORT_COUNTRY_TAIWAN,
	MAPI_CCFG_TEL_PORT_COUNTRY_CANADA,
	MAPI_CCFG_TEL_PORT_COUNTRY_ITALY,
	MAPI_CCFG_TEL_PORT_COUNTRY_BELGIUM,
	MAPI_CCFG_TEL_PORT_COUNTRY_NUM
} MAPI_CCFG_TEL_PORT_COUNTRY_T;
//phone setting (per-phone)
typedef struct mapi_ccfg_voip_phone {
    unsigned char   phone_idx;                                                  //MAPI_CCFG_VOIP_PHONE_F/MAPI_CCFG_VOIP_PHONE_U/...
    unsigned char   answer_all_eb;
    mapi_ccfg_voip_extension_t   ext_in[MAPI_CCFG_VOIP_MAX_TOTAL_ACCOUNT_NUM];
    mapi_ccfg_voip_extension_t   ext_out[MAPI_CCFG_VOIP_MAX_TOTAL_ACCOUNT_NUM];
    unsigned char   echo_cancel_ms;                                             //buffer length for echo cancelling, in ms
    unsigned char   vad_eb;                                                     //voice activity detection
    unsigned char   call_waiting_eb;
    unsigned char   CLIR_eb;                                                    //CLIR
	/*** constant items ***/
	unsigned char	MWI_eb;                                                     //message waiting indication
	unsigned char	DND_eb;                                                     //do not disturb
	unsigned char   country;
	/***********************/
} mapi_ccfg_voip_phone_t;

#define MAPI_CCFG_VOIP_DIAL_PLAN_PHONE_NUM_MAX_LEN      40
#define MAPI_CCFG_VOIP_DIAL_PLAN_COMMENTS_MAX_LEN       20

//emergency rule and dialing rule (per-rule)
typedef struct mapi_ccfg_voip_dial_rule {
    unsigned char   emergency;                                                  //1 if this is an emergency rule
    unsigned char   rule_idx;                                                   //0-based
    unsigned char   in_use;
    unsigned char   conn_type;                                                  //MAPI_CCFG_VOIP_PHONE_CONNECT_xxxx
    char            phone_number[MAPI_CCFG_VOIP_DIAL_PLAN_PHONE_NUM_MAX_LEN];
    char            comments[MAPI_CCFG_VOIP_DIAL_PLAN_COMMENTS_MAX_LEN];        //empty if no corresponding parameter in system config
    //unsigned char   removable;                                                  //if no corresponding parameter in system config, it's read-only and depends on "emergency"
} mapi_ccfg_voip_dial_rule_t;

#define	MAPI_CCFG_VOIP_CFG_MAX_USERID_LEN				48
#define	MAPI_CCFG_VOIP_CFG_MAX_SPEED_RULE               100

typedef struct mapi_ccfg_voip_speed_dial_rule {
	unsigned char	enable;											/* speed dial rule enable/disable */
	unsigned char	precedent;										/* speed dial rule precedent */
	unsigned char	next;											/* speed dial rule next */
	unsigned char	accountIdx;										/* speed dial rule accountIdx */
	unsigned char	phone_num[ MAPI_CCFG_VOIP_CFG_MAX_USERID_LEN ];	/* phone_num, according to SIP definition or it should be char */
} mapi_ccfg_voip_speed_dial_rule_t;

typedef struct mapi_ccfg_voip_speed_dial {
	unsigned char	enable;											/* speed dial rule enable/disable */
	unsigned char	start;										/* speed dial rule precedent */
	unsigned char	end;											/* speed dial rule next */
	unsigned char	rule_num;										/* speed dial rule accountIdx */
	mapi_ccfg_voip_speed_dial_rule_t	rule[ MAPI_CCFG_VOIP_CFG_MAX_SPEED_RULE ];	/* phone_num, according to SIP definition or it should be char */
} mapi_ccfg_voip_speed_dial_t;

typedef struct mapi_ccfg_voip_special_area
{
    struct mapi_ccfg_voip_account_s {
		char previousRegistrar[MAPI_CCFG_VOIP_FQDN_MAX_LEN];		/* previous registrar */
		char previousUserId[MAPI_CCFG_VOIP_USER_ID_MAX_LEN];		/* previous user id */
		char previousIPPort[32];									/* previous IP and port */
	} voipAccount[MAPI_CCFG_VOIP_MAX_SIP_ACCOUNT_NUM];
}mapi_ccfg_voip_special_area_t;

#ifdef __cplusplus
extern "C" {
#endif


extern int mapi_ccfg_get_voip_global(int tid, mapi_ccfg_voip_global_t* p_voip_glb, int factory_dft);
extern int mapi_ccfg_set_voip_global(int tid, mapi_ccfg_voip_global_t* p_voip_glb);

//note: "line_idx" must be specified in the input data structure
extern int mapi_ccfg_get_voip_sip(int tid, mapi_ccfg_voip_sip_line_t* p_voip_sip, int factory_dft);
extern int mapi_ccfg_set_voip_sip(int tid, mapi_ccfg_voip_sip_line_t* p_voip_sip);
//note: in mapi_ccfg_voip_sip_line_t structure, all codec list and their selected/unselected status,
// and their applying order are represented by two lists, below are help APIs to simply the process of updating codecs
//to get list of unselected codec
extern int mapi_ccfg_get_voip_sip_unselect_codec(mapi_ccfg_voip_sip_line_t* p_voip_sip, unsigned char *unselected_codec, int unselected_codec_sz);
//to select a codec
extern int mapi_ccfg_select_voip_sip_codec(mapi_ccfg_voip_sip_line_t* p_voip_sip, unsigned char codec);
//to unselect a codec
extern int mapi_ccfg_unselect_voip_sip_codec(mapi_ccfg_voip_sip_line_t* p_voip_sip, unsigned char codec);
//to update the list of selected codec
extern int mapi_ccfg_update_sip_selected_codec(mapi_ccfg_voip_sip_line_t* p_voip_sip, char *new_selected_codec, int selected_codec_sz);
//get codec info
extern int mapi_ccfg_get_one_voip_codec_info( unsigned char codec, mapi_ccfg_voip_codec_info_t* p_codec );
extern int mapi_ccfg_get_voip_codec_info_by_idx( unsigned char idx, mapi_ccfg_voip_codec_info_t* p_codec );
extern int mapi_ccfg_get_all_voip_codec_info( mapi_ccfg_voip_codec_info_t* p_codec );

//note: "phone_idx" must be specified in the input data structure
extern int mapi_ccfg_get_voip_phone(int tid, mapi_ccfg_voip_phone_t* p_voip_phone, int factory_dft);
extern int mapi_ccfg_set_voip_phone(int tid, mapi_ccfg_voip_phone_t* p_voip_phone);

//note: "emergency" (whether this is a emergency rule) and "rule_idx" must be specified in the input data structure
extern int mapi_ccfg_get_voip_dial_rule(int tid, mapi_ccfg_voip_dial_rule_t* p_voip_dial_rule, int factory_dft);
extern int mapi_ccfg_set_voip_dial_rule(int tid, mapi_ccfg_voip_dial_rule_t* p_voip_dial_rule);

extern int mapi_ccfg_get_voip_isdn(int tid, mapi_ccfg_voip_isdn_t* p_voip_isdn, int factory_dft);
extern int mapi_ccfg_set_voip_isdn(int tid, mapi_ccfg_voip_isdn_t* p_voip_isdn);

#ifdef MAPI_CCFG_VOICE_UMTS_SUPPORT
extern int mapi_ccfg_get_voip_umts(int tid, mapi_ccfg_voip_umts_t* p_voip_umts, int factory_dft);
extern int mapi_ccfg_set_voip_umts(int tid, mapi_ccfg_voip_umts_t* p_voip_umts);
#endif

//note: "emergency" (whether this is a emergency rule) and "rule_idx" must be specified in the input data structure
extern int mapi_ccfg_get_voip_speed_dial( int tid, mapi_ccfg_voip_speed_dial_t* p_voip_speed_dial );
extern int mapi_ccfg_set_voip_speed_dial( int tid, mapi_ccfg_voip_speed_dial_t* p_voip_speed_dial );


extern int mapi_ccfg_get_voip_special_area( int tid, mapi_ccfg_voip_special_area_t* p_voip_special_area );
extern int mapi_ccfg_set_voip_special_area( int tid, mapi_ccfg_voip_special_area_t* p_voip_special_area );

extern int mapi_ccfg_sync_voip_SIP_Data(int tid, int direction);
/*************** un-implemented function ******************/
extern int mapi_ccfg_reset_voip( int tid, unsigned char line_idx );
/**********************************************************/


#ifdef __cplusplus
}
#endif


#endif // _mid_mapi_ccfg_voip_h_
