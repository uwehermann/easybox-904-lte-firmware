/*
 * Arcadyan middle layer APIs for Firewall operations
 *
 * Copyright 2010, Arcadyan Corporation
 * All Rights Reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libArcComApi.h"
#include "mid_mapi_ccfgsal_firewall.h"

#include <sys/param.h>
//#include "all_cgis.h"
//#include "cbac.h"

#define	dprintf	printf

struct service_s serviceTable[MAX_SERVICE_POOL_SIZE] = {
	{"Frag", 		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					CBAC_IP_PROTO, 	INSPECT_FRAG},		// 0. INSPECT_FRAG_N
	{"TCP", 		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_TCP},		// 1. INSPECT_TCP_N
	{"UDP", 		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_UDP_PROTO,	INSPECT_UDP},		// 2. INSPECT_UDP_N
	{"FTP", 		CBAC_FTP_SERV, CBAC_FTP_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_FTP},		// 3. INSPECT_FTP_N
	{"H323", 		CBAC_H323_SERV, CBAC_H323_SERV, CBAC_WHITE_BOARD_SERV, CBAC_WHITE_BOARD_SERV, 0, 0, 0, 0, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_H323},		// 4. INSPECT_H323_N
	{"SMTP", 		CBAC_SMTP_SERV, CBAC_SMTP_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_SMTP},		// 5. INSPECT_SMTP_N
	{"HTTP", 		CBAC_HTTP_SERV1, CBAC_HTTP_SERV1, CBAC_HTTP_SERV2, CBAC_HTTP_SERV2, CBAC_HTTP_SERV3, CBAC_HTTP_SERV4, CBAC_HTTP_SERV5, CBAC_HTTP_SERV5, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_HTTP},		// 6. INSPECT_HTTP_N
	{"L2TP", 		CBAC_L2TP_SERV, CBAC_L2TP_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_UDP_PROTO,	INSPECT_L2TP},		// 7. INSPECT_L2TP_N
#if ACCTON_UI_SPEC == 2
	{"POP3", 		CBAC_SMTP_SERV, CBAC_SMTP_SERV, CBAC_POP3_SERV, CBAC_POP3_SERV, 0, 0, 0, 0, 0, 0,
#else
	{"POP3", 		CBAC_POP3_SERV, CBAC_POP3_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
#endif // ACCTON_UI_SPEC == 2
					CBAC_TCP_PROTO,	INSPECT_POP3},		// 8. INSPECT_POP3_N
	{"News", 		CBAC_NEWS_SERV, CBAC_NEWS_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_NEWS},		// 9. INSPECT_NEWS_N
	{"Telnet", 		CBAC_TELNET_SERV, CBAC_TELNET_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_TELNET},	// 10.INSPECT_TELNET_N
	{"HTTPS", 		CBAC_HTTPS_SERV, CBAC_HTTPS_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_HTTPS},		// 11.INSPECT_HTTPS_N
	{"ICQ", 		CBAC_ICQ_SERV, CBAC_ICQ_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_UDP_PROTO,	INSPECT_ICQ},		// 12.INSPECT_ICQ_N
	{"AOL", 		CBAC_AOL_SERV1, CBAC_AOL_SERV1, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_AOL},		// 13.INSPECT_AOL_N
//	{"MSN_Msg", 	CBAC_MSN_MSG_SERV, CBAC_MSN_MSG_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
//					CBAC_TCP_PROTO,	INSPECT_MSN_MESS},	// 14.INSPECT_MSN_MESS_N
	{"SIP", 		CBAC_SIP_SERV, CBAC_SIP_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_UDP_PROTO,	INSPECT_SIP},		// 14.INSPECT_SIP_N
#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
	{"UserDef2", 	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, INSPECT_USER_DEF2},				// 15.INSPECT_USER_DEF_N
#else
	{"Yahoo_Msg", 	CBAC_YAHOO_MSG_SERV, CBAC_YAHOO_MSG_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_YAHOO_MESS},// 15.INSPECT_YAHOO_MESS_N
#endif // NEED_USER_DEF2_IN_POLICY
	{"MediaPlayer",	CBAC_MS_MEDIAPLAYER_SERV, CBAC_MS_MEDIAPLAYER_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_MS_MEDIA},	// 16.INSPECT_MS_MEDIA_N
	{"RealPlayer", 	CBAC_REALPLAYER_SERV, CBAC_REALPLAYER_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_REAL_PLAY},	// 17.INSPECT_REAL_PLAY_N
	{"Napster", 	CBAC_NAPSTER_SERV, CBAC_NAPSTER_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_NAPSTER},	// 18.INSPECT_NAPSTER_N
	{"PPTP", 		CBAC_PPTP_SERV, CBAC_PPTP_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_PPTP},		// 19.INSPECT_PPTP_N
	{"TFTP", 		CBAC_TFTP_SERV, CBAC_TFTP_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_UDP_PROTO,	INSPECT_TFTP},		// 20.INSPECT_TFTP_N
#if ACCTON_UI_SPEC == 2
	{"HTTPOnly",	CBAC_HTTP_SERV1, CBAC_HTTP_SERV1, 0, 0, 0, 0, 0, 0, 0, 0,
#else
	{"HttpOnly",	CBAC_HTTP_SERV1, CBAC_HTTP_SERV1, CBAC_HTTP_SERV2, CBAC_HTTP_SERV2, CBAC_HTTP_SERV3, CBAC_HTTP_SERV4, CBAC_HTTP_SERV5, CBAC_HTTP_SERV5, 0, 0,
#endif // ACCTON_UI_SPEC == 2
					CBAC_TCP_PROTO,	INSPECT_HTTP_ONLY},	// 21.INSPECT_HTTP_ONLY_N
	{"H323Data", 	CBAC_H323_SERV, CBAC_H323_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_H323_DAT},	// 22.INSPECT_H323_DAT_N
	{"DNS", 		CBAC_DNS_SERV, CBAC_DNS_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_UDP_PROTO, INSPECT_DNS},		// 23.INSPECT_DNS_N
	{"SNMP", 		CBAC_SNMP_SERV1, CBAC_SNMP_SERV2, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_UDP_PROTO, INSPECT_SNMP},		// 24.INSPECT_SNMP_N
	{"RIP", 		CBAC_RIP_SERV2, CBAC_RIP_SERV2, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_UDP_PROTO, INSPECT_RIP},		// 25.INSPECT_RIP_N
	{"XWin", 		CBAC_XWIN_SERV1, CBAC_XWIN_SERV2, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_TCP_PROTO, INSPECT_XWIN},		// 26.INSPECT_XWIN_N
	{"RPC_NT", 		CBAC_NT_RPC_SERV, CBAC_NT_RPC_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_UDP_PROTO, INSPECT_RPC_NT},	// 27.INSPECT_RPC_NT_N
	{"Ident",	 	CBAC_AUTH_SERV, CBAC_AUTH_SERV, 0, 0, 0, 0, 0, 0, 0, 0,
					CBAC_TCP_PROTO, INSPECT_AUTH},		// 28.INSPECT_AUTH_N
	{"NetBIOS", 	CBAC_NT_RPC_SERV, CBAC_NT_RPC_SERV, CBAC_NBIOS_NS, CBAC_NBIOS_SS, CBAC_NBIOS_DS, CBAC_NBIOS_DS, 0, 0, 0, 0,
					CBAC_TCP_PROTO, INSPECT_NBIOS},		// 29.INSPECT_NBIOS_N
#ifdef _SURFCONTROL
	{"CPA", 		CBAC_HTTP_SERV1, CBAC_HTTP_SERV1, CBAC_HTTP_SERV2, CBAC_HTTP_SERV2, CBAC_HTTP_SERV3, CBAC_HTTP_SERV4, CBAC_HTTP_SERV5, CBAC_HTTP_SERV5, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_CPA},		// 30.INSPECT_CPA_N
#elif defined(_PARECONTROL)
	{"PareCtrl", 	CBAC_HTTP_SERV1, CBAC_HTTP_SERV1, CBAC_HTTP_SERV2, CBAC_HTTP_SERV2, CBAC_HTTP_SERV3, CBAC_HTTP_SERV4, CBAC_HTTP_SERV5, CBAC_HTTP_SERV5, 0, 0,
					CBAC_TCP_PROTO,	INSPECT_PCTL},		// 30.INSPECT_PCTL_N
#else
	{"tmp3", 		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, INSPECT_tmp3},					// 30.
#endif // _SURFCONTROL
	{"UserDef", 	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					0, INSPECT_USER_DEF},				// 31.INSPECT_USER_DEF_N
	//{"", 			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//				0, 0},								// NULL end (must exist)
};	// service table with (name, port number) pair


///// extend functions -------------------Start
int Check_IP(char *str) {
	int i, len, dot, ip[4];
	char ch, tmp[16];

	if(!str)
	   return FALSE;

	//******************************************************************************//
	// PTR-TI-0026 2004.05.27
	len = strlen(str);
	if (len > 15) return FALSE;

	dot = 0;

	for (i = 0;i < len;i++) {
		ch = str[i];
		if (ch < '0' || ch > '9') {
			if (ch != '.') return FALSE;
			dot++;
		}
	}

	if (dot != 3) return FALSE;
	//******************************************************************************//

	if (sscanf(str,"%d.%d.%d.%d%s",&ip[0],&ip[1],&ip[2],&ip[3],tmp)!= 4)
	   return FALSE;

	for (i=0;i<=3;i++)
	{
	   if (ip[i]<0 || ip[i]>255)
	      return FALSE;
	}
	return TRUE;
}

//**************************************************************************************************
// FUNCTION:  getPolicyNum
// PURPOSE:   get policy number in specific group ID (based on POLICY_COUNTER_OFFSET2)
// PARAMETERS:
// 			short grpID 			: group array index (for NDC always set 0, -1 for search all rule pool)
//			unsigned short *plyID	: policy ID array (NULL for needn't)
//			unsigned short *priority: this policy priority (NULL for needn't)
// RETURN:
//			>=0: policy number
//***************************************************************************************************
short mapi_ccfgsal_firewall_get_PolicyNum(int tid, short grpID, unsigned short *plyID, unsigned short *priority) {
	short plyCount;
	int num_listItem, max_rowSize;
	//char* pgrpIdx;
	unsigned short pID[MAX_POLICY_NUM];
	unsigned short pri[MAX_POLICY_NUM];
	long i, j, ruleID, upper, policyID;
	//long upper;
	char bypass;
//	short MAX_CBAC_EXTEND_RULE_NUM, HALF_GRP_RULE_NUM;
	char lists[MID_CCFG_MAX_VALUE_LEN];
//	char tmpBuf[16];

	plyCount = 0;
	num_listItem = 0;
	max_rowSize = 30;
//	MAX_CBAC_EXTEND_RULE_NUM = mapi_ccfg_idx_get_long( tid, "Firewall", "MAX_CBAC_EXTEND_RULE_NUM", 0);
//	HALF_GRP_RULE_NUM = mapi_ccfg_idx_get_long( tid, "Firewall", "HALF_GRP_RULE_NUM", 0);

	memset(pID, 0, (sizeof(unsigned short) * MAX_POLICY_NUM));
	memset(pri, 0, (sizeof(unsigned short) * MAX_POLICY_NUM));

	if (grpID == -1)
		upper = MAX_CBAC_EXTEND_RULE_NUM;
	else{
		mapi_ccfg_idx_get_str( tid, "ruleGrp", "rules", grpID, lists, sizeof(lists), "" );
		num_listItem = getListSize(lists, ',');
		upper = num_listItem;
	}

	for (i = 0 ; i <  upper; i++) {
		//printf("[getPolicyNum] i=%d\n", i);
		// setting ruleID & check this rule is valid
		if (grpID == -1) {
			ruleID = i;
			//printf("mapi_ccfg_idx_get_str( tid, "Firewallruls", "grpIdx", ruleID) %s\n",mapi_ccfg_idx_get_str( tid, "Firewallruls", "grpIdx", ruleID, tmpBuf, sizeof(tmpBuf), "" ));
			if (mapi_ccfg_idx_cmp_str( tid, "Firewallruls", "grpIdx", ruleID, "" ) == 0) continue;
		}
		else {
			ruleID =  getItemInList(lists,',',i,0) - 1;
			if (ruleID == -1) break;
		}

		if (plyCount >= MAX_POLICY_NUM) break;
		policyID = mapi_ccfg_idx_get_long( tid, "Firewallruls", "policyId", ruleID, 0);
		if (policyID >= POLICY_COUNTER_OFFSET2 ) {
			// bypass duplicate policy id
			bypass = FALSE;
			for (j = 0 ; j < plyCount ; j++) {
				if (policyID == pID[j]) {
					bypass = TRUE;
					break;
				}
			}
			if (bypass) continue;
			// record policy id number to number array
			pID[plyCount] = policyID;
			pri[plyCount] = mapi_ccfg_idx_get_long( tid, "Firewallruls", "priority", ruleID, 0);
#if ATRACE >= 3
			dprintf("getPolicyNum()> get policy[%d]=%u, priority:%u\n", plyCount, pID[plyCount], pri[plyCount]);
#endif
			plyCount++;
		}
	}


	// copy policy ID
	if (plyID)
		memcpy(plyID, pID, (sizeof(unsigned short) * MAX_POLICY_NUM));
	if (priority)
		memcpy(priority, pri, (sizeof(unsigned short) * MAX_POLICY_NUM));
#if ATRACE >= 3
	dprintf("getPolicyNum()> get policy num=%d\n", plyCount);
#endif
	return plyCount;
}
//**************************************************************************************************
// FUNCTION:  getPolicyStatus
// PURPOSE:   get policy status by input policy name string
// PARAMETERS:
// 			short grpID 			: group array index (for NDC always set 0, -1 for search all ext rule pool)
//			unsigned short plyID	: policy ID
// RETURN:
//			char *name				: policy name string (have NULL end, len < CBAC_RULE_NAME_LEN)
//			unsigned char *ip		: ip address char array (ex. 140,92,61,1, range upper bound(0 for no use))
//			short schId				: entry index in schedule pool (1 based index, 0 for wildcard)
//			char log				: log function DISABLE/ENABLE (FALSE/TRUE)
//			unsigned long *service	: filtering service status (ref. cbac.h)
// 			short *uProtocol		: user define protocol number (NULL for useless)
//			unsigned short *uDPort	: user define destination port array (NULL for useless)
// 			short *uProtocol2		: user define protocol 2 number (NULL for useless)
//			unsigned short *uDPort2	: user define destination port 2 array (NULL for useless)
//		0: get status succeed,  -1: invalid name input
//***************************************************************************************************
#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
int mapi_ccfgsal_firewall_get_PolicyStatus(int tid, short grpID, unsigned short plyID, char *name, unsigned char *ip, short *schId, char *log, unsigned long *service, short *uProtocol, unsigned short *uDPort, short *uProtocol2, unsigned short *uDPort2) {
	return getPolicyStatusDetail(tid, grpID, plyID, name, ip, NULL, schId, log, service, NULL, uProtocol, uDPort, uProtocol2, uDPort2, NULL);
}
#else
int mapi_ccfgsal_firewall_get_PolicyStatus(int tid, short grpID, unsigned short plyID, char *name, unsigned char *ip, short *schId, char *log, unsigned long *service, short *uProtocol, unsigned short *uDPort) {
	return getPolicyStatusDetail(tid, grpID, plyID, name, ip, NULL, schId, log, service, NULL, uProtocol, uDPort, NULL);
}
#endif // NEED_USER_DEF2_IN_POLICY

