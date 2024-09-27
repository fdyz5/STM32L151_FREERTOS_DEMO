#include "app.h"

#define COM_CFG   COM1

VAR_T  g_tVar;
uint16_t g_Reg[96];
uint16_t s_usIdxMax;
uint8_t g_ucUpdata = 0;
uint16_t g_usErr = 1,g_usRst = 1;
const char *cpSTVS="SL_XF01_230710T";

double g_fUTC,g_flatitude,g_flongitude;

const char WT[][20] = {"ʵʱ����ģʽ", "����Ϣģʽ", "MODEMģʽ", ""};
const char AUTO[][20] = {"�ر�", "��", "", ""};
const char SVMD[][20] = {"������ģʽ", "��������ģʽ", "", ""};
const char RPST[][20] = {"�ر�", "��", "", ""};
const char BAD[][20] = {"600", "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"};
const char CHC[][20] = {"��Ч��", "żЧ��", "��Ч��", ""};
const char MSVEN[][20] = {"�ر�", "����", "", ""};
const char SRTP[][20] = {"", "IP", "��������", "", ""};
const char CNTP[][20] = {"", "TCP", "UDP", "COAP", "LwM2M", "AEP"};
const char SMSVEN1[][20] = {"�ر�", "����", "", ""};
const char SRTP1[][20] = {"", "IP", "DNS", "", ""};
const char CNTP1[][20] = {"", "TCP", "UDP", "", ""};
const char SMSVEN2[][20] = {"�ر�", "����", "", ""};	
const char SRTP2[][20] = {"", "IP", "DNS", "", ""};
const char CNTP2[][20] = {"", "TCP", "UDP", "", ""};
const char TMSMD[][20] = {"��׼", "�Զ���", "Э��һ", "Э���", "����"};
const char BAD2[][20] = {"600", "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"};
const char CHC2[][20] = {"��Ч��", "żЧ��", "��Ч��", ""};
const char SRAEN[][20] = {"�ر�", "����", "", ""};
const char STSEN[][20] = {"�ر�", "����", "", ""};
const char RTUPLS[][20] = {"��", "��"};
const char PSMD[][20] = {"","������", "˫����", "������", "����˫����","��ͨ˫����","ZH����"};
const char UPRTUEN[][20] = {"�ر�", "����", "", ""};
const char UPUMTEN[][20] = {"�ر�", "����", "", ""};
const char SLEEPEN[][20] = {"�ر�", "����", "", ""};
const char SRFMEN[][20] = {"�ر�", "����", "", ""};
const char UPDELAYEN[][20] = {"�ر�", "����", "", ""};
const char PWRDY[][20] = {"5V", "24V", "12V", "5V"};
const char AI2MODE[][20] = {"4mA-20mA", "0mA-20mA", "4mA-20mA"};
const char AI1MODE[][20] =  {"0.5V-4.5V", "0V-5V", "0V-4.7V", "1.0V-5.0V"};
const char PULRATIO[][20] = {"0.1", "1", "10", "100", "1000", "��������"};
#ifdef __RS485__
const char FSTYPE[][20] = {"IFC300", "H200B", "����_803_V1", "����_803_V2", "����_MBUS_XT", "SCL_61", \
						   "SCL_70_1A", "WT4200S","UR_1000","PRAIS_V18","W803M_V11","SZTAX","CJ188","TOSHIBA","JKW_HM_18D","CJ188_V1",\
						   "EMF5000","CJ188_JY","CJ188_JC","TY_2000","CJ188_LYG","CJ188_DJ","BJXR_HG702","BJXR_HARD","BJXR_WATER",\
						   "MT","PRAIS","CJ188_YG","CJ188_XT","NHKJ_TR","CJ188_YG_V1","MODBUS_JY","SCL_61_RTU","MAG6000","SM100A","W803D",\
						   "JKW_UWM","XT_KT_DCS485","Q_SERIAL","GH_MAG","SHDNS","TWYH","DYP","SDBY","BX_ZHGS_C","LUGB_E","HYCL","DJ_WM01","V51C_ZJCY",\
						   "CJ188_JC19","SCL_61D_HZ","QDJK_JK","CQCY_DC","TAZHNY","TLU_C_100","TCS_LC","SCL_61D_YL","KT_141L","DG_FLOW",\
						   "MODBUS_DJ","GH_MAG_MODBUS","MODBUS_TUF2000","TGYB","TGYB_2","CJ188_JY2","SCL_61H_HZ","VC51_JLJLL","VC51_WHRZF","L_MAG",\
							"UVC","MQ_MODBUS","AM4_MODBUS","SD123_I950","AFTU_2","DJ_WM01_NP","V51C_DAOYITECH","MODBUS_NS","DTFX1020","TP_MODBUS",\
						   "MX39_34CR","BY_MODBUS","MLF_100H","V51C_DHZK","DYP_A07","DJ_WM1034","W803E","HG_NG","QDJC_MODBUS","TDS100_MODBUS",\
						   "CY_MFB","LXC_100","DTFX1020M","XJZD_MODBUS","XC_MODBUS","ULFM_MODBUS","SM100B","MFE_S","SCL_60_HZ","SENSUS_HRI","TJZNC",\
						   "SCL_60_HZ_YL","SCL_70_1A_YL","EMWM_MODBUS","FC002","EH6400A","WAVE_MODBUS","MODBUS_JYCS","CJ188_SZMT","SC_MODBUS",\
						   "SCL_61D5_6","MT_CJ188","MT_CJ188_YL","PRAIS_V18_YL","CJ188_ZL","SGLDY_MODBUS","L_MAG2","XJZD2_MODBUS","MODBUS_HSAZ",\
						   "HLY600_MODBUS","V51_MODBUS","MODBUS_TRQXY_D10","MODBUS_FLSD","TLU_C_100_YL","CJ188_DJ_HM05","WT4300_MODBUS","WALLJ_MODBUS",\
						   "MODBUS_WYYT","MODBUS_KFBJ","DLDC_MODBUS","MODBUS_SDWZ","MODBUS_80G","DIEHL_MODBUS","MODBUS_KFBJ2","EHP400_MODBUS","EHP400H_MODBUS",\
						   "CY_MODBUS","KEFB","Hiwits","SCL_61D5_Y01","KEFT","AM3_Y01_MODBUS","AM4_Y01_MODBUS","MODBUS_DPL","MODBUS_WWJ","DIEHL_AH_MODBUS",\
						   "SCL_61D5_AH", "MIK_P300","HC_CS9091","W801FH_Y01","MFB_Y01","CJ188_LYG_TY","MODBUS_IFC070","MFW_M_Y01","MODBUS_SUP","MODBUS_TJSURE"\
						   "MAG8000","KEFT_NZYL","MODBUS_MFE600E",							  
};
#endif

