/*
 * Arcadyan middle layer APIs for VoIP configuration
 *
 * Copyright 2011, Arcadyan Corporation
 * All Rights Reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libArcComApi.h>
#include "mid_mapi_ccfgsal_voip.h"


static mapi_ccfg_voip_codec_info_t	_mapi_ccfg_voip_codec_info[ MAPI_CCFG_VOIP_CODEC_MAX ] = { MAPI_CCFG_VOIP_CODEC_INFO_ALL };

//const unsigned char MAPI_CCFG_VOIP_DFT_SUPPORTED_CODEC_LIST[MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM]={
//	MAPI_CCFG_VOIP_CODEC_TYPE_G711_64_U,
//	MAPI_CCFG_VOIP_CODEC_TYPE_G711_64_A,
//	MAPI_CCFG_VOIP_CODEC_TYPE_G726_40_A,
//	MAPI_CCFG_VOIP_CODEC_TYPE_G726_32_A,
//	MAPI_CCFG_VOIP_CODEC_TYPE_G726_24_A,
//	MAPI_CCFG_VOIP_CODEC_TYPE_G726_16_A,
//	MAPI_CCFG_VOIP_CODEC_TYPE_G729,
//	MAPI_CCFG_VOIP_CODEC_TYPE_G722,
//	MAPI_CCFG_VOIP_CODEC_TYPE_G723,
//	MAPI_CCFG_VOIP_CODEC_TYPE_NONE,
//    };

/*
 * -- Helper functions --
 */

/*
 * -- Public functions --
 */
 
// Please refer to mid_mapi_ccfg.h for the usage of the input "mapi_ccfg_tuple_t* ptuple"
// in the below functions


/************************************************************/
/******************** CORE FUNCTIONS ************************/
/************************************************************/


/*******************************************************************************
 * Description
 *		Get VoIP global configuration
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_voip_glb:	    pointer to the buffer of data structure mapi_ccfg_voip_global_t.
 *	                    Required input fields: none
 *      factory_dft:    use 1 to extract the factory default configuration
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* 
 *
 * See Also
 *		mapi_ccfg_set_voip_global
 *
 ******************************************************************************/
int mapi_ccfg_get_voip_global(int tid, mapi_ccfg_voip_global_t* p_voip_glb, int factory_dft)
{
    int                 new_transc=0;
    char                sect_name[]="voip";
    int                 cnt=0;

    if(p_voip_glb==NULL){
        MID_TRACE("[%s] NULL p_voip_glb\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }
    
    //max_sip_line_num
    p_voip_glb->max_sip_line_num=MAPI_CCFG_VOIP_MAX_SIP_ACCOUNT_NUM;
    
    //max_isdn_msn_num
    p_voip_glb->max_isdn_msn_num=MAPI_CCFG_VOIP_MAX_ISDN_NUM;
    
    //max_dial_rule_num
    p_voip_glb->max_dial_rule_num=MAPI_CCFG_VOIP_MAX_DIAL_RULE_NUM;

    //phone_num
    p_voip_glb->phone_num=MAPI_CCFG_VOIP_PHONE_NUM;

    //arcor_customer
    p_voip_glb->arcor_customer=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "arcor_customer", 0));

    //t38_eb
    p_voip_glb->t38_eb=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "t38Cap_bFax", 0));

    //pmtu_adjust_eb
    p_voip_glb->pmtu_adjust_eb=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "pmtu_adjust_eb", 0));

    //bw_mn_eb
    p_voip_glb->bw_mn_eb=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "Bandwidth_Management", 0));

    //rtp_port_min
    p_voip_glb->rtp_port_min=(unsigned short)(mapi_ccfg_get_long(tid, sect_name, "rtp_minPort", MAPI_CCFG_VOIP_DFT_RTP_MIN_PORT));

    //rtp_port_max
    p_voip_glb->rtp_port_max=(unsigned short)(mapi_ccfg_get_long(tid, sect_name, "rtp_maxPort", MAPI_CCFG_VOIP_DFT_RTP_MAX_PORT));

    //hook_flash_min
    p_voip_glb->hook_flash_min=(unsigned short)(mapi_ccfg_get_long(tid, sect_name, "timer_hookFlashMin", MAPI_CCFG_VOIP_DFT_HKFLUSH_MIN));

    //hook_flash_max
    p_voip_glb->hook_flash_max=(unsigned short)(mapi_ccfg_get_long(tid, sect_name, "timer_hookFlashMax", MAPI_CCFG_VOIP_DFT_HKFLUSH_MAX));

    //pmtu_adjust_sz
    p_voip_glb->pmtu_adjust_sz=(unsigned short)(mapi_ccfg_get_long(tid, sect_name, "pmtu_adjust_sz", MAPI_CCFG_VOIP_DFT_PMTU_ADJ_SZ));

    //rtp_frame_ms
    p_voip_glb->rtp_frame_ms=(unsigned short)(mapi_ccfg_get_long(tid, sect_name, "rtp_pTime", MAPI_CCFG_VOIP_DFT_RTP_FRAME_TIME));

    // fxo_control
    p_voip_glb->fxo_control = (unsigned char)(mapi_ccfg_get_long(tid, sect_name, "FXOcontrol", 0));
    
    //sip_acc_max
    p_voip_glb->sip_acc_max=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "sip_acc_max", MAPI_CCFG_VOIP_DFT_SIP_ACC_MAX));

    //sip_acc_num
	for ( cnt = 0, p_voip_glb->sip_acc_num = 0; cnt < p_voip_glb->sip_acc_max; cnt++ ) {
		if ( mapi_ccfg_cmp_str_dft( tid, sect_name, "userId", "", "" ) != 0)
			p_voip_glb->sip_acc_num++;
	}

    //line_max
    p_voip_glb->line_max=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "line_max", MAPI_CCFG_VOIP_DFT_LINE_MAX));

    //line_max
    p_voip_glb->line_max=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "line_max", MAPI_CCFG_VOIP_DFT_LINE_MAX));

    //sess_line_max
    p_voip_glb->sess_line_max=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "sess_line_max", MAPI_CCFG_VOIP_DFT_SESS_LINE_MAX));

    //sess_total_max
    p_voip_glb->sess_total_max=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "sess_total_max", MAPI_CCFG_VOIP_DFT_SESS_TOTAL_MAX));

    //signal_proto
    strcpy( p_voip_glb->signal_proto, MAPI_CCFG_VOIP_DFT_SIGNAL_PROTO );

    //regions
    strcpy( p_voip_glb->regions, MAPI_CCFG_VOIP_DFT_REGIONS );

    //capability_list_1
	p_voip_glb->capability_list_1 = (unsigned short)MAPI_CCFG_VOIP_DFT_CAP_LIST_1;

    //sip_role
	p_voip_glb->sip_role = (unsigned char)MAPI_CCFG_VOIP_DFT_SIP_ROLE;

    //sip_ext
	p_voip_glb->sip_ext = (unsigned char)MAPI_CCFG_VOIP_DFT_SIP_EXT;

    //all_codec_list
    //memcpy(p_voip_glb->all_codec_list, MAPI_CCFG_VOIP_DFT_SUPPORTED_CODEC_LIST, sizeof(MAPI_CCFG_VOIP_DFT_SUPPORTED_CODEC_LIST));

    //uri_schemes
    strcpy( p_voip_glb->uri_schemes, MAPI_CCFG_VOIP_DFT_URI_SCHEMES );

    //rtp_dscp_mark
	p_voip_glb->rtp_dscp_mark = MAPI_CCFG_VOIP_DFT_RTP_DSCP_MARK;

    //rtp_evt_payload_type
	p_voip_glb->rtp_evt_payload_type = MAPI_CCFG_VOIP_DFT_RTP_EVT_PAYLOAD_TYPE;

    //fax_t38_rate
	p_voip_glb->fax_t38_rate = (unsigned char)MAPI_CCFG_VOIP_DFT_FAX_T38_RATE;

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
    }
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Set VoIP global configuration
 *
 * Parameters
 *		tid:        transaction ID returned by calling mapi_start_transc()
 *                  use 0 if no tid is provided (no transaction was opened before)
 *		p_voip_glb:	pointer to the buffer of data structure mapi_ccfg_voip_global_t.
 *	                Required input fields: none
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* related sections: voip
 *
 * See Also
 *		mapi_ccfg_get_voip_global
 *
 ******************************************************************************/
