/*
 * Arcadyan middle layer APIs for WAN configuration
 *
 * Copyright 2010, Arcadyan Corporation
 * All Rights Reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mid_mapi_ccfgsal_wan.h"

const char *MAPI_CCFG_WAN_ADMIN[MAPI_CCFG_WAN_ADMIN_NUM]={"disable", "enable"};
const char *MAPI_CCFG_WAN_PROTO[MAPI_CCFG_WAN_PROTO_NUM]={"static", "dhcp", "pppoe", "pppoa", "none"};
const char *MAPI_CCFG_WAN_PVC_ENCAP[MAPI_CCFG_WAN_PVC_ENCAP_NUM]={"vc-mux", "llc"};
const char *MAPI_CCFG_WAN_PVC_QOS[MAPI_CCFG_WAN_PVC_QOS_NUM]={"CBR", "NRT-VBR", "UBR", "RT-VBR", "UBR+"};
const char *MAPI_CCFG_WAN_IFNAME[MAPI_CCFG_WAN_TYPE_NUM]={"", "nas", "eth1", "ptm0", "ppp", "lte"};
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
 *		Get real WAN type by expected one
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		wan_type:       expected WAN type
 *
 * Returns
 *		* MID_FAIL(-1):		failed
 *		* others:			real WAN type
 *
 ******************************************************************************/
int mapi_ccfg_get_wan_real_type( int tid, int wan_type )
{
	int		new_transc = 0;

	if ( wan_type == MAPI_CCFG_WAN_TYPE_CURR )
	{
		if (tid == 0)
		{
			new_transc = 1;
			if ( (tid=mapi_start_transc()) == MID_FAIL )
				return MID_FAIL;
		}
		wan_type = mapi_ccfg_get_long(tid, "system", "wan_type", MAPI_CCFG_WAN_TYPE_DFT );
	}

	if ( wan_type == MAPI_CCFG_WAN_TYPE_DSL_AUTO )
	{
		if (tid == 0)
		{
			new_transc = 1;
			if ( (tid=mapi_start_transc()) == MID_FAIL )
				return MID_FAIL;
		}
		wan_type = mapi_ccfg_get_long(tid, "system", "active_wan_type", MAPI_CCFG_WAN_TYPE_ADSL );
		if ( wan_type != MAPI_CCFG_WAN_TYPE_VDSL )
			wan_type = MAPI_CCFG_WAN_TYPE_ADSL;
	}

	if ( new_transc == 1 )
		mapi_end_transc( tid );

	switch (wan_type)
	{
		case MAPI_CCFG_WAN_TYPE_ADSL:
		case MAPI_CCFG_WAN_TYPE_VDSL:
		case MAPI_CCFG_WAN_TYPE_ETHERNET:
		case MAPI_CCFG_WAN_TYPE_3G:
		case MAPI_CCFG_WAN_TYPE_LTE:
				return wan_type;
		default:
				return MID_FAIL;
	}
}


/*******************************************************************************
 * Description
 *		Get real base number of WAN section index by expected WAN type
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		wan_type:       expected WAN type
 *
 * Returns
 *		* MID_FAIL(-1):		failed
 *		* others:			real base number
 *
 ******************************************************************************/
int mapi_ccfg_get_wan_idx_base( int tid, int wan_type )
{
	wan_type = mapi_ccfg_get_wan_real_type( tid, wan_type );

	switch (wan_type)
	{
		case MAPI_CCFG_WAN_TYPE_ADSL:
				return MAPI_CCFG_WAN_IDBASE_ADSL;
		case MAPI_CCFG_WAN_TYPE_VDSL:
				return MAPI_CCFG_WAN_IDBASE_VDSL;
		case MAPI_CCFG_WAN_TYPE_ETHERNET:
				return MAPI_CCFG_WAN_IDBASE_ETHERNET;
		case MAPI_CCFG_WAN_TYPE_3G:
				return MAPI_CCFG_WAN_IDBASE_3G;
		case MAPI_CCFG_WAN_TYPE_LTE:
				return MAPI_CCFG_WAN_IDBASE_LTE;
		default:
				return MID_FAIL;
	}
}


/*******************************************************************************
 * Description
 *		Get real WAN section index by expected one
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		wan_type:       expected WAN type
 *		wan_idx:		0-base sequence number of WAN section in that WAN type
 *
 * Returns
 *		* MID_FAIL(-1):		failed
 *		* others:			real WAN section index
 *
 ******************************************************************************/
int mapi_ccfg_get_wan_real_idx( int tid, int wan_type, int wan_idx )
{
	int		base;

	if ( wan_idx < 0 )
		return MID_FAIL;

	base = mapi_ccfg_get_wan_idx_base( tid, wan_type );

	if ( base < 0 )
		return MID_FAIL;

	return base + wan_idx;
}


/*******************************************************************************
 * Description
 *		Get WAN global configuration
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		pwan_glb:	    pointer to the buffer of data structure mapi_ccfg_wan_global_t.
 *	                    Required input fields: none
 *      factory_dft:    use 1 to extract the factory default configuration
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 * See Also
 *		mapi_ccfg_set_wan_global
 *
 ******************************************************************************/