// s:�ַ���	b:BCD  u:���� p:�������					   
const CFG_T  s_taCfg[]=
{
	
{0, 0, 's', 0, "*************************************\r\n"}, 	
{0, 0, 's', 0, "               �����б�\r\n"}, 
{0, 0, 's', 0, "version:SL_XF01_V1.0_230710T\r\n"}, 	
{0, 0, 's', 0, "*************************************\r\n\r\n"}, 

	
{0, 0, 's', 0, "\r\n1---ģ���������\r\n"}, 
{(uint8_t *)&g_tVar.caID[0], 10, 's', "+ID", "ID��", 0},
{(uint8_t *)&g_tVar.caPHO[0], 16, 's', "+PHO", "SIM����", 0},
{(uint8_t *)&g_tVar.ucWT, 1, 'u', "+WT", "������ʽ", (char *)&WT},
{(uint8_t *)&g_tVar.ucAUTO, 1, 'u', "+AUTO", "������ʽ�Զ��л�����", (char *)&AUTO},
{(uint8_t *)&g_tVar.ucRPST, 1, 'u', "+WT", "DTU����״̬����", (char *)&WT},
{(uint8_t *)&g_tVar.ucSVMD, 1, 'u', "+SVMD", "������ģʽ", (char *)&SVMD},

{0, 0, 's', 0, "\r\n1---�û���������\r\n"},
{0, 0, 's', 0, "\r\n-->����1:\r\n"},
{(uint8_t *)&g_tVar.ucBAD, 1, 'u', "+BAD", "������", (char *)&BAD}, 
{(uint8_t *)&g_tVar.ucDAB, 1, 'u', "+DAB", "����λ", 0}, 
{(uint8_t *)&g_tVar.ucCHC, 1, 'u', "+CHC", "Ч��λ", (char *)&CHC},
{(uint8_t *)&g_tVar.ucSTO, 1, 'u', "+STO", "ֹͣλ", 0},
{0, 0, 's', 0, "\r\n-->����2:\r\n"},
{(uint8_t *)&g_tVar.ucBAD2, 1, 'u', "+BAD2", "������", (char *)&BAD2}, 
{(uint8_t *)&g_tVar.ucDAB2, 1, 'u', "+DAB2", "����λ", 0}, 
{(uint8_t *)&g_tVar.ucCHC2, 1, 'u', "+CHC2", "Ч��λ", (char *)&CHC2},
{(uint8_t *)&g_tVar.ucSTO2, 1, 'u', "+STO2", "ֹͣλ", 0},

{0, 0, 's', 0, "\r\n2---������������\r\n"},
{(uint8_t *)&g_tVar.ucMSVEN, 1, 'u', "+MSVEN", "Ӧ������ʹ��", (char *)&MSVEN},
{(uint8_t *)&g_tVar.ucSRTP, 1, 'u', "+SRTP", "������������", (char *)&SRTP},
{(uint8_t *)&g_tVar.caIP[0], 16, 's', "+IP", "IP��ַ", 0},
{(uint8_t *)&g_tVar.caDNS[0], 33, 's', "+DNS", "������", 0},
{(uint8_t *)&g_tVar.usPORT, 2, 'u', "+PORT", "�˿�", 0},
{(uint8_t *)&g_tVar.ucCNTP, 1, 'u', "+CNTP", "����Э��", (char *)&CNTP},
{(uint8_t *)&g_tVar.caDNS1IP[0], 16, 's', "+DNSIP1", "DNS1IP��ַ", 0},
{(uint8_t *)&g_tVar.caDNS2IP[0], 16, 's', "+DNSIP2", "DNS2IP��ַ", 0},

{0, 0, 's', 0, "\r\n3---��չ����1����\r\n"},
{(uint8_t *)&g_tVar.ucSMSVEN1, 1, 'u', "+SMSVEN1", "Ӧ������ʹ��", (char *)&SMSVEN1},
{(uint8_t *)&g_tVar.ucSSRTP1, 1, 'u', "+SRTP1", "������������", (char *)&SRTP1},
{(uint8_t *)&g_tVar.caSIP1[0], 16, 's', "+SIP1", "IP��ַ", 0},
{(uint8_t *)&g_tVar.caSDNS1[0], 33, 's', "+SDNS1", "������", 0},
{(uint8_t *)&g_tVar.usSPORT1, 2, 'u', "+SPORT1", "�˿�", 0},
{(uint8_t *)&g_tVar.ucCNTP1, 1, 'u', "+CNTP1", "����Э��", (char *)&CNTP1},

{0, 0, 's', 0, "\r\n4---��չ����2����\r\n"},
{(uint8_t *)&g_tVar.ucSMSVEN2, 1, 'u', "+SMSVEN2", "Ӧ������ʹ��", (char *)&SMSVEN2},
{(uint8_t *)&g_tVar.ucSSRTP2, 1, 'u', "+SRTP2", "������������", (char *)&SRTP2},
{(uint8_t *)&g_tVar.caSIP2[0], 16, 's', "+SIP2", "IP��ַ", 0},
{(uint8_t *)&g_tVar.caSDNS2[0], 33, 's', "+SDNS2", "������", 0},
{(uint8_t *)&g_tVar.usSPORT2, 2, 'u', "+SPORT2", "�˿�", 0},
{(uint8_t *)&g_tVar.ucCNTP2, 1, 'u', "+CNTP2", "����Э��", (char *)&CNTP2},

{0, 0, 's', 0, "\r\n5---ͨ������\r\n"},
{(uint8_t *)&g_tVar.usMTDN, 2, 'u', "+MTDN", "������ֽ���", 0}, 
{(uint8_t *)&g_tVar.usOHRT, 2, 'u', "+OHRT", "��������ʱ����", 0},
{(uint8_t *)&g_tVar.usRCTT, 2, 'u', "+RCTT", "����ʱ����", 0}, 
{(uint8_t *)&g_tVar.usLPIT, 2, 'u', "+LPIT", "��������ʱ��", 0},
{(uint8_t *)&g_tVar.usSWTT, 2, 'u', "+SWTT", "����Ϣ����ʱ��", 0},

{0, 0, 's', 0, "\r\n6---������������\r\n"},
{(uint8_t *)&g_tVar.caAPN[0], 33, 's', "+APN", "���������", 0},
{(uint8_t *)&g_tVar.caUN[0], 17, 's', "+UN", "�û���", 0},
{(uint8_t *)&g_tVar.caUP[0], 17, 's', "+UP", "����", 0},
{(uint8_t *)&g_tVar.caSRCD[0], 17, 's', "+SRCD", "�������", 0},

{0, 0, 's', 0, "\r\n7---RTU����\r\n"},
{(uint8_t *)&g_tVar.ucaMETID[0], 6, 'b', "+METID", "���", 0}, 
{(uint8_t *)&g_tVar.ucaDTUSN[0], 6, 'b', "+DTUSN", "�豸��ַ", 0},
{(uint8_t *)&g_tVar.usTPITV, 2, 'u', "+TPITV", "�ɼ����", 0}, 
{(uint8_t *)&g_tVar.usSPITV, 2, 'u', "+SPITV", "�ϱ����", 0}, 
{(uint8_t *)&g_tVar.ucUPRTUEN, 1, 'u', "+UPRTUEN", "�����ϱ�RTU����", (char *)&UPRTUEN}, 
{(uint8_t *)&g_tVar.ucUPUMTEN, 1, 'u', "+UPUMTEN", "�����ϱ����û�������", (char *)&UPUMTEN}, 
{(uint8_t *)&g_tVar.ucSLEEPEN, 1, 'u', "+SLEEPEN", "����ʹ��", (char *)&SLEEPEN}, 

{0, 0, 's', 0, "\r\n8---ģ��ͨ������\r\n"},
{0, 0, 's', 0, "\r\n--->��һ·\r\n"},
{(uint8_t *)&g_tVar.ucAI1MODE, 1, 'u', "+AI1MODE",  "�ź�����", (char *)&AI1MODE}, 
{(uint8_t *)&g_tVar.usRGL1, 2, 'u', "+RGL1", "������Сֵ", 0}, 
{(uint8_t *)&g_tVar.usRGB1, 2, 'u', "+RGB1", "�������ֵ", 0}, 
{(uint8_t *)&g_tVar.usUPL1, 2, 'u', "+UPL1", "����", 0}, 
{(uint8_t *)&g_tVar.usDOL1, 2, 'u', "+DOL1", "����", 0},
{0, 0, 's', 0, "\r\n--->�ڶ�·\r\n"},
{(uint8_t *)&g_tVar.ucAI2MODE, 1, 'u', "+AI2MODE",  "�ź�����", (char *)&AI2MODE}, 
{(uint8_t *)&g_tVar.usRGL2, 2, 'u', "+RGL2", "������Сֵ", 0}, 
{(uint8_t *)&g_tVar.usRGB2, 2, 'u', "+RGB2", "�������ֵ", 0}, 
{(uint8_t *)&g_tVar.usUPL2, 2, 'u', "+UPL2", "����", 0}, 
{(uint8_t *)&g_tVar.usDOL2, 2, 'u', "+DOL2", "����", 0},
{0, 0, 's', 0, "\r\n--->����·\r\n"},
{(uint8_t *)&g_tVar.ucAI3MODE, 1, 'u', "+AI3MODE",  "�ź�����", 0}, 
{(uint8_t *)&g_tVar.usRGL3, 2, 'u', "+RGL3", "������Сֵ", 0}, 
{(uint8_t *)&g_tVar.usRGB3, 2, 'u', "+RGB3", "�������ֵ", 0}, 
{(uint8_t *)&g_tVar.usUPL3, 2, 'u', "+UPL3", "����", 0}, 
{(uint8_t *)&g_tVar.usDOL3, 2, 'u', "+DOL3", "����", 0},

{0, 0, 's', 0, "\r\n9---����ɼ�����\r\n"},
{(uint8_t *)&g_tVar.uiPlsCntP, 4, 'p', "+PLSKP", "��������", 0}, 
{(uint8_t *)&g_tVar.uiPlsCntN, 4, 'p', "+PLSKP", "��������", 0},
{(uint8_t *)&g_tVar.ucALARM, 1, 'u', "+ALARM",  "������־", 0},
{(uint8_t *)&g_tVar.ucPSMD, 1, 'u', "+PSMD", "����ģʽ", (char *)&PSMD}, 
{(uint8_t *)&g_tVar.ucDEVADDR, 1, 'u', "+DEVADDR", "�ⲿ�豸��ַ", 0},
{(uint8_t *)&g_tVar.ucDEVADDR2, 1, 'u', "+DEVADDR2", "�ⲿ�����豸��ַ", 0},
#ifdef __RS485__
{(uint8_t *)&g_tVar.ucFSTYPE, 1, 'u', "+FSTYPE", "�������ͺ�", (char *)&FSTYPE},
#endif
{(uint8_t *)&g_tVar.ucSRCHN, 1, 'u', "+SRCHN", "ѹ��������ͨ��", 0},
{(uint8_t *)&g_tVar.ucSRFMEN, 1, 'u', "+SRFMEN", "ʹ����Ӵ���������", (char *)&SRFMEN},
{(uint8_t *)&g_tVar.ucRTUPLS, 1, 'u', "+RTUPLS", "�����������ʹ��", (char *)&RTUPLS},
{(uint8_t *)&g_tVar.ucPULRATIO, 1, 'u', "+PULRATIO", "�������", (char *)&PULRATIO},
{(uint8_t *)&g_tVar.ucSENSTIME, 1, 'u', "+SENSTIME", "������Ԥ��ʱ��", 0},
{(uint8_t *)&g_tVar.ucDTUDEV, 1, 'u', "+DTUDEV", "���̴���", 0},
{(uint8_t *)&g_tVar.usPLSCJ, 2, 'u', "+PLSCJ", "ÿ10����������", 0},
{(uint8_t *)&g_tVar.caUPTIME[0], 5, 's', "+UPTIME", "�ϱ�ʱ������", 0},
{(uint8_t *)&g_tVar.caIMEI[0], 16, 's', "+IMEI", "IMEI��", 0},
{(uint8_t *)&g_tVar.caCCID[0], 21, 's', "+ICCID", "ICCID��", 0},

{0, 0, 's', 0, "\r\n10---��չ����\r\n"},
{(uint8_t *)&g_tVar.ucTMSMD, 1, 'u', "+TMSMD", "����Э��", (char *)&TMSMD},
{(uint8_t *)&g_tVar.caDFOTP[0], 34, 's', "+DFOTP", "�Զ���������", 0},
{(uint8_t *)&g_tVar.caDFRGP[0], 72, 's', "+DFRGP", "�Զ���ע���", 0},
{(uint8_t *)&g_tVar.uiTPIDX, 4, 'u', "+TPIDX", "��������", 0},
{(uint8_t *)&g_tVar.uiSPIDX, 4, 'u', "+SPIDX", "�ϱ�����", 0},
{(uint8_t *)&g_tVar.uiMAXIDX, 4, 'u', "+MAXIDX", "�������", 0},
{(uint8_t *)&g_tVar.ucUPDELAYEN, 1, 'u', "+UPDELAYEN", "�ϱ��ӳ�", (char *)&UPDELAYEN},
{(uint8_t *)&g_tVar.ucPWRDY, 1, 'u', "+PWRDY", "�����ѹ", (char *)&PWRDY},

{0, 0, 's', 0, "\r\nYN>\r\n"},

};
	
