/*****************************************************************************
	Copyright (C), 2010-2011, BST Electric Co., Ltd.
	File name: BstMdc.c
		Author: zhang zhiwei Version: V0 Date: 20. Feb. 2017
	Description: 
		��˼�ض�ý����ʾ��ͨѶЭ���
	Version: V0
	Function List: 

	History: 
	<author>  <time>  <version >  <desc>
******************************************************************************/
//* Private include files -----------------------------------------------------
//* @{����ͷ�ļ���������(���ޱ��ļ���ʹ��)
#include "BvtPtl.h"
#include "SendFifo.h"
#include "RecvFifo.h"

#define BVT_ESC 0x8A	/* ת���ַ� */
#define BVT_STX 0x80	/* ֡��ʼ�ַ� */
#define BVT_ETX 0x81	/* ֡�����ַ� */
#define BVT_STX_AF 0xE7	/* ֡��ʼת���������ַ� */
#define BVT_ETX_AF 0xE8	/* ֡����ת���������ַ� */
#define BVT_ESC_AF 0x00	/* ת���ַ�ת���������ַ� */

#define BVT_MAX_FRAME_LENTH 44	/* ���֡�ֽڳ��� */
#define BVT_SEND_INTERVAL 50	/* �������ݼ��,��λms */
#define BVT_RECV_CHANNEL_SIZE 1	/* ����ͨ���������ʹ�С */
#define BVT_PTL_FRAMEID_POS 1	/* Э��֡ID��ַ */
#define BVT_PTL_DATASTART_POS 2	/* Э��֡���ݵ�ַ */

//**
// * @}

//* Private Macros and Data type -----------------------------------------------
//* @{˽�к�����ݶ�������(���ޱ��ļ���ʹ�õ�ȫ�ֱ����ͺ�)
#pragma pack(push)	
#pragma pack(1)

static unsigned char gBvtRecStatus = 0;
static unsigned char gBvtRecCnt = 0;
static unsigned char gBvtFrameBuf[BVT_MAX_FRAME_LENTH];

#pragma pack(pop)
//**
// * @}

//* Public function declaration ------------------------------------------------
//* @{˽�к�����������(���ޱ��ļ���ʹ��)

unsigned char BstBvtGetFrameDatLen(e_IDTYPE_T id);
unsigned char BstBvtRecvMonitor(void);
void BstBvtSendMonitor(void);

unsigned long BstBvtRecoverFrame(void *src,unsigned long srclen);
unsigned long BstBvtTransformFrame(void *src,unsigned long srclen,void *dst);
unsigned char BstBvtVerify(unsigned char *data, unsigned long length);


/*!
 * BstBvtPtlInit
 * ��˼�ض�ý����ʾ��ͨѶЭ����ʼ��
 * @param[in]   None
 * @param[out]  None
 * @retval  	None
 */
void BstBvtPtlInit(void)
{
	BstFifoMemSet(gBvtFrameBuf,0,BVT_MAX_FRAME_LENTH);
	SendFifoChannelInit(BST_BVTSEND_USE,BVT_SEND_INTERVAL,BstBvtHwSendData);//����ͨ����ʼ��
	RecvFifoChannelInit(BST_BVTRECV_USE,BVT_RECV_CHANNEL_SIZE);//����ͨ����ʼ��
}
/*!
 * BstBvtCopyFrameData
 * ����֡����
 * @param[in]   id��֡ID	dat��֡���ݵ�ַ
 * @param[out]  None
 * @retval  	None
 */
void BstBvtCopyFrameData(e_IDTYPE_T id,void *dat)
{
	unsigned long FrameLen = 0;
	
	if(id >= ID_UNKNOW)/*δ֪ID*/
		return;
	FrameLen = BstBvtGetFrameDatLen(id);
	BstFifoMemCpy(dat,&gBvtFrameBuf[BVT_PTL_DATASTART_POS],FrameLen);
}
/*!
 * BstBvtSetFrameData
 * ����֡����
 * @param[in]   id��֡ID	dat��֡���ݵ�ַ
 * @param[out]  None
 * @retval  	None
 */
