/*
 * Arcadyan middle layer definition for QoS operations
 *
 * Copyright 2010, Arcadyan Corporation
 * All Rights Reserved.
 *
 */

#ifndef _mid_mapi_ccfg_qos_h_
#define _mid_mapi_ccfg_qos_h_

#include "mid_mapi_ccfg.h"

#define MAPI_CCFG_QOS_PROFILE_NAME_LEN              16
#define MAPI_CCFG_QOS_GROUP_NAME_LEN                16

#define MAPI_CCFG_QOS_MAX_L1_NUM                    4   /* max number of L1 groups */
#define MAPI_CCFG_QOS_MAX_L2_NUM                    4   /* max number of L2 groups per L1 group */
#define MAPI_CCFG_QOS_MAX_FWG_NUM                   8   /* max number of forwarding groups per L2 group */

#define MAPI_CCFG_QOS_BW_STR_LEN                    16

#define MAPI_CCFG_QOS_MAX_L2_EGRESS_IF_NUM          8
#define MAPI_CCFG_QOS_IF_NAME_LEN                   10

#define MAPI_CCFG_QOS_UNSPECIFIED_BW                0xffffffff
#define MAPI_CCFG_QOS_NO_REMARK                     0xffff

enum mapi_ccfg_qos_fwg_e {
	MAPI_CCFG_QOS_FWG_SP=0,
	MAPI_CCFG_QOS_FWG_WRR=1,
};

/* data structures of QoS configuration to be keep in mid core */
typedef struct mapi_ccfg_qos_qm_fwg {                   /* structure of scheduling parameters of a forwarding group*/
    char        pname[MAPI_CCFG_QOS_PROFILE_NAME_LEN];  /* name of the profile */
    char        vname[MAPI_CCFG_QOS_GROUP_NAME_LEN];    /* variable name of the group in .glbcfg, including the NULL end */
    int         id;                                     /* used in tc as the class id */
    enum mapi_ccfg_qos_fwg_e  sp_or_wrr;                /* scheduling type, strict priority or WRR */
    int         priwei;                                 /* priority or weight */
    char        name[MAPI_CCFG_QOS_GROUP_NAME_LEN];     /* logical name of the group, including the NULL end */
} mapi_ccfg_qos_qm_fwg_t;
    
typedef struct mapi_ccfg_qos_qm_L2 {                    /* structure of scheduling parameters of a L2 group*/
    char        pname[MAPI_CCFG_QOS_PROFILE_NAME_LEN];  /* name of the profile */
    char        vname[MAPI_CCFG_QOS_GROUP_NAME_LEN];    /* variable name of the group in .glbcfg, including the NULL end */
    int         id;                                     /* used in tc as the class id */
    int         rv_Kbps;                                /* reserved bandwidth of the L2 group, in Kbps (*1024) */
    int         ul_Kbps;                                /* uplimit bandwidth of the L2 group, in Kbps (*1024) */
    int         egrs_if_num;                            /* involved egress interfaces */
    char        egrs_if_name[MAPI_CCFG_QOS_MAX_L2_EGRESS_IF_NUM][MAPI_CCFG_QOS_IF_NAME_LEN];
    int         egrs_enslv_if_num;                      /* involved enslaved egress interfaces (bridgeing interfaces)*/
    char        egrs_enslv_if_name[MAPI_CCFG_QOS_MAX_L2_EGRESS_IF_NUM][MAPI_CCFG_QOS_IF_NAME_LEN];
    int         fwg_num;                                /* number of forwarding groups */
    int         sp_fwg_num;                             /* number of strict priority forwarding groups */
    int         wrr_reserved_bw;                        /* bandwidth reserved for WRR, in Kbps*/
    char        name[MAPI_CCFG_QOS_GROUP_NAME_LEN];     /* logical name of the group, including the NULL end */
    mapi_ccfg_qos_qm_fwg_t*   fwg[MAPI_CCFG_QOS_MAX_FWG_NUM];
} mapi_ccfg_qos_qm_L2_t;

typedef struct mapi_ccfg_qos_qm_L1 {                    /* structure of scheduling parameters of a L1 group*/
    char        pname[MAPI_CCFG_QOS_PROFILE_NAME_LEN];  /* name of the profile */
    char        vname[MAPI_CCFG_QOS_GROUP_NAME_LEN];    /* variable name of the group in .glbcfg, including the NULL end */
    int         id;                                     /* used in tc as the class id */
    int         ds_Kbps;                                /* downstream rate of the L1 group, in Kbps (*1024) */
    int         us_Kbps;                                /* upstream rate of the L1 group, in Kbps (*1024) */
    int         L2_num;                                 /* number of layer 2 groups */
    char        name[MAPI_CCFG_QOS_GROUP_NAME_LEN];     /* logical name of the group, including the NULL end */
    mapi_ccfg_qos_qm_L2_t*    L2_grp[MAPI_CCFG_QOS_MAX_L2_NUM];
} mapi_ccfg_qos_qm_L1_t;

