/*
 * Arcadyan middle layer common configuration CLI
 *
 * Copyright 2010, Arcadyan Corporation
 * All Rights Reserved.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/wait.h>
#include "mid_mapi_trnx.h"
#include "mid_mapi_ccfg.h"

#include "mid_mapi_ccfgsal_wan.h"
#include "mid_mapi_ccfgsal_vlan.h"
#include "mid_mapi_ccfgsal_voip.h"
#include "mid_mapi_ccfgsal_qos.h"
#include "mid_mapi_ccfgsal_wlan.h"

int                 tid;
mid_ccfg_tuple_t    cfg_tuple;

/*
 * -- Helper functions --
 */

static int do_testWan(const char *str)
{
    mapi_ccfg_wan_global_t      wan_glb, ori_wan_glb;
    mapi_ccfg_wan_t             wan_cfg, ori_wan_cfg;
    mapi_ccfg_pvc_t             pvc_cfg, ori_pvc_cfg;

    memset((char *)&wan_glb, 0, sizeof(wan_glb));
    //get factory default wan global config
    if(mapi_ccfg_get_wan_global(tid, &wan_glb, 1)==MID_FAIL){
        printf("mapi_ccfg_get_wan_global fail\n");
        
        return 0;
    }
    printf("Factory default wan global settings:\n");
    printf("wan_type=%d\n", wan_glb.wan_type);
    printf("active_wan_type=%d\n", wan_glb.active_wan_type);
    printf("ppp_pass_thru=%d\n", wan_glb.ppp_pass_thru);
    printf("max_wan_num=%d\n", wan_glb.max_wan_num);
    printf("dns1=%d\n", (int)wan_glb.dns1);
    printf("dns2=%d\n", (int)wan_glb.dns2);

    memcpy((char *)&ori_wan_glb, (char *)&wan_glb, sizeof(wan_glb));
        
    //get wan global config
    if(mapi_ccfg_get_wan_global(tid, &wan_glb, 0)==MID_FAIL){
        printf("mapi_ccfg_get_wan_global fail\n");
        
        return 0;
    }
    printf("Current wan global settings:\n");
    printf("wan_type=%d\n", wan_glb.wan_type);
    printf("active_wan_type=%d\n", wan_glb.active_wan_type);
    printf("ppp_pass_thru=%d\n", wan_glb.ppp_pass_thru);
    printf("max_wan_num=%d\n", wan_glb.max_wan_num);
    printf("dns1=%s\n", wan_glb.dns1);
    printf("dns2=%s\n", wan_glb.dns2);

    memcpy((char *)&ori_wan_glb, (char *)&wan_glb, sizeof(wan_glb));
    
    //set back
    if(mapi_ccfg_set_wan_global(tid, &wan_glb)==MID_FAIL){
        printf("mapi_ccfg_set_wan_global fail\n");
        
        return 0;
    }
    
    //get again
    memset((char *)&wan_glb, 0, sizeof(wan_glb));
    if(mapi_ccfg_get_wan_global(tid, &wan_glb, 0)==MID_FAIL){
        printf("mapi_ccfg_get_wan_global fail\n");
        
        return 0;
    }
    if(memcmp((char *)&ori_wan_glb, (char *)&wan_glb, sizeof(wan_glb))!=0){
        printf("something wrong in mapi_ccfg_get_wan_global or mapi_ccfg_set_wan_global\n");
        
        return 0;        
    }
    
    //get target wan
    memset((char *)&wan_cfg, 0, sizeof(wan_cfg));
    strcpy(wan_cfg.section, str);
    wan_cfg.wan_type=0;
    if(mapi_ccfg_get_wan(tid, &wan_cfg, 0)==MID_FAIL){
        printf("mapi_ccfg_get_wan fail\n");
        
        return 0;
    }
    printf("@%s (wan_type=%d, wan_idx=%d)\n", wan_cfg.section, wan_cfg.wan_type, wan_cfg.wan_idx);
    printf("adminstate=%d\n", wan_cfg.adminstate);
    printf("ifname=%s\n", wan_cfg.ifname);    
    printf("proto=%s\n", MAPI_CCFG_WAN_PROTO[wan_cfg.proto]);
    printf("defaultroute=%d\n", wan_cfg.defaultroute);
    printf("ip4addr=%s\n", wan_cfg.ip4addr);
    printf("ip4mask=%s\n", wan_cfg.ip4mask);
    printf("ip4gateway=%s\n", wan_cfg.ip4gateway);
    printf("dns=%s\n", wan_cfg.dns);
    printf("mtu=%d\n", wan_cfg.mtu);
    printf("username=%s\n", wan_cfg.username);
    printf("password=%s\n", wan_cfg.password);
    printf("on-demand=%d\n", wan_cfg.on_demand);
    printf("idletime=%d\n", wan_cfg.idletime);
    printf("disconnecttype=%s\n", (wan_cfg.disconnecttype==MAPI_CCFG_WAN_DISCONNECT_ACTIVE)? "active":"passive");
    printf("disconnecttime=%s\n", wan_cfg.disconnecttime);
    printf("ncp=%s\n", wan_cfg.ncp);
    printf("lcp_echo_fail=%d\n", wan_cfg.lcp_echo_fail);
    printf("macaddr=%s\n", wan_cfg.macaddr);
    printf("brvlan_idx=%d\n", wan_cfg.brvlan_idx);
    printf("vlan_tagged=%d\n", wan_cfg.vlan_tagged);
    printf("vlan_1p=%d\n", wan_cfg.vlan_1p[0]);    
    printf("vlan_id=%d\n", wan_cfg.vlan_id[0]);
    
    memcpy((char *)&ori_wan_cfg, (char *)&wan_cfg, sizeof(wan_cfg));

    //set back
    wan_cfg.section[0]='0';
    if(mapi_ccfg_set_wan(tid, &wan_cfg)==MID_FAIL){
        printf("mapi_ccfg_set_wan fail\n");
        
        return 0;
    }
    
    //get again
    memset((char *)&wan_cfg, 0, sizeof(wan_cfg));
    wan_cfg.wan_type=ori_wan_cfg.wan_type;
    wan_cfg.wan_idx=ori_wan_cfg.wan_idx;
    if(mapi_ccfg_get_wan(tid, &wan_cfg, 0)==MID_FAIL){
        printf("mapi_ccfg_get_wan fail\n");
        
        return 0;
    }
    if(memcmp((char *)&ori_wan_cfg, (char *)&wan_cfg, sizeof(wan_cfg))!=0){
        printf("something wrong in mapi_ccfg_get_wan or mapi_ccfg_set_wan\n");

        return 0;
    }

    //get target pvc
    if(wan_cfg.wan_type==MAPI_CCFG_WAN_TYPE_ADSL){
        memset((char *)&pvc_cfg, 0, sizeof(pvc_cfg));
        pvc_cfg.unit=wan_cfg.wan_idx;
        if(mapi_ccfg_get_pvc(tid, &pvc_cfg, 0)==MID_FAIL){
            printf("mapi_ccfg_get_pvc fail\n");
        
            return 0;
        }
        
        printf("@pvc%03d\n", pvc_cfg.unit);
        printf("vci=%d\n", pvc_cfg.vci);
        printf("vpi=%d\n", pvc_cfg.vpi);
        printf("encaps=%s\n", MAPI_CCFG_WAN_PVC_ENCAP[pvc_cfg.encaps]);
        if(pvc_cfg.qos>=0 && pvc_cfg.qos<MAPI_CCFG_WAN_PVC_QOS_NUM){
            printf("qos=%s\n", MAPI_CCFG_WAN_PVC_QOS[pvc_cfg.qos]);
            printf("pcr=%d\n", pvc_cfg.pcr);
            printf("scr=%d\n", pvc_cfg.scr);
            printf("mbs=%d\n", pvc_cfg.mbs);
        }
        else{
            printf("unknown qos parameters\n");
        }
        memcpy((char *)&ori_pvc_cfg, (char *)&pvc_cfg, sizeof(pvc_cfg));
        
        //set back
        if(mapi_ccfg_set_pvc(tid, &pvc_cfg)==MID_FAIL){
            printf("mapi_ccfg_set_pvc fail\n");
        
            return 0;
        }
        
        //get again
        memset((char *)&pvc_cfg, 0, sizeof(pvc_cfg));
        pvc_cfg.unit=wan_cfg.wan_idx;
        if(mapi_ccfg_get_pvc(tid, &pvc_cfg, 0)==MID_FAIL){
            printf("mapi_ccfg_get_pvc fail\n");
        
            return 0;
        }
        if(memcmp((char *)&ori_pvc_cfg, (char *)&pvc_cfg, sizeof(pvc_cfg))!=0){
            printf("something wrong in mapi_ccfg_get_pvc or mapi_ccfg_set_pvc\n");
        
            return 0;
        }
    }

    return 0;
}