int mapi_ccfg_set_voip_global(int tid, mapi_ccfg_voip_global_t* p_voip_glb)
{
    int                 new_transc=0;
    char                sect_name[]="voip";
    
    if(p_voip_glb==NULL){
        MID_TRACE("[%s] NULL p_voip_glb\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    //max_sip_line_num
    //read-only
    
    //max_isdn_msn_num
    //read-only
    
    //max_dial_rule_num
    //read-only

    //phone_num
    //read-only

    //speed_dial_eb
    mapi_ccfg_set_long(tid, sect_name, "arcor_customer", (unsigned long)(p_voip_glb->arcor_customer));

    //t38_eb
    mapi_ccfg_set_long(tid, sect_name, "t38Cap_bFax", (unsigned long)(p_voip_glb->t38_eb));

    //pmtu_adjust_eb
    mapi_ccfg_set_long(tid, sect_name, "pmtu_adjust_eb", (unsigned long)(p_voip_glb->pmtu_adjust_eb));

    //bw_mn_eb
    mapi_ccfg_set_long(tid, sect_name, "Bandwidth_Management", (unsigned long)(p_voip_glb->bw_mn_eb));

    //rtp_port_min
    mapi_ccfg_set_long(tid, sect_name, "rtp_minPort", (unsigned long)(p_voip_glb->rtp_port_min));

    //rtp_port_max
    mapi_ccfg_set_long(tid, sect_name, "rtp_maxPort", (unsigned long)(p_voip_glb->rtp_port_max));

    //hook_flash_min
    mapi_ccfg_set_long(tid, sect_name, "timer_hookFlashMin", (unsigned long)(p_voip_glb->hook_flash_min));

    //hook_flash_max
    mapi_ccfg_set_long(tid, sect_name, "timer_hookFlashMax", (unsigned long)(p_voip_glb->hook_flash_max));

    //pmtu_adjust_sz
    mapi_ccfg_set_long(tid, sect_name, "pmtu_adjust_sz", (unsigned long)(p_voip_glb->pmtu_adjust_sz));

    //rtp_frame_ms
    mapi_ccfg_set_long(tid, sect_name, "rtp_pTime", (unsigned long)(p_voip_glb->rtp_frame_ms));

    // fxo_control
    mapi_ccfg_set_long(tid, sect_name, "FXOcontrol", (unsigned long)(p_voip_glb->fxo_control));

    //sip_acc_num
    //sip_acc_max
    //line_max
    //sess_line_max
    //sess_total_max
    //signal_proto
    //regions
    //capability_list_1
    //sip_role
    //sip_ext
    //uri_schemes
    //rtp_dscp_mark
    //rtp_evt_payload_type
    //fax_t38_rate
    // ==> read-only

    //all_codec_list
    //should be read-only
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Get configuration of a VoIP SIP account
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_voip_sip:	    pointer to the buffer of data structure mapi_ccfg_voip_sip_line_t.
 *	                    Required input fields:
 *                          1. p_voip_sip->line_idx
 *      factory_dft:    use 1 to extract the factory default configuration
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* 
 *
 * See Also
 *		mapi_ccfg_set_voip_sip
 *
 ******************************************************************************/
int mapi_ccfg_get_voip_sip(int tid, mapi_ccfg_voip_sip_line_t* p_voip_sip, int factory_dft)
{
    int                 i, new_transc=0;
    char                sect_name[16], para_name[16];
    int                 selected_codec_cnt;
    unsigned long       codec_s;
    unsigned long       codec_list;

    if(p_voip_sip==NULL){
        MID_TRACE("[%s] NULL p_voip_sip\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }
    
    sprintf(sect_name, "sip_acc_%d", p_voip_sip->line_idx+1);

    //enable
    p_voip_sip->enable=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "lineEnable", 0));
    
    //display_name
    mapi_ccfg_get_str(tid, sect_name, "displayName", p_voip_sip->display_name, MAPI_CCFG_VOIP_DISPLAY_NAME_MAX_LEN, NULL);
    
    //user_id
    mapi_ccfg_get_str(tid, sect_name, "userId", p_voip_sip->user_id, MAPI_CCFG_VOIP_USER_ID_MAX_LEN, NULL);
    
    //userId_area
    mapi_ccfg_get_str(tid, sect_name, "userId_area", p_voip_sip->userid_area, MAPI_CCFG_VOIP_USER_ID_MAX_LEN, NULL);
    
    //userId_local
    mapi_ccfg_get_str(tid, sect_name, "userId_local", p_voip_sip->userid_local, MAPI_CCFG_VOIP_USER_ID_MAX_LEN, NULL);

    //area_code_len
    //p_voip_sip->area_code_len=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "area_code_len", 0));

    //passwd
    mapi_ccfg_get_str(tid, sect_name, "password", p_voip_sip->passwd, MAPI_CCFG_VOIP_PASSWD_MAX_LEN, NULL);

    //realm
    mapi_ccfg_get_str(tid, sect_name, "realm", p_voip_sip->realm, MAPI_CCFG_VOIP_REALM_MAX_LEN, NULL);

    //use_auth_id
    p_voip_sip->use_auth_id=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "useAuthId", 0));

    //auth_id
    mapi_ccfg_get_str(tid, sect_name, "authId", p_voip_sip->auth_id, MAPI_CCFG_VOIP_AUTH_ID_MAX_LEN, NULL);

    //listen_port
    p_voip_sip->listen_port=(unsigned short)(mapi_ccfg_get_long(tid, sect_name, "sipPort", MAPI_CCFG_VOIP_DFT_SIP_PORT));

    //listen_wan
    p_voip_sip->listen_wan=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "ListenVc", 0));

    //sipdomain
    mapi_ccfg_get_str(tid, sect_name, "sipdomain", p_voip_sip->sipdomain, MAPI_CCFG_VOIP_FQDN_MAX_LEN, NULL);
    
    //registrar
    mapi_ccfg_get_str(tid, sect_name, "registrar", p_voip_sip->registrar, MAPI_CCFG_VOIP_FQDN_MAX_LEN, NULL);

    //registrar_port
    p_voip_sip->registrar_port=(unsigned short)(mapi_ccfg_get_long(tid, sect_name, "registrarPort", MAPI_CCFG_VOIP_DFT_SIP_PORT));

    //registrar_expire
    p_voip_sip->registrar_expire=(unsigned short)(mapi_ccfg_get_long(tid, sect_name, "registerExpire", MAPI_CCFG_VOIP_DFT_REGISTRAR_EXPIRE));

    //proxy
    mapi_ccfg_get_str(tid, sect_name, "proxy", p_voip_sip->proxy, MAPI_CCFG_VOIP_FQDN_MAX_LEN, NULL);

    //proxy_port
    p_voip_sip->proxy_port=(unsigned short)(mapi_ccfg_get_long(tid, sect_name, "proxyPort", MAPI_CCFG_VOIP_DFT_SIP_PORT));

    //use_ob_proxy
    p_voip_sip->use_ob_proxy=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "useOutboundProxy", 0));

    //ob_proxy
    mapi_ccfg_get_str(tid, sect_name, "outboundProxy", p_voip_sip->ob_proxy, MAPI_CCFG_VOIP_FQDN_MAX_LEN, NULL);

    //ob_proxy_port
    p_voip_sip->ob_proxy_port=(unsigned short)(mapi_ccfg_get_long(tid, sect_name, "obproxyPort", MAPI_CCFG_VOIP_DFT_SIP_PORT));

    //dtmf_tx_method
    p_voip_sip->dtmf_tx_method=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "dtmfTxMethod", 0));

    //fax_passthru_codec
    p_voip_sip->fax_passthru_codec=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "faxPassthruCodec", MAPI_CCFG_VOIP_CODEC_TYPE_G711_64_A));

    //use_dnssrv
    p_voip_sip->use_dnssrv=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "useDNSSRV", 1));

    //use_fetch_binding
    p_voip_sip->use_fetch_binding=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "enableRegisterQuery", 1));

    //codec_config
    p_voip_sip->codec_config=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "codec_config", 1));    

    //uri
	if (p_voip_sip->user_id[0] != '\0' && p_voip_sip->registrar[0] != '\0')
		sprintf( p_voip_sip->uri, "sip:%s@%s", p_voip_sip->user_id, p_voip_sip->registrar );
	else
		p_voip_sip->uri[0] = '\0';

	//all_codec_list and selected_codec_list
	memset(p_voip_sip->selected_codec_list, MAPI_CCFG_VOIP_CODEC_TYPE_NONE, sizeof(p_voip_sip->selected_codec_list));
	for ( i=0, codec_list=0x0, selected_codec_cnt=0; i<MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM; i++)
	{
		sprintf(para_name, "codec_s%d", i+1);
		codec_s = mapi_ccfg_get_long(tid, sect_name, para_name, MAPI_CCFG_VOIP_CODEC_TYPE_NONE);
		if ( MAPI_CCFG_VOIP_CODEC_IS_VALID( codec_s ) && !MID_BIT_CHECK( codec_list, codec_s) )
		{
			p_voip_sip->selected_codec_list[selected_codec_cnt++] = (unsigned char)codec_s;
			MID_BIT_SET( codec_list, codec_s );
		}
	}

	//region
	strcpy( p_voip_sip->region, MAPI_CCFG_VOIP_DFT_LINE_REGION );

	//stun_eb
	p_voip_sip->stun_eb = MAPI_CCFG_VOIP_DFT_STUN_ENABLE;

	//stun_server
	strcpy( p_voip_sip->stun_server, MAPI_CCFG_VOIP_DFT_STUN_SERVER );

	//pstn_fail_over
	p_voip_sip->pstn_fail_over = MAPI_CCFG_VOIP_DFT_PSTN_FAIL_OVER;

	//fax_pass_thru
    p_voip_sip->fax_pass_thru = (unsigned char)MAPI_CCFG_VOIP_DFT_SIP_LINE_FAX_PASS_THRU;

	//modem_pass_thru
    p_voip_sip->modem_pass_thru = (unsigned char)MAPI_CCFG_VOIP_DFT_SIP_LINE_MODEM_PASS_THRU;

	//proxy_transport
    p_voip_sip->proxy_transport = (unsigned char)MAPI_CCFG_VOIP_DFT_SIP_PROXY_TRANSPORT;

	//registrar_transport
    p_voip_sip->registrar_transport = (unsigned char)MAPI_CCFG_VOIP_DFT_SIP_REGISTRAR_TRANSPORT;

	//user_agent_transport
    p_voip_sip->user_agent_transport = (unsigned char)MAPI_CCFG_VOIP_DFT_SIP_USER_AGENT_TRANSPORT;

	//organization
	strcpy( p_voip_sip->organization, MAPI_CCFG_VOIP_DFT_SIP_ORG );

	//registrar_period
	p_voip_sip->registrar_period = p_voip_sip->registrar_expire;

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
    }
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Set configuration of a VoIP SIP account
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_voip_sip:	    pointer to the buffer of data structure mapi_ccfg_voip_sip_line_t.
 *	                    Required input fields:
 *                          1. p_voip_sip->line_idx, 0-based
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* related sections: sip_acc_N (where N is SIP account index+1, p_voip_sip->line_idx+1)
 *
 * See Also
 *		mapi_ccfg_get_voip_sip
 *
 ******************************************************************************/
