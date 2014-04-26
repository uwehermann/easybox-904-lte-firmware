#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libArcComApi.h>
#include "mid_mapi_ccfgsal_wlan.h"

/*******************************************************************************
 * Description
 *		Set WLAN security
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_sec_cfg:	pointer to the buffer of data structure mapi_ccfg_wlan_sec_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_sec_cfg is NULL.
 *
 * Note
 *		* related sections:
 *				p_wlan_sec_cfg->sec_mode is MAPI_CCFG_WLAN_SEC_WEP:
					8021x_B_N, wep_B_N
 *				p_wlan_sec_cfg->sec_mode is NOT MAPI_CCFG_WLAN_SEC_WEP:
					8021x_B_N
				where B is p_wlan_sec_cfg->band and N is p_wlan_sec_cfg->ap_idx
 *
 * See Also
 *
 ******************************************************************************/
#define MIN(x,y) ((x) < (y) ? (x) : (y))
int mapi_ccfg_set_wlan_sec(int tid, mapi_ccfg_wlan_sec_t* p_wlan_sec_cfg){
 
    int                 new_transc=0,i=0;
    char                tmp_buf[128],section[16],member[10],wep_section[10];
  	char				key_id[10];
	int					ret = MID_FAIL;
	char				wep_key[64];

    if ( p_wlan_sec_cfg == NULL ) {
        MID_TRACE("[%s] NULL p_wlan_sec_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }
    
    if ( tid == 0 ) {
        if ( (tid=mapi_start_transc()) == MID_FAIL )
            return MID_FAIL;
        new_transc = 1;
    }
    
    //decide which band and index should be config
    if((p_wlan_sec_cfg->band == MAPI_CCFG_WLAN_BAND_24GHZ)||(p_wlan_sec_cfg->band == MAPI_CCFG_WLAN_BAND_5GHZ)){   
    	sprintf(section,"8021x_%d_%d",p_wlan_sec_cfg->band,p_wlan_sec_cfg->ap_idx);
    	sprintf(wep_section,"wep_%d_%d",p_wlan_sec_cfg->band,p_wlan_sec_cfg->ap_idx);
    	//printf("set %s section\n",section);
    	//printf("set %s wep_section\n",wep_section);
    }else{
    	printf("[error][%s],band and index should be decided \n",__FUNCTION__);
		goto end_mapi_ccfg_set_wlan_sec;
    }
    
    //mapi_ccfg_set_long( tid, section, "WIRELESS_ENABLE", ui_value);

    //authentication
    mapi_ccfg_set_long( tid, section, "authen_mode", p_wlan_sec_cfg->authen_mode);

	if(p_wlan_sec_cfg->sec_mode==MAPI_CCFG_WLAN_SEC_OS){ //open mode
		
		//printf("set as open mode\n");
		mapi_ccfg_set_long( tid, section, "enableOS", 1);
		mapi_ccfg_set_long( tid, section, "enableWEP", 0);
		mapi_ccfg_set_long( tid, section, "enableSSN", 0);
	}else if(p_wlan_sec_cfg->sec_mode==MAPI_CCFG_WLAN_SEC_WEP){ //wep
		
		mapi_ccfg_set_long( tid, section, "enableOS", 0);
		mapi_ccfg_set_long( tid, section, "enableWEP", 1);
		mapi_ccfg_set_long( tid, section, "enableSSN", 0);

		mapi_ccfg_set_long( tid, wep_section, "keysize", p_wlan_sec_cfg->wep_key_len);
		mapi_ccfg_set_long( tid, wep_section, "defaultkey", p_wlan_sec_cfg->wep_dft_key_id);

		if((p_wlan_sec_cfg->wep_key_len == 5)||(p_wlan_sec_cfg->wep_key_len == 10) || //hex-64
			(p_wlan_sec_cfg->wep_key_len == 13)||(p_wlan_sec_cfg->wep_key_len == 26)|| //hex-104
			(p_wlan_sec_cfg->wep_key_len == 16)||(p_wlan_sec_cfg->wep_key_len == 32) //hex-128
			)
		{
				for(i=0;i<4;i++){
					unsigned len=strlen(p_wlan_sec_cfg->wep_key[i]);
					sprintf(key_id,"key%d",(i+1));
					
					if (len < p_wlan_sec_cfg->wep_key_len) {
						strncat(p_wlan_sec_cfg->wep_key[i],
						"00000000000000000000000000000000",
						MIN(p_wlan_sec_cfg->wep_key_len-len,32));
					}
					else if (len > p_wlan_sec_cfg->wep_key_len) {
						p_wlan_sec_cfg->wep_key[i][p_wlan_sec_cfg->wep_key_len]=0;
					}
					
					
			 		mapi_ccfg_set_str( tid,wep_section,key_id,&p_wlan_sec_cfg->wep_key[i][0]);
				}
		}
		else{
			printf("[error][%s],wep keysize should be decided \n",__FUNCTION__);
			goto end_mapi_ccfg_set_wlan_sec;
		}
		
	}else{
		mapi_ccfg_set_long( tid, section, "enableOS", 0);
		mapi_ccfg_set_long( tid, section, "enableWEP", 0);

	    if(p_wlan_sec_cfg->sec_mode==MAPI_CCFG_WLAN_SEC_WPA){ //wpa
		    mapi_ccfg_set_long( tid, section, "enableSSN", 1);
		}
		else if(p_wlan_sec_cfg->sec_mode==MAPI_CCFG_WLAN_SEC_WPA2){ //wpa2
		    mapi_ccfg_set_long( tid, section, "enableSSN", 2);
		}
		else{ //wpawpa2
		    mapi_ccfg_set_long( tid, section, "enableSSN", 3);
		}
		mapi_ccfg_set_long( tid, section, "wpa_cypher", p_wlan_sec_cfg->wpa_cypher_suite);
		mapi_ccfg_set_long( tid, section, "wpa2_cypher", p_wlan_sec_cfg->wpa2_cypher_suite);

		if(p_wlan_sec_cfg->psk_key_input == MAPI_CCFG_WLAN_WPA_PASSPHRASE_PSK){
			//printf("set as open wpa asciikey\n");
			mapi_ccfg_set_str( tid, section, "ssnpskASCIIkey", p_wlan_sec_cfg->psk_passphrase);
		}else{
			//printf("set as open wpa ssnpsk256key\n");
			mapi_ccfg_set_str( tid, section, "ssnpsk256key", p_wlan_sec_cfg->psk_256key);
		}
		mapi_ccfg_set_long( tid, section, "psk_key_input", p_wlan_sec_cfg->psk_key_input);
	}

	ret = MID_SUCCESS;


end_mapi_ccfg_set_wlan_sec:

	if ( new_transc )
		mapi_end_transc( tid );

	return ret;

}

int mapi_ccfg_get_wlan_sec(int tid, mapi_ccfg_wlan_sec_t* p_wlan_sec_cfg, int factory_dft){
	
	int                 i, new_transc=0;
	char                section[16],member[10],wep_section[10],temp,key_buff[256]={0},buff[33]={0},key_buff_temp[64]={0};
	int                 is_ssn=0;
	int					ret = MID_FAIL;
   // mapi_ccfg_qos_profile_t  *qos_pf=NULL;

    if(p_wlan_sec_cfg==NULL){
        MID_TRACE("[%s] NULL p_wlan_sec_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if (tid==0) {
        if((tid=mapi_start_transc())==MID_FAIL)
            return MID_FAIL;
        new_transc = 1;
    }
  
    if ( factory_dft )
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);

	 if((p_wlan_sec_cfg->band == MAPI_CCFG_WLAN_BAND_24GHZ)||(p_wlan_sec_cfg->band == MAPI_CCFG_WLAN_BAND_5GHZ)){   
    	sprintf(section,"8021x_%d_%d",p_wlan_sec_cfg->band,p_wlan_sec_cfg->ap_idx);
    	sprintf(wep_section,"wep_%d_%d",p_wlan_sec_cfg->band,p_wlan_sec_cfg->ap_idx);
    	//printf("get %s section\n",section);
    	//printf("or get %s section\n",wep_section);
    }else{
    	printf("[error][%s],band and index should be decided \n",__FUNCTION__);
		goto end_mapi_ccfg_get_wlan_sec;
    }

	//memset((char *)p_wlan_sec_cfg, 0, sizeof(*p_wlan_sec_cfg));
    
    if( mapi_ccfg_get_long( tid, section, "enableOS", 1)){
    	//printf("get open mode\n");
    	p_wlan_sec_cfg->sec_mode=MAPI_CCFG_WLAN_SEC_OS;
    }else if(mapi_ccfg_get_long( tid, section, "enableWEP", 1)){
    	//printf("get wep mode\n");
    	p_wlan_sec_cfg->sec_mode=MAPI_CCFG_WLAN_SEC_WEP;
    }else{
    	//printf("set as wpa mode!!\n");
        is_ssn=mapi_ccfg_get_long( tid, section, "enableSSN", 3);
    	
    	if (is_ssn == 1){
    		//printf("this is WPA only\n");
    		p_wlan_sec_cfg->sec_mode=MAPI_CCFG_WLAN_SEC_WPA;    		
    	}else if (is_ssn == 2){
    		//printf("this is WPA2 only\n");
    		p_wlan_sec_cfg->sec_mode=MAPI_CCFG_WLAN_SEC_WPA2;
    	}else if (is_ssn == 3){
    		//printf("this is WPA/WPA2 MIX\n");
    		p_wlan_sec_cfg->sec_mode=MAPI_CCFG_WLAN_SEC_WPAWPA2;
    	}else {
    		printf("[error] the enableSSN value was wrong!!\n");
			//goto end_mapi_ccfg_get_wlan_sec;
    	}
	}

	//authentication
    p_wlan_sec_cfg->authen_mode=mapi_ccfg_get_long( tid, section, "authen_mode", MAPI_CCFG_WLAN_AUTHEN_NO);
    //wep settings
	p_wlan_sec_cfg->wep_key_len=mapi_ccfg_get_long( tid, wep_section, "keysize", 5);
	p_wlan_sec_cfg->wep_dft_key_id=mapi_ccfg_get_long( tid, wep_section, "defaultkey", 0);

		if((p_wlan_sec_cfg->wep_key_len == 5)||(p_wlan_sec_cfg->wep_key_len == 10) || //hex-64
			(p_wlan_sec_cfg->wep_key_len == 13)||(p_wlan_sec_cfg->wep_key_len == 26)|| //hex-104
			(p_wlan_sec_cfg->wep_key_len == 16)||(p_wlan_sec_cfg->wep_key_len == 32) //hex-128
			)
		{
			char default_key[MAPI_CCFG_WLAN_WEP_MAX_KEYLEN]="00000000000000000000000000000000";
			default_key[p_wlan_sec_cfg->wep_key_len]=0;
			//printf("default_key=%s\n",default_key);
			for(i=0;i<4;i++){
				sprintf(member,"key%d",(i+1));
				mapi_ccfg_get_str( tid, wep_section, member , &p_wlan_sec_cfg->wep_key[i][0], MAPI_CCFG_WLAN_WEP_MAX_KEYLEN, default_key);
			}
		}
		else{
			printf("[error][%s],wep keysize should be decided \n",__FUNCTION__);
		//goto end_mapi_ccfg_get_wlan_sec;
		}

	//wpa/wpa2 settings
	p_wlan_sec_cfg->wpa_cypher_suite=mapi_ccfg_get_long( tid, section, "wpa_cypher", 1);
	p_wlan_sec_cfg->wpa2_cypher_suite=mapi_ccfg_get_long( tid, section, "wpa2_cypher", 2);
	p_wlan_sec_cfg->psk_key_input=mapi_ccfg_get_long( tid, section, "psk_key_input", MAPI_CCFG_WLAN_WPA_PASSPHRASE_PSK);
    mapi_ccfg_get_str( tid, section, "ssnpskASCIIkey", p_wlan_sec_cfg->psk_passphrase, MAPI_CCFG_WLAN_WPA_PASSPHRASE_KEY_LEN, "00000000");
    mapi_ccfg_get_str( tid, section, "ssnpsk256key", p_wlan_sec_cfg->psk_256key, MAPI_CCFG_WLAN_WPA_HEX_KEY_LEN, "0000000000000000000000000000000000000000000000000000000000000000");

	ret = MID_SUCCESS;


end_mapi_ccfg_get_wlan_sec:
    if( factory_dft ){
        mapi_ccfg_switch_in_out_factory_cfg( tid, 0 );
    }

	if ( new_transc )
		mapi_end_transc( tid );

	return ret;
}

int mapi_ccfg_get_wlan_1x(int tid, mapi_ccfg_wlan_1x_t* p_wlan_1x_cfg, int factory_dft){
	int                 new_transc=0;
	char                section[16];
   // mapi_ccfg_qos_profile_t  *qos_pf=NULL;

    if ( p_wlan_1x_cfg == NULL ) {
        MID_TRACE("[%s] NULL p_wlan_1x_cfg \n", __FUNCTION__);
        return MID_FAIL;
    }

    if ( tid == 0 ) {
        if ( (tid=mapi_start_transc()) == MID_FAIL )
            return MID_FAIL;
        new_transc = 1;
    }

    if ( factory_dft )
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);

	
   	sprintf(section,"8021x_%d_%d",0,0);
   

	//memset((char *)p_wlan_1x_cfg, 0, sizeof(*p_wlan_1x_cfg));
	
	p_wlan_1x_cfg->srv_port=mapi_ccfg_get_long( tid, section, "srv_port", 0);
	p_wlan_1x_cfg->idle_session_timeout=mapi_ccfg_get_long( tid, section, "idle_session_timeout", 300000);
	p_wlan_1x_cfg->re_auth_period=mapi_ccfg_get_long( tid, section, "reAuthPeriod", 3600000);
	p_wlan_1x_cfg->quiet_period=mapi_ccfg_get_long( tid, section, "quietPeriod", 60000);
	
	mapi_ccfg_get_str( tid, section, "srv_ip", p_wlan_1x_cfg->srv_ip, MAPI_CCFG_WLAN_IP4_LEN, "0.0.0.0");
	mapi_ccfg_get_str( tid, section, "secret_key", p_wlan_1x_cfg->secret_key, MAPI_CCFG_WLAN_1X_SECRET_KEY_MAX_LEN, "secret_key");
	mapi_ccfg_get_str( tid, section, "nas_id", p_wlan_1x_cfg->nas_id, MAPI_CCFG_WLAN_1X_NAS_ID_MAX_LEN, "test_tls");

    if( factory_dft ){
        mapi_ccfg_switch_in_out_factory_cfg( tid, 0 );
    }

	if ( new_transc )
		mapi_end_transc( tid );

	return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Set WLAN 802.1X
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_1x_cfg:	pointer to the buffer of data structure mapi_ccfg_wlan_1x_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_sec_cfg is NULL.
 *
 * Note
 *		* related sections: 8021x_B_N, wep_B_N (where B is p_wlan_1x_cfg->band and N is p_wlan_1x_cfg->ap_idx)
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_set_wlan_1x(int tid, mapi_ccfg_wlan_1x_t* p_wlan_1x_cfg){
	
	int                 new_transc=0;
    char                section[16];

    if ( p_wlan_1x_cfg == NULL ) {
        MID_TRACE("[%s] NULL p_wlan_1x_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if ( tid == 0 ) {
        if ( (tid=mapi_start_transc()) == MID_FAIL )
            return MID_FAIL;
        new_transc = 1;
    }

    sprintf(section,"8021x_%d_%d",0,0);
    
    mapi_ccfg_set_long( tid, section, "srv_port", p_wlan_1x_cfg->srv_port);	
    mapi_ccfg_set_long( tid, section, "idle_session_timeout", p_wlan_1x_cfg->idle_session_timeout);	
    mapi_ccfg_set_long( tid, section, "reAuthPeriod", p_wlan_1x_cfg->re_auth_period);	
    mapi_ccfg_set_long( tid, section, "quietPeriod", p_wlan_1x_cfg->quiet_period);	
    mapi_ccfg_set_str( tid, section, "srv_ip", p_wlan_1x_cfg->srv_ip);
    mapi_ccfg_set_str( tid, section, "secret_key", p_wlan_1x_cfg->secret_key);
    mapi_ccfg_set_str( tid, section, "nas_id", p_wlan_1x_cfg->nas_id);

	if ( new_transc )
		mapi_end_transc( tid );

	return MID_SUCCESS;
}

#define MAX_SUPPORT_MODE 12
	/*
	definition for Wireless Mode
	0x0: Mixed 802.11g and 802.11b
	0x1: 802.11b only
	0x2: 802.11a only
	0x4: 802.11g only
	0x6: 802.11n only
	0x7: Mixed 802.11n and 802.11g
	0x8: Mixed 802.11a and 802.11n
	0x9: Mixed 802.11n, 802.11g and 802.11b
	*/
	
unsigned char tblWirelessMode2Mask[MAX_SUPPORT_MODE]=
{
	(MAPI_CCFG_WLAN_11B_MASK | MAPI_CCFG_WLAN_11G_MASK),	// 0
	MAPI_CCFG_WLAN_11B_MASK,
	MAPI_CCFG_WLAN_11A_MASK,
		0,
	MAPI_CCFG_WLAN_11G_MASK,	// 4
		0,
	MAPI_CCFG_WLAN_11N_MASK,
	(MAPI_CCFG_WLAN_11N_MASK|MAPI_CCFG_WLAN_11G_MASK),			//7
	(MAPI_CCFG_WLAN_11N_MASK|MAPI_CCFG_WLAN_11A_MASK),				// 8
	(MAPI_CCFG_WLAN_11N_MASK|MAPI_CCFG_WLAN_11G_MASK|MAPI_CCFG_WLAN_11B_MASK),//9
	0,
	MAPI_CCFG_WLAN_11N_MASK
};


	

/*******************************************************************************
 * Description
 *		Get WLAN configuration
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_ap_cfg:	pointer to the buffer of data structure mapi_ccfg_wlan_ap_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_ap_cfg is NULL.
 *
 * Note
 *		Need to specify p_wlan_ap_cfg->band and	p_wlan_ap_cfg->ap_idx 
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_get_wlan_ap(int tid, mapi_ccfg_wlan_ap_t* p_wlan_ap_cfg, int factory_dft)
{
	int                 new_transc=0,i;
	char                section[16];
	unsigned char   band;   
	unsigned char   ap_idx;  
	unsigned long WirelessMode;
	char tmp_para[64];

    if ( p_wlan_ap_cfg == NULL ) {
        MID_TRACE("[%s] NULL p_wlan_ap_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if ( tid == 0 ) {
        if ( (tid=mapi_start_transc()) == MID_FAIL )
            return MID_FAIL;
        new_transc = 1;
    }
    
    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }
    sprintf(section,"wireless");
    
    // Sanity check
		band = p_wlan_ap_cfg->band;
		ap_idx = p_wlan_ap_cfg->ap_idx;
    if ( band >= MAPI_CCFG_WLAN_MAX_BAND_NUM || ap_idx >= MAPI_CCFG_WLAN_MAX_SSID_NUM ) {
        MID_TRACE("[%s] band or apidx out of range!\n", __FUNCTION__);
        return MID_FAIL;
    }
		//memset((char *)p_wlan_ap_cfg, 0, sizeof(*p_wlan_ap_cfg));
	
	
	 // Get Configuration
	sprintf(tmp_para,"%s_AP_PRIMARY_CH",(band)?"5G":"2.4G");
	p_wlan_ap_cfg->channel = (unsigned char)mapi_ccfg_get_long( tid, section, tmp_para, 0);

	sprintf(tmp_para,"%s_AP_CHMODE",(band)?"5G":"2.4G");
	WirelessMode = mapi_ccfg_get_long( tid, section, tmp_para, 9);
	
	sprintf(tmp_para,"%s_PROTECTION",(band)?"5G":"2.4G");
	p_wlan_ap_cfg->protect = (unsigned char)mapi_ccfg_get_long( tid, section, tmp_para, 1);
	
	sprintf(tmp_para,"%s_CHANNEL_BW",(band)?"5G":"2.4G");
	p_wlan_ap_cfg->bandwidth = (unsigned char)mapi_ccfg_get_long( tid, section, tmp_para, 1);
	
	sprintf(tmp_para,"%s_AP_EX_CH",(band)?"5G":"2.4G");
	p_wlan_ap_cfg->ht_ext_channel = (unsigned char)mapi_ccfg_get_long( tid, section, tmp_para, 0);
	
	sprintf(tmp_para,"%s_WMM_ENABLE",(band)?"5G":"2.4G");
	p_wlan_ap_cfg->wmm_capable = (unsigned char)mapi_ccfg_get_long( tid, section, tmp_para, 0);

	sprintf(tmp_para,"MBSS_ENABLE_%d_%d",band,ap_idx);
	p_wlan_ap_cfg->enable = (unsigned char)mapi_ccfg_get_long( tid, section, tmp_para, 1);


	if (band==MAPI_CCFG_WLAN_BAND0) {
		switch (ap_idx) {
			case 0:
		 					mapi_ccfg_get_str( tid, section, "AP_SSID", p_wlan_ap_cfg->ssid, MAPI_CCFG_WLAN_SSID_MAX_LEN, NULL);
		 					p_wlan_ap_cfg->ssid_hidden = (unsigned char)mapi_ccfg_get_long( tid, section, "hide_ssid", 0);
		 					p_wlan_ap_cfg->isolate_wlan_eb = (unsigned char)mapi_ccfg_get_long( tid, section, "isolate_ssid", 0);
		 					break;
			default :
		 					mapi_ccfg_get_str( tid, section, "AP_SSID_3", p_wlan_ap_cfg->ssid, MAPI_CCFG_WLAN_SSID_MAX_LEN, NULL);
		 					p_wlan_ap_cfg->ssid_hidden = (unsigned char)mapi_ccfg_get_long( tid, section, "hide_ssid_3", 0);
		 					p_wlan_ap_cfg->isolate_wlan_eb = (unsigned char)mapi_ccfg_get_long( tid, section, "isolate_ssid_3", 0);
		 					break;
		}
	}
	else {
		switch (ap_idx) {
			case 0:
		 					mapi_ccfg_get_str( tid, section, "AP_SSID_2", p_wlan_ap_cfg->ssid, MAPI_CCFG_WLAN_SSID_MAX_LEN, NULL);
		 					p_wlan_ap_cfg->ssid_hidden = (unsigned char)mapi_ccfg_get_long( tid, section, "hide_ssid_2", 0);
		 					p_wlan_ap_cfg->isolate_wlan_eb = (unsigned char)mapi_ccfg_get_long( tid, section, "isolate_ssid_2", 0);
		 					break;
			default :
		 					mapi_ccfg_get_str( tid, section, "AP_SSID_4", p_wlan_ap_cfg->ssid, MAPI_CCFG_WLAN_SSID_MAX_LEN, NULL);
		 					p_wlan_ap_cfg->ssid_hidden = (unsigned char)mapi_ccfg_get_long( tid, section, "hide_ssid_4", 0);
		 					p_wlan_ap_cfg->isolate_wlan_eb = (unsigned char)mapi_ccfg_get_long( tid, section, "isolate_ssid_4", 0);
		 					break;
		}
	}		
	p_wlan_ap_cfg->mode_mask = tblWirelessMode2Mask[MIN((MAX_SUPPORT_MODE-1),WirelessMode)];


	p_wlan_ap_cfg->band = band;
	p_wlan_ap_cfg->ap_idx = ap_idx;
		
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
 *		Set WLAN configuration
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_ap_cfg:	pointer to the buffer of data structure mapi_ccfg_wlan_ap_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_ap_cfg is NULL.
 *
 * Note
 *		* related sections: wireless
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_set_wlan_ap(int tid, mapi_ccfg_wlan_ap_t* p_wlan_ap_cfg)
{
	int                 new_transc=0,i;
	char                section[16];
	char tmp_para[64];
	unsigned char   band;   
	unsigned char   ap_idx; 
	unsigned long WirelessMode; 

    if ( p_wlan_ap_cfg == NULL ) {
        MID_TRACE("[%s] NULL p_wlan_ap_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if ( tid == 0 ) {
        if ( (tid=mapi_start_transc()) == MID_FAIL )
            return MID_FAIL;
        new_transc = 1;
    }

    sprintf(section,"wireless");

    // Sanity check
		band = p_wlan_ap_cfg->band;
		ap_idx = p_wlan_ap_cfg->ap_idx;
    if ( band >= MAPI_CCFG_WLAN_MAX_BAND_NUM || ap_idx >= MAPI_CCFG_WLAN_MAX_SSID_NUM ) {
        MID_TRACE("[%s] band or apidx out of range!\n", __FUNCTION__);
        return MID_FAIL;
    }
    
    sprintf(tmp_para,"%s_AP_PRIMARY_CH",(band)?"5G":"2.4G");
    mapi_ccfg_set_long( tid, section, tmp_para, p_wlan_ap_cfg->channel);	

    sprintf(tmp_para,"%s_AP_CHMODE",(band)?"5G":"2.4G");
    for (i=0;i<MAX_SUPPORT_MODE;i++)	
     if (p_wlan_ap_cfg->mode_mask == tblWirelessMode2Mask[i]) {
       WirelessMode = i;
	   break;
     }

	// 5G N only have different mode with 2.4G in most chipss
	if ( band == MAPI_CCFG_WLAN_BAND1 && WirelessMode == 6)
		WirelessMode = 11;
		
    mapi_ccfg_set_long( tid, section, tmp_para, MIN((MAX_SUPPORT_MODE-1),WirelessMode));	

    sprintf(tmp_para,"%s_PROTECTION",(band)?"5G":"2.4G");
    mapi_ccfg_set_long( tid, section, tmp_para, p_wlan_ap_cfg->protect);	

    sprintf(tmp_para,"%s_CHANNEL_BW",(band)?"5G":"2.4G");
    mapi_ccfg_set_long( tid, section, tmp_para, p_wlan_ap_cfg->bandwidth);	

    sprintf(tmp_para,"%s_AP_EX_CH",(band)?"5G":"2.4G");
    mapi_ccfg_set_long( tid, section, tmp_para, p_wlan_ap_cfg->ht_ext_channel);	

    sprintf(tmp_para,"%s_WMM_ENABLE",(band)?"5G":"2.4G");
    mapi_ccfg_set_long( tid, section, tmp_para, p_wlan_ap_cfg->wmm_capable);	

    sprintf(tmp_para,"MBSS_ENABLE_%d_%d",band,ap_idx);
    mapi_ccfg_set_long( tid, section, tmp_para, p_wlan_ap_cfg->enable);	


	if (band==MAPI_CCFG_WLAN_BAND0) {
		switch (ap_idx) {
			case 0:
		 					mapi_ccfg_set_str( tid, section, "AP_SSID", p_wlan_ap_cfg->ssid);
		 					mapi_ccfg_set_long( tid, section, "hide_ssid", p_wlan_ap_cfg->ssid_hidden);	
		 					mapi_ccfg_set_long( tid, section, "isolate_ssid", p_wlan_ap_cfg->isolate_wlan_eb);
		 					break;
			default :
		 					mapi_ccfg_set_str( tid, section, "AP_SSID_3", p_wlan_ap_cfg->ssid);
		 					mapi_ccfg_set_long( tid, section, "hide_ssid_3", p_wlan_ap_cfg->ssid_hidden);	
		 					mapi_ccfg_set_long( tid, section, "isolate_ssid_3", p_wlan_ap_cfg->isolate_wlan_eb);
		 					break;
		}
	}
	else {
		switch (ap_idx) {
			case 0:
		 					mapi_ccfg_set_str( tid, section, "AP_SSID_2", p_wlan_ap_cfg->ssid);
		 					mapi_ccfg_set_long( tid, section, "hide_ssid_2", p_wlan_ap_cfg->ssid_hidden);	
		 					mapi_ccfg_set_long( tid, section, "isolate_ssid_2", p_wlan_ap_cfg->isolate_wlan_eb);	
		 					break;
			default :
		 					mapi_ccfg_set_str( tid, section, "AP_SSID_4", p_wlan_ap_cfg->ssid);
		 					mapi_ccfg_set_long( tid, section, "hide_ssid_4", p_wlan_ap_cfg->ssid_hidden);	
		 					mapi_ccfg_set_long( tid, section, "isolate_ssid_4", p_wlan_ap_cfg->isolate_wlan_eb);
		 					break;
		}
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
 *		Get WDS configuration
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_wds_cfg:	pointer to the buffer of data structure mapi_ccfg_wds_ap_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_wds_cfg is NULL.
 *
 * Note
 *		* Need to specify p_wlan_wds_cfg->band 
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_get_wlan_wds(int tid, mapi_ccfg_wlan_wds_t* p_wlan_wds_cfg, int factory_dft)
{
	int                 new_transc=0,i;
	char                section[]="wds";
	unsigned char   band;   
	char tmp_para[64];

    if ( p_wlan_wds_cfg == NULL ) {
        MID_TRACE("[%s] NULL p_wlan_wds_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if ( tid == 0 ) {
        if ( (tid=mapi_start_transc()) == MID_FAIL )
            return MID_FAIL;
        new_transc = 1;
    }
    
    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }
    
    
    
    // Sanity check
		band = p_wlan_wds_cfg->band;
		
    if ( band >= MAPI_CCFG_WLAN_MAX_BAND_NUM  ) {
        MID_TRACE("[%s] band or apidx out of range!\n", __FUNCTION__);
        return MID_FAIL;
    }
		//memset((char *)p_wlan_wds_cfg, 0, sizeof(*p_wlan_wds_cfg));
	
	
	 // Get Configuration
		
	sprintf(tmp_para,"%s_ENABLE",(band)?"5G":"2.4G");
	p_wlan_wds_cfg->wds_enable = (unsigned char)mapi_ccfg_get_long( tid, section, tmp_para, 0);
	
	for (i=0;i<MAPI_CCFG_WLAN_MAX_WDS;i++)
	{
		sprintf(tmp_para,"%s_EB%d",((band)?"5G":"2.4G"),i);
		p_wlan_wds_cfg->wds_peer_eb[i] = (unsigned char)mapi_ccfg_get_long( tid, section, tmp_para, 0);
		sprintf(tmp_para,"%s_MAC%d",((band)?"5G":"2.4G"),i);
		mapi_ccfg_get_str( tid, section, tmp_para, p_wlan_wds_cfg->wds_peer_mac[i], MAPI_CCFG_WLAN_MAC_LEN, NULL);
		sprintf(tmp_para,"%s_SSID%d",((band)?"5G":"2.4G"),i);
		mapi_ccfg_get_str( tid, section, tmp_para, p_wlan_wds_cfg->wds_peer_ssid[i], MAPI_CCFG_WLAN_SSID_MAX_LEN, NULL);
	}

	p_wlan_wds_cfg->band = band;
	
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
 *		Set WDS configuration
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_wds_cfg:	pointer to the buffer of data structure mapi_ccfg_wds_ap_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_wds_cfg is NULL.
 *
 * Note
 *		* related sections: wds
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_set_wlan_wds(int tid, mapi_ccfg_wlan_wds_t* p_wlan_wds_cfg)
{
	int	new_transc=0,i;
	char                section[]="wds";
	char tmp_para[64];
	unsigned char   band;  
	
	if ( p_wlan_wds_cfg == NULL ) {
        MID_TRACE("[%s] NULL p_wlan_wds_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if ( tid == 0 ) {
        if ( (tid=mapi_start_transc()) == MID_FAIL )
            return MID_FAIL;
        new_transc = 1;
    }

  	
 		
    // Sanity check
		band = p_wlan_wds_cfg->band;
		
    if ( band >= MAPI_CCFG_WLAN_MAX_BAND_NUM ) {
        MID_TRACE("[%s] band or apidx out of range!\n", __FUNCTION__);
        return MID_FAIL;
    }
    
    sprintf(tmp_para,"%s_ENABLE",(band)?"5G":"2.4G");
	mapi_ccfg_set_long( tid, section, tmp_para, p_wlan_wds_cfg->wds_enable);	
		
	for (i=0;i<MAPI_CCFG_WLAN_MAX_WDS;i++)
	{
		sprintf(tmp_para,"%s_EB%d",((band)?"5G":"2.4G"),i);
		mapi_ccfg_set_long( tid, section, tmp_para, p_wlan_wds_cfg->wds_peer_eb[i]);	
		sprintf(tmp_para,"%s_MAC%d",((band)?"5G":"2.4G"),i);
		mapi_ccfg_set_str( tid, section, tmp_para, p_wlan_wds_cfg->wds_peer_mac[i]);
		sprintf(tmp_para,"%s_SSID%d",((band)?"5G":"2.4G"),i);
		mapi_ccfg_set_str( tid, section, tmp_para, p_wlan_wds_cfg->wds_peer_ssid[i]);
		
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
 *		Get WMM configuration
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_wmm_cfg:	pointer to the buffer of data structure mapi_ccfg_wlan_wmm_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_wmm_cfg is NULL.
 *
 * Note
 *		
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_get_wlan_wmm(int tid, mapi_ccfg_wlan_wmm_t* p_wlan_wmm_cfg, int factory_dft)
{
	int	new_transc=0,i;
	char                section[]="wireless";
	char *p,wmm_string[MAPI_CCFG_WLAN_WMM_MAX_LEN];
	
    if ( p_wlan_wmm_cfg == NULL ) {
        MID_TRACE("[%s] NULL p_wlan_wmm_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if ( tid == 0 ) {
        if ( (tid=mapi_start_transc()) == MID_FAIL )
            return MID_FAIL;
        new_transc = 1;
    }
    
    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }

    
#define GET_WMM(string,variable)    do {mapi_ccfg_get_str( tid, section, string, wmm_string, MAPI_CCFG_WLAN_WMM_MAX_LEN, NULL);\
	  for (i = 0, p = strtok(wmm_string,";"); p && i<MAPI_CCFG_WLAN_WMM_AC_NUM; p = strtok(NULL,";"), i++)\
	  p_wlan_wmm_cfg->variable[i] = (unsigned short) strtol(p, NULL, 10);}while(0)

	//memset((char *)p_wlan_wmm_cfg, 0, sizeof(*p_wlan_wmm_cfg));	

		GET_WMM("APAifsn",ApAIFSN);   
		GET_WMM("APCwmin",ApCWmin);   
		GET_WMM("APCwmax",ApCWmax);   
		GET_WMM("APTxop",ApTxop);   
		GET_WMM("ApACM",ApACM);   
		GET_WMM("BSSAifsn",BSSAIFSN);   
		GET_WMM("BSSCwmin",BSSCWmin);   
		GET_WMM("BSSCwmax",BSSCWmax);   
		GET_WMM("BSSTxop",BSSTxop);   
		GET_WMM("BSSACM",BSSACM);   
	
	    
    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
    }

    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
#undef GET_WMM		

	return MID_SUCCESS;		
}


/*******************************************************************************
 * Description
 *		Set WMM configuration
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_wmm_cfg:	pointer to the buffer of data structure mapi_ccfg_wlan_wmm_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_wmm_cfg is NULL.
 *
 * Note
 *		* related sections: wireless
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_set_wlan_wmm(int tid, mapi_ccfg_wlan_wmm_t* p_wlan_wmm_cfg)
{
	int	new_transc=0;
	char                section[]="wireless";
	char tmp_para[64];
		
			  
	if ( p_wlan_wmm_cfg == NULL ) {
		MID_TRACE("[%s] NULL p_wlan_wmm_cfg\n", __FUNCTION__);
		return MID_FAIL;
	}

    if ( tid == 0 ) {
        if ( (tid=mapi_start_transc()) == MID_FAIL )
            return MID_FAIL;
        new_transc = 1;
    }

#define SET_WMM(string,variable) sprintf(tmp_para,"%d;%d;%d;%d",p_wlan_wmm_cfg->variable[0],\
		p_wlan_wmm_cfg->variable[1],p_wlan_wmm_cfg->variable[2],p_wlan_wmm_cfg->variable[3]);\
		mapi_ccfg_set_str( tid, section, string, tmp_para)	


		SET_WMM("APAifsn",ApAIFSN);   
		SET_WMM("APCwmin",ApCWmin);   
		SET_WMM("APCwmax",ApCWmax);   
		SET_WMM("APTxop",ApTxop);   
		SET_WMM("ApACM",ApACM);   
		SET_WMM("BSSAifsn",BSSAIFSN);   
		SET_WMM("BSSCwmin",BSSCWmin);   
		SET_WMM("BSSCwmax",BSSCWmax);   
		SET_WMM("BSSTxop",BSSTxop);   
		SET_WMM("BSSACM",BSSACM);   
	
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
#undef SET_WMM		
		return MID_SUCCESS;
}


/*******************************************************************************
 * Description
 *		Get ACL configuration
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_acl_cfg:	pointer to the buffer of data structure mapi_ccfg_wlan_acl_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_acl_cfg is NULL.
 *
 * Note
 *		
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_get_wlan_acl(int tid, mapi_ccfg_wlan_acl_t* p_wlan_acl_cfg, int factory_dft)
{
	int	new_transc=0,i;
	char                section[]="mac_filter";
	unsigned char   band;   
	unsigned char   ap_idx;
	char tmp_para[64];
	unsigned char v;
		
    if ( p_wlan_acl_cfg == NULL ) {
        MID_TRACE("[%s] NULL p_wlan_acl_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if ( tid == 0 ) {
        if ( (tid=mapi_start_transc()) == MID_FAIL )
            return MID_FAIL;
        new_transc = 1;
    }
    
    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }

	band = p_wlan_acl_cfg->band;
	ap_idx = p_wlan_acl_cfg->ap_idx;
    if ( band >= MAPI_CCFG_WLAN_MAX_BAND_NUM || ap_idx >= MAPI_CCFG_WLAN_MAX_SSID_NUM ) {
        MID_TRACE("[%s] band or apidx out of range!\n", __FUNCTION__);
        return MID_FAIL;
    }
	//memset((char *)p_wlan_acl_cfg, 0, sizeof(*p_wlan_acl_cfg));
		
		// Get Configuration
	sprintf(tmp_para,"mf_type_%d_%d",band,ap_idx);
	v = (unsigned char)mapi_ccfg_get_long( tid, section, tmp_para, 0);
	if (v) {p_wlan_acl_cfg->enable=1;p_wlan_acl_cfg->action=v;}
	else	p_wlan_acl_cfg->enable=p_wlan_acl_cfg->action=MAPI_CCFG_WLAN_ACL_NONE;
			
	for (i=0;i<MAPI_CCFG_WLAN_ACL_MAX_MAC_NUM;i++)	{
		sprintf(tmp_para,"mf_mac%d_%d_%d",i,band,ap_idx);
		mapi_ccfg_get_str( tid, section, tmp_para, p_wlan_acl_cfg->mac[i], MAPI_CCFG_WLAN_MAC_LEN, NULL);
	}
	
	p_wlan_acl_cfg->band = band;
	p_wlan_acl_cfg->ap_idx = ap_idx;
	    
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
 *		Set ACL configuration
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_acl_cfg:	pointer to the buffer of data structure mapi_ccfg_wlan_acl_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_acl_cfg is NULL.
 *
 * Note
 *		* related sections: mac_filter
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_set_wlan_acl(int tid, mapi_ccfg_wlan_acl_t* p_wlan_acl_cfg)
{
	int	new_transc=0,i;
	char                section[]="mac_filter";
	unsigned char   band;   
	unsigned char   ap_idx;
	char tmp_para[64];
	unsigned char v;
	
	if ( p_wlan_acl_cfg == NULL ) {
		MID_TRACE("[%s] NULL p_wlan_acl_cfg\n", __FUNCTION__);
		return MID_FAIL;
	}

    if ( tid == 0 ) {
        if ( (tid=mapi_start_transc()) == MID_FAIL )
            return MID_FAIL;
        new_transc = 1;
    }

		band = p_wlan_acl_cfg->band;
		ap_idx = p_wlan_acl_cfg->ap_idx;
    if ( band >= MAPI_CCFG_WLAN_MAX_BAND_NUM || ap_idx >= MAPI_CCFG_WLAN_MAX_SSID_NUM ) {
        MID_TRACE("[%s] band or apidx out of range!\n", __FUNCTION__);
        return MID_FAIL;
    }


    sprintf(tmp_para,"mf_type_%d_%d",band,ap_idx);
    v = p_wlan_acl_cfg->enable;
    if (v) mapi_ccfg_set_long( tid, section, tmp_para, p_wlan_acl_cfg->action);	
    else mapi_ccfg_set_long( tid, section, tmp_para, MAPI_CCFG_WLAN_ACL_NONE);	
    	
			
		for (i=0;i<MAPI_CCFG_WLAN_ACL_MAX_MAC_NUM;i++)	{
				sprintf(tmp_para,"mf_mac%d_%d_%d",i,band,ap_idx);
				mapi_ccfg_set_str( tid, section, tmp_para, p_wlan_acl_cfg->mac[i]);
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
 *		Get WLAN timer configuration
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_timer_cfg:	pointer to the buffer of data structure mapi_ccfg_wlan_timer_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_timer_cfg is NULL.
 *
 * Note
 *		Need to specify p_wlan_timer_cfg->band 
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_get_wlan_timer(int tid, mapi_ccfg_wlan_timer_t* p_wlan_timer_cfg, int factory_dft)
{
	int	new_transc=0,i,j;
	char                section[]="wireless_timer";
	char tmp_para[64];
	unsigned char rule_num;
	char *p;
	unsigned char   band;
		
    if ( p_wlan_timer_cfg == NULL ) {
        MID_TRACE("[%s] NULL p_wlan_timer_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if ( tid == 0 ) {
        if ( (tid=mapi_start_transc()) == MID_FAIL )
            return MID_FAIL;
        new_transc = 1;
    }
    
    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }

	// Sanity check
	band = p_wlan_timer_cfg->band;
			
	if ( band >= MAPI_CCFG_WLAN_MAX_BAND_NUM  ) {
		MID_TRACE("[%s] band or apidx out of range!\n", __FUNCTION__);
		return MID_FAIL;
	}
		
		
 			
	// Get Configuration
	sprintf(tmp_para,"%s_enable",(band)?"5G":"2.4G");
	p_wlan_timer_cfg->enable = (unsigned char)mapi_ccfg_get_long( tid, section, tmp_para, 0);

	sprintf(tmp_para,"%s_deactivate",(band)?"5G":"2.4G");	
	p_wlan_timer_cfg->action = !(unsigned char)mapi_ccfg_get_long( tid, section, tmp_para, 0);

	sprintf(tmp_para,"%s_rule_num",(band)?"5G":"2.4G");	
	rule_num = (unsigned char)mapi_ccfg_get_long( tid, section, tmp_para, 1);
	p_wlan_timer_cfg->rule_num = rule_num;
	
	for (i=0;i<rule_num;i++)
	{
		unsigned char timer_string[MAPI_CCFG_WLAN_TIMER_MAX_LEN];
		
		sprintf(tmp_para,"%s_run%d_day",(band)?"5G":"2.4G",i);
		mapi_ccfg_get_str( tid, section, tmp_para, timer_string, MAPI_CCFG_WLAN_TIMER_MAX_LEN, NULL);
		for (j=0,p = strtok(timer_string,":"); p; p = strtok(NULL,":"), j++){
			unsigned char day;
  		day = (unsigned char) strtol(p, NULL, 10);
			p_wlan_timer_cfg->rule[i].weekday_mask |= 1<<day;
		}

		sprintf(tmp_para,"%s_run%d_start",(band)?"5G":"2.4G",i);
		mapi_ccfg_get_str( tid, section, tmp_para, timer_string, MAPI_CCFG_WLAN_TIMER_MAX_LEN, NULL);

		for (j=0,p = strtok(timer_string,":"); p&&j<2; p = strtok(NULL,":"), j++){
			unsigned char v;
  		v = (unsigned char) strtol(p, NULL, 10);;
			if (j) p_wlan_timer_cfg->rule[i].start_min = v;
			else p_wlan_timer_cfg->rule[i].start_hour = v;
		}			
		
		sprintf(tmp_para,"%s_run%d_end",(band)?"5G":"2.4G",i);
		mapi_ccfg_get_str( tid, section, tmp_para, timer_string, MAPI_CCFG_WLAN_TIMER_MAX_LEN, NULL);

		for (j=0,p = strtok(timer_string,":"); p&&j<2; p = strtok(NULL,":"), j++){
			unsigned char v;
  		v = (unsigned char) strtol(p, NULL, 10);;
			if (j) p_wlan_timer_cfg->rule[i].end_min = v;
			else p_wlan_timer_cfg->rule[i].end_hour = v;
		}			
	}
	for (;i<MAPI_CCFG_WLAN_TIMER_MAX_RULE_NUM;i++)
	{
		memset(&p_wlan_timer_cfg->rule[i],0,sizeof(mapi_ccfg_wlan_timer_rule_t));
	}

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
 *		Set WLAN timer configuration
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_timer_cfg:	pointer to the buffer of data structure mapi_ccfg_wlan_timer_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_timer_cfg is NULL.
 *
 * Note
*		* Need to specify p_wlan_timer_cfg->band
 *		* related sections: wireless_timer
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_set_wlan_timer(int tid, mapi_ccfg_wlan_timer_t* p_wlan_timer_cfg)
{
	int	new_transc=0,i,j;
	char                section[]="wireless_timer";
	char tmp_para[64],tmp_string[64];
	unsigned char band;	

	if ( p_wlan_timer_cfg == NULL ) {
        MID_TRACE("[%s] NULL p_wlan_timer_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if ( tid == 0 ) {
        if ( (tid=mapi_start_transc()) == MID_FAIL )
            return MID_FAIL;
        new_transc = 1;
    }


	// Sanity check
	band = p_wlan_timer_cfg->band;
			
	if ( band >= MAPI_CCFG_WLAN_MAX_BAND_NUM  ) {
		MID_TRACE("[%s] band or apidx out of range!\n", __FUNCTION__);
		return MID_FAIL;
	}
		
		
	sprintf(tmp_string,"%s_enable",(band)?"5G":"2.4G");
	mapi_ccfg_set_long( tid, section, tmp_string, p_wlan_timer_cfg->enable);	

	sprintf(tmp_string,"%s_deactivate",(band)?"5G":"2.4G");
	mapi_ccfg_set_long( tid, section, tmp_string, !p_wlan_timer_cfg->action);	

	sprintf(tmp_string,"%s_rule_num",(band)?"5G":"2.4G");	
	mapi_ccfg_set_long( tid, section, tmp_string, p_wlan_timer_cfg->rule_num);	
	
	for (i=0;i<p_wlan_timer_cfg->rule_num;i++)
	{
		int weeklen=0;
		for (j=0;j<7;j++)
			if ((p_wlan_timer_cfg->rule[i].weekday_mask>>j)&1){
				sprintf(tmp_string+weeklen,"%d:",j);
				weeklen+=2;
			}

		if (weeklen)
			tmp_string[weeklen-1]=0;
		sprintf(tmp_para,"%s_run%d_day",(band)?"5G":"2.4G",i);
		mapi_ccfg_set_str( tid, section, tmp_para, tmp_string);
	  
		sprintf(tmp_para,"%s_run%d_start",(band)?"5G":"2.4G",i);
		sprintf(tmp_string,"%02d:%02d",p_wlan_timer_cfg->rule[i].start_hour,p_wlan_timer_cfg->rule[i].start_min);
		mapi_ccfg_set_str( tid, section, tmp_para, tmp_string);

		sprintf(tmp_para,"%s_run%d_end",(band)?"5G":"2.4G",i);
		sprintf(tmp_string,"%02d:%02d",p_wlan_timer_cfg->rule[i].end_hour,p_wlan_timer_cfg->rule[i].end_min);
		mapi_ccfg_set_str( tid, section, tmp_para, tmp_string);		  
	}

	for (;i<MAPI_CCFG_WLAN_TIMER_MAX_RULE_NUM;i++)
	{
		// Clean String
	  sprintf(tmp_para,"%s_run%d_day",(band)?"5G":"2.4G",i);
	  mapi_ccfg_set_str( tid, section, tmp_para, "");
	  sprintf(tmp_para,"%s_run%d_start",(band)?"5G":"2.4G",i);
	  mapi_ccfg_set_str( tid, section, tmp_para, "");
	  sprintf(tmp_para,"%s_run%d_end",(band)?"5G":"2.4G",i);
	  mapi_ccfg_set_str( tid, section, tmp_para, "");		  
		
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
 *		Get WPS configuration
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_wps_cfg:	pointer to the buffer of data structure mapi_ccfg_wps_ap_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_wps_cfg is NULL.
 *
 * Note
 *		* Need to specify p_wlan_wps_cfg->band and p_wlan_wps_cfg->band
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_get_wlan_wps(int tid, mapi_ccfg_wlan_wps_t* p_wlan_wps_cfg, int factory_dft)
{
	int                 new_transc=0,i;
	char                section[]="wps";
	unsigned char   band,ap_idx;   
	char tmp_para[64];

    if ( p_wlan_wps_cfg == NULL ) {
        MID_TRACE("[%s] NULL p_wlan_wps_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if ( tid == 0 ) {
        if ( (tid=mapi_start_transc()) == MID_FAIL )
            return MID_FAIL;
        new_transc = 1;
    }
    
    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }
    
    
    
    // Sanity check
	band = p_wlan_wps_cfg->band;
	ap_idx  = p_wlan_wps_cfg->ap_idx;
		
	if ( band >= MAPI_CCFG_WLAN_MAX_BAND_NUM || ap_idx >= MAPI_CCFG_WLAN_MAX_SSID_NUM ) {
		MID_TRACE("[%s] band or apidx out of range!\n", __FUNCTION__);
		return MID_FAIL;
	}

	
	// Get Configuration
	sprintf(tmp_para,"enable_%d_%d",band,ap_idx);
	p_wlan_wps_cfg->enable = (unsigned char)mapi_ccfg_get_long( tid, section, tmp_para, 1);

	sprintf(tmp_para,"pinlen_%d_%d",band,ap_idx);
	p_wlan_wps_cfg->pin_len = (unsigned char)mapi_ccfg_get_long( tid, section, tmp_para, 8); 

	sprintf(tmp_para,"pin_%d_%d",band,ap_idx);
	mapi_ccfg_get_str( tid, section, tmp_para, p_wlan_wps_cfg->pincode, MAPI_CCFG_WLAN_WPS_PIN_MAX_LEN, "12345670");
	

	
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
 *		Set WPS configuration
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_wps_cfg:	pointer to the buffer of data structure mapi_ccfg_wps_ap_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_wps_cfg is NULL.
 *
 * Note
 *		* related sections: wps
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_set_wlan_wps(int tid, mapi_ccfg_wlan_wps_t* p_wlan_wps_cfg)
{
	int	new_transc=0,i;
	char                section[]="wps";
	char tmp_para[64];
	unsigned char   band,ap_idx;  
	
	if ( p_wlan_wps_cfg == NULL ) {
        MID_TRACE("[%s] NULL p_wlan_wps_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if ( tid == 0 ) {
        if ( (tid=mapi_start_transc()) == MID_FAIL )
            return MID_FAIL;
        new_transc = 1;
    }

  	
 		
    // Sanity check
	band = p_wlan_wps_cfg->band;
	ap_idx = p_wlan_wps_cfg->ap_idx;
		
	if ( band >= MAPI_CCFG_WLAN_MAX_BAND_NUM || ap_idx >= MAPI_CCFG_WLAN_MAX_SSID_NUM ) {
		MID_TRACE("[%s] band or apidx out of range!\n", __FUNCTION__);
		return MID_FAIL;
	}
    

	// Set Configuration
	sprintf(tmp_para,"enable_%d_%d",band,ap_idx);
	mapi_ccfg_set_long( tid, section, tmp_para, p_wlan_wps_cfg->enable);		

	sprintf(tmp_para,"pinlen_%d_%d",band,ap_idx);
	mapi_ccfg_set_long( tid, section, tmp_para, p_wlan_wps_cfg->pin_len);			

	sprintf(tmp_para,"pin_%d_%d",band,ap_idx);
	mapi_ccfg_set_str( tid, section, tmp_para, p_wlan_wps_cfg->pincode);

		
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
		
	return MID_SUCCESS;

	
}




/*******************************************************************************
 * Description
 *		Get WLAN global configuration
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_glb:	pointer to the buffer of data structure mapi_ccfg_wlan_global_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_glb is NULL.
 *
 * Note
 *		
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_get_wlan_global(int tid, mapi_ccfg_wlan_global_t* p_wlan_glb, int factory_dft)
{
	int new_transc=0;
	char				  section[]="wireless";
	
	if ( p_wlan_glb == NULL ) {
        MID_TRACE("[%s] NULL p_wlan_glb\n", __FUNCTION__);
        return MID_FAIL;
    }

	if ( tid == 0 ) {
		if ( (tid=mapi_start_transc()) == MID_FAIL )
			return MID_FAIL;
		new_transc = 1;
	}

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }
	p_wlan_glb->supported_band_num = (unsigned char)mapi_ccfg_get_long( tid, section, "SUPPORTED_BAND", 1);

	p_wlan_glb->band0_max_ap_num = (unsigned char)mapi_ccfg_get_long( tid, section, "2.4G_AP_NUM", 1);
	p_wlan_glb->band0_enable = (unsigned char)mapi_ccfg_get_long( tid, section, "2.4G_WIRELESS_ENABLE", 1);

#ifdef MAPI_CCFG_WLAN_CONCURRENT_DUAL_BAND_SUPPORT
	p_wlan_glb->band1_max_ap_num = (unsigned char)mapi_ccfg_get_long( tid, section, "5G_AP_NUM", 1);
    p_wlan_glb->band1_enable = (unsigned char)mapi_ccfg_get_long( tid, section, "5G_WIRELESS_ENABLE", 1);
#endif

	p_wlan_glb->button_act = (unsigned char)mapi_ccfg_get_long( tid, section, "BUTTON_ACT", 0);




    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
    }
	if(new_transc) {
		if(mapi_end_transc(tid)==MID_FAIL)
			return MID_FAIL;
	}    

	return MID_SUCCESS;
}



/*******************************************************************************
 * Description
 *		Set WLAN global configuration
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_glb:	pointer to the buffer of data structure mapi_ccfg_wlan_global_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_glb is NULL.
 *
 * Note
 *		
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_set_wlan_global(int tid, mapi_ccfg_wlan_global_t* p_wlan_glb)
{
	int new_transc=0;
	char				  section[]="wireless";
	
	if ( p_wlan_glb == NULL ) {
        MID_TRACE("[%s] NULL p_wlan_glb\n", __FUNCTION__);
        return MID_FAIL;
	}

	if ( tid == 0 ) {
		if ( (tid=mapi_start_transc()) == MID_FAIL )
			return MID_FAIL;
		new_transc = 1;
	}

	mapi_ccfg_set_long( tid, section, "2.4G_WIRELESS_ENABLE", p_wlan_glb->band0_enable);
#ifdef MAPI_CCFG_WLAN_CONCURRENT_DUAL_BAND_SUPPORT
	mapi_ccfg_set_long( tid, section, "5G_WIRELESS_ENABLE", p_wlan_glb->band1_enable);
#endif
	mapi_ccfg_set_long( tid, section, "BUTTON_ACT", p_wlan_glb->button_act);	



	if(new_transc) {
		if(mapi_end_transc(tid)==MID_FAIL)
			return MID_FAIL;
	}	 

    return MID_SUCCESS;
}


//
// OUTPUT: pu8Key[], the buffer size must be 20 bytes
// INPUT:  pPassphrase, a char string with NULL terminated char.
//         Key_type, 1 = 40bit WEP, 2 = 128bit WEP
//
// void vGenerateWepKey (BYTE *pu8Key, char *pPassphrase, int key_len)
static void vGenerateWepKey (unsigned char *pu8Key, unsigned char *pPassphrase, int key_len)
{
    unsigned int   seed = 0;
    int     i, len;

    len = strlen (pPassphrase);

    if ( (key_len == 5) || (key_len == 10) ) {
        int j;

        /*generate seed for random number generator using key string */
        if ( len != 0 ) {
            unsigned char *pseed = (unsigned char *)&seed;
            for (i=0; i<len; i++ ) {
// Bug Fix
#if 1//BIG_ENDIAN
  //#error : Are You Sure ?
                pseed[3-(i%4)] ^= (unsigned char) pPassphrase[i];
#else
                pseed[i%4] ^= (unsigned char) pPassphrase[i];
#endif

            }
        }

        /* generate keys. */
        //changed to generate each of 4 keys, since keys not contiguous
        for (j = 0; j < 4; j++) {
            for (i=0; i<5 ;i++ )  {
                /* (((26*8+1)*256-1)*4+1) */
                seed = 214013 * seed + 0x269ec3;
                pu8Key[j*5 + i] = (seed >> 16) & 0x7fff;
            }
        }
    } else  if ( (key_len == 13) || (key_len == 26) ){
        // assume 104-bit key and use MD5 for passphrase munging
        st_MD5_CTX     MD;
        char*       cp;
        char        password_buf[65];
        int         i;
        unsigned char       key[16];

        // Initialize MD5 structures
        md5Init (&MD);

        // concatenate input passphrase repeatedly to fill password_buf
        cp = password_buf;
        for (i=0; i<64; i++)
            *cp++ = pPassphrase[i % len];

        // generate 128-bit signature using MD5
        md5Update (&MD, (unsigned char *)password_buf, 64);
        md5Final ((unsigned char *)key, &MD);

        //  copy 13 bytes (104 bits) of MG5 generated signature to
        //  default key 0 (id = 1)
        memset (pu8Key, 0, 20);
        memcpy (pu8Key, key, 13);
    }
}



#ifdef MAPI_CCFG_WLAN_PROFILE_SUPPORT
/*******************************************************************************
 * Description
 *		Get WLAN profile configuration
 *
 * Parameters
 *		tid:			    transaction ID returned by calling mapi_start_transc()
 *      p_wlan_prof_cfg:    pointer to the buffer of data structure mapi_ccfg_wlan_profile_t
 *      factory_dft:        use 1 to get factory-default values
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_ap_cfg is NULL.
 *
 * Note
 *		Need to specify p_wlan_prof_cfg->profile_idx
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_get_wlan_profile(int tid, mapi_ccfg_wlan_profile_t* p_wlan_prof_cfg, int factory_dft)
{
	int             new_transc=0,rst=MID_FAIL;
	char            section[32];
	char            ssid_para[32], hide_ssid_para[32], isolate_ssid_para[32];

    if(p_wlan_prof_cfg==NULL){
        MID_TRACE("[%s] NULL p_wlan_prof_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if(tid==0){
        if((tid=mapi_start_transc())==MID_FAIL)
            return MID_FAIL;
        new_transc=1;
    }
    
    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }

    sprintf(section, "wireless_profile_%d", p_wlan_prof_cfg->profile_idx);
    p_wlan_prof_cfg->enable=(unsigned char)mapi_ccfg_get_long(tid, section, "enable", (p_wlan_prof_cfg->profile_idx==0)? 1:0);
    p_wlan_prof_cfg->cover_bands=(unsigned char)mapi_ccfg_get_long(tid, section, "cover_bands", MAPI_CCFG_WLAN_PROFILE_COVER_DUAL);

    if(p_wlan_prof_cfg->cover_bands&MAPI_CCFG_WLAN_PROFILE_COVER_DUAL==0){    //should not be here
        p_wlan_prof_cfg->enable=0;
        p_wlan_prof_cfg->ssid[0]='\0';
        p_wlan_prof_cfg->ssid_hidden=0;
        p_wlan_prof_cfg->isolate_wlan_eb=0;		
		goto err;
    }   
    //ssid, hide_ssid, isolate_ssid
    sprintf(section, "wireless");
    if((p_wlan_prof_cfg->cover_bands & MAPI_CCFG_WLAN_PROFILE_COVER_24GHZ)){ //covers 2.4GHz (2.4GHz or 2.4GHz+5GHz), read the corresponding ssid settings in 2.4GHz

        if(p_wlan_prof_cfg->profile_idx==0){
            sprintf(ssid_para, "AP_SSID");
            sprintf(hide_ssid_para, "hide_ssid");
            sprintf(isolate_ssid_para, "isolate_ssid");
        }
        else{
            sprintf(ssid_para, "AP_SSID_%d", p_wlan_prof_cfg->profile_idx*2+1);
            sprintf(hide_ssid_para, "hide_ssid_%d", p_wlan_prof_cfg->profile_idx*2+1);
            sprintf(isolate_ssid_para, "isolate_ssid_%d", p_wlan_prof_cfg->profile_idx*2+1);            
        }
    }
    else if((p_wlan_prof_cfg->cover_bands & MAPI_CCFG_WLAN_PROFILE_COVER_5GHZ)){ //covers 5GHz only, read the corresponding ssid settings in 5GHz
        sprintf(ssid_para, "AP_SSID_%d", p_wlan_prof_cfg->profile_idx*2+2);
        sprintf(hide_ssid_para, "hide_ssid_%d", p_wlan_prof_cfg->profile_idx*2+2);
        sprintf(isolate_ssid_para, "isolate_ssid_%d", p_wlan_prof_cfg->profile_idx*2+2);            
    }

    mapi_ccfg_get_str(tid, section, ssid_para, p_wlan_prof_cfg->ssid, MAPI_CCFG_WLAN_SSID_MAX_LEN, NULL);
    p_wlan_prof_cfg->ssid_hidden = (unsigned char)mapi_ccfg_get_long( tid, section, hide_ssid_para, 0);
	p_wlan_prof_cfg->isolate_wlan_eb = (unsigned char)mapi_ccfg_get_long( tid, section, isolate_ssid_para, 0);
	rst = MID_SUCCESS;
err:
    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
    }
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

	return rst;
}

/*******************************************************************************
 * Description
 *		Set WLAN profile configuration
 *
 * Parameters
 *		tid:			    transaction ID returned by calling mapi_start_transc()
 *      p_wlan_prof_cfg:    pointer to the buffer of data structure mapi_ccfg_wlan_profile_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_ap_cfg is NULL.
 *
 * Note
 *		Need to specify p_wlan_prof_cfg->profile_idx
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_set_wlan_profile(int tid, mapi_ccfg_wlan_profile_t* p_wlan_prof_cfg)
{
	int             new_transc=0,rst=MID_FAIL;
	int             enable_ssid;
	char            section[32];
	char            ap_eb_para[32], ssid_para[32], hide_ssid_para[32], isolate_ssid_para[32];

    if(p_wlan_prof_cfg==NULL){
        MID_TRACE("[%s] NULL p_wlan_prof_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if(tid==0){
        if((tid=mapi_start_transc())==MID_FAIL)
            return MID_FAIL;
        new_transc=1;
    }

    if(p_wlan_prof_cfg->cover_bands&MAPI_CCFG_WLAN_PROFILE_COVER_DUAL==0){    //should not be here
        //p_wlan_prof_cfg->enable=0;
		goto err;
    }

    //2.4GHz ssid, hide_ssid, isolate_ssid
    sprintf(section, "wireless");
    if(p_wlan_prof_cfg->profile_idx==0){
        sprintf(ssid_para, "AP_SSID");		
        sprintf(hide_ssid_para, "hide_ssid");
        sprintf(isolate_ssid_para, "isolate_ssid");
    }
    else{
        sprintf(ssid_para, "AP_SSID_%d", p_wlan_prof_cfg->profile_idx*2+1);
        sprintf(hide_ssid_para, "hide_ssid_%d", p_wlan_prof_cfg->profile_idx*2+1);
        sprintf(isolate_ssid_para, "isolate_ssid_%d", p_wlan_prof_cfg->profile_idx*2+1);
        
    }
    mapi_ccfg_set_str( tid, section, ssid_para, p_wlan_prof_cfg->ssid);
    mapi_ccfg_set_long( tid, section, hide_ssid_para, (long)(p_wlan_prof_cfg->ssid_hidden));
	mapi_ccfg_set_long( tid, section, isolate_ssid_para, (long)(p_wlan_prof_cfg->isolate_wlan_eb));

    //enable or disable the 2.4GHz ssid
    if(p_wlan_prof_cfg->enable && (p_wlan_prof_cfg->cover_bands & MAPI_CCFG_WLAN_PROFILE_COVER_24GHZ))
        enable_ssid=1;
    else
        enable_ssid=0;
    sprintf(ap_eb_para, "MBSS_ENABLE_0_%d", p_wlan_prof_cfg->profile_idx);
    mapi_ccfg_set_long(tid, section, ap_eb_para, (long)enable_ssid);

    //5GHz ssid, hide_ssid, isolate_ssid
    sprintf(ssid_para, "AP_SSID_%d", p_wlan_prof_cfg->profile_idx*2+2);
    sprintf(hide_ssid_para, "hide_ssid_%d", p_wlan_prof_cfg->profile_idx*2+2);
    sprintf(isolate_ssid_para, "isolate_ssid_%d", p_wlan_prof_cfg->profile_idx*2+2);            
    mapi_ccfg_set_str( tid, section, ssid_para, p_wlan_prof_cfg->ssid);
    mapi_ccfg_set_long( tid, section, hide_ssid_para, (long)(p_wlan_prof_cfg->ssid_hidden));
	mapi_ccfg_set_long( tid, section, isolate_ssid_para, (long)(p_wlan_prof_cfg->isolate_wlan_eb));

    //enable or disable the 5GHz ssid
    if(p_wlan_prof_cfg->enable && (p_wlan_prof_cfg->cover_bands & MAPI_CCFG_WLAN_PROFILE_COVER_5GHZ))
        enable_ssid=1;
    else
        enable_ssid=0;
    sprintf(ap_eb_para, "MBSS_ENABLE_1_%d", p_wlan_prof_cfg->profile_idx);
    mapi_ccfg_set_long(tid, section, ap_eb_para, (long)enable_ssid);

    //set profile configuration
    sprintf(section, "wireless_profile_%d", p_wlan_prof_cfg->profile_idx);
    mapi_ccfg_set_long(tid, section, "enable", (long)(p_wlan_prof_cfg->enable));
    mapi_ccfg_set_long(tid, section, "cover_bands", (long)(p_wlan_prof_cfg->cover_bands));
	rst=MID_SUCCESS;
err:    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

	return rst;
}

/*******************************************************************************
 * Description
 *		Get WLAN profile security configuration
 *
 * Parameters
 *		tid:                transaction ID returned by calling mapi_start_transc()
 *      profile_idx:        index to the profile (0-based)
 *      p_wlan_sec_cfg:     pointer to the buffer of data structure mapi_ccfg_wlan_sec_t
 *      factory_dft:        use 1 to get factory-default values
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_sec_cfg is NULL.
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_get_wlan_prof_sec(int tid, int profile_idx, mapi_ccfg_wlan_sec_t* p_wlan_sec_cfg, int factory_dft)
{
	int             new_transc=0,rst;
	unsigned char   cover_bands;
	char            section[32];

    if(p_wlan_sec_cfg==NULL){
        MID_TRACE("[%s] NULL p_wlan_sec_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if(tid==0){
        if((tid=mapi_start_transc())==MID_FAIL)
            return MID_FAIL;
        new_transc=1;
    }
    
    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }

    sprintf(section, "wireless_profile_%d", profile_idx);
    cover_bands=(unsigned char)mapi_ccfg_get_long(tid, section, "cover_bands", MAPI_CCFG_WLAN_PROFILE_COVER_DUAL);

    if((cover_bands & MAPI_CCFG_WLAN_PROFILE_COVER_24GHZ)){ //covers 2.4GHz (2.4GHz or 2.4GHz+5GHz), read the corresponding security settings in 2.4GHz
        p_wlan_sec_cfg->band=MAPI_CCFG_WLAN_BAND_24GHZ;
        p_wlan_sec_cfg->ap_idx=profile_idx;
        rst=mapi_ccfg_get_wlan_sec(tid, p_wlan_sec_cfg, factory_dft);
    }
    else if((cover_bands & MAPI_CCFG_WLAN_PROFILE_COVER_5GHZ)){ //covers 5GHz only, read the corresponding security settings in 5GHz
        p_wlan_sec_cfg->band=MAPI_CCFG_WLAN_BAND_5GHZ;
        p_wlan_sec_cfg->ap_idx=profile_idx;
        rst=mapi_ccfg_get_wlan_sec(tid, p_wlan_sec_cfg, factory_dft);
    }
    else{   //should not be here
        rst=MID_FAIL;
        memset((char *)p_wlan_sec_cfg, 0, sizeof(mapi_ccfg_wlan_sec_t));
    }

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
    }
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

	return rst;
}

/*******************************************************************************
 * Description
 *		Set WLAN profile security configuration
 *
 * Parameters
 *		tid:                transaction ID returned by calling mapi_start_transc()
 *      profile_idx:        index to the profile (0-based)
 *      p_wlan_sec_cfg:     pointer to the buffer of data structure mapi_ccfg_wlan_sec_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_sec_cfg is NULL.
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_set_wlan_prof_sec(int tid, int profile_idx, mapi_ccfg_wlan_sec_t* p_wlan_sec_cfg)
{
	int             new_transc=0;
	//unsigned char   cover_bands;
	//char            section[32];

    if(p_wlan_sec_cfg==NULL){
        MID_TRACE("[%s] NULL p_wlan_sec_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if(tid==0){
        if((tid=mapi_start_transc())==MID_FAIL)
            return MID_FAIL;
        new_transc=1;
    }

    //sprintf(section, "wireless_profile_%d", profile_idx);
    //cover_bands=(unsigned char)mapi_ccfg_get_long(tid, section, "cover_bands", MAPI_CCFG_WLAN_PROFILE_COVER_DUAL);

    //2.4GHz security
    p_wlan_sec_cfg->band=MAPI_CCFG_WLAN_BAND_24GHZ;
    p_wlan_sec_cfg->ap_idx=profile_idx;
    mapi_ccfg_set_wlan_sec(tid, p_wlan_sec_cfg);    

    //5GHz security
    p_wlan_sec_cfg->band=MAPI_CCFG_WLAN_BAND_5GHZ;
    p_wlan_sec_cfg->ap_idx=profile_idx;
    mapi_ccfg_set_wlan_sec(tid, p_wlan_sec_cfg);
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

	return MID_SUCCESS;
}


/*******************************************************************************
 * Description
 *		Get WLAN profile WPS configuration
 *
 * Parameters
 *		tid:                transaction ID returned by calling mapi_start_transc()
 *      profile_idx:        index to the profile (0-based)
 *      p_wlan_wps_cfg:     pointer to the buffer of data structure p_wlan_wps_cfg
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_wps_cfg is NULL.
 *
 * See Also
 *
 ******************************************************************************/

int mapi_ccfg_get_wlan_prof_wps(int tid, int profile_idx, mapi_ccfg_wlan_wps_t* p_wlan_wps_cfg, int factory_dft)
{
	int             new_transc=0,rst;
	unsigned char   cover_bands;
	char            section[32];

    if(p_wlan_wps_cfg==NULL){
        MID_TRACE("[%s] NULL p_wlan_wps_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if(tid==0){
        if((tid=mapi_start_transc())==MID_FAIL)
            return MID_FAIL;
        new_transc=1;
    }
    
    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }

    sprintf(section, "wireless_profile_%d", profile_idx);
    cover_bands=(unsigned char)mapi_ccfg_get_long(tid, section, "cover_bands", MAPI_CCFG_WLAN_PROFILE_COVER_DUAL);

    if((cover_bands & MAPI_CCFG_WLAN_PROFILE_COVER_24GHZ)){ //covers 2.4GHz (2.4GHz or 2.4GHz+5GHz), read the corresponding security settings in 2.4GHz
        p_wlan_wps_cfg->band=MAPI_CCFG_WLAN_BAND_24GHZ;
        p_wlan_wps_cfg->ap_idx=profile_idx;
        rst=mapi_ccfg_get_wlan_wps(tid, p_wlan_wps_cfg, factory_dft);
    }
    else if((cover_bands & MAPI_CCFG_WLAN_PROFILE_COVER_5GHZ)){ //covers 5GHz only, read the corresponding security settings in 5GHz
        p_wlan_wps_cfg->band=MAPI_CCFG_WLAN_BAND_5GHZ;
        p_wlan_wps_cfg->ap_idx=profile_idx;
        rst=mapi_ccfg_get_wlan_wps(tid, p_wlan_wps_cfg, factory_dft);
    }
    else{   //should not be here
        rst=MID_FAIL;
        memset((char *)p_wlan_wps_cfg, 0, sizeof(mapi_ccfg_wlan_wps_t));
    }

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
    }
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

	return rst;

}