static int do_testVlan(const char *str)
{
    int                         i;
    mapi_ccfg_vlan_global_t     vlan_glb;
    mapi_ccfg_vlan_t            vlan_cfg, ori_vlan_cfg;

    memset((char *)&vlan_glb, 0, sizeof(vlan_glb));
    //get vlan global config
    if(mapi_ccfg_get_vlan_global(tid, &vlan_glb, 0)==MID_FAIL){
        printf("mapi_ccfg_get_vlan_global fail\n");
        
        return 0;
    }
    printf("vlan global settings:\n");
    printf("enable=%d\n", vlan_glb.enable);
    printf("max_vlan_num=%d\n", vlan_glb.max_vlan_num);
    printf("active_vlan_num=%d\n", vlan_glb.active_vlan_num);
    printf("logical_port_num=%d\n", vlan_glb.logical_port_num);
    printf("logical_cpu_port=%d\n", vlan_glb.logical_cpu_port);
    printf("port_phyconf=%s\n", vlan_glb.port_phyconf);
    printf("lan_port=");
    for(i=0; i<MAPI_CCFG_VLAN_DFT_LAN_PORT_NUM; i++){
        printf("%d ", vlan_glb.lan_port[i]);
    }
    printf("\n");
    for(i=0; i<MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM; i++){
        printf("port_mask[%d]=0x%x\n", i, vlan_glb.port_mask[i]);
        printf("ssid_mask[%d]=0x%x\n", i, vlan_glb.ssid_mask[i]);
        printf("ethsw_vlanid_to_cpu[%d]=%d\n", i, vlan_glb.ethsw_vlanid_to_cpu[i]);
        printf("bind_to_brlan_id[%d]=%d\n", i, vlan_glb.bind_to_brlan_id[i]);
    }
    
    //get target vlan
    memset((char *)&vlan_cfg, 0, sizeof(vlan_cfg));
    vlan_cfg.vlan_idx=atoi(str);
    if(mapi_ccfg_get_vlan(tid, &vlan_cfg, 0)==MID_FAIL){
        printf("mapi_ccfg_get_vlan fail\n");
        
        return 0;
    }
    printf("vlan%d:\n", vlan_cfg.vlan_idx);
    //printf("vlan_name=%s\n", vlan_cfg.vlan_name);
    //printf("port_mask=0x%x\n", vlan_cfg.port_mask);
    //printf("ssid_mask=0x%x\n", vlan_cfg.ssid_mask);
    printf("brwan_mask=0x%x\n", vlan_cfg.brwan_mask);
    printf("bind_rtwan=%d\n", vlan_cfg.bind_rtwan);
    printf("vlan_tagged=%d\n", vlan_cfg.vlan_tagged);    
    printf("vlan_1p=%d\n", vlan_cfg.vlan_1p);
    printf("vlan_id=%d\n", vlan_cfg.vlan_id);
    printf("ip4addr=%s\n", vlan_cfg.ip4addr);
    printf("ip4mask=%s\n", vlan_cfg.ip4mask);
    printf("dhcp4s_eb=%d\n", vlan_cfg.dhcp4s_eb);
    printf("dhcp4pool_s=%s\n", vlan_cfg.dhcp4pool_s);
    printf("dhcp4pool_e=%s\n", vlan_cfg.dhcp4pool_e);
    printf("dhcp4lease=%s\n", vlan_cfg.dhcp4lease);
    printf("domainname=%s\n", vlan_cfg.domainname);
    printf("dhcp4_opt43_vendor_specific=%s\n", vlan_cfg.dhcp4_opt43_vendor_specific);
    printf("dhcp4_opt60_client_vendor_id=%s\n", vlan_cfg.dhcp4_opt60_client_vendor_id);
#ifdef MAPI_CCFG_VLAN_STATIC_DHCP
    for(i=0; i<MAPI_CCFG_VLAN_DHCP_MAX_STATIC_ENTRY; i++){
        printf("static_entry_eb[%d]=%d\n", i, vlan_cfg.static_entry_eb[i]);
        printf("static_ip4[%d]=%s\n", i, vlan_cfg.static_ip4[i]);
        printf("static_mac[%d]=%s\n", i, vlan_cfg.static_mac[i]);
    }
#endif
    
    memcpy((char *)&ori_vlan_cfg, (char *)&vlan_cfg, sizeof(vlan_cfg));

    //set back
    if(mapi_ccfg_set_vlan(tid, &vlan_cfg)==MID_FAIL){
        printf("mapi_ccfg_set_vlan fail\n");
        
        return 0;
    }
    
    //get again
    memset((char *)&vlan_cfg, 0, sizeof(vlan_cfg));
    vlan_cfg.vlan_idx=atoi(str);
    if(mapi_ccfg_get_vlan(tid, &vlan_cfg, 0)==MID_FAIL){
        printf("mapi_ccfg_get_vlan fail\n");
        
        return 0;
    }
    if(memcmp((char *)&ori_vlan_cfg, (char *)&vlan_cfg, sizeof(vlan_cfg))!=0){
        printf("something wrong in mapi_ccfg_get_vlan or mapi_ccfg_set_vlan\n");

        return 0;
    }

    return 0;
}

static int test_8021x(const char *str){
	 
	 mapi_ccfg_wlan_sec_t wlan_sec,ori_wlan_sec;
	 memset((char *)&wlan_sec, 0, sizeof(wlan_sec));
	 int i;
	 unsigned char key_buff[64];
    //get factory default wan global config
    
    wlan_sec.band =MAPI_CCFG_WLAN_BAND_24GHZ;
    wlan_sec.ap_idx=0;
    if(mapi_ccfg_get_wlan_sec(tid, &wlan_sec, 0)==MID_FAIL){
        printf("mapi_ccfg_get_wlan_sec fail\n");
        
        return 0;
    }

	memcpy(&ori_wlan_sec, &wlan_sec, sizeof(wlan_sec));
    
    printf("test_8021x===>\n");
    printf("sec_mod=%x\n",wlan_sec.sec_mode);
    printf("authen_mode=%x\n",wlan_sec.authen_mode);
     printf("wep_key_len=%x\n",wlan_sec.wep_key_len);
     printf("wep_dft_key_id=%x\n",wlan_sec.wep_dft_key_id);
     for(i=0;i<4;i++){
     	memset(key_buff,0,sizeof(key_buff));
     	memcpy(key_buff,&wlan_sec.wep_key[i][0],wlan_sec.wep_key_len);	
     	 printf("wep_key=%s\n",key_buff);
	}
	
	 printf("wpa_cypher_suite=%x\n",wlan_sec.wpa_cypher_suite);
	  printf("wpa2_cypher_suite=%x\n",wlan_sec.wpa2_cypher_suite);
	  printf("psk_key_input=%x\n",wlan_sec.psk_key_input);
	   printf("psk_passphrase=%s\n",wlan_sec.psk_passphrase);
	    printf("psk_256key=%s\n",wlan_sec.psk_256key);
	    printf("group_rekey_period=%x\n",wlan_sec.group_rekey_period);
	        printf("strict_update=%x\n",wlan_sec.strict_update);
	        
	        
	  printf("==============set new value===========\n");

	  wlan_sec.group_rekey_period=3600;

	  memset(&wlan_sec,sizeof(wlan_sec),0);
	  wlan_sec.sec_mode=MAPI_CCFG_WLAN_SEC_WEP;
	  memset(key_buff,0,sizeof(key_buff));
	  wlan_sec.wep_key_len = 13;
	  sprintf(key_buff,"%s","1234567890123");
	  strncpy(&wlan_sec.wep_key[0][0],key_buff,wlan_sec.wep_key_len);
	  sprintf(key_buff,"%s","1234567890124");
	  strncpy(&wlan_sec.wep_key[1][0],key_buff,wlan_sec.wep_key_len);
	  sprintf(key_buff,"%s","1234567890125");
	  strncpy(&wlan_sec.wep_key[2][0],key_buff,wlan_sec.wep_key_len);
	  sprintf(key_buff,"%s","1234567890126");
	  strncpy(&wlan_sec.wep_key[3][0],key_buff,wlan_sec.wep_key_len);
	  if(mapi_ccfg_set_wlan_sec(tid, &wlan_sec)==MID_FAIL){
        printf("mapi_ccfg_set_wlan fail\n");
        return 0;
      }


	  memset(&wlan_sec,sizeof(wlan_sec),0);
      if(mapi_ccfg_get_wlan_sec(tid, &wlan_sec, 0)==MID_FAIL){
        printf("mapi_ccfg_get_wlan_sec fail\n");
        return 0;
      }

  	  printf("==============read new value===========\n");
	   printf("wep_key_len=%x\n",wlan_sec.wep_key_len);
	   for(i=0;i<4;i++){
		  memset(key_buff,0,sizeof(key_buff));
		  memcpy(key_buff,&wlan_sec.wep_key[i][0],wlan_sec.wep_key_len);  
		   printf("wep_key=%s\n",key_buff);
	  }
	  



	// Restore 
	if(mapi_ccfg_set_wlan_sec(tid, &ori_wlan_sec)==MID_FAIL){
	  printf("mapi_ccfg_set_wlan fail\n");
	  return 0;
	}

	  memset(&wlan_sec,sizeof(wlan_sec),0);
      if(mapi_ccfg_get_wlan_sec(tid, &wlan_sec, 0)==MID_FAIL){
        printf("mapi_ccfg_get_wlan_sec fail\n");
        return 0;
      }	  

	 printf("==============restore old value===========\n");
     printf("wep_key_len=%x\n",wlan_sec.wep_key_len);
     for(i=0;i<4;i++){
     	memset(key_buff,0,sizeof(key_buff));
     	memcpy(key_buff,&wlan_sec.wep_key[i][0],wlan_sec.wep_key_len);	
     	 printf("wep_key=%s\n",key_buff);
	}
}

static int do_sync_voip_sipdata(const char *str)
{
    if(mapi_ccfg_sync_voip_SIP_Data(tid, atoi(str))==MID_FAIL){
        printf("mapi_ccfg_sync_voip_SIP_Data fail\n");
        
        return 0;
    }
    return 0;
}

