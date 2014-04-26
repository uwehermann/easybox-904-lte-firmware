#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mid_mapi_ccfgsal_util.h"

int indexOfC(char* str, const char delimiter, const int num){
	char *pos;
	int idx = 0;
	int length = 0;
	length = strlen(str);
	if(num == 0)
		return -1;

	for(pos=str;pos;){
		pos = strchr(pos,delimiter);
		if(pos){
			idx++;
			pos = pos+1;
			if(idx == num)
				break;
		}
	}
	if(!pos) {
		return -1;
	}
	return pos-str-1;
}

int getListSize(char *srcList, char delimiter){
	int numItem=0;
	if((!srcList)||(strlen(srcList)==0))
		return numItem;

	for(numItem=1; ;numItem++){
		if(indexOfC(srcList, delimiter, numItem) == -1)
			break;
	}
	return numItem;
}

int getStrInList(char *srcList, char delimiter, int idxItem, char* str, int sizeStr){
	int sp, ep;
	int lenItem=0;
	if((!srcList)||(strlen(srcList) == 0))
		return -1;

	sp = indexOfC(srcList, delimiter, idxItem);
	if((sp == -1)&&(idxItem != 0)){ //if idxItem is too large, assign the first item as idxItem.
		lenItem = 0;
		strcpy(str, "");
	}
	else{
		if(sp == -1) //idxItem = 0
			sp = 0;
		else
			sp++;
		ep = indexOfC(srcList, delimiter, idxItem+1);
		if(ep == -1){ //if idxItem+1 is too large
			ep = strlen(srcList);
		}
		lenItem = ep-sp;
		if(lenItem > sizeStr){
			lenItem = sizeStr;
		}
		strncpy(str, srcList+sp, lenItem);
		str[lenItem] = '\0';
//		printf("--------->[%s]%d srcList:%s sp:%d ep:%d str:%s\n", __FUNCTION__, __LINE__,srcList,sp,ep,str);
	}
	return lenItem;
}
void setStrInList(char *srcList, int size, const char delimiter, const int idxItem, char* str){
	//start from 0
	unsigned int lenList = 0;
	int sp, ep;

	lenList = getListSize(srcList, delimiter);
	if((idxItem==0)&&(lenList == 1)){	//insert into first pos when list szie == 1
		snprintf(srcList,size,"%s", str);
	}
	else if((idxItem>=lenList)||(idxItem < 0)){	//idx is larger or equal list size, replace the last one
//		sp = indexOfC(srcList, ',', lenList-1);
//		srcList[sp] = '\0';
//		sprintf(srcList,"%s%c%s", srcList, delimiter,str);
		printf("--------->[%s]%d out of boundary idxItem:%d lenList:%d str:%s\n", __FUNCTION__, __LINE__,idxItem,lenList,str);
	}
	else{ //insert into mid pos
		sp = indexOfC(srcList, delimiter, idxItem);
		ep = indexOfC(srcList, delimiter, idxItem+1);
		printf("sp:%d ep:%d\n", sp, ep);
		srcList[sp+1] = '\0';
		snprintf(srcList, size, "%s%s%s", srcList, str, (srcList+ep));
	}
}
void setItemInList(char *srcList, int size, char delimiter, int idxItem, int item){
	//start from 0
	char tmpBuf[10];
	snprintf(tmpBuf,sizeof(tmpBuf),"%d",item);
	setStrInList(srcList, size, delimiter, idxItem, tmpBuf);
}
int insertItemInList(char *srcList, int size, char delimiter, int idxItem, int item){
	//start from 0
	unsigned int lenList = 0;
	int sp;
	char *tmpBuf;

	lenList = getListSize(srcList, delimiter);
	if(lenList == 0){	//insert into first pos when list is empty
		snprintf(srcList,size,"%d", item);
	}
	else if((idxItem>=lenList)||(idxItem < 0)){	//idx is larger or equal list size, insert into last
		tmpBuf = (char*)malloc(size);
		snprintf(tmpBuf,size,"%s%c%d", srcList, delimiter,item);
		strcpy(srcList, tmpBuf);
		free(tmpBuf);
	}
	else if(idxItem == 0){	//insert into first pos
		tmpBuf = (char*)malloc(size);
		snprintf(tmpBuf,size,"%d%c%s", item, delimiter,srcList);
		strcpy(srcList, tmpBuf);
		free(tmpBuf);
	}
	else{ //insert into mid pos
		tmpBuf = (char*)malloc(size);
		sp = indexOfC(srcList, delimiter, idxItem);
		srcList[sp] = '\0';
		snprintf(tmpBuf, size, "%s%c%d%c%s", srcList, delimiter, item, delimiter, (srcList+sp+1));
		strcpy(srcList, tmpBuf);
		free(tmpBuf);
	}
	return ++lenList;
}

