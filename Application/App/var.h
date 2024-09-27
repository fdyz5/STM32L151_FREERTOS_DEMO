#ifndef _VAR_H
#define _VAR_H
#include "stm32l1xx.h"
#include <stdint.h>

#define MD_EC20   0
#define MD_M26    1
#define MD_BC26   2
#define MD_SLM130 3
#define MD_L610   4
typedef enum SENSOR_MD
{
    IFC300,
    H200B,//�����������ƣ�H200B���ܵ��������
    XT_803_V1,
    XT_803_V2,//����803C�°�Э��
    XT_MBUS,
    SCL_61,
    SCL_70_1A,
    WT4200S,
    UR_1000, // ����������������
    PRAIS_V18, // ����˹
    W803M_V11,
	SZTAX,
	CJ188,
	TOSHIBA,
	JKW_HM_18D,
	CJ188_V1,
	EMF5000,
	CJ188_JY,
	CJ188_JC,
	TY_2000,		
	CJ188_LYG,//���Ƹ�RTU
	CJ188_DJ,//����RTU
	BJXR_HG702,	//��������һ�����
	BJXR_HARD,	//����Ӳ�ȼ����
	BJXR_WATER,//����ˮ�ʼ����
	MT,		//����
	PRAIS, // ����˹
	CJ188_YG,	//RTU ��
	CJ188_XT,	//RTU����
	NHKJ_TR,		//�ܻۿƼ����������
	CJ188_YG_V1,//RTU ��
	MODBUS_JY,
	SCL_61_RTU,
	MAG6000,
	SM100A,
	W803D,
	JKW_UWM,
	XT_KT_DCS485,
	Q_SERIAL,
	GH_MAG,
	SHDNS,
	TWYH,
	DYP,
	SDBY,
	BX_ZHGS_C,
	LUGB_E,
	HYCL,
	DJ_WM01,
	V51C_ZJCY,//�㽭����
	CJ188_JC19,
	SCL_61D_HZ,
	QDJK_JK,
	CQCY_DC,
	TAZHNY,
	TLU_C_100,
	TCS_LC,
	SCL_61D_YL,
	KT_141L,
	DG_FLOW,
	MODBUS_DJ,
	GH_MAG_MODBUS,
	MODBUS_TUF2000,
	TGYB,
    TGYB_2,
	CJ188_JY2,
	SCL_61H_HZ,	
	VC51_JLJLL,//���ۼ����� �人���׷�
	VC51_WHRZF,//�人���׷�
	L_MAG, //������Դ
	UVC, //�콡����
	MQ_MODBUS, //����������������
	AM4_MODBUS, //ABB������
	SD123_I950,//SD123-I950 ˮ�����ܴ�����	
	AFTU_2, //�������������� ���ڽ���
	DJ_WM01_NP, //DJ_WM01�ͳ����������ƣ� ��������ѹ��  ���ݶ���
	V51C_DAOYITECH, //������һ�Ƽ�   Э��ͬV51_ZJCY
	MODBUS_NS, //����ˮ��  �Ͼ��ǿ��Զ�������ϵͳ���޹�˾	
	DTFX1020, //������������������
	TP_MODBUS,
	MX39_34CR,
	BY_MODBUS,
	MLF_100H,
	V51C_DHZK,  //�����ӿƼ�
	DYP_A07,	//Һλ������
	DJ_WM1034,	//������ˮ�������أ�
	W803E,
	HG_NG,
	QDJC_MODBUS, //����Э��  �ͻ�������ﶨ
	TDS100_MODBUS, //����������������
	CY_MFB,
	LXC_100,
	DTFX1020M,
	XJZD_MODBUS,
	XC_MODBUS,
	ULFM_MODBUS, //ULFM(A)����������������
	SM100B,
	MFE_S,
	SCL_60_HZ,
	SENSUS_HRI,
	TJZNC,
	SCL_60_HZ_YL,
	SCL_70_1A_YL,
	EMWM_MODBUS, //�Ϻ�����  ����	
	FC002, //�������������� �����¶�+ѹ����
	EH6400A,//�����Ƶ��Ӽ������� �����õ���ͨѶЭ���
	WAVE_MODBUS, //������ˮ��
	MODBUS_JYCS, //JYCS������ˮ��������
	CJ188_SZMT, //������̩ˮ��
	SC_MODBUS, //����������
	SCL_61D5_6, //����������+ѹ��
	MT_CJ188, //����������
	MT_CJ188_YL, //����������+ѹ��
	PRAIS_V18_YL,//����˹������+ѹ��	
	CJ188_ZL,//����ˮ��
	SGLDY_MODBUS,//�Ϻ��ϸ괫�����������޹�˾
	L_MAG2,//
	XJZD2_MODBUS,//�½��е�Ƽ�������ѹ����
	MODBUS_HSAZ,//��ˮ��֪���ˮ��
	HLY600_MODBUS,//�Ϻ�����������ת����
	V51_MODBUS,//����ʥ��Ԯˮ��ͨѶЭ��
	MODBUS_TRQXY_D10,
	MODBUS_FLSD,//����˼�µ��������	
	TLU_C_100_YL,//�㽭�����Ǳ�Ƽ����޹�˾[����ѹ��]
	CJ188_DJ_HM05,//����������������, ����ͬ����
	WT4300_MODBUS,
	WALLJ_MODBUS,
	MODBUS_WYYT,//��ѹһ�������
	MODBUS_KFBJ,	
	DLDC_MODBUS,
	MODBUS_SDWZ,
	MODBUS_80G,
	DIEHL_MODBUS,
	MODBUS_KFBJ2,
	EHP400_MODBUS,
	EHP400H_MODBUS,
	CY_MODBUS,
	KEFB,
	Hiwits,
	SCL_61D5_Y01,
	KEFT,
	AM3_Y01_MODBUS,
	AM4_Y01_MODBUS,
	MODBUS_DPL, //������������
	MODBUS_WWJ, //��������λ��	
	DIEHL_AH_MODBUS,
	SCL_61D5_AH,
	MIK_P300, //ѹ��������
	HC_CS9091,
	W801FH_Y01,
	MFB_Y01,
	CJ188_LYG_TY,
	MODBUS_IFC070,
	MFW_M_Y01,
	MODBUS_SUP,
	MODBUS_TJSURE,
	MAG8000,
	KEFT_NZYL,
	MODBUS_MFE600E,
}SENSOR_MD_ENUM;