void BstBvtSetFrameData(e_IDTYPE_T id,void *dat)
{
	unsigned char TranBuf[BVT_MAX_FRAME_LENTH] = {0};
	unsigned char FrameBuf[BVT_MAX_FRAME_LENTH] = {0};
	unsigned char *pFrameBuf = FrameBuf;
	unsigned char *pStarVer;
	unsigned char DatLen = 0;
	unsigned long TranLen = 0;/*ת����֡����*/
	
	if(id >= ID_UNKNOW)/*δ֪ID*/
		return;
	DatLen = BstBvtGetFrameDatLen(id);
	*pFrameBuf++ = BVT_STX;	/*����֡ͷ*/
	pStarVer = pFrameBuf;
	*pFrameBuf++ = id;	/*����ID*/
	BstFifoMemCpy(pFrameBuf,dat,DatLen);/*��������*/
	pFrameBuf += DatLen;
	*pFrameBuf++ = BstBvtVerify(pStarVer,DatLen+1);/*����У��ֵ*/
	*pFrameBuf++ = BVT_ETX;/*����֡β*/
	TranLen = BstBvtTransformFrame(FrameBuf,pFrameBuf-FrameBuf,TranBuf);/*֡ת��*/
	SendFifoChannelPush(BST_BVTSEND_USE,TranBuf,TranLen);/*ѹ�뷢��FIFO*/
}
/*!
 * BstBvtPtlMonitor
 * ��˼�ض�ý��ͨѶЭ����
 * @param[in]   None
 * @param[out]  None
 * @retval  	֡ID
 */
e_IDTYPE_T BstBvtPtlMonitor(void)
{		
	unsigned char RecSta;
	
	BstBvtSendMonitor();//���ͼ��
	RecSta = BstBvtRecvMonitor();//���ռ��
	
	if(RecSta == 0)
		return ID_UNKNOW;
	if(gBvtFrameBuf[BVT_PTL_FRAMEID_POS] >= ID_UNKNOW)
		return ID_UNKNOW;
	return (e_IDTYPE_T)gBvtFrameBuf[BVT_PTL_FRAMEID_POS];
}
/*!
 * BstBvtRecvMonitor
 * Э�����ݽ��ռ��
 * @param[in]   None
 * @param[out]  None
 * @retval  	����״̬ 1-������	0-������
 */
unsigned char BstBvtRecvMonitor(void)
{
	unsigned char lDat;
	unsigned long TranFrameLen = 0;	/*ת���󳤶�*/
	
	while(RecvFifoChannelCheck(BST_BVTRECV_USE) != 0)//������FIFO
	{
		BstFifoMemCpy(&lDat,RecvFifoChannelPop(BST_BVTRECV_USE),BVT_RECV_CHANNEL_SIZE);/* ȡ������ */
		if(gBvtRecStatus == 0)/* δ���յ�֡ͷ */
		{
			if(lDat == BVT_STX)/* �յ�֡ͷ */
			{
				gBvtFrameBuf[gBvtRecCnt++] = lDat;
				gBvtRecStatus = 1;
			}else{
				continue;
			}
		}else if(gBvtRecStatus == 1){/* ���ڽ������� */
			if(gBvtRecCnt < BVT_MAX_FRAME_LENTH)
			{
				if(lDat == BVT_ETX)/* �յ�֡β */
				{
					gBvtRecStatus = 2;
				}
				gBvtFrameBuf[gBvtRecCnt++] = lDat;				
			}else{/* ��Ч֡ */
				gBvtRecStatus = 0;
				gBvtRecCnt = 0;
			}
		}
		if(gBvtRecStatus == 2){/* ���������� */
			TranFrameLen = BstBvtRecoverFrame(gBvtFrameBuf,gBvtRecCnt);/* �ָ�֡���� */
			gBvtRecStatus = 0;
			gBvtRecCnt = 0;	
			if(gBvtFrameBuf[TranFrameLen-2] != 
				BstBvtVerify(&gBvtFrameBuf[1],TranFrameLen-3))/* ����У��ʧ�� */
			{
				return 0;
			}else{
				return 1;
			}
		}
	}
	return 0;
}
/*!
 * BstBvtSendMonitor
 * Э�����ݷ��ͼ��
 * @param[in]   None
 * @param[out]  None
 * @retval  	None
 */