int delItemInList(char *srcList, char delimiter, int idxItem){
	int sp, ep;
	if((!srcList)||(strlen(srcList) == 0))
		return -1;
	//printf("--------->[%s]%d srcList:%s idxItem:%d\n", __FUNCTION__, __LINE__,srcList,idxItem);
	sp = indexOfC(srcList, delimiter, idxItem);
	if((sp == -1)&&(idxItem != 0)){ //if idxItem is too large, assign the first item as idxItem.
		//printf("--------->[%s]%d\n", __FUNCTION__, __LINE__);
		return -1;
	}
	else{
		if(sp == -1) //idxItem = 0
			sp = 0;
		else
			sp++;
		ep = indexOfC(srcList, delimiter, idxItem+1);
//		printf("--------->[%s]%d sp:%d ep:%d\n", __FUNCTION__, __LINE__, sp, ep);
//		if(ep == -1){ //if idxItem+1 is too large
//			ep = strlen(srcList);
//		}

		if(sp == 0){  //del item is first or only one
			if( ep == -1)
				srcList[sp] = '\0';
			else
				strcpy(srcList, (srcList+ep+1));
//			printf("--------->[%s]%d sp:%d ep:%d srcList:%s\n", __FUNCTION__, __LINE__, sp, ep, srcList);
		}else{ //del item in mid or last
			if( ep == -1)
				srcList[sp-1] = '\0';
			else
				strcpy((srcList+sp), (srcList+ep+1));
//			printf("--------->[%s]%d sp:%d ep:%d srcList:%d\n", __FUNCTION__, __LINE__, sp, ep, srcList);
		}

	}
	return getListSize(srcList, delimiter);;
}

int getItemInList(char *srcList, char delimiter, int idxItem, int retdef){
	int value=-1;

	char item[50];

	if((!srcList)||(strlen(srcList) == 0))
		return retdef;

	getStrInList(srcList, delimiter, idxItem, item, sizeof(item));

	if(strlen(item) == 0)
		value = retdef;
	else
		value = atoi(item);
	return value;
}

int isListMatch(char *listA, char delimiter, void *listB, int listBtype, int num){
	//return 0 matched 1 different
	int i=-1;
	int sizelist;
	sizelist = getListSize(listA, delimiter);
	//printf("--------->[%s]%d sizelist:%d listA:%s\n", __FUNCTION__, __LINE__,sizelist,listA);
	if(sizelist != num)
		return 1;

	switch(listBtype) {
	    case UMNGCFG_INT8_TYPE:
			for(i=0;i<num;i++){
				if(((unsigned short)getItemInList(listA, delimiter, i, 0)) != ((char *)listB)[i]){
					//printf("listA[%d]:%d listB[%d]:%d",i,getItemInList(listA, delimiter, i), i, ((char *)listB)[i]);
					break;
				}
			}
	        break;
	    case UMNGCFG_UINT8_TYPE:
			for(i=0;i<num;i++){
				if(((unsigned short)getItemInList(listA, delimiter, i, 0)) != ((unsigned char *)listB)[i]){
					//printf("listA[%d]:%d listB[%d]:%d",i,getItemInList(listA, delimiter, i), i, ((unsigned char *)listB)[i]);
					break;
				}
			}
	        break;
	    case UMNGCFG_INT16_TYPE:
			for(i=0;i<num;i++){
				if(((unsigned short)getItemInList(listA, delimiter, i, 0)) != ((short *)listB)[i]){
					//printf("listA[%d]:%d listB[%d]:%d",i,getItemInList(listA, delimiter, i), i, ((short *)listB)[i]);
					break;
				}
			}
	        break;
	    case UMNGCFG_UINT16_TYPE:
			for(i=0;i<num;i++){
				if(((unsigned short)getItemInList(listA, delimiter, i, 0)) != ((unsigned short *)listB)[i]){
					//printf("listA[%d]:%d listB[%d]:%d",i,getItemInList(listA, delimiter, i), i, ((unsigned short *)listB)[i]);
					break;
				}
			}
	        break;
	    case UMNGCFG_INT32_TYPE:
			for(i=0;i<num;i++){
				if(((unsigned short)getItemInList(listA, delimiter, i, 0)) != ((int *)listB)[i]){
					//printf("listA[%d]:%d listB[%d]:%d",i,getItemInList(listA, delimiter, i), i, ((int *)listB)[i]);
					break;
				}
			}
	        break;
        case UMNGCFG_UINT32_TYPE:
			for(i=0;i<num;i++){
				if(((unsigned short)getItemInList(listA, delimiter, i, 0)) != ((unsigned int *)listB)[i]){
					//printf("listA[%d]:%d listB[%d]:%d",i,getItemInList(listA, delimiter, i), i, ((unsigned int *)listB)[i]);
					break;
				}
			}
	        break;
        default:
			printf("--------->[%s]%d without this type of data\n", __FUNCTION__, __LINE__);
            break;
    }


	if(i == num)
		return 0;
	return 1;
}