typedef struct mapi_ccfg_qos_qm_root {                  /* structure of scheduling parameters */
    char        pname[MAPI_CCFG_QOS_PROFILE_NAME_LEN];  /* name of the profile */
    int         ds_Kbps;                                /* downstream rate of root, in Kbps (*1024) */
    int         us_Kbps;                                /* upstream rate of root, in Kbps (*1024) */    
    int         total_fwg_num;                          /* total number of forwarding groups/queues */
    int         L1_num;                                 /* number of layer 1 groups */
    char        dft_fwg[MAPI_CCFG_QOS_GROUP_NAME_LEN];  /* name of the default fwg */
    mapi_ccfg_qos_qm_L1_t*    L1_grp[MAPI_CCFG_QOS_MAX_L1_NUM];
} mapi_ccfg_qos_qm_root_t;

typedef struct mapi_ccfg_qos_global {
    char        profile[MAPI_CCFG_QOS_PROFILE_NAME_LEN];    /* name of the current qos profile */
    int         enable;
} mapi_ccfg_qos_global_t;

typedef struct mapi_ccfg_qos_rate {                     /* structure of rate parameters */
    char        pname[MAPI_CCFG_QOS_PROFILE_NAME_LEN];  /* name of the profile */
    char        vname[MAPI_CCFG_QOS_GROUP_NAME_LEN];    /* variable name of the group in .glbcfg, including the NULL end */
    //int       id;                                     /* id of the target group */
    int         ds_ul_Kbps;                             /* downstream rate (for root, L1) or uplimit bandwidth (for L2) of the group, in Kbps (*1024) */
    int         us_rv_Kbps;                             /* upstream rate (for root, L1) or reserved bandwidth (for L2) of the group, in Kbps (*1024) */
} mapi_ccfg_qos_rate_t;

#define MAPI_CCFG_QOS_CLSFY_RU_NAME_LEN             16
#define MAPI_CCFG_QOS_CLSFY_RU_TAG_LEN              16

#define MAPI_CCFG_QOS_CLSFY_RU_MAC_STR_LEN          100
//#define MAPI_CCFG_QOS_CLSFY_RU_MAC_MAX_NUM        5
#define MAPI_CCFG_QOS_CLSFY_RU_MAC_LEN              20
#define MAPI_CCFG_QOS_CLSFY_RU_MAX_DYN_MAC_NUM      8

#define MAPI_CCFG_QOS_CLSFY_RU_INIF_STR_LEN         50
//#define MAPI_CCFG_QOS_CLSFY_RU_INIF_MAX_NUM       5

#define MAPI_CCFG_QOS_CLSFY_RU_IP_STR_LEN           100
#define MAPI_CCFG_QOS_CLSFY_RU_IP_LEN               20
//#define MAPI_CCFG_QOS_CLSFY_RU_IP_MAX_NUM         5
#define MAPI_CCFG_QOS_CLSFY_RU_MAX_DYN_IP_NUM       32

#define MAPI_CCFG_QOS_CLSFY_RU_APPROTO_STR_LEN      32
#define MAPI_CCFG_QOS_CLSFY_RU_PORT_STR_LEN         64
#define MAPI_CCFG_QOS_CLSFY_RU_DSCP_STR_LEN         32
#define MAPI_CCFG_QOS_CLSFY_RU_8021P_STR_LEN        32
#define MAPI_CCFG_QOS_CLSFY_RU_ETHTYPE_STR_LEN      16
#define MAPI_CCFG_QOS_CLSFY_RU_PKTLEN_STR_LEN       32
#define MAPI_CCFG_QOS_CLSFY_RU_DOMAIN_STR_LEN       128
#define MAPI_CCFG_QOS_CLSFY_RU_OPTION60_STR_LEN     64
#define MAPI_CCFG_QOS_CLSFY_RU_EXTRAMATCH_STR_LEN   128

#define MAPI_CCFG_QOS_CLSFY_SEQUENCE_STR_LEN        128