static int do_testVoip(void)
{
    int                         i, j;
    mapi_ccfg_voip_global_t     voip_glb, ori_voip_glb;
    mapi_ccfg_voip_sip_line_t   voip_sip, ori_voip_sip;
    mapi_ccfg_voip_phone_t      voip_phone, ori_voip_phone;
    mapi_ccfg_voip_dial_rule_t  voip_dial_rule, ori_voip_dial_rule;
    mapi_ccfg_voip_isdn_t       voip_isdn, ori_voip_isdn;
#ifdef MAPI_CCFG_VOICE_UMTS_SUPPORT
    mapi_ccfg_voip_umts_t       voip_umts, ori_voip_umts;
#endif
	mapi_ccfg_voip_codec_info_t	voip_codec;

    //-----test global settings
    memset((char *)&voip_glb, 0, sizeof(voip_glb));
    //get voip global config
    if(mapi_ccfg_get_voip_global(tid, &voip_glb, 0)==MID_FAIL){
        printf("mapi_ccfg_get_voip_global fail\n");
        
        return 0;
    }
    printf("voip global settings:\n");
    printf("max_sip_line_num=%d\n", voip_glb.max_sip_line_num);
    printf("max_isdn_msn_num=%d\n", voip_glb.max_isdn_msn_num);
    printf("max_dial_rule_num=%d\n", voip_glb.max_dial_rule_num);
    printf("phone_num=%d\n", voip_glb.phone_num);
    //printf("speed_dial_eb=%d\n", voip_glb.speed_dial_eb);
    printf("t38_eb=%d\n", voip_glb.t38_eb);
    printf("pmtu_adjust_eb=%d\n", voip_glb.pmtu_adjust_eb);
    printf("bw_mn_eb=%d\n", voip_glb.bw_mn_eb);
    printf("rtp_port_min=%d\n", voip_glb.rtp_port_min);
    printf("rtp_port_max=%d\n", voip_glb.rtp_port_max);
    printf("hook_flash_min=%d\n", voip_glb.hook_flash_min);
    printf("hook_flash_max=%d\n", voip_glb.hook_flash_max);
    printf("pmtu_adjust_sz=%d\n", voip_glb.pmtu_adjust_sz);
    printf("rtp_frame_ms=%d\n", voip_glb.rtp_frame_ms);

    printf("sip_acc_num=%u\n", voip_glb.sip_acc_num);
    printf("sip_acc_max=%u\n", voip_glb.sip_acc_max);
    printf("line_max=%u\n", voip_glb.line_max);
    printf("sess_line_max=%u\n", voip_glb.sess_line_max);
    printf("sess_total_max=%u\n", voip_glb.sess_total_max);
    printf("signal_proto=%s\n", voip_glb.signal_proto);
    printf("regions=%s\n", voip_glb.regions);
    printf("capability_list_1=0x%04x\n", voip_glb.capability_list_1);
    printf("sip_role=%u\n", voip_glb.sip_role);
    printf("sip_ext=%u\n", voip_glb.sip_ext);
    printf("uri_schemes=%s\n", voip_glb.uri_schemes);
    printf("rtp_dscp_mark=%u\n", voip_glb.rtp_dscp_mark);
    printf("rtp_evt_payload_type=%u\n", voip_glb.rtp_evt_payload_type);
    printf("fax_t38_rate=%u\n", voip_glb.fax_t38_rate);

    //set back
    if(mapi_ccfg_set_voip_global(tid, &voip_glb)==MID_FAIL){
        printf("mapi_ccfg_set_voip_global fail\n");
        
        return 0;
    }
    memcpy(&ori_voip_glb, &voip_glb, sizeof(voip_glb));
    //get again
    mapi_ccfg_get_voip_global(tid, &voip_glb, 0);
    if(memcmp(&ori_voip_glb, &voip_glb, sizeof(voip_glb))){
        printf("compare voip_global fail\n");
        
        return 0;
    }
    
    //-----test sip account
    for(i=0; i<voip_glb.max_sip_line_num; i++){
        memset(&voip_sip, 0, sizeof(voip_sip));
        voip_sip.line_idx=i;
        if(mapi_ccfg_get_voip_sip(tid, &voip_sip, 0)==MID_FAIL){
            printf("mapi_ccfg_get_voip_sip fail\n");
        
            return 0;
        }
        printf("sip account %d\n", voip_sip.line_idx);
        printf("enable=%d\n", voip_sip.enable);
        //if(voip_sip.enable!=0)
        {
            unsigned char unselected_codec[MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM];

            printf("display_name=%s\n", voip_sip.display_name);
            printf("user_id=%s\n", voip_sip.user_id);
            printf("passwd=%s\n", voip_sip.passwd);
            printf("realm=%s\n", voip_sip.realm);
            printf("use_auth_id=%d\n", voip_sip.use_auth_id);
            printf("auth_id=%s\n", voip_sip.auth_id);
            printf("listen_port=%d\n", voip_sip.listen_port);
            printf("listen_wan=%d\n", voip_sip.listen_wan);
            printf("registrar=%s\n", voip_sip.registrar);
            printf("registrar_port=%d\n", voip_sip.registrar_port);
            printf("registrar_expire=%d ms\n", voip_sip.registrar_expire);
            printf("proxy=%s\n", voip_sip.proxy);
            printf("proxy_port=%d\n", voip_sip.proxy_port);
            printf("use_ob_proxy=%d\n", voip_sip.use_ob_proxy);
            printf("ob_proxy=%s\n", voip_sip.ob_proxy);
            printf("ob_proxy_port=%d\n", voip_sip.ob_proxy_port);
            printf("dtmf_tx_method=%d\n", voip_sip.dtmf_tx_method);
            printf("fax_passthru_codec=%d\n", voip_sip.fax_passthru_codec);
            printf("use_dnssrv=%d\n", voip_sip.use_dnssrv);
            printf("use_fetch_binding=%d\n", voip_sip.use_fetch_binding);

            printf("region=%s\n", voip_sip.region);
            printf("stun_eb=%u\n", voip_sip.stun_eb);
            printf("stun_server=%s\n", voip_sip.stun_server);
            printf("pstn_fail_over=%u\n", voip_sip.pstn_fail_over);
            printf("fax_pass_thru=%u\n", voip_sip.fax_pass_thru);
            printf("modem_pass_thru=%u\n", voip_sip.modem_pass_thru);
            printf("proxy_transport=%u\n", voip_sip.proxy_transport);
            printf("registrar_transport=%u\n", voip_sip.registrar_transport);
            printf("user_agent_transport=%u\n", voip_sip.user_agent_transport);
            printf("organization=%s\n", voip_sip.organization);
            printf("registrar_period=%u\n", voip_sip.registrar_period);

            printf("\nselected codec: ");
            for(j=0; j<MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM; j++){
                if(voip_sip.selected_codec_list[j]!=MAPI_CCFG_VOIP_CODEC_TYPE_NONE){
                    printf("%d ", voip_sip.selected_codec_list[j]);
                }
            }
            printf("\n");
            
            //test help APIs
            //add codec 5
            mapi_ccfg_select_voip_sip_codec(&voip_sip, 5);
            //add codec 2
            mapi_ccfg_select_voip_sip_codec(&voip_sip, 2);
            //add codec 20
            mapi_ccfg_select_voip_sip_codec(&voip_sip, 20);
            printf("after add codecs 5 2 20, available codec: ");
            mapi_ccfg_get_voip_sip_unselect_codec(&voip_sip, unselected_codec, MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM);
            for(j=0; j<MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM; j++){
                if(unselected_codec[j]!=MAPI_CCFG_VOIP_CODEC_TYPE_NONE){
                    printf("%d ", unselected_codec[j]);
                }
            }
            printf("\nselected codec: ");
            for(j=0; j<MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM; j++){
                if(voip_sip.selected_codec_list[j]!=MAPI_CCFG_VOIP_CODEC_TYPE_NONE){
                    printf("%d ", voip_sip.selected_codec_list[j]);
                }
            }
            printf("\n");            

            //remove codec 5
            mapi_ccfg_unselect_voip_sip_codec(&voip_sip, 5);
            //remove codec 2
            mapi_ccfg_unselect_voip_sip_codec(&voip_sip, 2);
            //remove codec 20
            mapi_ccfg_unselect_voip_sip_codec(&voip_sip, 20); 
            printf("after remove codecs 5 2 20, available codec: ");
            mapi_ccfg_get_voip_sip_unselect_codec(&voip_sip, unselected_codec, MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM);
            for(j=0; j<MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM; j++){
                if(unselected_codec[j]!=MAPI_CCFG_VOIP_CODEC_TYPE_NONE){
                    printf("%d ", unselected_codec[j]);
                }
            }
            printf("\nselected codec: ");
            for(j=0; j<MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM; j++){
                if(voip_sip.selected_codec_list[j]!=MAPI_CCFG_VOIP_CODEC_TYPE_NONE){
                    printf("%d ", voip_sip.selected_codec_list[j]);
                }
            }
            printf("\n");
            
            //new codec list
            {
                unsigned char new_codec[MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM]={1, 2, 3, 5, 0,};
                mapi_ccfg_update_sip_selected_codec(&voip_sip, new_codec, MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM);
                printf("after apply new codec list 1, 2, 3, 5, available codec: ");
                mapi_ccfg_get_voip_sip_unselect_codec(&voip_sip, unselected_codec, MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM);
                for(j=0; j<MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM; j++){
                    if(unselected_codec[j]!=MAPI_CCFG_VOIP_CODEC_TYPE_NONE){
                        printf("%d ", unselected_codec[j]);
                    }
                }
                printf("\nselected codec: ");
                for(j=0; j<MAPI_CCFG_VOIP_MAX_CONFIGURABLE_CODEC_NUM; j++){
                    if(voip_sip.selected_codec_list[j]!=MAPI_CCFG_VOIP_CODEC_TYPE_NONE){
                        printf("%d ", voip_sip.selected_codec_list[j]);
                    }
                }
                printf("\n");
            }                
                      
        }
        memcpy(&ori_voip_sip, &voip_sip, sizeof(voip_sip));
        //set back
        if(mapi_ccfg_set_voip_sip(tid, &voip_sip)==MID_FAIL){
            printf("mapi_ccfg_set_voip_sip fail\n");
        
            return 0;
        }
	  #if 0
        //get again
        mapi_ccfg_get_voip_sip(tid, &voip_sip, 0);
        if(memcmp(&ori_voip_sip, &voip_sip, sizeof(voip_sip))){
            printf("test sip account %d fail\n", i);
        
            return 0;
        }
	  #endif
    }

    //-----test isdn
    memset(&voip_isdn, 0, sizeof(voip_isdn));
    if(mapi_ccfg_get_voip_isdn(tid, &voip_isdn, 0)==MID_FAIL){
        printf("mapi_ccfg_get_voip_isdn fail\n");
        
        return 0;
    }
    printf("isdn area code=%s\n", voip_isdn.area_code);
    for(i=0; i<voip_glb.max_isdn_msn_num; i++){
        printf("isdn MSN%d=%s\n", i, voip_isdn.msn_number[i]);
    }
    memcpy(&ori_voip_isdn, &voip_isdn, sizeof(voip_isdn));
    //set back
    if(mapi_ccfg_set_voip_isdn(tid, &voip_isdn)==MID_FAIL){
        printf("mapi_ccfg_set_voip_isdn fail\n");
        
        return 0;
    }
    //get again
    mapi_ccfg_get_voip_isdn(tid, &voip_isdn, 0);
    if(memcmp(&ori_voip_isdn, &voip_isdn, sizeof(voip_isdn))){
        printf("test isdn cfg fail\n");
        
        return 0;
    }

    //-----test emergency dial rule
    for(i=0; i<voip_glb.max_dial_rule_num; i++){
        memset(&voip_dial_rule, 0, sizeof(voip_dial_rule));
        voip_dial_rule.emergency=1;
        voip_dial_rule.rule_idx=i;
        if(mapi_ccfg_get_voip_dial_rule(tid, &voip_dial_rule, 0)==MID_FAIL){
            printf("mapi_ccfg_get_voip_dial_rule fail\n");
        
            return 0;
        }
        printf("emergency rule %d\n", voip_dial_rule.rule_idx);
        printf("in_use=%d\n", voip_dial_rule.in_use);
        if(voip_dial_rule.in_use!=0){
            printf("phone_number=%s\n", voip_dial_rule.phone_number);
            printf("conn_type=%d\n", voip_dial_rule.conn_type);
            printf("comments=%s\n", voip_dial_rule.comments);
        }
        memcpy(&ori_voip_dial_rule, &voip_dial_rule, sizeof(voip_dial_rule));
        //set back
        if(mapi_ccfg_set_voip_dial_rule(tid, &voip_dial_rule)==MID_FAIL){
            printf("mapi_ccfg_set_voip_dial_rule fail\n");
        
            return 0;
        }
        //get again
        mapi_ccfg_get_voip_dial_rule(tid, &voip_dial_rule, 0);
        if(memcmp(&ori_voip_dial_rule, &voip_dial_rule, sizeof(voip_dial_rule))){
            printf("test emergency dial rule %d fail\n", i);
        
            return 0;
        }
    }

    //-----test non-emergency dial rule
    for(i=0; i<voip_glb.max_dial_rule_num; i++){
        memset(&voip_dial_rule, 0, sizeof(voip_dial_rule));
        voip_dial_rule.emergency=0;
        voip_dial_rule.rule_idx=i;
        if(mapi_ccfg_get_voip_dial_rule(tid, &voip_dial_rule, 0)==MID_FAIL){
            printf("mapi_ccfg_get_voip_dial_rule fail\n");
        
            return 0;
        }
        printf("dial rule %d\n", voip_dial_rule.rule_idx);
        printf("in_use=%d\n", voip_dial_rule.in_use);
        if(voip_dial_rule.in_use!=0){
            printf("phone_number=%s\n", voip_dial_rule.phone_number);
            printf("conn_type=%d\n", voip_dial_rule.conn_type);
            printf("comments=%s\n", voip_dial_rule.comments);
        }
        memcpy(&ori_voip_dial_rule, &voip_dial_rule, sizeof(voip_dial_rule));
        //set back
        if(mapi_ccfg_set_voip_dial_rule(tid, &voip_dial_rule)==MID_FAIL){
            printf("mapi_ccfg_set_voip_dial_rule fail\n");
        
            return 0;
        }
        //get again
        mapi_ccfg_get_voip_dial_rule(tid, &voip_dial_rule, 0);
        if(memcmp(&ori_voip_dial_rule, &voip_dial_rule, sizeof(voip_dial_rule))){
            printf("test non-emergency dial rule %d fail\n", i);
        
            return 0;
        }
    }

    //-----test phone config
    for(i=0; i<voip_glb.phone_num; i++){
        memset(&voip_phone, 0, sizeof(voip_phone));
        voip_phone.phone_idx=i;
        if(mapi_ccfg_get_voip_phone(tid, &voip_phone, 0)==MID_FAIL){
            printf("mapi_ccfg_get_voip_phone fail\n");
        
            return 0;
        }    
        printf("phone %d:\n", i);
        printf("answer_all_eb=%d\n", voip_phone.answer_all_eb);
        printf("echo_cancel_ms=%d\n", voip_phone.echo_cancel_ms);
        printf("vad_eb=%d\n", voip_phone.vad_eb);
        printf("call_waiting_eb=%d\n", voip_phone.call_waiting_eb);
        printf("CLIR_eb=%d\n", voip_phone.CLIR_eb);

        printf("MWI_eb=%u\n", voip_phone.MWI_eb);
        printf("DND_eb=%u\n", voip_phone.DND_eb);

        printf("ext in (type, idx): ");
        for(j=0; j<MAPI_CCFG_VOIP_MAX_TOTAL_ACCOUNT_NUM; j++){
            printf("(%d,%d) ", voip_phone.ext_in[j].type, voip_phone.ext_in[j].logic_idx);
        }
        printf("\n");
        printf("ext out (type, idx): ");
        for(j=0; j<MAPI_CCFG_VOIP_MAX_TOTAL_ACCOUNT_NUM; j++){
            printf("(%d,%d) ", voip_phone.ext_out[j].type, voip_phone.ext_out[j].logic_idx);
        }
        printf("\n");
        memcpy(&ori_voip_phone, &voip_phone, sizeof(voip_phone));
        //set back
        if(mapi_ccfg_set_voip_phone(tid, &voip_phone)==MID_FAIL){
            printf("mapi_ccfg_set_voip_phone fail\n");
        
            return 0;
        }
        //get again
        mapi_ccfg_get_voip_phone(tid, &voip_phone, 0);
        if(memcmp(&ori_voip_phone, &voip_phone, sizeof(voip_dial_rule))){
            printf("test phone config %d fail\n", i);
        
            return 0;
        }
    }

	//-----test code info
	for ( i=1; i <= MAPI_CCFG_VOIP_CODEC_MAX; i++ ) {
		memset( &voip_codec, 0, sizeof(voip_codec) );
		if ( mapi_ccfg_get_voip_codec_info_by_idx( (unsigned char)i, &voip_codec ) != MID_SUCCESS ) {
			printf( "mapi_ccfg_get_voip_codec_info_by_idx(%d) fail\n", i );
			continue;
		}
        printf("codec index               : %u\n", voip_codec.idx );
        printf("codec ID                  : %u\n", voip_codec.codec );
        printf("codec pkt_period          : %u\n", voip_codec.pkt_period );
        printf("codec silence_suppression : %u\n", voip_codec.silence_suppression );
        printf("codec bit_rate            : %u\n", voip_codec.bit_rate );
	}

#ifdef MAPI_CCFG_VOICE_UMTS_SUPPORT
	//-----test umts voice
    memset(&voip_umts, 0, sizeof(voip_umts));
    if(mapi_ccfg_get_voip_umts(tid, &voip_umts, 0)==MID_FAIL){
        printf("mapi_ccfg_get_voip_umts fail\n");
        
        return 0;
    }
    printf("umts area code=%s\n", voip_umts.area_code);
    printf("umts msn number=%s\n", voip_umts.msn_number);

    memcpy(&ori_voip_umts, &voip_umts, sizeof(voip_umts));
    //set back
    if(mapi_ccfg_set_voip_umts(tid, &voip_umts)==MID_FAIL){
        printf("mapi_ccfg_set_voip_umts fail\n");
        
        return 0;
    }
    //get again
    mapi_ccfg_get_voip_umts(tid, &voip_umts, 0);
    if(memcmp(&ori_voip_umts, &voip_umts, sizeof(voip_umts))){
        printf("test umts cfg fail\n");
        
        return 0;
    }    
#endif
            
    return 0;
}