/*******************************************************************************
 * Description
 *		Set WLAN profile WPS configuration
 *
 * Parameters
 *		tid:                transaction ID returned by calling mapi_start_transc()
 *      profile_idx:        index to the profile (0-based)
 *      p_wlan_wps_cfg:     pointer to the buffer of data structure p_wlan_wps_cfg
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_wps_cfg is NULL.
 *
 * See Also
 *
 ******************************************************************************/

int mapi_ccfg_set_wlan_prof_wps(int tid, int profile_idx, mapi_ccfg_wlan_wps_t* p_wlan_wps_cfg)
{
	int             new_transc=0;

    if(p_wlan_wps_cfg==NULL){
        MID_TRACE("[%s] NULL p_wlan_wps_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if(tid==0){
        if((tid=mapi_start_transc())==MID_FAIL)
            return MID_FAIL;
        new_transc=1;
    }

 
    //2.4GHz WPS
    p_wlan_wps_cfg->band=MAPI_CCFG_WLAN_BAND_24GHZ;
    p_wlan_wps_cfg->ap_idx=profile_idx;
    mapi_ccfg_set_wlan_wps(tid, p_wlan_wps_cfg);    

    //5GHz WPS
    p_wlan_wps_cfg->band=MAPI_CCFG_WLAN_BAND_5GHZ;
    p_wlan_wps_cfg->ap_idx=profile_idx;
    mapi_ccfg_set_wlan_wps(tid, p_wlan_wps_cfg);

#ifdef MAPI_CCFG_WLAN_WPS_MUTUALLY_EXCLUSIVE	
	if (p_wlan_wps_cfg->enable == 1) {

	//2.4GHz WPS
	p_wlan_wps_cfg->band=MAPI_CCFG_WLAN_BAND_24GHZ;
	p_wlan_wps_cfg->ap_idx=(profile_idx)?0:1;
	p_wlan_wps_cfg->enable=0 ;
	mapi_ccfg_set_wlan_wps(tid, p_wlan_wps_cfg);	

	//5GHz WPS
	p_wlan_wps_cfg->band=MAPI_CCFG_WLAN_BAND_5GHZ;
	p_wlan_wps_cfg->ap_idx=(profile_idx)?0:1;
	p_wlan_wps_cfg->enable=0 ;
	mapi_ccfg_set_wlan_wps(tid, p_wlan_wps_cfg);
	}

#endif

    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

	return MID_SUCCESS;

}
#if 0
int mapi_ccfg_get_wlan_prof_timer(int tid, int profile_idx, mapi_ccfg_wlan_timer_t* p_wlan_timer_cfg, int factory_dft)
{
	int             new_transc=0,rst;
	unsigned char   cover_bands;
	char            section[32];

    if(p_wlan_timer_cfg==NULL){
        MID_TRACE("[%s] NULL p_wlan_timer_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if(tid==0){
        if((tid=mapi_start_transc())==MID_FAIL)
            return MID_FAIL;
        new_transc=1;
    }
    
    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }

    sprintf(section, "wireless_profile_%d", profile_idx);
    cover_bands=(unsigned char)mapi_ccfg_get_long(tid, section, "cover_bands", MAPI_CCFG_WLAN_PROFILE_COVER_DUAL);

    if((cover_bands & MAPI_CCFG_WLAN_PROFILE_COVER_24GHZ)){ //covers 2.4GHz (2.4GHz or 2.4GHz+5GHz), read the corresponding security settings in 2.4GHz
        p_wlan_timer_cfg->band=MAPI_CCFG_WLAN_BAND_24GHZ;
        p_wlan_timer_cfg->ap_idx=profile_idx;
        rst=mapi_ccfg_get_wlan_timer(tid, p_wlan_timer_cfg, factory_dft);
    }
    else if((cover_bands & MAPI_CCFG_WLAN_PROFILE_COVER_5GHZ)){ //covers 5GHz only, read the corresponding security settings in 5GHz
        p_wlan_timer_cfg->band=MAPI_CCFG_WLAN_BAND_5GHZ;
        p_wlan_timer_cfg->ap_idx=profile_idx;
        rst=mapi_ccfg_get_wlan_timer(tid, p_wlan_timer_cfg, factory_dft);
    }
    else{   //should not be here
        rst=MID_FAIL;
        memset((char *)p_wlan_timer_cfg, 0, sizeof(mapi_ccfg_wlan_timer_t));
    }

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
    }
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

	return rst;


}