int mapi_ccfg_set_voip_sip(int tid, mapi_ccfg_voip_sip_line_t* p_voip_sip)
{
    int                 i, new_transc=0;
    char                sect_name[16], para_name[16];
	int                 selected_codec_cnt;

    if(p_voip_sip==NULL){
        MID_TRACE("[%s] NULL p_voip_sip\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    sprintf(sect_name, "sip_acc_%d", p_voip_sip->line_idx+1);

    //enable
    mapi_ccfg_set_long(tid, sect_name, "lineEnable", (unsigned long)(p_voip_sip->enable));
    
    //display_name
    mapi_ccfg_set_str(tid, sect_name, "displayName", p_voip_sip->display_name);
    
    //user_id
    mapi_ccfg_set_str(tid, sect_name, "userId", p_voip_sip->user_id);
    
    //userId_area
    mapi_ccfg_set_str(tid, sect_name, "userId_area", p_voip_sip->userid_area);
    
    //userId_local
    mapi_ccfg_set_str(tid, sect_name, "userId_local", p_voip_sip->userid_local);

    //area_code_len
    //mapi_ccfg_set_long(tid, sect_name, "area_code_len", (unsigned long)(p_voip_sip->area_code_len));

    //passwd
    mapi_ccfg_set_str(tid, sect_name, "password", p_voip_sip->passwd);

    //realm
    mapi_ccfg_set_str(tid, sect_name, "realm", p_voip_sip->realm);

    //use_auth_id
    mapi_ccfg_set_long(tid, sect_name, "useAuthId", (unsigned long)(p_voip_sip->use_auth_id));

    //auth_id
    mapi_ccfg_set_str(tid, sect_name, "authId", p_voip_sip->auth_id);

    //listen_port
    mapi_ccfg_set_long(tid, sect_name, "sipPort", (unsigned long)(p_voip_sip->listen_port));

    //listen_wan
    mapi_ccfg_set_long(tid, sect_name, "ListenVc", (unsigned long)(p_voip_sip->listen_wan));

    // sipdomain
    mapi_ccfg_set_str(tid, sect_name, "sipdomain", p_voip_sip->sipdomain);
    
    //registrar
    mapi_ccfg_set_str(tid, sect_name, "registrar", p_voip_sip->registrar);

    //registrar_port
    mapi_ccfg_set_long(tid, sect_name, "registrarPort", (unsigned long)(p_voip_sip->registrar_port));

    //registrar_expire
    mapi_ccfg_set_long(tid, sect_name, "registerExpire", (unsigned long)(p_voip_sip->registrar_expire));

    //proxy
    mapi_ccfg_set_str(tid, sect_name, "proxy", p_voip_sip->proxy);

    //proxy_port
    mapi_ccfg_set_long(tid, sect_name, "proxyPort", (unsigned long)(p_voip_sip->proxy_port));

    //use_ob_proxy
    mapi_ccfg_set_long(tid, sect_name, "useOutboundProxy", (unsigned long)(p_voip_sip->use_ob_proxy));

    //ob_proxy
    mapi_ccfg_set_str(tid, sect_name, "outboundProxy", p_voip_sip->ob_proxy);

    //ob_proxy_port
    mapi_ccfg_set_long(tid, sect_name, "obproxyPort", (unsigned long)(p_voip_sip->ob_proxy_port));

    //dtmf_tx_method
    mapi_ccfg_set_long(tid, sect_name, "dtmfTxMethod", (unsigned long)(p_voip_sip->dtmf_tx_method));

    //fax_passthru_codec
    mapi_ccfg_set_long(tid, sect_name, "faxPassthruCodec", (unsigned long)(p_voip_sip->fax_passthru_codec));

    //use_dnssrv
    mapi_ccfg_set_long(tid, sect_name, "useDNSSRV", (unsigned long)(p_voip_sip->use_dnssrv));

    //use_fetch_binding
    mapi_ccfg_set_long(tid, sect_name, "enableRegisterQuery", (unsigned long)(p_voip_sip->use_fetch_binding));

    //codec_config
    mapi_ccfg_set_long(tid, sect_name, "codec_config", (unsigned long)(p_voip_sip->codec_config));
    

	for ( i=0, selected_codec_cnt=0; i < MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM; i++)
	{
		if ( MAPI_CCFG_VOIP_CODEC_IS_VALID( p_voip_sip->selected_codec_list[i] ) )
		{
			sprintf( para_name, "codec_s%d", ++selected_codec_cnt );
			mapi_ccfg_set_long( tid, sect_name, para_name, (unsigned long)p_voip_sip->selected_codec_list[i] );
		}
	}
	for ( ; selected_codec_cnt < MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM; selected_codec_cnt++)
	{
		sprintf( para_name, "codec_s%d", selected_codec_cnt+1 );
		mapi_ccfg_set_long( tid, sect_name, para_name, MAPI_CCFG_VOIP_CODEC_TYPE_NONE );
	}

    //some fields maybe correlated, check them here
    #ifdef MAPI_CCFG_VOICE_PROXY_REGISTRAR_SIPDOMAIN_CORRELATED
    //registrar
    mapi_ccfg_set_str(tid, sect_name, "registrar", p_voip_sip->proxy);
    //sipdomain
    mapi_ccfg_set_str(tid, sect_name, "sipdomain", p_voip_sip->proxy);
    #endif
    
    #ifdef MAPI_CCFG_VOICE_PROXYPORT_REGISTRARPORT_CORRELATED
    mapi_ccfg_set_long(tid, sect_name, "registrarPort", (unsigned long)(p_voip_sip->proxy_port));
    #endif
    
    //region
    //stun_eb
	//stun_server
    //pstn_fail_over
	//fax_pass_thru
	//modem_pass_thru
	//proxy_transport
	//registrar_transport
	//user_agent_transport
	//registrar_period
    // ==> read-only

    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Get the list of unselected code of a VoIP SIP account
 *
 * Parameters
 *		p_voip_sip:	    pointer to the buffer of data structure mapi_ccfg_voip_sip_line_t
 *	                    Required input fields:
 *                          1. the input *p_voip_sip is usually obtained by mapi_ccfg_get_voip_sip()
 *                             should contain valid content
 *      unselected_codec: point to the buffer to store returned unselected codecs
 *      unselected_codec_sz: size of the buffer *unselected_codec
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* 
 *
 * See Also
 *		mapi_ccfg_get_voip_sip, mapi_ccfg_select_voip_sip_codec, mapi_ccfg_unselect_voip_sip_codec
 *      mapi_ccfg_update_sip_selected_codec
 *
 ******************************************************************************/
int mapi_ccfg_get_voip_sip_unselect_codec(mapi_ccfg_voip_sip_line_t* p_voip_sip, unsigned char *unselected_codec, int unselected_codec_sz)
{
	int				cnt, cnt2;
	int				available_cnt = MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM;
	unsigned long	selected_list = 0x0;

	if(p_voip_sip==NULL || unselected_codec==NULL)
	{
		MID_TRACE("[%s] NULL input param\n", __FUNCTION__);
		return MID_FAIL;
	}

	memset( unselected_codec, MAPI_CCFG_VOIP_CODEC_TYPE_NONE, unselected_codec_sz );

	for ( cnt=0; cnt < MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM ; cnt++ )
	{
		if ( MAPI_CCFG_VOIP_CODEC_IS_VALID( p_voip_sip->selected_codec_list[cnt] ) )
		{
			MID_BIT_SET( selected_list, p_voip_sip->selected_codec_list[cnt] );
			available_cnt--;
		}
	}

	for ( cnt=1, cnt2=0; cnt <= MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM ; cnt++ )
	{
		if ( !MID_BIT_CHECK( selected_list, cnt ) )
		{
			unselected_codec[ cnt2++ ] = cnt;
			if ( cnt2 >= available_cnt || cnt2 >= unselected_codec_sz )
				break;
		}
	}
    
    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		to select a codec from the codec list of a VoIP SIP account
 *
 * Parameters
 *		p_voip_sip:	    pointer to the buffer of data structure mapi_ccfg_voip_sip_line_t
 *	                    Required input fields:
 *                          1. the input *p_voip_sip is usually obtained by mapi_ccfg_get_voip_sip()
 *                             should contain valid content
 *      codec:          to be selected codec
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or p_voip_sip is NULL or codec is wrong.
 *
 * Note
 *      * the new codec is added at the end of selected_codec_list[]
 *		* this function only help to update all_codec_list[] and selected_codec_list[] in *p_voip_sip,
 *        user should still use mapi_ccfg_set_voip_sip() to update the change to the system configuration
 *
 * See Also
 *		mapi_ccfg_set_voip_sip, mapi_ccfg_get_voip_sip_unselect_codec, mapi_ccfg_unselect_voip_sip_codec,
 *      mapi_ccfg_update_sip_selected_codec
 *
 ******************************************************************************/
int mapi_ccfg_select_voip_sip_codec(mapi_ccfg_voip_sip_line_t* p_voip_sip, unsigned char codec)
{
	int		cnt;

	if ( p_voip_sip == NULL || !MAPI_CCFG_VOIP_CODEC_IS_VALID( codec ) )
	{
		MID_TRACE("[%s] NULL input param\n", __FUNCTION__);
		return MID_FAIL;
	}

	for ( cnt=0; cnt < MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM; cnt++ )
	{
		if ( p_voip_sip->selected_codec_list[cnt] == codec )
			return MID_SUCCESS;
    }

	for ( cnt=0; cnt < MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM; cnt++ )
	{
		if ( p_voip_sip->selected_codec_list[cnt] == MAPI_CCFG_VOIP_CODEC_TYPE_NONE )
		{
			p_voip_sip->selected_codec_list[cnt] = codec;
			return MID_SUCCESS;
		}
	}

	return MID_FAIL;
}

/*******************************************************************************
 * Description
 *		to unselect a codec from the codec list of a VoIP SIP account
 *
 * Parameters
 *		p_voip_sip:	    pointer to the buffer of data structure mapi_ccfg_voip_sip_line_t
 *	                    Required input fields:
 *                          1. the input *p_voip_sip is usually obtained by mapi_ccfg_get_voip_sip()
 *                             should contain valid content
 *      codec:          to be unselected codec
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or p_voip_sip is NULL or codec is wrong or codec is not selected before.
 *
 * Note
 *		* this function only help to update all_codec_list[] and selected_codec_list[] in *p_voip_sip,
 *        user should still use mapi_ccfg_set_voip_sip() to update the change to the system configuration
 *
 * See Also
 *		mapi_ccfg_set_voip_sip, mapi_ccfg_get_voip_sip_unselect_codec, mapi_ccfg_select_voip_sip_codec
 *      mapi_ccfg_update_sip_selected_codec
 *
 ******************************************************************************/
int mapi_ccfg_unselect_voip_sip_codec(mapi_ccfg_voip_sip_line_t* p_voip_sip, unsigned char codec)
{
	int		cnt;

	if ( p_voip_sip == NULL || !MAPI_CCFG_VOIP_CODEC_IS_VALID( codec ) )
	{
		MID_TRACE("[%s] NULL input param\n", __FUNCTION__);
		return MID_FAIL;
	}

	for ( cnt=0; cnt < MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM; cnt++ )
	{
		if ( p_voip_sip->selected_codec_list[cnt] == codec )
		{
			p_voip_sip->selected_codec_list[cnt] = MAPI_CCFG_VOIP_CODEC_TYPE_NONE;
			return MID_SUCCESS;
		}
    }

	return MID_FAIL;
}

/*******************************************************************************
 * Description
 *		to update a list of new selected codec of a VoIP SIP account
 *
 * Parameters
 *		p_voip_sip:	    pointer to the buffer of data structure mapi_ccfg_voip_sip_line_t
 *	                    Required input fields:
 *                          1. the input *p_voip_sip is usually obtained by mapi_ccfg_get_voip_sip()
 *                             should contain valid content
 *      new_selected_codec: the new selected codec list
 *      selected_codec_sz: size of the buffer *new_selected_codec
 *      
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or p_voip_sip is NULL or new_selected_codec is NULL.
 *
 * Note
 *		* this function only help to update all_codec_list[] and selected_codec_list[] in *p_voip_sip,
 *        user should still use mapi_ccfg_set_voip_sip() to update the change to the system configuration
 *
 * See Also
 *		mapi_ccfg_set_voip_sip, mapi_ccfg_get_voip_sip_unselect_codec, mapi_ccfg_unselect_voip_sip_codec,
 *      mapi_ccfg_select_voip_sip_codec
 *
 ******************************************************************************/
int mapi_ccfg_update_sip_selected_codec(mapi_ccfg_voip_sip_line_t* p_voip_sip, char *new_selected_codec, int selected_codec_sz)
{
	int				cnt;
	int				selected_cnt = 0;
	unsigned long	codec_list = 0x0;

	if ( p_voip_sip == NULL || new_selected_codec == NULL )
	{
		MID_TRACE("[%s] NULL input param\n", __FUNCTION__);
		return MID_FAIL;
	}

	for ( cnt=0; cnt < selected_codec_sz; cnt++ )
	{
		if ( !MAPI_CCFG_VOIP_CODEC_IS_VALID( new_selected_codec[cnt] ) )
			continue;
		if ( MID_BIT_CHECK( codec_list, new_selected_codec[cnt] ) )
			continue;
		p_voip_sip->selected_codec_list[ selected_cnt++ ] = new_selected_codec[cnt];
	}

	for ( ; selected_cnt < MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM; selected_cnt++ )
	{
		p_voip_sip->selected_codec_list[ selected_cnt ] = MAPI_CCFG_VOIP_CODEC_TYPE_NONE;
	}

	return MID_SUCCESS;
}


/*******************************************************************************
 * Description
 *		Get one codec data
 *
 * Parameters
 *		codec:		identifier of codec, enumerated in MAPI_CCFG_VOIP_CODEC_TYPE_T
 *		p_codec:	pointer to the buffer of data structure mapi_ccfg_voip_phone_t.
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		codec is wrong or p_codec is NULL.
 *
 * See Also
 *		mapi_ccfg_get_all_voip_codec_info
 *
 ******************************************************************************/
int mapi_ccfg_get_one_voip_codec_info( unsigned char codec, mapi_ccfg_voip_codec_info_t* p_codec )
{
	unsigned char	cnt;

	if ( p_codec == NULL )
		return MID_FAIL;

	for ( cnt=0; cnt < MAPI_CCFG_VOIP_CODEC_MAX; cnt++ )
	{
		if (_mapi_ccfg_voip_codec_info[cnt].codec == codec)
		{
			memcpy( p_codec, &_mapi_ccfg_voip_codec_info[cnt], sizeof(*p_codec) );
			return MID_SUCCESS;
		}
	}

	return MID_FAIL;
}

/*******************************************************************************
 * Description
 *		Get one codec data
 *
 * Parameters
 *		idx:		index of codec, 1 ~ MAPI_CCFG_VOIP_CODEC_MAX
 *		p_codec:	pointer to the buffer of data structure mapi_ccfg_voip_phone_t.
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		codec is wrong or p_codec is NULL.
 *
 * See Also
 *		mapi_ccfg_get_all_voip_codec_info
 *
 ******************************************************************************/
int mapi_ccfg_get_voip_codec_info_by_idx( unsigned char idx, mapi_ccfg_voip_codec_info_t* p_codec )
{
	if ( idx <= 0 || idx > MAPI_CCFG_VOIP_CODEC_MAX || p_codec == NULL )
		return MID_FAIL;

	memcpy( p_codec, &_mapi_ccfg_voip_codec_info[idx-1], sizeof(*p_codec) );

	return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Get all codec data
 *
 * Parameters
 *		p_codec:	pointer to the buffer of an array of data structure mapi_ccfg_voip_phone_t.
 *					p_codec MUST be a buffer whose size is not less than
 *					MAPI_CCFG_VOIP_CODEC_MAX*sizeof(mapi_ccfg_voip_codec_info_t).
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		p_codec is NULL.
 *
 * See Also
 *		mapi_ccfg_get_one_voip_codec_info
 *
 ******************************************************************************/
int mapi_ccfg_get_all_voip_codec_info( mapi_ccfg_voip_codec_info_t* p_codec )
{
	if (p_codec == NULL )
		return MID_FAIL;

	memcpy( p_codec, _mapi_ccfg_voip_codec_info, sizeof(_mapi_ccfg_voip_codec_info) );

	return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Get configuration of a phone
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_voip_phone:	pointer to the buffer of data structure mapi_ccfg_voip_phone_t.
 *	                    Required input fields:
 *                          1. p_voip_phone->phone_idx
 *      factory_dft:    use 1 to extract the factory default configuration
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* 
 *
 * See Also
 *		mapi_ccfg_set_voip_sip
 *
 ******************************************************************************/
int mapi_ccfg_get_voip_phone(int tid, mapi_ccfg_voip_phone_t* p_voip_phone, int factory_dft)
{
    int                 i, new_transc=0;
    char                sect_name[16], para_name[24];
    unsigned long       val;

    if(p_voip_phone==NULL){
        MID_TRACE("[%s] NULL p_voip_phone\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }
    
    sprintf(sect_name, "phone_%d", p_voip_phone->phone_idx+1);

    //answer_all_eb
    p_voip_phone->answer_all_eb=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "answer_all", 1));
    
    //ext_in
    for(i=0; i<MAPI_CCFG_VOIP_MAX_TOTAL_ACCOUNT_NUM; i++){
        sprintf(para_name, "account_in_idx_%d", i+1);
        val=mapi_ccfg_get_long(tid, sect_name, para_name, MAPI_CCFG_VOIP_CONNECT_NONE);
        if(val<MAPI_CCFG_VOIP_CONNECT_ISDN_IDX_BASE){
            p_voip_phone->ext_in[i].type=MAPI_CCFG_VOIP_CONNECT_SIP_ACCOUNT;
            p_voip_phone->ext_in[i].logic_idx=(unsigned char)val;
        }
        else if(val<MAPI_CCFG_VOIP_CONNECT_PSTN_IDX_BASE){
            p_voip_phone->ext_in[i].type=MAPI_CCFG_VOIP_CONNECT_ISDN_MSN;
            p_voip_phone->ext_in[i].logic_idx=(unsigned char)(val-MAPI_CCFG_VOIP_CONNECT_ISDN_IDX_BASE);
        }
        else if(val==MAPI_CCFG_VOIP_CONNECT_PSTN_IDX_BASE){
            p_voip_phone->ext_in[i].type=MAPI_CCFG_VOIP_CONNECT_PSTN_NUM;
            p_voip_phone->ext_in[i].logic_idx=0;
        }
#ifdef MAPI_CCFG_VOICE_UMTS_SUPPORT
        else if(val==MAPI_CCFG_VOIP_CONNECT_MOBILE_IDX_BASE){
            p_voip_phone->ext_in[i].type=MAPI_CCFG_VOIP_CONNECT_MOBILE_NUM;
            p_voip_phone->ext_in[i].logic_idx=0;
        }
#endif
        else{
            p_voip_phone->ext_in[i].type=(unsigned char)val;
            p_voip_phone->ext_in[i].logic_idx=0;
        }
    }

    //ext_out
    for(i=0; i<MAPI_CCFG_VOIP_MAX_TOTAL_ACCOUNT_NUM; i++){
        sprintf(para_name, "account_out_idx_%d", i+1);
        val=mapi_ccfg_get_long(tid, sect_name, para_name, MAPI_CCFG_VOIP_CONNECT_NONE);
        if(val<MAPI_CCFG_VOIP_CONNECT_ISDN_IDX_BASE){
            p_voip_phone->ext_out[i].type=MAPI_CCFG_VOIP_CONNECT_SIP_ACCOUNT;
            p_voip_phone->ext_out[i].logic_idx=(unsigned char)val;
        }
        else if(val<MAPI_CCFG_VOIP_CONNECT_PSTN_IDX_BASE){
            p_voip_phone->ext_out[i].type=MAPI_CCFG_VOIP_CONNECT_ISDN_MSN;
            p_voip_phone->ext_out[i].logic_idx=(unsigned char)(val-MAPI_CCFG_VOIP_CONNECT_ISDN_IDX_BASE);
        }
        else if(val==MAPI_CCFG_VOIP_CONNECT_PSTN_IDX_BASE){
            p_voip_phone->ext_out[i].type=MAPI_CCFG_VOIP_CONNECT_PSTN_NUM;
            p_voip_phone->ext_out[i].logic_idx=0;
        }
#ifdef MAPI_CCFG_VOICE_UMTS_SUPPORT
        else if(val==MAPI_CCFG_VOIP_CONNECT_MOBILE_IDX_BASE){
            p_voip_phone->ext_out[i].type=MAPI_CCFG_VOIP_CONNECT_MOBILE_NUM;
            p_voip_phone->ext_out[i].logic_idx=0;
        }
#endif
        else{
            p_voip_phone->ext_out[i].type=(unsigned char)val;
            p_voip_phone->ext_out[i].logic_idx=0;
        }
    }

    //echo_cancel_ms
    p_voip_phone->echo_cancel_ms=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "ec_mode", MAPI_CCFG_VOIP_DFT_ECHO_CANCEL_MS));

    //vad_eb (mapping to silenceSuppEnable and confortNoiseEnable in .glbcfg)
    p_voip_phone->vad_eb=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "silenceSuppEnable", 0));

    //call_waiting_eb
    p_voip_phone->call_waiting_eb=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "callWaitingSetting", 0));

    //CLIR_eb
    p_voip_phone->CLIR_eb=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "blockCIDSetting", 0));

    //MWI_eb
    p_voip_phone->MWI_eb = MAPI_CCFG_VOIP_DFT_WMI_ENABLE;

    //DND_eb
    p_voip_phone->DND_eb = MAPI_CCFG_VOIP_DFT_DND_ENABLE;

	p_voip_phone->country = (unsigned char)(mapi_ccfg_get_long(tid, sect_name, "country", 0));

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
    }
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Set configuration of a phone
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_voip_phone:	pointer to the buffer of data structure mapi_ccfg_voip_phone_t.
 *	                    Required input fields:
 *                          1. p_voip_phone->phone_idx, 0-based
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* related sections: phone_N (where N is phone line index+1, p_voip_phone->phone_idx+1)
 *
 * See Also
 *		mapi_ccfg_get_voip_sip
 *
 ******************************************************************************/
