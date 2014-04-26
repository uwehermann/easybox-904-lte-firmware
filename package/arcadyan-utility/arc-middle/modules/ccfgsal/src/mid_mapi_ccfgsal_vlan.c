/*
 * Arcadyan middle layer APIs for VLAN configuration
 *
 * Copyright 2010, Arcadyan Corporation
 * All Rights Reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mid_mapi_ccfgsal_vlan.h"
#include "mid_mapi_ccfgsal_wan.h"

/*
 * -- Helper functions --
 */

int update_vlan_ifname_and_admin(int tid, mapi_ccfg_vlan_global_t *p_vlan_glb)
{
    //int                     new_transc=0;
    int                     i, j, port_mask;
    char                    vlan_sec[32];
    char                    *p, tmp_buf[64];
    char                    vlan_ifnames[MAPI_CCFG_VLAN_MAX_BR_LAN_NUM][64];
    //mapi_ccfg_vlan_t        vlan_cfg;
    int                     bind_to_brlan_id;

    if(tid==0 || p_vlan_glb==NULL){
        return MID_FAIL;
    }

    for(i=0; i<MAPI_CCFG_VLAN_MAX_BR_LAN_NUM; i++){
        memset(&vlan_ifnames[i][0], 0, 64);
    }

    if(p_vlan_glb->enable){
        //set ifnames of each br-lanX
        for(i=0; i<MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM; i++){
            //memset(&vlan_cfg, 0, sizeof(vlan_cfg));
            //vlan_cfg.vlan_idx=i+1;
            bind_to_brlan_id=p_vlan_glb->bind_to_brlan_id[i];
            if(bind_to_brlan_id>0 && bind_to_brlan_id<=MAPI_CCFG_VLAN_MAX_BR_LAN_NUM){
                //vlan_cfg.vlan_idx=bind_to_brlan_id;
                //mapi_ccfg_get_vlan(tid, &vlan_cfg, 0);
                //sprintf(vlan_sec, "lan%d", i);
                sprintf(vlan_sec, "lan%d", bind_to_brlan_id-1);
                if(p_vlan_glb->port_mask[i]!=0 || p_vlan_glb->ssid_mask[i]!=0){
                    //mapi_ccfg_set_str(tid, vlan_sec, "adminstate", "enable");

                    //update ifname
                    //memset(tmp_buf, 0, sizeof(tmp_buf));
                    //sprintf(tmp_buf, "eth0.%d ", vlan_cfg.vlan_id);
                    strcpy(tmp_buf, vlan_ifnames[bind_to_brlan_id-1]);
                    if(p_vlan_glb->port_mask[i]!=0){
                        p=tmp_buf+strlen(tmp_buf);                    
                        if(p==tmp_buf)
                            sprintf(p, "eth0.%d ", p_vlan_glb->ethsw_vlanid_to_cpu[i]);
                        else
                            sprintf(p, " eth0.%d ", p_vlan_glb->ethsw_vlanid_to_cpu[i]);
                    }
                    //ssid
                    p=tmp_buf+strlen(tmp_buf);
                    port_mask=p_vlan_glb->ssid_mask[i];
                    for(j=0; port_mask>0; j++){
                        if(port_mask&0x1){
                            if(p==tmp_buf)
                                sprintf(p, "wlan%d ", j);
                            else
                                sprintf(p, " wlan%d ", j);
                            p+=(strlen(p));
                        }
                        port_mask=port_mask>>1;
                    }
                    *(--p)='\0';
                    strcpy(vlan_ifnames[bind_to_brlan_id-1], tmp_buf);
                }
                //else{
                //    mapi_ccfg_set_str(tid, vlan_sec, "adminstate", "disable");
                //}
            }
        }
        for(i=0; i<MAPI_CCFG_VLAN_MAX_BR_LAN_NUM; i++){
            sprintf(vlan_sec, "lan%d", i);
            if(vlan_ifnames[i][0]!='\0'){                
                mapi_ccfg_set_str(tid, vlan_sec, "adminstate", "enable");
                mapi_ccfg_set_str(tid, vlan_sec, "ifname", &vlan_ifnames[i][0]);
            }
            //should we diable the br-lanX' adminstate here?
            else{
                mapi_ccfg_set_str(tid, vlan_sec, "adminstate", "disable");
            }
        }
    }
    else{
        //set administrate of non-default vlans to "disable"
        for(i=1; i<MAPI_CCFG_VLAN_MAX_BR_LAN_NUM; i++){
            //memset(&vlan_cfg, 0, sizeof(vlan_cfg));
            //vlan_cfg.vlan_idx=i+1;
            //mapi_ccfg_get_vlan(tid, &vlan_cfg, 0);
            sprintf(vlan_sec, "lan%d", i);
            mapi_ccfg_set_str(tid, vlan_sec, "adminstate", "disable");
        }
        
        //change ifname of lan
        //lan port
        //memset(&vlan_cfg, 0, sizeof(vlan_cfg));
        //vlan_cfg.vlan_idx=1;
        //mapi_ccfg_get_vlan(tid, &vlan_cfg, 0);
        memset(tmp_buf, 0, sizeof(tmp_buf));
        strcpy(tmp_buf, "eth0 ");
        //ssid
        p=tmp_buf+strlen(tmp_buf);
        port_mask=p_vlan_glb->ssid_mask[0];
        for(i=0; port_mask>0; i++){
            if(port_mask&0x1){            
                sprintf(p, "wlan%d ", i);
                p+=(strlen(p));
            }
            port_mask=port_mask>>1;
        }
        *(--p)='\0';

        mapi_ccfg_set_str(tid, "lan0", "ifname", tmp_buf);
    }
    
    return MID_SUCCESS;
}

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
 *		Get VLAN global configuration
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		pvlan_glb:	    pointer to the buffer of data structure mapi_ccfg_vlan_global_t.
 *	                    Required input fields: none
 *      factory_dft:    use 1 to extract the factory default configuration
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note 
 *
 * See Also
 *
 *
 ******************************************************************************/