//**************************************************************************************************
// FUNCTION:  getPolicyStatusDetail
// PURPOSE:   get policy status by input policy name string
// PARAMETERS:
// 			short grpID 			: group array index (for NDC always set 0, -1 for search all ext rule pool)
//			unsigned short plyID	: policy ID
// RETURN:
//			char *name				: policy name string (have NULL end, len < CBAC_RULE_NAME_LEN)
//			unsigned char *ip		: ip address char array (ex. 140,92,61,1, range upper bound(0 for no use))
//			unsigned char *mac		: mac address char array
//			short schId				: entry index in schedule pool (1 based index, 0 for wildcard)
//			char log				: log function DISABLE/ENABLE (FALSE/TRUE)
//			unsigned long *service	: filtering service status (NULL for useless, ref. cbac.h)
//			char *action			: allow or deny
// 			short *uProtocol		: user define protocol number (NULL for useless)
//			unsigned short *uDPort	: user define destination port array (NULL for useless)
// 			short *uProtocol2		: user define protocol 2 number (NULL for useless)
//			unsigned short *uDPort2	: user define destination port 2 array (NULL for useless)
//			unsigned *func			: rule function
//		0: get status succeed,  -1: invalid name input
//***************************************************************************************************
#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
int getPolicyStatusDetail(int tid, short grpID, unsigned short plyID, char *name, unsigned char *ip, unsigned char *mac, short *schId, char *log, unsigned long *service, char *action, short *uProtocol, unsigned short *uDPort, short *uProtocol2, unsigned short *uDPort2, char *func) {
	long i, j, ruleID;
	unsigned long servTmp;
	char hasUDef = FALSE;
#else
int getPolicyStatusDetail(int tid, short grpID, unsigned short plyID, char *name, unsigned char *ip, unsigned char *mac, short *schId, char *log, unsigned long *service, char *action, short *uProtocol, unsigned short *uDPort, char *func) {
	long i, j, ruleID;
	unsigned long servTmp;
#endif // NEED_USER_DEF2_IN_POLICY

	int grpIdx;
	char sRange[16];
//	short MAX_CBAC_EXTEND_RULE_NUM, HALF_GRP_RULE_NUM;
	int num_listItem=0;
	//int max_rowSize=30;
	char tmpBuf[32];

	// clear output buffer
	memset(name, 0, CBAC_RULE_NAME_LEN);
	name[0] = '\0';
	memset(ip, 0, 5);
	ip[0] = '\0';
	memset(sRange, 0, sizeof(sRange));
	*schId = 0;
	*log = 100;
	if (service) *service = 0L;
	servTmp = 0L;

	char lists[MID_CCFG_MAX_VALUE_LEN];
//	MAX_CBAC_EXTEND_RULE_NUM = mapi_ccfg_idx_get_long( tid, "Firewall", "MAX_CBAC_EXTEND_RULE_NUM", 0);
//	HALF_GRP_RULE_NUM = mapi_ccfg_idx_get_long( tid, "Firewall", "HALF_GRP_RULE_NUM", 0);

	// collect status
	for (i = 0 ; i < MAX_CBAC_EXTEND_RULE_NUM ; i++) {
		// setting ruleID & check this rule is valid
		ruleID = i;
		grpIdx = (int)mapi_ccfg_idx_get_long( tid, "Firewallruls", "grpIdx", ruleID, 0 );
		if (grpIdx == 0) continue;
		// specify group ID
		if ((grpID != -1) && ((grpID + 1) != grpIdx)) continue;
		if (plyID == mapi_ccfg_idx_get_long( tid, "Firewallruls", "policyId", ruleID, -1)) {
			if (name[0] == '\0')
				mapi_ccfg_idx_get_str( tid, "Firewallruls", "name", ruleID, name, CBAC_RULE_NAME_LEN, "");
#if ATRACE >= 2
			dprintf("getPolicyStatusDetail()> Get policy ID:%u, name:[%s]\n", plyID, name);
#endif
			if (ip[0] == '\0') {
				mapi_ccfg_idx_get_str( tid, "Firewallruls", "sRange", ruleID, sRange, sizeof(sRange), "");
				if (getItemInList(sRange,',',0,0) && getItemInList(sRange,',',1,0)) {
//					memcpy(ip, gSetting.ruleExt[ruleID].sSubnet, 3);
//					memcpy(&ip[3], gSetting.ruleExt[ruleID].sRange, 2);
					copyListToArray(ip, UMNGCFG_UINT8_TYPE, mapi_ccfg_idx_get_str( tid, "Firewallruls", "sSubnet", ruleID, tmpBuf, sizeof(tmpBuf), ""), ',', 3);
					copyListToArray(&ip[3], UMNGCFG_UINT8_TYPE, mapi_ccfg_idx_get_str( tid, "Firewallruls", "sRange", ruleID, tmpBuf, sizeof(tmpBuf), ""), ',', 2);
				}
				else copyListToArray(ip, UMNGCFG_UINT8_TYPE, mapi_ccfg_idx_get_str( tid, "Firewallruls", "srcIP", ruleID, tmpBuf, sizeof(tmpBuf), ""), ',', 4);
			}
#ifndef CBAC_FRAME_FILTER
//			if (mac) copyListToArray(mac, UMNGCFG_UINT8_TYPE, mapi_ccfg_idx_get_str( tid, "Firewallruls", "sMac", ruleID, tmpBuf, sizeof(tmpBuf), ""), ',', 6);
			mapi_ccfg_idx_get_str( tid, "Firewallruls", "sMac", ruleID, tmpBuf, sizeof(tmpBuf), "");
			if (mac) utilMacStr2Mac( tmpBuf, mac );
#endif // CBAC_FRAME_FILTER
			if (*schId == 0) *schId = (short)mapi_ccfg_idx_get_long( tid, "Firewallruls", "scheduleIdx", ruleID, 0);
			if (action) *action = (char)mapi_ccfg_idx_get_long( tid, "Firewallruls", "action", ruleID, 0);
			if (func) *func = (char)mapi_ccfg_idx_get_long( tid, "Firewallruls", "func", ruleID, 0);
			if (*log == 100) *log = (char)mapi_ccfg_idx_get_long( tid, "Firewallruls", "logFunc", ruleID, 0);
			mapi_ccfg_idx_get_str( tid, "Firewallruls", "dstPort", ruleID, lists, MID_CCFG_MAX_VALUE_LEN, "");
			num_listItem = getListSize(lists, ',');

			for (j = 0 ; j < MAX_SERVICE_POOL_SIZE ; j++) {
//				printf("--------->[%s]%d\n", __FUNCTION__, __LINE__);
//				printf("dstPort:%s\n", lists);
//				printf("serviceTable[%d].dstPort:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", j,
//				serviceTable[j].dstPort[0],
//				serviceTable[j].dstPort[1],
//				serviceTable[j].dstPort[2],
//				serviceTable[j].dstPort[3],
//				serviceTable[j].dstPort[4],
//				serviceTable[j].dstPort[5],
//				serviceTable[j].dstPort[6],
//				serviceTable[j].dstPort[7],
//				serviceTable[j].dstPort[8],
//				serviceTable[j].dstPort[9]
//				);

//				printf("isMatch:%d\n", isListMatch(lists, ',', serviceTable[j].dstPort, UMNGCFG_UINT16_TYPE, MAX_PORT_NUM));
//				printf("proto:%d\n", atoi(mapi_ccfg_idx_get_str( tid, "Firewallruls", "proto", ruleID, tmpBuf, sizeof(tmpBuf), "-1")));
//				printf("serviceTable[%d].proto:%d\n", j, serviceTable[j].proto);

				if (mapi_ccfg_idx_get_long( tid, "Firewallruls", "proto", ruleID, -1) == serviceTable[j].proto &&
					!isListMatch(lists, ',', (void *)serviceTable[j].dstPort, UMNGCFG_UINT16_TYPE, MAX_PORT_NUM) ) {
					//!memcmp(listsArray, serviceTable[j].dstPort, (sizeof(unsigned short) * MAX_PORT_NUM))) {

					if ((j == INSPECT_HTTP_N) || (j == INSPECT_HTTP_ONLY_N)) {
						if (mapi_ccfg_idx_get_long( tid, "Firewallruls", "code", ruleID, 0) != 0)
							servTmp = servTmp | serviceTable[INSPECT_HTTP_N].mask;
						else
							servTmp = servTmp | serviceTable[INSPECT_HTTP_ONLY_N].mask;
					}
					else servTmp = servTmp | serviceTable[j].mask;
#if ATRACE >= 2
					printf("getPolicyStatus()> policy ID:%u, add service: %08X\n", plyID, serviceTable[j].mask);
#endif
					break;
				}
			}
			// check user define service
			if (j == MAX_SERVICE_POOL_SIZE) {
#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
				if (hasUDef == TRUE) {
					if (uProtocol2) *uProtocol2 = (short)mapi_ccfg_idx_get_long( tid, "Firewallruls", "proto", ruleID, 0);
					//if (uDPort2) memcpy(uDPort2, listsArray, (sizeof(unsigned short) * MAX_PORT_NUM));
					if (uDPort2) copyListToArray(uDPort2, UMNGCFG_UINT16_TYPE, lists, ',', MAX_PORT_NUM));
					servTmp = servTmp | serviceTable[INSPECT_USER_DEF2_N].mask;
				}
				else {
#endif // NEED_USER_DEF2_IN_POLICY
					if (uProtocol) *uProtocol = (short)mapi_ccfg_idx_get_long( tid, "Firewallruls", "proto", ruleID, 0);
//					if (uDPort) memcpy(uDPort, listsArray, (sizeof(unsigned short) * MAX_PORT_NUM));
					if (uDPort) copyListToArray(uDPort, UMNGCFG_UINT16_TYPE, lists, ',', MAX_PORT_NUM);
					servTmp = servTmp | serviceTable[INSPECT_USER_DEF_N].mask;
#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
					hasUDef = TRUE;
				}
#endif // NEED_USER_DEF2_IN_POLICY
			}
			if (service) *service = servTmp;
		}
	}

	if (*log == 100) return -1;
	return 0;
}
#ifdef CBAC_RULE_SCHEDULE
//**************************************************************************************************
// FUNCTION:  delScheduleRule
// PURPOSE:   setting schedule rule
// PARAMETERS:
//			char *name	: rule name (NULL for no use)
//			short slot	: rule index (0 based,  -1 for no use)
// RETURN:
//			>=0: del rule success(slot index been del),  -1: invalid name/index input
//***************************************************************************************************
int delScheduleRule(int tid, char *name, short slot) {
	int i, tmpInt, num_listItem;
	char tmpStr[50];

	mapi_ccfg_get_str( tid, "sche", "rules", tmpStr, sizeof(tmpStr), "");
	if(strcmp(tmpStr, "") == 0)
		return -1; //rule array is empty
	num_listItem = getListSize(tmpStr, ',');

	// find old rule name entry
	if (name) {
		slot = -1;
		for (i = 0 ; i < num_listItem ; i++) {
//			if (!gSetting.sche.scheduleRule[i].name[0]) continue;
			tmpInt = getItemInList(tmpStr, ',', i, 0);
			if (mapi_ccfg_idx_cmp_str( tid, "sche", "name", tmpInt, name) == 0) {
				slot = tmpInt;
				break;
			}
		}
		if (slot == -1) return -1;
	}
	else if (slot != -1) {
//		if (!gSetting.sche.scheduleRule[slot].name[0]) return -1;
		for(i=0;i<num_listItem;i++){
			tmpInt = getItemInList(tmpStr, ',', i, 0);
			if(tmpInt == slot)
				break;
		}
		if(i >= num_listItem)
			return -1;
		if (mapi_ccfg_idx_cmp_str( tid, "sche", "name", slot, "") == 0) return -1;
	}
	else return -1;

	// del entry
//	memset(&gSetting.sche.scheduleRule[slot], 0, sizeof(struct scheduleRule_s));

	delItemInList(tmpStr, ',', i);
	mapi_ccfg_set_str( tid, "sche", "rules", tmpStr);
	mapi_ccfg_idx_unset_all( tid, "sche", slot);
	return slot;
}

//**************************************************************************************************
// FUNCTION:  setScheduleRule
// PURPOSE:   set/modify schedule rule
// PARAMETERS:
//			char func					: this schedule rule function disable/enable (FALSE/TRUE)
//			char *name					: rule name
//			char *nameOld				: old rule name (NULL for add new rule)
//			char *comm					: rule comment (NULL for no use)
//			struct scheduleTime_s *time	: schedule time structure pointer
// RETURN:
//			>=0: setting schedule rule index,  -1: pool full,  -2: invalid old name,  -3: null input struct
//
// Note
//		* related sections: sche
//
//***************************************************************************************************
int mapi_ccfgsal_firewall_set_ScheduleRule(int tid, char func, char *name, char *nameOld, char *comm, struct scheduleTime_s *time) {
	int i, slot = -1;
	char isNull = TRUE;
	char value[255];

	// check if need del old entry
	if (nameOld) {
		slot = delScheduleRule(tid, nameOld, -1);
		if (slot < 0) return -2;
	}

	// check null input struct
	for (i = 0 ; i < 8 ; i++) {
		if (time->stTime_h[i] && time->stTime_m[i] && time->edTime_h[i] && time->edTime_m)
			isNull = FALSE;
	}
	if (isNull) return -3;

	// find free slot
	if (slot == -1) {
		for (i = 0 ; i < MAX_SCHEDULE_RULE_NUM ; i++) {
//			BLOCK_PREEMPTION;
//			if (!gSetting.sche.scheduleRule[i].name[0]) {
			if (mapi_ccfg_idx_cmp_str( tid, "sche", "name", i, "") == 0) {
				slot = i;
//				gSetting.sche.scheduleRule[i].name[0] = (char)255; // mark config flag
				//mapi_ccfg_idx_set_long( tid, "sche", "name", i, -1);
//				UNBLOCK_PREEMPTION;
				break;
			}
//			UNBLOCK_PREEMPTION;
		}
	}
	if (slot == -1) return -1;
//dprintf("find free schedule slot: %d\n", slot);

	// setting variable
//	memset(&gSetting.sche.scheduleRule[slot], 0, sizeof(struct scheduleRule_s));
	mapi_ccfg_idx_unset_all( tid, "sche", slot);

//	gSetting.sche.scheduleRule[slot].func = func;
	mapi_ccfg_idx_set_long( tid, "sche", "func", slot, (long)func);
//	memcpy(gSetting.sche.scheduleRule[slot].name, name, (CBAC_RULE_NAME_LEN - 1));
	strncpy(value, name, (CBAC_RULE_NAME_LEN - 1));
	mapi_ccfg_idx_set_str( tid, "sche", "name", slot, name);
	if (comm) {
#ifdef NEED_SHCEDULE_GRP_RULE	// newFeature,  ## 1.04.38xx
//		memcpy(gSetting.sche.scheduleRule[slot].comment, comm, MAX_SHC_RULE_COMMENT);
//		gSetting.sche.scheduleRule[slot].comment[MAX_SHC_RULE_COMMENT - 9] = 0;

		strncpy(value, comm, MAX_SHC_RULE_COMMENT);
		value[MAX_SHC_RULE_COMMENT - 9] = 0;
		mapi_ccfg_idx_set_str( tid, "sche", "comment", slot, value);
#else
//		memcpy(gSetting.sche.scheduleRule[slot].comment, comm, (MAX_SHC_RULE_COMMENT - 1));
		strncpy(value, comm, MAX_SHC_RULE_COMMENT-1);
		mapi_ccfg_idx_set_str( tid, "sche", "comment", slot, value);
#endif // NEED_SHCEDULE_GRP_RULE
	}
//	memcpy(gSetting.sche.scheduleRule[slot].stTime_h, time->stTime_h, 8);
//	memcpy(gSetting.sche.scheduleRule[slot].stTime_m, time->stTime_m, 8);
//	memcpy(gSetting.sche.scheduleRule[slot].edTime_h, time->edTime_h, 8);
//	memcpy(gSetting.sche.scheduleRule[slot].edTime_m, time->edTime_m, 8);
	strcpy(value, "");
	copyArrayToList(value, ',', time->stTime_h, UMNGCFG_INT8_TYPE, 8, 0, 8, 10);
	mapi_ccfg_idx_set_str( tid, "sche", "stTime_h", slot, value);
	strcpy(value, "");
	copyArrayToList(value, ',', time->stTime_m, UMNGCFG_INT8_TYPE, 8, 0, 8, 10);
	mapi_ccfg_idx_set_str( tid, "sche", "stTime_m", slot, value);
	strcpy(value, "");
	copyArrayToList(value, ',', time->edTime_h, UMNGCFG_INT8_TYPE, 8, 0, 8, 10);
	mapi_ccfg_idx_set_str( tid, "sche", "edTime_h", slot, value);
	strcpy(value, "");
	copyArrayToList(value, ',', time->edTime_m, UMNGCFG_INT8_TYPE, 8, 0, 8, 10);
	mapi_ccfg_idx_set_str( tid, "sche", "edTime_m", slot, value);

	mapi_ccfg_get_str( tid, "sche", "rules", value, sizeof(value), "");
	insertItemInList(value, sizeof(value), ',', -1, slot);
	mapi_ccfg_set_str( tid, "sche", "rules", value);

	return slot;
}
//**************************************************************************************************
// FUNCTION:  getScheduleRule
// PURPOSE:   get schedule rule content
// PARAMETERS:
//			short schIdx				: schedule rule index (0 based)
// RETURN:
//			char *func					: this schedule rule function (NULL for no use)
//			char *name					: rule name (NULL for no use,  ref. config.h for string length)
//			char *comm					: rule comment (NULL for no use, ref. config.h for string length)
//			struct scheduleTime_s *time	: schedule time structure pointer (NULL for no use,  ref cbac.h)
//			1: success,  -1: invalid rule index
//***************************************************************************************************
int mapi_ccfgsal_firewall_get_ScheduleRule(int tid, short schIdx, char *func, char *name, char *comm, struct scheduleTime_s *time) {
	char *tmpValue;
	char tmpBuf[32];

	// check rule index invalid
	if (mapi_ccfg_idx_cmp_str( tid, "sche", "name", schIdx, "") == 0) return -1;


	// get rule content
	if (func) *func = (char)mapi_ccfg_idx_get_long( tid, "sche", "func", schIdx, 0);
	if (name) mapi_ccfg_idx_get_str( tid, "sche", "name", schIdx, name, CBAC_RULE_NAME_LEN, "");
	if (comm) mapi_ccfg_idx_get_str( tid, "sche", "comment", schIdx, comm, MAX_SHC_RULE_COMMENT, "");
	if (time) {
		tmpValue = mapi_ccfg_idx_get_str( tid, "sche", "stTime_h", schIdx, tmpBuf, sizeof(tmpBuf), "0,0,0,0,0,0,0,0");
		copyListToArray(time->stTime_h, UMNGCFG_INT8_TYPE, tmpValue,',',8);
		tmpValue = mapi_ccfg_idx_get_str( tid, "sche", "stTime_m", schIdx, tmpBuf, sizeof(tmpBuf), "0,0,0,0,0,0,0,0");
		copyListToArray(time->stTime_m, UMNGCFG_INT8_TYPE, tmpValue,',',8);
		tmpValue = mapi_ccfg_idx_get_str( tid, "sche", "edTime_h", schIdx, tmpBuf, sizeof(tmpBuf), "0,0,0,0,0,0,0,0");
		copyListToArray(time->edTime_h, UMNGCFG_INT8_TYPE, tmpValue,',',8);
		tmpValue = mapi_ccfg_idx_get_str( tid, "sche", "edTime_m", schIdx, tmpBuf, sizeof(tmpBuf), "0,0,0,0,0,0,0,0");
		copyListToArray(time->edTime_m, UMNGCFG_INT8_TYPE, tmpValue,',',8);
	}

	return 1;
}
#endif //CBAC_RULE_SCHEDULE
//**************************************************************************************************
// FUNCTION:  getCBACFunc
// PURPOSE:   get generic CBAC function status
// PARAMETERS:
//			signed char *inspect: input NULL for needn't
//			signed char *dos	: input NULL for needn't
//			signed char *ping	: input NULL for needn't
//			signed char *rip	: input NULL for needn't
// 			signed char *dmz	: input NULL for needn't
// RETURN:
//			signed char *inspect: inspect function on/off (TRUE/FALSE) (-1 for useless)
//			signed char *dos	: DoS function on/off (TRUE/FALSE) (-1 for useless)
//			signed char *ping	: deny ping from WAN on/off (TRUE/FALSE) (-1 for useless)
//			signed char *rip	: deny RIP v1 from WAN on/off (TRUE/FALSE) (-1 for useless)
//			signed char *dmz	: DMZ function on/off (TRUE/FALSE) (-1 for useless)
//			1: get success
//***************************************************************************************************
char getCBACFunc(int tid, signed char *inspect, signed char *dos, signed char *ping, signed char *rip, signed char *dmz) {
	if (inspect) *inspect = -1;
	if (ping) *ping = -1;
	if (dos) *dos = -1;
	if (rip) *rip = -1;
	if (dmz) *dmz = -1;

#if 1 //def CBAC_INSPECT_DOS
	if (ping) *ping = (char)mapi_ccfg_get_long( tid, "Firewall", "pingWAN", -1);
	if (dos)  *dos = (char)mapi_ccfg_get_long( tid, "Firewall", "dos_func", -1);
	if (rip)  *rip = (char)mapi_ccfg_get_long( tid, "Firewall", "ripDef", -1);
	if (dmz)  *dmz = (char)mapi_ccfg_get_long( tid, "DMZ", "func", -1);
#endif // CBAC_INSPECT_DOS
#if 1 //defined(CBAC_INSPECT_FRAGMENT) || defined(CBAC_INSPECT_TCP) || defined(CBAC_INSPECT_UDP)
	if (inspect) *inspect = (char)mapi_ccfg_get_long( tid, "inspectGlobal", "func", 0) & 0x1;
#endif // defined(CBAC_INSPECT_FRAGMENT) || defined(CBAC_INSPECT_TCP) || defined(CBAC_INSPECT_UDP)

	return 1;
}

//**************************************************************************************************
// FUNCTION:  getCBACFuncSpec
// PURPOSE:   get generic CBAC function status
// PARAMETERS:
//			signed char *SpiDos	: input NULL for needn't
//			signed char *ping	: input NULL for needn't
//			signed char *rip	: input NULL for needn't
//			signed char *dmz	: input NULL for needn't
// RETURN:
//			signed char *SpiDos	: DoS function on/off (TRUE/FALSE)
//			signed char *ping	: deny ping from WAN on/off (TRUE/FALSE)
//			signed char *rip	: deny RIP v1 from WAN on/off (TRUE/FALSE)
//			signed char *dmz	: DMZ function on/off (TRUE/FALSE)
//			1: get success
//***************************************************************************************************
char mapi_ccfgsal_firewall_get_CBACFuncSpec(int tid, signed char *SpiDos, signed char *ping, signed char *rip, signed char *dmz) {
	char rst;

//	rst = getCBACFunc(SpiDos, NULL, ping, rip, dmz);
	rst = getCBACFunc(tid, NULL, SpiDos, ping, rip, dmz);

	return rst;
}