void BstBvtSendMonitor(void)
{
	if(SendFifoChannelCheck(BST_BVTSEND_USE) == 0)/* ͨ�������� */
		return;
	SendFifoChannelPop(BST_BVTSEND_USE);/* ����ͨ������*/
}
/*!
 * BstBvtTransformFrame
 * Э��ת��֡����
 * @param[in]   src��ԭ���ݵ�ַ dst��ת�������ݵ�ַ 
				len��ԭ���ݳ���
 * @param[out]  None
 * @retval  	����ת����ĳ���
 */
unsigned long BstBvtTransformFrame(void *src,unsigned long srclen,void *dst)
{
	unsigned long Len = srclen-2;/*ȥ��֡ͷ��֡β*/
	unsigned long TranLen = 2;
	unsigned char Cnt = 0;
	unsigned char *lDst = (unsigned char*)dst;
	unsigned char *lSrc = (unsigned char*)src;
	
	*lDst++ = *lSrc++;/*ȡ��֡ͷ����*/
	while(Cnt < Len)
	{
		if(*lSrc == BVT_STX)/*����֡ͷ����*/
		{
			*lDst++ = BVT_ESC;
			*lDst++ = BVT_STX_AF;
			TranLen += 2;
		}else if(*lSrc == BVT_ETX){/*����֡β����*/
			*lDst++ = BVT_ESC;
			*lDst++ = BVT_ETX_AF;
			TranLen += 2;
		}else if(*lSrc == BVT_ESC){/*����ת���ַ�*/
			*lDst++ = BVT_ESC;
			*lDst++ = BVT_ESC_AF;
			TranLen += 2;
		}else{
			*lDst++ = *lSrc;
			TranLen++;
		}
		lSrc++;
		Cnt++;	
	}	
	*lDst = *lSrc;/*ȡ��֡β����*/
	return TranLen;
}
/*!
 * BstBvtRecoverFrame
 * Э��ȥ���ַ�ת�����ָ�����
 * @param[in]   src��ԭ���ݵ�ַ len��ԭ���ݳ���
 * @param[out]  None
 * @retval  	�ָ����֡����
 */
unsigned long BstBvtRecoverFrame(void *src,unsigned long srclen)
{
	unsigned long Len = srclen-2;/*ȥ��֡ͷ��֡β*/
	unsigned long TranLen = 2;/*�ָ��󳤶�,֡ͷ��֡β*/
	unsigned char Cnt = 0;
	unsigned char *lDst = (unsigned char*)src;
	unsigned char *lSrc = (unsigned char*)src;
	
	*lDst++ = *lSrc++;/*ȡ��֡ͷ����*/
	while(Cnt < Len)
	{
		if(*lSrc == BVT_ESC)
		{
			if(*(lSrc+1) == BVT_STX_AF)
				*lDst++ = BVT_STX;
			else if(*(lSrc+1) == BVT_ETX_AF)
				*lDst++ = BVT_ETX;
			else if(*(lSrc+1) == BVT_ESC_AF)
				*lDst++ = BVT_ESC;
			lSrc += 2;
			Cnt += 2;
		}else{
			*lDst++ = *lSrc++;
			Cnt++;
		}
		TranLen++;
	}
	*lDst = *lSrc;/*ȡ��֡β����*/
	return TranLen;
}
/*!
 * BstBvtVerify
 * Э��У���㷨
 * @param[in]   data����У�������ָ��	length����У������ݳ���
 * @param[out]  None
 * @retval  	None
 */
unsigned char BstBvtVerify(unsigned char *data, unsigned long length)
{
	unsigned char result = 0;
	unsigned int i;
	
	for (i = 0; i < length; i++)
	{
		result = result ^ data[i];
	}	
	return result & 0x7f;
}
/*!
 * BstBvtGetFrameDatLen
 * ��ȡ֡���ݳ���
 * @param[in]   id��֡ID	
 * @param[out]  None
 * @retval  	֡ID��Ӧ�����ݳ���
 */
unsigned char BstBvtGetFrameDatLen(e_IDTYPE_T id)
{
	unsigned char len = 0;
	switch(id)
	{
		case ID00_BASE: len = sizeof(s_BVTID0_T);break;
		case ID01_HEAR: len = sizeof(s_BVTID1_T);break;
		default:
			break;
	}
	return len;
}