int mapi_ccfg_get_vlan_global(int tid, mapi_ccfg_vlan_global_t* p_vlan_glb, int factory_dft)
{
    int                 new_transc=0;
    int                 i, j, vlan_idx;
    int                 port_idx, port_mask;
    char                para[64], tmp_buf[128];
    char                *p, *delim=" ";

    if(p_vlan_glb==NULL){
        MID_TRACE("[%s] NULL p_vlan_glb\n", __FUNCTION__);
        
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
    
    //enable_vlan
    p_vlan_glb->enable=mapi_ccfg_get_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, "enable_vlan", 0);

    //max_vlan
    p_vlan_glb->max_vlan_num=mapi_ccfg_get_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, "max_vlan", MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM);

    //active_vlan
    p_vlan_glb->active_vlan_num=mapi_ccfg_get_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, "active_vlan", 1);
    
    //logical_port_num
    p_vlan_glb->logical_port_num=mapi_ccfg_get_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, "logical_port_num", MAPI_CCFG_VLAN_LOGICAL_PORT_NUM);

    //logical_cpu_port
    p_vlan_glb->logical_cpu_port=mapi_ccfg_get_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, "logical_cpu_port", MAPI_CCFG_VLAN_LOGICAL_PORT_NUM);

    //port_phyconf
    mapi_ccfg_get_str(tid, MAPI_CCFG_VLAN_SECTION_NAME, "port_phyconf", p_vlan_glb->port_phyconf, MAPI_CCFG_VLAN_LOGICAL_PORT_MAP_STR_LEN, NULL);

    //lan_port
    mapi_ccfg_get_str(tid, MAPI_CCFG_VLAN_SECTION_NAME, "lan_port", tmp_buf, 128, NULL);
    for(i=0; i<MAPI_CCFG_VLAN_DFT_LAN_PORT_NUM; i++){
        p_vlan_glb->lan_port[i]=-1;
    }
    if((p=strtok(tmp_buf, delim))!=NULL){
        port_idx=strtol(p, NULL, 0);
        p_vlan_glb->lan_port[0]=port_idx;
        for(i=1; i<MAPI_CCFG_VLAN_DFT_LAN_PORT_NUM && (p=strtok(NULL, delim))!=NULL; i++){
            port_idx=strtol(p, NULL, 0);
            p_vlan_glb->lan_port[i]=port_idx;
        }
    }

    //wan_port
    p_vlan_glb->wan_port=mapi_ccfg_get_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, "wan_port", 0);

    //wan_lan_share
    p_vlan_glb->wan_lan_share=mapi_ccfg_get_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, "wan_lan_share", 0);

    //port_mask
    for(vlan_idx=0; vlan_idx<MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM; vlan_idx++){
        port_mask=0;
        sprintf(para, "vlan%d_port", vlan_idx+1);
        mapi_ccfg_get_str(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, tmp_buf, 128, NULL);
        //notice: vlan1_port(~vlan4port)@vlan refers to the ports in port_phyconf@vlan, not the ports in lan_port@vlan,
        //        however, port_mask is refer to the ports in lan_port@vlan ...
        if(tmp_buf[0]=='\0'){
            p_vlan_glb->port_mask[vlan_idx]=0;
        }
        else{
            if((p=strtok(tmp_buf, delim))!=NULL){
                port_idx=strtol(p, NULL, 0);    //index in port_phyconf
                //port_mask|=(1<<port_idx);
                for(j=0; j<MAPI_CCFG_VLAN_DFT_LAN_PORT_NUM; j++){
                    if(p_vlan_glb->lan_port[j]==port_idx){
                        port_mask|=(1<<j);
                        break;
                    }
                }
                for(i=1; (p=strtok(NULL, delim))!=NULL; i++){
                    port_idx=strtol(p, NULL, 0);
                    //port_mask|=(1<<port_idx);
                    for(j=0; j<MAPI_CCFG_VLAN_DFT_LAN_PORT_NUM; j++){
                        if(p_vlan_glb->lan_port[j]==port_idx){
                            port_mask|=(1<<j);
                            break;
                        }
                    }
                }
                p_vlan_glb->port_mask[vlan_idx]=port_mask;
            }
        }

        //ssid_mask
        port_mask=0;
        sprintf(para, "vlan%d_ssid", vlan_idx+1);
        mapi_ccfg_get_str(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, tmp_buf, 128, NULL);
        if(tmp_buf[0]=='\0'){
            p_vlan_glb->ssid_mask[vlan_idx]=0;
        }
        else{
            if((p=strtok(tmp_buf, delim))!=NULL){
                port_idx=strtol(p, NULL, 0);
                port_mask|=(1<<port_idx);
                for(i=1; (p=strtok(NULL, delim))!=NULL; i++){
                    port_idx=strtol(p, NULL, 0);
                    port_mask|=(1<<port_idx);
                }
                p_vlan_glb->ssid_mask[vlan_idx]=port_mask;
            }
        }
    }

    //ethsw_vlanid_to_cpu
    for(vlan_idx=0; vlan_idx<MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM; vlan_idx++){
        sprintf(para, "vlan%d_id", vlan_idx+1);    
        p_vlan_glb->ethsw_vlanid_to_cpu[vlan_idx]=mapi_ccfg_get_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, 0);
    }

    //bind_to_brlan_id
    for(vlan_idx=0; vlan_idx<MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM; vlan_idx++){
        sprintf(para, "vlan%d_at_brlanX", vlan_idx+1);    
        p_vlan_glb->bind_to_brlan_id[vlan_idx]=mapi_ccfg_get_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, 0);
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
 *		Set VLAN global configuration
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		pvlan_glb:	    pointer to the buffer of data structure mapi_ccfg_vlan_global_t.
 *	                    Required input fields: none
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note 
 *
 * See Also
 *
 *
 ******************************************************************************/