//**************************************************************************************************
// FUNCTION:  getPolicyIDbyName
// PURPOSE:   get policy ID by policy name
// PARAMETERS:
// 			short grpID : group array index (-1 for search all ext rule pool)
//			char *name	: policy name string (must have NULL end)
// RETURN:
//			>0: policy ID,   0: invalid name
//***************************************************************************************************
unsigned short getPolicyIDbyName(int tid, short grpID, char *name) {
	long i, ruleIdx;
	unsigned short plyID = 0;
	int grpIdx = 0;

	for (i = 0 ; i < MAX_CBAC_EXTEND_RULE_NUM ; i++) {
		// setting ruleID & check this rule is valid
		ruleIdx = i;
		if (mapi_ccfg_idx_cmp_str( tid, "Firewallruls", "grpIdx", ruleIdx, "") == 0) continue;

		// get policy from specific group ID
		grpIdx = (int)mapi_ccfg_idx_get_long( tid, "Firewallruls", "grpIdx", ruleIdx, 0);
		if ((grpID != -1) && ((grpID + 1) != grpIdx)) continue;

		if (mapi_ccfg_idx_cmp_str( tid, "Firewallruls", "name", ruleIdx, name) == 0) {
			plyID = (unsigned short)mapi_ccfg_idx_get_long( tid, "Firewallruls", "policyId", ruleIdx, 1);
			break;
		}
	}

	return plyID;
}
//**************************************************************************************************
// FUNCTION:  getRuleByPolicy
// PURPOSE:   get rule index array by specify policy ID
// PARAMETERS:
// 			short grpID 			: group array index (0 based, for NDC always set 0, -1 for search all rule pool)
//			unsigned short plyIDLow	: specify which policy ID lower range want to search
//			unsigned short plyIDUp	: specify which policy ID upper range want to search
//			long *ruleIdx			: rule index array (0 based index, NULL for useless)
//			short *grpIdx			: group index array (0 based index, NULL for useless)
//			unsigned short arrSize	: index array size (0 for useless)
// RETURN:
//			>=0: rule number
//***************************************************************************************************
short getRuleByPolicy(int tid, short grpID, unsigned short plyIDLow, unsigned short plyIDUp, long *ruleIdx, short *grpIdx, unsigned short arrSize) {
	short ruleCount = 0, grpD[MAX_CBAC_EXTEND_RULE_NUM];
	long i, j, ruleIndex, ruleD[MAX_CBAC_EXTEND_RULE_NUM];
	long tmp;


	// find policy ID
	if (ruleIdx) memset(ruleIdx, 0, (sizeof(long) * arrSize));
	if (grpIdx) memset(grpIdx, 0, (sizeof(short) * arrSize));
	memset(ruleD, 0, (sizeof(long) * MAX_CBAC_EXTEND_RULE_NUM));
	memset(grpD, 0, (sizeof(short) * MAX_CBAC_EXTEND_RULE_NUM));

	for (j = plyIDLow ; j <= plyIDUp ; j++) {
		for (i = 0 ; i < MAX_CBAC_EXTEND_RULE_NUM ; i++) {
			// setting ruleID & check this rule is valid
			ruleIndex = i;
			if ((tmp=mapi_ccfg_idx_get_long( tid, "Firewallruls", "grpIdx", ruleIndex, 0)) == 0) continue;

			// specific group ID
			if ((grpID != -1) && ((grpID + 1) != (short)tmp)) continue;
			if (ruleCount >= arrSize) break;

			if (mapi_ccfg_idx_get_long( tid, "Firewallruls", "policyId", ruleIndex, 0) == j) {
				// record rule index number to rule index array
				grpD[ruleCount] = tmp - 1;
				ruleD[ruleCount++] = ruleIndex;
#if ATRACE >= 3
				dprintf("getRuleByPolicy()> get rule[%d]=%u\n", (ruleCount - 1), ruleD[ruleCount - 1]);
#endif
			}
		}
	}

	// copy policy ID
	if (ruleIdx) memcpy(ruleIdx, ruleD, (sizeof(long) * arrSize));
	if (grpIdx) memcpy(grpIdx, grpD, (sizeof(short) * arrSize));

	return ruleCount;
}

//**************************************************************************************************
// FUNCTION:  findAccessGrpListIndex
// PURPOSE:   search input rule number in which group list index( if succ, group lock is still locked)
// PARAMETERS:
//			int grpID		: group array index (0 based)
//			long ruleID		: rule array index (-1 for need search free entry, 0 based)
//			char needLock	: is need lock group lock & queue lock(TRUE or FALSE)
// RETURN:
//		>= 0 rule array index,  -1 error input rule index,  -2 empty pool,  -3 pool full,
//								-4 synchronize limited
//***************************************************************************************************
long findAccessGrpListIndex(int tid, int grpID, long ruleID, char needLock) {
	long i;
	int num_listItem, max_rowSize;
	char lists[MID_CCFG_MAX_VALUE_LEN];
	max_rowSize = 30;
	int tmpRuleID;

#if ATRACE >= 3
	dprintf("findAccessGrpListIndex()> input grpID=%d, ruleID=%u\n", grpID, ruleID);
#endif

	//if (needLock) if (!lockGroup(-1, -1)) return -4;
	mapi_ccfg_idx_get_str( tid, "ruleGrp", "rules", grpID, lists, sizeof(lists), "" );
	num_listItem = getListSize(lists, ',');
	// change run-time var to gSetting. because configuration is add into gSetting.  pert/ 2003/08/29
	for (i = 0 ; i < num_listItem ; i++) {
		//if (!CBACVar.ruleGrp[grpID].rules[i]) {
//		if (!gSetting.ruleGrp[grpID].rules[i]) {
//			if (ruleID == -1) return i;
//			break;
//		}
		tmpRuleID = getItemInList(lists, ',', i,0);
		//if ((CBACVar.ruleGrp[grpID].rules[i] - 1) == ruleID) {
		if ((tmpRuleID - 1) == ruleID) {
			return i;
		}
	}


	// if error unlock queue lock
	//if (needLock) unlockGroup(-1, -1);

	// check pool full or return free rule index
	if ((i==num_listItem)||(ruleID == -1))
		return i;
	else if(i == num_listItem)
		return -3;

	// check pool empty
	if (!i) return -2;

	return -1;
}

//**************************************************************************************************
// FUNCTION:  updateGroup
// PURPOSE:   update rule pool
// PARAMETERS:
// 			short grpID : -1 - update all group,   >=0 - rule group index to be update
//
// RETURN:
//			TRUE: update succeed,  FALSE: synchronize limited
//***************************************************************************************************
char updateGroup(int tid, short grpID) {
	long i, j, lo, upp, ruleID;
	short schIdx;

	int type;
	int num_listItem;
	char lists[MID_CCFG_MAX_VALUE_LEN];

	if (grpID == -1) {
		lo = 0;
		upp = MAX_CBAC_GROUP_NUM;
	}
	else {
		lo = grpID;
		upp = grpID + 1;
	}

	//if (!lockGroup(grpID, -1)) return FALSE;

	for (i = lo ; i < upp ; i++) {
		mapi_ccfg_idx_get_str( tid, "ruleGrp", "rules", i, lists, sizeof(lists), "");
		num_listItem = getListSize(lists, ',');

		// check empty group
//		if (gSetting.ruleGrp[i].rules[0] == 0) continue;
		if (lists[0] == '\0') continue;

		// copy group content
		//CBACVar.ruleGrp[i] = gSetting.ruleGrp[i];

		// for schedule rule, need to do someting
		type = (int)mapi_ccfg_idx_get_long( tid, "ruleGrp", "type", i, 0);
		if ((type != CBAC_STANDARD_TYPE) &&
			(type != CBAC_EXTEND_TYPE)) continue;

		for (j = 0 ; j < num_listItem ; j++) {
			ruleID = getItemInList(lists, ',', j,0) - 1;
			if (ruleID == -1) break;

			if (type == CBAC_STANDARD_TYPE) {
//			schIdx = gSetting.ruleBas[ruleID].scheduleIdx;
//			if (schIdx) gSetting.ruleBas[ruleID].isSche = FALSE;
			schIdx = (short)mapi_ccfg_idx_get_long( tid, "Firewallruls", "scheduleIdx", ruleID, 0);
			if (schIdx) mapi_ccfg_idx_set_long( tid, "Firewallruls", "isSche", ruleID, FALSE);
			}
			else {
//				schIdx = gSetting.ruleExt[ruleID].scheduleIdx;
//				if (schIdx) gSetting.ruleExt[ruleID].isSche = FALSE;
				schIdx = (short)mapi_ccfg_idx_get_long( tid, "Firewallruls", "scheduleIdx", ruleID, 0);
				if (schIdx) mapi_ccfg_idx_set_long( tid, "Firewallruls", "isSche", ruleID, FALSE);

			}
//dprintf("updateGroup() schIdx:%u\n", schIdx);

//			if (schIdx) {
//				if ((j + 1) < num_listItem) {
//					if (getItemInList(lists, ',', j+1,0) != -1) {
//						// find next not schedule rule
//						nextRule = 0;
//						for (k = (j + 1) ; k < num_listItem ; k++) {
//							tmpRule = atoi(listsArray[k]) - 1;
//							if (tmpRule == -1) break;
//
//							if (type == CBAC_STANDARD_TYPE)
//								tmpSchIdx = (short)mapi_ccfg_idx_get_long( tid, "Firewallruls", "scheduleIdx", tmpRule, 0);
//							else
//								tmpSchIdx = (short)mapi_ccfg_idx_get_long( tid, "Firewallruls", "scheduleIdx", tmpRule, 0);
//
//							if (tmpSchIdx == 0) nextRule = tmpRule + 1;
//						}
//
////						CBACVar.ruleGrp[i].rules[j] = nextRule;
//					}
////					else CBACVar.ruleGrp[i].rules[j] = 0;
//				}
////				else CBACVar.ruleGrp[i].rules[j] = 0;
//			}
		}
	}

	//unlockGroup(grpID, -1);

	return TRUE;
}


//**************************************************************************************************
// FUNCTION:  delAccessRule
// PURPOSE:   delete access control rule from specific rule list
// PARAMETERS:
//			short grpID		: access-list array index
//			long ruleID		: rule pool array index (-1 for no use)
//			long ruleIdx	: rule index in group rule list (-1 for havn't this value)
//			char isSchedule	: is this rule a schedule rule, (needn't check invalid rule)
// RETURN:
//			0: delete succeed,  -1: update group fail,  -2: update std rule pool fail,
//			-3: update ext rule pool fail,  -4: rule ID invalid
//***************************************************************************************************
int delAccessRule(int tid, short grpID, long ruleID, long ruleIdx, char isSchedule) {
	//long j;
	char grpType;
	int num_listItem;
	char lists[MID_CCFG_MAX_VALUE_LEN];
	//char rules[MID_CCFG_MAX_VALUE_LEN];
	//int idx_hdr,idx_end;
	mapi_ccfg_idx_get_str( tid, "ruleGrp", "rules", grpID, lists, sizeof(lists),"");
	num_listItem = getListSize(lists, ',');
	grpType = (char)mapi_ccfg_idx_get_long( tid, "ruleGrp", "type", grpID, 0);

	if (ruleIdx == -1) ruleIdx = findAccessGrpListIndex(tid, (int)grpID, ruleID, FALSE);
	else ruleID = (long)(getItemInList(lists, ',', ruleIdx,0) - 1);
	if (!isSchedule && (ruleIdx < 0)) return -4;

	//dprintf("delAccessRule()> grpID=%d, ruleID=%ld, ruleIdx=%ld\n", grpID, ruleID, ruleIdx);

	// 1. reset action pool counter
	//updateActionPool(grpType, ruleID, 0, grpID, TRUE, NULL);

	// 2. delete rule index from rule array of group entry(it's waste time, need to do first)
	//    to do this job only when this rule is run-time in group list
//	if (ruleIdx >= 0) {
//		for (j = ruleIdx ; j < num_listItem ; j++) {
//			if (!gSetting.ruleGrp[grpID].rules[j]) break;

//			if (j == (HALF_GRP_RULE_NUM * 2 - 1)) gSetting.ruleGrp[grpID].rules[j] = 0;
//			else gSetting.ruleGrp[grpID].rules[j] = gSetting.ruleGrp[grpID].rules[j + 1];
//		}
//	}
	if((ruleIdx>=0)&&(ruleIdx < num_listItem)){
		ruleID = (long)(getItemInList(lists, ',', ruleIdx,0) - 1);
		mapi_ccfg_idx_unset_all( tid, "Firewallruls", ruleID);
		delItemInList(lists, ',', ruleIdx);
		num_listItem = num_listItem -1;
		mapi_ccfg_idx_set_str( tid, "ruleGrp", "rules", grpID, lists);
	}



	// 3. clear group type to standard type (if this group list is empty)
	if (num_listItem==0) {
		// reset group content
//		gSetting.ruleGrp[grpID].type = CBAC_STANDARD_TYPE;
//		gSetting.ruleGrp[grpID].defaultAction = CBAC_DENY_ACT;
//		mapi_ccfg_idx_set_long( tid, "ruleGrp", "type", grpID, CBAC_STANDARD_TYPE);
//		mapi_ccfg_idx_set_long( tid, "ruleGrp", "defaultAction", grpID, CBAC_DENY_ACT);
		mapi_ccfg_idx_unset_all( tid, "ruleGrp", grpID);
		// detach group index from interface first
		// needn't detach group index because user sometime will forget to re-config this variable.  ## 1.04.3237,  ## 1.04.xx37
#if 0
		for (j = 1 ; j < N_INTERFACE ; j++) {
			if (gSetting.Interface[j].ruleGrpIdx[0] == (grpID + 1)) gSetting.Interface[j].ruleGrpIdx[0] = 0;
			if (gSetting.Interface[j].ruleGrpIdx[1] == (grpID + 1)) gSetting.Interface[j].ruleGrpIdx[1] = 0;

			if (!gSetting.Interface[j].ruleGrpIdx[0] && !gSetting.Interface[j].ruleGrpIdx[1])
				gSetting.Interface[j].active = 0;
		}
#endif // 0
	}

	// 4. apply new setting to active group pool
	if (!updateGroup(tid, grpID)) {
		return -1;
	}
	// 5. clear rule entry, action pool & apply new setting to active rule pool
	if (grpType == CBAC_STANDARD_TYPE) {
//		memset(&gSetting.ruleBas[ruleID], 0, sizeof(struct cRuleBasic_s));
		mapi_ccfg_idx_unset_all( tid, "Firewallruls", ruleID);
//		if (!updateStandardRule(ruleID)) {
//			return -2;
//		}
	}
	// bugFix,   ## 1.04.3540,  ## 1.04.xx40
	else if (grpType == CBAC_EXTEND_TYPE) {
#if ATRACE >= 2
		dprintf("delAccessRule()> Delete ruleID: %ld\n", ruleID);
#endif
//		memset(&gSetting.ruleExt[ruleID], 0, sizeof(struct cRuleExtend_s));
		mapi_ccfg_idx_unset_all( tid, "Firewallruls", ruleID);

//		if (!updateExtendRule(ruleID)) {
//			return -3;
//		}
	}

#ifdef CBAC_FRAME_FILTER
	else if (grpType == CBAC_FRAME_TYPE) {
//		memset(&gSetting.ruleFrame[ruleID], 0, sizeof(struct cRuleFrame_s));
		mapi_ccfg_idx_unset_all( tid, "ruleFrame", ruleID);
	}
#endif // CBAC_FRAME_FILTER

	return 0;
}

//**************************************************************************************************
// FUNCTION:  deletePolicyByName_ID
// PURPOSE:   delete policy from specific group (or delete schedule rule, grpID is no use)
// PARAMETERS:
// 			short grpID 	: group array index (-1 for search all ext pool)
//			char *name		: policy name string (must have NULL end, NULL for no use)
//			unsigned short	: policy ID (0 for no use)
// RETURN:
//			0: delete succeed,  -1: update group fail,  -2: update std rule pool fail,
//			-3: update ext rule pool fail,  -4: invalid policy name or policy ID
//***************************************************************************************************
int deletePolicyByName_ID(int tid, short grpID, char *name, unsigned short plyID) {
	int ruleCount, retVar;
	long i, ruleID[HALF_GRP_RULE_NUM * 2];
	short grpPool[HALF_GRP_RULE_NUM * 2];

	// find policy name
	if (name) {
		if ((plyID = getPolicyIDbyName(tid, grpID, name)) == 0) return -4;
	}
	else {
		if (plyID == 0) return -4;
	}

	// get rule index array
	ruleCount = getRuleByPolicy(tid, -1, plyID, plyID, ruleID, grpPool, HALF_GRP_RULE_NUM * 2);
//dprintf("queue.c 525, ruleCount:%d\n", ruleCount);
	if (!ruleCount) return -4;

	// delete rule
	for (i = 0 ; i < ruleCount ; i++) {
#if ATRACE >= 0
		dprintf("deletePolicyByName_ID()> Delete ruleIdx: %ld\n", ruleID[i]);
#endif

		retVar = delAccessRule(tid, grpPool[i], ruleID[i], -1, TRUE);

		if (retVar < 0) {
#if ATRACE >= 0
			dprintf("deletePolicyByName_ID()> Delete ruleID: %ld, Fail %d !!!\n", ruleID[i], retVar);
#endif
			return retVar;
		}
	}

	return 0;
}
//**************************************************************************************************
// FUNCTION:  deletePolicyById
// PURPOSE:   delete policy
// PARAMETERS:
// 			short grpID 		: group array index (for NDC/SMC like UI always set 0)
//			unsigned short plyID: policy ID (get plyID from 'getPolicyNumNDC()')
// RETURN:
//			0: delete succeed,  -1: update group fail,  -2: update std rule pool fail,
//			-3: update ext rule pool fail,  -4: invalid policy ID
//
// Note
//		* related sections: Firewallruls, ruleGrp, ruleFrame
//
//***************************************************************************************************
int mapi_ccfgsal_firewall_del_PolicyById(int tid, short grpID, unsigned short plyID) {
	return deletePolicyByName_ID(tid, grpID, NULL, plyID);
}
//**************************************************************************************************
// FUNCTION:  findFreeRuleSlot
// PURPOSE:   find free rule pool slot( if succ, group lock is still locked)
// PARAMETERS:
//			char type	: rule pool type (CBAC_STANDARD_TYPE: standard,
//										  CBAC_EXTEND_TYPE: extended)
//			char lock	: need mark group ID (TRUE/FALSE)
// RETURN:
//		>= 0 rule array index,   -1: pool full
//***************************************************************************************************
long findFreeRuleSlot(int tid, char type, char lock) {
	long i, up;
	int grpId;

	if (type == CBAC_STANDARD_TYPE) up = MAX_CBAC_STANDARD_RULE_NUM;
	else up = MAX_CBAC_EXTEND_RULE_NUM;

	for (i = 0 ; i < up ; i++) {
		if (type == CBAC_STANDARD_TYPE) grpId = (int)mapi_ccfg_idx_get_long( tid, "Firewallruls", "grpIdx", i, 0);
		else grpId = (int)mapi_ccfg_idx_get_long( tid, "Firewallruls", "grpIdx", i, 0);
#if ATRACE >= 5
		dprintf("findFreeRuleSlot()> i=%ld,  grpID=%d\n", i, grpId);
#endif
		if (grpId == 0) {
			if (lock) {
				// mark config flag
				//BLOCK_PREEMPTION;
				if (grpId == 0) {	// possibly be interrupted, check again
//					*grpId = -1;
					mapi_ccfg_idx_set_long( tid, "Firewallruls", "grpIdx", i, -1);
					//UNBLOCK_PREEMPTION;
					break;
				}
				else {
					//UNBLOCK_PREEMPTION;
					continue;
				}
			}
			else break;
		}
	}
	if (i == up) return (long)-1;

	return i;
}