int mapi_ccfg_set_wlan_prof_timer(int tid, int profile_idx, mapi_ccfg_wlan_timer_t* p_wlan_timer_cfg)
{
	int             new_transc=0;

    if(p_wlan_timer_cfg==NULL){
        MID_TRACE("[%s] NULL p_wlan_timer_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if(tid==0){
        if((tid=mapi_start_transc())==MID_FAIL)
            return MID_FAIL;
        new_transc=1;
    }


    //2.4GHz timer
    p_wlan_timer_cfg->band=MAPI_CCFG_WLAN_BAND_24GHZ;
    p_wlan_timer_cfg->ap_idx=profile_idx;
    mapi_ccfg_set_wlan_timer(tid, p_wlan_timer_cfg);    

    //5GHz timer
    p_wlan_timer_cfg->band=MAPI_CCFG_WLAN_BAND_5GHZ;
    p_wlan_timer_cfg->ap_idx=profile_idx;
    mapi_ccfg_set_wlan_timer(tid, p_wlan_timer_cfg);
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

	return MID_SUCCESS;

}
#endif

/*******************************************************************************
 * Description
 *		Get WLAN profile ACL configuration
 *
 * Parameters
 *		tid:                transaction ID returned by calling mapi_start_transc()
 *      profile_idx:        index to the profile (0-based)
 *      p_wlan_acl_cfg:     pointer to the buffer of data structure p_wlan_acl_cfg
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_acl_cfg is NULL.
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_get_wlan_prof_acl(int tid, int profile_idx, mapi_ccfg_wlan_acl_t* p_wlan_acl_cfg, int factory_dft)
{
	int             new_transc=0,rst;
	unsigned char   cover_bands;
	char            section[32];

    if(p_wlan_acl_cfg==NULL){
        MID_TRACE("[%s] NULL p_wlan_acl_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if(tid==0){
        if((tid=mapi_start_transc())==MID_FAIL)
            return MID_FAIL;
        new_transc=1;
    }
    
    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }

    sprintf(section, "wireless_profile_%d", profile_idx);
    cover_bands=(unsigned char)mapi_ccfg_get_long(tid, section, "cover_bands", MAPI_CCFG_WLAN_PROFILE_COVER_DUAL);

    if((cover_bands & MAPI_CCFG_WLAN_PROFILE_COVER_24GHZ)){ //covers 2.4GHz (2.4GHz or 2.4GHz+5GHz), read the corresponding security settings in 2.4GHz
        p_wlan_acl_cfg->band=MAPI_CCFG_WLAN_BAND_24GHZ;
        p_wlan_acl_cfg->ap_idx=profile_idx;
        rst=mapi_ccfg_get_wlan_acl(tid, p_wlan_acl_cfg, factory_dft);
    }
    else if((cover_bands & MAPI_CCFG_WLAN_PROFILE_COVER_5GHZ)){ //covers 5GHz only, read the corresponding security settings in 5GHz
        p_wlan_acl_cfg->band=MAPI_CCFG_WLAN_BAND_5GHZ;
        p_wlan_acl_cfg->ap_idx=profile_idx;
        rst=mapi_ccfg_get_wlan_acl(tid, p_wlan_acl_cfg, factory_dft);
    }
    else{   //should not be here
        rst=MID_FAIL;
        memset((char *)p_wlan_acl_cfg, 0, sizeof(mapi_ccfg_wlan_acl_t));
    }

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
    }
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

	return rst;


}


/*******************************************************************************
 * Description
 *		Set WLAN profile ACL configuration
 *
 * Parameters
 *		tid:                transaction ID returned by calling mapi_start_transc()
 *      profile_idx:        index to the profile (0-based)
 *      p_wlan_acl_cfg:     pointer to the buffer of data structure p_wlan_acl_cfg
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_acl_cfg is NULL.
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_set_wlan_prof_acl(int tid, int profile_idx, mapi_ccfg_wlan_acl_t* p_wlan_acl_cfg)
{
	int             new_transc=0;

    if(p_wlan_acl_cfg==NULL){
        MID_TRACE("[%s] NULL p_wlan_acl_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }

    if(tid==0){
        if((tid=mapi_start_transc())==MID_FAIL)
            return MID_FAIL;
        new_transc=1;
    }


    //2.4GHz acl
    p_wlan_acl_cfg->band=MAPI_CCFG_WLAN_BAND_24GHZ;
    p_wlan_acl_cfg->ap_idx=profile_idx;
    mapi_ccfg_set_wlan_acl(tid, p_wlan_acl_cfg);    

    //5GHz acl
    p_wlan_acl_cfg->band=MAPI_CCFG_WLAN_BAND_5GHZ;
    p_wlan_acl_cfg->ap_idx=profile_idx;
    mapi_ccfg_set_wlan_acl(tid, p_wlan_acl_cfg);
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

	return MID_SUCCESS;

}


/*******************************************************************************
 * Description
 *		Set WLAN profile wep random key
 *
 * Parameters
 *		tid:                transaction ID returned by calling mapi_start_transc()
 *      profile_idx:        index to the profile (0-based)
 *      p_wlan_sec_cfg:     pointer to the buffer of data structure p_wlan_sec_cfg
 *	  p_passphrase, a char string with NULL terminated char.
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_sec_cfg is NULL.
 *
 * See Also
 *
 ******************************************************************************/


int mapi_ccfg_set_wlan_prof_wep_random_key(int tid, int profile_idx, unsigned char *p_passphrase ,mapi_ccfg_wlan_sec_t* p_wlan_sec_cfg)
{
	int             new_transc=0;
	mapi_ccfg_wlan_sec_t wlan_sec_cfg;
	int rst=MID_FAIL,i,j;
	unsigned char buf[20];
	//unsigned char   cover_bands;
	//char            section[32];

    if(p_wlan_sec_cfg==NULL){
        MID_TRACE("[%s] NULL p_wlan_sec_cfg\n", __FUNCTION__);
        return MID_FAIL;
    }
	
    if(p_passphrase==NULL){
        MID_TRACE("[%s] NULL p_passphrase\n", __FUNCTION__);
        return MID_FAIL;
    }
	

    if(tid==0){
        if((tid=mapi_start_transc())==MID_FAIL)
            return MID_FAIL;
        new_transc=1;
    }

	if (mapi_ccfg_get_wlan_prof_sec(tid, profile_idx, &wlan_sec_cfg, 0) == MID_FAIL ) goto err;

	// Check security first, must set to WEP before gen key, 
	// otherwise we dont know 64 or 128 to be generated.
	if (wlan_sec_cfg.sec_mode != MAPI_CCFG_WLAN_SEC_WEP ) goto err;
	if ( (wlan_sec_cfg.wep_key_len != 5) && (wlan_sec_cfg.wep_key_len != 10) &&
		  (wlan_sec_cfg.wep_key_len != 13) && (wlan_sec_cfg.wep_key_len != 26) )
		   goto err;

	vGenerateWepKey(buf,p_passphrase,wlan_sec_cfg.wep_key_len);
	if ( (wlan_sec_cfg.wep_key_len == 5) || (wlan_sec_cfg.wep_key_len == 10) ){
		for (i=0;i<4;i++) {
		 unsigned char *p=wlan_sec_cfg.wep_key[i];
		 for (j=0;j<5;j++,p+=2)
		  sprintf(p,"%02X",buf[i*5+j]);			 
		}
		//for (i=0;i<20;i++) printf("%02X",buf[i]);		
		//printf("\n");
		 	
		wlan_sec_cfg.wep_key_len = 10;
	}
	else {
		for (i=0;i<4;i++) {
		 unsigned char *p=wlan_sec_cfg.wep_key[i];
		 for (j=0;j<13;j++,p+=2)
		  sprintf(p,"%02X",buf[j]);			 
		}
		//for (i=0;i<13;i++) printf("%02X",buf[i]);		
		//printf("\n");
		
		wlan_sec_cfg.wep_key_len = 26;
	}

	memcpy(p_wlan_sec_cfg, &wlan_sec_cfg, sizeof(wlan_sec_cfg));


    //2.4GHz security
    p_wlan_sec_cfg->band=MAPI_CCFG_WLAN_BAND_24GHZ;
    p_wlan_sec_cfg->ap_idx=profile_idx;
    mapi_ccfg_set_wlan_sec(tid, p_wlan_sec_cfg);    

    //5GHz security
    p_wlan_sec_cfg->band=MAPI_CCFG_WLAN_BAND_5GHZ;
    p_wlan_sec_cfg->ap_idx=profile_idx;
    mapi_ccfg_set_wlan_sec(tid, p_wlan_sec_cfg);
    rst = MID_SUCCESS;
err:

    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
	return rst;

}

#endif

// For Calc Default PIN
static int atohi(char *in) {
        int i, j, me, retVar = 0, digi[10], maxDigi = strlen(in);
        char tmp[2];

        for (i = 0 ; i < maxDigi ; i++) {
            if ((in[i] == 'a') || (in[i] == 'A')) {
                     digi[i] = 10;
            }
            else if ((in[i] == 'b') || (in[i] == 'B')) {
                     digi[i] = 11;
            }
            else if ((in[i] == 'c') || (in[i] == 'C')) {
                    digi[i] = 12;
            }
            else if ((in[i] == 'd') || (in[i] == 'D')) {
                    digi[i] = 13;
            }
            else if ((in[i] == 'e') || (in[i] == 'E')) {
                    digi[i] = 14;
            }
			else if ((in[i] == 'f') || (in[i] == 'F')) {
					digi[i] = 15;
			}
			else {
					tmp[0] = in[i];
					tmp[1] = 0;
					digi[i] = atoi(tmp);
				}
		}
				
		for (i = 0 ; i < maxDigi ; i++) {
					me = 1;
				for (j = (i + 1) ; j < maxDigi ; j++) me = me * 16;
				retVar += digi[i] * me;
		}
				
		return retVar;
}

// For Calc Default PIN
static void calc_default_pin_code( const char *serialnumber, const char *macaddr, const char *wpakey, char *pin_code )
{
// the size of pin_code must be > 8
// the size of serialnumber must be > 10
// the size of wpakey must be > 10

    char buf[20] ;
    int i;

    char tm1[2] = {0, 0};
    int	len, s[12], m[16], k1, k2, x[4], y[4], z[4], w[10];

    unsigned long result, tmpi ;
    int ival, p[9];
    char ch ;

	MID_TRACE("[BUILD-PIN] PIN:%s, MAC:%s, Key:%s\n", serialnumber, macaddr, wpakey);

    // PIN Code for External Registrar
    // PIN ID: P1P2P3P4P5P6P7P8, (P1 ~ P8 is decimal digital)
    //
    // Mac:        0012BF-M7 M8 M9 M10 M11 M12
    // Serial No.: RYWWM-S6 S7 S8 S9 S10
    // WPA Key   : A0 A1 A2 A3 A4 A5 A6 A7 A8
    //			   W0 = A0%16, W1 = A1%16, ... W8 = A8%16
    //			   W9 = W0+W1+W2+W3+W4+W5+W6+W7+W8
    //
    // K1 = Last byte of (S7+S8+M11+M12) (e.g. R6263-12345, 0012BF-ABCDEF --> 2+3+E+F=22 --> 2)
    // K2 = Last byte of (M9+M10+S9+S10) (e.g. R6263-12345, 0012BF-ABCDEF --> C+D+4+5=22 --> 2)
    //	 if (wpakey != NULL)  // consider WPA Key is inputted during production
    //     (X1X2X3):3 digits (K1 XOR S10) AND W1,  (K1 XOR S9) OR W3,  (K1 XOR S8) XOR W5
    //     (Y1Y2Y3):3 digits (K2 XOR M10),  (K2 XOR M11) XOR W9, (K2 XOR M12)
    //     (Z1Z2Z3):3 digits (M11 XOR S10), (M12 XOR S9), (K1 XOR K2)   (e.g.  (E^5), (F^4), (2^2))
    //
	//	 else // original design, for the device with old boot loader
	//
    //     (X1X2X3):3 digits (K1 XOR S10),  (K1 XOR S9),  (K1 XOR S8)   (e.g.  (2^5), (2^4), (2^3))
    //     (Y1Y2Y3):3 digits (K2 XOR M10),  (K2 XOR M11), (K2 XOR M12)  (e.g.  (2^D), (2^E), (2^F))
    //     (Z1Z2Z3):3 digits (M11 XOR S10), (M12 XOR S9), (K1 XOR K2)   (e.g.  (E^5), (F^4), (2^2))
    //
    // P1~7 = (X1 X2 Y1 Y2 Z1 Z2 X3) mod 10000000
    //      = P1 P2 P3 P4 P5 P6 P7
    // P8 = [ 10 -(P1 x 3 + P2 + P3 x 3 + P4 + P5 x 3 + P6 + P7 x 3) mod 10 ] mod 10

    for (i = 6 ; i <= 10 ; i++) {
    	tm1[0] = serialnumber[i - 1];
    	s[i] = atohi(tm1);
    }
    //MID_TRACE("[BUILD-PIN] S6,7,8,9,10:[%1X,%1X,%1X,%1X,%1X]\n", s[6], s[7], s[8], s[9], s[10]);
    
    for (i = 7 ; i <= 12 ; i++) {
    	tm1[0] = macaddr[i - 1];
    	m[i] = atohi(tm1);
    }
    
    if (wpakey) {
    	w[9] = 0;
    	for (i=0; i<9; i++) {
    		w[i] = (unsigned char)wpakey[i] % 16;
    		//MID_TRACE("wpakey[%d] = %1X, w[%d] = %1X\n", i, wpakey[i], i, w[i]);
    		w[9]+=w[i];
    	}
    }
    //MID_TRACE("[BUILD-PIN] W9:[%1X]\n", w[9]);
    
    //MID_TRACE("[BUILD-PIN] M7,8,9,10,11,12:[%1X,%1X,%1X,%1X,%1X,%1X]\n", m[7], m[8], m[9], m[10], m[11], m[12]);

	k1 = (char)((s[7] + s[8] + m[11] + m[12]) % 16);
	k2 = (char)((m[9] + m[10] + s[9] + s[10]) % 16);
    //MID_TRACE("[BUILD-PIN] K1,2:[%1X,%1X]\n", k1, k2);

	if (wpakey) {
	    x[1] = (k1 ^ s[10]) & w[1];
	    x[2] = (k1 ^ s[9])  | w[3];
	    x[3] = (k1 ^ s[8])  ^ w[5];
	    //MID_TRACE("[BUILD-PIN] (K1 XOR S10)=(%1X XOR %1X)=%1X\n", k1, s[10], x[1]);
	    //MID_TRACE("[BUILD-PIN] (K1 XOR S9) =(%1X XOR %1X)=%1X\n", k1, s[9],  x[2]);
	    //MID_TRACE("[BUILD-PIN] (K1 XOR S8) =(%1X XOR %1X)=%1X\n", k1, s[8],  x[3]);
	    //MID_TRACE("[BUILD-PIN] (X1 X2 X3): [%1X, %1X, %1X]\n", x[1], x[2], x[3]);
	
	    y[1] = (k2 ^ m[10]);
	    y[2] = (k2 ^ m[11]) ^ w[9];
	    y[3] = (k2 ^ m[12]);
	    //MID_TRACE("[BUILD-PIN] (K2 XOR M10)=(%1X XOR %1X)=%1X\n", k2, m[10], y[1]);
	    //MID_TRACE("[BUILD-PIN] (K2 XOR M11)=(%1X XOR %1X)=%1X\n", k2, m[11], y[2]);
	    //MID_TRACE("[BUILD-PIN] (K2 XOR M12)=(%1X XOR %1X)=%1X\n", k2, m[12], y[3]);
	    //MID_TRACE("[BUILD-PIN] (Y1 Y2 Y3): [%1X, %1X, %1X]\n", y[1], y[2], y[3]);
	}
	else {
	    x[1] = (k1 ^ s[10]);
	    x[2] = (k1 ^ s[9]);
	    x[3] = (k1 ^ s[8]);
	    //MID_TRACE("[BUILD-PIN] (K1 XOR S10)=(%1X XOR %1X)=%1X\n", k1, s[10], x[1]);
	    //MID_TRACE("[BUILD-PIN] (K1 XOR S9) =(%1X XOR %1X)=%1X\n", k1, s[9],  x[2]);
	    //MID_TRACE("[BUILD-PIN] (K1 XOR S8) =(%1X XOR %1X)=%1X\n", k1, s[8],  x[3]);
	    //MID_TRACE("[BUILD-PIN] (X1 X2 X3): %1X%1X%1X\n", x[1], x[2], x[3]);
	
	    y[1] = (k2 ^ m[10]);
	    y[2] = (k2 ^ m[11]);
	    y[3] = (k2 ^ m[12]);
	    //MID_TRACE("[BUILD-PIN] (K2 XOR M10)=(%1X XOR %1X)=%1X\n", k2, m[10], y[1]);
	    //MID_TRACE("[BUILD-PIN] (K2 XOR M11)=(%1X XOR %1X)=%1X\n", k2, m[11], y[2]);
	    //MID_TRACE("[BUILD-PIN] (K2 XOR M12)=(%1X XOR %1X)=%1X\n", k2, m[12], y[3]);
	    //MID_TRACE("[BUILD-PIN] (Y1 Y2 Y3): %1X%1X%1X\n", y[1], y[2], y[3]);
	}

    z[1] = (m[11] ^ s[10]);
    z[2] = (m[12] ^ s[9]);
    z[3] = (k1 ^ k2);
    //MID_TRACE("[BUILD-PIN] (M11 XOR S10)=(%1X XOR %1X)=%1X\n", m[11], s[10], z[1]);
    //MID_TRACE("[BUILD-PIN] (M12 XOR S9) =(%1X XOR %1X)=%1X\n", m[12], s[9],  z[2]);
    //MID_TRACE("[BUILD-PIN] (K1  XOR K2) =(%1X XOR %1X)=%1X\n", k1, k2, z[3]);
    //MID_TRACE("[BUILD-PIN] (Z1 Z2 Z3): [%1X, %1X, %1X]\n", z[1], z[2], z[3]);
    
    sprintf(buf, "%1X%1X%1X%1X%1X%1X%1X",
    		x[1], x[2], y[1], y[2], z[1], z[2], x[3]);
    //MID_TRACE("[BUILD-PIN]: buf:'%s'\n", buf);

    // convert hex to decimal, and mod 10000000
    result = 0;
    len = strlen(buf) ;
    for( i = len-1 ; i >= 0 ; i-- ){
        ch = buf[i]  ;
        ival = 0;
        if( ch >= 'a' && ch <= 'f' ) ival = 10 + ch - 'a' ;
        else if( ch >= 'A' && ch <= 'F' ) ival = 10 + ch - 'A';
        else if( ch >= '0' && ch <= '9' ) ival = ch - '0';

        tmpi = ival << (4*(len-i-1)) ;
        tmpi %= 10000000;
        result += tmpi ;
        result %= 10000000 ;
    }
    //MID_TRACE("[BUILD-PIN]: result=%d\n", result );

    tmpi = result ;
    for( i = 1 ; i <= 7 ; i++ ){
        p[8-i] = tmpi % 10 ;
        tmpi = (tmpi - p[8-i])/10 ;
        //MID_TRACE("p[%d]=%d\n", 8-i, p[8-i] );
        pin_code[8-i-1] = '0' + p[8-i];
    }

    p[8] = (10 - ((p[1]*3+p[2]+p[3]*3+p[4]+p[5]*3+p[6]+p[7]*3) % 10)) % 10 ;

    pin_code[7] = '0' + p[8];
    pin_code[8] = '\0';

}


/*******************************************************************************
 * Description
 *		Get and Set WPS default PIN
 *
 * Parameters
 *		tid:			transaction ID returned by calling mapi_start_transc()
 *      p_wlan_wps_cfg:	pointer to the buffer of data structure mapi_ccfg_wps_ap_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_wps_cfg is NULL.
 *
 * Note
 *		
 *
 * See Also
 *
 ******************************************************************************/


#define MAX_SN_LEN 13 //11+(sizeof"/n")+1

int mapi_ccfg_set_wlan_wps_default_pin(int tid )
{
	int                 new_transc=0,rst,len;
	unsigned char   band,ap_idx;   
	unsigned char   pincode[MAPI_CCFG_WLAN_WPS_PIN_MAX_LEN];
	mapi_ccfg_wlan_wps_t wlan_wps_cfg;
	char mac[MAPI_CCFG_WLAN_MAC_LEN]="743170EF0C44\n";
	char sn[MAX_SN_LEN]="R1530031409\n";
	char wpakey[MAPI_CCFG_WLAN_WPA_PASSPHRASE_KEY_LEN]="1234567890\n";
	char*			pBuf=NULL;
	char			sCmd[64];

    if ( tid == 0 ) {
        if ( (tid=mapi_start_transc()) == MID_FAIL )
            return MID_FAIL;
        new_transc = 1;
    }

	if ( ( pBuf=osMalloc( 1024 ) ) == NULL )
	{
			MID_TRACE("[%s] osMalloc fail\n", __FUNCTION__);
			goto fail;
	}


	sprintf(sCmd,"/usr/sbin/uboot_env.sh --get --name wpa_key 2>&-");	
	if ( osSystem_GetOutput( sCmd, pBuf, 1024 ) < 0 ){
		MID_TRACE("[%s] execute \"%s\" fail\n", __FUNCTION__,pBuf,sCmd);
		goto fail;	}
	if (pBuf[0]) memcpy(wpakey,pBuf, MIN(strlen(pBuf),MAPI_CCFG_WLAN_WPA_PASSPHRASE_KEY_LEN));	


	sprintf(sCmd,"/usr/sbin/uboot_env.sh --get --name serial 2>&-");	
	if ( osSystem_GetOutput( sCmd, pBuf, 1024 ) < 0 )	{
		MID_TRACE("[%s] execute \"%s\" fail\n", __FUNCTION__,pBuf,sCmd);
		goto fail;	}
	if (pBuf[0])
		if (strlen(pBuf)==MAX_SN_LEN-1)
		 memcpy(sn,pBuf, strlen(pBuf));	
	
	sprintf(sCmd,"/usr/sbin/uboot_env.sh --get --name ethaddr 2>&-");
	if ( osSystem_GetOutput( sCmd, pBuf, 1024 ) < 0 )	{
		MID_TRACE("[%s] execute \"%s\" fail\n", __FUNCTION__,pBuf,sCmd);
		goto fail;
	}
	if (pBuf[0]) {
		int i,j=0;
		for (i=0;i<MIN(MAPI_CCFG_WLAN_MAC_LEN,strlen(pBuf));i++)
		 if (pBuf[i]!=':')
		  mac[j++]=pBuf[i];
		
		mac[j]=0;
	}
	
	
	
    rst = MID_SUCCESS;

	//printf("SN=%sMAC=%sWPSKEY=%s",sn,mac,wpakey);
    calc_default_pin_code(sn, mac, wpakey, pincode);
	//printf("pin=%s",pincode);

	for (band=0 ; band<MAPI_CCFG_WLAN_MAX_BAND_NUM ; band++)
	 for (ap_idx=0 ; ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM ; ap_idx++)
	  {
	   wlan_wps_cfg.band=band;
	   wlan_wps_cfg.ap_idx=ap_idx;
	   rst = mapi_ccfg_get_wlan_wps(tid, &wlan_wps_cfg, 0);
	   if (rst == MID_FAIL) goto fail;
	   memcpy(wlan_wps_cfg.pincode,pincode,MAPI_CCFG_WLAN_WPS_PIN_MAX_LEN);
	   rst = mapi_ccfg_set_wlan_wps(tid, &wlan_wps_cfg);  
	   if (rst == MID_FAIL) goto fail;
	  }
			
	MID_TRACE("pincode=%s\n",pincode);	
fail:
	if (pBuf) osFree( pBuf );

    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
	return rst;

	
}



/*******************************************************************************
 * Description
 *		Get device default setting
 *
 * Parameters
 *		tid:				transaction ID returned by calling mapi_start_transc()
 *      p_wlan_device_dft:	pointer to the buffer of data structure mapi_ccfg_wps_ap_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or p_wlan_device_dft is NULL.
 *
 * Note
 *          this function is used to get default settings which were different from
 *          device to device for security issue.
 *
 * See Also
 *
 ******************************************************************************/

int mapi_ccfg_get_wlan_device_default(int tid, mapi_ccfg_wlan_device_dft_t* p_wlan_device_dft)
{
		int 				new_transc=0,rst,len;
		unsigned char	pincode[MAPI_CCFG_WLAN_WPS_PIN_MAX_LEN];
		mapi_ccfg_wlan_wps_t wlan_wps_cfg;
		char mac[MAPI_CCFG_WLAN_MAC_LEN]="743170EF0C44\n";
		char sn[MAX_SN_LEN]="R1530031409\n";
		char wpakey[MAPI_CCFG_WLAN_WPA_PASSPHRASE_KEY_LEN]="1234567890\n";
		char*			pBuf=NULL,*p;
		char			sCmd[64];
	
		if ( tid == 0 ) {
			if ( (tid=mapi_start_transc()) == MID_FAIL )
				return MID_FAIL;
			new_transc = 1;
		}
	
		if ( ( pBuf=osMalloc( 1024 ) ) == NULL )
		{
				MID_TRACE("[%s] osMalloc fail\n", __FUNCTION__);
				goto fail;
		}
	
	
		sprintf(sCmd,"/usr/sbin/uboot_env.sh --get --name wpa_key 2>&-");	
		if ( osSystem_GetOutput( sCmd, pBuf, 1024 ) < 0 ){
			MID_TRACE("[%s] execute \"%s\" fail\n", __FUNCTION__,pBuf,sCmd);
			goto fail;	}
		if (pBuf[0]) memcpy(wpakey,pBuf, MIN(strlen(pBuf),MAPI_CCFG_WLAN_WPA_PASSPHRASE_KEY_LEN));	
	
	
		sprintf(sCmd,"/usr/sbin/uboot_env.sh --get --name serial 2>&-"); 
		if ( osSystem_GetOutput( sCmd, pBuf, 1024 ) < 0 )	{
			MID_TRACE("[%s] execute \"%s\" fail\n", __FUNCTION__,pBuf,sCmd);
			goto fail;	}
		if (pBuf[0])
			if (strlen(pBuf)==MAX_SN_LEN-1)
			 memcpy(sn,pBuf, strlen(pBuf)); 
		
		sprintf(sCmd,"/usr/sbin/uboot_env.sh --get --name ethaddr 2>&-");
		if ( osSystem_GetOutput( sCmd, pBuf, 1024 ) < 0 )	{
			MID_TRACE("[%s] execute \"%s\" fail\n", __FUNCTION__,pBuf,sCmd);
			goto fail;
		}
		if (pBuf[0]) {
			int i,j=0;
			for (i=0;i<MIN(MAPI_CCFG_WLAN_MAC_LEN,strlen(pBuf));i++)
			 if (pBuf[i]!=':')
			  mac[j++]=pBuf[i];
			
			mac[j]=0;
		}
		

		
		rst = MID_SUCCESS;
	
		//printf("SN=%sMAC=%sWPSKEY=%s",sn,mac,wpakey);
		calc_default_pin_code(sn, mac, wpakey, pincode);
		//printf("pin=%s",pincode);

		p=strchr(wpakey,'\n');
		if (p) *p=0;
	
		memcpy(p_wlan_device_dft->pincode,pincode,MAPI_CCFG_WLAN_WPS_PIN_MAX_LEN);
		memcpy(p_wlan_device_dft->psk_passphrase,(unsigned char*)wpakey,MAPI_CCFG_WLAN_WPA_PASSPHRASE_KEY_LEN);

	fail:
		if (pBuf) osFree( pBuf );
	
		if(new_transc){
			if(mapi_end_transc(tid)==MID_FAIL){
				return MID_FAIL;
			}
		}
		return rst;
	
		

}