int var_cfg(char *cmd, char *buf)
{
	char *p,*q;
	int idx,temp;
	uint16_t offset;

	p = strstr((char *)cmd,"YN");	
	if(p != NULL)
	{
		if(strstr(cmd, "+RCCCSR" ) != NULL)
		{
			sprintf(buf,"RCCCSR:%08X\r\n",RCC->CSR);
			RCC->CSR |= RCC_CSR_RMVF;
		}			
		else if(strstr(cmd, "+LIST" ) != NULL)
		{
			sprintf(buf,"OK\r\n");				
			var_list();						
		}
		else if(strstr(cmd, "+STVS" ) != NULL) //�汾
		{
			sprintf(buf,"%s", cpSTVS);						
		}
		else if(strstr(cmd, "+RST" ) != NULL) //��λ
		{					
			NVIC_SystemReset();
		}
		else if(strstr(cmd, "+TIME=?" ) != NULL) //��ȡʱ��
		{
			bsp_rtc_get_time();	
			sprintf(buf,"\r\nTIME:%04d-%02d-%02d %02d:%02d:%02d\r\n", 
							  g_tRTC.Year,g_tRTC.Mon,g_tRTC.Day,g_tRTC.Hour,g_tRTC.Min,g_tRTC.Sec);				                  		
		}
		else if((p = strstr(cmd, "+TIME=" )) != NULL) //ͬ��ʱ��
		{
			sprintf(buf,"OK\r\n");	
			p += 6;
			g_tRTC.Sec  = (p[17]-0x30)*10 + (p[18]-0x30); 
			g_tRTC.Min  = (p[14]-0x30)*10 + (p[15]-0x30);
			g_tRTC.Hour = (p[11]-0x30)*10 + (p[12]-0x30);                    
			g_tRTC.Day  = (p[8]-0x30)*10  + (p[9]-0x30);                                        
			g_tRTC.Mon  = (p[5]-0x30)*10  + (p[6]-0x30);
			g_tRTC.Year = (p[2]-0x30)*10  + (p[3]-0x30);                                         					
			bsp_rtc_set_time(g_tRTC.Year,g_tRTC.Mon,g_tRTC.Day,g_tRTC.Hour,g_tRTC.Min,g_tRTC.Sec);		
			g_ucRtcCali = 1;			
		}					
		else if(strstr(cmd, "+LOAD" ) != NULL)  //��λĬ������
		{
			if(var_load() == 1)
			{
				sprintf(buf,"OK\r\n");	
			}							
		}
		else if(strstr(cmd, "+CHKMD" ) != NULL) //�Լ���Ϣ 
		{
			g_ucRptTrig = 1;
			g_ucAqtTrig = 1;					
			sprintf(buf,"OK\r\n");						
		}
		else if(strstr(cmd, "+CLRFH" ) != NULL) //���FLASH 
		{
			uint32_t addr;
			
			g_tVar.uiTPIDX = 0;   //��������	
			addr = OFFSET_OF(VAR_T, uiTPIDX);
			var_WriteBytes((uint8_t *)&g_tVar.uiTPIDX, addr, 4); 	
			var_ReadBytes((uint8_t *)&g_tVar.uiTPIDX, addr, 4); 					
			g_tVar.uiSPIDX = 0;   //�ϱ�����	
			addr = OFFSET_OF(VAR_T, uiSPIDX);
			var_WriteBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
			var_ReadBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4);  
			comSendBuf(COM_CFG, (uint8_t *)"OK\r\n", 4);

			sf_EraseChip();
			sprintf(buf,"OK\r\n");							
		}				
		else if(strstr(cmd, "+OLTST" ) != NULL) //�����ϱ�
		{
			g_ucRptTrig = 1;
			g_ucAqtTrig = 1;
			
			sprintf(buf,"OK\r\n");						
		}
		//else if((strstr(cmd, "+AI" ) != NULL) &&(strstr(cmd, "+AI1MODE" ) == NULL)) //����ָ�� ��+AI1MODE��ͻ�����ɼ�
		else if((strcmp(cmd, "YN+AI\r\n") == 0)||(strcmp(cmd, "YN+AI") == 0))	//����ָ�� ��+AI��ͷ��ָ�����ͻ�������ɼ�
		{
			g_ucAqtTrig = 1;
			sprintf(buf,"OK\r\n");						
		}		
		else if(strstr(cmd, "+READDATA" ) != NULL) //�����ɼ�
		{
			g_ucAqtTrig = 1;
			sprintf(buf,"OK\r\n");						
		}				
		else if(strstr(cmd, "+SETIMEI" ) != NULL) //�Զ�����SIM����
		{
			
		}						
		else if(strstr(cmd, "+CSQ" ) != NULL)
		{
			g_ucRptTrig = 1;					
			sprintf(buf,"OK\r\n");
		}
		else if(strstr(cmd, "+RPT" ) != NULL)
		{
			sprintf(buf,"sta: %02X state: %02X\r\n",rpt_sta(),rpt_state());
		}			
		else
		{
			for(idx=0; idx<s_usIdxMax; idx++)
			{	
				p = strstr(cmd, s_taCfg[idx].cmd);
				if(p != NULL)
				{
					p += strlen(s_taCfg[idx].cmd);
					if(*p == '=')		
					{
						p += 1;
						if(*p == '?') //��
						{
							sprintf(buf, "%s:", s_taCfg[idx].cmd+1);					
							if(s_taCfg[idx].type == 'b') //BCD����
							{	
								bcd_to_str(s_taCfg[idx].addr, buf+strlen(buf), s_taCfg[idx].size);		
							}
							else if(s_taCfg[idx].type == 's') //�ַ�������
							{
								sprintf(buf+strlen(buf), "%s", s_taCfg[idx].addr); 														
							}
							else if(s_taCfg[idx].type == 'p') //��������
							{							
								sprintf(buf+strlen(buf), "%u,%u", g_tVar.uiPlsCntP,g_tVar.uiPlsCntN); 														
							}					
							else
							{
								memset((uint8_t *)&temp, 0, sizeof(temp));	
								memcpy((uint8_t *)&temp, s_taCfg[idx].addr, s_taCfg[idx].size);						
								sprintf(buf+strlen(buf), "%u", temp); 						
							}
							strcat(buf, ";");			   
				
						}
						else //д
						{
							if(s_taCfg[idx].type == 'b') //BCD����
							{
								str_to_bcd(p, s_taCfg[idx].addr, s_taCfg[idx].size*2);								
							}
							else if(s_taCfg[idx].type == 's') //�ַ�������
							{
								q = strstr(cmd, "\r\n");
								if(q != NULL) *q = 0; 							
								memset(s_taCfg[idx].addr, 0, s_taCfg[idx].size);
								memcpy(s_taCfg[idx].addr, p, strlen(p));													
							}
							else if(s_taCfg[idx].type == 'p') //��������
							{						
								q = strstr(cmd, ",");
								if(q != NULL)
								{
									*q = 0; q += 1;	
									temp = atoi(p); 							
									memcpy((uint8_t *)&g_tVar.uiPlsCntP, (uint8_t *)&temp, s_taCfg[idx].size);
									offset = (uint8_t *)&g_tVar.uiPlsCntP - &g_tVar.ucInit;					
									var_WriteBytes((uint8_t *)&g_tVar.uiPlsCntP, offset, s_taCfg[idx].size);							
									temp = atoi(q); 
									memcpy((uint8_t *)&g_tVar.uiPlsCntN, (uint8_t *)&temp, s_taCfg[idx].size);							
									offset = (uint8_t *)&g_tVar.uiPlsCntN - &g_tVar.ucInit;					
									var_WriteBytes((uint8_t *)&g_tVar.uiPlsCntN, offset, s_taCfg[idx].size);
												
									sprintf(buf,"OK\r\n");												
								}															
							}					
							else
							{
								temp = atoi(p); 
								memcpy(s_taCfg[idx].addr, (uint8_t *)&temp, s_taCfg[idx].size);							
							}
							
							if(s_taCfg[idx].type != 'p')
							{
								offset = s_taCfg[idx].addr - &g_tVar.ucInit;					
								if(var_WriteBytes(s_taCfg[idx].addr, offset, s_taCfg[idx].size) == 1)
								{	

									sprintf(buf,"OK\r\n");	
								}
							}					
						}
													
						break;
					}
				}
			}
		}		
	}
	
	return 0;
}