//-----qos related test functions
void show_qos_config(mapi_ccfg_qos_profile_t *qos_pf)
{
    int                 i, j, k, if_idx;
	mapi_ccfg_qos_qm_root_t   *qm_root;
	mapi_ccfg_qos_qm_L1_t     *qm_L1;
	mapi_ccfg_qos_qm_L2_t     *qm_L2;
	mapi_ccfg_qos_qm_fwg_t    *qm_fwg;    
	mapi_ccfg_qos_clsfy_ru_t  *clsfy_ru;
	char                dscp[8], _8021p[8];
	
	qm_root=qos_pf->qm;
	printf("QoS profile %s:\n", qos_pf->name);
	printf("total_fwg_num=%d\n", qm_root->total_fwg_num);
	printf("dft_fwg=%s\n", qm_root->dft_fwg);
	printf("L1_num=%d\n", qm_root->L1_num);
	for(i=0; i<qm_root->L1_num; i++){
	    qm_L1=qm_root->L1_grp[i];
	    printf("%s:\n", qm_L1->vname);
	    printf(" cid=%x\n", qm_L1->id);
	    printf(" ds_rate=%d\n", qm_L1->ds_Kbps);
	    printf(" us_rate=%d\n", qm_L1->us_Kbps);
	    printf(" L2_num=%d\n", qm_L1->L2_num);
	    printf(" name=%s\n", qm_L1->name);
        for(j=0; j<qm_L1->L2_num; j++){
            qm_L2=qm_L1->L2_grp[j];
 	        printf(" %s:\n", qm_L2->vname);
 	        printf("  cid=%x\n", qm_L2->id);
 	        printf("  rv_rate=%d\n", qm_L2->rv_Kbps);
	        printf("  ul_rate=%d\n", qm_L2->ul_Kbps);
	        printf("  egrs_if_num=%d, egrs_if=", qm_L2->egrs_if_num);
	        for(if_idx=0; if_idx<qm_L2->egrs_if_num; if_idx++){
	            printf("%s ", qm_L2->egrs_if_name[if_idx]);
	        }
	        printf("\n");
	        printf("  egrs_enslv_if_num=%d, egrs_enslv_if=", qm_L2->egrs_enslv_if_num);
	        for(if_idx=0; if_idx<qm_L2->egrs_enslv_if_num; if_idx++){
	            printf("%s ", qm_L2->egrs_enslv_if_name[if_idx]);
	        }
	        printf("\n");
	        printf("  fwg_num=%d\n", qm_L2->fwg_num);
	        printf("  sp_fwg_num=%d\n", qm_L2->sp_fwg_num);
	        printf("  wrr_reserved_bw=%d\n", qm_L2->wrr_reserved_bw);
	        printf("  name=%s\n", qm_L2->name);
	        for(k=0; k<qm_L2->fwg_num; k++){
	            qm_fwg=qm_L2->fwg[k];
 	            printf("  %s\n", qm_fwg->vname);
 	            printf("   cid=%x\n", qm_fwg->id);
                printf("   sp_or_wrr=%d\n", qm_fwg->sp_or_wrr);
                printf("   priwei=%d\n", qm_fwg->priwei);
                printf("   name=%s\n", qm_fwg->name);
            }
        }
    }
        
    printf("clsfy_seq=%s\n", qos_pf->clsfy->ru_order.seq);
    for(i=0; i<MAPI_QOS_CLSFY_MAX_RU_NUM; i++){
        clsfy_ru=&(qos_pf->clsfy->ru[i]);
        printf("ru%02x_name=%s\n", i, clsfy_ru->name);
        if(strcmp(clsfy_ru->name, "~")==0){
            continue;
        }
        printf("ru%02x_smac=%s\n", i, clsfy_ru->smac);
        printf("ru%02x_inif=%s\n", i, clsfy_ru->inif);
        printf("ru%02x_inphyif=%s\n", i, clsfy_ru->inphyif);
        printf("ru%02x_srcip=%s\n", i, clsfy_ru->srcip);
        printf("ru%02x_dst=%s\n", i, clsfy_ru->dstip);
        printf("ru%02x_approto=%s\n", i, clsfy_ru->approto);
        printf("ru%02x_sport=%s\n", i, clsfy_ru->sport);
        printf("ru%02x_dport=%s\n", i, clsfy_ru->dport);
        printf("ru%02x_dscp=%s\n", i, clsfy_ru->dscp);
        printf("ru%02x_8021p=%s\n", i, clsfy_ru->_8021p);
        printf("ru%02x_ethtype=%s\n", i, clsfy_ru->ethtype);
        printf("ru%02x_pktlen=%s\n", i, clsfy_ru->pkylen);
        
        if(clsfy_ru->dscp_remark==MAPI_CCFG_QOS_NO_REMARK){
            strcpy(dscp, "no");
        }
        else{
            sprintf(dscp, "0x%x", clsfy_ru->dscp_remark);
        }
        if(clsfy_ru->_1p_remark==MAPI_CCFG_QOS_NO_REMARK){
            strcpy(_8021p, "no");
        }
        else{
            sprintf(_8021p, "%d", clsfy_ru->_1p_remark);
        }
        printf("ru%02x_act=%s,%s,%s,\n", i, dscp, _8021p, clsfy_ru->fwg_name);
    }
}