int mapi_ccfg_get_wan_global(int tid, mapi_ccfg_wan_global_t* p_wan_glb, int factory_dft)
{
    int                 new_transc=0;
    char                system_sect[]="system";
    char                null_ip4[]="0.0.0.0";

    if(p_wan_glb==NULL){
        MID_TRACE("[%s] NULL p_wan_glb\n", __FUNCTION__);
        
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
    
    //wan_type
    p_wan_glb->wan_type=mapi_ccfg_get_long(tid, system_sect, "wan_type", MAPI_CCFG_WAN_UNKNOWN_VALUE);
    
    //active_wan_type
    p_wan_glb->active_wan_type=mapi_ccfg_get_long(tid, system_sect, "active_wan_type", MAPI_CCFG_WAN_UNKNOWN_VALUE);
    
    //ppp_pass_thru
    p_wan_glb->ppp_pass_thru=mapi_ccfg_get_long(tid, system_sect, "ppp_pass_thru", 0);

    //max_wan_num
    p_wan_glb->max_wan_num=mapi_ccfg_get_long(tid, system_sect, "max_wan_num", MAPI_CCFG_WAN_DFT_MAX_WAN_NUM);

    //dns1
    mapi_ccfg_get_str(tid, system_sect, "dns1", p_wan_glb->dns1, MAPI_CCFG_WAN_IP4_LEN, null_ip4);

    //dns2
    mapi_ccfg_get_str(tid, system_sect, "dns2", p_wan_glb->dns2, MAPI_CCFG_WAN_IP4_LEN, null_ip4);
    
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
 *		Set WAN global configuration
 *
 * Parameters
 *		tid:        transaction ID returned by calling mapi_start_transc()
 *                  use 0 if no tid is provided (no transaction was opened before)
 *		pwan_glb:	pointer to the buffer of data structure mapi_ccfg_wan_global_t.
 *	                Required input fields: none
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* related sections: system
 *
 * See Also
 *		mapi_ccfg_get_wan_global
 *
 ******************************************************************************/
int mapi_ccfg_set_wan_global(int tid, mapi_ccfg_wan_global_t* p_wan_glb)
{
    int                 new_transc=0;
    char                system_sect[]="system";
    //int                 adsl_adminstate, vdsl_adminstate, ether_adminstate;
    
    if(p_wan_glb==NULL){
        MID_TRACE("[%s] NULL p_wan_glb\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    //wan_type
	if ( p_wan_glb->wan_type == MAPI_CCFG_WAN_TYPE_CURR )
		p_wan_glb->wan_type = mapi_ccfg_get_long(tid, system_sect, "wan_type", MAPI_CCFG_WAN_TYPE_DFT );
	else
	    mapi_ccfg_set_long(tid, system_sect, "wan_type", p_wan_glb->wan_type);

    if(p_wan_glb->wan_type!=MAPI_CCFG_WAN_TYPE_DSL_AUTO){
        //if not AUTO, set active_wan_type to same as wan_type
        //or the system automatically ignores active_wan_type in non-auto cases?
        mapi_ccfg_set_long(tid, system_sect, "active_wan_type", p_wan_glb->wan_type);
    }
    
    //ppp_pass_thru
    mapi_ccfg_set_long(tid, system_sect, "ppp_pass_thru", p_wan_glb->ppp_pass_thru);

    //max_wan_num
    mapi_ccfg_set_long(tid, system_sect, "max_wan_num", p_wan_glb->max_wan_num);

    //dns1
    mapi_ccfg_set_str(tid, system_sect, "dns1", p_wan_glb->dns1);

    //dns2
    mapi_ccfg_set_str(tid, system_sect, "dns1", p_wan_glb->dns2);

    //update adminstate of different types of WANs ??
    #if 0
    adsl_adminstate=MAPI_CCFG_WAN_ADMIN_DISABLE;
    vdsl_adminstate=MAPI_CCFG_WAN_ADMIN_DISABLE;
    ether_adminstate=MAPI_CCFG_WAN_ADMIN_DISABLE;
    switch(p_wan_glb->wan_type){
        case MAPI_CCFG_WAN_TYPE_DSL_AUTO:
            adsl_adminstate=MAPI_CCFG_WAN_ADMIN_ENABLE;
            vdsl_adminstate=MAPI_CCFG_WAN_ADMIN_ENABLE;
            break;        
        case MAPI_CCFG_WAN_TYPE_ADSL:
            adsl_adminstate=MAPI_CCFG_WAN_ADMIN_ENABLE;
            break;
        case MAPI_CCFG_WAN_TYPE_VDSL:
            vdsl_adminstate=MAPI_CCFG_WAN_ADMIN_ENABLE;
            break;
        case MAPI_CCFG_WAN_TYPE_ETHERNET:
            ether_adminstate=MAPI_CCFG_WAN_ADMIN_ENABLE;
            break;
        default:
            adsl_adminstate=MAPI_CCFG_WAN_ADMIN_ENABLE;
            vdsl_adminstate=MAPI_CCFG_WAN_ADMIN_ENABLE;
            ether_adminstate=MAPI_CCFG_WAN_ADMIN_ENABLE;
            break;         
    }
    
    for(i=0; i<p_wan_glb->max_wan_num; i++){
        mapi_ccfg_enable_wan(tid, MAPI_CCFG_WAN_TYPE_ADSL, i, adsl_adminstate);
        mapi_ccfg_enable_wan(tid, MAPI_CCFG_WAN_TYPE_VDSL, i, vdsl_adminstate);
        mapi_ccfg_enable_wan(tid, MAPI_CCFG_WAN_TYPE_ETHERNET, i, ether_adminstate);
    }        
    #endif
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Get a WAN interface configuration
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_wan_cfg:	    pointer to the buffer of data structure mapi_ccfg_wan_t.
 *	                    Required input fields:
 *                          1. p_wan_cfg->wan_type and p_wan_cfg->wan_idx; OR
 *                          2. if p_wan_cfg->wan_type==0, p_wan_cfg->section[] is used to specify the target wan section
 *      factory_dft:    use 1 to extract the factory default configuration
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* If p_wan_cfg->wan_type is MAPI_CCFG_WAN_TYPE_DSL_AUTO orMAPI_CCFG_WAN_TYPE_DSL_CURR,
 *			it will be changed to the real WAN type. Please refer to mapi_ccfg_get_wan_real_type().
 *
 * See Also
 *		mapi_ccfg_set_wan
 *
 ******************************************************************************/
int mapi_ccfg_get_wan(int tid, mapi_ccfg_wan_t* p_wan_cfg, int factory_dft)
{
    int                 new_transc=0;
    int                 i, base, idx;
    char                tmp_buf[128];
    char                null_ip4[]="0.0.0.0";
    int                 rsult=MID_FAIL;
    
    if(p_wan_cfg==NULL){
        MID_TRACE("[%s] NULL p_wan_cfg\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }

    if (p_wan_cfg->wan_type!=0) {
		idx = mapi_ccfg_get_wan_real_idx( tid, p_wan_cfg->wan_type, p_wan_cfg->wan_idx );
        if ( idx < 0 ) {
            MID_TRACE("[%s] unknown wan_type %d or wrong wan_idx %d \n", __FUNCTION__, p_wan_cfg->wan_type, p_wan_cfg->wan_idx);
            if(new_transc)
                mapi_end_transc(tid);
            return MID_FAIL;
        }
        sprintf(p_wan_cfg->section, "wan%03d", idx );
    }
    else{
        sscanf(p_wan_cfg->section, "wan%d", &idx);
        if(idx>=MAPI_CCFG_WAN_IDBASE_LTE){
            p_wan_cfg->wan_type=MAPI_CCFG_WAN_TYPE_LTE;
            base=MAPI_CCFG_WAN_IDBASE_LTE;
        }
        else if(idx>=MAPI_CCFG_WAN_IDBASE_3G){
            p_wan_cfg->wan_type=MAPI_CCFG_WAN_TYPE_3G;
            base=MAPI_CCFG_WAN_IDBASE_3G;
        }
        else if(idx>=MAPI_CCFG_WAN_IDBASE_ETHERNET){
            p_wan_cfg->wan_type=MAPI_CCFG_WAN_TYPE_ETHERNET;
            base=MAPI_CCFG_WAN_IDBASE_ETHERNET;
        }
        else if(idx>=MAPI_CCFG_WAN_IDBASE_VDSL){
            p_wan_cfg->wan_type=MAPI_CCFG_WAN_TYPE_VDSL;
            base=MAPI_CCFG_WAN_IDBASE_VDSL;
        }
        else{
            p_wan_cfg->wan_type=MAPI_CCFG_WAN_TYPE_ADSL;
            base=MAPI_CCFG_WAN_IDBASE_ADSL;
        }
        p_wan_cfg->wan_idx=idx-base;
    }
    
    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }
    
    //proto
    if(mapi_ccfg_get_str2(tid, p_wan_cfg->section, "proto", tmp_buf, sizeof(tmp_buf))==MID_FAIL){
        MID_TRACE("[%s] can't get proto in %s\n", __FUNCTION__, p_wan_cfg->section);

        //since "proto" is a basic variable in a wan section, finding no "proto" indicates the wan section may not exist
        goto get_wan_end;
    }
    p_wan_cfg->proto=MAPI_CCFG_WAN_UNKNOWN_VALUE;
    for(i=0; i<MAPI_CCFG_WAN_PROTO_NUM; i++){
        if(strcmp(tmp_buf, MAPI_CCFG_WAN_PROTO[i])==0){
            p_wan_cfg->proto=i;
            
            break;
        }
    }
    
    //adminstate
    mapi_ccfg_get_str(tid, p_wan_cfg->section, "adminstate", tmp_buf, sizeof(tmp_buf), NULL);
    p_wan_cfg->adminstate=MAPI_CCFG_WAN_ADMIN_DISABLE;
    for(i=0; i<MAPI_CCFG_WAN_ADMIN_NUM; i++){
        if(strcmp(tmp_buf, MAPI_CCFG_WAN_ADMIN[i])==0){
            p_wan_cfg->adminstate=i;
            
            break;
        }
    }
    
    //defaultroute
    p_wan_cfg->defaultroute=mapi_ccfg_get_long(tid, p_wan_cfg->section, "defaultroute", 0);
    
    //ifname
    mapi_ccfg_get_str(tid, p_wan_cfg->section, "ifname", p_wan_cfg->ifname, MAPI_CCFG_WAN_IFNAME_LEN, NULL);
    
    //ip4addr
    mapi_ccfg_get_str(tid, p_wan_cfg->section, "ip4addr", p_wan_cfg->ip4addr, MAPI_CCFG_WAN_IP4_LEN, null_ip4);
    
    //ip4mask
    mapi_ccfg_get_str(tid, p_wan_cfg->section, "ip4mask", p_wan_cfg->ip4mask, MAPI_CCFG_WAN_IP4_LEN, null_ip4);

    //ip4gateway
    mapi_ccfg_get_str(tid, p_wan_cfg->section, "ip4gateway", p_wan_cfg->ip4gateway, MAPI_CCFG_WAN_IP4_LEN, null_ip4);

    //dns
    mapi_ccfg_get_str(tid, p_wan_cfg->section, "dns", p_wan_cfg->dns, MAPI_CCFG_WAN_DNS_LEN, NULL);
    
    //mtu
    p_wan_cfg->mtu=mapi_ccfg_get_long(tid, p_wan_cfg->section, "mtu", 0);
    
    //username
    mapi_ccfg_get_str(tid, p_wan_cfg->section, "username", p_wan_cfg->username, MAPI_CCFG_WAN_USERNAME_LEN, NULL);

    //password
    mapi_ccfg_get_str(tid, p_wan_cfg->section, "password", p_wan_cfg->password, MAPI_CCFG_WAN_PASSWORD_LEN, NULL);

    //on-demand
    p_wan_cfg->on_demand=mapi_ccfg_get_long(tid, p_wan_cfg->section, "on-demand", 0);

    //idletime
    p_wan_cfg->idletime=mapi_ccfg_get_long(tid, p_wan_cfg->section, "idletime", 0);    

    //auto_reconnect
    p_wan_cfg->auto_reconnect=mapi_ccfg_get_long(tid, p_wan_cfg->section, "auto_reconnect", 1);

    //lcp_echo_interval
    p_wan_cfg->lcp_echo_interval=mapi_ccfg_get_long(tid, p_wan_cfg->section, "lcp_echo_interval", MAPI_CCFG_WAN_DFT_LCP_ECHO_INTERVAL);

    //lcp_echo_fail
    p_wan_cfg->lcp_echo_fail=mapi_ccfg_get_long(tid, p_wan_cfg->section, "lcp_echo_fail", MAPI_CCFG_WAN_DFT_LCP_ECHO_FAIL);

    //ncp
    mapi_ccfg_get_str(tid, p_wan_cfg->section, "ncp", p_wan_cfg->ncp, MAPI_CCFG_WAN_NCP_LEN, NULL);

    //disconnecttype
    p_wan_cfg->disconnecttype=mapi_ccfg_get_long(tid, p_wan_cfg->section, "disconnecttype", 0);    

    //disconnecttime
    mapi_ccfg_get_str(tid, p_wan_cfg->section, "disconnecttime", p_wan_cfg->disconnecttime, MAPI_CCFG_WAN_DISCONNECTTIME_LEN, NULL);

    //macaddr
    mapi_ccfg_get_str(tid, p_wan_cfg->section, "macaddr", p_wan_cfg->macaddr, MAPI_CCFG_WAN_MAC_LEN, NULL);
    
    //brvlan_idx
    p_wan_cfg->brvlan_idx=mapi_ccfg_get_long(tid, p_wan_cfg->section, "brvlan_idx_1b", 0);

    //vlan_tagged
    p_wan_cfg->vlan_tagged=mapi_ccfg_get_long(tid, p_wan_cfg->section, "vlan_tagged", 0);

    //vlan_1p, vlan_id
    for(i=0; i<MAPI_CCFG_WAN_MAX_VLAN_TAGS; i++){
        char    _ip_var[16], _id_var[16];
        if(i==0){   //for backward compatible
            strcpy(_ip_var, "vlan_1p");
            strcpy(_id_var, "vlan_id");
        }
        else{
            sprintf(_ip_var, "vlan_1p%d", i);
            sprintf(_id_var, "vlan_id%d", i);
        }
        p_wan_cfg->vlan_1p[i]=(unsigned short)(mapi_ccfg_get_long(tid, p_wan_cfg->section, _ip_var, 0));
        p_wan_cfg->vlan_id[i]=(unsigned short)(mapi_ccfg_get_long(tid, p_wan_cfg->section, _id_var, 0));
    }
    
    //clientid
    mapi_ccfg_get_str(tid, p_wan_cfg->section, "clientid", p_wan_cfg->clientid, MAPI_CCFG_WAN_DHCPCLIENTID_LEN, NULL);
    
    //nat
    p_wan_cfg->nat=mapi_ccfg_get_long(tid, p_wan_cfg->section, "nat", MAPI_CCFG_WAN_NAT_ON);
    
    //ip6_enable
    p_wan_cfg->ip6_enable=(char)(mapi_ccfg_get_long(tid, p_wan_cfg->section, "ip6_enable", 0));

    //accept_ra
    p_wan_cfg->accept_ra=(char)(mapi_ccfg_get_long(tid, p_wan_cfg->section, "accept_ra", 0));

    //send_rs
    //p_wan_cfg->send_rs=(char)(mapi_ccfg_get_long(tid, p_wan_cfg->section, "send_rs", 0));
    
    //dhcp6c
    p_wan_cfg->dhcp6c=(char)(mapi_ccfg_get_long(tid, p_wan_cfg->section, "dhcp6c", 0));
    
    //rs_interval
    p_wan_cfg->rs_interval=(unsigned short)(mapi_ccfg_get_long(tid, p_wan_cfg->section, "rs_interval", 0));
    
    //ip6_tunnel_pfx
    mapi_ccfg_get_str(tid, p_wan_cfg->section, "ip6_tunnel_pfx", p_wan_cfg->ip6_tunnel_pfx, MAPI_CCFG_WAN_MAX_IP6_PFX_LEN, NULL);

    //ip6_tunnel_peer
    mapi_ccfg_get_str(tid, p_wan_cfg->section, "ip6_tunnel_peer", p_wan_cfg->ip6_tunnel_peer, MAPI_CCFG_WAN_IP4_LEN, NULL);

    //ip6_tunnel_6rd_manual
    p_wan_cfg->ip6_tunnel_6rd_manual=(char)(mapi_ccfg_get_long(tid, p_wan_cfg->section, "ip6_tunnel_6rd_manual", 0));

    //ip6_tunnel_6rd_ip4masklen
    p_wan_cfg->ip6_tunnel_6rd_ip4masklen=(char)(mapi_ccfg_get_long(tid, p_wan_cfg->section, "ip6_tunnel_6rd_ip4masklen", 0));
    
    rsult=MID_SUCCESS;
    
get_wan_end:
    
    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
    }
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    return rsult;
}

/*******************************************************************************
 * Description
 *		Set a WAN interface configuration
 *
 * Parameters
 *		tid:        transaction ID returned by calling mapi_start_transc()
 *                  use 0 if no tid is provided (no transaction was opened before)
 *		p_wan_cfg:	pointer to the buffer of data structure mapi_ccfg_wan_t.
 *	                Required input fields:
 *                      1. p_wan_cfg->wan_type and p_wan_cfg->wan_idx; OR
 *                      2. if p_wan_cfg->wan_type==0, p_wan_cfg->section[] is used to specify the target wan section
 *                  
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* related sections: wanNNN (where NNN is 3-digits real index of WAN section)
 *		* If p_wan_cfg->wan_type is MAPI_CCFG_WAN_TYPE_DSL_AUTO orMAPI_CCFG_WAN_TYPE_DSL_CURR,
 *			the target WAN section is the real WAN type.
 *			Please refer to mapi_ccfg_get_wan_real_idx() to get the related WAN section index.
 *
 * See Also
 *		mapi_ccfg_get_wan
 *
 ******************************************************************************/
int mapi_ccfg_set_wan(int tid, mapi_ccfg_wan_t* p_wan_cfg)
{
    int                 i, new_transc=0;
    int                 base, idx;
	int					wan_type;
    char                tmp_buf[128];
    
    if(p_wan_cfg==NULL){
        MID_TRACE("[%s] NULL p_wan_cfg\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    if(p_wan_cfg->wan_type!=0){
		wan_type = mapi_ccfg_get_wan_real_type( tid, p_wan_cfg->wan_type );
		idx = mapi_ccfg_get_wan_real_idx( tid, wan_type, p_wan_cfg->wan_idx );
        if ( idx < 0 ) {
            MID_TRACE("[%s] unknown wan_type %d or wrong wan_idx %d \n", __FUNCTION__, p_wan_cfg->wan_type, p_wan_cfg->wan_idx);
			if ( new_transc == 1)
				mapi_end_transc(tid);
			return MID_FAIL;
		}
		p_wan_cfg->wan_type = wan_type;
        sprintf(p_wan_cfg->section, "wan%03d", idx);
    }
    else{
        sscanf(p_wan_cfg->section, "wan%d", &idx);
        if(idx>=MAPI_CCFG_WAN_IDBASE_LTE){
            p_wan_cfg->wan_type=MAPI_CCFG_WAN_TYPE_LTE;
            base=MAPI_CCFG_WAN_IDBASE_LTE;
        }
        else if(idx>=MAPI_CCFG_WAN_IDBASE_3G){
            p_wan_cfg->wan_type=MAPI_CCFG_WAN_TYPE_3G;
            base=MAPI_CCFG_WAN_IDBASE_3G;
        }
        else if(idx>=MAPI_CCFG_WAN_IDBASE_ETHERNET){
            p_wan_cfg->wan_type=MAPI_CCFG_WAN_TYPE_ETHERNET;
            base=MAPI_CCFG_WAN_IDBASE_ETHERNET;
        }
        else if(idx>=MAPI_CCFG_WAN_IDBASE_VDSL){
            p_wan_cfg->wan_type=MAPI_CCFG_WAN_TYPE_VDSL;
            base=MAPI_CCFG_WAN_IDBASE_VDSL;
        }
        else{
            p_wan_cfg->wan_type=MAPI_CCFG_WAN_TYPE_ADSL;
            base=MAPI_CCFG_WAN_IDBASE_ADSL;
        }
        p_wan_cfg->wan_idx=idx-base;
    }
    
    //proto
    if(p_wan_cfg->proto<0 || p_wan_cfg->proto>=MAPI_CCFG_WAN_PROTO_NUM){
        tmp_buf[0]='\0';
    }
    else{
        strcpy(tmp_buf, MAPI_CCFG_WAN_PROTO[p_wan_cfg->proto]);
    }
    if(mapi_ccfg_set_str(tid, p_wan_cfg->section, "proto", tmp_buf)==MID_FAIL){
        MID_TRACE("[%s] can't set proto in %s, which may not exist\n", __FUNCTION__, p_wan_cfg->section);
        if(new_transc){
            mapi_end_transc(tid);
        }
        
        return MID_FAIL;
    }

    //adminstate
    if(p_wan_cfg->adminstate<0 || p_wan_cfg->adminstate>=MAPI_CCFG_WAN_ADMIN_NUM){
        tmp_buf[0]='\0';
    }
    else{
        strcpy(tmp_buf, MAPI_CCFG_WAN_ADMIN[p_wan_cfg->adminstate]);
    }
    mapi_ccfg_set_str(tid, p_wan_cfg->section, "adminstate", tmp_buf);
    
    //defaultroute
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "defaultroute", p_wan_cfg->defaultroute);
    
    //ifname
    //mapi_ccfg_set_str(tid, p_wan_cfg->section, "ifname", p_wan_cfg->ifname);
    
    //ip4addr
    mapi_ccfg_set_str(tid, p_wan_cfg->section, "ip4addr", p_wan_cfg->ip4addr);
    
    //ip4mask
    mapi_ccfg_set_str(tid, p_wan_cfg->section, "ip4mask", p_wan_cfg->ip4mask);

    //ip4gateway
    mapi_ccfg_set_str(tid, p_wan_cfg->section, "ip4gateway", p_wan_cfg->ip4gateway);

    //dns
    mapi_ccfg_set_str(tid, p_wan_cfg->section, "dns", p_wan_cfg->dns);
    
    //mtu
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "mtu", p_wan_cfg->mtu);

    //username
    mapi_ccfg_set_str(tid, p_wan_cfg->section, "username", p_wan_cfg->username);

    //password
    mapi_ccfg_set_str(tid, p_wan_cfg->section, "password", p_wan_cfg->password);

    //on-demand
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "on-demand", p_wan_cfg->on_demand);

    //idletime
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "idletime", p_wan_cfg->idletime);    

    //auto_reconnect
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "auto_reconnect", p_wan_cfg->auto_reconnect);

    //lcp_echo_interval
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "lcp_echo_interval", p_wan_cfg->lcp_echo_interval);

    //lcp_echo_fail
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "lcp_echo_fail", p_wan_cfg->lcp_echo_fail);    

    //ncp
    mapi_ccfg_set_str(tid, p_wan_cfg->section, "ncp", p_wan_cfg->ncp);

    //disconnecttype
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "disconnecttype", p_wan_cfg->disconnecttype);    

    //disconnecttime
    mapi_ccfg_set_str(tid, p_wan_cfg->section, "disconnecttime", p_wan_cfg->disconnecttime);
    
    //macaddr
    mapi_ccfg_set_str(tid, p_wan_cfg->section, "macaddr", p_wan_cfg->macaddr);

    //brvlan_idx
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "brvlan_idx_1b", p_wan_cfg->brvlan_idx);

    //vlan_tagged
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "vlan_tagged", p_wan_cfg->vlan_tagged);
    //set ifname based on vlan_tagged
	switch(p_wan_cfg->wan_type){
        case MAPI_CCFG_WAN_TYPE_3G:
            sprintf(tmp_buf, "%s%d",
                    MAPI_CCFG_WAN_IFNAME[p_wan_cfg->wan_type], MAPI_CCFG_WAN_IDBASE_3G+p_wan_cfg->wan_idx);
            break;
        case MAPI_CCFG_WAN_TYPE_LTE:
            sprintf(tmp_buf, "%s%d",
                    MAPI_CCFG_WAN_IFNAME[p_wan_cfg->wan_type], p_wan_cfg->wan_idx);
            break;            
        case MAPI_CCFG_WAN_TYPE_ETHERNET:
        case MAPI_CCFG_WAN_TYPE_VDSL:
            if(p_wan_cfg->vlan_tagged){
                sprintf(tmp_buf, "%s.%d",
                        MAPI_CCFG_WAN_IFNAME[p_wan_cfg->wan_type], p_wan_cfg->vlan_id[0]);
            }
            else{
                sprintf(tmp_buf, "%s",
                        MAPI_CCFG_WAN_IFNAME[p_wan_cfg->wan_type]);
            }
            break;

        default:
            if(p_wan_cfg->vlan_tagged){
                sprintf(tmp_buf, "%s%d.%d",
                        MAPI_CCFG_WAN_IFNAME[p_wan_cfg->wan_type], p_wan_cfg->wan_idx, p_wan_cfg->vlan_id[0]);
            }
            else{
                sprintf(tmp_buf, "%s%d",
                        MAPI_CCFG_WAN_IFNAME[p_wan_cfg->wan_type], p_wan_cfg->wan_idx);
            }
            break;
    }

    mapi_ccfg_set_str(tid, p_wan_cfg->section, "ifname", tmp_buf);
    
    //vlan_1p, vlan_id
    for(i=0; i<MAPI_CCFG_WAN_MAX_VLAN_TAGS; i++){
        char    _ip_var[16], _id_var[16];
        if(i==0){   //for backward compatible
            strcpy(_ip_var, "vlan_1p");
            strcpy(_id_var, "vlan_id");
        }
        else{
            sprintf(_ip_var, "vlan_1p%d", i);
            sprintf(_id_var, "vlan_id%d", i);
        }
        mapi_ccfg_set_long(tid, p_wan_cfg->section, _ip_var, (long)(p_wan_cfg->vlan_1p[i]));
        mapi_ccfg_set_long(tid, p_wan_cfg->section, _id_var, (long)(p_wan_cfg->vlan_id[i]));
    }
    
    //clientid
    mapi_ccfg_set_str(tid, p_wan_cfg->section, "clientid", p_wan_cfg->clientid);

    //nat
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "nat", p_wan_cfg->nat);

    //ip6_enable
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "ip6_enable", (long)(p_wan_cfg->ip6_enable));

    //accept_ra
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "accept_ra", (long)(p_wan_cfg->accept_ra));

    //send_rs
    //mapi_ccfg_set_long(tid, p_wan_cfg->section, "send_rs", (long)(p_wan_cfg->send_rs));

    //dhcp6c
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "dhcp6c", (long)(p_wan_cfg->dhcp6c));

    //rs_interval
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "rs_interval", (long)(p_wan_cfg->rs_interval));

    //ip6_tunnel_pfx
    mapi_ccfg_set_str(tid, p_wan_cfg->section, "ip6_tunnel_pfx", p_wan_cfg->ip6_tunnel_pfx);

    //ip6_tunnel_peer
    mapi_ccfg_set_str(tid, p_wan_cfg->section, "ip6_tunnel_peer", p_wan_cfg->ip6_tunnel_peer);

    //ip6_tunnel_6rd_manual
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "ip6_tunnel_6rd_manual", (long)(p_wan_cfg->ip6_tunnel_6rd_manual));

    //ip6_tunnel_6rd_ip4masklen
    mapi_ccfg_set_long(tid, p_wan_cfg->section, "ip6_tunnel_6rd_ip4masklen", (long)(p_wan_cfg->ip6_tunnel_6rd_ip4masklen));

    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Get WAN PVC configuration
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_pvc_cfg:	    pointer to the buffer of data structure mapi_ccfg_pvc_t.
 *	                    Required input fields:
 *                          1. p_pvc_cfg->pvc_unit
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
 *		mapi_ccfg_set_pvc
 *
 ******************************************************************************/