typedef struct
{ 
    uint8_t *addr;  //��ַ    
    uint8_t size;   //�ֽ� 
    char    type;   //����	
    char    *cmd;   //����   
    char    *name;  //���� 
    void 	*list;  //�б�    
} CFG_T;

#pragma pack(1) //ǿ���ֽڶ���
typedef struct
{ 
	uint8_t  ucInit;    //��ʼ����־
	uint32_t uiTPIDX;   //��������	
	uint32_t uiSPIDX;   //�ϱ�����
	uint32_t uiMAXIDX;  //�������
	
	//  1---ģ���������	
    char  	 caID[10]; //ID��
	char     caPHO[16]; //SIM����	
    uint8_t  ucWT; //������ʽ 	
    uint8_t  ucAUTO; //������ʽ�Զ��л�����
    uint8_t  ucRPST; //DTU����״̬����
    uint8_t  ucSVMD; //������ģʽ	
	
	//  2---�û���������	
	//����1	
    uint8_t  ucBAD; //������ 
    uint8_t  ucDAB; //����λ
    uint8_t  ucCHC; //У��λ
    uint8_t  ucSTO; //ֹͣλ	
	//����2	
    uint8_t  ucBAD2; //������ 
    uint8_t  ucDAB2; //����λ
    uint8_t  ucCHC2; //У��λ
    uint8_t  ucSTO2; //ֹͣλ 	

	//  3---������������	
	uint8_t  ucMSVEN;    //Ӧ������ʹ��	
	uint8_t  ucSRTP;    //������������	
	char     caIP[16];  //IP��ַ
	char     caDNS[33];  //������
    uint16_t usPORT;    //�˿�	
    uint8_t  ucCNTP; //���紫��Э��TCP or UDP
	char     caDNS1IP[16];  //DNS1IP��ַ	
	char     caDNS2IP[16];  //DNS2IP��ַ		
	
    //  4---��չ��������
	//    --->��չ����1:
	uint8_t  ucSMSVEN1;    //Ӧ������ʹ��	
	uint8_t  ucSSRTP1;    //������������	
	char     caSIP1[16];  //IP��ַ
	char     caSDNS1[33];  //������
    uint16_t usSPORT1;    //�˿�	
    uint8_t  ucCNTP1; //���紫��Э��	
	//    --->��չ����2:
	uint8_t  ucSMSVEN2;    //Ӧ������ʹ��	
	uint8_t  ucSSRTP2;    //������������	
	char     caSIP2[16];  //IP��ַ
	char     caSDNS2[33];  //������
    uint16_t usSPORT2;    //�˿�	
    uint8_t  ucCNTP2; //���紫��Э��	
	
	//  5---ͨ������
    uint16_t usMTDN; //������ֽ��� 
    uint16_t usOHRT; //��������ʱ����  
    uint16_t usRCTT; //����ʱ����  
    uint16_t usLPIT; //��������ʱ��  
    uint16_t usSWTT; //����Ϣ����ʱ��
	
	//  6---������������ 	
    char  	 caAPN[33]; //���������
    char  	 caUN[17]; //�û���
    char  	 caUP[17]; //����
    char  	 caSRCD[17]; //�������	
    char  	 caSMSC1[21]; //����Ա����

	//7---RTU����
	uint8_t  ucaMETID[6]; //���ַ 
	uint8_t  ucaDTUSN[6];//�豸��ַ
	uint16_t usTPITV;   //�������	
	uint16_t usSPITV;   //�ϱ����
	uint8_t  ucUPRTUEN;//�����ϱ�RTU����  
	uint8_t  ucUPUMTEN;//�����ϱ����û�������  
	uint8_t  ucSLEEPEN;//����ʹ�� 

	//  8---�Զ���ģʽ����
	//--->��һ·	
    uint8_t  ucAI1MODE; //�ź�����
    uint16_t usRGL1; //��С����
    uint16_t usRGB1; //������� 
    uint16_t usUPL1; //����
    uint16_t usDOL1; //����  
	//--->�ڶ�·		
    uint8_t  ucAI2MODE; //�ź�����
    uint16_t usRGL2; //��С����
    uint16_t usRGB2; //������� 
    uint16_t usUPL2; //����
    uint16_t usDOL2; //����
	//--->����·		
    uint8_t  ucAI3MODE; //�ź�����
    uint16_t usRGL3; //��С����
    uint16_t usRGB3; //������� 
    uint16_t usUPL3; //����
    uint16_t usDOL3; //����

	//9---����ɼ�����
	uint32_t uiPlsCntP; //�������� 
	uint32_t uiPlsCntN; //�������� 
	uint32_t ucALARM; //������־
	uint8_t  ucPSMD;    //����ģʽ	
    uint8_t  ucDEVADDR; //�ⲿ�豸��ַ 
    uint8_t  ucFSTYPE; //�������ͺ�   
    uint8_t  ucSRCHN; //ѹ��������ͨ�� 
    uint8_t  ucSRFMEN; //ʹ����Ӵ���������  	
    uint8_t  ucRTUPLS; //�����������ʹ��	
    uint8_t  ucPULRATIO; //������� 	
    uint8_t  ucSENSTIME; //������Ԥ��ʱ�� 	
    uint8_t  ucDTUDEV; //���̴���
    uint16_t usPLSCJ; //ÿ10����������	
    char  	 caUPTIME[5]; //�ϱ�ʱ������	
	char     caIMEI[16]; //IMEI��

    char  	 caDFOTP[34]; //�Զ���������
    char  	 caDFRGP[72]; //�Զ���ע���	
	uint8_t  ucTMSMD;    //����Э��
	uint8_t  IDLDT;    //��������ʱ��
	
	char     caCCID[21]; //CCID��
    uint8_t  ucDEVADDR2; //�ⲿ�豸��ַ 	
    uint8_t  ucSVTYPE; 	 //���������� 
    uint8_t  ucMODULE;  //ģ��	
	
	char     caFileName[65]; //�ļ���
	uint32_t uiFileSize; //�ļ���С	
	uint32_t uiFileOffset; //�ļ�ƫ��
	
	uint8_t  ucUPDELAYEN; //�ϱ��ӳ�ʹ��
	uint8_t  ucPWRDY;     //VOUT��ѹ	
} VAR_T;

extern  VAR_T  g_tVar;
extern  const char *cpSTVS;
extern  uint8_t g_ucUpdata; 
extern  uint16_t g_Reg[96];
extern  uint16_t g_usErr,g_usRst;
extern  double g_fUTC,g_flatitude,g_flongitude;

int var_init(void);
int var_list(void);
int var_load(void);
int var_cfg(char *cmd, char *buf);
int var_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
int var_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);

#endif