int mapi_ccfg_set_voip_phone(int tid, mapi_ccfg_voip_phone_t* p_voip_phone)
{
    int                 i, new_transc=0;
    char                sect_name[16], para_name[24];
    unsigned long       val;
    int                     copy_phone=0;
    mapi_ccfg_voip_phone_t  tmp_phone;

    if(p_voip_phone==NULL){
        MID_TRACE("[%s] NULL p_voip_phone\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    if(p_voip_phone->phone_idx==MAPI_CCFG_VOIP_PHONE_ISDN){
        //when setting isdn phone (phone_3), the setting should be applied to phone_4 too
        memcpy((char*)&tmp_phone, (char*)p_voip_phone, sizeof(mapi_ccfg_voip_phone_t));
        tmp_phone.phone_idx=MAPI_CCFG_VOIP_PHONE_ISDN+1;
        copy_phone=1;
    }

do_copy_phone:
    sprintf(sect_name, "phone_%d", p_voip_phone->phone_idx+1);

    //answer_all_eb
    mapi_ccfg_set_long(tid, sect_name, "answer_all", (unsigned long)(p_voip_phone->answer_all_eb));
    
    //ext_in
    for(i=0; i<MAPI_CCFG_VOIP_MAX_TOTAL_ACCOUNT_NUM; i++){
        sprintf(para_name, "account_in_idx_%d", i+1);
        switch(p_voip_phone->ext_in[i].type){
            case MAPI_CCFG_VOIP_CONNECT_SIP_ACCOUNT:
                val=(unsigned long)(p_voip_phone->ext_in[i].logic_idx);
                break;
            case MAPI_CCFG_VOIP_CONNECT_ISDN_MSN:
                val=(unsigned long)(p_voip_phone->ext_in[i].logic_idx+MAPI_CCFG_VOIP_CONNECT_ISDN_IDX_BASE);
                break;
            case MAPI_CCFG_VOIP_CONNECT_PSTN_NUM:
                val=(unsigned long)(p_voip_phone->ext_in[i].logic_idx+MAPI_CCFG_VOIP_CONNECT_PSTN_IDX_BASE);
                break;
#ifdef MAPI_CCFG_VOICE_UMTS_SUPPORT
            case MAPI_CCFG_VOIP_CONNECT_MOBILE_NUM:
                val=(unsigned long)(p_voip_phone->ext_in[i].logic_idx+MAPI_CCFG_VOIP_CONNECT_MOBILE_IDX_BASE);
                break;
#endif
            //special cases
            default:
                val=(unsigned long)(p_voip_phone->ext_in[i].type);
                break;
        }
        mapi_ccfg_set_long(tid, sect_name, para_name, val);
    }

    //ext_out
    for(i=0; i<MAPI_CCFG_VOIP_MAX_TOTAL_ACCOUNT_NUM; i++){
        sprintf(para_name, "account_out_idx_%d", i+1);
        switch(p_voip_phone->ext_out[i].type){
            case MAPI_CCFG_VOIP_CONNECT_SIP_ACCOUNT:
                val=(unsigned long)(p_voip_phone->ext_out[i].logic_idx);
                break;
            case MAPI_CCFG_VOIP_CONNECT_ISDN_MSN:
                val=(unsigned long)(p_voip_phone->ext_out[i].logic_idx+MAPI_CCFG_VOIP_CONNECT_ISDN_IDX_BASE);
                break;
            case MAPI_CCFG_VOIP_CONNECT_PSTN_NUM:
                val=(unsigned long)(p_voip_phone->ext_out[i].logic_idx+MAPI_CCFG_VOIP_CONNECT_PSTN_IDX_BASE);
                break;
#ifdef MAPI_CCFG_VOICE_UMTS_SUPPORT
            case MAPI_CCFG_VOIP_CONNECT_MOBILE_NUM:
                val=(unsigned long)(p_voip_phone->ext_out[i].logic_idx+MAPI_CCFG_VOIP_CONNECT_MOBILE_IDX_BASE);
                break;
#endif
            //special cases
            default:
                val=(unsigned long)(p_voip_phone->ext_out[i].type);
                break;
        }
        mapi_ccfg_set_long(tid, sect_name, para_name, val);
    }

    //echo_cancel_ms
    mapi_ccfg_set_long(tid, sect_name, "ec_mode", (unsigned long)(p_voip_phone->echo_cancel_ms));

    //vad_eb (mapping to silenceSuppEnable and confortNoiseEnable in .glbcfg)
    mapi_ccfg_set_long(tid, sect_name, "silenceSuppEnable", (unsigned long)(p_voip_phone->vad_eb));
    mapi_ccfg_set_long(tid, sect_name, "confortNoiseEnable", (unsigned long)(p_voip_phone->vad_eb));

    //call_waiting_eb
    mapi_ccfg_set_long(tid, sect_name, "callWaitingSetting", (unsigned long)(p_voip_phone->call_waiting_eb));
    
    //CLIR_eb
    mapi_ccfg_set_long(tid, sect_name, "blockCIDSetting", (unsigned long)(p_voip_phone->CLIR_eb));

	//country
	mapi_ccfg_set_long(tid, sect_name, "country", (unsigned long)(p_voip_phone->country));
	
	//MWI_eb
	//DND_eb
	// ===> read-only
	
	if(copy_phone==1){
	    copy_phone=0;
	    p_voip_phone=&tmp_phone;
	    goto do_copy_phone;
    }

    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Get configuration of a dialing rule
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_voip_dial_rule: pointer to the buffer of data structure mapi_ccfg_voip_dial_rule_t.
 *	                    Required input fields:
 *                          1. p_voip_dial_rule->emergency
 *                          2. p_voip_dial_rule->rule_idx
 *      factory_dft:    use 1 to extract the factory default configuration
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* 
 *
 * See Also
 *		mapi_ccfg_set_voip_dial_rule
 *
 ******************************************************************************/
int mapi_ccfg_get_voip_dial_rule(int tid, mapi_ccfg_voip_dial_rule_t* p_voip_dial_rule, int factory_dft)
{
    int                 new_transc=0, rule_idx;
    char                sect_name[24], para_name[24];

    if(p_voip_dial_rule==NULL){
        MID_TRACE("[%s] NULL p_voip_dial_rule\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }

    if(p_voip_dial_rule->emergency)
        strcpy(sect_name, "emergency_rule");
    else
        strcpy(sect_name, "dialing_rule");
    rule_idx=p_voip_dial_rule->rule_idx+1;

    //in_use
    sprintf(para_name, "R%d_In_Use", rule_idx);
    p_voip_dial_rule->in_use=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, para_name, 0));
    
    //conn_type
    sprintf(para_name, "R%d_Connection_Type", rule_idx);
    p_voip_dial_rule->conn_type=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, para_name, MAPI_CCFG_VOIP_CONNECT_NONE));

    //phone_number
    sprintf(para_name, "R%d_Phone_Number", rule_idx);
    mapi_ccfg_get_str(tid, sect_name, para_name, p_voip_dial_rule->phone_number, MAPI_CCFG_VOIP_DIAL_PLAN_PHONE_NUM_MAX_LEN, NULL);

    //comments
    sprintf(para_name, "R%d_Comment", rule_idx);
    mapi_ccfg_get_str(tid, sect_name, para_name, p_voip_dial_rule->comments, MAPI_CCFG_VOIP_DIAL_PLAN_COMMENTS_MAX_LEN, NULL);

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
    }
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Set configuration of a dialing rule
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_voip_dial_rule: pointer to the buffer of data structure mapi_ccfg_voip_dial_rule_t.
 *	                    Required input fields:
 *                          1. p_voip_dial_rule->emergency, 0-dialing rule, 1-emergency rule
 *                          2. p_voip_dial_rule->rule_idx, 0-based
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* related sections:
 *			p_voip_dial_rule->emergency is 0: dialing_rule
 *			p_voip_dial_rule->emergency is 1: emergency_rule
 *
 * See Also
 *		mapi_ccfg_get_voip_dial_rule
 *
 ******************************************************************************/