int var_list(void)
{
	char *p, buf[128];
	uint32_t idx,temp;	
	
	for(idx=0; idx<s_usIdxMax; idx++)
	{
	
		if(s_taCfg[idx].addr != NULL) 
		{
			sprintf(buf, " -->%s:", s_taCfg[idx].name);						
			if(s_taCfg[idx].type == 'b') //BCD����
			{	
				bcd_to_str(s_taCfg[idx].addr, buf+strlen(buf), s_taCfg[idx].size);		
			}
			else if(s_taCfg[idx].type == 's') //�ַ�������
			{
				s_taCfg[idx].addr[s_taCfg[idx].size-1] = 0;					
				sprintf(buf+strlen(buf), "%s", s_taCfg[idx].addr); 														
			}
			else
			{
				temp = 0;
				memcpy((uint8_t *)&temp, s_taCfg[idx].addr, s_taCfg[idx].size);
				if(s_taCfg[idx].list != NULL )
				{
					p = (char *)s_taCfg[idx].list + temp*20;
					sprintf(buf+strlen(buf), "%s", p); 					
				}
				else
				{
					sprintf(buf+strlen(buf), "%u", (uint32_t)temp);
				}				
			}
		}
		else
		{
			sprintf(buf, "%s", s_taCfg[idx].name);				
		}
		strcat(buf, "\r\n");			   
		comSendBuf(COM_CFG, (uint8_t *)buf, strlen(buf));		
	}

	return 1;
}