int copyListToArray(void *dstlist, const int dstlisttype, char *srclist, char delimiter, int num){
	int i;
	int sizeList;
	sizeList = getListSize(srclist, delimiter);
	switch(dstlisttype) {
	    case UMNGCFG_INT8_TYPE:
			for(i=0;i<num;i++){
				if(i >= sizeList)
					((char *)dstlist)[i] = 0;
				else
					((char *)dstlist)[i] = getItemInList(srclist, delimiter, i, 0);
			}
	        break;
	    case UMNGCFG_UINT8_TYPE:
			for(i=0;i<num;i++){
				if(i >= sizeList)
					((unsigned char *)dstlist)[i] = 0;
				else
					((unsigned char *)dstlist)[i] = getItemInList(srclist, delimiter, i, 0);
			}
	        break;
	    case UMNGCFG_INT16_TYPE:
			for(i=0;i<num;i++){
				if(i >= sizeList)
					((short *)dstlist)[i] = 0;
				else
					((short *)dstlist)[i] = getItemInList(srclist, delimiter, i, 0);
			}
	        break;
	    case UMNGCFG_UINT16_TYPE:
			for(i=0;i<num;i++){
				if(i >= sizeList)
					((unsigned short *)dstlist)[i] = 0;
				else
					((unsigned short *)dstlist)[i] = getItemInList(srclist, delimiter, i, 0);
			}
	        break;
	    case UMNGCFG_INT32_TYPE:
			for(i=0;i<num;i++){
				if(i >= sizeList)
					((int *)dstlist)[i] = 0;
				else
					((int *)dstlist)[i] = getItemInList(srclist, delimiter, i, 0);
			}
	        break;
        case UMNGCFG_UINT32_TYPE:
			for(i=0;i<num;i++){
				if(i >= sizeList)
					((unsigned int *)dstlist)[i] = 0;
				else
					((unsigned int *)dstlist)[i] = getItemInList(srclist, delimiter, i, 0);
			}
	        break;
        default:
        	printf("--------->[%s]%d without this type of data\n", __FUNCTION__, __LINE__);
            break;
    }
    return 0;
}