typedef struct mapi_ccfg_qos_clsfy_ru {                                 /* structure of classification rule parameters */
    char        pname[MAPI_CCFG_QOS_PROFILE_NAME_LEN];                  /* name of the profile */
    char        name[MAPI_CCFG_QOS_CLSFY_RU_NAME_LEN];                  /* name of the rule */
    int         id;                                                     /* rule index (0-based) */
    /* criterias */
    char        smac[MAPI_CCFG_QOS_CLSFY_RU_MAC_STR_LEN];               /* source mac */
    char        inif[MAPI_CCFG_QOS_CLSFY_RU_INIF_STR_LEN];              /* incoming interface, e.g., "ppp100,br-lan" (should not include bridged interfaces)*/
    char        inphyif[MAPI_CCFG_QOS_CLSFY_RU_INIF_STR_LEN];           /* incoming physical interface (for bridged interfaces)*/
    char        srcip[MAPI_CCFG_QOS_CLSFY_RU_IP_STR_LEN];
    char        dstip[MAPI_CCFG_QOS_CLSFY_RU_IP_STR_LEN];
    char        approto[MAPI_CCFG_QOS_CLSFY_RU_APPROTO_STR_LEN];        /* application/protocol */
    char        sport[MAPI_CCFG_QOS_CLSFY_RU_PORT_STR_LEN];
    char        dport[MAPI_CCFG_QOS_CLSFY_RU_PORT_STR_LEN];
    char        dscp[MAPI_CCFG_QOS_CLSFY_RU_DSCP_STR_LEN];
    char        _8021p[MAPI_CCFG_QOS_CLSFY_RU_8021P_STR_LEN];
    char        ethtype[MAPI_CCFG_QOS_CLSFY_RU_ETHTYPE_STR_LEN];
    char        pkylen[MAPI_CCFG_QOS_CLSFY_RU_PKTLEN_STR_LEN];
    char        domain[MAPI_CCFG_QOS_CLSFY_RU_DOMAIN_STR_LEN];
    char        dhcpopt60[MAPI_CCFG_QOS_CLSFY_RU_OPTION60_STR_LEN];
    char        extra_match[MAPI_CCFG_QOS_CLSFY_RU_EXTRAMATCH_STR_LEN]; /* extra criteria, represented by iptables matching string */
    /* actions */
    int         dscp_remark;
    int         _1p_remark;
    char        fwg_name[MAPI_CCFG_QOS_GROUP_NAME_LEN];
} mapi_ccfg_qos_clsfy_ru_t;

typedef struct mapi_ccfg_qos_clsfy_order {                              /* structure of scheduling parameters */
    char        pname[MAPI_CCFG_QOS_PROFILE_NAME_LEN];                  /* name of the profile */
    char        seq[MAPI_CCFG_QOS_CLSFY_SEQUENCE_STR_LEN];              /* sequence of classification rules */
} mapi_ccfg_qos_clsfy_order_t;

#define MAPI_QOS_CLSFY_MAX_RU_NUM       16

typedef struct mapi_ccfg_qos_clsfy {
    mapi_ccfg_qos_clsfy_order_t     ru_order;
    mapi_ccfg_qos_clsfy_ru_t        ru[MAPI_QOS_CLSFY_MAX_RU_NUM];
} mapi_ccfg_qos_clsfy_t;

typedef struct mapi_ccfg_qos_profile {
    char                            name[MAPI_CCFG_QOS_PROFILE_NAME_LEN];       /* name of the profile */
    mapi_ccfg_qos_qm_root_t         *qm;
    mapi_ccfg_qos_clsfy_t           *clsfy;
} mapi_ccfg_qos_profile_t;

extern mapi_ccfg_qos_profile_t      *target_qos_pf;


#ifdef __cplusplus
extern "C" {
#endif


extern mapi_ccfg_qos_profile_t *mapi_ccfg_qos_open_profile(int tid, char *pf_name, int factory_dft);
extern int mapi_ccfg_qos_close_profile(mapi_ccfg_qos_profile_t *qos_pf);
extern int mapi_ccfg_qos_set_qm_root(int tid, mapi_ccfg_qos_qm_root_t* pqm_data);
extern int mapi_ccfg_qos_set_qm_L1(int tid, mapi_ccfg_qos_qm_L1_t* pL1_data);
extern int mapi_ccfg_qos_set_qm_L2(int tid, mapi_ccfg_qos_qm_L2_t* pL2_data);
extern int mapi_ccfg_qos_set_qm_fwg(int tid, mapi_ccfg_qos_qm_fwg_t* pfwg_data);

extern int mapi_ccfg_qos_get_global(int tid, mapi_ccfg_qos_global_t *pglb, int factory_dft);
extern int mapi_ccfg_qos_set_global(int tid, mapi_ccfg_qos_global_t *pglb);

extern int mapi_ccfg_qos_get_clsfy_ru(int tid, mapi_ccfg_qos_clsfy_ru_t* pru, int factory_dft);
extern int mapi_ccfg_qos_set_clsfy_ru(int tid, mapi_ccfg_qos_clsfy_ru_t* pru);
extern int mapi_ccfg_qos_get_clsfy_order(int tid, mapi_ccfg_qos_clsfy_order_t* porder, int factory_dft);
extern int mapi_ccfg_qos_set_clsfy_order(int tid, mapi_ccfg_qos_clsfy_order_t* porder);


#ifdef __cplusplus
}
#endif


#endif // _mid_mapi_ccfg_qos_h_