//**************************************************************************************************
// FUNCTION:  insertExtRuleToGroup
// PURPOSE:   compare extended rule priority and insert new rule to rule list array
//            if priority number of new rule is the same with old one, last input slot will insert in
//            first slot in the same rules
// PARAMETERS:
// 			short grpID 			: group array index (0 based)
//			long ruleID				: new rule array index (0 based)
//			unsigned short priority	: new rule priority
// RETURN:
//			TRUE: insert succeed,  -1: rule list full
//***************************************************************************************************
int insertExtRuleToGroup(int tid, short grpID, long ruleID, unsigned short priority) {
	int i, pos = -1;
	long tmpRule, priBased;
	//long tempIdx, newIdx = 0;
	//char value[255], *tmp;
	//int tmpValue[1];


	int num_listItem;
	//int max_rowSize;
	char lists[MID_CCFG_MAX_VALUE_LEN];
	// check rule list full
	if (findAccessGrpListIndex(tid, grpID, -1, FALSE) < 0) return -1;
	mapi_ccfg_idx_get_str( tid, "ruleGrp", "rules", grpID, lists, sizeof(lists), "");
	num_listItem = getListSize(lists, ',');
	// ready to insert ruleID
	//BLOCK_PREEMPTION;

	//find first rule in rule Grop which priority is less than the new rule
//	for (i = 0 ; i < HALF_GRP_RULE_NUM * 2 ; i++) {
//		if (!newIdx) {
//			tmpRule = (long)gSetting.ruleGrp[grpID].rules[i] - 1L;
//
//			if ((priority <= gSetting.ruleExt[tmpRule].priority) || (tmpRule == -1)) {
//				newIdx = ruleID + 1L;
////dprintf("insertExtRuleToGroup() grpID:%d, ruleID:%ld, priority:%u, insert at:%d\n", grpID, ruleID, priority, i);
//				if (pos == -1) pos = i;
//			}
//		}
//
//		if (newIdx) {
//			tempIdx = (long)gSetting.ruleGrp[grpID].rules[i];
//			gSetting.ruleGrp[grpID].rules[i] = (unsigned short)newIdx;
//			newIdx = tempIdx;
//
//			if (!newIdx) break;
//		}
//	}
	if(num_listItem == 0){
		num_listItem = insertItemInList(lists, sizeof(lists), ',', 0, ruleID+1);
		mapi_ccfg_idx_set_str( tid, "ruleGrp", "rules", grpID, lists);
	}else{
		for (i = 0 ; i < num_listItem ; i++) {
			tmpRule = (long)getItemInList(lists, ',', i,0) - 1L;
			if ((long)priority <= mapi_ccfg_idx_get_long( tid, "Firewallruls", "priority", tmpRule, 0)) {
				num_listItem = insertItemInList(lists, sizeof(lists), ',', i, ruleID+1);
				mapi_ccfg_idx_set_str( tid, "ruleGrp", "rules", grpID, lists);
				if (pos == -1) pos = i;
				break;
			}
		}
		if(i == num_listItem){
			num_listItem = insertItemInList(lists, sizeof(lists), ',', i, ruleID+1);
			mapi_ccfg_idx_set_str( tid, "ruleGrp", "rules", grpID, lists);
			if (pos == -1) pos = i;
		}
	}

	// sort all rules priority to prevent some rules has same priority
	priBased = 0;
	for (i = 0 ; i < num_listItem ; i++) {
		tmpRule = (long)getItemInList(lists, ',', i,0) - 1L;
		if (tmpRule == -1) break;

		if (mapi_ccfg_idx_get_long( tid, "Firewallruls", "priority", tmpRule, 0) <= (long)priBased) {
			priBased++;
//			gSetting.ruleExt[tmpRule].priority = priBased;
			mapi_ccfg_idx_set_long( tid, "Firewallruls", "priority", tmpRule, (long)priBased);

			// maybe need modify run-time pool
//			CBACVar.ruleExt[tmpRule].priority = priBased;
		}
		else{
//			priBased = gSetting.ruleExt[tmpRule].priority;
			priBased = (unsigned short)mapi_ccfg_idx_get_long( tid, "Firewallruls", "priority", tmpRule, 0);
		}
	}
	//UNBLOCK_PREEMPTION;

	// check group list(add first rule), need set group type
	if (pos == 0){
//		gSetting.ruleGrp[grpID].type = CBAC_EXTEND_TYPE;
		mapi_ccfg_idx_set_long( tid, "ruleGrp", "type", grpID, CBAC_EXTEND_TYPE);
	}

	// maybe need to disconnect run-time TCP/UDP inspect session.  newFeature,  ## 1.04.53xx
#ifdef NEED_DISCON_INSPECT_SESSION
//	i = delSessionByIPPort(grpID, ruleID, FALSE);
//	if (i <= -1) dprintf("insertExtRuleToGroup() no any run-time session match rule %d, ret:%d\n", ruleID, i);
//	else dprintf("insertExtRuleToGroup() Delete UDP session %d for rule %d\n", i, ruleID);
#endif // NEED_DISCON_INSPECT_SESSION

	return TRUE;
}
void GetCurDateTime(struct tm *now_tm) {
	//NTP_Query(now_tm, 1);
	time_t t;
	struct tm *p;

	t = time((time_t *)0);
	p = localtime(&t);
	memcpy(now_tm, p, sizeof(struct tm));
}
//**************************************************************************************************
// FUNCTION:  matchTimePeriod
// PURPOSE:   compare input time is in specific time period
// PARAMETERS:
//			char inHr	: input hour number
//			char inMin	: input minute number
//			char inSec	: input second number
//			char upHr	: upper bound hour number
//			char upMin	: upper bound minute number
//			char upSec	: upper bound second number
//			char loHr	: lower bound hour number
//			char loMin	: lower bound minute number
//			char loSec	: lower bound second number
// RETURN:
//			TRUE: match time period,  FALSE: no match
//***************************************************************************************************
char matchTimePeriod(char inHr, char inMin, char inSec, char upHr, char upMin, char upSec, char loHr, char loMin, char loSec) {
	unsigned long tMin_in, tMin_up, tMin_lo;

	tMin_in = inHr * 3600 + inMin * 60 + inSec;
	tMin_up = upHr * 3600 + upMin * 60 + upSec;
	tMin_lo = loHr * 3600 + loMin * 60 + loSec;
#if ATRACE >= 2
	dprintf("matchTimePeriod()> in:[%02u:%02u:%02u](%lu), hi:[%02u:%02u:%02u](%lu), lo:[%02u:%02u:%02u](%lu)\n", inHr, inMin, inSec, tMin_in, upHr, upMin, upSec, tMin_up, loHr, loMin, loSec, tMin_lo);
#endif

	// to complete one hour
	if ((tMin_in >= tMin_lo) && (tMin_in < tMin_up)) return TRUE;

	// need support over one day,  ## newFeature, 1.04.37xx
	if (tMin_lo > tMin_up) {
		if ((tMin_in >= tMin_lo) || (tMin_in < tMin_up)) return TRUE;
	}

	return FALSE;
}

//**************************************************************************************************
// FUNCTION:  checkSchedule
// PURPOSE:   dynamic add/prune rule id from group rule list (only support: day,hr,min)
// RETURN:
//			1: set rule succeed,  0: do nothing,  -1: group rule list full,  -2: update group fail
//***************************************************************************************************
// re-writing this API to support new feature,  ## 1.04.38xx
int checkSchedule(int tid) {
	int i, retVar = 0, wk, needCheck;
	char hrSt[28], minSt[28], hrEd[28], minEd[28], scheFunc, retW, retD;
	// char ruleFunc;
	short schID, grpID;
	//long ruleIdx;
	int grpIdx,scheduleIdx;
#ifdef NEED_SHCEDULE_GRP_RULE
	int j;
	char wkNum[28];
	int size_wkNum=0;
#endif // NEED_SHCEDULE_GRP_RULE
//#ifdef CBAC_HAS_TIME_TASK
	struct tm cur;

	GetCurDateTime(&cur);
//#else
//	struct timeBase_s cur;
//
//	memset(&cur, 0, sizeof(struct timeBase_s));
//	timeTranslator(GetTime(), &cur, NULL);
//#endif // CBAC_HAS_TIME_TASK

	wk = cur.tm_wday + 1;

	// ready for checking time schedule
	// check basic rule
	for (i = 0 ; i < MAX_CBAC_STANDARD_RULE_NUM ; i++) {
		grpIdx = (short)mapi_ccfg_idx_get_long( tid, "Firewallruls", "grpIdx", i, 0);
		scheduleIdx = (short)mapi_ccfg_idx_get_long( tid, "Firewallruls", "scheduleIdx", i, 0);
		// bypass useless, config, no schedule rules
		if (grpIdx <= 0) continue;
		if (scheduleIdx == 0) continue;
		schID = scheduleIdx - 1;
		grpID = grpIdx - 1;
		//ruleFunc = gSetting.ruleBas[i].func;
		mapi_ccfg_idx_get_str( tid, "sche", "stTime_h", schID, hrSt, sizeof(hrSt), "0");
		mapi_ccfg_idx_get_str( tid, "sche", "stTime_m", schID, minSt, sizeof(minSt), "0");
		mapi_ccfg_idx_get_str( tid, "sche", "edTime_h", schID, hrEd, sizeof(hrEd), "0");
		mapi_ccfg_idx_get_str( tid, "sche", "edTime_m", schID, minEd, sizeof(minEd), "0");
		scheFunc = (char)mapi_ccfg_idx_get_long( tid, "sche", "func", schID, 0);
#ifdef NEED_SHCEDULE_GRP_RULE
		{
			mapi_ccfg_idx_get_str( tid, "sche", "wkNum", schID, wkNum, sizeof(wkNum), "");
			 //wknum in comment
		}
#endif // NEED_SHCEDULE_GRP_RULE

		// check prune rule first, to get more empty group list slot
		if (mapi_ccfg_idx_get_long( tid, "Firewallruls", "isSche", i, 0) != 0) {

#ifdef NEED_SHCEDULE_GRP_RULE
			for (j = 0 ; j < 8 ; j++)
#endif //def NEED_SHCEDULE_GRP_RULE
			{
				needCheck = 0;
#ifdef NEED_SHCEDULE_GRP_RULE
				if (getItemInList(wkNum, ',', j,0) == 0) continue;
				if ((getItemInList(wkNum, ',', j,0) != 1) && (wk != (getItemInList(wkNum, ',', j,0) - 1))) continue;
				retW = matchTimePeriod(cur.tm_hour
										,cur.tm_min
										,cur.tm_sec
										,(getItemInList(hrEd, ',', j,0) - 1)
										,(getItemInList(minEd, ',', j,0) - 1)
										,0
										,(getItemInList(hrSt, ',', j,0) - 1)
										,(getItemInList(minSt, ',', j,0) - 1)
										,0);

				if (/*!ruleFunc || */((scheFunc != CBAC_SCHE_FUNC_OFF) && (retW == 0)))
					needCheck = 1;
#else
				retW = matchTimePeriod(cur.tm_hour
										,cur.tm_min
										,cur.tm_sec
										,(getItemInList(hrEd, ',', wk,0) - 1)
										,(getItemInList(minEd, ',', wk,0) - 1)
										,0
										,(getItemInList(hrSt, ',', wk,0) - 1)
										,(getItemInList(minSt, ',', wk,0) - 1)
										,0);
				retD = matchTimePeriod(cur.tm_hour
										,cur.tm_min
										,cur.tm_sec
										,(getItemInList(hrEd, ',', 0,0) - 1)
										,(getItemInList(minEd, ',', 0,0) - 1)
										,0
										,(getItemInList(hrSt, ',', 0,0) - 1)
										,(getItemInList(minSt, ',', 0,0) - 1)
										,0);

				// check weekday[1]~[7] time period & everyday[0] time period

				//if (ruleFunc == 0) needCheck = 1;
				//else
				{
					switch (scheFunc) {
						case CBAC_SCHE_FUNC_ON: // 1
							if ((retW == 0) && (retD == 0)) needCheck = 1;
							break;
						case CBAC_SCHE_FUNC_EVERY: // 2
							if (retD == 0) needCheck = 1;
							break;
						case CBAC_SCHE_FUNC_DAY: // 3
							if (retW == 0) needCheck = 1;
							break;
					}
				}
#endif // NEED_SHCEDULE_GRP_RULE
				if (needCheck != 0)

				{

#if ATRACE >= 2
					dprintf("checkSchedule()> will prune bas rule: %ld,  group: %d\n", i, grpID);
#endif

//#if 1
//					pruneRuleFromGroup(grpID, i);
//#else
//					if (pruneRuleFromGroup(grpID, i) == -2) {
//						retVar = -2;
//						continue;
//					}
//#endif // 1

#ifdef NEED_NORMAL_LOG
					// after prune filter rule, need update log pool
//					updateLogPool(-1, -2, i, NULL, CBAC_PF_DEL, TRUE, NULL);
//					if(gSetting.mailFunc)
//						SendMailAlert(mail);
#endif // NEED_NORMAL_LOG

					// set flag
//					gSetting.ruleBas[i].isSche = FALSE;
					mapi_ccfg_idx_set_long( tid, "Firewallruls", "isSche", i, 0);
					if (!retVar) retVar = 1;
				}

			}
		}
		// check add rule
		else {
#ifdef NEED_SHCEDULE_GRP_RULE
			for (j = 0 ; j < 8 ; j++)
#endif //def NEED_SHCEDULE_GRP_RULE
			{
				needCheck = 0;
#ifdef NEED_SHCEDULE_GRP_RULE
				if (getItemInList(wkNum, ',', j,0) == 0) continue;
				if ((getItemInList(wkNum, ',', j,0) != 1) && (wk != (getItemInList(wkNum, ',', j,0) - 1))) continue;
				retW = matchTimePeriod(cur.tm_hour
										,cur.tm_min
										,cur.tm_sec
										,(getItemInList(hrEd, ',', j,0) - 1)
										,(getItemInList(minEd, ',', j,0) - 1)
										,0
										,(getItemInList(hrSt, ',', j,0) - 1)
										,(getItemInList(minSt, ',', j,0) - 1)
										,0);

				if (/*ruleFunc && */((scheFunc == CBAC_SCHE_FUNC_OFF) || ((scheFunc != CBAC_SCHE_FUNC_OFF) && retW)))
					needCheck = 1;
#else
				retW = matchTimePeriod(cur.tm_hour
										,cur.tm_min
										,cur.tm_sec
										,(getItemInList(hrEd, ',', wk,0) - 1)
										,(getItemInList(minEd, ',', wk,0) - 1)
										,0
										,(getItemInList(hrSt, ',', wk,0) - 1)
										,(getItemInList(minSt, ',', wk,0) - 1)
										,0);
				retD = matchTimePeriod(cur.tm_hour
										,cur.tm_min
										,cur.tm_sec
										,(getItemInList(hrEd, ',', 0,0) - 1)
										,(getItemInList(minEd, ',', 0,0) - 1)
										,0
										,(getItemInList(hrSt, ',', 0,0) - 1)
										,(getItemInList(minSt, ',', 0,0) - 1)
										,0);

				// check weekday[1]~[7] time period & everyday[0] time period

				//if (ruleFunc != 0)
				{
					switch (scheFunc) {
						case CBAC_SCHE_FUNC_OFF: // 0
							needCheck = 1;
							break;
						case CBAC_SCHE_FUNC_ON: // 1
							if ((retW != 0) || (retD != 0)) needCheck = 1;
							break;
						case CBAC_SCHE_FUNC_EVERY: // 2
							if (retD != 0) needCheck = 1;
							break;
						case CBAC_SCHE_FUNC_DAY: // 3
							if (retW != 0) needCheck = 1;
							break;
					}
				}

#endif // NEED_SHCEDULE_GRP_RULE
				if (needCheck != 0)

				{
#if ATRACE >= 2
					dprintf("checkSchedule()> will insert bas rule: %ld,  group: %d\n", i, grpID);
#endif

					// insert rule ID to group & update group run-time var
//#if 1
//					plugRuleIntoGroup(grpID, i);
//#else
//					if ((ruleIdx = findAccessGrpListIndex(tid, grpID, -1, TRUE)) < 0) {
//						retVar = -1;
//						continue;
//					}
//					gSetting.ruleGrp[grpID].rules[ruleIdx] = i;
//					// after called 'findAccessGrpListIndex()', need unlock group
//					unlockGroup(-1, -1);
//
//					if (!updateGroup(tid, grpID)) {
//						retVar = -2;
//						continue;
//					}
//#endif // 1

#ifdef NEED_NORMAL_LOG
//					// after insert filter rule, need update log pool
//					updateLogPool(-1, -2, i, NULL, CBAC_PF_ADD, TRUE, NULL);
#endif // NEED_NORMAL_LOG

					// set flag
//					gSetting.ruleBas[i].isSche = TRUE;
					mapi_ccfg_idx_set_long( tid, "Firewallruls", "isSche", i, 1);
					if (!retVar) retVar = 1;
				}

			}

		}
	}



	return retVar;
}

