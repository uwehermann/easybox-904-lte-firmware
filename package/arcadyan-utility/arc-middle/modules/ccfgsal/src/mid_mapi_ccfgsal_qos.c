/*
 * Arcadyan middle layer APIs for qos operations
 *
 * Copyright 2010, Arcadyan Corporation
 * All Rights Reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mid_mapi_ccfgsal_qos.h"

/*
 * -- Helper functions --
 */
int qos_free_qm_fwg(mapi_ccfg_qos_qm_fwg_t* p_fwg)
{
    if(p_fwg==NULL)
        return MID_FAIL;
    free(p_fwg);
    
    return MID_SUCCESS;
}

int qos_free_qm_L2(mapi_ccfg_qos_qm_L2_t* p_L2)
{
    int i;
    
    if(p_L2==NULL)
        return MID_FAIL;
    for(i=0; i<p_L2->fwg_num; i++){
        qos_free_qm_fwg(p_L2->fwg[i]);
    }
    free(p_L2);
    
    return MID_SUCCESS;
}

int qos_free_qm_L1(mapi_ccfg_qos_qm_L1_t* p_L1)
{
    int i;
    
    if(p_L1==NULL)
        return MID_FAIL;
    for(i=0; i<p_L1->L2_num; i++){
        qos_free_qm_L2(p_L1->L2_grp[i]);
    }
    free(p_L1);
    
    return MID_SUCCESS;
}

int qos_free_qm_root(mapi_ccfg_qos_qm_root_t* p_qm)
{
    int i;
    
    if(p_qm==NULL)
        return MID_FAIL;
    for(i=0; i<p_qm->L1_num; i++){
        qos_free_qm_L1(p_qm->L1_grp[i]);
    }
    free(p_qm);
    
    return MID_SUCCESS;
}

int qos_free_profile(mapi_ccfg_qos_profile_t* qos_pf)
{
    if(qos_pf==NULL)
        return MID_FAIL;
        
    //free qm data
    qos_free_qm_root(qos_pf->qm);
    
    //free clsfy data
    free(qos_pf->clsfy);
    
    //free itself
    free(qos_pf);
    
    return MID_SUCCESS;
}

int qos_get_qm_fwg(int tid, mapi_ccfg_qos_qm_fwg_t* pfwg_data)
{
    char                sp_or_wrr, tmp_buf[128];

    //get fwg parameters
    mapi_ccfg_get_str(tid, pfwg_data->pname, pfwg_data->vname, tmp_buf, 128, NULL);
    sscanf(tmp_buf, "%x,%c,%d,%s",
           &(pfwg_data->id), &sp_or_wrr, &(pfwg_data->priwei), pfwg_data->name);
    pfwg_data->sp_or_wrr=(sp_or_wrr=='s'? MAPI_CCFG_QOS_FWG_SP:MAPI_CCFG_QOS_FWG_WRR);

    return MID_SUCCESS;
}

