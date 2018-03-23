/*******************************************************************************
	Copyright (C), 2010-2011, BST Electric Co., Ltd.
	File name: BvtPtl.h
	Author: zhang zhiwei Version: V0 Date: 20. Feb. 2017
	Description: 
		��˼�ض�ý����ʾ��ͨѶЭ���
	Others: 
	Function List: 

	History: 
	1.
		Date:
		Author:
		Modification:
	2. 
******************************************************************************/
#ifndef _BVTPTL_H_
#define _BVTPTL_H_

//* Public Macros/Configuration and Data type --------------------------------
//* @{������/���ú����ݶ�������
#pragma pack(push)	
#pragma pack(1)

//*ID = 0��֡����*/
/* ����2�����ݽṹ */
typedef struct _id0_dat2_{
	unsigned char Hund: 7;	/* ��λ��ʾ�ַ�ASCII���ʾ */
	unsigned char Reserved: 1;
}s_ID0DAT2_T;
/* ����3�����ݽṹ */
typedef struct _id0_dat3_{
	unsigned char Tend: 7;	/* ʮλ��ʾ�ַ�ASCII���ʾ */
	unsigned char Reserved: 1;
}s_ID0DAT3_T;
/* ����4�����ݽṹ */
typedef struct _id0_dat4_{
	unsigned char Unit: 7;	/* ��λ��ʾ�ַ�ASCII���ʾ */
	unsigned char Reserved: 1;
}s_ID0DAT4_T;
/* ����5�����ݽṹ */
typedef struct _id0_dat5_{
	unsigned char ArrowUp: 1;	/*�ϼ�ͷ*/
	unsigned char ArrowDn: 1;	/*�¼�ͷ*/
	unsigned char Runing: 1;	/*����*/
	unsigned char Arrived: 1;	/*��վʹ��*/
	unsigned char VoidEn: 1;	/*������վʹ��*/
	unsigned char PowerDown: 1;	/*����ʹ��*/
	unsigned char Reserved: 1;	/*Ԥ��*/
	unsigned char Reserved1: 1;	/*Ԥ��*/
}s_ID0DAT5_T;

typedef struct _bvt_id0_{
	s_ID0DAT2_T Data2;	/* ����2 */
	s_ID0DAT3_T Data3;	/* ����3 */
	s_ID0DAT4_T Data4;	/* ����4 */
	s_ID0DAT5_T Data5;	/* ����5 */
	unsigned char Func[2];/*����6/7/8*/
	unsigned char Language1;
	unsigned char Language2;
	unsigned char Year;
	unsigned char Month;
	unsigned char Day;
	unsigned char Week;
	unsigned char Hour;
	unsigned char Min;
	unsigned char Second;
}s_BVTID0_T;

/*����ID֡����*/
typedef struct _bvt_id1_{
	unsigned char Data1;
	unsigned char Data2;
}s_BVTID1_T;


//ID����ö��
typedef enum _id_type_{
	ID00_BASE = 0,	/* ������ϢID ����¥�㣬��ͷ��������Ϣ*/
	ID01_HEAR,		/* ����ID */
	
	ID_UNKNOW,		/* δ֪ID��Ϣ */
}e_IDTYPE_T;


#pragma pack(pop)
//**
// * @}	

//* Public function declaration -----------------------------------------------
//* @{����������������(�ɹ��ⲿ�ļ�ʹ��)

void BstBvtPtlInit(void);
void BstBvtSetFrameData(e_IDTYPE_T id,void *dat);
void BstBvtCopyFrameData(e_IDTYPE_T id,void *dat);
e_IDTYPE_T BstBvtPtlMonitor(void);
//**
// * @}	

#endif //_BVTPTL_H_ 