//**************************************************************************************************
// FUNCTION:  addPolicyByApDetail
// PURPOSE:   add new policy to extended access-list with detail rule content
// PARAMETERS:
// 			short grpID 			: group array index (for SMC like 0: WAN-out, 1: WAN-in)
//			unsigned short priority	: new rule priority (0 for equal to policy ID)
//			char action				: new rule action (for NDC always set 'CBAC_DENY_ACT')
//			unsigned char type		: ref. config.h (for NDC always set 0)
//			unsigned char code		: ref. config.h (for NDC always set 0)
//			char *name				: policy name string (must have NULL end)
//			unsigned char *smac		: source MAC address (NULL for useless, only useful when not define CBAC_FRAME_FILTER)
//			unsigned char *ipS		: src ip address char array (ex. 140,92,61,1, range upper bound(all 0 content for wildcard))
//			unsigned char *ipD		: dst ip address char array (ex. 140,92,61,1, range upper bound(all 0 content for wildcard))
//			unsigned char *maskS	: src wildcard mask, 4 char array (NULL for useless)
//			unsigned char *maskD	: dst wildcard mask, 4 char array (NULL for useless)
//			short schId				: entry index in schedule pool (0 for no use, 1 based index)
//			char log				: log function DISABLE/ENABLE (FALSE/TRUE)
//			char func				: rule function DISABLE/ENABLE (FALSE/TRUE)
//			unsigned long service	: filtering service status (ref. cbac.h)
//			char needActive			: need on-line activate (no/yes <==> FALSE/TRUE)
// 			short uProtocol			: user define protocol number (-1 for useless)
//			unsigned short *uDPort	: user define destination port array (NULL for useless)
// 			short uProtocol2		: user define protocol number (-1 for useless)
//			unsigned short *uDPort2	: user define destination port array (NULL for useless)
//			char needDupCheck		: need duplicate check (TRUE/FALSE)
// RETURN:
//			TRUE: insert succeed,  0: insert fail (pool full),  -1: on-line activate fail,  -2: name duplicate
//***************************************************************************************************
// re-write this function to support mac parameter.  newFeature,  ## 1.04.55xx
#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
int addPolicyByApDetail(int tid, short grpID, unsigned short priority, char action, unsigned char type, unsigned char code, char *name, unsigned char *smac, unsigned char *ipS, unsigned char *ipD, unsigned char *maskS, unsigned char *maskD, short schId, char log, char func, unsigned long service, char needActive, short uProtocol, unsigned short *uDPort, short uProtocol2, unsigned short *uDPort2, char needDupCheck) {
#else
int addPolicyByApDetail(int tid, short grpID, unsigned short priority, char action, unsigned char type, unsigned char code, char *name, unsigned char *smac, unsigned char *ipS, unsigned char *ipD, unsigned char *maskS, unsigned char *maskD, short schId, char log, char func, unsigned long service, char needActive, short uProtocol, unsigned short *uDPort, char needDupCheck) {
#endif // NEED_USER_DEF2_IN_POLICY
	unsigned long plyID;
	long i;
	int ruleCount = 0, sch, succd=0, retVar = TRUE, servId[MAX_SERVICE_POOL_SIZE];
	long ruleID[MAX_SERVICE_POOL_SIZE];
	char dup = FALSE;
	//char *tmp;
	//int tmpValue[1];
	//int idx_end;
#if defined(_SURFCONTROL) || defined(_PARECONTROL)
	char needURL;
#endif // defined(_SURFCONTROL) || defined(_PARECONTROL)
	int policyCounter2 = (int)mapi_ccfg_get_long( tid, "Firewall", "policyCounter2", 500);
	char	tmpBuf[128];
	char	nameBuf[CBAC_RULE_NAME_LEN];
	memset(ruleID, 0, (MAX_SERVICE_POOL_SIZE * sizeof(long)));
#if ATRACE >= 3
	if ((uProtocol >= 0) && uDPort) dprintf("addPolicyByAp()> uProtocol:%d, uDPort[0]:%u, uDPort[1]:%u\n", uProtocol, uDPort[0], uDPort[1]);
#endif

	// check duplicate name policy
	for (i = 0  ; i < MAX_CBAC_EXTEND_RULE_NUM ; i++) {
		mapi_ccfg_idx_get_str( tid, "Firewallruls", "name", i, tmpBuf, sizeof(tmpBuf), "_NULL_");
		if(strcmp(tmpBuf, "_NULL_") == 0){
			continue;
		}
		else if (strcmp(tmpBuf, name) == 0) {
			if (needDupCheck) return -2;
			else {
				dup = TRUE;
				break;
			}
		}
	}

	// get unique policy ID & priority
	if (dup == FALSE) {
#if ACCTON_UI_SPEC == 2
		policyCounter2 += 6;
		if (plyID >= POLICY_COUNTER_UP) plyID = POLICY_COUNTER_OFFSET2;
		plyID = policyCounter2;
#else
		plyID = ++policyCounter2;
		if (plyID == POLICY_COUNTER_UP) plyID = POLICY_COUNTER_OFFSET2;
#endif // ACCTON_UI_SPEC == 2
	}
	else plyID = policyCounter2;
	mapi_ccfg_set_long( tid, "Firewall", "policyCounter2", (long)policyCounter2);
	if (!priority) priority = plyID;

	// find empty access-list rule entry for every service
	for (i = 0 ; i < MAX_SERVICE_POOL_SIZE ; i++) {
		// bypass useless service & non-select service
		if ((i <= INSPECT_N_LOW) || (i > INSPECT_N_UP)) continue;
		if (((service << (MAX_SERVICE_POOL_SIZE - 1 - i)) & 0x80000000L) == 0) continue;
		// find empty gSetting extended rule slot
		if ((ruleID[ruleCount] = findFreeRuleSlot(tid, CBAC_EXTEND_TYPE, TRUE)) == -1) {
			retVar = 0;
			goto END_CODE;
		}
		servId[ruleCount] = i;
#if ATRACE >= 2
		dprintf("addPolicyByAp()> input service[%d]: %d\n", ruleCount, servId[ruleCount]);
#endif
		ruleCount++;
	}
	succd = ruleCount;
	// assign value to gSetting extended rule struct
#if defined(_SURFCONTROL) || defined(_PARECONTROL)
	needURL = FALSE;
#endif // defined(_SURFCONTROL) || defined(_PARECONTROL)
	memset( nameBuf, 0, sizeof(nameBuf) );
	strncpy( nameBuf, name, MIN(strlen(name),CBAC_RULE_NAME_LEN-1) );
	for (i = 0 ; i < ruleCount ; i++) {
//		memset(&gSetting.ruleExt[ruleID[i]], 0, sizeof(struct cRuleExtend_s));
		mapi_ccfg_idx_unset_all( tid, "Firewallruls", ruleID[i]);
		mapi_ccfg_idx_set_str( tid, "Firewallruls", "name", ruleID[i], nameBuf);
//		gSetting.ruleExt[ruleID[i]].func = func;
//		gSetting.ruleExt[ruleID[i]].scheduleIdx = schId;
//		gSetting.ruleExt[ruleID[i]].policyId = plyID;
//		gSetting.ruleExt[ruleID[i]].priority = priority;
//		gSetting.ruleExt[ruleID[i]].action = action;
//		gSetting.ruleExt[ruleID[i]].type = type;
//		gSetting.ruleExt[ruleID[i]].logFunc = log;
		mapi_ccfg_idx_set_long( tid, "Firewallruls", "func", ruleID[i], (long)func);
		mapi_ccfg_idx_set_long( tid, "Firewallruls", "scheduleIdx", ruleID[i], (long)schId);
		mapi_ccfg_idx_set_long( tid, "Firewallruls", "policyId", ruleID[i], (long)plyID);
		mapi_ccfg_idx_set_long( tid, "Firewallruls", "priority", ruleID[i], (long)priority);
		mapi_ccfg_idx_set_long( tid, "Firewallruls", "action", ruleID[i], (long)action);
		mapi_ccfg_idx_set_long( tid, "Firewallruls", "type", ruleID[i], (long)type);
		mapi_ccfg_idx_set_long( tid, "Firewallruls", "logFunc", ruleID[i], (long)log);

		// set source MAC address.  newFeature,  ## 1.04.55xx
#ifndef CBAC_FRAME_FILTER
		if (smac != NULL) {
//			memcpy(gSetting.ruleExt[ruleID[i]].sMac, smac, 6);
			copyArrayToList(tmpBuf, ',', smac, UMNGCFG_UINT8_TYPE, 6, 0, 6, 16);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "sMac", ruleID[i], tmpBuf);

//dprintf("addPolicyByApDetail(%d) sMAC:[%02x:%02x:%02x:%02x:%02x:%02x]\n", ruleID[i], smac[0], smac[1], smac[2], smac[3], smac[4], smac[5]);
		}
#endif // CBAC_FRAME_FILTER
		// set src ip
		if (ipS[4]) { // will setting ip range

//			memset(&gSetting.ruleExt[ruleID[i]].srcIP[4], 255, 4);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "srcIP", ruleID[i], "255,255,255,255");

			// set range
//			memcpy(gSetting.ruleExt[ruleID[i]].sSubnet, ipS, 3);
//			memcpy(gSetting.ruleExt[ruleID[i]].sRange, &ipS[3], 2);
			tmpBuf[0] = '\0';
			copyArrayToList(tmpBuf, ',', ipS, UMNGCFG_UINT8_TYPE, 5, 0, 3, 10);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "sSubnet", ruleID[i], tmpBuf);
			tmpBuf[0] = '\0';
			copyArrayToList(tmpBuf, ',', &ipS[3], UMNGCFG_UINT8_TYPE, 2, 0, 2, 10);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "sRange", ruleID[i], tmpBuf);
		}
		else {
			if ((ipS[0] + ipS[1] + ipS[2] + ipS[3]) == 0){
//				memset(&gSetting.ruleExt[ruleID[i]].srcIP[4], 255, 4);
				mapi_ccfg_idx_set_str( tid, "Firewallruls", "srcIP", ruleID[i], "255,255,255,255");
			}
			else{
//				memcpy(gSetting.ruleExt[ruleID[i]].srcIP, ipS, 4);
				tmpBuf[0] = '\0';
				copyArrayToList(tmpBuf, ',', ipS, UMNGCFG_UINT8_TYPE, 5, 0, 4, 10);
				//printf("--------->[%s]%d srcIP:%s ipS:%d.%d.%d.%d\n", __FUNCTION__, __LINE__,value,ipS[0],ipS[1],ipS[2],ipS[3]);
				mapi_ccfg_idx_set_str( tid, "Firewallruls", "srcIP", ruleID[i], tmpBuf);
			}

			if (maskS){
//				memcpy(&gSetting.ruleExt[ruleID[i]].srcIP[4], maskS, 4);
				mapi_ccfg_idx_get_str( tid, "Firewallruls", "srcIP", ruleID[i], tmpBuf, sizeof(tmpBuf), "");
				copyArrayToList(tmpBuf, ',', maskS, UMNGCFG_UINT8_TYPE, 4, 4, 4, 10);
				//printf("--------->[%s]%d srcIP:%s\n", __FUNCTION__, __LINE__,value);
				mapi_ccfg_idx_set_str( tid, "Firewallruls", "srcIP", ruleID[i], tmpBuf);
			}
		}
		// set dst ip
		if (ipD[4]) { // will setting ip range
//			memset(&gSetting.ruleExt[ruleID[i]].dstIP[4], 255, 4);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "dstIP", ruleID[i], "255,255,255,255");

			// set range
//			memcpy(gSetting.ruleExt[ruleID[i]].dSubnet, ipD, 3);
//			memcpy(gSetting.ruleExt[ruleID[i]].dRange, &ipD[3], 2);
			tmpBuf[0] = '\0';
			copyArrayToList(tmpBuf, ',', ipD, UMNGCFG_UINT8_TYPE, 5, 0, 3, 10);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "dSubnet", ruleID[i], tmpBuf);
			tmpBuf[0] = '\0';
			copyArrayToList(tmpBuf, ',', &ipD[3], UMNGCFG_UINT8_TYPE, 2, 0, 2, 10);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "dRange", ruleID[i], tmpBuf);
		}
		else {
			if ((ipD[0] + ipD[1] + ipD[2] + ipD[3]) == 0){
//				memset(&gSetting.ruleExt[ruleID[i]].dstIP[4], 255, 4);
				mapi_ccfg_idx_set_str( tid, "Firewallruls", "dstIP", ruleID[i], "255,255,255,255");
			}
			else{
//				memcpy(gSetting.ruleExt[ruleID[i]].dstIP, ipD, 4);
				tmpBuf[0] = '\0';
				copyArrayToList(tmpBuf, ',', ipD, UMNGCFG_UINT8_TYPE, 4, 0, 4, 10);
				mapi_ccfg_idx_set_str( tid, "Firewallruls", "dstIP", ruleID[i], tmpBuf);
			}
			if (maskD){
//				memcpy(&gSetting.ruleExt[ruleID[i]].dstIP[4], maskD, 4);
				mapi_ccfg_idx_get_str( tid, "Firewallruls", "dstIP", ruleID[i], tmpBuf, sizeof(tmpBuf), "");
				copyArrayToList(tmpBuf, ',', maskD, UMNGCFG_UINT8_TYPE, 4, 4, 4, 10);
				mapi_ccfg_idx_set_str( tid, "Firewallruls", "dstIP", ruleID[i], tmpBuf);
			}
		}
		// set protocol & port range
//if (uDPort) dprintf("queue.c 524, servId[%d]:%d, proto:%d, port0:%u\n", i, servId[i], uProtocol, uDPort[0]);
		if ((servId[i] == INSPECT_USER_DEF_N) && (uProtocol >= 0) && uDPort) {
//dprintf("addPolicyByAp(1)> i:%d, ruleID:%ld, servId[i]:%d, protocol:%u\n", i, ruleID[i], servId[i], uProtocol);
//			gSetting.ruleExt[ruleID[i]].proto = uProtocol;
//			memcpy(gSetting.ruleExt[ruleID[i]].dstPort, uDPort, (sizeof(unsigned short) * MAX_PORT_NUM));
			mapi_ccfg_idx_set_long( tid, "Firewallruls", "proto", ruleID[i], (long)uProtocol);
			tmpBuf[0] = '\0';
			copyArrayToList(tmpBuf, ',', uDPort, UMNGCFG_UINT16_TYPE, MAX_PORT_NUM, 0, MAX_PORT_NUM, 10);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "dstPort", ruleID[i], tmpBuf);
		}
#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
		// set protocol & port range 2
//if (uDPort2) dprintf("queue.c 532, servId[%d]:%d, proto2:%d, port2-0:%u\n", i, servId[i], uProtocol2, uDPort2[0]);
		else if ((servId[i] == INSPECT_USER_DEF2_N) && (uProtocol2 >= 0) && uDPort2) {
//dprintf("addPolicyByAp(1.5)> i:%d, ruleID:%ld, servId[i]:%d, protocol2:%u\n", i, ruleID[i], servId[i], uProtocol2);
//			gSetting.ruleExt[ruleID[i]].proto = uProtocol2;
//			memcpy(gSetting.ruleExt[ruleID[i]].dstPort, uDPort2, (sizeof(unsigned short) * MAX_PORT_NUM));
			mapi_ccfg_idx_set_long( tid, "Firewallruls", "proto", ruleID[i], (long)uProtocol2);
			tmpBuf[0] = '\0';
			copyArrayToList(tmpBuf, ',', uDPort2, , UMNGCFG_UINT16_TYPE, MAX_PORT_NUM, 0, MAX_PORT_NUM, 10);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "dstPort", ruleID[i], tmpBuf);
		}
#endif // NEED_USER_DEF2_IN_POLICY
		else {
			if ((servId[i] == INSPECT_USER_DEF_N)
#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
				|| (servId[i] == INSPECT_USER_DEF2_N)
#endif // NEED_USER_DEF2_IN_POLICY
				) {
				printf("addPolicyByAp()> Error input user define service !!!\n");
			}
//dprintf("addPolicyByAp(2)> i:%d, ruleID:%ld, servId[i]:%d, protocol:%u\n", i, ruleID[i], servId[i], serviceTable[servId[i]].proto);
//			gSetting.ruleExt[ruleID[i]].proto = serviceTable[servId[i]].proto;
//			memcpy(gSetting.ruleExt[ruleID[i]].dstPort, serviceTable[servId[i]].dstPort, (sizeof(unsigned short) * MAX_PORT_NUM));
			mapi_ccfg_idx_set_long( tid, "Firewallruls", "proto", ruleID[i], (long)serviceTable[servId[i]].proto);
			tmpBuf[0] = '\0';
			copyArrayToList(tmpBuf, ',', serviceTable[servId[i]].dstPort, UMNGCFG_UINT16_TYPE, MAX_PORT_NUM, 0, MAX_PORT_NUM, 10);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "dstPort", ruleID[i], tmpBuf);
		}
		// for Http filtering need add URL blocking flag
		if (servId[i] == INSPECT_HTTP_N) {
//			gSetting.ruleExt[ruleID[i]].code = 0x01;
			mapi_ccfg_idx_set_long( tid, "Firewallruls", "code", ruleID[i], 1);
#if defined(_SURFCONTROL) || defined(_PARECONTROL)
			needURL = TRUE;
#endif // defined(_SURFCONTROL) || defined(_PARECONTROL)
		}
#ifdef _SURFCONTROL
		else if (servId[i] == INSPECT_CPA_N) {
			if (needURL){
//				gSetting.ruleExt[ruleID[i]].code = 0x03;
				mapi_ccfg_idx_set_long( tid, "Firewallruls", "code", ruleID[i], 3);
			}
			else{
//				gSetting.ruleExt[ruleID[i]].code = 0x02;
				mapi_ccfg_idx_set_long( tid, "Firewallruls", "code", ruleID[i], 2);
			}
		}
#elif defined(_PARECONTROL)
		else if (servId[i] == INSPECT_PCTL_N) {
			if (needURL){
//				gSetting.ruleExt[ruleID[i]].code = 0x03;
				mapi_ccfg_idx_set_long( tid, "Firewallruls", "code", ruleID[i], 3);
			}
			else{
//				gSetting.ruleExt[ruleID[i]].code = 0x02;
				mapi_ccfg_idx_set_long( tid, "Firewallruls", "code", ruleID[i], 2);
			}
		}
#endif // _SURFCONTROL
		else{
//			gSetting.ruleExt[ruleID[i]].code = code;
			mapi_ccfg_idx_set_long( tid, "Firewallruls", "code", ruleID[i], code);
		}

		// until last value set OK, unlock group lock
//		gSetting.ruleExt[ruleID[i]].grpIdx = grpID + 1;
		mapi_ccfg_idx_set_long( tid, "Firewallruls", "grpIdx", ruleID[i], grpID + 1);

		// update run-time var (if some rule activate fail, following rule will not activate)
//		if (needActive && (retVar != -1)) {
//			if (!updateExtendRule(ruleID[i])) {
//				retVar = -1;
//				if (i < succd) succd = i;
//			}
//		}
	}
	// schedule is setting 'always blocking'
	if (needActive && (retVar != -1)) {
		for (i = 0 ; i < ruleCount ; i++) {
			// insert new rule to group list, dependence on rule priority, and setting new rule priority
			insertExtRuleToGroup(tid, grpID, ruleID[i], priority);

			// update run-time var (if some rule activate fail, following rule will not activate)
			if (needActive && (retVar != -1)) {
				if (!updateGroup(tid, grpID)) {
					retVar = -1;
					if (i < succd) succd = i;
				}
			}
		}
	}
	// trigger schedule checking
	sch = checkSchedule(tid);
	if (sch == -1) retVar = 0;
	if (sch == -2) retVar = -1;