int qos_get_qm_L2(int tid, int L1_id, int L2_id, mapi_ccfg_qos_qm_L2_t* pL2_data)
{
    char                tmp_buf[128];
    char                rv_Kbps[MAPI_CCFG_QOS_BW_STR_LEN];
    char                ul_Kbps[MAPI_CCFG_QOS_BW_STR_LEN];
    char                egrs_if[MAPI_CCFG_QOS_MAX_L2_EGRESS_IF_NUM*MAPI_CCFG_QOS_IF_NAME_LEN];
    char                egrs_enslv_if[MAPI_CCFG_QOS_MAX_L2_EGRESS_IF_NUM*MAPI_CCFG_QOS_IF_NAME_LEN];
    char                sp_rt_para[16];
    int                 i;
    char                *p, *delim="+";

    //get L2 parameters
    egrs_if[0]='\0';
    egrs_enslv_if[0]='\0';
    mapi_ccfg_get_str(tid, pL2_data->pname, pL2_data->vname, tmp_buf, 128, NULL);
    sscanf(tmp_buf, "%x,%[^','],%[^','],%[^','],%[^','],%d,%d,%[^','],%d,%s",
           &(pL2_data->id), rv_Kbps, ul_Kbps, egrs_if, egrs_enslv_if,
           &(pL2_data->fwg_num), &(pL2_data->sp_fwg_num), sp_rt_para,
           &(pL2_data->wrr_reserved_bw), pL2_data->name);

    if(rv_Kbps[0]=='~')
        pL2_data->rv_Kbps=MAPI_CCFG_QOS_UNSPECIFIED_BW;
    else
        pL2_data->rv_Kbps=strtol(rv_Kbps, NULL, 0);

    if(ul_Kbps[0]=='~')
        pL2_data->ul_Kbps=MAPI_CCFG_QOS_UNSPECIFIED_BW;
    else
        pL2_data->ul_Kbps=strtol(ul_Kbps, NULL, 0);

    //parse egrs_if
    pL2_data->egrs_if_num=0;
    if(egrs_if[0]!='~'){   //here '~' stands for "nothing"
        if((p=strtok(egrs_if, delim))!=NULL){
            strncpy(pL2_data->egrs_if_name[0], p, MAPI_CCFG_QOS_IF_NAME_LEN-1);
            for(i=1; (p=strtok(NULL, delim))!=NULL; i++){
                strncpy(pL2_data->egrs_if_name[i], p, MAPI_CCFG_QOS_IF_NAME_LEN-1);
            }
            pL2_data->egrs_if_num=i;
        }
    }
//printf("egrs_if_num=%d\n", pL2_data->egrs_if_num);
    //parse egrs_enslv_if_num
    pL2_data->egrs_enslv_if_num=0;
    if(egrs_enslv_if[0]!='~'){   //here '~' stands for "nothing"
        if((p=strtok(egrs_enslv_if, delim))!=NULL){
            strncpy(pL2_data->egrs_enslv_if_name[0], p, MAPI_CCFG_QOS_IF_NAME_LEN-1);
            for(i=1; (p=strtok(NULL, delim))!=NULL; i++){
                strncpy(pL2_data->egrs_enslv_if_name[i], p, MAPI_CCFG_QOS_IF_NAME_LEN-1);
            }
            pL2_data->egrs_enslv_if_num=i;
        }
    }
//printf("egrs_enslv_if_num=%d\n", pL2_data->egrs_enslv_if_num);

    for(i=0; i<pL2_data->fwg_num; i++){
        if((pL2_data->fwg[i]=(mapi_ccfg_qos_qm_fwg_t *)malloc(sizeof(mapi_ccfg_qos_qm_fwg_t)))==NULL){
		    MID_TRACE("[%s] malloc failed for fwg[%d]\n", __FUNCTION__, i);
		    return MID_FAIL;
        }

        memset((char *)pL2_data->fwg[i], 0, sizeof(mapi_ccfg_qos_qm_fwg_t));
        strcpy(pL2_data->fwg[i]->pname, pL2_data->pname);
        sprintf(pL2_data->fwg[i]->vname, "fwg%d%d_%d", L1_id, L2_id, i);
        if(qos_get_qm_fwg(tid, pL2_data->fwg[i])==MID_FAIL){
		    MID_TRACE("[%s] qos_get_qm_fwg failed for fwg_[%d]\n", __FUNCTION__, i);
		    return MID_FAIL;
		}
    }
    
    return MID_SUCCESS;
}

int qos_get_qm_L1(int tid, int L1_id, mapi_ccfg_qos_qm_L1_t* pL1_data)
{
    int                 i;
    char                tmp_buf[128];
    char                ds_Kbps[MAPI_CCFG_QOS_BW_STR_LEN];
    char                us_Kbps[MAPI_CCFG_QOS_BW_STR_LEN];

    //get L1 parameters
    mapi_ccfg_get_str(tid, pL1_data->pname, pL1_data->vname, tmp_buf, 128, NULL);
    sscanf(tmp_buf, "%x,%[^','],%[^','],%d,%s",
           &(pL1_data->id), ds_Kbps, us_Kbps, &(pL1_data->L2_num), pL1_data->name);

    if(ds_Kbps[0]=='~')
        pL1_data->ds_Kbps=MAPI_CCFG_QOS_UNSPECIFIED_BW;
    else
        pL1_data->ds_Kbps=strtol(ds_Kbps, NULL, 0);

    if(us_Kbps[0]=='~')
        pL1_data->us_Kbps=MAPI_CCFG_QOS_UNSPECIFIED_BW;
    else
        pL1_data->us_Kbps=strtol(us_Kbps, NULL, 0);

    for(i=0; i<pL1_data->L2_num; i++){
        if((pL1_data->L2_grp[i]=(mapi_ccfg_qos_qm_L2_t *)malloc(sizeof(mapi_ccfg_qos_qm_L2_t)))==NULL){
		    MID_TRACE("[%s] malloc failed for L2_grp[%d]\n", __FUNCTION__, i);
		    return MID_FAIL;
        }

        memset((char *)pL1_data->L2_grp[i], 0, sizeof(mapi_ccfg_qos_qm_L2_t));
        strcpy(pL1_data->L2_grp[i]->pname, pL1_data->pname);
        sprintf(pL1_data->L2_grp[i]->vname, "vif%d_%d", L1_id, i);
        if(qos_get_qm_L2(tid, L1_id, i, pL1_data->L2_grp[i])==MID_FAIL){
		    MID_TRACE("[%s] qos_get_qm_L2 failed for L2_grp[%d]\n", __FUNCTION__, i);
		    return MID_FAIL;
		}
    }
    
    return MID_SUCCESS;
}