static int qos_status(void)
{
    mapi_ccfg_qos_global_t    qos_glb;
    
    memset((char *)&qos_glb, 0, sizeof(mapi_ccfg_qos_global_t));
    if(mapi_ccfg_qos_get_global(tid, &qos_glb, 0)==MID_SUCCESS){
        printf("profile@qos_glb=%s\n", qos_glb.profile);
        printf("enable@qos_glb=%d\n", qos_glb.enable);
        
        return MID_SUCCESS;
    }
    else{
        printf("get QOS global section failed\n");
        
        return MID_FAIL;
    }
}

static int qos_show(const char *pf_name)
{
    mapi_ccfg_qos_profile_t  *qos_pf=NULL;
    
	if(pf_name==NULL){
	    return MID_FAIL;
	}
	
    if((qos_pf=mapi_ccfg_qos_open_profile(tid, pf_name, 0))==NULL){
	    fprintf(stderr,
	            "Could not open profile %s\n", pf_name);
        return MID_FAIL;
	}

	show_qos_config(qos_pf);

	mapi_ccfg_qos_close_profile(qos_pf);
	
	return MID_SUCCESS;
}

static int qos_rw_test(const char *pf_name)
{
    int                 i, j, k, rst=MID_FAIL;
    mapi_ccfg_qos_profile_t  *qos_pf=NULL;
	mapi_ccfg_qos_qm_root_t   *qm_root;
	mapi_ccfg_qos_qm_L1_t     *qm_L1;
	mapi_ccfg_qos_qm_L2_t     *qm_L2;
	mapi_ccfg_qos_qm_fwg_t    *qm_fwg;
	
	if(pf_name==NULL){
	    return MID_FAIL;
	}
	
    if((qos_pf=mapi_ccfg_qos_open_profile(tid, pf_name, 0))==NULL){
	    fprintf(stderr,
	            "Could not open profile %s\n", pf_name);
        return MID_FAIL;
	}
	
	//print original config
	show_qos_config(qos_pf);

	//write back the config
	qm_root=qos_pf->qm;
	for(i=0; i<qm_root->L1_num; i++){
	    qm_L1=qm_root->L1_grp[i];
        for(j=0; j<qm_L1->L2_num; j++){
            qm_L2=qm_L1->L2_grp[j];
	        for(k=0; k<qm_L2->fwg_num; k++){
	            qm_fwg=qm_L2->fwg[k];
                if(mapi_ccfg_qos_set_qm_fwg(tid, qm_fwg)!=MID_SUCCESS)
                    goto close_pf;
            }
            if(mapi_ccfg_qos_set_qm_L2(tid, qm_L2)!=MID_SUCCESS)
                goto close_pf;
        }
        if(mapi_ccfg_qos_set_qm_L1(tid, qm_L1)!=MID_SUCCESS)
            goto close_pf;
    }
    if(mapi_ccfg_qos_set_qm_root(tid, qm_root)!=MID_SUCCESS)
        goto close_pf;

    if(mapi_ccfg_qos_set_clsfy_order(tid, &(qos_pf->clsfy->ru_order))!=MID_SUCCESS)
        goto close_pf;
        
    for(i=0; i<MAPI_QOS_CLSFY_MAX_RU_NUM; i++){
        if(mapi_ccfg_qos_set_clsfy_ru(tid, &(qos_pf->clsfy->ru[i]))!=MID_SUCCESS)
            goto close_pf;
    }
    
    mapi_ccfg_qos_close_profile(qos_pf);
    
    printf("\nafter writing back the config, read again...\n");
    if((qos_pf=mapi_ccfg_qos_open_profile(tid, pf_name, 0))!=NULL){    //re-open the config
	    show_qos_config(qos_pf);
	    rst=MID_SUCCESS;
    }

close_pf:
    if(qos_pf){      
        mapi_ccfg_qos_close_profile(qos_pf);
    }
    
    return rst;
}

#undef PRINT_WLAN_GLOBAL
#undef PRINT_WLAN_CFG()
#undef PRINT_WDS_CFG()
#undef PRINT_WMM_CFG()
#undef PRINT_ACL_CFG()
#undef PRINT_WPS_CFG()
#undef PRINT_WLAN_TIMER_CFG()

unsigned char *mode_str[]={"","B","G","BG","A","","","","N","","GN","BGN","AN","","",""};

static int testWlan_ap(void)
{
	mapi_ccfg_wlan_global_t wlan_global; 
	 mapi_ccfg_wlan_ap_t   wlan_ap, ori_wlan_ap[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM];
	 unsigned band,ap_idx;
	 unsigned char enable[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM]={0,1,0,1};
	 unsigned char mode_mask[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM]={10,10,4,4};
	 unsigned char protect[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM]={0,0,1,1};
	 unsigned char bandwidth[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM]={1,1,0,0};
	 unsigned char channel[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM]={2,2,36,36};
	 unsigned char ht_ext_channel[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM]={6,6,0,0};
	 unsigned char ssid[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM][MAPI_CCFG_WLAN_SSID_MAX_LEN]=
	 { "2g_main_ssid","2g_snd_ssid","5g_main_ssid","5g_snd_ssid"};
	 unsigned char ssid_hidden[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM]={1,1,0,0};
	 unsigned char wmm_capable[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM]={0,0,1,1};
	 

#define PRINT_WLAN_CFG() 	  		do {printf("Bnd%1d idx%1d|%2d|%4s|%7d|%4d|%4d|%4d|%4d|%4d|%s\n",band,ap_idx,wlan_ap.enable,\
	  				mode_str[wlan_ap.mode_mask],wlan_ap.protect,wlan_ap.bandwidth,wlan_ap.channel,\
	  			wlan_ap.ht_ext_channel,wlan_ap.ssid_hidden,wlan_ap.wmm_capable,wlan_ap.ssid);} while(0);

#ifdef	MAPI_CCFG_WLAN_CONCURRENT_DUAL_BAND_SUPPORT
#define PRINT_WLAN_GLOBAL() 	  		do {printf("\nWifi Band_Num:%d ",wlan_global.supported_band_num);\
					printf("Bnd0_APNUM:%d ",wlan_global.band0_max_ap_num);\
					printf("Bnd1_APNUM:%d ",wlan_global.band1_max_ap_num);\
					printf("\n\n");}while(0)
#else
#define PRINT_WLAN_GLOBAL() 	  		do {printf("Wifi Band_Num:%d ",wlan_global.supported_band_num);\
					printf("Bnd0_APNUM:%d ",wlan_global.band0_max_ap_num);\
					printf("\n");}while(0)

#endif

	 
	memset((char *)&wlan_global, 0, sizeof(wlan_global));
	if(mapi_ccfg_get_wlan_global(tid, &wlan_global, 0)==MID_FAIL){
         printf("mapi_ccfg_get_wlan_global fail\n");
         return 0;	
	}

	PRINT_WLAN_GLOBAL();
	 
	 printf("----------- Get Wireless Configuration -------------\n");
	 printf("         |en|mode|protect|  bw|  ch|exch| hid| wmm|ssid\n");
	 
	 for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++)
	  for (ap_idx=0;ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM;ap_idx++)
	  {
	  		int i;
	  		memset((char *)&wlan_ap, 0, sizeof(wlan_ap));
	  		wlan_ap.band = band;
	  		wlan_ap.ap_idx = ap_idx;
	  		if(mapi_ccfg_get_wlan_ap(tid, &wlan_ap, 0)==MID_FAIL){
            printf("mapi_ccfg_get_wlan_ap fail\n");
            return 0;
        }
        
        PRINT_WLAN_CFG();
        
	  		memcpy(&ori_wlan_ap[band][ap_idx],&wlan_ap,sizeof(wlan_ap)); // Back up configuration
	  }

	 printf("----------- Set Wireless Configuration -------------\n");
	 for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++)
	  for (ap_idx=0;ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM;ap_idx++)
	  {
	  		int i;
	  		memset((char *)&wlan_ap, 0, sizeof(wlan_ap));
	  		wlan_ap.band = band;
	  		wlan_ap.ap_idx = ap_idx;
	  		
	  		wlan_ap.enable = enable[band][ap_idx];
	  		wlan_ap.mode_mask = mode_mask[band][ap_idx];
	  		wlan_ap.protect = protect[band][ap_idx];
	  		wlan_ap.bandwidth =bandwidth[band][ap_idx];
	  		wlan_ap.channel= channel[band][ap_idx];
	  		wlan_ap.ssid_hidden= ssid_hidden[band][ap_idx];
	  		wlan_ap.ht_ext_channel= ht_ext_channel[band][ap_idx];
	  		wlan_ap.wmm_capable= wmm_capable[band][ap_idx];
	  		
	  		memcpy(wlan_ap.ssid,ssid[band][ap_idx],MAPI_CCFG_WLAN_SSID_MAX_LEN);

				PRINT_WLAN_CFG();
	  		
	  		if(mapi_ccfg_set_wlan_ap(tid, &wlan_ap)==MID_FAIL){
            printf("mapi_ccfg_set_wlan_ap fail\n");
            return 0;
        }
	  		
	  }

	  
  printf("----------- Get Wireless Configuration Again -------------\n");	  
	 for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++)
	  for (ap_idx=0;ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM;ap_idx++)
	  {
	  		int i;
	  		memset((char *)&wlan_ap, 0, sizeof(wlan_ap));
	  		wlan_ap.band = band;
	  		wlan_ap.ap_idx = ap_idx;
	  		if(mapi_ccfg_get_wlan_ap(tid, &wlan_ap, 0)==MID_FAIL){
            printf("mapi_ccfg_get_wlan_ap fail\n");
            return 0;
        }
        
				PRINT_WLAN_CFG();
  		
	  }	  
	  
  printf("----------- Restore Wireless Configuration -------------\n");	  
	 for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++)
	  for (ap_idx=0;ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM;ap_idx++)
	  {
	  		int i;

	  		if(mapi_ccfg_set_wlan_ap(tid, &ori_wlan_ap[band][ap_idx])==MID_FAIL){
            printf("mapi_ccfg_set_wlan_ap fail\n");
            return 0;
        }	  		
	  		
	  		memset((char *)&wlan_ap, 0, sizeof(wlan_ap));
	  		wlan_ap.band = band;
	  		wlan_ap.ap_idx = ap_idx;
	  		if(mapi_ccfg_get_wlan_ap(tid, &wlan_ap, 0)==MID_FAIL){
            printf("mapi_ccfg_get_wlan_ap fail\n");
            return 0;
        }
        
				PRINT_WLAN_CFG();

	  }	  
	  

}