int mapi_ccfg_set_vlan_global(int tid, mapi_ccfg_vlan_global_t* p_vlan_glb)
{
    int                 new_transc=0;
    int                 i, vlan_idx, active_vlan_num;
    int                 port_idx, port_mask, unalloc_lan_port_mask, unalloc_ssid_mask;
    char                para[64], tmp_buf[128];
    char                *p;

    if(p_vlan_glb==NULL){
        MID_TRACE("[%s] NULL p_vlan_glb\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    //actually the fields below should be read-only, not changable
    /*
    //max_vlan
    mapi_ccfg_set_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, "max_vlan", p_vlan_glb->max_vlan_num);
    
    //logical_port_num
    mapi_ccfg_set_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, "logical_port_num", p_vlan_glb->logical_port_num);

    //logical_cpu_port
    mapi_ccfg_set_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, "logical_cpu_port", p_vlan_glb->logical_cpu_port);

    //port_phyconf
    mapi_ccfg_set_str(tid, MAPI_CCFG_VLAN_SECTION_NAME, "port_phyconf", p_vlan_glb->port_phyconf);

    //lan_port
    memset(tmp_buf, 0, sizeof(tmp_buf));
    p=tmp_buf;
    for(i=0; i<MAPI_CCFG_VLAN_DFT_LAN_PORT_NUM; i++){
        if(p_vlan_glb->lan_port[i]>=0){
            sprintf(p, "%d ", p_vlan_glb->lan_port[i]);
            p+=(strlen(p));
        }
        else{
            break;
        }
    }
    if(p>tmp_buf){
        *(--p)='\0';
    }
    mapi_ccfg_set_str(tid, MAPI_CCFG_VLAN_SECTION_NAME, "lan_port", tmp_buf);

    //wan_port
    mapi_ccfg_set_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, "wan_port", p_vlan_glb->wan_port);
    */

    //wan_lan_share
    mapi_ccfg_set_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, "wan_lan_share", p_vlan_glb->wan_lan_share);

    //verify port_mask and ssid_mask conflict, and double check p_vlan_glb->enable
    //(for the case that only vlan1 has nonempty port_mask and ssid_mask)
    active_vlan_num=0;
    unalloc_lan_port_mask=(1<<MAPI_CCFG_VLAN_DFT_LAN_PORT_NUM)-1;
    unalloc_ssid_mask=(1<<MAPI_CCFG_VLAN_DFT_SSID_NUM)-1;

    for(vlan_idx=0; vlan_idx<MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM; vlan_idx++){
        #if 0   //trunking port may belong to multiple vlans simultaneously
        //check port conflict
        for(i=vlan_idx+1; i<MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM; i++){
            port_mask=(p_vlan_glb->port_mask[i])&(p_vlan_glb->port_mask[vlan_idx]);
            if(port_mask!=0){
                p_vlan_glb->port_mask[i]&=(~port_mask);
            }
            port_mask=p_vlan_glb->ssid_mask[i]&p_vlan_glb->ssid_mask[vlan_idx];
            if(port_mask!=0){
                p_vlan_glb->ssid_mask[i]&=(~port_mask);
            }
        }
        #endif
        port_mask=unalloc_lan_port_mask&(p_vlan_glb->port_mask[vlan_idx]);
        unalloc_lan_port_mask&=(~port_mask);
        port_mask=unalloc_ssid_mask&p_vlan_glb->ssid_mask[vlan_idx];
        unalloc_ssid_mask&=(~port_mask);
        
        if(p_vlan_glb->port_mask[vlan_idx]!=0 || p_vlan_glb->ssid_mask[vlan_idx]!=0){
            ++active_vlan_num;
        }
    }
    if(unalloc_lan_port_mask!=0){
        p_vlan_glb->port_mask[0]|=unalloc_lan_port_mask;
    }
    if(unalloc_ssid_mask!=0){
        p_vlan_glb->ssid_mask[0]|=unalloc_ssid_mask;
    }
    if(p_vlan_glb->enable==1 && active_vlan_num<=1){
        p_vlan_glb->enable=0;
    }

    for(vlan_idx=0; vlan_idx<MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM; vlan_idx++){
        //port_mask
        memset(tmp_buf, 0, sizeof(tmp_buf));
        p=tmp_buf;
        port_mask=p_vlan_glb->port_mask[vlan_idx];
        for(i=0; port_mask>0; i++){
            if(port_mask&0x1){
                //notice: vlan1_port(~vlan4port)@vlan refers to the ports in port_phyconf@vlan, not the ports in lan_port@vlan,
                //        however, port_mask is refer to the ports in lan_port@vlan ...
                //sprintf(p, "%d ", i);
                sprintf(p, "%d ", p_vlan_glb->lan_port[i]);
                p+=(strlen(p));
            }
            port_mask=port_mask>>1;
        }
        if(p>tmp_buf){
            *(--p)='\0';
        }
        
        sprintf(para, "vlan%d_port", vlan_idx+1);
        mapi_ccfg_set_str(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, tmp_buf);

        //ssid_mask
        memset(tmp_buf, 0, sizeof(tmp_buf));
        p=tmp_buf;
        port_mask=p_vlan_glb->ssid_mask[vlan_idx];
        for(i=0; port_mask>0; i++){
            if(port_mask&0x1){            
                sprintf(p, "%d ", i);
                p+=(strlen(p));
            }
            port_mask=port_mask>>1;
        }
        if(p>tmp_buf){
            *(--p)='\0';
        }
        
        sprintf(para, "vlan%d_ssid", vlan_idx+1);
        mapi_ccfg_set_str(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, tmp_buf);
    }

    //ethsw_vlanid_to_cpu
    for(vlan_idx=0; vlan_idx<MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM; vlan_idx++){
        sprintf(para, "vlan%d_id", vlan_idx+1);    
        mapi_ccfg_set_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, p_vlan_glb->ethsw_vlanid_to_cpu[vlan_idx]);
    }

    //bind_to_brlan_id
    for(vlan_idx=0; vlan_idx<MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM; vlan_idx++){
        sprintf(para, "vlan%d_at_brlanX", vlan_idx+1);    
        mapi_ccfg_set_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, p_vlan_glb->bind_to_brlan_id[vlan_idx]);
    }

    //enable_vlan (and update ifname and adminstate of each vlan interface)
    update_vlan_ifname_and_admin(tid, p_vlan_glb);
    mapi_ccfg_set_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, "enable_vlan", p_vlan_glb->enable);

    //active_vlan
    mapi_ccfg_set_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, "active_vlan", active_vlan_num);

    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }

    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Get a VLAN configuration
 *
 * Parameters
 *		tid:            transaction ID returned by calling mapi_start_transc()
 *                      use 0 if no tid is provided (no transaction was opened before)
 *		p_vlan_cfg:	    pointer to the buffer of data structure mapi_ccfg_vlan_t.
 *	                    Required input fields:
 *                          1. p_vlan_cfg->vlan_idx
 *      factory_dft:    use 1 to extract the factory default configuration
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note 
 *
 * See Also
 *		mapi_ccfg_set_vlan
 *
 ******************************************************************************/
