/*
 * Arcadyan middle layer APIs for NAT operations
 *
 * Copyright 2010, Arcadyan Corporation
 * All Rights Reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "libArcComApi.h"
#include "mid_mapi_ccfgsal_nat.h"

#define _DBG_CCFG_NAT_LV	0

int mapi_ccfgsal_nat_get_NATStatus(int tid){
	int isEnable = 0; //, idxIf = MID_FAIL;

//	idxIf = mapi_ccfg_idx_search_any_by_val( mapi_ccfg_tid, "NAT", "if", ifname );
//	if( idxIf != MID_FAIL)
//		isEnable = mapi_ccfg_idx_get_long(mapi_ccfg_tid, "NAT", "func",idxIf ,0);
	isEnable = mapi_ccfg_get_long(tid, "NAT", "func", 0);
	return isEnable;
}


//**************************************************************************************************
// FUNCTION:  mapi_ccfgsal_nat_set_NATStatus
// PURPOSE:   enable/disable NAT functions
// PARAMETERS:
// 			iisEnable	: 1-enable, 0-disable
//			ifname		: array of names of interface types, list of "lan" or "wlan"
// RETURN:
//
// Note
//		* related sections: NAT
//
//***************************************************************************************************
void mapi_ccfgsal_nat_set_NATStatus(int tid, int isEnable, char** ifname, int sizeif){
	int idxIf = MID_FAIL;
	int i;

	printf("[mapi_ccfgsal_nat_set_NATStatus] set NAT %s on %d interface", (isEnable==0)?"Disable":"Enable", sizeif);
	mapi_ccfg_set_long(tid, "NAT", "func", isEnable);
	for(i=0; i<sizeif; i++){
		printf(" %s", ifname[i]);
		idxIf = mapi_ccfg_idx_search_any_by_val( tid, "NAT", "if", ifname[i] );
		mapi_ccfg_idx_set_long(tid, "NAT", "func",idxIf ,isEnable);
	}
	printf("\n");

}


void mapi_ccfgsal_nat_get_Conf2PubServ(int tid, struct PubServ_t *pub_serv, int idx){
	//convert idx from GUI to config before call this function
	char umngVarName[32], tmpBuf[50], strBuf[30];
	unsigned int i;
	unsigned long lIpF=0, lIpT=0;


	mapi_ccfg_idx_get_str( tid, "NATPubServ", "group_name", idx, pub_serv->group_name, sizeof(pub_serv->group_name), "");
	pub_serv->group_member_num = mapi_ccfg_idx_get_long( tid, "NATPubServ", "group_member_num", idx, 0);
	memset(pub_serv->Global_Port, 0, sizeof(struct IPRange_t) * PORT_RANGE_GROUP_MAX);
	for(i=0; i<PORT_RANGE_GROUP_MAX ; i++){
		sprintf(umngVarName, "Global_Port%d", i);
		mapi_ccfg_idx_get_str( tid, "NATPubServ", umngVarName, idx, tmpBuf, sizeof(tmpBuf), "");
		if(strcmp(tmpBuf, "") != 0){
			strncpy(strchr(tmpBuf,',')," ",1);
			strncpy(strchr(tmpBuf,',')," ",1);
			sscanf(tmpBuf, "%s %hd %hd", strBuf, &pub_serv->Global_Port[i].b_port, &pub_serv->Global_Port[i].e_port);
			if(strcmp(strBuf,"TCP") == 0)
				pub_serv->Global_Port[i].Protocol = TCP_PROTO;
			else if(strcmp(strBuf,"UDP") == 0)
				pub_serv->Global_Port[i].Protocol = UDP_PROTO;
			else
				printf("--------->[%s]%d unknow PROTCOL %s\n", __FUNCTION__, __LINE__,strBuf);
		}
	}
	mapi_ccfg_idx_get_str( tid, "NATPubServ", "Protocol", idx, tmpBuf, sizeof(tmpBuf), "");
	if(strcmp(tmpBuf, "TCP") == 0)
		pub_serv->Protocol = TCP_PROTO;
	else if(strcmp(tmpBuf, "UDP") == 0)
		pub_serv->Protocol = UDP_PROTO;
	else
		pub_serv->Protocol = 0;

	memset(pub_serv->Global_IP, 0, sizeof(struct IPRange_t) * IP_RANGE_GROUP_MAX);
	for(i=0; i<IP_RANGE_GROUP_MAX ; i++){
		sprintf(umngVarName, "Global_IP%d", i);
		mapi_ccfg_idx_get_str( tid, "NATPubServ", umngVarName, idx, tmpBuf, sizeof(tmpBuf), "");
		if(strcmp(tmpBuf, "") != 0){
			getStrInList(tmpBuf, ',', IPRANGE_SRU_IPF, strBuf, sizeof(strBuf));
			inet_pton(AF_INET, strBuf, &lIpF);
			pub_serv->Global_IP[i].ip = lIpF;
			getStrInList(tmpBuf, ',', IPRANGE_SRU_IPT, strBuf, sizeof(strBuf));
			inet_pton(AF_INET, strBuf, &lIpT);
			pub_serv->Global_IP[i].count = lIpT - lIpF +1;
		}
	}
	mapi_ccfg_idx_get_str( tid, "NATPubServ", "Private_IP", idx, tmpBuf, sizeof(tmpBuf), "");
	inet_pton(AF_INET, tmpBuf, &pub_serv->Private_IP);
	memset(pub_serv->Private_Port, 0, sizeof(struct IPRange_t) * PORT_RANGE_GROUP_MAX);
	for(i=0; i<PORT_RANGE_GROUP_MAX ; i++){
		sprintf(umngVarName, "Private_Port%d", i);
		mapi_ccfg_idx_get_str( tid, "NATPubServ", umngVarName, idx, tmpBuf, sizeof(tmpBuf), "");
		if(strcmp(tmpBuf, "") != 0){
			strncpy(strchr(tmpBuf,',')," ",1);
			strncpy(strchr(tmpBuf,',')," ",1);
			sscanf(tmpBuf, "%s %hd %hd", strBuf, &pub_serv->Private_Port[i].b_port, &pub_serv->Private_Port[i].e_port);
			if(strcmp(strBuf,"TCP") == 0)
				pub_serv->Private_Port[i].Protocol = TCP_PROTO;
			else if(strcmp(strBuf,"UDP") == 0)
				pub_serv->Private_Port[i].Protocol = UDP_PROTO;
			else{
				printf("--------->[%s]%d unknow PROTCOL %s\n", __FUNCTION__, __LINE__,strBuf);
				strcpy(strBuf,"0");
			}
		}
	}
	pub_serv->Netno = mapi_ccfg_idx_get_long( tid, "NATPubServ", "Netno", idx, 0);
	mapi_ccfg_idx_get_str( tid, "NATPubServ", "Service_Name", idx, pub_serv->Service_Name, sizeof(pub_serv->Service_Name), "");
	pub_serv->leaseTime = mapi_ccfg_idx_get_long( tid, "NATPubServ", "leaseTime", idx, 0);
	mapi_ccfg_idx_get_str( tid, "NATPubServ", "remoteIP", idx, tmpBuf, sizeof(tmpBuf), "");
	sscanf(tmpBuf, "%d.%d.%d.%d", (int *)(&pub_serv->remoteIP[0]), (int *)(&pub_serv->remoteIP[1]), (int *)(&pub_serv->remoteIP[2]), (int *)(&pub_serv->remoteIP[3]));
	pub_serv->enable = mapi_ccfg_idx_get_long( tid, "NATPubServ", "func", idx, 0);
}


//**************************************************************************************************
// FUNCTION:  mapi_ccfgsal_nat_set_PubServ2Conf
// PURPOSE:   set special application
// PARAMETERS:
// 			idx 	 : index of entry
//			pub_serv : new settings
// RETURN:
//
// Note
//		* related sections: NATPubServ
//
//***************************************************************************************************
void mapi_ccfgsal_nat_set_PubServ2Conf(int tid, int idx, struct PubServ_t *pub_serv){
	//convert idx from GUI to config before call this function
	char umngVarName[32], tmpBuf[50];
	char strBuf[30];
	unsigned int i;

	//clear all section by index
	mapi_ccfg_idx_unset_all( tid, "NATPubServ", idx);

	mapi_ccfg_idx_set_str( tid, "NATPubServ", "group_name", idx, pub_serv->group_name);
	mapi_ccfg_idx_set_long( tid, "NATPubServ", "group_member_num", idx, pub_serv->group_member_num);
	for(i=0; i<PORT_RANGE_GROUP_MAX ; i++){
		if(pub_serv->Global_Port[i].Protocol == TCP_PROTO)
			strcpy(strBuf,"TCP");
		else if(pub_serv->Global_Port[i].Protocol == UDP_PROTO)
			strcpy(strBuf,"UDP");
		else{
			printf("--------->[%s]%d unknow PROTCOL %d\n", __FUNCTION__, __LINE__, pub_serv->Global_Port[i].Protocol);
			strcpy(strBuf,"0");
		}
		sprintf(tmpBuf, "%s,%d,%d",strBuf, pub_serv->Global_Port[i].b_port, pub_serv->Global_Port[i].e_port);
		sprintf(umngVarName, "Global_Port%d", i);
		mapi_ccfg_idx_set_str( tid, "NATPubServ", umngVarName, idx, tmpBuf);
	}
	if(pub_serv->Protocol == TCP_PROTO)
		mapi_ccfg_idx_set_str( tid, "NATPubServ", "Protocol", idx, "TCP");
	else if(pub_serv->Protocol == UDP_PROTO)
		mapi_ccfg_idx_set_str( tid, "NATPubServ", "Protocol", idx, "UDP");
	else
		mapi_ccfg_idx_set_str( tid, "NATPubServ", "Protocol", idx, "TCPUDP");
	for(i=0; i<IP_RANGE_GROUP_MAX ; i++){
		inet_ntop(AF_INET, &pub_serv->Global_IP[i].ip, strBuf, sizeof(strBuf)-1);
		strcpy(tmpBuf, strBuf);
		pub_serv->Global_IP[i].ip += (pub_serv->Global_IP[i].count-1);
		inet_ntop(AF_INET, &pub_serv->Global_IP[i].ip, strBuf, sizeof(strBuf)-1);
		sprintf(tmpBuf, "%s,%s",tmpBuf, strBuf);
		sprintf(umngVarName, "Global_IP%d", i);
		mapi_ccfg_idx_set_str( tid, "NATPubServ", umngVarName, idx, tmpBuf);
	}
	inet_ntop(AF_INET, &pub_serv->Private_IP, tmpBuf, sizeof(tmpBuf));
	mapi_ccfg_idx_set_str( tid, "NATPubServ", "Private_IP", idx, tmpBuf);

	for(i=0; i<PORT_RANGE_GROUP_MAX ; i++){
		if(pub_serv->Private_Port[i].Protocol == TCP_PROTO)
			strcpy(strBuf,"TCP");
		else if(pub_serv->Private_Port[i].Protocol == UDP_PROTO)
			strcpy(strBuf,"UDP");
		else{
			printf("--------->[%s]%d unknow PROTCOL %d\n", __FUNCTION__, __LINE__, pub_serv->Private_Port[i].Protocol);
			strcpy(strBuf,"0");
		}
		sprintf(tmpBuf, "%s,%d,%d", strBuf, pub_serv->Private_Port[i].b_port, pub_serv->Private_Port[i].e_port);
		sprintf(umngVarName, "Private_Port%d", i);
		mapi_ccfg_idx_set_str( tid, "NATPubServ", umngVarName, idx, tmpBuf);
	}
	mapi_ccfg_idx_set_long( tid, "NATPubServ", "Netno", idx, pub_serv->Netno);
	mapi_ccfg_idx_set_str( tid, "NATPubServ", "Service_Name", idx, pub_serv->Service_Name);
	mapi_ccfg_idx_set_long( tid, "NATPubServ", "leaseTime", idx, pub_serv->leaseTime);
	sprintf(tmpBuf, "%d.%d.%d.%d", (int)pub_serv->remoteIP[0], (int)pub_serv->remoteIP[1], (int)pub_serv->remoteIP[2], (int)pub_serv->remoteIP[3]);
	mapi_ccfg_idx_set_str( tid, "NATPubServ", "remoteIP", idx, tmpBuf);
	mapi_ccfg_idx_set_long( tid, "NATPubServ", "func", idx, pub_serv->enable);
}


void mapi_ccfgsal_nat_get_AddressMapping(int tid, int idx, struct AddrMapping_t *addr_mapping){

	char tmpBuf[32];

	addr_mapping->func = mapi_ccfg_idx_get_long( tid, "NATAddressMapping", "func", idx, 0 );

	addr_mapping->ifno = mapi_ccfg_idx_get_long( tid, "NATAddressMapping", "ifno", idx, -1 );

	mapi_ccfg_idx_get_str( tid, "NATAddressMapping", "srcipb", idx, tmpBuf, sizeof(tmpBuf), "0.0.0.0" );
	inet_pton(AF_INET, tmpBuf, &(addr_mapping->srcipb));
	addr_mapping->srcipb = ntohl(addr_mapping->srcipb);

	mapi_ccfg_idx_get_str( tid, "NATAddressMapping", "dstipb", idx, tmpBuf, sizeof(tmpBuf), "0.0.0.0" );
	inet_pton(AF_INET, tmpBuf, &(addr_mapping->dstipb));
	addr_mapping->dstipb = ntohl(addr_mapping->dstipb);

	mapi_ccfg_idx_get_str( tid, "NATAddressMapping", "dstipe", idx, tmpBuf, sizeof(tmpBuf), "0.0.0.0" );
	inet_pton(AF_INET, tmpBuf, &(addr_mapping->dstipe));
	addr_mapping->dstipe = ntohl(addr_mapping->dstipe);
}


//**************************************************************************************************
// FUNCTION:  mapi_ccfgsal_nat_set_AddressMapping
// PURPOSE:   set address mapping
// PARAMETERS:
// 			idx 		 : index of entry
//			addr_mapping : new settings
// RETURN:
//
// Note
//		* related sections: NATAddressMapping
//
//***************************************************************************************************
void mapi_ccfgsal_nat_set_AddressMapping(int tid, int idx, struct AddrMapping_t *addr_mapping){

	char tmpBuf[32];

	//ip address in network order at config

	mapi_ccfg_idx_set_long( tid, "NATAddressMapping", "func", idx, addr_mapping->func );

	mapi_ccfg_idx_set_long( tid, "NATAddressMapping", "ifno", idx, addr_mapping->ifno );

	addr_mapping->srcipb = htonl(addr_mapping->srcipb);
	inet_ntop(AF_INET, &(addr_mapping->srcipb), tmpBuf, sizeof(tmpBuf)-1);
	mapi_ccfg_idx_set_str( tid, "NATAddressMapping", "srcipb", idx, tmpBuf );

	addr_mapping->dstipb = htonl(addr_mapping->dstipb);
	inet_ntop(AF_INET, &(addr_mapping->dstipb), tmpBuf, sizeof(tmpBuf)-1);
	mapi_ccfg_idx_set_str( tid, "NATAddressMapping", "dstipb", idx, tmpBuf );

	addr_mapping->dstipe = htonl(addr_mapping->dstipe);
	inet_ntop(AF_INET, &(addr_mapping->dstipe), tmpBuf, sizeof(tmpBuf)-1);
	mapi_ccfg_idx_set_str( tid, "NATAddressMapping", "dstipe", idx, tmpBuf );

}

void mapi_ccfgsal_nat_get_DMZ(int tid, int idx, struct Zone_t *zone){

	char RuleList[255];
	int sizeRuleList = 0, i=-1, tmpIdx;


	mapi_ccfg_get_str( tid, "DMZ", "RuleList", RuleList, sizeof(RuleList), "" );
	sizeRuleList = getListSize(RuleList, ',');
	if(sizeRuleList > 0){
		for(i=0; i<sizeRuleList; i++){
			tmpIdx = getItemInList(RuleList, ',',i,-1);
			if(tmpIdx == idx){
				mapi_ccfg_idx_get_str( tid, "DMZ", "g_addr", idx, zone->g_addr, sizeof(zone->g_addr), "0.0.0.0" );
				mapi_ccfg_idx_get_str( tid, "DMZ", "l_addr", idx, zone->l_addr, sizeof(zone->l_addr), "0.0.0.0" );
				zone->ifno = mapi_ccfg_idx_get_long( tid, "DMZ", "ifno", idx, -1 );
				break;
			}
		}
		if(i > sizeRuleList)
			i = -1;
	}
	if(i == -1){
		strcpy(zone->g_addr, "0.0.0.0");
		strcpy(zone->l_addr, "0.0.0.0");
		zone->ifno = -1;
	}
}


//**************************************************************************************************
// FUNCTION:  mapi_ccfgsal_nat_add_DMZ
// PURPOSE:   add DMZ
// PARAMETERS:
// 			idx  : index of entry
//			zone : new settings
// RETURN:
//
// Note
//		* related sections: DMZ
//
//***************************************************************************************************
void mapi_ccfgsal_nat_add_DMZ(int tid, int idx, struct Zone_t zone){

	char RuleList[255];
	int sizeRuleList = 0, i=-1, tmpIdx;


	mapi_ccfg_get_str( tid, "DMZ", "RuleList", RuleList, sizeof(RuleList), "" );
	sizeRuleList = getListSize(RuleList, ',');
	if(sizeRuleList > 0){
		for(i=0; i<sizeRuleList; i++){
			tmpIdx = getItemInList(RuleList, ',',i,-1);
			if(tmpIdx == idx){
				break;
			}
		}
		if(i > sizeRuleList)
			i = -1;
	}
	if(i == -1){
		insertItemInList(RuleList, sizeof(RuleList), ',', -1, idx);
		mapi_ccfg_set_str( tid, "DMZ", "RuleList", RuleList );
	}

	mapi_ccfg_idx_set_str( tid, "DMZ", "g_addr", idx, zone.g_addr );
	mapi_ccfg_idx_set_str( tid, "DMZ", "l_addr", idx, zone.l_addr );
	mapi_ccfg_idx_set_long( tid, "DMZ", "ifno", idx, zone.ifno );
}


//**************************************************************************************************
// FUNCTION:  mapi_ccfgsal_nat_del_DMZ
// PURPOSE:   delete DMZ
// PARAMETERS:
// 			idx  : index of entry
// RETURN:
//
// Note
//		* related sections: DMZ
//
//***************************************************************************************************
int mapi_ccfgsal_nat_del_DMZ(int tid, int idx){

	char RuleList[255];
	int sizeRuleList = 0, i=-1, tmpIdx;
	int ret;

	ret = NAT_RET_FAIL;

	mapi_ccfg_get_str( tid, "DMZ", "RuleList", RuleList, sizeof(RuleList), "" );
	sizeRuleList = getListSize(RuleList, ',');
	if(sizeRuleList > 0){
		for(i=0; i<sizeRuleList; i++){
			tmpIdx = getItemInList(RuleList, ',',i,-1);
			if(tmpIdx == idx){
				delItemInList(RuleList, ',', i);
				mapi_ccfg_set_str( tid, "DMZ", "RuleList", RuleList);
				mapi_ccfg_idx_unset_all(tid, "DMZ", i);
				ret = NAT_RET_SUCCESS;
				break;
			}
		}
	}
	return ret;
}

int comparePort(const void *arg1, const void *arg2) {
	int ret = 0;
	if(((struct PortRange_t *)arg1)->b_port >  ((struct PortRange_t *)arg2)->b_port){
		ret = 1;
	}
	else if(((struct PortRange_t *)arg1)->b_port <  ((struct PortRange_t *)arg2)->b_port){
		ret = -1;
	}
	else {
		ret = 0;
	}
	return  ret;
}

int compareInt(const void *arg1, const void *arg2) {
	return  (*(int *)arg1 - *(int *)arg2);
}


long mapi_ccfgsal_nat_portConfig(int tid, struct PortRange_t *ports, int numPort){
	int retPorts = 0;
	int i=0, j=0, idx=0, sizeConfig=0;
	long tmpL = 0;
	const int szTmpS = 255;
	int sizeRuleList = 0, szPortInfo=0,tmpIdx=0;
	char RuleList[szTmpS], tmpS[szTmpS], tmpName[szTmpS], strBuf[30];
	//add port here to expend the config
	char *configlist[] = {
		"lte", "diag_port",
	};
	if((ports == NULL)|| (numPort <= 0))
		numPort = 0;

	sizeConfig = sizeof(configlist)/sizeof(char*)/2;

	for(idx=0; idx<sizeConfig; idx++){
		if(retPorts >= numPort)
			break;
		tmpL = mapi_ccfg_get_long(tid, configlist[idx*2], configlist[idx*2+1], 0);
		if(tmpL != 0){
			ports[retPorts].Protocol = 0;
			ports[retPorts].b_port = tmpL;
			ports[retPorts].e_port = tmpL;
			retPorts++;
		}
	}
	if(numPort == 0)
		retPorts = sizeConfig;

	if(retPorts <= numPort){
		ports[retPorts].b_port = mapi_ccfg_get_long(tid, "voip", "rtp_minPort", -1);
		ports[retPorts].e_port = mapi_ccfg_get_long(tid, "voip", "rtp_maxPort", -1);
	}
	retPorts++;

	//SMBA
	if(mapi_ccfg_get_long( tid, "samba", "enable", 0)
		&&mapi_ccfg_get_long( tid, "samba", "remoteEnable", 0)){
		if(retPorts <= numPort){
			//tcp
			ports[retPorts].Protocol = 6;
			ports[retPorts].b_port = 445;
			ports[retPorts].e_port = 445;
			retPorts++;
			//udp 135,137,138,139
			ports[retPorts].Protocol = 17;
			ports[retPorts].b_port = 135;
			ports[retPorts].e_port = 135;
			retPorts++;
			ports[retPorts].Protocol = 17;
			ports[retPorts].b_port = 137;
			ports[retPorts].e_port = 139;
			retPorts++;
		}else{
			retPorts += 3;
		}
	}
	//FTPD
	if(mapi_ccfg_get_long( tid, "ftpdef", "enable", 0) == 1){
		mapi_ccfg_get_str( tid, "ftpdef", "config", tmpS, sizeof(tmpS), "");
		if(getItemInList(tmpS, ':', 3, 0) > 0){
			if(retPorts <= numPort){
				//tcp
				ports[retPorts].Protocol = 6;
				ports[retPorts].b_port = getItemInList(tmpS, ':', 0, 0);
				ports[retPorts].e_port = getItemInList(tmpS, ':', 0, 0);
				retPorts++;
				mapi_ccfg_get_str( tid, "ftpdef", "passive_port_range", tmpS, sizeof(tmpS), "");
				ports[retPorts].Protocol = 6;
				ports[retPorts].b_port = getItemInList(tmpS, ':', 0, 0);
				ports[retPorts].e_port = getItemInList(tmpS, ':', 1, 0);
				retPorts++;
			}else{
				retPorts+=2;
			}
		}
	}
	//Remote management
	if(mapi_ccfg_get_long( tid, "httpd", "remote_mgnt", 0) > 0){
		tmpL = mapi_ccfg_get_long( tid, "system", "remote_mgnt_port", 0);
		if(tmpL == 0)
			tmpL = 443;
		if(retPorts <= numPort){
			ports[retPorts].Protocol = 6;
			ports[retPorts].b_port = tmpL;
			ports[retPorts].e_port = tmpL;
		}
		retPorts++;
	}
	//TR69
	tmpL = mapi_ccfg_get_long( tid, "tr69", "randomport", 0);
	if( tmpL > 0){
		if(retPorts <= numPort){
			ports[retPorts].Protocol = 6;
			ports[retPorts].b_port = tmpL;
			ports[retPorts].e_port = tmpL;
		}
		retPorts++;
	}



	//NAT port mapping
	//if(mapi_ccfg_get_long(tid, "NATPubServ", "func", 0) == 1)
	{
		mapi_ccfg_get_str( tid, "NATPubServ", "RuleList", RuleList, sizeof(RuleList), "");
		sizeRuleList = getListSize(RuleList, ',');
		#if (_DBG_CCFG_NAT_LV > 5)
		printf("[NATPubServ] RuleList[%d]:%s\n", sizeRuleList, RuleList);
		#endif //(_DBG_CCFG_NAT_LV > 5)
		for(i=0; i<sizeRuleList; i++){
			tmpIdx = getItemInList(RuleList, ',',i,-1);
			j=0;
			sprintf(tmpName, "Global_Port%d", j);
			mapi_ccfg_idx_get_str( tid, "NATPubServ", tmpName, tmpIdx, tmpS, sizeof(tmpS), "");
			while(strcmp(tmpS,"") != 0){ //go through each port setting
				#if (_DBG_CCFG_NAT_LV > 5)
				printf("[NATPubServ] Global_Port:%s=%s\n", tmpName, tmpS);
				#endif //(_DBG_CCFG_NAT_LV > 5)
				szPortInfo = getListSize(tmpS, ',');
				if(szPortInfo >= 3){
					if(getItemInList(tmpS, ',', 1, 0)!=0){ //b_port != 0
						if(retPorts <= retPorts){
							//sscanf(tmpS, "%s %d %d", strBuf, &(ports[retPorts].b_port), &(ports[retPorts].e_port));
							getStrInList(tmpS, ',', 0, strBuf, sizeof(strBuf));
							if(strcmp(strBuf,"TCP") == 0)
								ports[retPorts].Protocol = TCP_PROTO;
							else if(strcmp(strBuf,"UDP") == 0)
								ports[retPorts].Protocol = UDP_PROTO;
							else{
								ports[retPorts].Protocol = 0;
								printf("--------->[%s]%d unknow PROTCOL %s\n", __FUNCTION__, __LINE__,strBuf);
							}
//							ports[retPorts].Protocol = getItemInList(tmpS, ',', 0, 0);
							ports[retPorts].b_port = getItemInList(tmpS, ',', 1, 0);
							ports[retPorts].e_port = getItemInList(tmpS, ',', 2, 0);
						}
						#if (_DBG_CCFG_NAT_LV > 3)
						printf("[NATPubServ] Protocol:%d b_port:%d e_port:%d\n", ports[retPorts].Protocol, ports[retPorts].b_port, ports[retPorts].e_port);
						#endif //(_DBG_CCFG_NAT_LV > 3)
						retPorts++;
					}
				}else{
					printf("[Incorrect Info]%s@NATPubServ=%s\n", tmpName, tmpS);
				}
				sprintf(tmpName, "Global_Port%d", ++j);
				mapi_ccfg_idx_get_str( tid, "NATPubServ", tmpName, tmpIdx, tmpS, sizeof(tmpS), "");
			}
		}
	}


	return retPorts;
}

long mapi_ccfgsal_nat_findFreePort(int tid, int stPort, int edPort){
	int i=0, idxConf=0, idxSocket=0;
	int iFreePort = 0;
	const int numPorts = 500;
	int actNumPort=0,actConfNumPort=0;
	struct PortRange_t portsRange[numPorts];
	int iPorts[numPorts];
	int isHit=0;

	//get config ports
	actConfNumPort = mapi_ccfgsal_nat_portConfig(tid, portsRange, numPorts);
	qsort((void *)portsRange, actConfNumPort, sizeof(struct PortRange_t), comparePort);
	#if (_DBG_CCFG_NAT_LV > 5)
	printf("---->Config sorted Port number:%d\n", actConfNumPort);
	printf("Config sorted Port Range array\n");
	for(i=0; i<actConfNumPort; i++){
		printf("idx:%d proto:%d b_port:%d e_port:%d\n", i, portsRange[i].Protocol, portsRange[i].b_port, portsRange[i].e_port);
	}
	#endif //(_DBG_CCFG_NAT_LV > 5)

	//get wan soockets
	actNumPort = osWANSockPortGet ( 0, iPorts, numPorts ); //all type
	qsort((void *)iPorts, actNumPort, sizeof(int), compareInt);
	#if (_DBG_CCFG_NAT_LV > 5)
	printf("---->Config sorted Port number:%d\n", actNumPort);
	printf("Config sorted Port Range array\n");
	for(i=0; i<actNumPort; i++){
		printf("idx:%d port:%d\n", i, iPorts[i]);
	}
	#endif //(_DBG_CCFG_NAT_LV > 5)

	for(i=stPort; i<=edPort; i++){
		isHit = 0;
		//check config ports
		for(idxConf=0; idxConf<actConfNumPort; idxConf++){
			if(portsRange[idxConf].b_port > i)
				break;
			if(portsRange[idxConf].b_port == i){
				#if (_DBG_CCFG_NAT_LV > 3)
				printf("[find in Config]Hit!! idx:%d proto:%d b_port:%d e_port:%d\n", idxConf, portsRange[idxConf].Protocol, portsRange[idxConf].b_port, portsRange[idxConf].e_port);
				#endif //(_DBG_CCFG_NAT_LV > 3)
				isHit++;
				break;
			}else if(portsRange[idxConf].e_port > 0){
				if(portsRange[idxConf].e_port >= i){
					#if (_DBG_CCFG_NAT_LV > 3)
					printf("[find in Config]Hit!! idx:%d proto:%d b_port:%d e_port:%d\n", idxConf, portsRange[idxConf].Protocol, portsRange[idxConf].b_port, portsRange[idxConf].e_port);
					#endif //(_DBG_CCFG_NAT_LV > 3)
					isHit++;
					break;
				}
			}

		}
		if(isHit > 0)
			continue;
		//check wan soockets
		for(idxSocket=0; idxSocket<actNumPort; idxSocket++){
			if(iPorts[idxSocket] > i)
				break;
			if(iPorts[idxSocket] == i){
				#if (_DBG_CCFG_NAT_LV > 3)
				printf("[find in socket]Hit!! idx:%d port:%d\n", idxSocket, iPorts[idxSocket]);
				#endif //(_DBG_CCFG_NAT_LV > 3)
				isHit++;
				break;
			}
		}
		if(isHit == 0)
			break;
	}
	if(isHit == 0)
		iFreePort = i;
	else
		iFreePort = 0;

	return iFreePort;
}

long mapi_ccfgsal_nat_findRandFreePort(int tid, int stPort, int edPort){
	srand(time(0));
	stPort = stPort + rand()%(edPort-stPort+1);
	return mapi_ccfgsal_nat_findFreePort(tid, stPort, edPort);
}


//////////////////////////HTTP redirect function
void mapi_ccfgsal_nat_redirect(int action, char* hostIP, char* proto, int sport, int dport){
	//action 0: delete 1: insert
	char strAct[]="-D";
	char qStr[256], tmpBuf[512];
	char *lockFileName = "/var/lock/iptables.lock";
	int fd;

	if(action == 1)
		strAct[1] = 'I';
	if(utilGetlockByName(lockFileName, &fd, UTIL_FILE_LCK) == ARC_COM_OK){
		strcpy(qStr, "(iptables -t nat -N HTTPREDIR_NAT_RULES >/dev/null 2>&1)");
		osSystem_GetOutput(qStr, tmpBuf, sizeof(tmpBuf));
		if((hostIP != NULL)	&&(strlen(hostIP) != 0)){
			sprintf(qStr, "iptables -t nat %s HTTPREDIR_NAT_RULES -p %s ! -d %s --dport %d -j REDIRECT --to-port %d"
							, strAct
							, proto
							, hostIP
							, sport
							, dport
							);
		}else{
			sprintf(qStr, "iptables -t nat %s HTTPREDIR_NAT_RULES -p %s --dport %d -j REDIRECT --to-port %d"
							, strAct
							, proto
							, sport
							, dport
							);
		}
		osSystem_GetOutput(qStr, tmpBuf, sizeof(tmpBuf));
//			printf("[mapi_ccfgsal_nat_redirect] qStr:%s\n", qStr);
//			printf("[mapi_ccfgsal_nat_redirect] tmpBuf:%s\n", tmpBuf);
		utilGetlockByName(lockFileName, &fd, UTIL_FILE_UNLCK);

	}else{
		printf("LOCK fail:%s\n", __FUNCTION__);
	}

}

void mapi_ccfgsal_nat_redirectEnable(char* hostIP, int isSSLSupported){
		mapi_ccfgsal_nat_redirect(1, NULL, "udp", 53, 55330);
		mapi_ccfgsal_nat_redirect(1, hostIP, "tcp", 80, 39990);
		if(isSSLSupported == 1){
			mapi_ccfgsal_nat_redirect(1, hostIP, "tcp", 443, 34567);
		}

}

void mapi_ccfgsal_nat_redirectDisable(char* hostIP, int isSSLSupported){

//	char qStr[256], tmpBuf[512];
//	char *lockFileName = "/var/lock/iptables.lock";
//	int fd;
//	if(utilGetlockByName(lockFileName, &fd, UTIL_FILE_LCK) == ARC_COM_OK){
//		strcpy(qStr, "(iptables -t nat -F HTTPREDIR_NAT_RULES >/dev/null 2>&1)");
//		osSystem_GetOutput(qStr, tmpBuf, sizeof(tmpBuf));
//
//	}else{
//		printf("LOCK fail:%s\n", __FUNCTION__);
//	}
		mapi_ccfgsal_nat_redirect(0, NULL, "udp", 53, 55330);
		mapi_ccfgsal_nat_redirect(0, hostIP, "tcp", 80, 39990);
		if(isSSLSupported == 1){
			mapi_ccfgsal_nat_redirect(0, hostIP, "tcp", 443, 34567);
		}

}
void mapi_ccfgsal_nat_dnat(int action, char* proto, char* srcIP, int srcPort, char* dstIP, int dstPort){
	//action 0: delete 1: insert
	char strAct[]="-D";
	char qStr[256], tmpBuf[512];
	char *lockFileName = "/var/lock/iptables.lock";
	int fd;
	int szqStr = sizeof(qStr);

	if(action == 1)
		strAct[1] = 'I';
	if(utilGetlockByName(lockFileName, &fd, UTIL_FILE_LCK) == ARC_COM_OK){
		if(action == 0)
			strcpy(qStr, "(");
		else
			strcpy(qStr, "");
		sprintf(tmpBuf, "iptables -t nat %s PREROUTING", strAct);
		strncat(qStr, tmpBuf, szqStr);
		if((proto != NULL)&&(strlen(proto)>0)){
			sprintf(tmpBuf, " -p %s", proto);
			strncat(qStr, tmpBuf, szqStr);
		}
		if((srcIP != NULL)&&(strlen(srcIP)>0)){
			sprintf(tmpBuf, " -d %s", srcIP);
			strncat(qStr, tmpBuf, szqStr);
		}
		if(srcPort > 0){
			sprintf(tmpBuf, " --dport %d", srcPort);
			strncat(qStr, tmpBuf, szqStr);
		}
		strcat(qStr, " -j DNAT --to-destination");
		if((dstIP != NULL)&&(strlen(dstIP)>0)){
			sprintf(tmpBuf, " %s", dstIP);
			strncat(qStr, tmpBuf, szqStr);
		}
		if(dstPort > 0){
			sprintf(tmpBuf, ":%d", dstPort);
			strncat(qStr, tmpBuf, szqStr);
		}
		if(action == 0){
			strcat(qStr, " >/dev/null 2>&1)");

		}

		osSystem_GetOutput(qStr, tmpBuf, sizeof(tmpBuf));
//		printf("[mapi_ccfgsal_nat_dnat] qStr:%s\n", qStr);
//		printf("[mapi_ccfgsal_nat_dnat] tmpBuf:%s\n", tmpBuf);
		utilGetlockByName(lockFileName, &fd, UTIL_FILE_UNLCK);
	}else{
		printf("LOCK fail:%s\n", __FUNCTION__);
	}

}
//////////////////////////UPnP function
void mapi_ccfgsal_nat_UPnPClient(int action, char* proto, char* intf, int sport, char* dhostIP, int dport){
	//action 0: delete 1: insert
	char strAct[]="D";
	char qStr[64], tmpBuf[512];
	char *lockFileName = "/var/lock/iptables.lock";
	int fd;

	if(action == 1)
		strAct[0] = 'I';
	if(utilGetlockByName(lockFileName, &fd, UTIL_FILE_LCK) == ARC_COM_OK){

		sprintf(qStr, "iptables -t nat -%s PREROUTING -i %s -p %s --dport %d -j DNAT --to %s:%d"
						, strAct
						, intf
						, proto
						, sport
						, dhostIP
						, dport
						);
		//printf("insert a rule(1) for UPnP: %s\n", qStr);
		osSystem_GetOutput(qStr, tmpBuf, sizeof(tmpBuf));

		sprintf(qStr, "iptables -%s FORWARD -d %s -p %s --dport %d -j ACCEPT"
						, strAct
						, dhostIP
						, proto
						, dport
						);
		//printf("insert a rule(2) for UPnP: %s\n", qStr);
		osSystem_GetOutput(qStr, tmpBuf, sizeof(tmpBuf));
		utilGetlockByName(lockFileName, &fd, UTIL_FILE_UNLCK);
	}else{
		printf("LOCK fail:%s\n", __FUNCTION__);
	}
}
void mapi_ccfgsal_nat_AddUPnPClient(char* proto, char* intf, int sport, char* dhostIP, int dport){
	mapi_ccfgsal_nat_UPnPClient(1, proto, intf, sport, dhostIP, dport);
}
void mapi_ccfgsal_nat_DelUPnPClient(char* proto, char* intf, int sport, char* dhostIP, int dport){
	mapi_ccfgsal_nat_UPnPClient(0, proto, intf, sport, dhostIP, dport);
}