static int testWlan_wds(void)
{
	mapi_ccfg_wlan_wds_t   wlan_wds, ori_wlan_wds[MAPI_CCFG_WLAN_MAX_BAND_NUM];
	int band,ap_idx;
	unsigned char wds_enable[MAPI_CCFG_WLAN_MAX_BAND_NUM]={1,1};
	unsigned char wds_peer_eb[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_WDS]={{1,0,1,0},{0,1,0,1}};
	unsigned char wds_peer_mac[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_WDS][MAPI_CCFG_WLAN_MAC_LEN]=
	{ "AA:AA:AA:BB:BB:BB","","CC:CC:CC:DD:DD:DD","",
		"","55:66:77:77:66:55","","88:22:88:22:88:22"};
	

#define PRINT_WDS_CFG() do{printf("Bnd%d WDS_EN|%2d|\n",band,wlan_wds.wds_enable);\
        for (i=0;i<MAPI_CCFG_WLAN_MAX_WDS;i++)\
        	printf("Bnd%d WDS%d  |%2d|%s\n",band,i,wlan_wds.wds_peer_eb[i],wlan_wds.wds_peer_mac[i]);}while(0);

	 printf("----------- Get WDS Configuration -------------\n");
	 printf("           |EN|WDS_ENTREY_MAC\n");
	 
	 for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++) {
	 			int i;
	  		memset((char *)&wlan_wds, 0, sizeof(wlan_wds));
	  		wlan_wds.band = band;

	  		if(mapi_ccfg_get_wlan_wds(tid, &wlan_wds, 0)==MID_FAIL){
            printf("mapi_ccfg_get_wlan_wds fail\n");
            return 0;
        }
        
        PRINT_WDS_CFG();
	  		memcpy(&ori_wlan_wds[band],&wlan_wds,sizeof(wlan_wds)); // Back up configuration
	  }
	  
	  printf("----------- Set WDS Configuration -------------\n");
	  for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++) {
	  	int i;
	  	memset((char *)&wlan_wds, 0, sizeof(wlan_wds));
	  	wlan_wds.band = band;
	  	
	  	wlan_wds.wds_enable = wds_enable[band];
	  	for (i=0;i<MAPI_CCFG_WLAN_MAX_WDS;i++) {
	  		wlan_wds.wds_peer_eb[i] = wds_peer_eb[band][i];
				memcpy(wlan_wds.wds_peer_mac[i],wds_peer_mac[band][i],MAPI_CCFG_WLAN_MAC_LEN);
			}

	  	if(mapi_ccfg_set_wlan_wds(tid, &wlan_wds)==MID_FAIL){
          printf("mapi_ccfg_set_wlan_wds fail\n");
           return 0;
      }	  	
	  	
	  	PRINT_WDS_CFG();
	  }
	  
	  printf("----------- Get WDS Configuration Again -------------\n");
	  for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++) {
	  	int i;
	  	memset((char *)&wlan_wds, 0, sizeof(wlan_wds));
	  	wlan_wds.band = band;

	  	if(mapi_ccfg_get_wlan_wds(tid, &wlan_wds, 0)==MID_FAIL){
           printf("mapi_ccfg_get_wlan_wds fail\n");
           return 0;
      }
	  	
	  	PRINT_WDS_CFG();
	  }
	  
	  printf("----------- Restore WDS Configuration -------------\n");
	  for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++) {
	  	int i;
	  	
	  	if(mapi_ccfg_set_wlan_wds(tid, &ori_wlan_wds[band])==MID_FAIL){
            printf("mapi_ccfg_get_wlan_wds fail\n");
            return 0;
        }	  		
			
			memset((char *)&wlan_wds, 0, sizeof(wlan_wds));
	  	wlan_wds.band = band;

	  	if(mapi_ccfg_get_wlan_wds(tid, &wlan_wds, 0)==MID_FAIL){
           printf("mapi_ccfg_get_wlan_wds fail\n");
           return 0;
      }
	  		  	
	  	PRINT_WDS_CFG();
	  }

	
}




static int testWlan_wps(void)
{
	mapi_ccfg_wlan_wps_t   wlan_wps, ori_wlan_wps[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM];
	int band,ap_idx;
	unsigned char wps_enable[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM]={0,1,0,1};
	unsigned char wps_pinlen[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM]={8,4,4,8};
	unsigned char wps_pin[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM][MAPI_CCFG_WLAN_WPS_PIN_MAX_LEN]=
				{"11111111","2222","3333","44444444"};

	

#define PRINT_WPS_CFG() do{printf("Bnd%d idx%d|%2d|%6d|%s\n",band,ap_idx,wlan_wps.enable,\
	wlan_wps.pin_len,wlan_wps.pincode);}while(0);

	 printf("----------- Get WPS Configuration -------------\n");
	 printf("         |EN|PinLen|PinCode\n");
	 
	 for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++) 
	 	for (ap_idx=0;ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM;ap_idx++){
	  		memset((char *)&wlan_wps, 0, sizeof(wlan_wps));
	  		wlan_wps.band = band;
			wlan_wps.ap_idx = ap_idx;
	  		if(mapi_ccfg_get_wlan_wps(tid, &wlan_wps, 0)==MID_FAIL){
            	printf("mapi_ccfg_get_wlan_wds fail\n");
            	return 0;
        	}
        
        PRINT_WPS_CFG();
	  	memcpy(&ori_wlan_wps[band][ap_idx],&wlan_wps,sizeof(wlan_wps)); // Back up configuration
	  }
	  
	  printf("----------- Set WPS Configuration -------------\n");
	  for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++) 
	  	for (ap_idx=0;ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM;ap_idx++){
	  	memset((char *)&wlan_wps, 0, sizeof(wlan_wps));
	  	wlan_wps.band = band;
		wlan_wps.ap_idx = ap_idx;
	  	
	  	wlan_wps.enable = wps_enable[band][ap_idx];
		wlan_wps.pin_len= wps_pinlen[band][ap_idx];
		memcpy(wlan_wps.pincode,wps_pin[band][ap_idx],MAPI_CCFG_WLAN_WPS_PIN_MAX_LEN);
		
	  	if(mapi_ccfg_set_wlan_wps(tid, &wlan_wps)==MID_FAIL){
          printf("mapi_ccfg_get_wlan_wps fail\n");
           return 0;
      }	  	
	  	
	  	PRINT_WPS_CFG();
	  }
	  
	  printf("----------- Get WPS Configuration Again -------------\n");
	  for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++) 
		 for (ap_idx=0;ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM;ap_idx++){
			 memset((char *)&wlan_wps, 0, sizeof(wlan_wps));
			 wlan_wps.band = band;
			 wlan_wps.ap_idx = ap_idx;
			 if(mapi_ccfg_get_wlan_wps(tid, &wlan_wps, 0)==MID_FAIL){
			 printf("mapi_ccfg_get_wlan_wds fail\n");
			 return 0;
		 }
		 
		 PRINT_WPS_CFG();
	  }
	  
	  printf("----------- Restore WPS Configuration -------------\n");
	  for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++) 
	  	for (ap_idx=0;ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM;ap_idx++){
	  	
	  	if(mapi_ccfg_set_wlan_wps(tid, &ori_wlan_wps[band][ap_idx])==MID_FAIL){
            printf("mapi_ccfg_set_wlan_wps fail\n");
            return 0;
        }	  		
			
		memset((char *)&wlan_wps, 0, sizeof(wlan_wps));
	  	wlan_wps.band = band;
		wlan_wps.ap_idx = ap_idx;

	  	if(mapi_ccfg_get_wlan_wps(tid, &wlan_wps, 0)==MID_FAIL){
           printf("mapi_ccfg_get_wlan_wps fail\n");
           return 0;
      }
	  		  	
	  	PRINT_WPS_CFG();
	  }

	
}



static int testWlan_wmm(void)
{
			mapi_ccfg_wlan_wmm_t   wlan_wmm, ori_wlan_wmm;
			int band,ap_idx,i;
			unsigned char  ApCWmin[MAPI_CCFG_WLAN_WMM_AC_NUM]={7,8,9,10};
	    unsigned char  ApCWmax[MAPI_CCFG_WLAN_WMM_AC_NUM]={11,12,13,14};
	    unsigned char  ApAIFSN[MAPI_CCFG_WLAN_WMM_AC_NUM]={1,2,3,4};
	    unsigned short ApTxop[MAPI_CCFG_WLAN_WMM_AC_NUM]={500,600,700,800};
	    unsigned char  ApACM[MAPI_CCFG_WLAN_WMM_AC_NUM]={15,16,17,18};
	    unsigned char  BSSCWmin[MAPI_CCFG_WLAN_WMM_AC_NUM]={57,58,59,60};
	    unsigned char  BSSCWmax[MAPI_CCFG_WLAN_WMM_AC_NUM]={61,62,63,64};
	    unsigned char  BSSAIFSN[MAPI_CCFG_WLAN_WMM_AC_NUM]={71,72,73,74};
	    unsigned short BSSTxop[MAPI_CCFG_WLAN_WMM_AC_NUM]={1500,1600,1700,1800};
	    unsigned char  BSSACM[MAPI_CCFG_WLAN_WMM_AC_NUM]={81,82,83,84};
			
#define PRINT_WMM_CFG()	 do {for (i=0;i<MAPI_CCFG_WLAN_WMM_AC_NUM;i++) \
			printf("%3d |%6d|%6d|%6d|%5d|%4d| |%6d|%6d|%6d|%5d|%4d\n",i,\
						wlan_wmm.ApCWmin[i],wlan_wmm.ApCWmax[i],wlan_wmm.ApAIFSN[i],wlan_wmm.ApTxop[i],wlan_wmm.ApACM[i],\
						wlan_wmm.BSSCWmin[i],wlan_wmm.BSSCWmax[i],wlan_wmm.BSSAIFSN[i],wlan_wmm.BSSTxop[i],wlan_wmm.BSSACM[i]);} while(0);

			memset((char *)&wlan_wmm, 0, sizeof(wlan_wmm));
			
			printf("----------- Get WMM Configuration -------------\n");
			printf(" AC |ACWmin|ACWmax|AAIFSN|ATxop|AACM| |BCWmin|BCWmax|BAIFSN|BTxop|BACM\n");
		
		
			if(mapi_ccfg_get_wlan_wmm(tid, &wlan_wmm, 0)==MID_FAIL){
		       printf("mapi_ccfg_get_wlan_wmm fail\n");
		       return 0;
		  }
		  memcpy(&ori_wlan_wmm,&wlan_wmm,sizeof(wlan_wmm)); // Back up configuration
		  
		  PRINT_WMM_CFG();
		  
		  printf("----------- Set WMM Configuration -------------\n");
		  memset((char *)&wlan_wmm, 0, sizeof(wlan_wmm));
		  for (i=0;i<MAPI_CCFG_WLAN_WMM_AC_NUM;i++) {
		  	wlan_wmm.ApCWmin[i]=ApCWmin[i];
		  	wlan_wmm.ApCWmax[i]=ApCWmax[i];
		  	wlan_wmm.ApAIFSN[i]=ApAIFSN[i];
		  	wlan_wmm.ApTxop[i]=ApTxop[i];
		  	wlan_wmm.ApACM[i]=ApACM[i];
		  	wlan_wmm.BSSCWmin[i]=BSSCWmin[i];
		  	wlan_wmm.BSSCWmax[i]=BSSCWmax[i];
		  	wlan_wmm.BSSAIFSN[i]=BSSAIFSN[i];
		  	wlan_wmm.BSSTxop[i]=BSSTxop[i];
		  	wlan_wmm.BSSACM[i]=BSSACM[i];
		  }
			PRINT_WMM_CFG();

	  	if(mapi_ccfg_set_wlan_wmm(tid, &wlan_wmm)==MID_FAIL){
            printf("mapi_ccfg_set_wlan_wmm fail\n");
            return 0;
			}
			
			printf("----------- Get WMM Configuration -------------\n");
			memset((char *)&wlan_wmm, 0, sizeof(wlan_wmm));
			if(mapi_ccfg_get_wlan_wmm(tid, &wlan_wmm, 0)==MID_FAIL){
		       printf("mapi_ccfg_get_wlan_wmm fail\n");
		       return 0;
		  }
			PRINT_WMM_CFG();
			
			printf("----------- Restore WMM Configuration -------------\n");
			if(mapi_ccfg_set_wlan_wmm(tid, &ori_wlan_wmm)==MID_FAIL){
            printf("mapi_ccfg_set_wlan_wmm fail\n");
            return 0;
			}
			
			memset((char *)&wlan_wmm, 0, sizeof(wlan_wmm));
			if(mapi_ccfg_get_wlan_wmm(tid, &wlan_wmm, 0)==MID_FAIL){
		       printf("mapi_ccfg_get_wlan_wmm fail\n");
		       return 0;
		  }		
			
			PRINT_WMM_CFG();
}