int copyArrayToList(char *dstlist, const char delimiter, void * srcA, const int srcAtype, const int sizeA, const int startPos, const int num, int base){
	int i = 0;
	int idx_start=0,idx_end = 0;
	char str_tail[256],printStr[20],firstStr[16];
	if((num <= 0)||(startPos < 0)||!dstlist||(sizeA <= 0))
		return -1;
	if(base == 16){
		strcpy(firstStr,"%02x");
		sprintf(printStr,"%c%s", delimiter, "%02x");
	}
	else{
		strcpy(firstStr,"%d");
		sprintf(printStr,"%c%s", delimiter, "%d");
	}
	//put string before target
	//strcpy(dstlist, "");
//	for(i=0;i<sizeA;i++)
//		printf("%d,",srcA[i]);
//	printf("\n");
	idx_start = indexOfC(dstlist, delimiter, startPos);
	if(idx_start != -1){
		dstlist[idx_start] = '\0';
	}
	//get string after target
	strcpy(str_tail, "");
	idx_end = indexOfC(dstlist, delimiter, startPos+num);
	if(idx_end != -1){
		strncpy(str_tail, (dstlist+idx_end+1), strlen(dstlist)-idx_end-1);
		str_tail[strlen(dstlist)-idx_end-1] = '\0';
	}


	//attach int array
	i = 0;

	switch(srcAtype) {
	    case UMNGCFG_INT8_TYPE:
			if(strlen(dstlist) == 0){
				i = 1;
				sprintf(dstlist,firstStr, ((char *)srcA)[0]);
			}
			for(; i<num; i++){
				if( i >= sizeA) //Source array is not big enough. To use the last value in source to fill in dest.
					sprintf(dstlist+strlen(dstlist),printStr, ((char *)srcA)[sizeA-1]);
				else
					sprintf(dstlist+strlen(dstlist),printStr, ((char *)srcA)[i]);
			}
	        break;
	    case UMNGCFG_UINT8_TYPE:
			if(strlen(dstlist) == 0){
				i = 1;
				sprintf(dstlist,firstStr, ((unsigned char *)srcA)[0]);
			}
			for(; i<num; i++){
				if( i >= sizeA) //Source array is not big enough. To use the last value in source to fill in dest.
					sprintf(dstlist+strlen(dstlist),printStr, ((unsigned char *)srcA)[sizeA-1]);
				else
					sprintf(dstlist+strlen(dstlist),printStr, ((unsigned char *)srcA)[i]);
			}
	        break;
	    case UMNGCFG_INT16_TYPE:
			if(strlen(dstlist) == 0){
				i = 1;
				sprintf(dstlist,firstStr, ((short *)srcA)[0]);
			}
			for(; i<num; i++){
				if( i >= sizeA) //Source array is not big enough. To use the last value in source to fill in dest.
					sprintf(dstlist+strlen(dstlist),printStr, ((short *)srcA)[sizeA-1]);
				else
					sprintf(dstlist+strlen(dstlist),printStr, ((short *)srcA)[i]);
			}
	        break;
	    case UMNGCFG_UINT16_TYPE:
			if(strlen(dstlist) == 0){
				i = 1;
				sprintf(dstlist,firstStr, ((unsigned short *)srcA)[0]);
			}
			for(; i<num; i++){
				if( i >= sizeA) //Source array is not big enough. To use the last value in source to fill in dest.
					sprintf(dstlist+strlen(dstlist),printStr, ((unsigned short *)srcA)[sizeA-1]);
				else
					sprintf(dstlist+strlen(dstlist),printStr, ((unsigned short *)srcA)[i]);
			}
	        break;
	    case UMNGCFG_INT32_TYPE:
			if(strlen(dstlist) == 0){
				i = 1;
				sprintf(dstlist,firstStr, ((int *)srcA)[0]);
			}
			for(; i<num; i++){
				if( i >= sizeA) //Source array is not big enough. To use the last value in source to fill in dest.
					sprintf(dstlist+strlen(dstlist),printStr, ((int *)srcA)[sizeA-1]);
				else
					sprintf(dstlist+strlen(dstlist),printStr, ((int *)srcA)[i]);
			}
	        break;
        case UMNGCFG_UINT32_TYPE:
			if(strlen(dstlist) == 0){
				i = 1;
				sprintf(dstlist,firstStr, ((unsigned long *)srcA)[0]);
			}
			for(; i<num; i++){
				if( i >= sizeA) //Source array is not big enough. To use the last value in source to fill in dest.
					sprintf(dstlist+strlen(dstlist),printStr, ((unsigned int *)srcA)[sizeA-1]);
				else
					sprintf(dstlist+strlen(dstlist),printStr, ((unsigned int *)srcA)[i]);
			}
	        break;
        default:
        	printf("--------->[%s]%d without this type of data\n", __FUNCTION__, __LINE__);
            break;
    }



	//attach tail
	if(strcmp(str_tail, "") != 0)
		sprintf(dstlist+strlen(dstlist), "%c%s", delimiter, str_tail);

	return 0;
}