END_CODE:
	// erase half-insert rule
	for (i = succd ; i < ruleCount ; i++){
//		gSetting.ruleExt[ruleID[i]].grpIdx = 0;
		mapi_ccfg_idx_set_long( tid, "Firewallruls", "grpIdx", ruleID[i], 0);
	}

	return retVar;
}
//**************************************************************************************************
// FUNCTION:  addPolicyByAp
// PURPOSE:   add new policy to extended access-list
// PARAMETERS:
// 			short grpID 			: group array index (for NDC always set 0)
//			unsigned short priority	: new rule priority (for NDC always set 0)
//			char action				: new rule action (for NDC always set 'CBAC_DENY_ACT')
//			unsigned char type		: ref. config.h (for NDC always set 0)
//			unsigned char code		: ref config.h (for NDC always set 0)
//			char *name				: policy name string (must have NULL end)
//			unsigned char *ipS		: src ip address char array (ex. 140,92,61,1, range upper bound(0 for no use))
//			unsigned char *ipD		: dst ip address char array (ex. 140,92,61,1, range upper bound(0 for no use))
//			short schId				: entry index in schedule pool (0 for no use, 1 based index)
//			char log				: log function DISABLE/ENABLE (FALSE/TRUE)
//			unsigned long service	: filtering service status (ref. cbac.h)
//			char needActive			: need on-line activate (no/yes <==> FALSE/TRUE)
// 			short uProtocol			: user define protocol number (-1 for useless)
//			unsigned short *uDPort	: user define destination port array (NULL for useless)
// 			short uProtocol2		: user define protocol number (-1 for useless)
//			unsigned short *uDPort2	: user define destination port array (NULL for useless)
//			char needDupCheck		: need duplicate check (TRUE/FALSE)
// RETURN:
//			TRUE: insert succeed,  0: insert fail (pool full),  -1: on-line activate fail,  -2: name duplicate
//***************************************************************************************************
#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
int addPolicyByAp(int tid, short grpID, unsigned short priority, char action, unsigned char type, unsigned char code, char *name, unsigned char *ipS, unsigned char *ipD, short schId, char log, unsigned long service, char needActive, short uProtocol, unsigned short *uDPort, short uProtocol2, unsigned short *uDPort2, char needDupCheck) {
	return addPolicyByApDetail(tid, grpID, priority, action, type, code, name, NULL, ipS, ipD, NULL, NULL, schId, log, TRUE, service, needActive, uProtocol, uDPort, uProtocol2, uDPort2, needDupCheck);	// newFeature,  ## 1.04.55xx
}
#else
int addPolicyByAp(int tid, short grpID, unsigned short priority, char action, unsigned char type, unsigned char code, char *name, unsigned char *ipS, unsigned char *ipD, short schId, char log, unsigned long service, char needActive, short uProtocol, unsigned short *uDPort, char needDupCheck) {
	return addPolicyByApDetail(tid, grpID, priority, action, type, code, name, NULL, ipS, ipD, NULL, NULL, schId, log, TRUE, service, needActive, uProtocol, uDPort, needDupCheck);	// newFeature,  ## 1.04.55xx
}
#endif // NEED_USER_DEF2_IN_POLICY
//**************************************************************************************************
// FUNCTION:  setPolicyByAp
// PURPOSE:   add/modify policy to extended access-list
// PARAMETERS:
// 			short grpID 			: group array index (for NDC/SMC like UI always set 0)
//			char *name				: policy name string (must have NULL end, len < CBAC_RULE_NAME_LEN ref. config.h)
//			char *nameOld			: old policy name (NULL for only add new policy, len < CBAC_RULE_NAME_LEN ref. config.h)
//			unsigned short idOld	: old policy ID (0 for no use)
//			unsigned char *ip		: source ip address char array (ex. 140,92,61,1, range upper bound(0 for no use))
//			short schId				: entry index in schedule pool (0 for no use, 1 based index)
//			char log				: log function DISABLE/ENABLE (FALSE/TRUE)
//			unsigned long service	: filtering service status (ref. cbac.h)
// 			short uProtocol			: user define protocol number (-1 for useless)
//			unsigned short *uDPort	: user define destination port array (NULL for useless)
// 			short uProtocol2		: user define protocol number (-1 for useless)
//			unsigned short *uDPort2	: user define destination port array (NULL for useless)
// RETURN:
//			TRUE: set succeed,
//			-1: update group fail,
//			-3: update ext rule pool fail,  -4: invalid policy name or policy ID,
//			-5: insert fail (pool full),  -6: on-line activate fail,  -7: name duplicate
//
// Note
//		* related sections: Firewallruls, ruleGrp, ruleFrame
//
//***************************************************************************************************
#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
int mapi_ccfgsal_firewall_set_PolicyByAp(int tid, short grpID, char *name, char *nameOld, unsigned short idOld, unsigned char *ip, short schId, char log, unsigned long service, short uProtocol, unsigned short *uDPort, short uProtocol2, unsigned short *uDPort2) {
#else
int mapi_ccfgsal_firewall_set_PolicyByAp(int tid, short grpID, char *name, char *nameOld, unsigned short idOld, unsigned char *ip, short schId, char log, unsigned long service, short uProtocol, unsigned short *uDPort) {
#endif // NEED_USER_DEF2_IN_POLICY
	int retVar;
	unsigned char ipD[5] = {0, 0, 0, 0, 0};

	if (idOld) {
		retVar = deletePolicyByName_ID(tid, grpID, NULL, idOld);
		dprintf("deletePolicyByName_ID() 1 retVar=%d\n", retVar);

		if (retVar < 0) return retVar;
	}
	else if (nameOld) {
		retVar = deletePolicyByName_ID(tid, grpID, nameOld, 0);
		dprintf("deletePolicyByName_ID() 2 retVar=%d\n", retVar);

		if (retVar < 0) return retVar;
	}
//	printf("--------->[%s]%d\n", __FUNCTION__, __LINE__);
//	printf("policy detial\n");
//	printf("name:%s\n",name);
//	printf("ip:%d.%d.%d.%d.%d.%d.%d.%d\n",ip[0],ip[1],ip[2],ip[3],ip[4],ip[5],ip[6],ip[7]);
//	printf("ipD:%d.%d.%d.%d.%d.%d.%d.%d\n",ipD[0],ipD[1],ipD[2],ipD[3],ipD[4],ipD[5],ipD[6],ipD[7]);
//	printf("schId:%d\n",schId);
//	printf("log:%d\n",log);
//	printf("service:0x%x\n",service);
//	printf("uProtocol:%d\n",uProtocol);
//	printf("uDPort:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",uDPort[0],uDPort[1],uDPort[2],uDPort[3],uDPort[4],uDPort[5],uDPort[6],uDPort[7],uDPort[8],uDPort[9]);
#ifdef NEED_USER_DEF2_IN_POLICY	// newFeature,  ## 1.04.46xx
;
#else
	retVar = addPolicyByAp(tid, grpID, 0, CBAC_DENY_ACT, 0, 0, name, ip, ipD, schId, log, service, TRUE, uProtocol, uDPort, TRUE);
#endif // NEED_USER_DEF2_IN_POLICY
//printf("--------->[%s]%d\n", __FUNCTION__, __LINE__);
///dprintf("addPolicyByAp() retVar=%d\n", retVar);
	if (retVar <= 0) return (retVar - 5);

	return TRUE;
}
/////////------------------------------ui.c----------------------------------
#ifdef CBAC_FRAME_FILTER
//**************************************************************************************************
// FUNCTION:  setFrameFilterPolicyDetail
// PURPOSE:   add/modify policy to frame filter access-list
// PARAMETERS:
// 			short grpID 			: group array index (0 based)
//			unsigned short plyID	: policy ID
//			char action				: access right (define in config.h)
//			unsigned char *sMac		: source MAC array (NULL for no use)
//			unsigned short eTypeLo	: lower bound of ethernet type
//			unsigned short eTypeHi	: upper bound of ethernet type (both eTypeLo & eTypeHi 0 for no use)
// RETURN:
//			1: set succeed,
//			-1: update group fail,  -2: on-line activate fail,  -3: insert fail (pool full)
//			-4: synchronize limited
//***************************************************************************************************
// newFeature,  ## 1.04.49xx
int setFrameFilterPolicyDetail(short grpID, unsigned short plyID, char action, unsigned char *sMac, unsigned short eTypeLo, unsigned short eTypeHi) {
	int i, eptMac;
	long ruleID = -1, newSlot = -1, oldSlot = -1;
	short freeGrpSlot = -1;
	unsigned char allMask[6] = {255, 255, 255, 255, 255, 255};
	unsigned char nullMac[6] = {0, 0, 0, 0, 0, 0};
	char tmpBuf[MID_CCFG_MAX_VALUE_LEN];
	int tmpValue[1];
	char rules[MID_CCFG_MAX_VALUE_LEN];

#if ATRACE >= 3
	dprintf("setFrameFilterPolicy() plyID:%u, grpID:%d, sMac:%02x:%02x:%02x:%02x:%02x:%02x\n", plyID, grpID, sMac[0], sMac[1], sMac[2], sMac[3], sMac[4], sMac[5]);
#endif
	// find policy position in rule pool
	for (i = 0 ; i < MAX_CBAC_FRAME_RULE_NUM ; i++) {
		// check this rule is valid
//		if (!gSetting.ruleFrame[i].grpIdx)
		if(mapi_ccfg_idx_get_long( tid, "ruleFrame", "grpIdx", i, 0) == 0)
		{
			if (newSlot == -1) newSlot = i;
			continue;
		}

//		if (gSetting.ruleFrame[i].policyId == plyID)
		if(mapi_ccfg_idx_get_long( tid, "ruleFrame", "policyId", i, 0) == plyID)
		{
			oldSlot = i;
			ruleID = oldSlot;
			if (newSlot != -1) break;
		}
	}
	if (ruleID == -1) {
		if (newSlot == -1) return -3;
		ruleID = newSlot;
	}
#if ATRACE >= 3
	dprintf("setFrameFilterPolicy() plyID:%u, ruleID:%ld, oldSlot:%ld, newSlot:%ld\n", plyID, ruleID, oldSlot, newSlot);
#endif

	// update rule content
	BLOCK_PREEMPTION;
	if (oldSlot == -1) {
//		memset(&gSetting.ruleFrame[ruleID], 0, sizeof(struct cRuleFrame_s));
//		mt_sprintf(gSetting.ruleFrame[ruleID].name, "frameRule%d", plyID);
//		gSetting.ruleFrame[ruleID].grpIdx = grpID + 1;
//		gSetting.ruleFrame[ruleID].policyId = plyID;
		mapi_ccfg_idx_unset_all( tid, "ruleFrame", ruleID);
		sprintf(tmpBuf, "frameRule%d", plyID);
		mapi_ccfg_idx_set_str( tid, "ruleFrame", "name", ruleID, tmpBuf);
		mapi_ccfg_idx_set_long( tid, "ruleFrame", "grpIdx", ruleID, (long)grpID + 1);
		mapi_ccfg_idx_set_long( tid, "ruleFrame", "policyId", ruleID, (long)plyID);
	}
//	gSetting.ruleFrame[ruleID].action = action;
	mapi_ccfg_idx_set_long( tid, "ruleFrame", "action", ruleID, action);

	// for useless empty entry, must permit (or socket bind will error)
	// for useless entry, just disable
	// setting MAC field
	eptMac = FALSE;
	if ((memcmp(sMac, nullMac, 6) == 0) || (sMac == NULL)) {
//		memset(&gSetting.ruleFrame[ruleID].mask[CBAC_FRAME_SMAC], 0, 6);
//		memset(&gSetting.ruleFrame[ruleID].content[CBAC_FRAME_SMAC], 0, 6);
		mapi_ccfg_idx_get_str( tid, "ruleFrame", "mask", ruleID, tmpBuf, sizeof(tmpBuf), "");
		tmpValue[0] = 0;
		copyArrayToList(tmpBuf, ',', tmpValue, UMNGCFG_INT32_TYPE, 1, CBAC_FRAME_SMAC, 6, 10);
		mapi_ccfg_idx_set_str( tid, "ruleFrame", "mask", ruleID, tmpBuf);
		mapi_ccfg_idx_set_str( tid, "ruleFrame", "content", ruleID, tmpBuf, sizeof(tmpBuf), "");
		tmpValue[0] = 0;
		copyArrayToList(tmpBuf, ',', tmpValue, UMNGCFG_INT32_TYPE, 1, CBAC_FRAME_SMAC, 6, 10);
		mapi_ccfg_idx_set_str( tid, "ruleFrame", "content", ruleID, tmpBuf);

		eptMac = TRUE;
	}
	else {
//		memcpy(&gSetting.ruleFrame[ruleID].mask[CBAC_FRAME_SMAC], allMask, 6);
//		memcpy(&gSetting.ruleFrame[ruleID].content[CBAC_FRAME_SMAC], sMac, 6);
//		gSetting.ruleFrame[ruleID].func = TRUE;
		tmpBuf[0] = '\0';
		copyArrayToList(tmpBuf, ',', allMask, UMNGCFG_INT32_TYPE, 6, CBAC_FRAME_SMAC, 6, 10);
		mapi_ccfg_idx_set_str( tid, "ruleFrame", "mask", ruleID, tmpBuf);
		tmpBuf[0] = '\0';
		copyArrayToList(value, ',', sMac, UMNGCFG_INT32_TYPE, 6, CBAC_FRAME_SMAC, 6, 16);
		mapi_ccfg_idx_set_str( tid, "ruleFrame", "content", ruleI, tmpBufD);
		mapi_ccfg_idx_set_long( tid, "ruleFrame", "func", ruleID, TRUE);


	}

	// setting ethernet type field
	if ((eTypeLo == 0) && (eTypeHi == 0)) {
//		memset(&gSetting.ruleFrame[ruleID].mask[CBAC_FRAME_ETHER], 0, 2);
//		memset(&gSetting.ruleFrame[ruleID].content[CBAC_FRAME_ETHER], 0, 2);
//		if (eptMac == TRUE) gSetting.ruleFrame[ruleID].func = FALSE;
		mapi_ccfg_idx_get_str( tid, "ruleFrame", "mask", ruleID, tmpBuf, sizeof(tmpBuf), "");
		tmpValue[0] = 0;
		copyArrayToList(tmpBuf, ',', tmpValue, UMNGCFG_INT32_TYPE, 1, CBAC_FRAME_ETHER, 2, 10);
		mapi_ccfg_idx_set_str( tid, "ruleFrame", "mask", ruleID, tmpBuf);
		mapi_ccfg_idx_get_str( tid, "ruleFrame", "content", ruleID, tmpBuf, sizeof(tmpBuf), "");
		tmpValue[0] = 0;
		copyArrayToList(tmpBuf, ',', tmpValue, UMNGCFG_INT32_TYPE, 1, CBAC_FRAME_ETHER, 2, 10);
		mapi_ccfg_idx_set_str( tid, "ruleFrame", "content", ruleID, tmpBuf);
		if (eptMac == TRUE)
			mapi_ccfg_idx_set_long( tid, "ruleFrame", "func", ruleID, FALSE);

	}
	else {
//		memcpy(&gSetting.ruleFrame[ruleID].content[CBAC_FRAME_ETHER], (unsigned char *)&eTypeLo, 2);
//		memcpy(&gSetting.ruleFrame[ruleID].mask[CBAC_FRAME_ETHER], (unsigned char *)&eTypeHi, 2);
//		gSetting.ruleFrame[ruleID].func = TRUE;
		mapi_ccfg_idx_get_str( tid, "ruleFrame", "content", ruleID, tmpBuf, sizeof(tmpBuf), "");
		copyArrayToList(tmpBuf, ',', eTypeLo, UMNGCFG_INT32_TYPE, 2, CBAC_FRAME_ETHER, 2, 10);
		mapi_ccfg_idx_set_str( tid, "ruleFrame", "content", ruleID, tmpBuf);
		mapi_ccfg_idx_get_str( tid, "ruleFrame", "mask", ruleID, tmpBuf, sizeof(tmpBuf), "");
		copyArrayToList(tmpBuf, ',', eTypeHi, UMNGCFG_INT32_TYPE, 2, CBAC_FRAME_ETHER, 2, 10);
		mapi_ccfg_idx_set_str( tid, "ruleFrame", "mask", ruleID, tmpBuf);
		mapi_ccfg_idx_set_long( tid, "ruleFrame", "func", ruleID, TRUE);

	}
	UNBLOCK_PREEMPTION;

	// on-line activate
	if (oldSlot == -1) {
//		if (!lockGroup(grpID, -1)) return -4;

		// find free slot in rule group
		mapi_ccfg_idx_get_str( tid, "ruleGrp", "rules", grpID, rules, sizeof(lists), "");
		for (i = 0 ; i < (HALF_GRP_RULE_NUM * 2) ; i++) {
//			if (gSetting.ruleGrp[grpID].rules[i] == 0) {
			if(getItemInList(rules, ',', i,0) == 0){
				freeGrpSlot = i;
				break;
			}
		}
		if (freeGrpSlot == -1) {
//			unlockGroup(grpID, -1);
			return -1;
		}

		// update group rule & activate
#if ATRACE >= 3
		dprintf("setFrameFilterPolicy() freeGrpSlot: %d\n", freeGrpSlot);
#endif
//		gSetting.ruleGrp[grpID].rules[freeGrpSlot] = ruleID + 1;
		strcpy(tmpBuf, rules);
		tmpValue[0] = ruleID + 1;
		copyArrayToList(tmpBuf, ',', tmpValue, UMNGCFG_INT32_TYPE, 1, freeGrpSlot, 1, 10);
		mapi_ccfg_idx_set_str( tid, "ruleGrp", "rules", grpID, tmpBuf);

		if ((freeGrpSlot + 1) < (HALF_GRP_RULE_NUM * 2)){
//			gSetting.ruleGrp[grpID].rules[freeGrpSlot + 1] = 0;
			tmpValue[0] = 0;
			copyArrayToList(tmpBuf, ',', tmpValue, UMNGCFG_INT32_TYPE, 1, freeGrpSlot + 1, 1, 10);
			mapi_ccfg_idx_set_str( tid, "ruleGrp", "rules", grpID, tmpBuf);
		}

//		unlockGroup(grpID, -1);

//		if (!updateGroup(tid, grpID)) return -2;
	}

	return 1;
}
//**************************************************************************************************
// FUNCTION:  setFrameFilterPolicy
// PURPOSE:   add/modify policy to frame filter access-list
// PARAMETERS:
// 			short grpID 			: group array index (0 based)
//			unsigned short plyID	: policy ID
//			char action				: access right (define in config.h)
//			unsigned char *sMac		: source MAC array (NULL for no use)
// RETURN:
//			1: set succeed,
//			-1: update group fail,  -2: on-line activate fail,  -3: insert fail (pool full)
//			-4: synchronize limited
//***************************************************************************************************
// newFeature,  ## 1.04.49xx
int setFrameFilterPolicy(short grpID, unsigned short plyID, char action, unsigned char *sMac) {
	return setFrameFilterPolicyDetail(grpID, plyID, action, sMac, 0, 0);
}

////**************************************************************************************************
//// FUNCTION:  getFrameFilterPolicyDetail
//// PURPOSE:   get policy status by input policy ID
//// PARAMETERS:
//// 			short grpID 			: group array index (0 based, until now is useless)
////			unsigned short plyID	: policy ID
////			unsigned char *sMac		: source MAC array (NULL for no use)
////			unsigned short *eTypeLo	: lower bound of ethernet type (NULL for no use)
////			unsigned short *eTypeHi	: upper bound of ethernet type (NULL for no use)
//// RETURN:
////			1: get succeed,  -1: invalid policy ID
////***************************************************************************************************
//// newFeature,  ## 1.04.49xx
int getFrameFilterPolicyDetail(short grpID, unsigned short plyID, unsigned char *sMac, unsigned short *eTypeLo, unsigned short *eTypeHi) {
	int i;
	long ruleID = -1;
	char tmpBuf[32];

	// get rule index
	for (i = 0 ; i < MAX_CBAC_FRAME_RULE_NUM ; i++) {
		// check this rule is valid
//		if (!gSetting.ruleFrame[i].grpIdx) continue;
		if(mapi_ccfg_idx_get_long( tid, "ruleFrame", "grpIdx", i, 0) == 0) continue;

//		if (gSetting.ruleFrame[i].policyId == plyID) {
		if(mapi_ccfg_idx_get_long( tid, "ruleFrame", "grpIdx", i, 0) == plyID){
			ruleID = i;
			break;
		}
	}
	if (ruleID == -1) return -1;

#if ATRACE >= 3
	dprintf("getFrameFilterPolicy() plyID:%u, rule:%ld\n", plyID, ruleID);
#endif

	// collect status
//	if (sMac) memcpy(sMac, &gSetting.ruleFrame[ruleID].content[CBAC_FRAME_SMAC], 6);
//	if (eTypeLo) memcpy((unsigned char *)eTypeLo, &gSetting.ruleFrame[ruleID].content[CBAC_FRAME_ETHER], 2);
//	if (eTypeHi) memcpy((unsigned char *)eTypeHi, &gSetting.ruleFrame[ruleID].mask[CBAC_FRAME_ETHER], 2);
	mapi_ccfg_idx_get_long( tid, "ruleFrame", "content", ruleID, "", tmpBuf, sizeof(tmpBuf));
//	if (sMac) copyListToArray(sMac, UMNGCFG_UINT8_TYPE, tmpBuf+indexOfC(tmpBuf, ',', CBAC_FRAME_SMAC)+1, ',', 6);
	if (mac) sscanf(tmpBuf+indexOfC(tmpBuf+indexOfC(tmpBuf, ',', CBAC_FRAME_SMAC)+1, "%hhx,%hhx,%hhx,%hhx,%hhx,%hhx", sMac[0], sMac[1], sMac[2], sMac[3], sMac[4], sMac[5]);
	if (eTypeLo) copyListToArray(eTypeLo, UMNGCFG_UINT8_TYPE, tmpBuf+indexOfC(tmpBuf, ',', CBAC_FRAME_ETHER)+1, ',', 2);
	if (eTypeHi) copyListToArray(eTypeHi, UMNGCFG_UINT8_TYPE, tmpBuf+indexOfC(tmpBuf, ',', CBAC_FRAME_ETHER)+1, ',', 2);
	return 1;
}

//**************************************************************************************************
// FUNCTION:  getFrameFilterPolicy
// PURPOSE:   get policy status by input policy ID
// PARAMETERS:
// 			short grpID 			: group array index (0 based, until now is useless)
//			unsigned short plyID	: policy ID
//			unsigned char *sMac		: source MAC array (NULL for no use)
// RETURN:
//			1: get succeed,  -1: invalid policy ID
//***************************************************************************************************
// newFeature,  ## 1.04.49xx
int getFrameFilterPolicy(short grpID, unsigned short plyID, unsigned char *sMac) {
	return getFrameFilterPolicyDetail(grpID, plyID, sMac, NULL, NULL);
}
#endif //CBAC_FRAME_FILTER
//**************************************************************************************************
// FUNCTION:  setExtPolicyAct
// PURPOSE:   add/modify policy to extended access-list
// PARAMETERS:
// 			short grpID 			: group array index (for NDC/SMC like UI always set 0, 0 based)
//			unsigned short plyID	: policy ID
//			unsigned char sip_lo	: lower bound ip address (0 for no use)
//			unsigned char sip_hi	: hi bound ip address (0 for no use)
//			unsigned short sPort_lo	: source port lower boundary (0 for no use)
//			unsigned short sPort_hi	: source port upper boundary (0 for no use)
//			unsigned char *sMac		: source MAC array (NULL for no use)
//			char protoType			: 0- no use,  1- TCP,  2- UDP,  3- Both
//			char action				: action (CBAC_DENY_ACT/CBAC_PERMIT_ACT)
// 			(can delete policy by setting all value to no use)
// RETURN:
//			1: set succeed,
//			-1: update group fail,  -2: update std rule pool fail,  -3: update ext rule pool fail
//			-4: on-line activate fail,  -5: delete old rule fail,  -6: insert fail (pool full)
//***************************************************************************************************
int setExtPolicyAct(int tid, short grpID, unsigned short plyID, unsigned char sip_lo, unsigned char sip_hi, unsigned short sPort_lo, unsigned short sPort_hi, unsigned char *sMac, char protoType, char action) {
	unsigned char proto[2];
	unsigned char ifIP[4];//(unsigned char *)&if_table[T_LAN_INT].ipaddr;
	long i, retVar, num = 0, ruleID[2];
	char hasSet = 0;
	char tmpBuf[32];
	int tmpValue[1];
	//char *tmp;

	mapi_ccfg_get_str( tid, "lan0", "ip4addr", tmpBuf, sizeof(tmpBuf), "0.0.0.0" );
	utilIpStr2Ip( tmpBuf, ifIP );
	// get old rule index
	retVar = deletePolicyByName_ID(tid, grpID, NULL, plyID);
	if ((retVar < 0) && (retVar != -4)) return retVar; // return -1, -2 or -3
	// check protocol & find empty access-list rule entry
	if (protoType == 0) {
		num = 1;
		if ((ruleID[0] = findFreeRuleSlot(tid, CBAC_EXTEND_TYPE, TRUE)) == -1) return -6;
		proto[0] = 0;
	}
	if (protoType == 1) {
		num = 1;
		if ((ruleID[0] = findFreeRuleSlot(tid, CBAC_EXTEND_TYPE, TRUE)) == -1) return -6;
		proto[0] = CBAC_TCP_PROTO;
	}
	else if (protoType == 2) {
		num = 1;
		if ((ruleID[0] = findFreeRuleSlot(tid, CBAC_EXTEND_TYPE, TRUE)) == -1) return -6;
		proto[0] = CBAC_UDP_PROTO;
	}
	else if (protoType == 3) {
		num = 2;
		if ((ruleID[0] = findFreeRuleSlot(tid, CBAC_EXTEND_TYPE, TRUE)) == -1) return -6;
		if ((ruleID[1] = findFreeRuleSlot(tid, CBAC_EXTEND_TYPE, TRUE)) == -1) return -6;
		proto[0] = CBAC_TCP_PROTO;
		proto[1] = CBAC_UDP_PROTO;
	}
	// add rule
	for (i = 0 ; i < num ; i++) {
		// setting value to gSetting
		//memset(&gSetting.ruleExt[ruleID[i]], 0, sizeof(struct cRuleExtend_s));
		mapi_ccfg_idx_unset_all( tid, "Firewallruls", ruleID[i]);
		hasSet = 0;

//		mt_sprintf(gSetting.ruleExt[ruleID[i]].name, "LinkSys%u", plyID);
//		gSetting.ruleExt[ruleID[i]].scheduleIdx = 0;
//		gSetting.ruleExt[ruleID[i]].policyId = plyID;
//		gSetting.ruleExt[ruleID[i]].priority = plyID;
//		gSetting.ruleExt[ruleID[i]].action = action;
//		gSetting.ruleExt[ruleID[i]].func = TRUE;	// must set active this var
//		memset(&gSetting.ruleExt[ruleID[i]].srcIP[4], 255, 4);
//		memset(&gSetting.ruleExt[ruleID[i]].dstIP[4], 255, 4);
//		gSetting.ruleExt[ruleID[i]].proto = proto[i];

		sprintf(tmpBuf, "LinkSys%u", plyID);
		mapi_ccfg_idx_set_str( tid, "Firewallruls", "name", ruleID[i], tmpBuf);
		mapi_ccfg_idx_set_long( tid, "Firewallruls", "scheduleIdx", ruleID[i], 0);
		mapi_ccfg_idx_set_long( tid, "Firewallruls", "policyId", ruleID[i], (long)plyID);
		mapi_ccfg_idx_set_long( tid, "Firewallruls", "priority", ruleID[i], (long)plyID);
		mapi_ccfg_idx_set_long( tid, "Firewallruls", "action", ruleID[i], (long)action);
		mapi_ccfg_idx_set_long( tid, "Firewallruls", "func", ruleID[i], TRUE);
		mapi_ccfg_idx_get_str( tid, "Firewallruls", "srcIP", ruleID[i], tmpBuf, sizeof(tmpBuf), "");
		tmpValue[0] = 255;
		copyArrayToList(tmpBuf, ',', tmpValue, UMNGCFG_INT32_TYPE, 1, 4, 4, 10);
		mapi_ccfg_idx_set_str( tid, "Firewallruls", "srcIP", ruleID[i], tmpBuf);
		mapi_ccfg_idx_get_str( tid, "Firewallruls", "dstIP", ruleID[i], tmpBuf, sizeof(tmpBuf), "");
		tmpValue[0] = 255;
		copyArrayToList(tmpBuf, ',', tmpValue, UMNGCFG_INT32_TYPE, 1, 4, 4, 10);
		mapi_ccfg_idx_set_str( tid, "Firewallruls", "dstIP", ruleID[i], tmpBuf);
		mapi_ccfg_idx_set_long( tid, "Firewallruls", "proto", ruleID[i], (long)proto[i]);

		if (sPort_lo && sPort_hi) {
//			gSetting.ruleExt[ruleID[i]].srcPort[0] = sPort_lo;
//			gSetting.ruleExt[ruleID[i]].srcPort[1] = sPort_hi;
			mapi_ccfg_idx_get_str( tid, "Firewallruls", "srcPort", ruleID[i], tmpBuf, sizeof(tmpBuf), "");
			setItemInList(tmpBuf, sizeof(tmpBuf), ',', 0, sPort_lo);
			setItemInList(tmpBuf, sizeof(tmpBuf), ',', 1, sPort_hi);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "srcPort", ruleID[i], tmpBuf);
			hasSet++;
		}
#ifdef NEED_SRC_MAC_FILTER
		if (sMac) {
//			memcpy(gSetting.ruleExt[ruleID[i]].sMac, sMac, 6);
			tmpBuf[0] = '\0';
			copyArrayToList(tmpBuf, ',', sMac, UMNGCFG_UINT8_TYPE, 6, 0, 6, 16);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "sMac", ruleID[i], tmpBuf);
			hasSet++;
		}
#endif // NEED_SRC_MAC_FILTER
		if (sip_lo && sip_hi) {
//			gSetting.ruleExt[ruleID[i]].sSubnet[0] = ifIP[0];
//			gSetting.ruleExt[ruleID[i]].sSubnet[1] = ifIP[1];
//			gSetting.ruleExt[ruleID[i]].sSubnet[2] = ifIP[2];
//			gSetting.ruleExt[ruleID[i]].sRange[0] = sip_lo;
//			gSetting.ruleExt[ruleID[i]].sRange[1] = sip_hi;
			mapi_ccfg_idx_get_str( tid, "Firewallruls", "sSubnet", ruleID[i], tmpBuf, sizeof(tmpBuf), "");
			copyArrayToList( tmpBuf, ',', ifIP, UMNGCFG_UINT8_TYPE, 3, 0, 3, 10);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "sSubnet", ruleID[i], tmpBuf);
			mapi_ccfg_idx_get_str( tid, "Firewallruls", "sRange", ruleID[i], tmpBuf, sizeof(tmpBuf), "");
			setItemInList(tmpBuf, sizeof(tmpBuf), ',', 0, sip_lo);
			setItemInList(tmpBuf, sizeof(tmpBuf), ',', 1, sip_hi);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "sRange", ruleID[i], tmpBuf);
			hasSet++;
		}
		// until last value set OK, unlock group lock