int qos_get_qm_root(int tid, char *pf_name, mapi_ccfg_qos_qm_root_t* pqm_data)
{
    int                 i;

    //pname
    strcpy(pqm_data->pname, pf_name);
    
    //ds_Kbps
    pqm_data->ds_Kbps=mapi_ccfg_get_long(tid, pf_name, "ds_Kbps", 0);

    //us_Kbps
    pqm_data->us_Kbps=mapi_ccfg_get_long(tid, pf_name, "us_Kbps", 0);
    
    //total_fwg_num
    pqm_data->total_fwg_num=mapi_ccfg_get_long(tid, pf_name, "total_fwg_num", 0);

    //L1_num
    pqm_data->L1_num=mapi_ccfg_get_long(tid, pf_name, "lnk_num", 0);

    //dft_fwg
    mapi_ccfg_get_str(tid, pf_name, "dft_fwg", pqm_data->dft_fwg, MAPI_CCFG_QOS_GROUP_NAME_LEN, NULL);

    for(i=0; i<pqm_data->L1_num; i++){
        if((pqm_data->L1_grp[i]=(mapi_ccfg_qos_qm_L1_t *)malloc(sizeof(mapi_ccfg_qos_qm_L1_t)))==NULL){
		    MID_TRACE("[%s] malloc failed for L1_grp[%d]\n", __FUNCTION__, i);
		    return MID_FAIL;
        }

        memset((char *)pqm_data->L1_grp[i], 0, sizeof(mapi_ccfg_qos_qm_L1_t));
        strcpy(pqm_data->L1_grp[i]->pname, pf_name);
        sprintf(pqm_data->L1_grp[i]->vname, "lnk%d", i);
        if(qos_get_qm_L1(tid, i, pqm_data->L1_grp[i])==MID_FAIL){
		    MID_TRACE("[%s] qos_get_qm_L1 failed for L1_grp[%d]\n", __FUNCTION__, i);
		    return MID_FAIL;
		}
    }
    
    return MID_SUCCESS;
}

/*
 * -- Public functions --
 */

// Please refer to mid_qos.h for the usage of the input data structures
// in the below functions


/************************************************************/
/******************** CORE FUNCTIONS ************************/
/************************************************************/

/*******************************************************************************
 * Description
 *		Update qm root configuration
 *
 * Parameters
 *		tid:	    transaction ID returned by calling mapi_start_transc()
 *		pqm_data:   pointer to the buffer of data structure mapi_ccfg_qos_qm_root_t.
 *	                Required input fields:
 *                      1. pqm_data->pname: the profile section name in .glbcfg
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or pdstip is NULL.
 *
 * Note
 *		* related sections: pqm_data->pname
 *
 * See Also
 *		mapi_ccfg_qos_set_qm_L1, mapi_ccfg_qos_set_qm_L2, mapi_ccfg_qos_set_qm_fwg
 *
 ******************************************************************************/