int mapi_ccfg_get_vlan(int tid, mapi_ccfg_vlan_t* p_vlan_cfg, int factory_dft)
{
    int                 new_transc=0;
    int                 i, vlan_idx;
    //int                 port_idx, port_mask;
    char                para[64], vlan_sec[32], tmp_buf[128];
    //char                *p, *delim=" ";
    char                null_ip4[]="0.0.0.0";
    char                dft_lease[]="infinite";
    int                 rsult=MID_FAIL;

    if(p_vlan_cfg==NULL){
        MID_TRACE("[%s] NULL p_vlan_cfg\n", __FUNCTION__);
        
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

    vlan_idx=p_vlan_cfg->vlan_idx;
    sprintf(vlan_sec, "lan%d", vlan_idx-1);

    #if 0
    //vlan_name
    sprintf(para, "vlan%d_name", vlan_idx);
    if(mapi_ccfg_get_str2(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, p_vlan_cfg->vlan_name, MAPI_CCFG_VLAN_NAME_STR_LEN)==MID_FAIL){
        MID_TRACE("[%s] can't get %s\n", __FUNCTION__, para);

        //since "vlan_name" is a basic variable of a vlan, finding no "vlan_name" indicates the vlan may not exist
        goto get_vlan_end;
    }
    #endif
    
    //port_mask and ssid_mask were moved into struct mapi_ccfg_vlan_global
    /*
    //port_mask
    port_mask=0;
    sprintf(para, "vlan%d_port", vlan_idx);
    mapi_ccfg_get_str(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, tmp_buf, 128, NULL);
    if(tmp_buf[0]=='\0'){
        p_vlan_cfg->port_mask=0;
    }
    else{
        if((p=strtok(tmp_buf, delim))!=NULL){
            port_idx=strtol(p, NULL, 0);
            port_mask|=(1<<port_idx);
            for(i=1; (p=strtok(NULL, delim))!=NULL; i++){
                port_idx=strtol(p, NULL, 0);
                port_mask|=(1<<port_idx);
            }
            p_vlan_cfg->port_mask=port_mask;
        }
    }

    //ssid_mask
    port_mask=0;
    sprintf(para, "vlan%d_ssid", vlan_idx);
    mapi_ccfg_get_str(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, tmp_buf, 128, NULL);
    if(tmp_buf[0]=='\0'){
        p_vlan_cfg->ssid_mask=0;
    }
    else{
        if((p=strtok(tmp_buf, delim))!=NULL){
            port_idx=strtol(p, NULL, 0);
            port_mask|=(1<<port_idx);
            for(i=1; (p=strtok(NULL, delim))!=NULL; i++){
                port_idx=strtol(p, NULL, 0);
                port_mask|=(1<<port_idx);
            }
            p_vlan_cfg->ssid_mask=port_mask;
        }
    }
    */
    
    //brwan_mask
    //sprintf(para, "vlan%d_brwan", vlan_idx);
    //p_vlan_cfg->brwan_mask=mapi_ccfg_get_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, 0);
    p_vlan_cfg->brwan_mask=mapi_ccfg_get_long(tid, vlan_sec, "brwan", 0);

    //bind_rtwan
    //sprintf(para, "vlan%d_bind_rtwan", vlan_idx);
    //p_vlan_cfg->bind_rtwan=mapi_ccfg_get_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, 0);
    p_vlan_cfg->bind_rtwan=mapi_ccfg_get_long(tid, vlan_sec, "rtwan", 0);
    
    //vlan_tagged
    //sprintf(para, "vlan%d_tagged", vlan_idx);
    //p_vlan_cfg->vlan_tagged=mapi_ccfg_get_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, 0);
    p_vlan_cfg->vlan_tagged=mapi_ccfg_get_long(tid, vlan_sec, "vlan_tagged", 0);

    //vlan_1p
    //sprintf(para, "vlan%d_1p", vlan_idx);
    //p_vlan_cfg->vlan_1p=(unsigned short)(mapi_ccfg_get_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, 0));
    p_vlan_cfg->vlan_1p=(unsigned short)(mapi_ccfg_get_long(tid, vlan_sec, "vlan_1p", 0));

    //vlan_id
    //sprintf(para, "vlan%d_id", vlan_idx);
    //p_vlan_cfg->vlan_id=(unsigned short)(mapi_ccfg_get_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, 0));
    p_vlan_cfg->vlan_id=(unsigned short)(mapi_ccfg_get_long(tid, vlan_sec, "vlan_id", 0));

    //ip4addr
    mapi_ccfg_get_str(tid, vlan_sec, "ip4addr", p_vlan_cfg->ip4addr, MAPI_CCFG_VLAN_IP4_LEN, null_ip4);

    //ip4mask
    mapi_ccfg_get_str(tid, vlan_sec, "ip4mask", p_vlan_cfg->ip4mask, MAPI_CCFG_VLAN_IP4_LEN, null_ip4);

    //dhcp4s_eb
    p_vlan_cfg->dhcp4s_eb=mapi_ccfg_get_long(tid, vlan_sec, "dhcp4s_eb", 0);

    //dhcp4pool_s
    mapi_ccfg_get_str(tid, vlan_sec, "dhcp4pool_s", p_vlan_cfg->dhcp4pool_s, MAPI_CCFG_VLAN_IP4_LEN, null_ip4);

    //dhcp4pool_e
    mapi_ccfg_get_str(tid, vlan_sec, "dhcp4pool_e", p_vlan_cfg->dhcp4pool_e, MAPI_CCFG_VLAN_IP4_LEN, null_ip4);

    //dhcp4lease
    mapi_ccfg_get_str(tid, vlan_sec, "dhcp4lease", p_vlan_cfg->dhcp4lease, MAPI_CCFG_VLAN_DHCP_LEASE_LEN, dft_lease);

    //domainname
    mapi_ccfg_get_str(tid, vlan_sec, "domainname", p_vlan_cfg->domainname, MAPI_CCFG_VLAN_DOMAINNAME_LEN, NULL);
    
    //dhcp4_opt43_vendor_specific
    mapi_ccfg_get_str(tid, vlan_sec, "dhcp_vendor_option", p_vlan_cfg->dhcp4_opt43_vendor_specific, MAPI_CCFG_VLAN_DHCP_OPT43_LEN, NULL);

    //dhcp4_opt60_client_vendor_id
    mapi_ccfg_get_str(tid, vlan_sec, "dhcp_option_60", p_vlan_cfg->dhcp4_opt60_client_vendor_id, MAPI_CCFG_VLAN_DHCP_OPT60_LEN, NULL);

    //dhcp4_opt240
    mapi_ccfg_get_str(tid, vlan_sec, "dhcp_option_240", p_vlan_cfg->dhcp4_opt240, MAPI_CCFG_VLAN_DHCP_OPT240_LEN, NULL);

#ifdef MAPI_CCFG_VLAN_STATIC_DHCP
    for(i=0; i<MAPI_CCFG_VLAN_DHCP_MAX_STATIC_ENTRY; i++){
        //static_entry_eb[]
        sprintf(tmp_buf, "dhcpip%d_eb", i+1);
        p_vlan_cfg->static_entry_eb[i]=(char)(mapi_ccfg_get_long(tid, vlan_sec, tmp_buf, 0));
        //static_ip4[]
        sprintf(tmp_buf, "dhcpip%d", i+1);
        mapi_ccfg_get_str(tid, vlan_sec, tmp_buf, p_vlan_cfg->static_ip4[i], MAPI_CCFG_VLAN_IP4_LEN, NULL);
        //static_mac[]
        sprintf(tmp_buf, "dhcpmac%d", i+1);
        mapi_ccfg_get_str(tid, vlan_sec, tmp_buf, p_vlan_cfg->static_mac[i], MAPI_CCFG_VLAN_MAC_LEN, NULL);
    }
#endif

#ifdef MAPI_CCFG_VLAN_ALIAS_SUBNET_CONFIG
    //alias_ip4addr
    mapi_ccfg_get_str(tid, vlan_sec, "alias_ip4addr", p_vlan_cfg->alias_ip4addr, MAPI_CCFG_VLAN_IP4_LEN, null_ip4);

    //alias_ip4mask
    mapi_ccfg_get_str(tid, vlan_sec, "alias_ip4mask", p_vlan_cfg->alias_ip4mask, MAPI_CCFG_VLAN_IP4_LEN, null_ip4);

    //alias_dhcp4pool_s
    mapi_ccfg_get_str(tid, vlan_sec, "alias_dhcp4pool_s", p_vlan_cfg->alias_dhcp4pool_s, MAPI_CCFG_VLAN_IP4_LEN, null_ip4);

    //alias_dhcp4pool_e
    mapi_ccfg_get_str(tid, vlan_sec, "alias_dhcp4pool_e", p_vlan_cfg->alias_dhcp4pool_e, MAPI_CCFG_VLAN_IP4_LEN, null_ip4);
#endif

    rsult=MID_SUCCESS;

get_vlan_end:
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
 *		Set a VLAN configuration
 *
 * Parameters
 *		tid:        transaction ID returned by calling mapi_start_transc()
 *                  use 0 if no tid is provided (no transaction was opened before)
 *		p_vlan_cfg:	pointer to the buffer of data structure mapi_ccfg_vlan_t.
 *	                Required input fields:
 *                      1. p_vlan_cfg->vlan_idx, 1-based
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* related sections: vlan, lanN (where N is VLAN index, p_vlan_cfg->vlan_idx)
 *
 * See Also
 *		mapi_ccfg_get_vlan
 *
 ******************************************************************************/
int mapi_ccfg_set_vlan(int tid, mapi_ccfg_vlan_t* p_vlan_cfg)
{
    int                 new_transc=0;
    int                 i, vlan_idx;
    //int                 port_mask;
    //char                *p;
    char                para[64], vlan_sec[32], tmp_buf[128];
//    int                 wan_type, wan_base;
//    char                wan_sec[32], wan_ifn1[32], wan_ifn2[32];
    
    if(p_vlan_cfg==NULL){
        MID_TRACE("[%s] NULL p_vlan_cfg\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }

    //pre-process
    vlan_idx=p_vlan_cfg->vlan_idx;
    sprintf(vlan_sec, "lan%d", vlan_idx-1);
        
	//if(p_vlan_cfg->port_mask==0){
	//    p_vlan_cfg->vlan_id=0;
	//}

    #if 0
    //vlan_name
    sprintf(para, "vlan%d_name", vlan_idx);
    if(mapi_ccfg_get_str2(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, tmp_buf, sizeof(tmp_buf))==MID_FAIL){
        MID_TRACE("[%s] can't get %s\n", __FUNCTION__, para);

        //since "vlan_name" is a basic variable of a vlan, finding no "vlan_name" indicates the vlan may not exist
        return MID_FAIL;
    }
    mapi_ccfg_set_str(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, p_vlan_cfg->vlan_name);
    #endif

    //port_mask and ssid_mask were moved into struct mapi_ccfg_vlan_global
    /*
    //port_mask
    memset(tmp_buf, 0, sizeof(tmp_buf));
    p=tmp_buf;
    port_mask=p_vlan_cfg->port_mask;
    for(i=0; port_mask>0; i++){
        if(port_mask&0x1){            
            sprintf(p, "%d ", i);
            p+=(strlen(p));
        }
        port_mask=port_mask>>1;
    }
    if(p>tmp_buf){
            *(--p)='\0';
    }
        
    sprintf(para, "vlan%d_port", vlan_idx);
    mapi_ccfg_set_str(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, tmp_buf);

    //ssid_mask
    memset(tmp_buf, 0, sizeof(tmp_buf));
    p=tmp_buf;
    port_mask=p_vlan_cfg->ssid_mask;
    for(i=0; port_mask>0; i++){
        if(port_mask&0x1){            
            sprintf(p, "%d ", i);
            p+=(strlen(p));
        }
        port_mask=port_mask>>1;
    }
    if(p>tmp_buf){
            *(--p)='\0';
    }
        
    sprintf(para, "vlan%d_ssid", vlan_idx);
    mapi_ccfg_set_str(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, tmp_buf);
    */

    //brwan_mask
    //sprintf(para, "vlan%d_brwan", vlan_idx);
    //mapi_ccfg_set_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, p_vlan_cfg->brwan_mask);
    mapi_ccfg_set_long(tid, vlan_sec, "brwan", p_vlan_cfg->brwan_mask);

    //bind_rtwan
    //sprintf(para, "vlan%d_bind_rtwan", vlan_idx);
    //mapi_ccfg_set_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, p_vlan_cfg->bind_rtwan);
    mapi_ccfg_set_long(tid, vlan_sec, "rtwan", p_vlan_cfg->bind_rtwan);

    //vlan_tagged
    //sprintf(para, "vlan%d_tagged", vlan_idx);
    //mapi_ccfg_set_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, p_vlan_cfg->vlan_tagged);
    mapi_ccfg_set_long(tid, vlan_sec, "vlan_tagged", p_vlan_cfg->vlan_tagged);

    //vlan_1p
    //sprintf(para, "vlan%d_1p", vlan_idx);
    //mapi_ccfg_set_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, (long)(p_vlan_cfg->vlan_1p));
    mapi_ccfg_set_long(tid, vlan_sec, "vlan_1p", (long)(p_vlan_cfg->vlan_1p));

    //vlan_id
    //sprintf(para, "vlan%d_id", vlan_idx);
    //mapi_ccfg_set_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, para, (long)(p_vlan_cfg->vlan_id));
    mapi_ccfg_set_long(tid, vlan_sec, "vlan_id", (long)(p_vlan_cfg->vlan_id));

    //ip4addr
    mapi_ccfg_set_str(tid, vlan_sec, "ip4addr", p_vlan_cfg->ip4addr);

    //ip4mask
    mapi_ccfg_set_str(tid, vlan_sec, "ip4mask", p_vlan_cfg->ip4mask);

    //dhcp4s_eb
    mapi_ccfg_set_long(tid, vlan_sec, "dhcp4s_eb", p_vlan_cfg->dhcp4s_eb);

    //dhcp4pool_s
    mapi_ccfg_set_str(tid, vlan_sec, "dhcp4pool_s", p_vlan_cfg->dhcp4pool_s);

    //dhcp4pool_e
    mapi_ccfg_set_str(tid, vlan_sec, "dhcp4pool_e", p_vlan_cfg->dhcp4pool_e);

    //dhcp4lease
    mapi_ccfg_set_str(tid, vlan_sec, "dhcp4lease", p_vlan_cfg->dhcp4lease);

    //domainname
    mapi_ccfg_set_str(tid, vlan_sec, "domainname", p_vlan_cfg->domainname);
    
    //dhcp4_opt43_vendor_specific
    mapi_ccfg_set_str(tid, vlan_sec, "dhcp_vendor_option", p_vlan_cfg->dhcp4_opt43_vendor_specific);

    //dhcp4_opt60_client_vendor_id
    mapi_ccfg_set_str(tid, vlan_sec, "dhcp_option_60", p_vlan_cfg->dhcp4_opt60_client_vendor_id);

    //dhcp4_opt240
    mapi_ccfg_set_str(tid, vlan_sec, "dhcp_option_240", p_vlan_cfg->dhcp4_opt240);

#ifdef MAPI_CCFG_VLAN_STATIC_DHCP
    for(i=0; i<MAPI_CCFG_VLAN_DHCP_MAX_STATIC_ENTRY; i++){
        //static_entry_eb[]
        sprintf(tmp_buf, "dhcpip%d_eb", i+1);
        mapi_ccfg_set_long(tid, vlan_sec, tmp_buf, (long)(p_vlan_cfg->static_entry_eb[i]));
        //static_ip4[]
        sprintf(tmp_buf, "dhcpip%d", i+1);
        mapi_ccfg_set_str(tid, vlan_sec, tmp_buf, p_vlan_cfg->static_ip4[i]);
        //static_mac[]
        sprintf(tmp_buf, "dhcpmac%d", i+1);
        mapi_ccfg_set_str(tid, vlan_sec, tmp_buf, p_vlan_cfg->static_mac[i]);
    }
#endif

#ifdef MAPI_CCFG_VLAN_ALIAS_SUBNET_CONFIG
    //alias_ip4addr
    mapi_ccfg_set_str(tid, vlan_sec, "alias_ip4addr", p_vlan_cfg->alias_ip4addr);

    //alias_ip4mask
    mapi_ccfg_set_str(tid, vlan_sec, "alias_ip4mask", p_vlan_cfg->alias_ip4mask);

    //alias_dhcp4pool_s
    mapi_ccfg_set_str(tid, vlan_sec, "alias_dhcp4pool_s", p_vlan_cfg->alias_dhcp4pool_s);

    //alias_dhcp4pool_e
    mapi_ccfg_set_str(tid, vlan_sec, "alias_dhcp4pool_e", p_vlan_cfg->alias_dhcp4pool_e);
#endif

    /*
    //ifname & adminstate
    memset(tmp_buf, 0, sizeof(tmp_buf));
    p=tmp_buf;
	    if(p_vlan_cfg->port_mask!=0){
	        sprintf(p, "eth0.%d ", p_vlan_cfg->vlan_id);
	        p+=(strlen(p));
	    }
    port_mask=p_vlan_cfg->ssid_mask;
    for(i=0; port_mask>0; i++){
        if(port_mask&0x1){
            sprintf(p, "wlan%d ", i);
            p+=(strlen(p));
        }
        port_mask=port_mask>>1;
    }
    */
    /*
    port_mask=p_vlan_cfg->brwan_mask;
    if(port_mask>0){
        wan_type=mapi_ccfg_get_long(tid, "system", "wan_type", 0);
        switch(wan_type){
            case MAPI_CCFG_WAN_TYPE_ADSL:
                wan_base=MAPI_CCFG_WAN_IDBASE_ADSL;
                break;
            case MAPI_CCFG_WAN_TYPE_VDSL:
                wan_base=MAPI_CCFG_WAN_IDBASE_VDSL;
                break;
            case MAPI_CCFG_WAN_TYPE_ETHERNET:
                wan_base=MAPI_CCFG_WAN_IDBASE_ETHERNET;
                break;
            default:
                wan_base=MAPI_CCFG_WAN_IDBASE_ADSL;
                break;
        }
        for(i=0; port_mask>0; i++){
            if(port_mask&0x1){
                if(wan_type==MAPI_CCFG_WAN_TYPE_DSL_AUTO){
                    sprintf(wan_sec, "wan%03d", i+MAPI_CCFG_WAN_IDBASE_ADSL);
                    mapi_ccfg_get_str(tid, wan_sec, "ifname", wan_ifn1, 32, NULL);
                    sprintf(wan_sec, "wan%03d", i+MAPI_CCFG_WAN_IDBASE_VDSL);
                    mapi_ccfg_get_str(tid, wan_sec, "ifname", wan_ifn2, 32, NULL);
                    sprintf(p, "%s %s ", wan_ifn1, wan_ifn2);
                }
                else{
                    sprintf(wan_sec, "wan%03d", i+wan_base);
                    mapi_ccfg_get_str(tid, wan_sec, "ifname", wan_ifn1, 32, NULL);
                    sprintf(p, "%s ", wan_ifn1);
                }
                p+=(strlen(p));
            }
            port_mask=port_mask>>1;
        }
    }
*/
    /*
    if(p==tmp_buf){
        mapi_ccfg_set_str(tid, vlan_sec, "adminstate", "disable");
    }
    if(p>tmp_buf){
        *(--p)='\0';
        mapi_ccfg_set_str(tid, vlan_sec, "ifname", tmp_buf);
        mapi_ccfg_set_str(tid, vlan_sec, "adminstate", "enable");
    }
    */
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Enable/disable the user network VLAN feature
 *
 * Parameters
 *		tid:        transaction ID returned by calling mapi_start_transc()
 *                  use 0 if no tid is provided (no transaction was opened before)
 *		enable:     0 for disabling the user network VLAN feature;
 *                  else for enabling the user network VLAN feature
 *
 * Returns
 *		* MID_SUCCESS(0):	successfully
 *		* MID_FAIL(-1):		failed to process transaction or the variable does not exist
 *							or ptuple is NULL.
 *
 * Note
 *		* related sections: vlan, lan1, ... lanN (where N is MAPI_CCFG_VLAN_DFT_MAX_VLAN_NUM)
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_enable_vlan_global(int tid, int enable)
{
    int                 new_transc=0;
    int                     i, j, port_mask;
    char                vlan_sec[32];
    char                *p, tmp_buf[64];
    mapi_ccfg_vlan_t    vlan_cfg;
    mapi_ccfg_vlan_global_t vlan_glb;
    
    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }

    mapi_ccfg_get_vlan_global(tid, &vlan_glb, 0);
    vlan_glb.enable=enable;
    update_vlan_ifname_and_admin(tid, &vlan_glb);
        mapi_ccfg_set_long(tid, MAPI_CCFG_VLAN_SECTION_NAME, "enable_vlan", 1);
        
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    return MID_SUCCESS;    
}