int var_load(void)
{
	//  ��ʼ����־
    g_tVar.ucInit = 0xAA; 
//    g_tVar.uiTPIDX = 0;   //��������	
//    g_tVar.uiSPIDX = 0;   //�ϱ�����
    g_tVar.uiMAXIDX = 4096;  //�������  MX25L12835F ��������	
	
	//  1---ģ���������
	memset(g_tVar.caID, 0, 10);
    sprintf(g_tVar.caID, "03710001"); //ID�� 
	memset(g_tVar.caPHO, 0, 16);	
    sprintf(g_tVar.caPHO, "00000000000"); //SIM����  
    g_tVar.ucWT = 0; //������ʽ 	
    g_tVar.ucAUTO = 0; //������ʽ�Զ��л�����
    g_tVar.ucRPST = 1; //DTU����״̬����
    g_tVar.ucSVMD = 0; //������ģʽ	
	
	//  2---�û���������	
	//����1	
    g_tVar.ucBAD = 4; //������ 
    g_tVar.ucDAB = 8; //����λ
    g_tVar.ucCHC = 0; //У��λ
    g_tVar.ucSTO = 1; //ֹͣλ	
	//����2	
    g_tVar.ucBAD2 = 4; //������ 
    g_tVar.ucDAB2 = 8; //����λ
    g_tVar.ucCHC2 = 0; //У��λ
    g_tVar.ucSTO2 = 1; //ֹͣλ 	

	//  3---������������	
    g_tVar.ucMSVEN = 1;    //Ӧ������ʹ��	
    g_tVar.ucSRTP = 1;    //������������

	memset(g_tVar.caIP, 0, 16);
//    sprintf(g_tVar.caIP, "110.87.107.225");//IP��ַ   
//    g_tVar.usPORT = 7270;   //IP�˿� sokit����

//    sprintf(g_tVar.caIP, "114.115.136.216");//IP��ַ   
//    g_tVar.usPORT = 8809;   //IP�˿� С��ϵͳ������TCP
	
//    sprintf(g_tVar.caIP, "114.115.136.216");//IP��ַ   
//    g_tVar.usPORT = 8808;   //IP�˿� С��ϵͳ���RTU	

//    sprintf(g_tVar.caIP, "114.115.136.216");//IP��ַ   
//    g_tVar.usPORT = 6666;   //IP�˿� С��ϵͳNB��UDP 	
	
//    sprintf(g_tVar.caIP, "114.115.202.57");//IP��ַ   
//    g_tVar.usPORT = 6690;   //IP�˿� ���ƽ̨UDP 	

//    sprintf(g_tVar.caIP, "60.205.5.172");//IP��ַ  ��Դ 
//    g_tVar.usPORT = 7089;   //IP�˿� ���ƽ̨UDP 

//    sprintf(g_tVar.caIP, "182.61.28.205");//IP��ַ �¿��� 
//    g_tVar.usPORT = 19002;   //IP�˿�  TCP	
	
	
//    sprintf(g_tVar.caIP, "180.101.147.115");//IP��ַ   
//    g_tVar.usPORT = 5683;   //IP�˿�  ����iotƽ̨ 

    sprintf(g_tVar.caIP, "114.115.136.216");//IP��ַ   
    g_tVar.usPORT = 6690;   //IP�˿� �������������Ʒ���ƽ̨ UDP 	

	memset(g_tVar.caDNS, 0, 33);
    sprintf(g_tVar.caDNS, "www.xmyn.com");  //������  
#ifdef __SL6200N__
    g_tVar.ucCNTP = 4; //���紫��Э�� 1=TCP 2=UDP 3=COAP  4=LwM2M  5=AEP 6=��ƽ̨	
#else
    g_tVar.ucCNTP = 2; //���紫��Э�� 1=TCP 2=UDP 3=COAP  4=LwM2M  5=AEP 6=��ƽ̨
#endif
	memset(g_tVar.caDNS1IP, 0, 16);
    sprintf(g_tVar.caDNS1IP, "211.138.151.161");  //DNS1IP��ַ	
	memset(g_tVar.caDNS2IP, 0, 16);
    sprintf(g_tVar.caDNS2IP, "103.44.145.245");  //DNS2IP��ַ	
	
    //  4---��չ��������
	//    --->��չ����1:
    g_tVar.ucSMSVEN1 = 1;    //Ӧ������ʹ��	
    g_tVar.ucSSRTP1 = 1;    //������������	
	memset(g_tVar.caSIP1, 0, 16);
    sprintf(g_tVar.caSIP1, "114.115.136.216"); //IP��ַ	
	memset(g_tVar.caSDNS1, 0, 33);
    sprintf(g_tVar.caSDNS1, "www.xmyn.com");  //������   
    g_tVar.usSPORT1 = 8001; //�˿�	
    g_tVar.ucCNTP1 = 2; //���紫��Э��	
	//    --->��չ����2:
    g_tVar.ucSMSVEN2 = 0;    //Ӧ������ʹ��	
    g_tVar.ucSSRTP2 = 1;    //������������	
	memset(g_tVar.caSIP2, 0, 16);
    sprintf(g_tVar.caSIP2, "0.0.0.0"); //IP��ַ	
	memset(g_tVar.caSDNS2, 0, 33);
    sprintf(g_tVar.caSDNS2, "www.xmyn.com");  //������   
    g_tVar.usSPORT2 = 7010; //�˿�	
    g_tVar.ucCNTP2 = 2; //���紫��Э��		
	
	//  5---ͨ������
    g_tVar.usMTDN = 1024; //������ֽ��� 
    g_tVar.usOHRT = 60; //��������ʱ����  
    g_tVar.usRCTT = 3; //����ʱ����  
    g_tVar.usLPIT = 60; //��������ʱ��  
    g_tVar.usSWTT = 0; //����Ϣ����ʱ��
	
	//  6---������������ 
	memset(g_tVar.caAPN, 0, 33);
    sprintf(g_tVar.caAPN, "CMNET"); //���������		
	memset(g_tVar.caUN, 0, 17);
    sprintf(g_tVar.caUN, ""); //�û���
	memset(g_tVar.caUP, 0, 17);
    sprintf(g_tVar.caUP, ""); //����
	memset(g_tVar.caSRCD, 0, 17);
    sprintf(g_tVar.caSRCD, "*98#"); //�������
	memset(g_tVar.caSMSC1, 0, 21);
    sprintf(g_tVar.caSMSC1, "");  //����Ա����

	//7---RTU����
	g_tVar.ucaMETID[0] = 0x00; //���ַ
	g_tVar.ucaMETID[1] = 0x00; //���ַ    
	g_tVar.ucaMETID[2] = 0x00; //���ַ    
	g_tVar.ucaMETID[3] = 0x00; //���ַ
	g_tVar.ucaMETID[4] = 0x00; //���ַ
	g_tVar.ucaMETID[5] = 0x00; //���ַ 

    g_tVar.ucaDTUSN[0] = 0x00;//�豸��ַ(BCD)
    g_tVar.ucaDTUSN[1] = 0x00;//�豸��ַ(BCD)    
    g_tVar.ucaDTUSN[2] = 0x00;//�豸��ַ(BCD) 
    g_tVar.ucaDTUSN[3] = 0x00;//�豸��ַ(BCD)    
    g_tVar.ucaDTUSN[4] = 0x00;//�豸��ַ(BCD)
    g_tVar.ucaDTUSN[5] = 0x00;//�豸��ַ(BCD)

	
    g_tVar.usTPITV = 7200; //������� 
    g_tVar.usSPITV = 480; //�ϱ����
   
    g_tVar.ucUPRTUEN= 1;//�����ϱ�RTU����  
    g_tVar.ucUPUMTEN = 0;//�����ϱ����û�������  
    g_tVar.ucSLEEPEN = 1;//����ʹ�� 

	//  8---�Զ���ģʽ����
	//--->��һ·	
    g_tVar.ucAI1MODE = 0; //�ź�����
    g_tVar.usRGL1 = 0; //��С����
    g_tVar.usRGB1 = 1000; //������� 
    g_tVar.usUPL1 =1000; //����
    g_tVar.usDOL1 = 0; //����  
	//--->�ڶ�·		
    g_tVar.ucAI2MODE = 0; //�ź�����
    g_tVar.usRGL2 = 0; //��С����
    g_tVar.usRGB2= 1000; //������� 
    g_tVar.usUPL2 = 1000; //����
    g_tVar.usDOL2 = 0; //����
	//--->����·		
    g_tVar.ucAI3MODE = 0; //�ź�����
    g_tVar.usRGL3 = 0; //��С����
    g_tVar.usRGB3= 1000; //������� 
    g_tVar.usUPL3 = 1000; //����
    g_tVar.usDOL3 = 0; //����	
	
	//9---����ɼ�����
    g_tVar.uiPlsCntP = 0; //�������� 
    g_tVar.uiPlsCntN = 0; //�������� 
    g_tVar.ucALARM = 0; //������־
    g_tVar.ucPSMD = 3;    //����ģʽ	
    g_tVar.ucDEVADDR = 8; //�ⲿ�豸��ַ 
    g_tVar.ucDEVADDR2 = 1; //�ⲿ�����豸��ַ 	
    g_tVar.ucFSTYPE = HYCL; //�������ͺ�   
    g_tVar.ucSRCHN = 1; //ѹ��������ͨ�� 
    g_tVar.ucSRFMEN = 0; //ʹ����Ӵ���������  
#ifdef __RS485__	
    g_tVar.ucRTUPLS = 0; //�����������ʹ��	
#else
    g_tVar.ucRTUPLS = 1; //�����������ʹ��	
#endif
    g_tVar.ucPULRATIO = 2; //������� 	
    g_tVar.ucSENSTIME = 2; //������Ԥ��ʱ�� 	
    g_tVar.ucDTUDEV = 16; //���̴���
    g_tVar.usPLSCJ = 10; //ÿ10����������
	memset(g_tVar.caUPTIME, 5, 16);
    sprintf(g_tVar.caUPTIME, "0024"); //�ϱ�ʱ������		
	memset(g_tVar.caIMEI, 0, 16);
    sprintf(g_tVar.caIMEI, "000000000000000"); //IMEI��	
	
    sprintf(g_tVar.caCCID, "00000000000000000000"); //CCID��		

	memset(g_tVar.caDFOTP, 0, 34);
    sprintf(g_tVar.caDFOTP, "");  //�Զ���������	
	memset(g_tVar.caDFRGP, 0, 72);
    sprintf(g_tVar.caDFRGP, "");  //�Զ���ע���	
    g_tVar.ucTMSMD = 0;    //����Э��
    g_tVar.IDLDT = 0;    //��������ʱ��	 


    g_tVar.ucDEVADDR2 = 0; //�ⲿ�豸��ַ 	
    g_tVar.ucSVTYPE = 0; //���������� 
#ifdef __SL6200N__
	#ifdef __SLM130__
		g_tVar.ucMODULE = MD_SLM130; //ģ��
	#else
		g_tVar.ucMODULE = MD_BC26; //ģ��
	#endif
#else
		g_tVar.ucMODULE = MD_EC20; //ģ��
#endif	
	memset(g_tVar.caFileName, 0, 65);	
	g_tVar.uiFileSize = 0; //�ļ���С	
	g_tVar.uiFileOffset = 0; //�ļ�ƫ��
	
	g_tVar.ucUPDELAYEN = 0;
	
	g_tVar.ucPWRDY = 0;//VOUT_5V

	return var_WriteBytes((uint8_t *)&g_tVar.ucInit, 0, sizeof(VAR_T)); 	
}

int var_init(void)
{
	var_ReadBytes((uint8_t *)&g_tVar, 0, sizeof(g_tVar));
	if(g_tVar.ucInit != 0xAA)		
	{
		var_ReadBytes((uint8_t *)&g_tVar, 0, sizeof(g_tVar));
		if(g_tVar.ucInit != 0xAA)
		{
			g_tVar.uiTPIDX = 0;   //��������	
			g_tVar.uiSPIDX = 0;   //�ϱ�����		
			var_load();	
		}			
	}
	
	s_usIdxMax = sizeof(s_taCfg)/sizeof(CFG_T);	
	
    g_tVar.uiMAXIDX = 4096; //�������  MX25L12835F ��������  	
	
	return 1;
}

int var_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	return ee_WriteBytes(0, _pWriteBuf, _usAddress, _usSize);		
}

int var_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	return ee_ReadBytes(_pReadBuf, _usAddress, _usSize);	
}

