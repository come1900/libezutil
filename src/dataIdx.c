/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * crc.h - src/library/libezutil
 *
 * $Id: crc.h 5884 2008-7-16 11:33:42Z wu_junjie $
 *
 *  Explain:
 *     crc16/crc32
 *
 *  Update:
 *     2008-7-16 11:33:24 WuJunjie 10221 Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <assert.h>

#include "dataIdx.h"

#define ARG_USED(x) (void)&x;

// 数据长度
#define def_MAX_DATA_IDX 32

int existId(dataIdx_t *tContainer, const unsigned short Id)
{
	size_t i = 0;
	for (i = 0; i < def_MAX_DATA_IDX; i++)
	{
		if (Id == (tContainer + i)->id)
		{
			return i;
		}
	}

	return -1;
}

// -1 没有空位了
// -2 不可能
int updateData(dataIdx_t *tContainer, const dataIdx_t *pData)
{
	unsigned short zeroID = def_MAX_DATA_IDX;

	size_t i = 0;
	for (i = 0; i < def_MAX_DATA_IDX; i++)
	{
		// 正常更新
		if (pData->id == (tContainer + i)->id)
		{
			// *(tContainer + i) = *pData;
			(tContainer + i)->id = pData->id;
			(tContainer + i)->tempreture = pData->tempreture;
			(tContainer + i)->lastUpdateMs = pData->lastUpdateMs;

			return i;
		}

		//记录空位
		if (def_MAX_DATA_IDX == zeroID && (tContainer + i)->id == 0)
			zeroID = i;
	}

	// 没有找到
	if (i >= def_MAX_DATA_IDX)
	{
		if (zeroID < def_MAX_DATA_IDX)
		{
			// *(tContainer + i) = *pData;
			(tContainer + zeroID)->id = pData->id;
			(tContainer + zeroID)->tempreture = pData->tempreture;
			(tContainer + zeroID)->lastUpdateMs = pData->lastUpdateMs;

			return zeroID;
		}
		else
		{
			return -1;
		}
	}

	return -2;
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
//  gcc -D_With_Main_TTST -o dataIdx dataIdx.c

#ifdef _With_Main_TTST
dataIdx_t tempreture[def_MAX_DATA_IDX] = {{0}};

int main(int argc, char *argv[])
{
	dataIdx_t ESP6600 = {6600, 3200, 111};
	dataIdx_t ESP6601 = {6601, 3200, 111};
	dataIdx_t ESP6602 = {6602, 3200, 111};
	dataIdx_t ESP6603 = {6603, 3200, 111};
	dataIdx_t ESP6604 = {6604, 3200, 111};
	dataIdx_t ESP6605 = {6605, 3200, 111};
	dataIdx_t ESP6606 = {6606, 3200, 111};

	int updateRet = 0;
	int pos6600 = 0;

	updateRet = updateData(tempreture, &ESP6600);
	assert(updateRet == 0);
	updateRet = updateData(tempreture, &ESP6600);
	assert(updateRet == 0);
	updateRet = updateData(tempreture, &ESP6601);
	assert(updateRet == 1);
	printf("updateRet:%d\n", updateRet);

	pos6600 = existId(tempreture, 6600);
	assert(pos6600 == 0);

	printf("pos6600:%d\n", pos6600);

	return 0;
}
#endif //_With_Main_TTST