static int testWlan_acl(void)
{
			mapi_ccfg_wlan_acl_t   wlan_acl, ori_wlan_acl[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM];
			unsigned char band,ap_idx;
			int i;
			unsigned enable[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM]={1,0,0,1};
			unsigned action[MAPI_CCFG_WLAN_MAX_BAND_NUM][MAPI_CCFG_WLAN_MAX_SSID_NUM]={2,1,1,1};
			unsigned *mac[4]={"11:11:11:11:22:22","44:55:55:66:33:44","77:77:77:88:88:88","AA:BB:CC:DD:FF:AA"};
			unsigned *mac2[4]={"00:55:55:00:00:55","33:11:55:66:5A:4A","B7:B7:A7:A8:A8:08","A5:5B:6C:2D:3F:2A"};
#define PRINT_ACL_CFG()	 do{			\
	printf("Bnd%d idx%d En   |%d\nBnd%d idx%d Type |%d\n",band,ap_idx,wlan_acl.enable,band,ap_idx,wlan_acl.action);\
			for (i=0;i<MAPI_CCFG_WLAN_MAX_SSID_NUM;i++)\
				if (wlan_acl.mac[i][0]) printf("Bnd%d idx%d MAC%02d|%s\n",band,ap_idx,i,&wlan_acl.mac[i][0]);}while(0)

		printf("----------- Get ACL Configuration -------------\n");
		printf("               |Value\n");
		
	 for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++)
	  for (ap_idx=0;ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM;ap_idx++) {
	  	memset((char *)&wlan_acl, 0, sizeof(wlan_acl));
	  	wlan_acl.band = band;
	  	wlan_acl.ap_idx = ap_idx;

  	
	  	if(mapi_ccfg_get_wlan_acl(tid, &wlan_acl, 0)==MID_FAIL){
            printf("mapi_ccfg_get_wlan_acl fail\n");
            return 0;
      }
        
			PRINT_ACL_CFG();

					
			memcpy(&ori_wlan_acl[band][ap_idx],&wlan_acl,sizeof(wlan_acl));		// Back up configuration
		}	

		printf("----------- Set ACL Configuration -------------\n");			        
	 for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++)
	  for (ap_idx=0;ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM;ap_idx++) {
	  	memset((char *)&wlan_acl, 0, sizeof(wlan_acl));
	  	wlan_acl.band = band;
	  	wlan_acl.ap_idx = ap_idx;
	  	wlan_acl.enable = enable[band][ap_idx];
	  	wlan_acl.action = action[band][ap_idx];
	  	memcpy(wlan_acl.mac[0],mac[band*MAPI_CCFG_WLAN_MAX_SSID_NUM+ap_idx],MAPI_CCFG_WLAN_MAC_LEN);
	  	memcpy(wlan_acl.mac[1],mac2[band*MAPI_CCFG_WLAN_MAX_SSID_NUM+ap_idx],MAPI_CCFG_WLAN_MAC_LEN);

	  	PRINT_ACL_CFG();
	  		  	
	  	if(mapi_ccfg_set_wlan_acl(tid, &wlan_acl)==MID_FAIL){
            printf("mapi_ccfg_set_wlan_acl fail\n");
            return 0;
			}
	  }
	  
		printf("----------- Get ACL Configuration Again -------------\n");
	 for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++)
	  for (ap_idx=0;ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM;ap_idx++) {
	  	memset((char *)&wlan_acl, 0, sizeof(wlan_acl));
	  	wlan_acl.band = band;
	  	wlan_acl.ap_idx = ap_idx;

  	
	  	if(mapi_ccfg_get_wlan_acl(tid, &wlan_acl, 0)==MID_FAIL){
            printf("mapi_ccfg_get_wlan_acl fail\n");
            return 0;
      }
        
			PRINT_ACL_CFG();
		}		  
		
		printf("----------- Restore ACL Configuration -------------\n");			        
	 for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++)
	  for (ap_idx=0;ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM;ap_idx++) {

	  		  	
	  	if(mapi_ccfg_set_wlan_acl(tid, &ori_wlan_acl[band][ap_idx])==MID_FAIL){
            printf("mapi_ccfg_set_wlan_acl fail\n");
            return 0;
			}
			memset((char *)&wlan_acl, 0, sizeof(wlan_acl));
	  	wlan_acl.band = band;
	  	wlan_acl.ap_idx = ap_idx;			
	  	if(mapi_ccfg_get_wlan_acl(tid, &wlan_acl, 0)==MID_FAIL){
            printf("mapi_ccfg_get_wlan_acl fail\n");
            return 0;
      }
        			
	  	PRINT_ACL_CFG();			
	  }		
		
}


static int testWlan_timer(void)
{
		mapi_ccfg_wlan_timer_t   wlan_timer, ori_wlan_timer;
		int i;
		int band,ap_idx;
		mapi_ccfg_wlan_timer_rule_t rule[MAPI_CCFG_WLAN_TIMER_MAX_RULE_NUM] ={
			{0x23,14,30,17,30},
			{0x7f,4,30,7,30},
			{0x40,6,10,12,20},
			{0x4a,8,15,12,45},
			{0x44,1,0,2,0},
		};

	
#define PRINT_WLAN_TIMER_CFG()	 do {	 \
			printf("enable|%2d|       |     |\naction|%2d|       |     |\n",wlan_timer.enable,wlan_timer.action);\
		 for (i=0;i<MAPI_CCFG_WLAN_TIMER_MAX_RULE_NUM;i++){\
	 		int day=wlan_timer.rule[i].weekday_mask,j;\
	 		if (i>=wlan_timer.rule_num) break;\
	 	 	printf("rule %d|  |",i);\
	 	 	for (j=0;j<7;j++) printf("%c",(day&(1<<j))?'0'+j:' ');\
	 	 	printf("|%02d:%02d|%02d:%02d|\n",wlan_timer.rule[i].start_hour,wlan_timer.rule[i].start_min,\
	 	 	wlan_timer.rule[i].end_hour,wlan_timer.rule[i].end_min);\
	 	}}while(0)


	 	printf("----------- Get Band 0 Timer Configuration -------------\n");
	 	printf("      |EN|WEEKDAY|START|END\n");
	 	memset((char *)&wlan_timer, 0, sizeof(wlan_timer));
		wlan_timer.band = 0;
	  if(mapi_ccfg_get_wlan_timer(tid, &wlan_timer, 0)==MID_FAIL){
          printf("mapi_ccfg_get_wlan_timer fail\n");
          return 0;
    }
    memcpy(&ori_wlan_timer,&wlan_timer,sizeof(wlan_timer)); // Back up configuration
	 
		PRINT_WLAN_TIMER_CFG();
		
		printf("----------- Set Timer Configuration -------------\n");
		memset((char *)&wlan_timer, 0, sizeof(wlan_timer));
		wlan_timer.band = 0;
		wlan_timer.enable=1;
		wlan_timer.action=0;
		wlan_timer.rule_num=5;
		for (i=0;i<wlan_timer.rule_num;i++) wlan_timer.rule[i]=rule[i];
	  if(mapi_ccfg_set_wlan_timer(tid, &wlan_timer)==MID_FAIL){
         printf("mapi_ccfg_set_wlan_timer fail\n");
         return 0;
		}
				
		PRINT_WLAN_TIMER_CFG();


	 	printf("----------- Get Timer Configuration Again -------------\n");
		memset((char *)&wlan_timer, 0, sizeof(wlan_timer));
		wlan_timer.band = 0;
	  if(mapi_ccfg_get_wlan_timer(tid, &wlan_timer, 0)==MID_FAIL){
          printf("mapi_ccfg_get_wlan_timer fail\n");
          return 0;
    }
        
	 
		PRINT_WLAN_TIMER_CFG();        
		

	 	printf("----------- Restore Timer Configuration -------------\n");
	  if(mapi_ccfg_set_wlan_timer(tid, &ori_wlan_timer)==MID_FAIL){
         printf("mapi_ccfg_set_wlan_timer fail\n");
         return 0;
		}
	 	
	 	memset((char *)&wlan_timer, 0, sizeof(wlan_timer));
		wlan_timer.band = 0;
	 	if(mapi_ccfg_get_wlan_timer(tid, &wlan_timer, 0)==MID_FAIL){
         printf("mapi_ccfg_get_wlan_timer fail\n");
         return 0;
   	}
	 
		PRINT_WLAN_TIMER_CFG();		


}


static int do_testWlan(void)
{
	testWlan_ap();
	testWlan_wds();
	testWlan_wps();
	testWlan_wmm();
	testWlan_acl();
	testWlan_timer();
}

