////////////////////////////////////////////////Copyright (c)////////////////////////////////////////////////
//                                      ������������Ƭ����չ���޹�˾
//                                            ��    ��    ��
//                                              �𿨲�Ʒ�� 
//
//                                          http://www.zlgmcu.com
//
//--------------�ļ���Ϣ-------------------------------------------------------------------------------------
// �� �� ��:    MUR200Cmd.h
// �ļ�����:    MUR-200�����
// ��    ��:    V1.00 
// �� �� ��:    ����ɭ
// ��������:    2010.03.11
//===========================================================================================================
//-----------------�޸ļ�¼----------------------------------------------------------------------------------
// �޸�����:
// ��ǰ�汾:    
// �� �� ��:
// �޸�����:
//-----------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __MUR200_H
#define __MUR200_H
//===========================================================================================================
#include "StatusCode.h"
#include "SWCode.h"
//===========================================================================================================
#ifdef __cplusplus
	extern "C" {
#endif
//============= �������� ====================================================================================
// PCD����ģʽ
#define ISO14443_TYPEA				0
#define ISO14443_TYPEB				1
#define ISO18092_NFCIP				2			// RC523��֧��

#define ISO14443_3A_REQALL          0x52		// �������еĿ�
#define ISO14443_3A_REQIDL          0x26		// ������еĿ�
#define SELECT_LEVEL_1				0x93		// һ������ײ/ѡ��
#define SELECT_LEVEL_2				0x95		// ��������ײ/ѡ��
#define SELECT_LEVEL_3				0x97		// ��������ײ/ѡ��

#define KEY_TYPE_A            		0x60		// ��ԿA
#define KEY_TYPE_B            		0x61		// ��ԿB

#define KEY_SOURCE_EXT				0x00		// ʹ���ⲿ�������Կ��֤
#define KEY_SOURCE_E2				0x80		// ʹ���ڲ�E2����Կ��֤
		
#define MIFARE_INCREMENT            0xC1		// ֵ��ģʽ
#define MIFARE_DECREMENT            0xC0		// ֵ��ģʽ
#define MIFARE_RESTORE              0xC2		// ����ģʽ

#define SL3_INCREMENT				0xB7		// ֵ��ģʽ
#define SL3_DECREMENT				0xB9		// ֵ��ģʽ
#define SL3_RESTORE					0xC3		// ����ģʽ
#define SL3_TRANSFER				0xB5		// ����ģʽ

#define SL3_PLAIN_READ				0x33		// ���Ķ�ģʽ
#define SL3_CIPHER_READ				0x31		// ���Ķ�ģʽ
#define SL3_PLAIN_WRITE				0xA3		// ����дģʽ
#define SL3_CIPHER_WRITE			0xA1		// ����дģʽ

// TypeB��
#define	ISO14443_3B_REQIDL			0x00		// ���еĿ�
#define	ISO14443_3B_REQALL			0x08		// ���еĿ�
//============= APDU���������Ͷ��� ==========================================================================
#define	APDU_STATE1 				1			// APDU�������1:	CLA	INS	P1	P2
#define	APDU_STATE2 				2			// APDU�������2:	CLA	INS	P1	P2			Le
#define	APDU_STATE3 				3			// APDU�������3:	CLA	INS	P1	P2	Lc Data
#define	APDU_STATE4 				4			// APDU�������4:	CLA	INS	P1	P2	Lc Data Le
//============= ȫ�ֱ���/�������� ===========================================================================

typedef struct 
{
	unsigned char ATQ[2];						// ISO14443A�������Ӧ
	unsigned char UIDLen;						// ISO14443A��UID���ֽ���
	unsigned char UID[10];						// ISO14443A��UID
	unsigned char SAK;							// ISO14443A��ѡ��Ӧ��
} ACTIVEPARAA;

typedef struct 
{
	unsigned char PUPI[4];						// αΨһPICC��ʶ��
	unsigned char AppDat[4];					// Ӧ������
	unsigned char ProtInf[3];					// ������Ϣ
} ACTIVEPARAB;	

typedef struct
{
	unsigned char Case;							// APDU����������
	unsigned char CLA;
	unsigned char INS;
	unsigned char P1;
	unsigned char P2;
	unsigned char Lc;
	unsigned char *pSBuf;
	unsigned char Le;
}C_APDU;										// APDU ����

typedef struct
{
	unsigned short SW;
	unsigned int RcvLen;						// ���յ��ֽ�����������SW
	unsigned char *pRBuf;						// ��Ӧ�����ݣ�������SW
}R_APDU;										// APDU ��Ӧ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:	int __stdcall MUR200Init(void)
// ��������:	MUR200��ʼ��
// ��ڲ���:	-					
// ���ڲ���:	-
// �� �� ֵ:	��ǰ����MUR-200�ĸ���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int __stdcall MUR200Init(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:	unsigned char MUR200Open(unsigned char ucDvcIndex)
// ��������:	��MUR-200
// ��ڲ���:	unsigned char ucDvcIndex		// �豸������					
// ���ڲ���:	-
// �� �� ֵ:	STATUS_COMM_OK -- �򿪳ɹ���	����ֵ -- ʧ��
// ˵    ��:	���ж��MUR-200�豸����ucDvcIndex�ֱ�Ϊ0��1��2......
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MUR200Open(unsigned char ucDvcIndex);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:	unsigned char MUR200Reset(void)
// ��������:	MUR-200��λ
// ��ڲ���:	-				
// ���ڲ���:	-
// �� �� ֵ:	STATUS_COMM_OK -- �򿪳ɹ���	����ֵ -- ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MUR200Reset(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:	int __stdcall MUR200Exit(void)
// ��������:	MUR-200�ر��˳�
// ��ڲ���:	-				
// ���ڲ���:	-
// �� �� ֵ:	STATUS_COMM_OK -- �򿪳ɹ���	����ֵ -- ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char  __stdcall MUR200Exit(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:	unsigned char GetDvcInfo(unsigned int *pInfoLen, unsigned char *pInfo)
// ��������:	��ȡ�豸��Ϣ
// ��ڲ���:	unsigned int nRBufSize			// ���ջ������ĳ���				
// ���ڲ���:	unsigned int *pInfoLen			// �豸��Ϣ����
//				unsigned char *pInfo			// �豸��Ϣ
// �� �� ֵ:	STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall GetDvcInfo(unsigned int nRBufSize, unsigned int *pInfoLen, unsigned char *pInfo);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:	unsigned char PcdConfig(unsigned char ucType)
// ��������:	���ö�д����PCD
// ��ڲ���:	unsigned char ucType			// TYPEA -- ISO14443A,TYPEB -- ISO14443B
// ���ڲ���:	-
// �� �� ֵ:	STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PcdConfig(unsigned char ucType);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:	unsigned char PcdClose(void)
// ��������:	�رն�д����PCD
// ��ڲ���:	--
// ���ڲ���:	-
// �� �� ֵ:	STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PcdClose(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PcdLoadKey(unsigned char ucKeyType, unsigned char ucKeySector,
//										 unsigned char ucKeyLen, const unsigned char *pKey)
// ��������:    װ����Կ
// ��ڲ���:    unsigned char ucKeyType			// ��Կ���� KEY_A -- ��ԿA;KEY_B -- ��ԿB;
//              unsigned char ucKeySector		// ��Կ�������
//				unsigned char ucKeyLen			// ��Կ���ֽ���(6/16�ֽ�)
//				unsigned char *pKey				// д�����Կ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
// ˵    ��:	�ú����ǶԶ�д�����������ǶԿ�Ƭִ����֤��Կ������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PcdLoadKey(	unsigned char ucKeyType, unsigned char ucKeySector,
									unsigned char ucKeyLen, const unsigned char *pKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PcdWriteReg(unsigned char ucReg, unsigned char ucValue)
// ��������:    д�Ĵ���
// ��ڲ���:    unsigned char Reg               // �Ĵ�����ַ
//              unsigned char Value             // д�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PcdWriteReg(unsigned char ucReg, unsigned char ucValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PcdReadReg(unsigned char ucReg, unsigned char *pValue)
// ��������:    д�Ĵ���
// ��ڲ���:    unsigned char ucReg				// �Ĵ�����ַ
// ���ڲ���:    unsigned char *pValue			// ����������
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PcdReadReg(unsigned char ucReg, unsigned char *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PcdISOType(unsigned char ucTXMode)
// ��������:    ����PCD��Э��ģʽ
// ��ڲ���:    unsigned char ucType			// TYPEA -- ISO14443A,TYPEB -- ISO14443B
// ���ڲ���:	-
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PcdISOType(unsigned char ucType);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SetIO(unsigned char ucContrl, unsigned char ucOpenTm,
//									unsigned char ucCloseTm, unsigned char ucRepCnt)
// ��������:    ��ucContrl����1��λ
// ��ڲ���:    unsigned char ucContrl			// Contrl^0 ������,	Contrl^1 ���,
//												// Contrl^2 �̵�,   Contrl^3 ����,	
//				unsigned char ucOpenTm;			// ��ʱ�䣬ȡֵ0-255�� 10ms �ķֱ���(0--һֱ��)
//				unsigned char ucCloseTm;		// �ر�ʱ�䣬ȡֵ0-255�� 10ms �ķֱ���
//				unsigned char ucRepCnt;			// �ظ�����
// ���ڲ���:	-
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SetIO(  unsigned char ucContrl, unsigned char ucOpenTm,
								unsigned char ucCloseTm, unsigned char ucRepCnt);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ClearIO( unsigned char ucContrl)
// ��������:    �ر�ucContrl����1��λ
// ��ڲ���:    unsigned char ucContrl			// Contrl^0 ������,	Contrl^1 ���,
//												// Contrl^2 �̵�,   Contrl^3 ����,	
// ���ڲ���:	-
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//unsigned char __stdcall ClearIO(unsigned char ucContrl);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ReadIO(unsigned char *pIoStatus, 
//									 unsigned char *pContrl, unsigned char *pOpenTm,
//									 unsigned char *pCloseTm, unsigned char *pRepCnt)
// ��������:    ��ucContrl����1��λ
// ��ڲ���:    -
// ���ڲ���:	unsigned char *pIoStatus		// IO�ڵ�ǰ״̬
//				unsigned char *pContrl			// IO�ϴ����õ�ֵ
//				unsigned char *pOpenTm;			// IO�ϴ����õ�ֵ
//				unsigned char *pCloseTm;		// IO�ϴ����õ�ֵ
//				unsigned char *pRepCnt;			// IO�ϴ����õ�ֵ
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ReadIO( unsigned char *pIoStatus, 
							    unsigned char *pContrl, unsigned char *pOpenTm,
								unsigned char *pCloseTm, unsigned char *pRepCnt);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:	void PiccReset(INT8U ucPause_1ms,INT8U ucWait_1ms)
// ��������:	ʹRF������1����ͣ���ÿ���λ
// ��ڲ���:	INT8U ucPause_1ms				// ��ͣʱ�䣬�ر�RF����ʱ������´򿪣�
//												// ��Ϊ0�����´�
//				INT8U ucWait_1ms				// RF�����´򿪺�ȴ�ʱ�䣬��Ϊ0�򲻵ȴ�
// ���ڲ���:	-
// �� �� ֵ:	-
// ˵    ��:	�ȴ�ʱ��ɸ��ݿ��Ĺ��Ķ�������Mifare1���Ĺ��Ľ�С���ȴ������뼴�ɣ�
//				��CPU�����Ľϴ���Ҫ�ȴ�80�������ҡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall  PiccReset(unsigned char ucPause_1ms, unsigned char ucWait_1ms);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char DvcWriteUserKey(const unsigned char *pKey)
// ��������:    д���û���֤����������Կ
// ��ڲ���:    const unsigned char *pKey		// �û���Կ
// ���ڲ���:	-
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall DvcWriteUserKey(const unsigned char *pKey);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char DvcAuthUserKey(unsigned char ucMode, const unsigned char *pKey)
// ��������:    �û���֤������
// ��ڲ���:    unsigned char ucMode			// ��֤ģʽ	0x01--����ģʽ��	0x02--����ģʽ
//				const unsigned char *pKey		// �û���Կ
// ���ڲ���:	-
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall DvcAuthUserKey(unsigned char ucMode, const unsigned char *pKey);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char DvcWriteUserEEPROM(unsigned int uiOffset, unsigned int nBytes, 
//												 const unsigned char *pBuf)
// ��������:    д�û�E2PROM
// ��ڲ���:    unsigned int uiOffset			// ƫ����
//				unsigned int nBytes				// д����ֽ���
//				unsigned char *pBuf				// д�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	��д���������������Զ���ת
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall DvcWriteUserE2PROM(unsigned int uiOffset, unsigned int nBytes, 
										   const unsigned char *pBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char DvcReadUserE2PROM(unsigned int uiOffset, unsigned int nBytes, 
//												unsigned char *pBuf)
// ��������:    ���û�E2PROM
// ��ڲ���:    unsigned int uiOffset			// ƫ����
//				unsigned int nBytes				// �������ֽ���
//				unsigned char *pBuf				// ����������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	��д���������������Զ���ת
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall DvcReadUserE2PROM(unsigned int uiOffset, unsigned int nBytes, unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ProdRecord(unsigned int uiSLen, const unsigned char *pSBuf,   
//										 unsigned int nRBufSize, unsigned short *pRLen, unsigned char *pRBuf) 
// ��������:    ����/���Լ�¼
// ��ڲ���:    unsigned int uiSLen				// ������Ϣ�ĳ���
//				unsigned char *pSBuf			// ��Ϣ
//				unsigned int nRBufSize			// ���ջ������ĳ���
// ���ڲ���:	unsigned int *pRLen				// ������Ϣ���ֽ�
//				unsigned char *pRBuf			// ���յ���Ϣ
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ProdRecord(unsigned int ucSLen, const unsigned char *pSBuf, 
								   unsigned int nRBufSize, unsigned int *pRLen, unsigned char *pRBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ProdRecord(unsigned int uiSLen, const unsigned char *pSBuf,   
//										 unsigned int nRBufSize, unsigned short *pRLen, unsigned char *pRBuf) 
// ��������:    ����/���Լ�¼
// ��ڲ���:    unsigned int uiSLen				// ������Ϣ�ĳ���
//				unsigned char *pSBuf			// ��Ϣ
//				unsigned int nRBufSize			// ���ջ������ĳ���
// ���ڲ���:	unsigned int *pRLen				// ������Ϣ���ֽ�
//				unsigned char *pRBuf			// ���յ���Ϣ
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ProdRecord(unsigned int ucSLen, const unsigned char *pSBuf, 
								   unsigned int nRBufSize, unsigned int *pRLen, unsigned char *pRBuf);


//===========================================================================================================
//CMD_TYPE_MIFARE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MFActivate( unsigned char ucMode, unsigned char ucReqCode,  
//											ACTIVEPARAA *pAInfo)
// ��������:   	A�Ϳ�����
// ��ڲ���:    unsigned char ucMode			// ����ײģʽ
//				unsigned char ucReqCode			// �������
// ���ڲ���:    ACTIVEPARAA *pAInfo				// ISO14443 A��������Ϣ
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�  
// ˵    ��:	�ú����Զ�ʶ��M1 ��PLUS CPU����SL1��SL2��SL3
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MFActivate( unsigned char ucMode, unsigned char ucReqCode, ACTIVEPARAA *pAInfo);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MFDeSelect(void)
// ��������:    ������Ŀ�����
// �������:    -		
// �������:    -
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MFDeSelect(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    void ValueToData(unsigned char *pBuf, unsigned char usBNr, long lValue)
// ��������:    ��ֵת��Ϊ���ݿ�ģʽ
// �������:    unsigned char usBNr				// ֵ��ŵ�ַ
//				long iValue						// ֵ
// �������:    unsigned char *pBuf				// 16�ֽ�ֵ���ʽ����
// �� �� ֵ:    -
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __stdcall ValueToData(unsigned char *pBuf, unsigned char ucBNr, long lValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char DataToValue(unsigned char *pBuf, long *pValue)
// ��������:    �����ݿ�ת��Ϊֵ
// �������:    unsigned char *pBuf				// 16�ֽ�ֵ���ʽ����	
// �������:    long *pValue					// ֵ()
// �� �� ֵ:    -
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall DataToValue(unsigned char *pBuf, long *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MFHaltA(void)
// ��������:   	������ΪHALT״̬
// ��ڲ���:    -
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MFHalt(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MFAuth(unsigned char ucAuthMode, unsigned char ucBlock, 
//									 const unsigned char *pKey)
// ��������:   	Mifare����Կ��֤
// ��ڲ���:    unsigned char ucAuthMode		// ��֤ģʽ 
//													KEY_TYPE_A | KEY_SOURCE_EXT	�ⲿ�������ԿA��֤
//													KEY_TYPE_A | KEY_SOURCE_E2	�ڲ�E2����ԿA��֤
//													KEY_TYPE_B | KEY_SOURCE_EXT	�ⲿ�������ԿB��֤
//													KEY_TYPE_B | KEY_SOURCE_E2	�ڲ�E2����ԿB��֤
//				unsigned char ucBlock			// ��֤�Ŀ��
//				unsigned char *pKey				// ʹ��KEY_SOURCE_E2ģʽʱ��pKey[0]Ϊ��Կ��ŵ�����,������Ч
//												// ʹ��KEY_SOURCE_EXTģʽʱ��pKeyΪ16�ֽ���Կ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	�ú����Զ�ʶ��M1 ��PLUS CPU����SL1��SL2��SL3
//				����ֻ��6�ֽ���Կ�����(��M1 S50/70��)���ȡ�����16�ֽ���Կ��ǰ6�ֽ���Ϊ��Կ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MFAuth(unsigned char ucAuthMode, unsigned char ucBlock, const unsigned char *pKey);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MFRead(unsigned char ucBNr, unsigned char ucExt, unsigned char *pBuf)
// ��������:   	MF������
// ��ڲ���:    unsigned char ucBNr				// ������ʼ���
//				unsigned char ucExt				// ���Ŀ���(ȡֵ��Χ1��3)
// ���ڲ���:    unsigned char *pBuf				// ����������
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	�ú����Զ�ʶ��M1 ��PLUS CPU����SL1��SL2��SL3
//				����MF1��PLUS SL1�Ŀ������б������Ŀ���Ҫ��ͬһ�����ڣ���������
//				����PLUS SL2/SL3�Ŀ��������������Ŀ����ͬһ������(��Ϊ���Զ��������ÿ�)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MFRead(unsigned char ucBNr, unsigned char ucExt, unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MFWrite(unsigned char ucBNr, unsigned char ucExt, const unsigned char *pBuf)
// ��������:   	MF�����д
// ��ڲ���:    unsigned char ucBNr				// д����ʼ���
//				unsigned char ucExt				// д�Ŀ���(ȡֵ��Χ1��3)
// ���ڲ���:    unsigned char *pBuf				// д�������
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	�ú����Զ�ʶ��M1 ��PLUS CPU����SL1��SL2��SL3
//				����MF1��PLUS SL1�Ŀ������б������Ŀ���Ҫ��ͬһ�����ڣ���������
//				����PLUS SL2/SL3�Ŀ��������������Ŀ����ͬһ������(��Ϊ���Զ��������ÿ�)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MFWrite(unsigned char ucBNr, unsigned char ucExt, const unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MFReadValue(unsigned char ucBNr, long *pValue)
// ��������:   	MF����ȡֵ��
// ��ڲ���:    unsigned char ucBNr				// ���Ŀ��
// ���ڲ���:    long *pValue					// ������ֵ
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	�ú����Զ�ʶ��M1 ��PLUS CPU����SL1��SL2��SL3
//				PLUS S/X SL2 �� PLUS S SL3��֧��ֵ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MFReadValue(unsigned char ucBNr, long *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MFWriteValue(unsigned char ucBNr, long lValue)
// ��������:   	MF��д��ֵ
// ��ڲ���:    unsigned char ucBNr				// д�Ŀ��
//				long lValue						// д���ֵ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	�ú����Զ�ʶ��M1 ��PLUS CPU����SL1��SL2��SL3
//				PLUS S/X SL2 �� PLUS S SL3��֧��ֵ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MFWriteValue(unsigned char ucBNr, long lValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MFValueOper(unsigned char ucOperMode, unsigned char ucSBlock, 
//										  unsigned char ucDBlock, long lValue)
// ��������:    Mifareֵ��������(PLUS CPU����SL2��ֵ����)
// ��ڲ���:    unsigned char ucOperMode		// ֵ����ģʽ
//													MIFARE_INCREMENT ��Դ���ֵ����ָ����ֵ��ŵ�Ŀ�Ŀ�
//													MIFARE_DECREMENT ��Դ���ֵ��ȥָ����ֵ��ŵ�Ŀ�Ŀ�
//													MIFARE_RESTORE   ��Դ���ֵ���Ƶ�Ŀ�Ŀ�
//				unsigned char ucSBlock			// ������Դ���ַ��
//				unsigned char ucDBlock			// ֵ������Ŀ�ؿ�
//				long lValue						// �Ǹ����з�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	�ú����Զ�ʶ��M1 ��PLUS CPU����SL1��SL2��SL3
//				MIFARE_RESTORE ����ģʽ�ǽ�ָ����ֵ�鸴�Ƶ�Ŀ�Ŀ飬һ������ֵ������ı��ݴ���(��ģʽ�£�
//				����ֵ��Ч��Ϊ����ֵ����)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall  MFValueOper(unsigned char ucOperMode, unsigned char ucSBlock, 
									 unsigned char ucDBlock, long lValue);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MF0ULWrite(unsigned char ucBlock, const unsigned char *pBuf)
// ��������:    MF0 ULд����
// ��ڲ���:    unsigned char ucBlock			// д��Ŀ��ַ��
//				unsigned char *pBuf				// 4�ֽ�д���ݻ�������ַ��
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ� 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MF0ULWrite(unsigned char ucBlock, const unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MF1Auth(unsigned char ucAuthMode, unsigned char ucBlock, unsigned char *pKey)
// ��������:    MF1 S50/70����֤��Կ
// ��ڲ���:    unsigned char ucAuthMode		// ��֤ģʽ 
//													KEY_TYPE_A | KEY_SOURCE_EXT	�ⲿ�������ԿA��֤
//													KEY_TYPE_A | KEY_SOURCE_E2	�ڲ�E2����ԿA��֤
//													KEY_TYPE_B | KEY_SOURCE_EXT	�ⲿ�������ԿB��֤
//													KEY_TYPE_B | KEY_SOURCE_E2	�ڲ�E2����ԿB��֤
//				unsigned char ucBlock			// ��֤�Ŀ��	MF1 S50����0��63
//												//				MF1 S70����0��255
//				unsigned char *pKey				// ʹ��KEY_SOURCE_E2ģʽʱ��pKey[0]Ϊ��Կ��ŵ�����,������Ч
//												// ʹ��KEY_SOURCE_EXTģʽʱ��pKeyΪ6�ֽ���Կ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MF1Auth(unsigned char ucAuthMode, unsigned char ucBlock, const unsigned char *pKey);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MF1Read(unsigned char ucBNr, unsigned char *pBuf)
// ��������:   	MF1 S50/70�����ݿ�
// ��ڲ���:    unsigned char ucBNr				// ������ʼ���
// ���ڲ���:    unsigned char *pBuf				// ����������
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MF1Read(unsigned char ucBNr, unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MF1Write(unsigned char ucBNr, const unsigned char *pBuf)
// ��������:   	MF1 S50/S70д���ݿ�
// ��ڲ���:    unsigned char ucBNr				// д����ʼ���
//				unsigned char *pBuf				// д�������
// ���ڲ���:	-
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MF1Write(unsigned char ucBNr, const unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MF1ReadValue(unsigned char ucBNr, long *pValue)
// ��������:   	MF1 S50/S70����ȡֵ��
// ��ڲ���:    unsigned char ucBNr				// ���Ŀ��
// ���ڲ���:    long *pValue					// ������ֵ
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MF1ReadValue(unsigned char ucBNr, long *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MF1WriteValue(unsigned char ucBNr, long lValue)
// ��������:   	MF1 S50/S70��д��ֵ
// ��ڲ���:    unsigned char ucBNr				// д�Ŀ��
//				long lValue					// д����ֵ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MF1WriteValue(unsigned char ucBNr, long lValue);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char MF1ValueOper(unsigned char ucOperMode, unsigned char ucSBlock, 
//										   unsigned char ucDBlock, long lValue)
// ��������:    MF1 S50/S70ֵ����
// ��ڲ���:    unsigned char ucOperMode		// ֵ����ģʽ
//													MIFARE_INCREMENT ��Դ���ֵ����ָ����ֵ��ŵ�Ŀ�Ŀ�
//													MIFARE_DECREMENT ��Դ���ֵ��ȥָ����ֵ��ŵ�Ŀ�Ŀ�
//													MIFARE_RESTORE   ��Դ���ֵ���Ƶ�Ŀ�Ŀ�
//				unsigned char ucSBlock			// ������Դ���ַ��
//				unsigned char ucDBlock			// ֵ������Ŀ�ؿ�
//				long lValue						// �Ǹ����з�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ģʽΪMIFARE_RESTOREʱ��������lValue����Ч��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall  MF1ValueOper(unsigned char ucOperMode, unsigned char ucSBlock, 
									  unsigned char ucDBlock, long lValue);

//===========================================================================================================
//CMD_TYPE_ISO7816
//===========================================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SelCurCard(unsigned char ucCardNo)
// ��������:    ѡ��ǰ������SAM����
// ��ڲ���:    unsigned char ucCardNo			// SAM������(1��8)�������ڸ÷�Χ���򱣳�ԭ��
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SelCurCard(unsigned char ucCardNo);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char GetCurCard(unsigned char *pCardNo)
// ��������:    ��ȡ��ǰ������SAM��������
// ��ڲ���:    -				
// ���ڲ���:    unsigned char *pCardNo			// SAM������(1��8)
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall GetCurCard(unsigned char *pCardNo);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char __stdcall SAMClose(unsigned char ucCardNo)
// ��������:    �ر�SAM��
// ��ڲ���:    unsigned char ucCardNo			// SAM������(1��8)�������ڸ÷�Χ�ڣ���Ϊ�ر����е�SAM
// ���ڲ���:	
// �� �� ֵ:    STATUS_SUCCESS -- �����ɹ�������ֵ -- ����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SAMClose(unsigned char ucCardNo);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char CiccReset(unsigned char ucCardNo, unsigned char ucRstFD, unsigned int nRBufSize,
//										unsigned int *pATRLen, unsigned char *pATRBuf)
// ��������:    ��Ƭ��λ(���ǵ�һ��ִ�����临λ��֮��ִ�������ȸ�λ)
// ��ڲ���:    unsigned char ucCardNo			// �����ţ�1��8
//				unsigned char ucRstFD			// ��λʱ�Ĳ�����	0x11--9600bps
//																	0x13--38400bps
//				unsigned int nRBufSize			// ���ջ������ĳ���
// ���ڲ���:    unsigned int *pATRLen			// ��λ��Ϣ���ֽ���
//				unsigned char *pATRBuf			// ��λ��Ϣ
// �� �� ֵ:    -
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall CiccReset(unsigned char ucCardNo, unsigned char ucRstFD, unsigned int nRBufSize,
								  unsigned int *pATRLen, unsigned char *pATRBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PPS(unsigned char *pPPS)
// ��������:    Э��Ͳ���ѡ��
// ��ڲ���:    unsigned char *pPPS				// Э�����
//				PPS[0]							// ָ���Ƿ����PPS1��PPS2��PPS3
//				PPS[1]							// F/D
//				PPS[2]							// N
//				PPS[3]							// ����
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�;	���� -- ʧ�ܡ�
// ע    ��:	PPS[0].4	0--PPS1�����ڣ�	1--PPS1����
// 				PPS[0].5	0--PPS2�����ڣ�	1--PPS2����
// 				PPS[0].6	0--PPS3�����ڣ�	1--PPS3����
//				��PPS[0]��ָ�������ڵĲ������ò�������Ҫ����(����ֵ����)��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall CiccPPS(unsigned char *pPPS);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char CiccTP0(unsigned int nSLen, const unsigned char *pSBuf, unsigned int nRBufSize
//									  unsigned int *pRLen, unsigned char *pRBuf)
// ��������:    �Ӵ�ʽIC���Ĵ���Э��T = 0
// ��ڲ���:    unsigned int nSLen				// ���͵��ֽ���	
//				unsigned char *pSBuf			// �������ݻ�����
//				unsigned int nRBufSize			// �û����ջ���������		
// ���ڲ���:	unsigned int *pRLen				// �������ݵĳ���
//				unsigned char *pRBuf			// �������ݻ�����
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�;	���� -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall CiccTP0(unsigned int nSLen, const unsigned char *pSBuf, unsigned int nRBufSize, 
								unsigned int *pRLen, unsigned char *pRBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char CiccTP1(unsigned int nSLen, const unsigned char *pSBuf, unsigned int nRBufSize, 
//									  unsigned int *pRLen, unsigned char *pRBuf)
// ��������:    �Ӵ�ʽIC���Ĵ���Э��T = 1(����������S��)
// ��ڲ���:    unsigned int nSLen				// ���͵��ֽ���	
//				unsigned char *pSBuf			// �������ݻ�����
//				unsigned int nRBufSize			// �û����ջ���������		
// ���ڲ���:	unsigned int *pRLen				// �������ݵĳ���
//				unsigned char *pRBuf			// �������ݻ�����
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�;	���� -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall CiccTP1(unsigned int nSLen, const unsigned char *pSBuf, unsigned int nRBufSize, 
								unsigned int *pRLen, unsigned char *pRBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char CiccSearch(unsigned int nRBufSize, unsigned int *pRLen, unsigned char *pRBuf)
// ��������:    ���������еĿ�
// ��ڲ���:    unsigned int nRBufSize			// ���ջ�������С		
// ���ڲ���:	unsigned int *pRLen				// �������ݵĳ���
//				unsigned char *pRBuf			// �������ݻ�����
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�;	���� -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall CiccSearch(unsigned int nRBufSize, unsigned int *pRLen, unsigned char *pRBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned short __stdcall CiccTPDU(const C_APDU *pAPDUCmd, R_APDU *pAPDURes)
// ��������:    T = 0 ��T = 1Э���APDU����
// ��ڲ���:    C_APDU *pAPDUCmd				// �����APDU�ṹ��
//				unsigned int nRBufSize			// ���ջ������Ĵ�С
// ���ڲ���:    R_APDU *pAPDURes				// ��Ӧ��APDU�ṹ��
// �� �� ֵ:    ����ִ��״̬��SW(����ִ�гɹ�����SW, ���򷵻�����ִ�������Ľ��)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned short __stdcall CiccTPDU(const C_APDU *pAPDUCmd, unsigned int nRBufSize, R_APDU *pAPDURes);

//===========================================================================================================
// CMD_TYPE_COMM_PROT	
//===========================================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccRequest(unsigned char ucReqCode,unsigned char *pATQ)
// ��������:    A�Ϳ���������
// ��ڲ���:    unsigned char ucReqCode			// �������	ISO14443_3A_REQIDL	0x26	IDLE
//												// 			ISO14443_3A_REQALL	0x52	ALL
// ���ڲ���:    unsigned char *pATQ				// �����Ӧ��,2�ֽ�
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ� 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccRequest(unsigned char ucReqCode, unsigned char *pATQ);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccAnticoll( unsigned char unMode, unsigned char ucSelCode,  
//											unsigned char ucBitCnt, unsigned char *pUID)
// ��������:    λ��ʽ����ײ
// ��ڲ���:    unsigned char unMode			// 1 -- ִ�з���ײѭ����0 -- ִֻ��һ�η���ײ
//				unsigned char ucSelCode			// ����ײ����	SELECT_CASCADE_LEVEL_1����1��
//												// 				SELECT_CASCADE_LEVEL_2����2��
//												// 				SELECT_CASCADE_LEVEL_3����3��
//				unsigned char ucBitCnt			// ��֪���ŵ�λ��
// ���ڲ���:    unsigned char *pUID				// �������кŻ�������ַ��4�ֽڡ�
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	unMode = 0 ʱ��ִֻ��һ�η���ײ����ģʽ����ͬһʱ�̣�ֻ����һ�ſ������߸�Ӧ����    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccAnticoll(unsigned char unMode, unsigned char ucSelCode, 
							   		 unsigned char ucBitCnt, unsigned char *pUID);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccSelect(unsigned char ucSelCode, unsigned char *pUID, unsigned char *pSAK)
// ��������:    ѡ��
// ��ڲ���:    unsigned char ucSelCode			// ����ײ����	SELECT_LEVEL_1����1��
//												// 				SELECT_LEVEL_2����2��
//												// 				SELECT_LEVEL_3����3��
//				unsigned char *pUID				// 4�ֽ����кš�
// ���ڲ���:    unsigned char *pSAK				// �������кŻ�������ַ��4�ֽڡ�
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccSelect(unsigned char ucSelCode, unsigned char *pUID, unsigned char *pSAK);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccHaltA(void)
// ��������:   	������ΪHALT״̬
// ��ڲ���:    -
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccHaltA(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccRequestATS(unsigned char ucCID,unsigned char *pATS)
// ��������:    ����ѡ����Ӧ��
// �������:    unsigned char ucCID				// ��Ѱַ��PICC�߼���� 0x00~0x0E		
// �������:    unsigned char *pATS				// Ӧ����ѡ��ATS��
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccRequestATS(unsigned char ucCID,unsigned char *pATS);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccPPSRequest(unsigned char DSI_DRI)
// ��������:    Э��Ͳ���ѡ������
// �������:    unsigned char DSI_DRI			// b3:2 -- PCD  -> PICC��λ�ʱ���
//												// b1:0 -- PICC -> PCD ��λ�ʱ���
// �������:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	CID�������룬�����Զ���'����ѡ����Ӧ��'��ȡ
//				DSI_DRI:	(00)b -> 106Kb	(01)b -> 212Kb	(10)b -> 424Kb	(11)b -> 847Kb
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccPPSRequest(unsigned char DSI_DRI);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccDeSelect(void)
// ��������:    ������Ŀ�����
// �������:    -		
// �������:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccDeSelect(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccRequestB( unsigned char ucReqCode, unsigned char ucAFI, 
//											unsigned char N, ACTIVEPARAB *pActiParaB)
// ��������:    B�Ϳ�����
// ��ڲ���:    unsigned char ucReqCode			// �������	ISO14443_3B_REQIDL 0x00 -- ���еĿ�
//												//			ISO14443_3B_REQALL 0x08 -- ���еĿ�
//				unsigned char ucAFI				// Ӧ�ñ�ʶ����0x00��ȫѡ
//				unsigned char N					// ʱ϶����,ȡֵ��Χ0--4��
// ���ڲ���:    ACTIVEPARAB *pActiParaB			// ����Ӧ��11�ֽ�
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ� 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccRequestB(unsigned char ucReqCode, unsigned char ucAFI, 
									 unsigned char N, ACTIVEPARAB *pActiParaB);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccSlotMarker(unsigned char N, ACTIVEPARAB *pActiParaB)
// ��������:    ʱ϶��Ƿ���ײ
// ��ڲ���:    unsigned char N��				// ʱ϶��ǣ�ȡֵ��Χ2-16����PiccRequestB()�����й�
// ���ڲ���:    ACTIVEPARAB *pActiveParaB		// ����Ӧ��
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ� 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccSlotMarker(unsigned char N, ACTIVEPARAB *pActiParaB);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccAttrib(unsigned char *pPUPI, unsigned char ucCID, unsigned char ucProType, 
//										 unsigned char ucINFLen, const unsigned char *pHigherLayerINF, 
//										 unsigned char *pRLen, unsigned char *pAATTRIB)
// ��������:    ѡ��PICC
// ��ڲ���:    unsigned char *pPUPI			// 4�ֽ�PICC��ʶ��
//				unsigned char ucCID				// 0 - 14,����֧��CID��������Ϊ0000
//				unsigned char ucProType			// ֧�ֵ�Э�飬�������Ӧ�е�ProtocolTypeָ��
//				unsigned char ucINFLen			// �߲������ֽ���
//				unsigned char *pHigherLayerINF	// �߲�������Ϣ
//				unsigned int nRBufSize			// ���ջ������ĳ���	
// ���ڲ���:    unsigned char *pRLen			// ATTRIB�����Ӧ���ֽ���
//				unsigned char *pAATTRIB			// ATTRIB�����Ӧ
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccAttrib(unsigned char *pPUPI, unsigned char ucCID, unsigned char ucProType, 
								   unsigned char ucINFLen, const unsigned char *pHigherLayerINF, 
								   unsigned int nRBufSize, unsigned char *pRLen, unsigned char *pAATTRIB);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccHaltB(unsigned char *pPUPI)
// ��������:    ����
// ��ڲ���:    unsigned char *pPUPI					// 4�ֽ�PICC��ʶ��
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccHaltB(unsigned char *pPUPI);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccTPCL(unsigned int nSendLen, const unsigned char *pSendBuf, 
//									   unsigned int nRBufSize, unsigned char *pRcvBuf, unsigned int *pRcvLen)
// ��������:    ʵ�ַǽӴ�ʽIC������Э��(T = CL)����ISO14443-4 
// �������:    unsigned int  uiSendLen			// ���͵��ֽ���	
//				unsigned char *pSendBuf			// ���͵�����
//				unsigned int nRBufSize			// ���ջ�������С	
// �������:    unsigned int*pRcvLen			// ���յ��ֽ���
//				unsigned char *pRcvBuf			// ���յ�����
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccTPCL(unsigned int nSendLen, const unsigned char *pSendBuf, unsigned int nRBufSize,
								 unsigned int *pRcvLen, unsigned char *pRcvBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned short __stdcall PiccTPDU(const C_APDU *pAPDUCmd, R_APDU *pAPDURes)
// ��������:    T = CLЭ���APDU����
// ��ڲ���:    C_APDU *pAPDUCmd				// �����APDU�ṹ��
//				unsigned int nRBufSize			// ���ջ������Ĵ�С
// ���ڲ���:    R_APDU *pAPDURes				// ��Ӧ��APDU�ṹ��
// �� �� ֵ:    ����ִ��״̬��SW(����ִ�гɹ�����SW, ���򷵻�����ִ�������Ľ��)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned short __stdcall PiccTPDU(const C_APDU *pAPDUCmd, unsigned int nRBufSize, R_APDU *pAPDURes);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccActivate( unsigned char ucMode, unsigned char ucReqCode,  
//											ACTIVEPARAA *pAInfo)
// ��������:   	A�Ϳ�����
// ��ڲ���:    unsigned char ucMode			// ����ײģʽ
//				unsigned char ucReqCode			// �������
// ���ڲ���:    ACTIVEPARAA *pAInfo				// ISO14443 A��������Ϣ
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccActivate( unsigned char ucMode, unsigned char ucReqCode, ACTIVEPARAA *pAInfo);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccActivateB(unsigned char ucReqCode, unsigned char ucAFI, 
//											ACTIVEPARAB *pActiParaB)
// ��������:    B�Ϳ�����
// ��ڲ���:    unsigned char ucReqCode			// �������	ISO14443_3B_REQIDL 0x00 -- ���еĿ�
//												//			ISO14443_3B_REQALL 0x08 -- ���еĿ�
//				unsigned char ucAFI				// Ӧ�ñ�ʶ����0x00��ȫѡ
// ���ڲ���:    ACTIVEPARAB *pActiParaB			// ����Ӧ��11�ֽ�
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	-   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccActivateB(unsigned char ucReqCode, unsigned char ucAFI, ACTIVEPARAB *pActiParaB);
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ExChangeBlock3(unsigned char CRC_EN, unsigned int uiTpr, unsigned int uiTre,
//											 unsigned int nSendBits, const unsigned char *pSBuf, 
//											 unsigned int nRBufSize, unsigned int *pTime,
//										     unsigned int *pRcvBits, unsigned char *pRBuf)
// ��������:    PCD��PICC�������ݿ�
// ��ڲ���:    unsigned char CRC_EN,			// CRC_EN.7 = 1 -- ����ʹ��CRC;	0 -- ���ͽ�ֹCRC; 
//												// CRC_EN.6 = 1 -- ����ʹ��CRC; 0 -- ���ս�ֹCRC; 
//				unsigned int uiTpr				// ��ʱ����Ƶֵ
//				unsigned int uiTre				// ��ʱʱ�� = (uiTre + 1) * ((uiTpr + 1) / 6.78)us
//				unsigned int nSendBits			// ���͵�λ���������͵�λ������8��������������CRCʹ����Ч	
//				unsigned char *pSBuf			// ���͵�����	
//				unsigned int nRBufSize			// ���ջ������Ĵ�С
// ���ڲ���:    unsigned int *pTime				// ִ�е�ʱ�� = pTime * ((uiTpr + 1) / 6.78)us
//				unsigned int *pRcvBits			// ���յ�λ��
//				unsigned char *pRBuf			// ���յ�����
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ� 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ExChangeBlock3( unsigned char CRC_EN, unsigned int uiTpr, unsigned int uiTre,
										unsigned int nSendBits, const unsigned char *pSBuf, 
										unsigned int nRBufSize, unsigned int *pTime,
										unsigned int *pRcvBits, unsigned char *pRBuf);
//===========================================================================================================
//CMD_TYPE_PLUS_CPU
//===========================================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL0WritePerso(unsigned int uiBNr, const unsigned char *pBuf)
// ��������:    SL0 ���˻���������
// ��ڲ���:    unsigned int uiBNr				// ���ַ
//				unsigned char *pBuf				// д�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL0WritePerso(unsigned int uiBNr, const unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL0CommitPerso(void)
// ��������:    SL0 �ύ���˻�
// ��ڲ���:    -
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
//				��SL0��PLUS CPU����ֻ���޸������µ�ַ����ִ��"SL0�ύ���˻�"���
//				0x9000(������Կ)
//				0x9001(���ÿ���Կ)
//				0x9002(SL2������Կ��ֻ��֧��SL2�Ŀ����и���Կ)
//				0x9003(SL3������Կ��ֻ��֧��SL3�Ŀ����и���Կ)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL0CommitPerso(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL1Auth(unsigned char ucAuthMode, unsigned char ucBlock, unsigned char *pKey)
// ��������:    SL1��֤��Կ
// ��ڲ���:    unsigned char ucAuthMode		// ��֤ģʽ 
//													KEY_TYPE_A | KEY_SOURCE_EXT	�ⲿ�������ԿA��֤
//													KEY_TYPE_A | KEY_SOURCE_E2	�ڲ�E2����ԿA��֤
//													KEY_TYPE_B | KEY_SOURCE_EXT	�ⲿ�������ԿB��֤
//													KEY_TYPE_B | KEY_SOURCE_E2	�ڲ�E2����ԿB��֤
//				unsigned char ucBlock			// ��֤�Ŀ��	MF1 S50����0��63
//												//				MF1 S70����0��255
//				unsigned char *pKey				// ʹ��KEY_SOURCE_E2ģʽʱ��pKey[0]Ϊ��Կ��ŵ�����,������Ч
//												// ʹ��KEY_SOURCE_EXTģʽʱ��pKeyΪ6�ֽ���Կ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL1Auth(unsigned char ucAuthMode, unsigned char ucBlock, const unsigned char *pKey);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL1Read(unsigned char ucBlock, unsigned char *pBuf)
// ��������:   	SL1�����ݿ�
// ��ڲ���:    unsigned char ucBlock			// ������ʼ���
// ���ڲ���:    unsigned char *pBuf				// ����������
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL1Read(unsigned char ucBlock, unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL1Write(unsigned char ucBlock, const unsigned char *pBuf)
// ��������:   	SL1д���ݿ�
// ��ڲ���:    unsigned char ucBNr				// д����ʼ���
//				unsigned char *pBuf				// д�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL1Write(unsigned char ucBlock, const unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL1ReadValue(unsigned char ucBNr, long *pValue)
// ��������:   	SL1����ȡֵ��
// ��ڲ���:    unsigned char ucBNr				// ���Ŀ��
// ���ڲ���:    long *pValue					// ������ֵ
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL1ReadValue(unsigned char ucBNr, long *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL1WriteValue(unsigned char ucBNr, long lValue)
// ��������:   	SL1��д��ֵ
// ��ڲ���:    unsigned char ucBNr				// д�Ŀ��
//				long *pValue					// д����ֵ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL1WriteValue(unsigned char ucBNr, long lValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL1ValueOper(unsigned char ucOperMode, unsigned char ucSBlock, 
//										   unsigned char ucDBlock, long lValue)
// ��������:    SL1ֵ����
// ��ڲ���:    unsigned char ucOperMode		// ֵ����ģʽ
//													MIFARE_INCREMENT ��Դ���ֵ����ָ����ֵ��ŵ�Ŀ�Ŀ�
//													MIFARE_DECREMENT ��Դ���ֵ��ȥָ����ֵ��ŵ�Ŀ�Ŀ�
//													MIFARE_RESTORE   ��Դ���ֵ���Ƶ�Ŀ�Ŀ�
//				unsigned char ucSBlock			// ������Դ���ַ��
//				unsigned char ucDBlock			// ֵ������Ŀ�ؿ�
//				long lValue						// �Ǹ����з�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ģʽΪMIFARE_RESTOREʱ��������lValue����Ч��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall  SL1ValueOper(unsigned char ucOperMode, unsigned char ucSBlock, 
									  unsigned char ucDBlock, long lValue);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL2AESAuth(unsigned char ucAuthMode, unsigned int uiKNr, 
//										 const unsigned char *pAESKey)
// ��������:    SL2��֤��Կ
// ��ڲ���:    unsigned char ucAuthMode		// ��֤ģʽ 
//													KEY_SOURCE_EXT	�ⲿ�������Կ��֤
//													KEY_SOURCE_E2	�ڲ�E2����Կ��֤
//				unsigned int uiKNr				// 2KByte�Ŀ���ȡֵ��Χ0x4000��0x403F
//												// 4KByte�Ŀ���ȡֵ��Χ0x4000��0x404F
//				unsigned char *pKey				// ʹ��KEY_SOURCE_E2ģʽʱ��pKey[0]Ϊ��Կ��ŵ�����,������Ч
//												// ʹ��KEY_SOURCE_EXTģʽʱ��pKeyΪ6�ֽ���Կ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
//				uiKNr��ȡֵ��ֹ0x4000��0x404F����ϸ���������PLUS CPU��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL2AESAuth(unsigned char ucAuthMode, unsigned int uiKNr, 
								   const unsigned char *pAESKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL2CryptoAuth(unsigned char ucAuthMode, unsigned int uiKNr, 
//											const unsigned char *pKey)
// ��������:    SL2��֤��Կ
// ��ڲ���:    unsigned char ucAuthMode		// ��֤ģʽ 
//													KEY_TYPE_A | KEY_SOURCE_EXT	�ⲿ�������ԿA��֤
//													KEY_TYPE_A | KEY_SOURCE_E2	�ڲ�E2����ԿA��֤
//													KEY_TYPE_B | KEY_SOURCE_EXT	�ⲿ�������ԿB��֤
//													KEY_TYPE_B | KEY_SOURCE_E2	�ڲ�E2����ԿB��֤
//				unsigned char ucBlock			// 2KByte�Ŀ���ȡֵ��Χ0��127
//												// 4KByte�Ŀ���ȡֵ��Χ0��255
//				unsigned char *pKey				// ʹ��KEY_SOURCE_E2ģʽʱ��pKey[0]Ϊ��Կ��ŵ�����,������Ч
//												// ʹ��KEY_SOURCE_EXTģʽʱ��pKeyΪ6�ֽ���Կ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL2CryptoAuth(unsigned char ucAuthMode, unsigned char ucBlock, 
									  const unsigned char *pKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL2MultiBlockRead(unsigned char ucBNr, unsigned char ucExt, unsigned char *pBuf)
// ��������:   	SL2����
// ��ڲ���:    unsigned char ucBNr				// ������ʼ���
//				unsigned char ucExt				// ���Ŀ���(ȡֵ��Χ1��3)
// ���ڲ���:    unsigned char *pBuf				// ����������
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL2MultiBlockRead(unsigned char ucBNr, unsigned char ucExt, unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL2MultiBlockWrite(unsigned char ucBNr, unsigned char ucExt, 
//												 const unsigned char *pBuf)
// ��������:   	SL2 ���д
// ��ڲ���:    unsigned char ucBNr				// д����ʼ���
//				unsigned char ucExt				// д�Ŀ���(ȡֵ��Χ1��3)
//				unsigned char *pBuf				// д�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL2MultiBlockWrite(unsigned char ucBNr, unsigned char ucExt, 
										   const unsigned char *pBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL3FirstAuth( unsigned char ucAuthMode, unsigned int uiKNr, 
//											const unsigned char *pAESKey)
// ��������:    SL3 �״���֤
// ��ڲ���:    unsigned char ucAuthMode		// ��֤ģʽ 
//													KEY_SOURCE_EXT	�ⲿ�������Կ��֤
//													KEY_SOURCE_E2	�ڲ�E2����Կ��֤
//				unsigned int uiKNr				// ��Կ��ַ
//				unsigned char *pKey				// ʹ��KEY_SOURCE_E2ģʽʱ��pKey[0]Ϊ��Կ��ŵ�����,������Ч
//												// ʹ��KEY_SOURCE_EXTģʽʱ��pKeyΪ6�ֽ���Կ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
//				uiKNr��ȡֵ��ֹ0x4000��0x404F����ϸ���������PLUS CPU��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3FirstAuth(unsigned char ucAuthMode, unsigned int uiKNr, 
									 const unsigned char *pAESKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL3FollowingAuth( unsigned char ucAuthMode, unsigned int uiKNr, 
//												const unsigned char *pAESKey)
// ��������:    SL3 ������֤
// ��ڲ���:    unsigned char ucAuthMode		// ��֤ģʽ 
//													KEY_SOURCE_EXT	�ⲿ�������Կ��֤
//													KEY_SOURCE_E2	�ڲ�E2����Կ��֤
//				unsigned int uiKNr				// ��Կ��ַ
//				unsigned char *pKey				// ʹ��KEY_SOURCE_E2ģʽʱ��pKey[0]Ϊ��Կ��ŵ�����,������Ч
//												// ʹ��KEY_SOURCE_EXTģʽʱ��pKeyΪ6�ֽ���Կ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
//				���ݿ��������Կ��ַΪ0x4000��0x404Fż����ַΪ��ԿA,������ַΪ��ԿB
//				uiKNr��ȡֵ��ֹ0x4000��0x404F����ϸ���������PLUS CPU��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3FollowingAuth(unsigned char ucAuthMode, unsigned int uiKNr, 
										 const unsigned char *pAESKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL3ResetAuth(void)
// ��������:    SL3 ��λ��֤
// ��ڲ���:    -
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3ResetAuth(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL3Read(  unsigned char ucMode, unsigned char ucBNr, 
//										unsigned char ucExt, unsigned char *pBuf)
// ��������:   	SL3 �����ݿ�
// ��ڲ���:    unsigned char ucMode			// ����ģʽ	0x31 -- ������MAC���������ģ���Ӧ��MAC
//												//			0x33 -- ������MAC���������ģ���Ӧ��MAC
//				unsigned int uiBNr				// ������ʼ���
//				unsigned char ucExt				// ���Ŀ���
// ���ڲ���:    unsigned char *pBuf				// ����������
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ģʽ�е�����/������ָRF���е����ݴ����Ƿ���ܣ���д����PC֮�䴫�������ʼ��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3Read(unsigned char ucMode, unsigned int uiBNr, 
								unsigned char ucExt, unsigned char *pBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL3Write( unsigned char ucMode, unsigned char ucBNr, 
//										unsigned char ucExt, const unsigned char *pBuf)
// ��������:   	SL3 д
// ��ڲ���:    unsigned char ucMode			// д��ģʽ	0xA1 -- ������MAC���������ģ���Ӧ��MAC��
//															0xA3 -- ������MAC���������ģ���Ӧ��MAC��
//				unsigned int uiBNr				// д����ʼ���
//				unsigned char ucExt				// д�Ŀ���(ȡֵ��Χ1��3)
//				unsigned char *pBuf				// д�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ģʽ�е�����/������ָRF���е����ݴ����Ƿ���ܣ���д����PC֮�䴫�������ʼ��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3Write(unsigned char ucMode, unsigned int uiBNr, 
								 unsigned char ucExt, const unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL3ReadValue(unsigned char ucMode, unsigned int uiBNr, long *pValue)
// ��������:   	SL3����ȡֵ��
// ��ڲ���:    unsigned char ucMode			// ����ģʽ	0x31 -- ������MAC���������ģ���Ӧ��MAC
//												//			0x33 -- ������MAC���������ģ���Ӧ��MAC
//				unsigned int uiBNr				// ���Ŀ��
// ���ڲ���:    long *pValue					// ������ֵ
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3ReadValue(unsigned char ucMode, unsigned int uiBNr, long *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL3WriteValue(unsigned char ucMode, unsigned int uiBNr, long lValue)
// ��������:   	SL3��д��ֵ
// ��ڲ���:    unsigned char ucMode			// д��ģʽ	0xA1 -- ������MAC���������ģ���Ӧ��MAC��
//															0xA3 -- ������MAC���������ģ���Ӧ��MAC��
//				unsigned int uiBNr				// д�Ŀ��
//				long *pValue					// д����ֵ
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3WriteValue(unsigned char ucMode, unsigned int uiBNr, long lValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL3ValueOper( unsigned char ucMode, unsigned int uiSBNr, 
//											unsigned int uiDBNr, long lValue)
// ��������:    SL3ֵ����
// ��ڲ���:    unsigned char ucOperMode		// ֵ����ģʽ
//													SL3_INCREMENT	��Դ���ֵ����ָ����ֵ��ŵ�Ŀ�Ŀ�
//													SL3_DECREMENT	��Դ���ֵ��ȥָ����ֵ��ŵ�Ŀ�Ŀ�
//													SL3_RESTORE		��Դ���ֵ���Ƶ�Ŀ�Ŀ�
//													SL3_TRANSFER	���������е����ݴ��䵽ָ���Ŀ�	
//				unsigned int uiSBNr				// ������Դ���ַ(ֻ�������ݿ�)
//				unsigned int uiDBNr				// ֵ������Ŀ�ؿ�(ֻ�������ݿ�)
//				long lValue						// �Ǹ����з�������
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	ģʽΪSL3_RESTOREʱ��������lValue����Ч��
//				ģʽΪSL3_TRANSFERʱ��������lValue����Ч �͡�ucSBlock����Ч��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall  SL3ValueOper(unsigned char ucOperMode,  unsigned int uiSBNr, 
									  unsigned int uiDBNr, long lValue);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL3ProximityCheck(unsigned char ucKeyMode, 
//												const unsigned char *pProxCheckKey, unsigned int *pTime)
// ��������:    �м̹������
// �������:    unsigned char ucKeyMode			// 0 -- ʹ��PICC��֤ʱ�ĻỰKey, 
//												// 1 -- ʹ��ProximityCheckKey
//				unsigned char *pProxCheckKey	// ProximityCheckKey(��Կ��ַ:0xA001),����ucKeyMode=1ʱ��Ч
// �������:    unsigned int *pTime				// ����ִ��ʱ��(us)
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	PPS1�̶�Ϊ0x00���ó���֧�ֱ䲨����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3ProximityCheck(unsigned char ucKeyMode, const unsigned char *pProxCheckKey, 
										  unsigned int *pTime);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char unsigned char PLUSSL3VCSupport(const unsigned char *pIID)
// ��������:    ѡ��Ӧ��(���⿨)
// �������:    unsigned char *pIID						// Ӧ�ñ�ʶ(Installation Identifier,0xB001)
// �������:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	PLUS S��֧�ָ�����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3VCSupport(const unsigned char *pIID);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL3VCSupportLast( const unsigned char *pIID, const unsigned char *pENCKey, 
//												const unsigned char *pMACKey, unsigned char *pCardInfo)
// ��������:    ѡ��Ӧ��(���⿨������ʵ��UID����Ϣ)
// �������:    unsigned char *pIID						// Ӧ�ñ�ʶ(Installation Identifier,0xB001)
//				unsigned char *pENCKey					// ���ݼ�����Կ(��Կ��ַ:0xA080)
//				unsigned char *pMACKey					// MAC������Կ (��Կ��ַ:0xA081)
// �������:    unsigned char *pCardInfo				// ����16�ֽ����� ��ѡ��Ƭʹ��
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	��IID�����ϣ��򷵻ص���ϢΪ16�ֽڵ����������IID��Ч������Ϣ����
//				pCardInfo[1:2] -- PiccCap2
//				pCardInfo[0] = 0x83		4�ֽڿ���	pCardInfo[3:6] -- 4�ֽڿ���
//				pCardInfo[0] = 0x03		7�ֽڿ���	pCardInfo[3:9] -- 7�ֽڿ���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3VCSupportLast(const unsigned char *pIID, const unsigned char *pENCKey, 
										 const unsigned char *pMACKey, unsigned char *pCardInfo);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL3VCSelect(unsigned char *pCardInfo, unsigned char *pMACKey)
// ��������:    ѡ��(���⿨)
// �������:    unsigned char *pCardInfo				// SupportLast����16�ֽ����� ��ѡ��Ƭʹ��
//              unsigned char *pMACKey					// ����MAC����Կ(��Կ��ַ: 0xA000)
// �������:    -             
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	PLUS S��֧�ָ�����	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3VCSelect(const unsigned char *pCardInfo, const unsigned char *pMACKey);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char SL3VCDeselect(void)
// ��������:    ȡ�����⿨ѡ��
// �������:    -
// �������:    -             
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	PLUS S��֧�ָ�����	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3VCDeselect(void);
//===========================================================================================================
#ifdef __cplusplus
	}
#endif

#endif				// __MUR200_H