//		gSetting.ruleExt[ruleID[i]].grpIdx = grpID + 1;
		mapi_ccfg_idx_set_long( tid, "Firewallruls", "grpIdx", ruleID[i], (long)grpID + 1);
		if (hasSet) {
			// insert new rule to group list, dependence on rule priority, and setting new rule priority
			insertExtRuleToGroup(tid, grpID, ruleID[i], plyID);

			// update run-time var (if some rule activate fail, following rule will not activate)
//			if (!updateExtendRule(ruleID[i]) || !updateGroup(tid, grpID)) return -4;
			if (!updateGroup(tid, grpID)) return -4;
		}
		else {
//			memset(&gSetting.ruleExt[ruleID[i]], 0, sizeof(struct cRuleExtend_s));
			mapi_ccfg_idx_unset_all( tid, "Firewallruls", ruleID[i]);
		}

	}

	return 1;
}

//**************************************************************************************************
// FUNCTION:  setExtPolicy
// PURPOSE:   add/modify policy to extended access-list
// PARAMETERS:
// 			short grpID 			: group array index (for NDC/SMC like UI always set 0, 0 based)
//			unsigned short plyID	: policy ID
//			unsigned char sip_lo	: lower bound ip address (0 for no use)
//			unsigned char sip_hi	: hi bound ip address (0 for no use)
//			unsigned short sPort_lo	: source port lower boundary (0 for no use)
//			unsigned short sPort_hi	: source port upper boundary (0 for no use)
//			unsigned char *sMac		: source MAC array (NULL for no use)
//			char protoType			: 0- no use,  1- TCP,  2- UDP,  3- Both
// 			(can delete policy by setting all value to no use)
// RETURN:
//			1: set succeed,
//			-1: update group fail,  -2: update std rule pool fail,  -3: update ext rule pool fail
//			-4: on-line activate fail,  -5: delete old rule fail,  -6: insert fail (pool full)
//
// Note
//		* related sections: Firewallruls, ruleGrp, ruleFrame
//
//***************************************************************************************************
int mapi_ccfgsal_firewall_set_ExtPolicy(int tid, short grpID, unsigned short plyID, unsigned char sip_lo, unsigned char sip_hi, unsigned short sPort_lo, unsigned short sPort_hi, unsigned char *sMac, char protoType) {
	return setExtPolicyAct(tid, grpID, plyID, sip_lo, sip_hi, sPort_lo, sPort_hi, sMac, protoType, CBAC_PERMIT_ACT);
}

//**************************************************************************************************
// FUNCTION:  getExtPolicy
// PURPOSE:   get policy status by input policy ID
// PARAMETERS:
// 			short grpID 			: group array index (for NDC/SMC like UI always set 0)
//			unsigned short plyID	: policy ID
//			unsigned char *sip_lo	: lower bound ip address (NULL for no use)
//			unsigned char *sip_hi	: hi bound ip address (NULL for no use)
//			unsigned short *sPort_lo: source port lower boundary (NULL for no use)
//			unsigned short *sPort_hi: source port upper boundary (NULL for no use)
//			unsigned char *sMac		: source MAC array (NULL for no use)
//			char *protoType			: 0- no use,  1- TCP,  2- UDP,  3- Both (NULL for no use)
// RETURN:
//			1: get succeed,  -1: invalid policy ID
//***************************************************************************************************
int mapi_ccfgsal_firewall_get_ExtPolicy(int tid, short grpID, unsigned short plyID, unsigned char *sip_lo, unsigned char *sip_hi, unsigned short *sPort_lo, unsigned short *sPort_hi, unsigned char *sMac, char *protoType) {
	long ruleID[20];	// ruleID[2], bugFix.  ## 1.04.3541,  ## 1.04.xx41
	unsigned short num = 0;
	int proto;
	char tmpBuf[32];

	// get rule index
	if ((num = getRuleByPolicy(tid, grpID, plyID, plyID, ruleID, NULL, 2)) == 0)
		return -1;

#if ATRACE >= 0
	dprintf("getExtPolicy() num: %u,  rule[0]: %ld,  rule[1]: %ld\n", num, ruleID[0], ruleID[1]);
#endif

	// collect status
	if (sip_lo && sip_hi) {
//		*sip_lo = gSetting.ruleExt[ruleID[0]].sRange[0];
//		*sip_hi = gSetting.ruleExt[ruleID[0]].sRange[1];

		mapi_ccfg_idx_get_str( tid, "Firewallruls", "sRange", ruleID[0], tmpBuf, sizeof(tmpBuf), "");
		*sip_lo = getItemInList(tmpBuf, ',', 0,0);
		*sip_hi = getItemInList(tmpBuf, ',', 1,0);
	}
	if (sPort_lo && sPort_hi) {
//		*sPort_lo = gSetting.ruleExt[ruleID[0]].srcPort[0];
//		*sPort_hi = gSetting.ruleExt[ruleID[0]].srcPort[1];
		mapi_ccfg_idx_get_str( tid, "Firewallruls", "srcPort", ruleID[0], tmpBuf, sizeof(tmpBuf), "");
		*sPort_lo = getItemInList(tmpBuf, ',', 0,0);
		*sPort_hi = getItemInList(tmpBuf, ',', 1,0);
	}
#ifdef NEED_SRC_MAC_FILTER
//	if (sMac) memcpy(sMac, gSetting.ruleExt[ruleID[0]].sMac, 6);
	if (sMac){
		mapi_ccfg_idx_get_str( tid, "Firewallruls", "sMac", ruleID[0], tmpBuf, sizeof(tmpBuf), "");
		sscanf(tmpBuf, "%hhx,%hhx,%hhx,%hhx,%hhx,%hhx", &sMac[0], &sMac[1], &sMac[2], &sMac[3], &sMac[4], &sMac[5]);
	}
#endif // NEED_SRC_MAC_FILTER
	if (protoType) {
		proto = (int)mapi_ccfg_idx_get_long( tid, "Firewallruls", "proto", ruleID[0], 0);
		if (proto == CBAC_TCP_PROTO) {
			if (num == 1)
				*protoType = 1;
			else if (proto == CBAC_UDP_PROTO)
				*protoType = 3;
		}
		else if (proto == CBAC_UDP_PROTO) {
			if (num == 1)
				*protoType = 2;
			else if (mapi_ccfg_idx_get_long( tid, "Firewallruls", "proto", ruleID[1], 0) == CBAC_TCP_PROTO)
				*protoType = 3;
		}
	}

	return 1;
}

#ifdef CBAC_URL_BLOCKING
//**************************************************************************************************
// FUNCTION:  getURLKeyword
// PURPOSE:   get URL blocking keyword string
// PARAMETERS:
//			short idx	: string index ('0' based, maximun size of keyword string pool is MAX_URL_NUM, ref. config.h)
// RETURN:
//			char *kword	: keyword string buffer (length = MAX_URL_NAME_LEN, ref. config.h)
//			1: setting success,  0: invalid index number
//***************************************************************************************************
char mapi_ccfgsal_firewall_get_URLKeyword(int tid, char *kword, short idx) {
	// check index number
	if ((idx >= MAX_URL_NUM) || (idx < 0)) return 0;

	// get value
	memset(kword, 0, MAX_URL_NAME_LEN);
//	if (!URLconfFlag[idx])
//		strcpy(kword, gSetting.urlKeyword[idx]);
	mapi_ccfg_idx_get_str( tid, "Firewall", "urlKeyword", idx, kword, MAX_URL_NAME_LEN, "");

	return 1;
}
//**************************************************************************************************
// FUNCTION:  setURLKeyword
// PURPOSE:   set URL blocking keyword string (will truncate useless leading and tail space/tab characters)
// PARAMETERS:
//			char *kword	: keyword string (length(include NULL end) < MAX_URL_NAME_LEN, ref. config.h)
//			short idx	: string index ('0' based, maximun size of keyword string pool is MAX_URL_NUM, ref. config.h)
// RETURN:
//			1: setting success,  0: invalid index number
//
// Note
//		* related sections: Firewall
//
//***************************************************************************************************
char mapi_ccfgsal_firewall_set_URLKeyword(int tid, char *kword, short idx) {
	int len, st = 0;
	char value[MAX_URL_NAME_LEN];

	// check index number
	if ((idx >= MAX_URL_NUM) || (idx < 0)) return 0;

	// truncate useless head and tail space/tab
	while((kword[st] == ' ') || (kword[st] == 9)) st++;
	len = strlen(kword);
	// bugfix  # 1.04.2228
	while((kword[len - 1] == ' ') || (kword[len - 1] == 9)/* || (kword[len - 1] == '/')*/) {
		kword[len - 1] = 0;
		len--;
	}

	// setting value
//	URLconfFlag[idx] = TRUE;
//	memset(gSetting.urlKeyword[idx], 0, MAX_URL_NAME_LEN);
	mapi_ccfg_idx_unset_all( tid, "Firewall", idx);
	if ((len - st) >= MAX_URL_NAME_LEN){
//		memcpy(gSetting.urlKeyword[idx], &kword[st], (MAX_URL_NAME_LEN - 1));
		memcpy(value, &kword[st], (MAX_URL_NAME_LEN - 1));
	}
	else{
//		strcpy(gSetting.urlKeyword[idx], &kword[st]);
		strcpy(value, &kword[st]);
	}
	mapi_ccfg_idx_set_str( tid, "Firewall", "urlKeyword", idx, value);
//	URLconfFlag[idx] = FALSE;

	// on-line activate
//	UpdateURLString(idx);

	return 1;
}

#endif //def CBAC_URL_BLOCKING


//**************************************************************************************************
// FUNCTION:  setCBACFuncPure
// PURPOSE:   only setting generic CBAC function status
// PARAMETERS:
//			signed char inspect	: inspect function on/off (TRUE/FALSE) (-1 for no use)
//			signed char dos		: DoS function on/off (TRUE/FALSE) (-1 for no use)
//			signed char ping	: deny ping from WAN on/off (TRUE/FALSE) (-1 for no use)
//			signed char rip		: deny RIP v1 from WAN on/off (TRUE/FALSE) (-1 for no use)
//			signed char dmz		: dmz function on/off (TRUE/FALSE) (-1 for no use)
// RETURN:
//			1: setting success
//***************************************************************************************************
// newFeature,  ## 1.04.36xx
char setCBACFuncPure(int tid, signed char inspect, signed char dos, signed char ping, signed char rip, signed char dmz) {
	int tmpValue;
#ifdef CBAC_INSPECT_DOS
//	if (ping != -1) gSetting.pingWAN = ping;
//	if (dos != -1) gSetting.dos_func = dos;
//	if (rip != -1) gSetting.ripDef = rip;
//	if (dmz != -1) gSetting.dmzFunc = dmz;

	if (ping != -1) mapi_ccfg_set_long( tid, "Firewall", "pingWAN", (long)ping);
	if (dos != -1)  mapi_ccfg_set_long( tid, "Firewall", "dos_func", (long)dos);
	if (rip != -1)  mapi_ccfg_set_long( tid, "Firewall", "ripDef", (long)rip);
	if (dmz != -1)  mapi_ccfg_set_long( tid, "DMZ", "func", (long)dmz);

#endif // CBAC_INSPECT_DOS

#if 1 //defined(CBAC_INSPECT_FRAGMENT) || defined(CBAC_INSPECT_TCP) || defined(CBAC_INSPECT_UDP)
	// to adapt to qos-inspect function, by ygchen
	// set only the last bit of gSetting.inspectGlobal.func
	if (inspect != -1){
//		gSetting.inspectGlobal.func = (gSetting.inspectGlobal.func & 0xFE) | (inspect & 0x01);
		tmpValue = (int)mapi_ccfg_get_long( tid, "inspectGlobal", "func", 0);
		tmpValue = (tmpValue & 0xFE) | (inspect & 0x01);
		mapi_ccfg_set_long( tid, "inspectGlobal", "func", (long)tmpValue);
	}
#endif // defined(CBAC_INSPECT_FRAGMENT) || defined(CBAC_INSPECT_TCP) || defined(CBAC_INSPECT_UDP)

	return 1;
}

//**************************************************************************************************
// FUNCTION:  setCBACFunc
// PURPOSE:   setting generic CBAC function status
// PARAMETERS:
//			signed char inspect	: inspect function on/off (TRUE/FALSE) (-1 for no use)
//			signed char dos		: DoS function on/off (TRUE/FALSE) (-1 for no use)
//			signed char ping	: deny ping from WAN on/off (TRUE/FALSE) (-1 for no use)
//			signed char rip		: deny RIP v1 from WAN on/off (TRUE/FALSE) (-1 for no use)
//			signed char dmz		: dmz function on/off (TRUE/FALSE) (-1 for no use)
// RETURN:
//			1: setting success
//***************************************************************************************************
char setCBACFunc(int tid, signed char inspect, signed char dos, signed char ping, signed char rip, signed char dmz) {
	//char	tmpBuf[32];
	int idxIf=0;
	// if SPI & DoS disable, WAN input access control must disable (vise versa)
	if (inspect != -1) {
		if (inspect) {
#if NEED_NEW_ACL_DEFAULT_VAL == 0

			// setting inspection rule & inbound access list number
  #ifdef _IPSEC
//			gSetting.Interface[0].ruleGrpIdx[0] = 2;
//			gSetting.Interface[0].ruleInsIdx[0] = 1;
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "Loopback" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 2);
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleInsIdxIn", idxIf, 1);
  #endif // _IPSEC
//			gSetting.Interface[T_WAN_INT].ruleGrpIdx[0] = 2;
//			gSetting.Interface[T_WAN_INT].ruleInsIdx[0] = 1;
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "wan" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 2);
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleInsIdxIn", idxIf, 1);
  #ifdef _PPPOE
	#if NEED_MULTIPLE_PPPOE == 1
			{
				int i;

				for (i = 0 ; i < N_PPPoE_NUM ; i++) {
//					gSetting.Interface[i].ruleGrpIdx[0] = 2;
//					gSetting.Interface[i].ruleInsIdx[0] = 1;
					sprintf(tmpBuf, "ppp%d", i);
					idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", tmpBuf );
					if(idxIf < 0){
						idxIf = mapi_ccfg_idx_find_unused( tid, "FirewallIf", "if", 1 );
						mapi_ccfg_idx_set_str( tid, "FirewallIf", "if", idxIf, tmpBuf);
					}
					mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 2);
					mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleInsIdxIn", idxIf, 1);
				}
			}
	#else
//			gSetting.Interface[T_PPPoE_INT].ruleGrpIdx[0] = 2;
//			gSetting.Interface[T_PPPoE_INT].ruleInsIdx[0] = 1;
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "ppp0" );
			if(idxIf < 0){
				idxIf = mapi_ccfg_idx_find_unused( tid, "FirewallIf", "if", 1 );
				mapi_ccfg_idx_set_str( tid, "FirewallIf", "if", idxIf, "ppp0");
			}
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 2);
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleInsIdxIn", idxIf, 1);
	#endif // NEED_MULTIPLE_PPPOE == 1
  #endif // _PPPOE
  #if defined(_SSUART) || defined(_SSIO) || (_USB_UMTS != 0)