static int wlan_show(void)
{
	mapi_ccfg_wlan_global_t wlan_global;
	mapi_ccfg_wlan_ap_t   wlan_ap;
	mapi_ccfg_wlan_wds_t   wlan_wds;
	mapi_ccfg_wlan_wps_t   wlan_wps;
	mapi_ccfg_wlan_wmm_t  wlan_wmm;
	mapi_ccfg_wlan_acl_t   wlan_acl;
	mapi_ccfg_wlan_timer_t   wlan_timer;
	unsigned band,ap_idx;
	int i;

	memset((char *)&wlan_global, 0, sizeof(wlan_global));
	if(mapi_ccfg_get_wlan_global(tid, &wlan_global, 0)==MID_FAIL){
         printf("mapi_ccfg_get_wlan_global fail\n");
         return 0;	
	}

	PRINT_WLAN_GLOBAL();
	
	 printf("----------- Get Wireless Configuration -------------\n");
	 printf("         |en|mode|protect|  bw|  ch|exch| hid| wmm|ssid\n");
	 
	 for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++)
	  for (ap_idx=0;ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM;ap_idx++)
	  {
	  		int i;
	  		memset((char *)&wlan_ap, 0, sizeof(wlan_ap));
	  		wlan_ap.band = band;
	  		wlan_ap.ap_idx = ap_idx;
	  		if(mapi_ccfg_get_wlan_ap(tid, &wlan_ap, 0)==MID_FAIL){
            printf("mapi_ccfg_get_wlan_ap fail\n");
            return 0;
        }
        PRINT_WLAN_CFG();

        
	  }	
	  
	 printf("----------- Get WDS Configuration -------------\n");
	 printf("           |EN|WDS_ENTREY_MAC\n");
	 
	 for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++) {
	 			int i;
	  		memset((char *)&wlan_wds, 0, sizeof(wlan_wds));
	  		wlan_wds.band = band;

	  		if(mapi_ccfg_get_wlan_wds(tid, &wlan_wds, 0)==MID_FAIL){
            printf("mapi_ccfg_get_wlan_wds fail\n");
            return 0;
        }
				PRINT_WDS_CFG()	
	  }
	  	  
	 printf("----------- Get WPS Configuration -------------\n");
	 printf("         |EN|PinLen|PinCode\n");
	 
	 for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++) 
	 	for (ap_idx=0;ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM;ap_idx++){
	  		memset((char *)&wlan_wps, 0, sizeof(wlan_wps));
	  		wlan_wps.band = band;
			wlan_wps.ap_idx = ap_idx;
	  		if(mapi_ccfg_get_wlan_wps(tid, &wlan_wps, 0)==MID_FAIL){
            	printf("mapi_ccfg_get_wlan_wds fail\n");
            	return 0;
        	}
        
        PRINT_WPS_CFG();
	 }

			memset((char *)&wlan_wmm, 0, sizeof(wlan_wmm));
			
			printf("----------- Get WMM Configuration -------------\n");
			printf(" AC |ACWmin|ACWmax|AAIFSN|ATxop|AACM| |BCWmin|BCWmax|BAIFSN|BTxop|BACM\n");
		
		
			if(mapi_ccfg_get_wlan_wmm(tid, &wlan_wmm, 0)==MID_FAIL){
		       printf("mapi_ccfg_get_wlan_wmm fail\n");
		       return 0;
		  }
		  PRINT_WMM_CFG()

		printf("----------- Get ACL Configuration -------------\n");
		printf("               |Value\n");
		
	 for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++)
	  for (ap_idx=0;ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM;ap_idx++) {
	  	memset((char *)&wlan_acl, 0, sizeof(wlan_acl));
	  	wlan_acl.band = band;
	  	wlan_acl.ap_idx = ap_idx;

  	
	  	if(mapi_ccfg_get_wlan_acl(tid, &wlan_acl, 0)==MID_FAIL){
            printf("mapi_ccfg_get_wlan_acl fail\n");
            return 0;
      }
        
			PRINT_ACL_CFG();

		}	
		  
	 	printf("----------- Get Bnd0 Timer Configuration -------------\n");
	 	printf("      |EN|WEEKDAY|START|END\n");
	 	memset((char *)&wlan_timer, 0, sizeof(wlan_timer));
		wlan_timer.band = 0;
	  if(mapi_ccfg_get_wlan_timer(tid, &wlan_timer, 0)==MID_FAIL){
          printf("mapi_ccfg_get_wlan_timer fail\n");
          return 0;
    }
	 
	PRINT_WLAN_TIMER_CFG();		  

	 	printf("----------- Get Bnd1 Timer Configuration -------------\n");
	 	memset((char *)&wlan_timer, 0, sizeof(wlan_timer));
		wlan_timer.band = 1;
	  if(mapi_ccfg_get_wlan_timer(tid, &wlan_timer, 0)==MID_FAIL){
          printf("mapi_ccfg_get_wlan_timer fail\n");
          return 0;
    }
	 
	PRINT_WLAN_TIMER_CFG();		  
	
}	


static int security_show(void)
{
	unsigned band,ap_idx;
	mapi_ccfg_wlan_sec_t wlan_sec;
	mapi_ccfg_wlan_1x_t wlan_1x;
	int i;

	unsigned char *sec_mdoe_str[]={"NONE","WEP","WPA","MIX","WPA2"};
	unsigned char *auth_str[]={"NONE","1x"};
	
	memset((char *)&wlan_sec, 0, sizeof(wlan_sec));
	memset((char *)&wlan_1x, 0, sizeof(wlan_1x));

	for (band=0;band<MAPI_CCFG_WLAN_MAX_BAND_NUM;band++)
	 for (ap_idx=0;ap_idx<MAPI_CCFG_WLAN_MAX_SSID_NUM;ap_idx++) {
	 	
		wlan_sec.band =band;
		wlan_sec.ap_idx=ap_idx;
		if(mapi_ccfg_get_wlan_sec(tid, &wlan_sec, 0)==MID_FAIL){
			printf("mapi_ccfg_get_wlan_sec fail\n");
			
			return 0;
		}

	
		printf("\nBand%d Idx%d Security:%s Auth:%s\n",band,ap_idx,sec_mdoe_str[wlan_sec.sec_mode],auth_str[wlan_sec.authen_mode]);
		if (wlan_sec.sec_mode==MAPI_CCFG_WLAN_SEC_WEP){
			printf("keysize=%d default_key_id=%d\n",wlan_sec.wep_key_len,wlan_sec.wep_dft_key_id);
			for (i=0;i<4;i++) printf("key%d=%s\n",i+1,wlan_sec.wep_key[i]);
		}
		else if (wlan_sec.sec_mode > MAPI_CCFG_WLAN_SEC_WEP) {
			printf("KeyType=%s Key=%s\n",(wlan_sec.psk_key_input==0)?"Passphrase":"HEX",
				(wlan_sec.psk_key_input==0)?wlan_sec.psk_passphrase:wlan_sec.psk_256key);
		}
	 
	}
}



int main(int argc, const char *argv[])
{
	int     commit=0;
	int     sync=0;
	int     write=0;
	int     stat=1;
	int     done=0;
	int     i;
	char    commit_flag;
	char*	pSect = NULL;

    if (argc > 1) {
        if ( (tid=mapi_start_transc()) == MID_FAIL ) {
		    fprintf(stderr,
			    "Could not start a transaction to arc middle, possible reasons are:\n"
			    "    - Insufficient memory\n"
			    "    - midcore not initialized yet\n"
			    "    - Insufficient socket resource\n"
		    );
		    done=1;
		    stat=1;
        }
        else {
	    	for(i=1 ; i<argc; i++){
                if(!strcmp(argv[i], "testwan") && (i+1)<argc) {
	    			stat=do_testWan(argv[++i]);
	    			done++;
				}
				else if(!strcmp(argv[i], "testvlan") && (i+1)<argc) {
	    			stat=do_testVlan(argv[++i]);
	    			done++;
				}
				else if(!strcmp(argv[i], "testvoip")) {
	    			stat=do_testVoip();
	    			done++;
				}
				else if(!strcmp(argv[i], "sync_voip_sipdata") && (i+1)<argc) {
	    			stat=do_sync_voip_sipdata(argv[++i]);
	    			done++;
				}
				else if(!strcmp(argv[i], "testwlan")) {
	    			stat=do_testWlan();
	    			done++;
				}
				else if(!strcmp(argv[i], "wlan_show")) {
	    			stat=wlan_show();
	    			done++;
				}
				else if(!strcmp(argv[i], "1x_show")) {
	    			stat=security_show();
	    			done++;
				}				
				else if(!strcmp(argv[i], "qos_status")){
	    			if(qos_status()==MID_SUCCESS)
	    			    stat=1;
	    			else
	    			    stat=0;
	    			done++;
	    	    }
                else if(!strcmp(argv[i], "qos_show") && (i+1)<argc){
	    			if(qos_show(argv[++i])==MID_SUCCESS)
	    			    stat=1;
	    			else
	    			    stat=0;
	    			done++;
	    		}
                else if(!strcmp(argv[i], "qos_rw_test") && (i+1)<argc){
	    			if(qos_rw_test(argv[++i])==MID_SUCCESS)
	    			    stat=1;
	    			else
	    			    stat=0;
	    			done++;
	    		}
	    		else if(!strcmp(argv[i], "test_8021x") ){
	    			if(test_8021x(argv[++i])==MID_SUCCESS)
	    			    stat=1;
	    			else
	    			    stat=0;
	    			done++;
	    		}
	    		else if(!strcmp(argv[i], "gen_dft_pin") ){
	    			if(mapi_ccfg_set_wlan_wps_default_pin(tid)==MID_SUCCESS)
	    			    stat=1;
	    			else
	    			    stat=0;
	    			done++;
	    		}	    			    		
	    		else {
	    			fprintf(stderr, "Illegal option '%s' !\n", argv[i]);
	    			done=0;
	    			break;
	    		}
	    	}
            
            mapi_end_transc(tid);
	    }
    }
    
    if (!done) {
		fprintf(stderr,
			"Usage:\n"
			"	ccfgsal_cli testwan <wan section>\n"
			"	ccfgsal_cli testvlan <vlan idx>(1-based)\n"
			"	ccfgsal_cli testvoip\n"
			"	ccfgsal_cli sync_voip_sipdata <direction> (0: default --> sipdata, 1:  config --> sipdata, 2: sipdata --> config )\n"
			"	ccfgsal_cli testwlan\n"
			"	ccfgsal_cli wlan_show\n"
			"	ccfgsal_cli 1x_show\n"
			"	ccfgsal_cli test_8021x\n"
			"	ccfgsal_cli qos_status\n"
			"	ccfgsal_cli gen_dft_pin\n"
			"	ccfgsal_cli qos_show <profile name>\n"
			"	ccfgsal_cli qos_rw_test <profile name>\n"

		);

		stat=1;
	}

	return stat;
}