int mapi_ccfg_qos_set_qm_root(int tid, mapi_ccfg_qos_qm_root_t* pqm_data)
{
    int                 new_transc=0;
    
    if(pqm_data==NULL){
        MID_TRACE("[%s] NULL pqm_data\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    //ds_Kbps
    mapi_ccfg_set_long(tid, pqm_data->pname, "ds_Kbps", pqm_data->ds_Kbps);

    //us_Kbps
    mapi_ccfg_set_long(tid, pqm_data->pname, "us_Kbps", pqm_data->us_Kbps);
    
    /* not open for write
    //total_fwg_num
    mapi_ccfg_set_long(tid, pqm_data->pname, "total_fwg_num", pqm_data->total_fwg_num);
    */
    
    //L1_num
    mapi_ccfg_set_long(tid, pqm_data->pname, "lnk_num", pqm_data->L1_num);

    //dft_fwg
    mapi_ccfg_set_str(tid, pqm_data->pname, "dft_fwg", pqm_data->dft_fwg);

    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Update L1 configuration
 *
 * Parameters
 *		tid:	    transaction ID returned by calling mapi_start_transc()
 *		pL1_data:   pointer to the buffer of data structure mapi_ccfg_qos_qm_L1_t.
 *	                Required input fields:
 *                      1. pL1_data->pname: the profile section name in .glbcfg
 *                      2. pL1_data->vname: the variable name of the link
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or pdstip is NULL.
 *
 * Note
 *		* related sections: pL1_data->pname
 *
 * See Also
 *		mapi_ccfg_qos_set_qm_L2, mapi_ccfg_qos_set_qm_fwg
 *
 ******************************************************************************/
int mapi_ccfg_qos_set_qm_L1(int tid, mapi_ccfg_qos_qm_L1_t* pL1_data)
{
    int                 new_transc=0;
    char                tmp_buf[128];
    char                ds_Kbps[MAPI_CCFG_QOS_BW_STR_LEN];
    char                us_Kbps[MAPI_CCFG_QOS_BW_STR_LEN];
    
    if(pL1_data==NULL){
        MID_TRACE("[%s] NULL pL1_data\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }

    //prepare ds_Kbps and us_Kbps
    if(pL1_data->ds_Kbps==MAPI_CCFG_QOS_UNSPECIFIED_BW)
        strcpy(ds_Kbps, "~");
    else
        sprintf(ds_Kbps, "%d", pL1_data->ds_Kbps);

    if(pL1_data->us_Kbps==MAPI_CCFG_QOS_UNSPECIFIED_BW)
        strcpy(us_Kbps, "~");
    else
        sprintf(us_Kbps, "%d", pL1_data->us_Kbps);

    //set L1 parameters
    sprintf(tmp_buf, "%x,%s,%s,%d,%s",
            pL1_data->id, ds_Kbps, us_Kbps, pL1_data->L2_num, pL1_data->name);
    mapi_ccfg_set_str(tid, pL1_data->pname, pL1_data->vname, tmp_buf);

    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Update L2 configuration
 *
 * Parameters
 *		tid:	    transaction ID returned by calling mapi_start_transc()
 *		pL2_data:   pointer to the buffer of data structure mapi_ccfg_qos_qm_L2_t.
 *                      1. pL2_data->pname: the profile section name in .glbcfg
 *                      2. pL2_data->vname: the variable name of the link
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or pdstip is NULL.
 *
 * Note
 *		* related sections: pL2_data->pname
 *
 * See Also
 *		mapi_ccfg_qos_set_qm_L1, mapi_ccfg_qos_set_qm_fwg
 *
 ******************************************************************************/
int mapi_ccfg_qos_set_qm_L2(int tid, mapi_ccfg_qos_qm_L2_t* pL2_data)
{
    int                 new_transc=0;
    char                rv_Kbps[MAPI_CCFG_QOS_BW_STR_LEN];
    char                ul_Kbps[MAPI_CCFG_QOS_BW_STR_LEN];
    char                tmp_buf[128];
    char                egrs_if[MAPI_CCFG_QOS_MAX_L2_EGRESS_IF_NUM*MAPI_CCFG_QOS_IF_NAME_LEN];
    char                egrs_enslv_if[MAPI_CCFG_QOS_MAX_L2_EGRESS_IF_NUM*MAPI_CCFG_QOS_IF_NAME_LEN];
    char                sp_rt_para[]="6/30";
    int                 i;
    char                *p;
    
    if(pL2_data==NULL){
        MID_TRACE("[%s] NULL pL2_data\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    //prepare rv_Kbps and ul_Kbps
    if(pL2_data->rv_Kbps==MAPI_CCFG_QOS_UNSPECIFIED_BW)
        strcpy(rv_Kbps, "~");
    else
        sprintf(rv_Kbps, "%d", pL2_data->rv_Kbps);

    if(pL2_data->ul_Kbps==MAPI_CCFG_QOS_UNSPECIFIED_BW)
        strcpy(ul_Kbps, "~");
    else
        sprintf(ul_Kbps, "%d", pL2_data->ul_Kbps);

    //prepare egrs_if
    p=egrs_if;
    if(pL2_data->egrs_if_num==0){
        strcpy(p, "~");
    }
    else{
        for(i=0; i<pL2_data->egrs_if_num; i++){
            sprintf(p, "%s+", pL2_data->egrs_if_name[i]);
            if(i<pL2_data->egrs_if_num-1)
                p+=(strlen(p));
            else
                *(p+strlen(p)-1)='\0';
        }
    }
    //prepare egrs_enslv_if_num
    p=egrs_enslv_if;
    if(pL2_data->egrs_enslv_if_num==0){
        strcpy(p, "~");
    }
    else{
        for(i=0; i<pL2_data->egrs_enslv_if_num; i++){
            sprintf(p, "%s+", pL2_data->egrs_enslv_if_name[i]);
            if(i<pL2_data->egrs_enslv_if_num-1)
                p+=(strlen(p));
            else
                *(p+strlen(p)-1)='\0';
        }
    }
    //set L2 parameters
    sprintf(tmp_buf, "%x,%s,%s,%s,%s,%d,%d,%s,%d,%s",
           pL2_data->id, rv_Kbps, ul_Kbps, egrs_if, egrs_enslv_if,
           pL2_data->fwg_num, pL2_data->sp_fwg_num, sp_rt_para,
           pL2_data->wrr_reserved_bw, pL2_data->name);
           
    mapi_ccfg_set_str(tid, pL2_data->pname, pL2_data->vname, tmp_buf);
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Update fwg configuration
 *
 * Parameters
 *		tid:	    transaction ID returned by calling mapi_start_transc()
 *		pfwg_data:  pointer to the buffer of data structure mapi_ccfg_qos_qm_fwg_t.
 *                      1. pfwg_data->pname: the profile section name in .glbcfg
 *                      2. pfwg_data->vname: the variable name of the link
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or pdstip is NULL.
 *
 * Note
 *		* related sections: pfwg_data->pname
 *
 * See Also
 *		mapi_ccfg_qos_set_qm_L1, mapi_ccfg_qos_set_qm_L2
 *
 ******************************************************************************/
int mapi_ccfg_qos_set_qm_fwg(int tid, mapi_ccfg_qos_qm_fwg_t* pfwg_data)
{
    int                 new_transc=0;
    char                tmp_buf[128];
   
    if(pfwg_data==NULL){
        MID_TRACE("[%s] NULL pfwg_data\n", __FUNCTION__);
        
        return MID_FAIL;
    }

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }

    //set fwg parameters
    sprintf(tmp_buf, "%x,%c,%d,%s",
            pfwg_data->id, (pfwg_data->sp_or_wrr==MAPI_CCFG_QOS_FWG_SP? 's':'w'), pfwg_data->priwei, pfwg_data->name);     
    mapi_ccfg_set_str(tid, pfwg_data->pname, pfwg_data->vname, tmp_buf);
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Allocate and retrive default QOS configuration
 *
 * Parameters
 *		tid:	        transaction ID returned by calling mapi_start_transc()
 *		pf_name:        name of the qos profile.
 *      factory_dft:    use 1 to get factory default configuration
 *
 * Returns
 *		pointer to the global qos profile variable; NULL if failed
 *
 * See Also
 *
 ******************************************************************************/
mapi_ccfg_qos_profile_t *mapi_ccfg_qos_open_profile(int tid, char *pf_name, int factory_dft)
{
    int                 i, new_transc=0;
    mapi_ccfg_qos_profile_t  *qos_pf=NULL;

    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return NULL;
        }
    }
    
    if((qos_pf=(mapi_ccfg_qos_profile_t*)malloc(sizeof(mapi_ccfg_qos_profile_t)))==NULL){
        MID_TRACE("[%s] malloc(mapi_ccfg_qos_profile_t) failed, size %d\n", __FUNCTION__, sizeof(mapi_ccfg_qos_profile_t));
        
        return NULL;
    }

    memset((char *)qos_pf, 0, sizeof(*qos_pf));
    if((qos_pf->qm=(mapi_ccfg_qos_qm_root_t*)malloc(sizeof(mapi_ccfg_qos_qm_root_t)))==NULL){
        MID_TRACE("[%s] malloc(mapi_ccfg_qos_qm_root_t) failed, size %d\n", __FUNCTION__, sizeof(mapi_ccfg_qos_profile_t));
            
        return NULL;
    }
    strcpy(qos_pf->name, pf_name);

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 1);
    }

    if(qos_get_qm_root(tid, pf_name, qos_pf->qm)==MID_FAIL){
        MID_TRACE("[%s] qos_get_qm_root() failed\n", __FUNCTION__);
        
        qos_free_profile(qos_pf);

        if(factory_dft){
            mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
        }

        return NULL;
    }
    
    //clsfy
    if((qos_pf->clsfy=(mapi_ccfg_qos_clsfy_t *)malloc(sizeof(mapi_ccfg_qos_clsfy_t)))==NULL){
        MID_TRACE("[%s] malloc(mapi_ccfg_qos_clsfy_t) failed\n", __FUNCTION__);
        
        qos_free_profile(qos_pf);

        if(factory_dft){
            mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
        }

        return NULL;
    }
    
    memset((char *)qos_pf->clsfy, 0, sizeof(mapi_ccfg_qos_clsfy_t));
    strcpy(qos_pf->clsfy->ru_order.pname, pf_name);
    mapi_ccfg_get_str(tid, pf_name, "clsfy_seq", qos_pf->clsfy->ru_order.seq, MAPI_CCFG_QOS_CLSFY_SEQUENCE_STR_LEN, NULL);

    if(factory_dft){
        mapi_ccfg_switch_in_out_factory_cfg(tid, 0);
    }    

    for(i=0; i<MAPI_QOS_CLSFY_MAX_RU_NUM; i++){
        strcpy(qos_pf->clsfy->ru[i].pname, pf_name);
        qos_pf->clsfy->ru[i].id=i;
        mapi_ccfg_qos_get_clsfy_ru(tid, &(qos_pf->clsfy->ru[i]), factory_dft);
    }

    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            qos_free_profile(qos_pf);
            
            return NULL;
        }
    }
    
    return qos_pf;
}

/*******************************************************************************
 * Description
 *		Free allocated default QOS configuration
 *
 * Parameters
 *      qos_pf:     pointer to the previous allocated qos profile
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or pdstip is NULL.
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_qos_close_profile(mapi_ccfg_qos_profile_t *qos_pf)
{
    if(qos_pf!=NULL){
        qos_free_profile(qos_pf);
        
        return MID_SUCCESS;
    }
    
    return MID_FAIL;
}

/*******************************************************************************
 * Description
 *		Get current QOS global state - active profile and enable/disable
 *
 * Parameters
 *		tid:	        transaction ID returned by calling mapi_start_transc()
 *      pglb:           pointer to the buffer of data structure mapi_ccfg_qos_global_t
 *      factory_dft:    use 1 to get factory default configuration
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or pstatus is NULL.
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_qos_get_global(int tid, mapi_ccfg_qos_global_t *pglb, int factory_dft)
{
    int                 new_transc=0;

	if(pglb==NULL){
		MID_TRACE( "[%s] NULL pglb\n", __FUNCTION__ );
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
        
    //profile
    mapi_ccfg_get_str(tid, "qos_glb", "profile", pglb->profile, MAPI_CCFG_QOS_PROFILE_NAME_LEN, NULL);
    
    //enable
    pglb->enable=mapi_ccfg_get_long(tid, "qos_glb", "enable", 0);

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
 *		Set current QOS global state - active profile and enable/disable
 *
 * Parameters
 *		tid:	    transaction ID returned by calling mapi_start_transc()
 *      pglb:       pointer to the buffer of data structure mapi_ccfg_qos_global_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or pstatus is NULL.
 *
 * Note
 *		* related sections: qos_glb
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_qos_set_global(int tid, mapi_ccfg_qos_global_t *pglb)
{
    int                 new_transc=0;

	if(pglb==NULL){
		MID_TRACE( "[%s] NULL pglb\n", __FUNCTION__ );
		return MID_FAIL;
	}
	
    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    //profile
    mapi_ccfg_set_str(tid, "qos_glb", "profile", pglb->profile);
    
    //enable
    mapi_ccfg_set_long(tid, "qos_glb", "enable", pglb->enable);
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Get classification rule
 *
 * Parameters
 *		tid:	        transaction ID returned by calling mapi_start_transc()
 *      pru:            pointer to the buffer of data structure mapi_ccfg_qos_clsfy_ru_t
 *      factory_dft:    use 1 to get factory default configuration
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or pstatus is NULL.
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_qos_get_clsfy_ru(int tid, mapi_ccfg_qos_clsfy_ru_t* pru, int factory_dft)
{
    int                 new_transc=0;
    char                vname[16], tmp_buf[128], dscp[8], _8021p[8];
    
	if(pru==NULL){
		MID_TRACE( "[%s] NULL pru\n", __FUNCTION__ );
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
    
    //name
    sprintf(vname, "ru%02x_name", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, pru->name, MAPI_CCFG_QOS_CLSFY_RU_NAME_LEN, NULL);
    if(strcmp(pru->name, "~")==0){
        goto get_clsfy_ru_end;
    }
    
    //smac
    sprintf(vname, "ru%02x_smac", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, pru->smac, MAPI_CCFG_QOS_CLSFY_RU_MAC_STR_LEN, NULL);

    //inif
    sprintf(vname, "ru%02x_inif", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, pru->inif, MAPI_CCFG_QOS_CLSFY_RU_INIF_STR_LEN, NULL);

    //inphyif
    sprintf(vname, "ru%02x_inphyif", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, pru->inphyif, MAPI_CCFG_QOS_CLSFY_RU_INIF_STR_LEN, NULL);

    //srcip
    sprintf(vname, "ru%02x_srcip", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, pru->srcip, MAPI_CCFG_QOS_CLSFY_RU_IP_STR_LEN, NULL);

    //dstip
    sprintf(vname, "ru%02x_dstip", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, pru->dstip, MAPI_CCFG_QOS_CLSFY_RU_IP_STR_LEN, NULL);

    //approto
    sprintf(vname, "ru%02x_approto", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, pru->approto, MAPI_CCFG_QOS_CLSFY_RU_APPROTO_STR_LEN, NULL);

    //sport
    sprintf(vname, "ru%02x_sport", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, pru->sport, MAPI_CCFG_QOS_CLSFY_RU_PORT_STR_LEN, NULL);

    //dport
    sprintf(vname, "ru%02x_dport", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, pru->dport, MAPI_CCFG_QOS_CLSFY_RU_PORT_STR_LEN, NULL);

    //dscp
    sprintf(vname, "ru%02x_dscp", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, pru->dscp, MAPI_CCFG_QOS_CLSFY_RU_DSCP_STR_LEN, NULL);

    //8021p
    sprintf(vname, "ru%02x_8021p", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, pru->_8021p, MAPI_CCFG_QOS_CLSFY_RU_8021P_STR_LEN, NULL);

    //ethtype
    sprintf(vname, "ru%02x_ethtype", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, pru->ethtype, MAPI_CCFG_QOS_CLSFY_RU_ETHTYPE_STR_LEN, NULL);

    //pkylen
    sprintf(vname, "ru%02x_pkylen", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, pru->pkylen, MAPI_CCFG_QOS_CLSFY_RU_PKTLEN_STR_LEN, NULL);
    
    //domain
    sprintf(vname, "ru%02x_domain", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, pru->domain, MAPI_CCFG_QOS_CLSFY_RU_DOMAIN_STR_LEN, NULL);

    //dhcpopt60
    sprintf(vname, "ru%02x_dhcpopt60", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, pru->dhcpopt60, MAPI_CCFG_QOS_CLSFY_RU_OPTION60_STR_LEN, NULL);

    //extra_match
    sprintf(vname, "ru%02x_exm", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, pru->extra_match, MAPI_CCFG_QOS_CLSFY_RU_EXTRAMATCH_STR_LEN, NULL);

    //action
    sprintf(vname, "ru%02x_act", pru->id);
    mapi_ccfg_get_str(tid, pru->pname, vname, tmp_buf, 128, NULL);
    sscanf(tmp_buf, "%[^','],%[^','],%[^','],", dscp, _8021p, pru->fwg_name);
    if(strcmp(dscp, "no")==0){
        pru->dscp_remark=0xffff;
    }
    else{
        pru->dscp_remark=strtol(dscp, NULL, 0);
    }
    if(strcmp(_8021p, "no")==0){
        pru->_1p_remark=0xffff;
    }
    else{
        pru->_1p_remark=strtol(_8021p, NULL, 0);
    }
    
get_clsfy_ru_end:
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
 *		Set classification rule
 *
 * Parameters
 *		tid:	        transaction ID returned by calling mapi_start_transc()
 *      pru:            pointer to the buffer of data structure mapi_ccfg_qos_clsfy_ru_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or pstatus is NULL.
 *
 * Note
 *		* related sections: pru->pname
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_qos_set_clsfy_ru(int tid, mapi_ccfg_qos_clsfy_ru_t* pru)
{
    int                 new_transc=0;
    char                vname[16], tmp_buf[128], dscp[8], _8021p[8];
    
	if(pru==NULL){
		MID_TRACE( "[%s] NULL pru\n", __FUNCTION__ );
		return MID_FAIL;
	}
	
    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    //name
    sprintf(vname, "ru%02x_name", pru->id);
    mapi_ccfg_set_str(tid, pru->pname, vname, pru->name);
    
    if(strcmp(pru->name, "~")==0){
        return MID_SUCCESS;
    }
    
    //smac
    sprintf(vname, "ru%02x_smac", pru->id);
    mapi_ccfg_set_str(tid, pru->pname, vname, pru->smac);

    //inif
    sprintf(vname, "ru%02x_inif", pru->id);
    mapi_ccfg_set_str(tid, pru->pname, vname, pru->inif);

    //inphyif
    sprintf(vname, "ru%02x_inphyif", pru->id);
    mapi_ccfg_set_str(tid, pru->pname, vname, pru->inphyif);

    //srcip
    sprintf(vname, "ru%02x_srcip", pru->id);
    mapi_ccfg_set_str(tid, pru->pname, vname, pru->srcip);

    //dstip
    sprintf(vname, "ru%02x_dstip", pru->id);
    mapi_ccfg_set_str(tid, pru->pname, vname, pru->dstip);

    //approto
    sprintf(vname, "ru%02x_approto", pru->id);
    mapi_ccfg_set_str(tid, pru->pname, vname, pru->approto);

    //sport
    sprintf(vname, "ru%02x_sport", pru->id);
    mapi_ccfg_set_str(tid, pru->pname, vname, pru->sport);

    //dport
    sprintf(vname, "ru%02x_dport", pru->id);
    mapi_ccfg_set_str(tid, pru->pname, vname, pru->dport);

    //dscp
    sprintf(vname, "ru%02x_dscp", pru->id);
    mapi_ccfg_set_str(tid, pru->pname, vname, pru->dscp);

    //8021p
    sprintf(vname, "ru%02x_8021p", pru->id);
    mapi_ccfg_set_str(tid, pru->pname, vname, pru->_8021p);

    //ethtype
    sprintf(vname, "ru%02x_ethtype", pru->id);
    mapi_ccfg_set_str(tid, pru->pname, vname, pru->ethtype);

    //pkylen
    sprintf(vname, "ru%02x_pkylen", pru->id);
    mapi_ccfg_set_str(tid, pru->pname, vname, pru->pkylen);

    //domain
    sprintf(vname, "ru%02x_domain", pru->id);
    mapi_ccfg_set_str(tid, pru->pname, vname, pru->domain);

    //dhcpopt60
    sprintf(vname, "ru%02x_dhcpopt60", pru->id);
    mapi_ccfg_set_str(tid, pru->pname, vname, pru->dhcpopt60);

    //extra_match
    sprintf(vname, "ru%02x_exm", pru->id);
    mapi_ccfg_set_str(tid, pru->pname, vname, pru->extra_match);

    //action
    sprintf(vname, "ru%02x_act", pru->id);
    if(pru->dscp_remark==0xffff){
        strcpy(dscp, "no");
    }
    else{
        sprintf(dscp, "0x%x", pru->dscp_remark);
    }
    if(pru->_1p_remark==0xffff){
        strcpy(_8021p, "no");
    }
    else{
        sprintf(_8021p, "%d", pru->_1p_remark);
    }
    sprintf(tmp_buf, "%s,%s,%s,", dscp, _8021p, pru->fwg_name);
    mapi_ccfg_set_str(tid, pru->pname, vname, tmp_buf);

    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    return MID_SUCCESS;
}

/*******************************************************************************
 * Description
 *		Get order of classification rule
 *
 * Parameters
 *		tid:	        transaction ID returned by calling mapi_start_transc()
 *      porder:         pointer to the buffer of data structure mapi_ccfg_qos_clsfy_order_t
 *      factory_dft:    use 1 to get factory default configuration
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or pstatus is NULL.
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_qos_get_clsfy_order(int tid, mapi_ccfg_qos_clsfy_order_t* porder, int factory_dft)
{
    int                 new_transc=0;

	if(porder==NULL){
		MID_TRACE( "[%s] NULL porder\n", __FUNCTION__ );
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
        
    //clsfy_seq
    mapi_ccfg_get_str(tid, porder->pname, "clsfy_seq", porder->seq, MAPI_CCFG_QOS_CLSFY_SEQUENCE_STR_LEN, NULL);

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
 *		Set order of classification rule
 *
 * Parameters
 *		tid:	        transaction ID returned by calling mapi_start_transc()
 *      porder:         pointer to the buffer of data structure mapi_ccfg_qos_clsfy_order_t
 *
 * Returns
 *		MID_SUCCESS(0): successfully
 *		MID_FAIL(-1):   failed to process transaction or pstatus is NULL.
 *
 * Note
 *		* related sections:porder->pname
 *
 * See Also
 *
 ******************************************************************************/
int mapi_ccfg_qos_set_clsfy_order(int tid, mapi_ccfg_qos_clsfy_order_t* porder)
{
    int                 new_transc=0;

	if(porder==NULL){
		MID_TRACE( "[%s] NULL porder\n", __FUNCTION__ );
		return MID_FAIL;
	}
	
    if(tid==0){
        new_transc=1;
        
        if((tid=mapi_start_transc())==MID_FAIL){
            return MID_FAIL;
        }
    }
        
    //clsfy_seq
    mapi_ccfg_set_str(tid, porder->pname, "clsfy_seq", porder->seq);
    
    if(new_transc){
        if(mapi_end_transc(tid)==MID_FAIL){
            return MID_FAIL;
        }
    }
    
    return MID_SUCCESS;
}