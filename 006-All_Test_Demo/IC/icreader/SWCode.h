////////////////////////////////////////////Copyright (c)//////////////////////////////////////////
//                                   ������������Ƭ����չ���޹�˾
//                                         ��    ��    ��
//                                           �𿨲�Ʒ��
//
//                                     http://www.zlgmcu.com
//
//-------------- �ļ���Ϣ--------------------------------------------------------------------------
// �� �� ��:    SWCode.h
// �ļ�����:    SW״̬����
// ��    ��:    V1.0 
// �� �� ��:    ����ɭ
// ��������:    2009.07.17
// ˵    ��:	Ϊ�˷��㴦����SW1 ��SW2 �ϲ�Ϊһ��16λ�����ݽ��д���SW1�ڸ�8λ
//=================================================================================================
//-------------- �޸ļ�¼--------------------------------------------------------------------------
// �޸�����:
// ��ǰ�汾:    
// �� �� ��:
// �޸�����:
// ע    ��: 
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __SW_CODE_H
#define __SW_CODE_H

//=================================================================================================
#define SW_SUCCESS					0x9000		// �����ɹ�
#define SW_OTHER_DATA				0x6100		// �����ɹ���������xx(��8λָ��)�ֽڿ��Կ��Խ���

#define SW_PART_DATA_INVALID		0x6281		// �������ݿ�����Ч
#define SW_FILE_OVER				0x6282		// ����Le�ֽ�ǰ���ļ�/��¼�ѽ���
#define SW_CHOOSE_FILE_INVALID		0x6283		// ѡ����ļ���Ч
#define SW_FCI_INVALID				0x6284		// FCI��Ч

#define SW_AUTH_ERR					0x6300		// ��֤������(��֤ʶ�������)
#define SW_LAST_WRITE_FULL_FILE		0x6381		// ͨ�����д���������ļ�
#define SW_OTHER_ERR				0x63C0		// 0x63Cx x������ʣ��ֵ(��ʣ����֤����)

#define SW_FLAGE_NO_CHANGE			0x6400		// ��ʶ״̬λδ��

//#define SW_NV_ERR					0x6500		// �洢������
#define SW_WRITE_ERR				0x6581		// д����

#define SW_LENGTH_ERR				0x6700		// ���ȴ���

#define SW_NV_ERR					0x6800		// �洢������
#define SW_NAD_NOT_SUPPORT			0x6881		// �߼��ŵ���֧��
#define SW_MAC_NOT_SUPPORT			0x6882		// ��֧�ְ�ȫ����ģʽ

#define SW_CONNOT_PERFORM 			0x6900		// �޷�����Ĳ���
#define SW_CMD_REJECT 				0x6901		// �������(��Ч״̬)
#define SW_CMD_MISMATCH_FILE		0x6981		// �������ļ��ṹ������
#define SW_SECURITY_NOT_SATISFIED	0x6982		// ��ȫ״̬������
#define SW_AUTH_LOCK				0x6983		// ��֤��ʽ������
#define SW_RND_INVALID				0x6984		// �������Ч
#define SW_USE_NOT_SATISFIED		0x6985		// ʹ������������(Ӧ�ñ�����)
#define SW_CMD_PROHIBITED			0x6986		// ���������(�޵�ǰEF)
#define SW_NO_SM					0x6987		// �ް�ȫ����(MAC)
#define SW_SM_DATA_ERR				0x6988		// ��ȫ�����������ȷ

#define SW_PARAMETER_ERR			0x6A00		// ��������
#define SW_APP_LOCKED				0x6A81		// Ӧ�ñ�����
#define SW_FILE_NOT_FIND			0x6A82		// �ļ�δ�ҵ�
#define SW_RECORD_NOT_FIND			0x6A83		// ��¼δ�ҵ�
#define SW_FILE_SPACE_LESS			0x6A84		// �ļ��ռ䲻��
#define SW_P1_P2_INVALID			0x6A86		// P1/P2��������ȷ
#define SW_KEY_NOT_FIND				0x6A88		// �����Կδ�ҵ�

#define SW_OFFSET_ERR				0x6B00		// ƫ��������
#define SW_LE_ERR					0x6C00		// 0x6Cxx Le���ȴ���ʵ�ʳ�����xx
#define SW_INS_INVALID				0x6D00		// INS��Ч/��֧��
#define SW_CLA_INVALID				0x6E00		// CLA��Ч
#define SW_DATA_INVALID				0x6F00		// ������Ч

#define SW_MAC_ERR					0x9302		// MAC����(����0x6988)
#define SW_APP_PERMANENTLY_LOCKED	0x9303		// Ӧ�ñ���������

#define SW_INSUFFICIENT_MONEY		0x9401		// ����
#define SW_KEY_INDEX_NONSUPPORT		0x9403		// ��Կ������֧��
#define SW_MAC_INVALID				0x9406		// MAC��Ч
//============= ������SW�޹ص�״̬���� ============================================================
#define SW_OK						0x0000		// �����ɹ�
//=================================================================================================
#endif				// __SW_CODE_H