int mapi_ccfg_set_voip_dial_rule(int tid, mapi_ccfg_voip_dial_rule_t* p_voip_dial_rule)
{
    int                 new_transc=0;
    char                sect_name[24], para_name[24];
    int                 rule_idx;

    if(p_voip_dial_rule==NULL){
        MID_TRACE("[%s] NULL p_voip_dial_rule\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }

    if(p_voip_dial_rule->emergency)
        strcpy(sect_name, "emergency_rule");
    else
        strcpy(sect_name, "dialing_rule");
    rule_idx=p_voip_dial_rule->rule_idx+1;
    
    //in_use
    sprintf(para_name, "R%d_In_Use", rule_idx);
    mapi_ccfg_set_long(tid, sect_name, para_name, (unsigned long)(p_voip_dial_rule->in_use));
    
    //conn_type
    sprintf(para_name, "R%d_Connection_Type", rule_idx);
    mapi_ccfg_set_long(tid, sect_name, para_name, (unsigned long)(p_voip_dial_rule->conn_type));

    //phone_number
    sprintf(para_name, "R%d_Phone_Number", rule_idx);
    mapi_ccfg_set_str(tid, sect_name, para_name, p_voip_dial_rule->phone_number);

    //comments
    sprintf(para_name, "R%d_Comment", rule_idx);
    mapi_ccfg_set_str(tid, sect_name, para_name, p_voip_dial_rule->comments);

    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Get configuration of ISDN MSNs
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_voip_isdn:    pointer to the buffer of data structure mapi_ccfg_voip_isdn_t.
 *	                    Required input fields: none
 *      factory_dft:    use 1 to extract the factory default configuration
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* 
 *
 * See Also
 *		mapi_ccfg_set_voip_isdn
 *
 ******************************************************************************/
int mapi_ccfg_get_voip_isdn(int tid, mapi_ccfg_voip_isdn_t* p_voip_isdn, int factory_dft)
{
    int                 i, j, new_transc=0;
    char                sect_name[]="isdn", para_name[24];

    if(p_voip_isdn==NULL){
        MID_TRACE("[%s] NULL p_voip_isdn\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }

    p_voip_isdn->AddISDNNumberAllowed=(unsigned char)(mapi_ccfg_get_long(tid, sect_name, "AddISDNNumberAllowed", 0));
    //area_code
    mapi_ccfg_get_str(tid, sect_name, "isdn_area_code", p_voip_isdn->area_code, MAPI_CCFG_VOIP_AREA_CODE_MAX_LEN, NULL);

    //msn_number
    for(i=0; i<MAPI_CCFG_VOIP_ISDN_MAX_NUMBER; i++){
        sprintf(para_name, "msn_number_%d", i);
        mapi_ccfg_get_str(tid, sect_name, para_name, &(p_voip_isdn->msn_number[i][0]), MAPI_CCFG_VOIP_MSN_MAX_LEN, NULL);
        sprintf(para_name, "cfu_number_%d", i);
        mapi_ccfg_get_str(tid, sect_name, para_name, &(p_voip_isdn->cfu_number[i][0]), MAPI_CCFG_VOIP_MSN_MAX_LEN, NULL);
        sprintf(para_name, "cfb_number_%d", i);
        mapi_ccfg_get_str(tid, sect_name, para_name, &(p_voip_isdn->cfb_number[i][0]), MAPI_CCFG_VOIP_MSN_MAX_LEN, NULL);
        sprintf(para_name, "cfnr_number_%d", i);
        mapi_ccfg_get_str(tid, sect_name, para_name, &(p_voip_isdn->cfnr_number[i][0]), MAPI_CCFG_VOIP_MSN_MAX_LEN, NULL);
    }

	//isdn_extension
    for(i=0; i<MAPI_CCFG_VOIP_ISDN_EXTENSIONS; i++){
    	for(j=0; j<MAPI_CCFG_VOIP_ISDN_MAX_NUMBER; j++){
    		sprintf(para_name, "msn_index_%d_%d", i, j);
    		p_voip_isdn->msn_index[i][j] = (char) mapi_ccfg_get_long(tid, sect_name, para_name, 0);
    	}
    	sprintf(para_name, "rcv_all_%d", i);
    	p_voip_isdn->rcv_all[i] = (char) mapi_ccfg_get_long(tid, sect_name, para_name, 0);
	}

    p_voip_isdn->auto_detect_off = (int)(mapi_ccfg_get_long(tid, sect_name, "auto_detect_off", 0));
    p_voip_isdn->point2point_enable = (int)(mapi_ccfg_get_long(tid, sect_name, "point2point_enable", 0));

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
    }
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Set configuration of ISDN MSNs
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_voip_isdn:    pointer to the buffer of data structure mapi_ccfg_voip_isdn_t.
 *	                    Required input fields: none
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* related sections: isdn
 *
 * See Also
 *		mapi_ccfg_get_voip_isdn
 *
 ******************************************************************************/
int mapi_ccfg_set_voip_isdn(int tid, mapi_ccfg_voip_isdn_t* p_voip_isdn)
{
    int                 i, j, new_transc=0;
    char                sect_name[]="isdn", para_name[24];

    if(p_voip_isdn==NULL){
        MID_TRACE("[%s] NULL p_voip_isdn\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }

    mapi_ccfg_set_long(tid, sect_name, "AddISDNNumberAllowed", (unsigned long)(p_voip_isdn->AddISDNNumberAllowed));
    //area_code
    mapi_ccfg_set_str(tid, sect_name, "isdn_area_code", p_voip_isdn->area_code);

    //msn_number
    for(i=0; i<MAPI_CCFG_VOIP_ISDN_MAX_NUMBER; i++){
        sprintf(para_name, "msn_number_%d", i);
        mapi_ccfg_set_str(tid, sect_name, para_name, &(p_voip_isdn->msn_number[i][0]));
        sprintf(para_name, "cfu_number_%d", i);
        mapi_ccfg_set_str(tid, sect_name, para_name, &(p_voip_isdn->cfu_number[i][0]));
        sprintf(para_name, "cfb_number_%d", i);
        mapi_ccfg_set_str(tid, sect_name, para_name, &(p_voip_isdn->cfb_number[i][0]));
        sprintf(para_name, "cfnr_number_%d", i);
        mapi_ccfg_set_str(tid, sect_name, para_name, &(p_voip_isdn->cfnr_number[i][0]));
    }

	//isdn_extension
    for(i=0; i<MAPI_CCFG_VOIP_ISDN_EXTENSIONS; i++){
    	for(j=0; j<MAPI_CCFG_VOIP_ISDN_MAX_NUMBER; j++){
    		sprintf(para_name, "msn_index_%d_%d", i, j);
    		mapi_ccfg_set_long(tid, sect_name, para_name, p_voip_isdn->msn_index[i][j]);
    	}
    	sprintf(para_name, "rcv_all_%d", i);
    	mapi_ccfg_set_long(tid, sect_name, para_name, p_voip_isdn->rcv_all[i]);
	}

    mapi_ccfg_set_long(tid, sect_name, "auto_detect_off", p_voip_isdn->auto_detect_off);

    mapi_ccfg_set_long(tid, sect_name, "point2point_enable", p_voip_isdn->point2point_enable);

    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

    return MID_SUCCESS;
}

#ifdef MAPI_CCFG_VOICE_UMTS_SUPPORT

/*******************************************************************************
 * Description
 *		Get configuration of mobile MSN
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_voip_umts:    pointer to the buffer of data structure mapi_ccfg_voip_umts_t.
 *	                    Required input fields: none
 *      factory_dft:    use 1 to extract the factory default configuration
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* 
 *
 * See Also
 *		mapi_ccfg_set_voip_umts
 *
 ******************************************************************************/
int mapi_ccfg_get_voip_umts(int tid, mapi_ccfg_voip_umts_t* p_voip_umts, int factory_dft)
{
    int                 new_transc=0;
    char                sect_name[]="umts_voice";

    if(p_voip_umts==NULL){
        MID_TRACE("[%s] NULL p_voip_umts\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }

    //area_code
    mapi_ccfg_get_str(tid, sect_name, "umts_area_code", p_voip_umts->area_code, MAPI_CCFG_VOIP_AREA_CODE_MAX_LEN, NULL);

    //msn_number
    mapi_ccfg_get_str(tid, sect_name, "umts_number", p_voip_umts->msn_number, MAPI_CCFG_VOIP_MSN_MAX_LEN, NULL);

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
    }
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Set configuration of mobile MSN
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_voip_umts:    pointer to the buffer of data structure mapi_ccfg_voip_umts_t.
 *	                    Required input fields: none
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* related sections: umts_voice
 *
 * See Also
 *		mapi_ccfg_get_voip_umts
 *
 ******************************************************************************/
int mapi_ccfg_set_voip_umts(int tid, mapi_ccfg_voip_umts_t* p_voip_umts)
{
    int                 new_transc=0;
    char                sect_name[]="umts_voice";

    if(p_voip_umts==NULL){
        MID_TRACE("[%s] NULL p_voip_umts\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }

    //area_code
    mapi_ccfg_set_str(tid, sect_name, "umts_area_code", p_voip_umts->area_code);

    //msn_number
    mapi_ccfg_set_str(tid, sect_name, "umts_number", p_voip_umts->msn_number);

    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

    return MID_SUCCESS;
}

#endif // MAPI_CCFG_VOICE_UMTS_SUPPORT

/*******************************************************************************
 * Description
 *		Get all configuration of speed dialing
 *
 * Parameters
 *		tid:				transaction ID returned by calling mapi_start_transc()
 *							use 0 if no tid is provided (no transaction was opened before)
 *		p_voip_speed_dial:	pointer to the buffer of data structure mapi_ccfg_voip_speed_dial_t.
 *      recCnt:				record count of p_voip_speed_dial
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or p_voip_speed_dial is NULL.
 *
 * Note
 *		*
 *
 * See Also
 *		mapi_ccfg_set_voip_speed_dial
 *
 ******************************************************************************/
#define Sip_Data_File "/tmp/Sip_Data" //"/tmp/SpeedDialing" 

#define	COMMAND_BUFFERSIZE 30000 

#ifdef Sip_Data_File
typedef struct mapi_ccfg_voip_sip_data {
    mapi_ccfg_voip_speed_dial_t p_voip_speed_dial;
    mapi_ccfg_voip_special_area_t p_voip_special_area;
} mapi_ccfg_voip_sip_data_t;
#endif
 
int mapi_ccfg_sync_voip_SIP_Data(int tid, int direction) 
{
#ifdef Sip_Data_File
    int fd = -1;
    int len, file_len;
    
    mapi_ccfg_voip_sip_data_t p_voip_sip_data = {0};
    
    mapi_ccfg_voip_speed_dial_t p_voip_speed_dial_temp = {0};
    
    mapi_ccfg_voip_speed_dial_t* p_voip_speed_dial = &p_voip_speed_dial_temp;
        
#ifdef COMMAND_BUFFERSIZE
	char tmp[COMMAND_BUFFERSIZE];
	char qStr[50];

    if(direction<0 || direction>2)
    {
        printf("!%s: Cannot support direction %d \n", __func__, direction);    
        return MID_FAIL;
    }
        
    sprintf(qStr,"mtd_access.sh read sipdata %s", Sip_Data_File);
	osSystem_GetOutput(qStr,tmp,sizeof(tmp));
#else
    if(direction<0 || direction>2)
    {
        printf("!%s: Cannot support direction %d \n", __func__, direction);    
        return MID_FAIL;
    }	
#endif    
    len = sizeof(mapi_ccfg_voip_sip_data_t);
    
    fd = open(Sip_Data_File, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    
    if (fd == -1)
    {         
        
        MID_TRACE("!%s: Cannot open %s \n", __func__, Sip_Data_File);
        printf("!%s: Cannot open %s \n", __func__, Sip_Data_File);
    
        return MID_FAIL;
    }

    file_len = lseek(fd, 0, SEEK_END);
        
    lseek(fd, 0, SEEK_SET);

    if(file_len<len)
    {
        MID_TRACE("!%s: file_len(%d) < len(%d) skip read \n", __func__, file_len, len);
    }
    else
    {
        file_len = read(fd, (char *)&p_voip_sip_data, len);
        if(file_len != len)
        {
            MID_TRACE("!%s: len(%d) != file_len(%d) read fail \n", __func__, len, file_len);
        }
        else
        {
            int							new_transc=0, rule_idx;
        	char						sKey[24];
        	char						sBuf[128];
        	mapi_ccfg_voip_speed_dial_rule_t	stZero = { 0, 0, 0, 0, 0 };
        	mapi_ccfg_voip_speed_dial_rule_t	*pRule;
        	char*						pPtr;
        	char*						pNext;
    	        
            if (tid==0)
        	{
        		new_transc=1;
        		if ( (tid=mapi_start_transc()) ==MID_FAIL )
        			return MID_FAIL;
        	}
        	
        	if(direction == 2)
        	{
        	    p_voip_speed_dial = &(p_voip_sip_data.p_voip_speed_dial);
        	    
        	    mapi_ccfg_unset_multi( tid, "speed_dial", "*" );
	
            	mapi_ccfg_set_long(tid, "speed_dial", "enable", (unsigned long)(p_voip_speed_dial->enable));
            	mapi_ccfg_set_long(tid, "speed_dial", "start", (unsigned long)(p_voip_speed_dial->start));
            	mapi_ccfg_set_long(tid, "speed_dial", "end", (unsigned long)(p_voip_speed_dial->end));
            	mapi_ccfg_set_long(tid, "speed_dial", "rule_num", (unsigned long)(p_voip_speed_dial->rule_num));
            
            	for ( rule_idx=1, pRule=p_voip_speed_dial->rule; rule_idx <= MAPI_CCFG_VOIP_CFG_MAX_SPEED_RULE; rule_idx++, pRule++ )
            	{
            		sprintf( sKey, "rule_R%d", rule_idx);
            		sprintf( sBuf, "%u,%u,%u,%u,%s", pRule->enable, pRule->precedent, pRule->next, pRule->accountIdx, (char*)pRule->phone_num );
            		mapi_ccfg_set_str( tid, "speed_dial", sKey, sBuf );
            	}
            
            	mapi_ccfg_commit_lazy_send( tid, MAPI_CCFG_COMMIT_FILE|MAPI_CCFG_COMMIT_FLASH, NULL );
        	}
        	else
        	{
            	if(direction == 0)//if(factory_dft)
                {
                    mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
                }
            	
            	p_voip_speed_dial->enable=(unsigned char)(mapi_ccfg_get_long(tid, "speed_dial", "enable", 0));
            	p_voip_speed_dial->start=(unsigned char)(mapi_ccfg_get_long(tid, "speed_dial", "start", 0));
            	p_voip_speed_dial->end=(unsigned char)(mapi_ccfg_get_long(tid, "speed_dial", "end", 0));
            	p_voip_speed_dial->rule_num=(unsigned char)(mapi_ccfg_get_long(tid, "speed_dial", "rule_num", 0));
            
            	for ( rule_idx=1, pRule=p_voip_speed_dial->rule; rule_idx <= MAPI_CCFG_VOIP_CFG_MAX_SPEED_RULE; rule_idx++, pRule++ )
            	{
            		*pRule = stZero;
            		sprintf( sKey, "rule_R%d", rule_idx);
            		mapi_ccfg_get_str( tid, "speed_dial", sKey, sBuf, sizeof(sBuf), "" );
            
            		pNext = sBuf;
            
            		if ( ( pPtr=osStrTok_r( pNext, ",", &pNext ) ) == NULL )
            			continue;
            		pRule->enable = (unsigned char)atoi(pPtr);
            
            		if ( ( pPtr=osStrTok_r( pNext, ",", &pNext ) ) == NULL )
            			continue;
            		pRule->precedent = (unsigned char)atoi(pPtr);
            
            		if ( ( pPtr=osStrTok_r( pNext, ",", &pNext ) ) == NULL )
            			continue;
            		pRule->next = (unsigned char)atoi(pPtr);
            
            		if ( ( pPtr=osStrTok_r( pNext, ",", &pNext ) ) == NULL )
            			continue;
            		pRule->accountIdx = (unsigned char)atoi(pPtr);
            
            		if (pNext != NULL)
            			utilStrNCpy( (char*)pRule->phone_num, pNext, sizeof(pRule->phone_num) );
            	}
                
                if(direction == 0)//if(factory_dft)
                {
                    mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
                }
                
                if(direction == 0)
                {
                    memset((char *)&p_voip_sip_data, 0, sizeof(p_voip_sip_data));
                }
                else if(direction == 1)
                {                
                    memset((char *)&(p_voip_sip_data.p_voip_special_area), 0, sizeof(mapi_ccfg_voip_special_area_t));
                }
                    
                memcpy((char *)&(p_voip_sip_data.p_voip_speed_dial), (char *)p_voip_speed_dial, sizeof(mapi_ccfg_voip_speed_dial_t));
                
                lseek(fd, 0, SEEK_SET);
        
                file_len = write(fd,  (char *)&p_voip_sip_data, len);
                if(file_len != len)
                {
                    MID_TRACE("!%s: len(%d) != file_len(%d) write fail \n", __func__, len, file_len);
                }
            }
            
        	if ( new_transc )
        		mapi_end_transc( tid );       
        }
    }
    
    if (fd > 0)
    {
        close(fd);
    }    
    
#ifdef COMMAND_BUFFERSIZE
    sprintf(qStr,"mtd_access.sh write sipdata %s", Sip_Data_File);
	osSystem_GetOutput(qStr,tmp,sizeof(tmp));
#endif  
  
#endif       
    return MID_SUCCESS;
}
 
int mapi_ccfg_get_voip_speed_dial( int tid, mapi_ccfg_voip_speed_dial_t* p_voip_speed_dial )
{
#ifdef Sip_Data_File
    int fd = -1;
    int len, file_len;
    
    mapi_ccfg_voip_sip_data_t p_voip_sip_data = {0};
    
#ifdef COMMAND_BUFFERSIZE
	char tmp[COMMAND_BUFFERSIZE];
	char qStr[50];

    sprintf(qStr,"mtd_access.sh read sipdata %s", Sip_Data_File);
	osSystem_GetOutput(qStr,tmp,sizeof(tmp));
#endif

    if ( p_voip_speed_dial == NULL )
	{
		MID_TRACE("[%s] NULL p_voip_speed_dial\n", __FUNCTION__);
		return MID_FAIL;
	}
	
    //MID_TRACE("##@@!! %s \n", __func__);
    len = sizeof(mapi_ccfg_voip_sip_data_t);
    
    fd = open(Sip_Data_File, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    
    if (fd == -1)
    { 
        int							new_transc=0, rule_idx;
    	char						sKey[24];
    	char						sBuf[128];
    	mapi_ccfg_voip_speed_dial_rule_t	stZero = { 0, 0, 0, 0, 0 };
    	mapi_ccfg_voip_speed_dial_rule_t	*pRule;
    	char*						pPtr;
    	char*						pNext;
	
        MID_TRACE("!%s: Cannot open %s \n", __func__, Sip_Data_File);
        printf("!%s: Cannot open %s \n", __func__, Sip_Data_File);
    
        if (tid==0)
    	{
    		new_transc=1;
    		if ( (tid=mapi_start_transc()) ==MID_FAIL )
    			return MID_FAIL;
    	}
    	
    	if(1)//if(factory_dft)
        {
            mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
        }
    	
    	p_voip_speed_dial->enable=(unsigned char)(mapi_ccfg_get_long(tid, "speed_dial", "enable", 0));
    	p_voip_speed_dial->start=(unsigned char)(mapi_ccfg_get_long(tid, "speed_dial", "start", 0));
    	p_voip_speed_dial->end=(unsigned char)(mapi_ccfg_get_long(tid, "speed_dial", "end", 0));
    	p_voip_speed_dial->rule_num=(unsigned char)(mapi_ccfg_get_long(tid, "speed_dial", "rule_num", 0));
    
    	for ( rule_idx=1, pRule=p_voip_speed_dial->rule; rule_idx <= MAPI_CCFG_VOIP_CFG_MAX_SPEED_RULE; rule_idx++, pRule++ )
    	{
    		*pRule = stZero;
    		sprintf( sKey, "rule_R%d", rule_idx);
    		mapi_ccfg_get_str( tid, "speed_dial", sKey, sBuf, sizeof(sBuf), "" );
    
    		pNext = sBuf;
    
    		if ( ( pPtr=osStrTok_r( pNext, ",", &pNext ) ) == NULL )
    			continue;
    		pRule->enable = (unsigned char)atoi(pPtr);
    
    		if ( ( pPtr=osStrTok_r( pNext, ",", &pNext ) ) == NULL )
    			continue;
    		pRule->precedent = (unsigned char)atoi(pPtr);
    
    		if ( ( pPtr=osStrTok_r( pNext, ",", &pNext ) ) == NULL )
    			continue;
    		pRule->next = (unsigned char)atoi(pPtr);
    
    		if ( ( pPtr=osStrTok_r( pNext, ",", &pNext ) ) == NULL )
    			continue;
    		pRule->accountIdx = (unsigned char)atoi(pPtr);
    
    		if (pNext != NULL)
    			utilStrNCpy( (char*)pRule->phone_num, pNext, sizeof(pRule->phone_num) );
    	}
        
        if(1)//if(factory_dft)
        {
            mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
        }
        
    	if ( new_transc )
    		mapi_end_transc( tid );
    		
        return MID_SUCCESS;
    }

    file_len = lseek(fd, 0, SEEK_END);
        
    lseek(fd, 0, SEEK_SET);

    if(file_len<len)
    {
        MID_TRACE("!%s: file_len(%d) < len(%d) skip read \n", __func__, file_len, len);
    }
    else
    {
        file_len = read(fd, (char *)&p_voip_sip_data, len);
        if(file_len != len)
        {
            MID_TRACE("!%s: len(%d) != file_len(%d) read fail \n", __func__, len, file_len);
        }
        else
        {
            memcpy((char *)p_voip_speed_dial, (char *)&(p_voip_sip_data.p_voip_speed_dial), sizeof(mapi_ccfg_voip_speed_dial_t));
        }
    }
    
    if (fd > 0)
    {
        close(fd);
    }
    
#else

   	int							new_transc=0, rule_idx;
	char						sKey[24];
	char						sBuf[128];
	mapi_ccfg_voip_speed_dial_rule_t	stZero = { 0, 0, 0, 0, 0 };
	mapi_ccfg_voip_speed_dial_rule_t	*pRule;
	char*						pPtr;
	char*						pNext;

    if ( p_voip_speed_dial == NULL )
	{
		MID_TRACE("[%s] NULL p_voip_speed_dial\n", __FUNCTION__);
		return MID_FAIL;
	}
	
	if (tid==0)
	{
		new_transc=1;
		if ( (tid=mapi_start_transc()) ==MID_FAIL )
			return MID_FAIL;
	}
	
	p_voip_speed_dial->enable=(unsigned char)(mapi_ccfg_get_long(tid, "speed_dial", "enable", 0));
	p_voip_speed_dial->start=(unsigned char)(mapi_ccfg_get_long(tid, "speed_dial", "start", 0));
	p_voip_speed_dial->end=(unsigned char)(mapi_ccfg_get_long(tid, "speed_dial", "end", 0));
	p_voip_speed_dial->rule_num=(unsigned char)(mapi_ccfg_get_long(tid, "speed_dial", "rule_num", 0));

	for ( rule_idx=1, pRule=p_voip_speed_dial->rule; rule_idx <= MAPI_CCFG_VOIP_CFG_MAX_SPEED_RULE; rule_idx++, pRule++ )
	{
		*pRule = stZero;
		sprintf( sKey, "rule_R%d", rule_idx);
		mapi_ccfg_get_str( tid, "speed_dial", sKey, sBuf, sizeof(sBuf), "" );

		pNext = sBuf;

		if ( ( pPtr=osStrTok_r( pNext, ",", &pNext ) ) == NULL )
			continue;
		pRule->enable = (unsigned char)atoi(pPtr);

		if ( ( pPtr=osStrTok_r( pNext, ",", &pNext ) ) == NULL )
			continue;
		pRule->precedent = (unsigned char)atoi(pPtr);

		if ( ( pPtr=osStrTok_r( pNext, ",", &pNext ) ) == NULL )
			continue;
		pRule->next = (unsigned char)atoi(pPtr);

		if ( ( pPtr=osStrTok_r( pNext, ",", &pNext ) ) == NULL )
			continue;
		pRule->accountIdx = (unsigned char)atoi(pPtr);

		if (pNext != NULL)
			utilStrNCpy( (char*)pRule->phone_num, pNext, sizeof(pRule->phone_num) );
	}

	if ( new_transc )
		mapi_end_transc( tid );
#endif

	return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Set all configuration of speed dialing
 *
 * Parameters
 *		tid:				transaction ID returned by calling mapi_start_transc()
 *							use 0 if no tid is provided (no transaction was opened before)
 *		p_voip_speed_dial:	pointer to the buffer of data structure mapi_ccfg_voip_speed_dial_t.
 *      recCnt:				record count of p_voip_speed_dial
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or p_voip_speed_dial is NULL.
 *
 * Note
 *		*
 *
 * See Also
 *		mapi_ccfg_get_voip_speed_dial
 *
 ******************************************************************************/
int mapi_ccfg_set_voip_speed_dial( int tid, mapi_ccfg_voip_speed_dial_t* p_voip_speed_dial )
{
#ifdef Sip_Data_File
    int fd = -1;
    int len, file_len;
    
    mapi_ccfg_voip_sip_data_t p_voip_sip_data = {0};
    
#ifdef COMMAND_BUFFERSIZE
	char tmp[COMMAND_BUFFERSIZE];
	char qStr[50];

    sprintf(qStr,"mtd_access.sh write sipdata %s", Sip_Data_File);
#endif

    if ( p_voip_speed_dial == NULL )
	{
		MID_TRACE("[%s] NULL p_voip_speed_dial\n", __FUNCTION__);
		return MID_FAIL;
	}
	
    //MID_TRACE("##@@!! %s \n", __func__);
    
    len = sizeof(mapi_ccfg_voip_sip_data_t);

    fd = open(Sip_Data_File, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    
    if (fd == -1)    
    { 
        MID_TRACE("!%s: Cannot open %s \n", __func__, Sip_Data_File);
        printf("!%s: Cannot open %s \n", __func__, Sip_Data_File);
        return MID_FAIL;
    }
    
    file_len = lseek(fd, 0, SEEK_END);
        
    lseek(fd, 0, SEEK_SET);    

    if(file_len<len)
    {
        MID_TRACE("!%s: file_len(%d) < len(%d) skip read \n", __func__, file_len, len);
    }
    else
    {
        file_len = read(fd, (char *)&p_voip_sip_data, len);
        if(file_len != len)
        {
            MID_TRACE("!%s: len(%d) != file_len(%d) read fail \n", __func__, len, file_len);
        }
        else
        {
            memcpy((char *)&(p_voip_sip_data.p_voip_speed_dial), (char *)p_voip_speed_dial, sizeof(mapi_ccfg_voip_speed_dial_t));
        }
    }
    
    lseek(fd, 0, SEEK_SET);
    
    file_len = write(fd,  (char *)&p_voip_sip_data, len);
    if(file_len != len)
    {
        MID_TRACE("!%s: len(%d) != file_len(%d) write fail \n", __func__, len, file_len);
    }

    if (fd > 0)
    {
        close(fd);
    }
    
#ifdef COMMAND_BUFFERSIZE
	osSystem_GetOutput(qStr,tmp,sizeof(tmp));
#endif
    
#else
    int							new_transc=0, rule_idx;
	char						sKey[24];
	char						sBuf[128];
	mapi_ccfg_voip_speed_dial_rule_t	*pRule;
	
    if ( p_voip_speed_dial == NULL )
	{
		MID_TRACE("[%s] NULL p_voip_speed_dial\n", __FUNCTION__);
		return MID_FAIL;
	}
	
	if (tid==0)
	{
		new_transc=1;
		if ( (tid=mapi_start_transc()) ==MID_FAIL )
			return MID_FAIL;
	}

	mapi_ccfg_unset_multi( tid, "speed_dial", "*" );
	
	mapi_ccfg_set_long(tid, "speed_dial", "enable", (unsigned long)(p_voip_speed_dial->enable));
	mapi_ccfg_set_long(tid, "speed_dial", "start", (unsigned long)(p_voip_speed_dial->start));
	mapi_ccfg_set_long(tid, "speed_dial", "end", (unsigned long)(p_voip_speed_dial->end));
	mapi_ccfg_set_long(tid, "speed_dial", "rule_num", (unsigned long)(p_voip_speed_dial->rule_num));

	for ( rule_idx=1, pRule=p_voip_speed_dial->rule; rule_idx <= MAPI_CCFG_VOIP_CFG_MAX_SPEED_RULE; rule_idx++, pRule++ )
	{
		sprintf( sKey, "rule_R%d", rule_idx);
		sprintf( sBuf, "%u,%u,%u,%u,%s", pRule->enable, pRule->precedent, pRule->next, pRule->accountIdx, (char*)pRule->phone_num );
		mapi_ccfg_set_str( tid, "speed_dial", sKey, sBuf );
	}

	mapi_ccfg_commit_lazy_send( tid, MAPI_CCFG_COMMIT_FILE|MAPI_CCFG_COMMIT_FLASH, NULL );

	if ( new_transc )
		mapi_end_transc( tid );
#endif

	return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Delete all configuration of speed dialing
 *
 * Parameters
 *		tid:				transaction ID returned by calling mapi_start_transc()
 *							use 0 if no tid is provided (no transaction was opened before)
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction
 *
 * Note
 *		*
 *
 * See Also
 *		mapi_ccfg_get_all_voip_speed_dial, mapi_ccfg_set_all_voip_speed_dial
 *
 ******************************************************************************/
int mapi_ccfg_del_all_voip_speed_dial( int tid )
{
	int		new_transc=0;

	if (tid==0)
	{
		new_transc=1;
		if ( (tid=mapi_start_transc()) ==MID_FAIL )
			return MID_FAIL;
	}

	mapi_ccfg_unset_multi( tid, "speed_dial", "*" );

	if ( new_transc )
		mapi_end_transc( tid );

	return MID_SUCCESS;
}


/*************** un-implemented function ******************/

int mapi_ccfg_reset_voip( int tid, unsigned char line_idx )
{
	return MID_UNSUPPORT;
}

/**********************************************************/


/*******************************************************************************
 * Description
 *		Get all configuration of speed dialing
 *
 * Parameters
 *		tid:				transaction ID returned by calling mapi_start_transc()
 *							use 0 if no tid is provided (no transaction was opened before)
 *		p_voip_speed_dial:	pointer to the buffer of data structure mapi_ccfg_voip_speed_dial_t.
 *      recCnt:				record count of p_voip_speed_dial
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or p_voip_speed_dial is NULL.
 *
 * Note
 *		*
 *
 * See Also
 *		mapi_ccfg_set_voip_special_area
 *
 ******************************************************************************/
int mapi_ccfg_get_voip_special_area( int tid, mapi_ccfg_voip_special_area_t* p_voip_special_area )
{
 #ifdef Sip_Data_File
    int fd = -1;
    int len, file_len;
    
    mapi_ccfg_voip_sip_data_t p_voip_sip_data = {0};
    
#ifdef COMMAND_BUFFERSIZE
	char tmp[COMMAND_BUFFERSIZE];
	char qStr[50];

    sprintf(qStr,"mtd_access.sh read sipdata %s", Sip_Data_File);
	osSystem_GetOutput(qStr,tmp,sizeof(tmp));
#endif

    if ( p_voip_special_area == NULL )
	{
		MID_TRACE("[%s] NULL p_voip_special_area\n", __FUNCTION__);
		return MID_FAIL;
	}
	
    //MID_TRACE("##@@!! %s \n", __func__);
    len = sizeof(mapi_ccfg_voip_sip_data_t);
    
    fd = open(Sip_Data_File, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    
    if (fd == -1)
    { 
        MID_TRACE("!%s: Cannot open %s \n", __func__, Sip_Data_File);
        return MID_SUCCESS;
    }

    file_len = lseek(fd, 0, SEEK_END);
        
    lseek(fd, 0, SEEK_SET);

    if(file_len<len)
    {
        MID_TRACE("!%s: file_len(%d) < len(%d) skip read \n", __func__, file_len, len);
    }
    else
    {
        file_len = read(fd, (char *)&p_voip_sip_data, len);
        if(file_len != len)
        {
            MID_TRACE("!%s: len(%d) != file_len(%d) read fail \n", __func__, len, file_len);
        }
        else
        {
            memcpy((char *)p_voip_special_area, (char *)&(p_voip_sip_data.p_voip_special_area), sizeof(mapi_ccfg_voip_special_area_t));
        }
    }
    
    if (fd > 0)
    {
        close(fd);
    }
    
#endif

	return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Set all configuration of speed dialing
 *
 * Parameters
 *		tid:				transaction ID returned by calling mapi_start_transc()
 *							use 0 if no tid is provided (no transaction was opened before)
 *		p_voip_speed_dial:	pointer to the buffer of data structure mapi_ccfg_voip_speed_dial_t.
 *      recCnt:				record count of p_voip_speed_dial
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or p_voip_speed_dial is NULL.
 *
 * Note
 *		*
 *
 * See Also
 *		mapi_ccfg_get_voip_special_area
 *
 ******************************************************************************/
int mapi_ccfg_set_voip_special_area( int tid, mapi_ccfg_voip_special_area_t* p_voip_special_area )
{
#ifdef Sip_Data_File
    int fd = -1;
    int len, file_len;
    
    mapi_ccfg_voip_sip_data_t p_voip_sip_data = {0};
    
#ifdef COMMAND_BUFFERSIZE
	char tmp[COMMAND_BUFFERSIZE];
	char qStr[50];

    sprintf(qStr,"mtd_access.sh write sipdata %s", Sip_Data_File);
#endif

    if ( p_voip_special_area == NULL )
	{
		MID_TRACE("[%s] NULL p_voip_special_area\n", __FUNCTION__);
		return MID_FAIL;
	}
	
    //MID_TRACE("##@@!! %s \n", __func__);
    
    len = sizeof(mapi_ccfg_voip_sip_data_t);

    fd = open(Sip_Data_File, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    
    if (fd == -1)    
    { 
        MID_TRACE("!%s: Cannot open %s \n", __func__, Sip_Data_File);
        printf("!%s: Cannot open %s \n", __func__, Sip_Data_File);
        return MID_FAIL;
    }
    
    file_len = lseek(fd, 0, SEEK_END);
        
    lseek(fd, 0, SEEK_SET);    

    if(file_len<len)
    {
        MID_TRACE("!%s: file_len(%d) < len(%d) skip read \n", __func__, file_len, len);
    }
    else
    {
        file_len = read(fd, (char *)&p_voip_sip_data, len);
        if(file_len != len)
        {
            MID_TRACE("!%s: len(%d) != file_len(%d) read fail \n", __func__, len, file_len);
        }
        else
        {
            memcpy((char *)&(p_voip_sip_data.p_voip_special_area), (char *)p_voip_special_area, sizeof(mapi_ccfg_voip_special_area_t));
        }
    }
    
    lseek(fd, 0, SEEK_SET);
    
    file_len = write(fd,  (char *)&p_voip_sip_data, len);
    if(file_len != len)
    {
        MID_TRACE("!%s: len(%d) != file_len(%d) write fail \n", __func__, len, file_len);
    }

    if (fd > 0)
    {
        close(fd);
    }
    
#ifdef COMMAND_BUFFERSIZE
	osSystem_GetOutput(qStr,tmp,sizeof(tmp));
#endif
    
#endif
	return MID_SUCCESS;
}