//			gSetting.Interface[T_COM1_INT].ruleGrpIdx[0] = 2;
//			gSetting.Interface[T_COM1_INT].ruleInsIdx[0] = 1;
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "com1" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 2);
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleInsIdxIn", idxIf, 1);
  #endif // defined(_SSUART) || defined(_SSIO) || (_USB_UMTS != 0)
  #if defined(_PPTP) && (NEED_WAN_PPTP == 1)
//			gSetting.Interface[T_PPTP_INT].ruleGrpIdx[0] = 2;
//			gSetting.Interface[T_PPTP_INT].ruleInsIdx[0] = 1;
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "PPTP" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 2);
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleInsIdxIn", idxIf, 1);
  #endif // defined(_PPTP) && (NEED_WAN_PPTP == 1)

			// force open access list function
  #ifdef _IPSEC
//			gSetting.Interface[0].active = 1;				// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "Loopback" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "active", idxIf, 1);
  #endif // _IPSEC
//			gSetting.Interface[T_WAN_INT].active = 1;		// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "wan" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "active", idxIf, 1);
  #ifdef _PPPOE
	#if NEED_MULTIPLE_PPPOE == 1
			{
				int i;

				for (i = 0 ; i < N_PPPoE_NUM ; i++) {
//					gSetting.Interface[i].active = 1;
					sprintf(tmpBuf, "ppp%d", i);
					idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", tmpBuf );
					if(idxIf < 0){
						idxIf = mapi_ccfg_idx_find_unused( tid, "FirewallIf", "if", 1 );
						mapi_ccfg_idx_set_str( tid, "FirewallIf", "if", idxIf, tmpBuf);
					}
					mapi_ccfg_idx_set_long( tid, "FirewallIf", "active", idxIf, 1);
				}
			}
	#else
//			gSetting.Interface[T_PPPoE_INT].active = 1;		// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "ppp0" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "active", idxIf, 1);
	#endif // NEED_MULTIPLE_PPPOE == 1
  #endif // _PPPOE
  #if defined(_SSUART) || defined(_SSIO) || (_USB_UMTS != 0)
//			gSetting.Interface[T_COM1_INT].active = 1;		// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "com1" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "active", idxIf, 1);
  #endif // defined(_SSUART) || defined(_SSIO) || (_USB_UMTS != 0)
  #if defined(_PPTP) && (NEED_WAN_PPTP == 1)
//			gSetting.Interface[T_PPTP_INT].active = 1;		// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "PPTP" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "active", idxIf, 1);
  #endif // defined(_PPTP) && (NEED_WAN_PPTP == 1)

#else

  #ifdef _IPSEC
//			gSetting.Interface[0].ruleGrpIdx[0] = 2;			// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "Loopback" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 2);
  #endif // _IPSEC
//			gSetting.Interface[T_WAN_INT].ruleGrpIdx[0] = 2;	// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "wan" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 2);
  #ifdef _PPPOE
	#if NEED_MULTIPLE_PPPOE == 1
			{
				int i;

				for (i = 0 ; i < N_PPPoE_NUM ; i++) {
//					gSetting.Interface[i].ruleGrpIdx[0] = 2;
					sprintf(tmpBuf, "ppp%d", i);
					idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", tmpBuf );
					if(idxIf < 0){
						idxIf = mapi_ccfg_idx_find_unused( tid, "FirewallIf", "if", 1 );
						mapi_ccfg_idx_set_str( tid, "FirewallIf", "if", idxIf, tmpBuf);
					}
					mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 2);
				}
			}
	#else
//			gSetting.Interface[T_PPPoE_INT].ruleGrpIdx[0] = 2;	// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "ppp0" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 2);
	#endif // NEED_MULTIPLE_PPPOE == 1
  #endif // _PPPOE
  #if defined(_SSUART) || defined(_SSIO) || (_USB_UMTS != 0)
//			gSetting.Interface[T_COM1_INT].ruleGrpIdx[0] = 2;	// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "com1" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 2);
  #endif // defined(_SSUART) || defined(_SSIO) || (_USB_UMTS != 0)
  #if defined(_PPTP) && (NEED_WAN_PPTP == 1)
//			gSetting.Interface[T_PPTP_INT].ruleGrpIdx[0] = 2;	// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "PPTP" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 2);
  #endif // defined(_PPTP) && (NEED_WAN_PPTP == 1)

#endif // NEED_NEW_ACL_DEFAULT_VAL == 0
		}
		else {
#if NEED_NEW_ACL_DEFAULT_VAL == 0

  #ifdef _IPSEC
//			gSetting.Interface[0].ruleGrpIdx[0] = 0;
//			gSetting.Interface[0].ruleInsIdx[0] = 0;
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "Loopback" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 0);
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleInsIdxIn", idxIf, 0);
  #endif // _IPSEC
//			gSetting.Interface[T_WAN_INT].ruleGrpIdx[0] = 0;
//			gSetting.Interface[T_WAN_INT].ruleInsIdx[0] = 0;
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "wan" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 0);
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleInsIdxIn", idxIf, 0);
  #ifdef _PPPOE
	#if NEED_MULTIPLE_PPPOE == 1
			{
				int i;

				for (i = 0 ; i < N_PPPoE_NUM ; i++) {
//					gSetting.Interface[i].ruleGrpIdx[0] = 0;
//					gSetting.Interface[i].ruleInsIdx[0] = 0;
					sprintf(tmpBuf, "ppp%d", i);
					idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", tmpBuf );
					if(idxIf < 0){
						idxIf = mapi_ccfg_idx_find_unused( tid, "FirewallIf", "if", 1 );
						mapi_ccfg_idx_set_str( tid, "FirewallIf", "if", idxIf, tmpBuf);
					}
					mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 0);
					mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleInsIdxIn", idxIf, 0);
				}
			}
	#else
//			gSetting.Interface[T_PPPoE_INT].ruleGrpIdx[0] = 0;
//			gSetting.Interface[T_PPPoE_INT].ruleInsIdx[0] = 0;
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "ppp0" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 0);
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleInsIdxIn", idxIf, 0);
	#endif // NEED_MULTIPLE_PPPOE == 1
  #endif // _PPPOE
  #if defined(_SSUART) || defined(_SSIO) || (_USB_UMTS != 0)
//			gSetting.Interface[T_COM1_INT].ruleGrpIdx[0] = 0;
//			gSetting.Interface[T_COM1_INT].ruleInsIdx[0] = 0;
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "com1" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 0);
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleInsIdxIn", idxIf, 0);
  #endif // defined(_SSUART) || defined(_SSIO) || (_USB_UMTS != 0)
  #if defined(_PPTP) && (NEED_WAN_PPTP == 1)
//			gSetting.Interface[T_PPTP_INT].ruleGrpIdx[0] = 0;
//			gSetting.Interface[T_PPTP_INT].ruleInsIdx[0] = 0;
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "PPTP" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 0);
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleInsIdxIn", idxIf, 0);
  #endif // defined(_PPTP) && (NEED_WAN_PPTP == 1)

			// force close access list function
  #ifdef _IPSEC
//			gSetting.Interface[0].active = 0;				// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "Loopback" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "active", idxIf, 0);
  #endif // _IPSEC
//			gSetting.Interface[T_WAN_INT].active = 0;		// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "wan" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "active", idxIf, 0);
  #ifdef _PPPOE
	#if NEED_MULTIPLE_PPPOE == 1
			{
				int i;

				for (i = 0 ; i < N_PPPoE_NUM ; i++) {
//					gSetting.Interface[i].active = 0;
					sprintf(tmpBuf, "ppp%d", i);
					idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", tmpBuf );
					if(idxIf < 0){
						idxIf = mapi_ccfg_idx_find_unused( tid, "FirewallIf", "if", 1 );
						mapi_ccfg_idx_set_str( tid, "FirewallIf", "if", idxIf, tmpBuf);
					}
					mapi_ccfg_idx_set_long( tid, "FirewallIf", "active", idxIf, 0);
				}
			}
	#else
//			gSetting.Interface[T_PPPoE_INT].active = 0;		// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "ppp0" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "active", idxIf, 0);
	#endif // NEED_MULTIPLE_PPPOE == 1
  #endif // _PPPOE
  #if defined(_SSUART) || defined(_SSIO) || (_USB_UMTS != 0)
//			gSetting.Interface[T_COM1_INT].active = 0;		// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "com1" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "active", idxIf, 0);
  #endif // defined(_SSUART) || defined(_SSIO) || (_USB_UMTS != 0)
  #ifdef _PPTP
//			gSetting.Interface[T_PPTP_INT].active = 0;		// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "PPTP" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "active", idxIf, 0);
  #endif // _PPTP

#else

  #ifdef _IPSEC
//			gSetting.Interface[0].ruleGrpIdx[0] = 0;			// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "Loopback" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "active", idxIf, 0);
  #endif // _IPSEC
//			gSetting.Interface[T_WAN_INT].ruleGrpIdx[0] = 0;	// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "wan" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "active", idxIf, 0);
  #ifdef _PPPOE
	#if NEED_MULTIPLE_PPPOE == 1
			{
				int i;

				for (i = 0 ; i < N_PPPoE_NUM ; i++) {
//					gSetting.Interface[i].ruleGrpIdx[0] = 0;
					sprintf(tmpBuf, "ppp%d", i);
					idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", tmpBuf );
					if(idxIf < 0){
						idxIf = mapi_ccfg_idx_find_unused( tid, "FirewallIf", "if", 1 );
						mapi_ccfg_idx_set_str( tid, "FirewallIf", "if", idxIf, tmpBuf);
					}
					mapi_ccfg_idx_set_long( tid, "FirewallIf", "active", idxIf, 0);
				}
			}
	#else
//			gSetting.Interface[T_PPPoE_INT].ruleGrpIdx[0] = 0;	// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "ppp0" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 0);
	#endif // NEED_MULTIPLE_PPPOE == 1
  #endif // _PPPOE
  #if defined(_SSUART) || defined(_SSIO) || (_USB_UMTS != 0)
//			gSetting.Interface[T_COM1_INT].ruleGrpIdx[0] = 0;	// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "com1" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 0);
  #endif // defined(_SSUART) || defined(_SSIO) || (_USB_UMTS != 0)
  #ifdef _WAN_PPTP
//			gSetting.Interface[T_PPTP_INT].ruleGrpIdx[0] = 0;	// interface CBAC function
			idxIf = mapi_ccfg_idx_search_any_by_val( tid, "FirewallIf", "if", "PPTP" );
			mapi_ccfg_idx_set_long( tid, "FirewallIf", "ruleGrpIdxIn", idxIf, 0);
  #endif // _WAN_PPTP

#endif // NEED_NEW_ACL_DEFAULT_VAL == 0
		}
	}

	return setCBACFuncPure(tid, inspect, dos, ping, rip, dmz);
}
//**************************************************************************************************
// FUNCTION:  setCBACFuncSpec
// PURPOSE:   setting generic CBAC function
// PARAMETERS:
//			signed char SpiDos	: inspect & DoS function on/off (TRUE/FALSE) (-1 for no use)
//			signed char ping	: deny ping from WAN on/off (TRUE/FALSE) (-1 for no use)
//			signed char rip		: deny RIP v1 from WAN on/off (TRUE/FALSE) (-1 for no use)
//			signed char dmz		: DMZ function on/off (TRUE/FALSE) (-1 for no use)
// RETURN:
//			1: setting success
//
// Note
//		* related sections: Firewall, FirewallIf, inspectGlobal, DMZ
//
//***************************************************************************************************
char mapi_ccfgsal_firewall_set_CBACFuncSpec(int tid, signed char SpiDos, signed char ping, signed char rip, signed char dmz) {

	return setCBACFunc(tid, SpiDos, SpiDos, ping, rip, dmz);
}

//**************************************************************************************************
// FUNCTION:  updateExtRuleSrcSubnet
// PURPOSE:   update extend rule
// PARAMETERS:
//			long ruleID					: specify index number (0 based, -1 for update all)
//			unsigned char oldSubnet[3]	: old subnet (only need 192.168.2.*)
//			unsigned char newSubnet[3]	: new subnet (only need 192.168.3.*)
// RETURN:
//			0: update success,  1: update fail
//
// Note
//		* related sections: Firewallruls
//
//***************************************************************************************************
char mapi_ccfg_firewall_update_ExtRuleSrcSubnet(int tid, long ruleID, unsigned char *oldSubnet, unsigned char *newSubnet) {
	long i, lo = 0, up = MAX_CBAC_EXTEND_RULE_NUM;
	char strIPs[20];
	unsigned char tmpIPs[4];

	if (ruleID != -1) {
		lo = ruleID;
		up = lo + 1;
	}
//dprintf("updateExtRuleSrcSubnet(%ld)> old:[%u.%u.%u], new:[%u.%u.%u]\n", ruleID,
//		oldSubnet[0], oldSubnet[1], oldSubnet[2], newSubnet[0], newSubnet[1], newSubnet[2]);

	// update gSetting content
	for (i = lo ; i < up ; i++) {
		// bypass empty slot
		//if (gSetting.ruleExt[i].grpIdx == 0) continue;
		if( mapi_ccfg_idx_get_long( tid, "Firewallruls", "grpIdx", i, 0 ) == 0) continue;
		// update content
//		if (memcmp(gSetting.ruleExt[i].srcIP, oldSubnet, 3) == 0)
//			memcpy(gSetting.ruleExt[i].srcIP, newSubnet, 3);
//		if (memcmp(gSetting.ruleExt[i].sSubnet, oldSubnet, 3) == 0)
//			memcpy(gSetting.ruleExt[i].sSubnet, newSubnet, 3);

		mapi_ccfg_idx_get_str( tid, "Firewallruls", "srcIP", i, strIPs, sizeof(strIPs), "0.0.0.0");
		copyListToArray(tmpIPs, UMNGCFG_UINT8_TYPE, strIPs, ',', 4);
		if (memcmp(tmpIPs, oldSubnet, 3) == 0){
			copyArrayToList(strIPs, ',', newSubnet, UMNGCFG_UINT8_TYPE, 3, 0, 3, 10);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "srcIP", i, strIPs);
		}
		mapi_ccfg_idx_get_str( tid, "Firewallruls", "sSubnet", i, strIPs, sizeof(strIPs), "0.0.0.0");
		copyListToArray(tmpIPs, UMNGCFG_UINT8_TYPE, strIPs, ',', 3);
		if (memcmp(tmpIPs, oldSubnet, 3) == 0){
			copyArrayToList(strIPs, ',', newSubnet, UMNGCFG_UINT8_TYPE, 3, 0, 3, 10);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "sSubnet", i, strIPs);
		}
	}

	// on-line active (needn't update group pool run-time variable, because
	// every content is unchange, except source ip.
//	if (!updateExtendRule(-1)) goto ERR_END;

	return 0;

//ERR_END:
	// recover gSetting content
	for (i = lo ; i < up ; i++) {
		// bypass empty slot
//		if (gSetting.ruleExt[i].grpIdx == 0) continue;
		if( mapi_ccfg_idx_get_long( tid, "Firewallruls", "grpIdx", i, 0 ) == 0) continue;

		// update content
//		if (memcmp(gSetting.ruleExt[i].srcIP, newSubnet, 3) == 0)
//			memcpy(gSetting.ruleExt[i].srcIP, oldSubnet, 3);
//		if (memcmp(gSetting.ruleExt[i].sSubnet, newSubnet, 3) == 0)
//			memcpy(gSetting.ruleExt[i].sSubnet, oldSubnet, 3);
		mapi_ccfg_idx_get_str( tid, "Firewallruls", "srcIP", i, strIPs, sizeof(strIPs), "0.0.0.0");
		copyListToArray(tmpIPs, UMNGCFG_UINT8_TYPE, strIPs, ',', 4);
		if (memcmp(tmpIPs, oldSubnet, 3) == 0){
			copyArrayToList(strIPs, ',', newSubnet, UMNGCFG_UINT8_TYPE, 3, 0, 3, 10);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "srcIP", i, strIPs);
		}
		mapi_ccfg_idx_get_str( tid, "Firewallruls", "sSubnet", i, strIPs, sizeof(strIPs), "0.0.0.0");
		copyListToArray(tmpIPs, UMNGCFG_UINT8_TYPE, strIPs, ',', 3);
		if (memcmp(tmpIPs, oldSubnet, 3) == 0){
			copyArrayToList(strIPs, ',', newSubnet, UMNGCFG_UINT8_TYPE, 3, 0, 3, 10);
			mapi_ccfg_idx_set_str( tid, "Firewallruls", "sSubnet", i, strIPs);
		}
	}
	return 1;
}

//**************************************************************************************************
// FUNCTION:  mapi_ccfg_firewall_tr69ConnReqACL
// PURPOSE:   update rules for tr69 connection request ACL
// PARAMETERS:
//			unsigned short ConnReqPort			: specify the port number for connectionr request
// RETURN:
//			0: update success,  1: update fail
//
// Note
//		* related sections: tr69
//
//***************************************************************************************************
char mapi_ccfg_firewall_tr69ConnReqACL(int tid, char* ipStrs[], unsigned short ConnReqPort)
{
	char ipBuf[50], tmpBuf[128];
	char qStr[128];
	int i, isLoacalTid=0;
	char *ipStr;
	char *lockFileName = "/var/lock/iptables.lock";
	int fd;
	if(tid == 0){
		tid = mapi_start_transc();
		isLoacalTid = 1;
	}
	if(utilGetlockByName(lockFileName, &fd, UTIL_FILE_LCK) == ARC_COM_OK){
		osSystem_GetOutput("iptables -F TR69_RULES >/dev/null 2>&1 || iptables -N TR69_RULES", tmpBuf, sizeof(tmpBuf));
		osSystem_GetOutput("iptables -t nat -F TR69_NAT_RULES >/dev/null 2>&1 || iptables -t nat -N SERVICES_NAT_RULES", tmpBuf, sizeof(tmpBuf));
		if(ipStrs == NULL){
			for(i=0 ; 1 ;i++){
				mapi_ccfg_idx_get_str( tid, "tr69", "ACLSrc", i, ipBuf, sizeof(ipBuf), "");
				if(ipBuf[0] != '\0'){
					sprintf(qStr, "iptables -A TR69_RULES -s %s -p tcp --dport %d -j ACCEPT", ipBuf, ConnReqPort);
					osSystem_GetOutput(qStr,tmpBuf, sizeof(tmpBuf));
					sprintf(qStr, "iptables -t nat -A TR69_NAT_RULES -s %s -p tcp --dport %d -j ACCEPT", ipBuf, ConnReqPort);
					osSystem_GetOutput(qStr,tmpBuf, sizeof(tmpBuf));
				}else{
					break;
				}
			}
		}else{
			i = 0;
			ipStr = ipStrs[i];

			while(strlen(ipStr) > 0)
			{
				sprintf(qStr, "iptables -A TR69_RULES -s %s -p tcp --dport %d -j ACCEPT", ipStr, ConnReqPort);
				osSystem_GetOutput(qStr,tmpBuf, sizeof(tmpBuf));
				sprintf(qStr, "iptables -t nat -A TR69_NAT_RULES -s %s -p tcp --dport %d -j ACCEPT", ipStr, ConnReqPort);
				osSystem_GetOutput(qStr,tmpBuf, sizeof(tmpBuf));
				mapi_ccfg_idx_set_str (tid, "tr69", "ACLSrc", i, ipStr);
				i++;
				ipStr = ipStrs[i];
			}
		}
		if(i == 0){
			sprintf(qStr,"iptables -A TR69_RULES -p tcp --dport %d -j ACCEPT", ConnReqPort);
			osSystem_GetOutput(qStr,tmpBuf, sizeof(tmpBuf));
			sprintf(qStr,"iptables -t nat -A TR69_NAT_RULES -p tcp --dport %d -j ACCEPT", ConnReqPort);
			osSystem_GetOutput(qStr,tmpBuf, sizeof(tmpBuf));
		}
		utilGetlockByName(lockFileName, &fd, UTIL_FILE_UNLCK);
	}else{
		printf("LOCK fail:mapi_ccfg_firewall_tr69ConnReqACL\n");
	}
	osSystem_GetOutput("ccfg_cli synccfg TR69", tmpBuf, sizeof(tmpBuf));

	if(isLoacalTid == 1)
		mapi_end_transc(tid);
	return 0;
}
///// extend functions -------------------end


///// mapi adapter interface
