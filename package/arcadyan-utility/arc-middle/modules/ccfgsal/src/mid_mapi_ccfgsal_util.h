#ifndef _mid_mapi_ccfg_util_h_
#define _mid_mapi_ccfg_util_h_

#include "mid_mapi_ccfg.h"

int   indexOfC(char* str, const char delimiter, const int num);
int   getListSize(char *srcList, char delimiter);
int   getStrInList(char *srcList, char delimiter, int idxItem, char* str, int sizeStr);
void  setStrInList(char *srcList, int size, const char delimiter, const int idxItem, char* str);
void  setItemInList(char *srcList, int size, char delimiter, int idxItem, int item);
int   insertItemInList(char *srcList, int size, char delimiter, int idxItem, int item);
int   delItemInList(char *srcList, char delimiter, int idxItem);
int   getItemInList(char *srcList, char delimiter, int idxItem, int retdef);
int   isListMatch(char *listA, char delimiter, void *listB, int listBtype, int num);
int   copyListToArray(void *dstlist, const int dstlisttype, char *srclist, char delimiter, int num);
int   copyArrayToList(char *dstlist, const char delimiter, void * srcA, const int srcAtype, const int sizeA, const int startPos, const int num, int base);

#endif // _mid_mapi_ccfg_util_h_