int mapi_ccfg_get_pvc(int tid, mapi_ccfg_pvc_t* p_pvc_cfg, int factory_dft)
{
    int                 new_transc=0;
    int                 i;
    char                tmp_buf[128];
    char                pvc_sect[8];
    char                qos[16], *p;
        
    if(p_pvc_cfg==NULL){
        MID_TRACE("[%s] NULL p_pvc_cfg\n", __FUNCTION__);
        
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
    
    sprintf(pvc_sect, "pvc%03d", p_pvc_cfg->unit);
    
    //vci
    p_pvc_cfg->vci=mapi_ccfg_get_long(tid, pvc_sect, "vci", MAPI_CCFG_WAN_UNKNOWN_VALUE);

    //vpi
    p_pvc_cfg->vpi=mapi_ccfg_get_long(tid, pvc_sect, "vpi", MAPI_CCFG_WAN_UNKNOWN_VALUE);

    //encaps
    mapi_ccfg_get_str(tid, pvc_sect, "encaps", tmp_buf, sizeof(tmp_buf), NULL);
    p_pvc_cfg->encaps=MAPI_CCFG_WAN_PVC_ENCAP_LLC;
    for(i=0; i<MAPI_CCFG_WAN_PVC_ENCAP_NUM; i++){
        if(strcmp(tmp_buf, MAPI_CCFG_WAN_PVC_ENCAP[i])==0){
            p_pvc_cfg->encaps=i;
            
            break;
        }
    }

    //qos, pcr, scr, mbs
    mapi_ccfg_get_str(tid, pvc_sect, "qos", tmp_buf, sizeof(tmp_buf), NULL);
    if(tmp_buf[0]=='"')
        p=tmp_buf+1;
    else
        p=tmp_buf;
    sscanf(p, "%[^,]", qos);

    if((p=strstr(tmp_buf, "pcr="))!=NULL){
        sscanf(p+4, "%d,%*s", &(p_pvc_cfg->pcr));
    }
    else{
        p_pvc_cfg->pcr=MAPI_CCFG_WAN_UNKNOWN_VALUE;
    }

    if((p=strstr(tmp_buf, "scr="))!=NULL){
        sscanf(p+4, "%d,%*s", &(p_pvc_cfg->scr));
    }
    else{
        p_pvc_cfg->scr=MAPI_CCFG_WAN_UNKNOWN_VALUE;
    }

    if((p=strstr(tmp_buf, "mbs="))!=NULL){
        sscanf(p+4, "%d", &(p_pvc_cfg->mbs));
    }
    else{
        p_pvc_cfg->mbs=MAPI_CCFG_WAN_UNKNOWN_VALUE;
    }

    p_pvc_cfg->qos=MAPI_CCFG_WAN_UNKNOWN_VALUE;
    for(i=0; i<MAPI_CCFG_WAN_PVC_QOS_NUM; i++){
        if(strcmp(qos, MAPI_CCFG_WAN_PVC_QOS[i])==0){
            p_pvc_cfg->qos=i;

            break;
        }
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
 *		Set WAN PVC configuration
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_pvc_cfg:	    pointer to the buffer of data structure mapi_ccfg_pvc_t.
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* related sections: pvcNNN (where NNN is 3-digits PVC unit, p_pvc_cfg->unit)
 *
 * See Also
 *		mapi_ccfg_get_pvc
 *
 ******************************************************************************/
int mapi_ccfg_set_pvc(int tid, mapi_ccfg_pvc_t* p_pvc_cfg)
{
    int                 new_transc=0;
    char                tmp_buf[128];
    char                pvc_sect[8];
        
    if(p_pvc_cfg==NULL){
        MID_TRACE("[%s] NULL p_pvc_cfg\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    sprintf(pvc_sect, "pvc%03d", p_pvc_cfg->unit);
    
    //vci
    mapi_ccfg_set_long(tid, pvc_sect, "vci", p_pvc_cfg->vci);

    //vpi
    mapi_ccfg_set_long(tid, pvc_sect, "vpi", p_pvc_cfg->vpi);

    //encaps
    if(p_pvc_cfg->encaps<0 || p_pvc_cfg->encaps>=MAPI_CCFG_WAN_PVC_ENCAP_NUM){
        tmp_buf[0]='\0';
    }
    else{
        strcpy(tmp_buf, MAPI_CCFG_WAN_PVC_ENCAP[p_pvc_cfg->encaps]);
    }
    mapi_ccfg_set_str(tid, pvc_sect, "encaps", tmp_buf);

    //qos, pcr, scr, mbs
    if(p_pvc_cfg->qos<0 || p_pvc_cfg->qos>=MAPI_CCFG_WAN_PVC_QOS_NUM){
        sprintf(tmp_buf, "%s,aal5:max_pcr=%d,min_pcr=%d,pcr=%d,scr=%d,mbs=%d",
                "UNKNOWN",
                p_pvc_cfg->pcr, p_pvc_cfg->pcr, p_pvc_cfg->pcr, p_pvc_cfg->scr, p_pvc_cfg->mbs);
    }
    else{    
        sprintf(tmp_buf, "%s,aal5:max_pcr=%d,min_pcr=%d,pcr=%d,scr=%d,mbs=%d",
                MAPI_CCFG_WAN_PVC_QOS[p_pvc_cfg->qos],
                p_pvc_cfg->pcr, p_pvc_cfg->pcr, p_pvc_cfg->pcr, p_pvc_cfg->scr, p_pvc_cfg->mbs);
    }
    mapi_ccfg_set_str(tid, pvc_sect, "qos", tmp_buf);
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Get 3G/4G WAN APN configuration
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_apn_cfg:	    pointer to the buffer of data structure mapi_ccfg_apn_t.
 *	                    Required input fields:
 *                          1. p_apn_cfg->unit
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
 *		mapi_ccfg_set_apn
 *
 ******************************************************************************/
int mapi_ccfg_get_apn(int tid, mapi_ccfg_apn_t* p_apn_cfg, int factory_dft)
{
    int                 new_transc=0;
    //int                 i;
    //char                tmp_buf[128];
    char                apn_sect[8];
        
    if(p_apn_cfg==NULL){
        MID_TRACE("[%s] NULL p_apn_cfg\n", __FUNCTION__);
        
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
    
    sprintf(apn_sect, "apn%03d", p_apn_cfg->unit);
    
    //band
    p_apn_cfg->band=mapi_ccfg_get_long(tid, apn_sect, "band", MAPI_CCFG_WAN_APN_DEFAULT_BAND);

    //dl_channel
    p_apn_cfg->dl_channel=mapi_ccfg_get_long(tid, apn_sect, "dl_channel", MAPI_CCFG_WAN_APN_DEFAULT_DLCHANNEL);

    //plmn
    mapi_ccfg_get_str(tid, apn_sect, "plmn", p_apn_cfg->plmn, MAPI_CCFG_WAN_APN_PLMN_LEN, NULL);

    //apn
    mapi_ccfg_get_str(tid, apn_sect, "apn", p_apn_cfg->apn, MAPI_CCFG_WAN_APN_APN_LEN, NULL);

    //pin
    mapi_ccfg_get_str(tid, apn_sect, "pin", p_apn_cfg->pin, MAPI_CCFG_WAN_APN_PIN_LEN, NULL);

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
 *		Set 3G/4G WAN APN configuration
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_apn_cfg:	    pointer to the buffer of data structure mapi_ccfg_apn_t.
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* related sections: apnNNN (where NNN is 3-digits APN unit, p_apn_cfg->unit)
 *
 * See Also
 *		mapi_ccfg_get_apn
 *
 ******************************************************************************/
int mapi_ccfg_set_apn(int tid, mapi_ccfg_apn_t* p_apn_cfg)
{
    int                 new_transc=0;
    //char                tmp_buf[128];
    char                apn_sect[8];
        
    if(p_apn_cfg==NULL){
        MID_TRACE("[%s] NULL p_apn_cfg\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    sprintf(apn_sect, "apn%03d", p_apn_cfg->unit);
    
    //band
    mapi_ccfg_set_long(tid, apn_sect, "band", p_apn_cfg->band);

    //dl_channel
    mapi_ccfg_set_long(tid, apn_sect, "dl_channel", p_apn_cfg->dl_channel);

    //plmn
    mapi_ccfg_set_str(tid, apn_sect, "plmn", p_apn_cfg->plmn);

    //apn
    mapi_ccfg_set_str(tid, apn_sect, "apn", p_apn_cfg->apn);

    //pin
    mapi_ccfg_set_str(tid, apn_sect, "pin", p_apn_cfg->pin);
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Set WAN adminstate
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		wan_type:	    should be MAPI_CCFG_WAN_TYPE_ADSL, MAPI_CCFG_WAN_TYPE_VDSL, or
 *                      MAPI_CCFG_WAN_TYPE_ETHERNET
 *      wan_idx:        0-based
 *      enable:         use MAPI_CCFG_WAN_ADMIN_ENABLE to activate the WAN;
 *                      use MAPI_CCFG_WAN_ADMIN_DISABLE to deactivate the WAN
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* related sections: wanNNN (where NNN is 3-digits real index of WAN section)
 *		* If wan_type is MAPI_CCFG_WAN_TYPE_DSL_AUTO orMAPI_CCFG_WAN_TYPE_DSL_CURR,
 *			the target WAN section is the real WAN type.
 *			Please refer to mapi_ccfg_get_wan_real_idx() to get the related WAN section index.
 *
 * See Also
 *		
 *
 ******************************************************************************/
int mapi_ccfg_enable_wan(int tid, int wan_type, int wan_idx, int adminstate)
{
    int                 new_transc=0;
    int                 idx;
    char                wan_sect[8];
            
    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }

	idx = mapi_ccfg_get_wan_real_idx( tid, wan_type, wan_idx );
    if ( idx < 0 ) {
        MID_TRACE("[%s] unknown wan_type %d or wrong wan_idx %d \n", __FUNCTION__, wan_type, wan_idx);
		if ( new_transc == 1)
			mapi_end_transc(tid);
		return MID_FAIL;
	}
    
    sprintf(wan_sect, "wan%03d", idx);
    mapi_ccfg_set_str(tid, wan_sect, "adminstate", (char*)MAPI_CCFG_WAN_ADMIN[adminstate]);

    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    return MID_SUCCESS;
}