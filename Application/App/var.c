#include "app.h"

#define COM_CFG   COM1

VAR_T  g_tVar;
uint16_t g_Reg[96];
uint16_t s_usIdxMax;
uint8_t g_ucUpdata = 0;
uint16_t g_usErr = 1,g_usRst = 1;
const char *cpSTVS="SL_XF01_230710T";

double g_fUTC,g_flatitude,g_flongitude;

const char WT[][20] = {"实时在线模式", "短信息模式", "MODEM模式", ""};
const char AUTO[][20] = {"关闭", "打开", "", ""};
const char SVMD[][20] = {"多中心模式", "主备中心模式", "", ""};
const char RPST[][20] = {"关闭", "打开", "", ""};
const char BAD[][20] = {"600", "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"};
const char CHC[][20] = {"无效验", "偶效验", "奇效验", ""};
const char MSVEN[][20] = {"关闭", "开启", "", ""};
const char SRTP[][20] = {"", "IP", "域名解析", "", ""};
const char CNTP[][20] = {"", "TCP", "UDP", "COAP", "LwM2M", "AEP"};
const char SMSVEN1[][20] = {"关闭", "开启", "", ""};
const char SRTP1[][20] = {"", "IP", "DNS", "", ""};
const char CNTP1[][20] = {"", "TCP", "UDP", "", ""};
const char SMSVEN2[][20] = {"关闭", "开启", "", ""};	
const char SRTP2[][20] = {"", "IP", "DNS", "", ""};
const char CNTP2[][20] = {"", "TCP", "UDP", "", ""};
const char TMSMD[][20] = {"标准", "自定义", "协议一", "协议二", "新天"};
const char BAD2[][20] = {"600", "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"};
const char CHC2[][20] = {"无效验", "偶效验", "奇效验", ""};
const char SRAEN[][20] = {"关闭", "开启", "", ""};
const char STSEN[][20] = {"关闭", "开启", "", ""};
const char RTUPLS[][20] = {"否", "是"};
const char PSMD[][20] = {"","单脉冲", "双脉冲", "三脉冲", "正反双脉冲","普通双脉冲","ZH脉冲"};
const char UPRTUEN[][20] = {"关闭", "开启", "", ""};
const char UPUMTEN[][20] = {"关闭", "开启", "", ""};
const char SLEEPEN[][20] = {"关闭", "开启", "", ""};
const char SRFMEN[][20] = {"关闭", "开启", "", ""};
const char UPDELAYEN[][20] = {"关闭", "开启", "", ""};
const char PWRDY[][20] = {"5V", "24V", "12V", "5V"};
const char AI2MODE[][20] = {"4mA-20mA", "0mA-20mA", "4mA-20mA"};
const char AI1MODE[][20] =  {"0.5V-4.5V", "0V-5V", "0V-4.7V", "1.0V-5.0V"};
const char PULRATIO[][20] = {"0.1", "1", "10", "100", "1000", "参数有误"};
#ifdef __RS485__
const char FSTYPE[][20] = {"IFC300", "H200B", "新天_803_V1", "新天_803_V2", "新天_MBUS_XT", "SCL_61", \
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

// s:字符串	b:BCD  u:整数 p:脉冲底数					   
const CFG_T  s_taCfg[]=
{
	
{0, 0, 's', 0, "*************************************\r\n"}, 	
{0, 0, 's', 0, "               参数列表\r\n"}, 
{0, 0, 's', 0, "version:SL_XF01_V1.0_230710T\r\n"}, 	
{0, 0, 's', 0, "*************************************\r\n\r\n"}, 

	
{0, 0, 's', 0, "\r\n1---模块参数设置\r\n"}, 
{(uint8_t *)&g_tVar.caID[0], 10, 's', "+ID", "ID号", 0},
{(uint8_t *)&g_tVar.caPHO[0], 16, 's', "+PHO", "SIM卡号", 0},
{(uint8_t *)&g_tVar.ucWT, 1, 'u', "+WT", "工作方式", (char *)&WT},
{(uint8_t *)&g_tVar.ucAUTO, 1, 'u', "+AUTO", "工作方式自动切换开关", (char *)&AUTO},
{(uint8_t *)&g_tVar.ucRPST, 1, 'u', "+WT", "DTU工作状态报告", (char *)&WT},
{(uint8_t *)&g_tVar.ucSVMD, 1, 'u', "+SVMD", "服务器模式", (char *)&SVMD},

{0, 0, 's', 0, "\r\n1---用户串口设置\r\n"},
{0, 0, 's', 0, "\r\n-->串口1:\r\n"},
{(uint8_t *)&g_tVar.ucBAD, 1, 'u', "+BAD", "波特率", (char *)&BAD}, 
{(uint8_t *)&g_tVar.ucDAB, 1, 'u', "+DAB", "数据位", 0}, 
{(uint8_t *)&g_tVar.ucCHC, 1, 'u', "+CHC", "效验位", (char *)&CHC},
{(uint8_t *)&g_tVar.ucSTO, 1, 'u', "+STO", "停止位", 0},
{0, 0, 's', 0, "\r\n-->串口2:\r\n"},
{(uint8_t *)&g_tVar.ucBAD2, 1, 'u', "+BAD2", "波特率", (char *)&BAD2}, 
{(uint8_t *)&g_tVar.ucDAB2, 1, 'u', "+DAB2", "数据位", 0}, 
{(uint8_t *)&g_tVar.ucCHC2, 1, 'u', "+CHC2", "效验位", (char *)&CHC2},
{(uint8_t *)&g_tVar.ucSTO2, 1, 'u', "+STO2", "停止位", 0},

{0, 0, 's', 0, "\r\n2---数据中心设置\r\n"},
{(uint8_t *)&g_tVar.ucMSVEN, 1, 'u', "+MSVEN", "应用中心使能", (char *)&MSVEN},
{(uint8_t *)&g_tVar.ucSRTP, 1, 'u', "+SRTP", "数据中心类型", (char *)&SRTP},
{(uint8_t *)&g_tVar.caIP[0], 16, 's', "+IP", "IP地址", 0},
{(uint8_t *)&g_tVar.caDNS[0], 33, 's', "+DNS", "主域名", 0},
{(uint8_t *)&g_tVar.usPORT, 2, 'u', "+PORT", "端口", 0},
{(uint8_t *)&g_tVar.ucCNTP, 1, 'u', "+CNTP", "网络协议", (char *)&CNTP},
{(uint8_t *)&g_tVar.caDNS1IP[0], 16, 's', "+DNSIP1", "DNS1IP地址", 0},
{(uint8_t *)&g_tVar.caDNS2IP[0], 16, 's', "+DNSIP2", "DNS2IP地址", 0},

{0, 0, 's', 0, "\r\n3---扩展中心1设置\r\n"},
{(uint8_t *)&g_tVar.ucSMSVEN1, 1, 'u', "+SMSVEN1", "应用中心使能", (char *)&SMSVEN1},
{(uint8_t *)&g_tVar.ucSSRTP1, 1, 'u', "+SRTP1", "数据中心类型", (char *)&SRTP1},
{(uint8_t *)&g_tVar.caSIP1[0], 16, 's', "+SIP1", "IP地址", 0},
{(uint8_t *)&g_tVar.caSDNS1[0], 33, 's', "+SDNS1", "主域名", 0},
{(uint8_t *)&g_tVar.usSPORT1, 2, 'u', "+SPORT1", "端口", 0},
{(uint8_t *)&g_tVar.ucCNTP1, 1, 'u', "+CNTP1", "网络协议", (char *)&CNTP1},

{0, 0, 's', 0, "\r\n4---扩展中心2设置\r\n"},
{(uint8_t *)&g_tVar.ucSMSVEN2, 1, 'u', "+SMSVEN2", "应用中心使能", (char *)&SMSVEN2},
{(uint8_t *)&g_tVar.ucSSRTP2, 1, 'u', "+SRTP2", "数据中心类型", (char *)&SRTP2},
{(uint8_t *)&g_tVar.caSIP2[0], 16, 's', "+SIP2", "IP地址", 0},
{(uint8_t *)&g_tVar.caSDNS2[0], 33, 's', "+SDNS2", "主域名", 0},
{(uint8_t *)&g_tVar.usSPORT2, 2, 'u', "+SPORT2", "端口", 0},
{(uint8_t *)&g_tVar.ucCNTP2, 1, 'u', "+CNTP2", "网络协议", (char *)&CNTP2},

{0, 0, 's', 0, "\r\n5---通信设置\r\n"},
{(uint8_t *)&g_tVar.usMTDN, 2, 'u', "+MTDN", "最大传输字节数", 0}, 
{(uint8_t *)&g_tVar.usOHRT, 2, 'u', "+OHRT", "心跳报告时间间隔", 0},
{(uint8_t *)&g_tVar.usRCTT, 2, 'u', "+RCTT", "重连时间间隔", 0}, 
{(uint8_t *)&g_tVar.usLPIT, 2, 'u', "+LPIT", "最后包空闲时间", 0},
{(uint8_t *)&g_tVar.usSWTT, 2, 'u', "+SWTT", "短信息工作时间", 0},

{0, 0, 's', 0, "\r\n6---服务中心设置\r\n"},
{(uint8_t *)&g_tVar.caAPN[0], 33, 's', "+APN", "接入点名称", 0},
{(uint8_t *)&g_tVar.caUN[0], 17, 's', "+UN", "用户名", 0},
{(uint8_t *)&g_tVar.caUP[0], 17, 's', "+UP", "密码", 0},
{(uint8_t *)&g_tVar.caSRCD[0], 17, 's', "+SRCD", "服务代码", 0},

{0, 0, 's', 0, "\r\n7---RTU参数\r\n"},
{(uint8_t *)&g_tVar.ucaMETID[0], 6, 'b', "+METID", "表号", 0}, 
{(uint8_t *)&g_tVar.ucaDTUSN[0], 6, 'b', "+DTUSN", "设备地址", 0},
{(uint8_t *)&g_tVar.usTPITV, 2, 'u', "+TPITV", "采集间隔", 0}, 
{(uint8_t *)&g_tVar.usSPITV, 2, 'u', "+SPITV", "上报间隔", 0}, 
{(uint8_t *)&g_tVar.ucUPRTUEN, 1, 'u', "+UPRTUEN", "主动上报RTU数据", (char *)&UPRTUEN}, 
{(uint8_t *)&g_tVar.ucUPUMTEN, 1, 'u', "+UPUMTEN", "主动上报大用户表数据", (char *)&UPUMTEN}, 
{(uint8_t *)&g_tVar.ucSLEEPEN, 1, 'u', "+SLEEPEN", "休眠使能", (char *)&SLEEPEN}, 

{0, 0, 's', 0, "\r\n8---模拟通道设置\r\n"},
{0, 0, 's', 0, "\r\n--->第一路\r\n"},
{(uint8_t *)&g_tVar.ucAI1MODE, 1, 'u', "+AI1MODE",  "信号类型", (char *)&AI1MODE}, 
{(uint8_t *)&g_tVar.usRGL1, 2, 'u', "+RGL1", "量程最小值", 0}, 
{(uint8_t *)&g_tVar.usRGB1, 2, 'u', "+RGB1", "量程最大值", 0}, 
{(uint8_t *)&g_tVar.usUPL1, 2, 'u', "+UPL1", "上限", 0}, 
{(uint8_t *)&g_tVar.usDOL1, 2, 'u', "+DOL1", "下限", 0},
{0, 0, 's', 0, "\r\n--->第二路\r\n"},
{(uint8_t *)&g_tVar.ucAI2MODE, 1, 'u', "+AI2MODE",  "信号类型", (char *)&AI2MODE}, 
{(uint8_t *)&g_tVar.usRGL2, 2, 'u', "+RGL2", "量程最小值", 0}, 
{(uint8_t *)&g_tVar.usRGB2, 2, 'u', "+RGB2", "量程最大值", 0}, 
{(uint8_t *)&g_tVar.usUPL2, 2, 'u', "+UPL2", "上限", 0}, 
{(uint8_t *)&g_tVar.usDOL2, 2, 'u', "+DOL2", "下限", 0},
{0, 0, 's', 0, "\r\n--->第三路\r\n"},
{(uint8_t *)&g_tVar.ucAI3MODE, 1, 'u', "+AI3MODE",  "信号类型", 0}, 
{(uint8_t *)&g_tVar.usRGL3, 2, 'u', "+RGL3", "量程最小值", 0}, 
{(uint8_t *)&g_tVar.usRGB3, 2, 'u', "+RGB3", "量程最大值", 0}, 
{(uint8_t *)&g_tVar.usUPL3, 2, 'u', "+UPL3", "上限", 0}, 
{(uint8_t *)&g_tVar.usDOL3, 2, 'u', "+DOL3", "下限", 0},

{0, 0, 's', 0, "\r\n9---脉冲采集设置\r\n"},
{(uint8_t *)&g_tVar.uiPlsCntP, 4, 'p', "+PLSKP", "正向脉冲", 0}, 
{(uint8_t *)&g_tVar.uiPlsCntN, 4, 'p', "+PLSKP", "反向脉冲", 0},
{(uint8_t *)&g_tVar.ucALARM, 1, 'u', "+ALARM",  "报警标志", 0},
{(uint8_t *)&g_tVar.ucPSMD, 1, 'u', "+PSMD", "脉冲模式", (char *)&PSMD}, 
{(uint8_t *)&g_tVar.ucDEVADDR, 1, 'u', "+DEVADDR", "外部设备地址", 0},
{(uint8_t *)&g_tVar.ucDEVADDR2, 1, 'u', "+DEVADDR2", "外部备用设备地址", 0},
#ifdef __RS485__
{(uint8_t *)&g_tVar.ucFSTYPE, 1, 'u', "+FSTYPE", "流量计型号", (char *)&FSTYPE},
#endif
{(uint8_t *)&g_tVar.ucSRCHN, 1, 'u', "+SRCHN", "压力传感器通道", 0},
{(uint8_t *)&g_tVar.ucSRFMEN, 1, 'u', "+SRFMEN", "使能外接串口流量计", (char *)&SRFMEN},
{(uint8_t *)&g_tVar.ucRTUPLS, 1, 'u', "+RTUPLS", "板载脉冲计数使能", (char *)&RTUPLS},
{(uint8_t *)&g_tVar.ucPULRATIO, 1, 'u', "+PULRATIO", "脉冲板变比", (char *)&PULRATIO},
{(uint8_t *)&g_tVar.ucSENSTIME, 1, 'u', "+SENSTIME", "传感器预热时间", 0},
{(uint8_t *)&g_tVar.ucDTUDEV, 1, 'u', "+DTUDEV", "厂商代码", 0},
{(uint8_t *)&g_tVar.usPLSCJ, 2, 'u', "+PLSCJ", "每10立方脉冲数", 0},
{(uint8_t *)&g_tVar.caUPTIME[0], 5, 's', "+UPTIME", "上报时间区间", 0},
{(uint8_t *)&g_tVar.caIMEI[0], 16, 's', "+IMEI", "IMEI号", 0},
{(uint8_t *)&g_tVar.caCCID[0], 21, 's', "+ICCID", "ICCID号", 0},

{0, 0, 's', 0, "\r\n10---扩展设置\r\n"},
{(uint8_t *)&g_tVar.ucTMSMD, 1, 'u', "+TMSMD", "传输协议", (char *)&TMSMD},
{(uint8_t *)&g_tVar.caDFOTP[0], 34, 's', "+DFOTP", "自定义心跳包", 0},
{(uint8_t *)&g_tVar.caDFRGP[0], 72, 's', "+DFRGP", "自定义注册包", 0},
{(uint8_t *)&g_tVar.uiTPIDX, 4, 'u', "+TPIDX", "采样索引", 0},
{(uint8_t *)&g_tVar.uiSPIDX, 4, 'u', "+SPIDX", "上报索引", 0},
{(uint8_t *)&g_tVar.uiMAXIDX, 4, 'u', "+MAXIDX", "最大索引", 0},
{(uint8_t *)&g_tVar.ucUPDELAYEN, 1, 'u', "+UPDELAYEN", "上报延迟", (char *)&UPDELAYEN},
{(uint8_t *)&g_tVar.ucPWRDY, 1, 'u', "+PWRDY", "输出电压", (char *)&PWRDY},

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
		else if(strstr(cmd, "+STVS" ) != NULL) //版本
		{
			sprintf(buf,"%s", cpSTVS);						
		}
		else if(strstr(cmd, "+RST" ) != NULL) //复位
		{					
			NVIC_SystemReset();
		}
		else if(strstr(cmd, "+TIME=?" ) != NULL) //读取时钟
		{
			bsp_rtc_get_time();	
			sprintf(buf,"\r\nTIME:%04d-%02d-%02d %02d:%02d:%02d\r\n", 
							  g_tRTC.Year,g_tRTC.Mon,g_tRTC.Day,g_tRTC.Hour,g_tRTC.Min,g_tRTC.Sec);				                  		
		}
		else if((p = strstr(cmd, "+TIME=" )) != NULL) //同步时钟
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
		else if(strstr(cmd, "+LOAD" ) != NULL)  //复位默认配置
		{
			if(var_load() == 1)
			{
				sprintf(buf,"OK\r\n");	
			}							
		}
		else if(strstr(cmd, "+CHKMD" ) != NULL) //自检信息 
		{
			g_ucRptTrig = 1;
			g_ucAqtTrig = 1;					
			sprintf(buf,"OK\r\n");						
		}
		else if(strstr(cmd, "+CLRFH" ) != NULL) //清除FLASH 
		{
			uint32_t addr;
			
			g_tVar.uiTPIDX = 0;   //采样索引	
			addr = OFFSET_OF(VAR_T, uiTPIDX);
			var_WriteBytes((uint8_t *)&g_tVar.uiTPIDX, addr, 4); 	
			var_ReadBytes((uint8_t *)&g_tVar.uiTPIDX, addr, 4); 					
			g_tVar.uiSPIDX = 0;   //上报索引	
			addr = OFFSET_OF(VAR_T, uiSPIDX);
			var_WriteBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
			var_ReadBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4);  
			comSendBuf(COM_CFG, (uint8_t *)"OK\r\n", 4);

			sf_EraseChip();
			sprintf(buf,"OK\r\n");							
		}				
		else if(strstr(cmd, "+OLTST" ) != NULL) //触发上报
		{
			g_ucRptTrig = 1;
			g_ucAqtTrig = 1;
			
			sprintf(buf,"OK\r\n");						
		}
		//else if((strstr(cmd, "+AI" ) != NULL) &&(strstr(cmd, "+AI1MODE" ) == NULL)) //避免指令 和+AI1MODE冲突触发采集
		else if((strcmp(cmd, "YN+AI\r\n") == 0)||(strcmp(cmd, "YN+AI") == 0))	//避免指令 和+AI开头的指令发生冲突，触发采集
		{
			g_ucAqtTrig = 1;
			sprintf(buf,"OK\r\n");						
		}		
		else if(strstr(cmd, "+READDATA" ) != NULL) //触发采集
		{
			g_ucAqtTrig = 1;
			sprintf(buf,"OK\r\n");						
		}				
		else if(strstr(cmd, "+SETIMEI" ) != NULL) //自动设置SIM卡号
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
						if(*p == '?') //读
						{
							sprintf(buf, "%s:", s_taCfg[idx].cmd+1);					
							if(s_taCfg[idx].type == 'b') //BCD类型
							{	
								bcd_to_str(s_taCfg[idx].addr, buf+strlen(buf), s_taCfg[idx].size);		
							}
							else if(s_taCfg[idx].type == 's') //字符串类型
							{
								sprintf(buf+strlen(buf), "%s", s_taCfg[idx].addr); 														
							}
							else if(s_taCfg[idx].type == 'p') //脉冲类型
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
						else //写
						{
							if(s_taCfg[idx].type == 'b') //BCD类型
							{
								str_to_bcd(p, s_taCfg[idx].addr, s_taCfg[idx].size*2);								
							}
							else if(s_taCfg[idx].type == 's') //字符串类型
							{
								q = strstr(cmd, "\r\n");
								if(q != NULL) *q = 0; 							
								memset(s_taCfg[idx].addr, 0, s_taCfg[idx].size);
								memcpy(s_taCfg[idx].addr, p, strlen(p));													
							}
							else if(s_taCfg[idx].type == 'p') //脉冲类型
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
			if(s_taCfg[idx].type == 'b') //BCD类型
			{	
				bcd_to_str(s_taCfg[idx].addr, buf+strlen(buf), s_taCfg[idx].size);		
			}
			else if(s_taCfg[idx].type == 's') //字符串类型
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
	//  初始化标志
    g_tVar.ucInit = 0xAA; 
//    g_tVar.uiTPIDX = 0;   //采样索引	
//    g_tVar.uiSPIDX = 0;   //上报索引
    g_tVar.uiMAXIDX = 4096;  //最大索引  MX25L12835F 扇区数量	
	
	//  1---模块参数设置
	memset(g_tVar.caID, 0, 10);
    sprintf(g_tVar.caID, "03710001"); //ID号 
	memset(g_tVar.caPHO, 0, 16);	
    sprintf(g_tVar.caPHO, "00000000000"); //SIM卡号  
    g_tVar.ucWT = 0; //工作方式 	
    g_tVar.ucAUTO = 0; //工作方式自动切换开关
    g_tVar.ucRPST = 1; //DTU工作状态报告
    g_tVar.ucSVMD = 0; //服务器模式	
	
	//  2---用户串口设置	
	//串口1	
    g_tVar.ucBAD = 4; //波特率 
    g_tVar.ucDAB = 8; //数据位
    g_tVar.ucCHC = 0; //校验位
    g_tVar.ucSTO = 1; //停止位	
	//串口2	
    g_tVar.ucBAD2 = 4; //波特率 
    g_tVar.ucDAB2 = 8; //数据位
    g_tVar.ucCHC2 = 0; //校验位
    g_tVar.ucSTO2 = 1; //停止位 	

	//  3---数据中心设置	
    g_tVar.ucMSVEN = 1;    //应用中心使能	
    g_tVar.ucSRTP = 1;    //数据中心类型

	memset(g_tVar.caIP, 0, 16);
//    sprintf(g_tVar.caIP, "110.87.107.225");//IP地址   
//    g_tVar.usPORT = 7270;   //IP端口 sokit测试

//    sprintf(g_tVar.caIP, "114.115.136.216");//IP地址   
//    g_tVar.usPORT = 8809;   //IP端口 小表系统集中器TCP
	
//    sprintf(g_tVar.caIP, "114.115.136.216");//IP地址   
//    g_tVar.usPORT = 8808;   //IP端口 小表系统大表RTU	

//    sprintf(g_tVar.caIP, "114.115.136.216");//IP地址   
//    g_tVar.usPORT = 6666;   //IP端口 小表系统NB表UDP 	
	
//    sprintf(g_tVar.caIP, "114.115.202.57");//IP地址   
//    g_tVar.usPORT = 6690;   //IP端口 大表平台UDP 	

//    sprintf(g_tVar.caIP, "60.205.5.172");//IP地址  京源 
//    g_tVar.usPORT = 7089;   //IP端口 大表平台UDP 

//    sprintf(g_tVar.caIP, "182.61.28.205");//IP地址 新开普 
//    g_tVar.usPORT = 19002;   //IP端口  TCP	
	
	
//    sprintf(g_tVar.caIP, "180.101.147.115");//IP地址   
//    g_tVar.usPORT = 5683;   //IP端口  电信iot平台 

    sprintf(g_tVar.caIP, "114.115.136.216");//IP地址   
    g_tVar.usPORT = 6690;   //IP端口 物联网大数据云服务平台 UDP 	

	memset(g_tVar.caDNS, 0, 33);
    sprintf(g_tVar.caDNS, "www.xmyn.com");  //主域名  
#ifdef __SL6200N__
    g_tVar.ucCNTP = 4; //网络传输协议 1=TCP 2=UDP 3=COAP  4=LwM2M  5=AEP 6=云平台	
#else
    g_tVar.ucCNTP = 2; //网络传输协议 1=TCP 2=UDP 3=COAP  4=LwM2M  5=AEP 6=云平台
#endif
	memset(g_tVar.caDNS1IP, 0, 16);
    sprintf(g_tVar.caDNS1IP, "211.138.151.161");  //DNS1IP地址	
	memset(g_tVar.caDNS2IP, 0, 16);
    sprintf(g_tVar.caDNS2IP, "103.44.145.245");  //DNS2IP地址	
	
    //  4---扩展中心设置
	//    --->扩展中心1:
    g_tVar.ucSMSVEN1 = 1;    //应用中心使能	
    g_tVar.ucSSRTP1 = 1;    //数据中心类型	
	memset(g_tVar.caSIP1, 0, 16);
    sprintf(g_tVar.caSIP1, "114.115.136.216"); //IP地址	
	memset(g_tVar.caSDNS1, 0, 33);
    sprintf(g_tVar.caSDNS1, "www.xmyn.com");  //主域名   
    g_tVar.usSPORT1 = 8001; //端口	
    g_tVar.ucCNTP1 = 2; //网络传输协议	
	//    --->扩展中心2:
    g_tVar.ucSMSVEN2 = 0;    //应用中心使能	
    g_tVar.ucSSRTP2 = 1;    //数据中心类型	
	memset(g_tVar.caSIP2, 0, 16);
    sprintf(g_tVar.caSIP2, "0.0.0.0"); //IP地址	
	memset(g_tVar.caSDNS2, 0, 33);
    sprintf(g_tVar.caSDNS2, "www.xmyn.com");  //主域名   
    g_tVar.usSPORT2 = 7010; //端口	
    g_tVar.ucCNTP2 = 2; //网络传输协议		
	
	//  5---通信设置
    g_tVar.usMTDN = 1024; //最大传输字节数 
    g_tVar.usOHRT = 60; //心跳报告时间间隔  
    g_tVar.usRCTT = 3; //重连时间间隔  
    g_tVar.usLPIT = 60; //最后包空闲时间  
    g_tVar.usSWTT = 0; //短信息工作时间
	
	//  6---服务中心设置 
	memset(g_tVar.caAPN, 0, 33);
    sprintf(g_tVar.caAPN, "CMNET"); //接入点名称		
	memset(g_tVar.caUN, 0, 17);
    sprintf(g_tVar.caUN, ""); //用户名
	memset(g_tVar.caUP, 0, 17);
    sprintf(g_tVar.caUP, ""); //密码
	memset(g_tVar.caSRCD, 0, 17);
    sprintf(g_tVar.caSRCD, "*98#"); //服务代码
	memset(g_tVar.caSMSC1, 0, 21);
    sprintf(g_tVar.caSMSC1, "");  //管理员号码

	//7---RTU参数
	g_tVar.ucaMETID[0] = 0x00; //表地址
	g_tVar.ucaMETID[1] = 0x00; //表地址    
	g_tVar.ucaMETID[2] = 0x00; //表地址    
	g_tVar.ucaMETID[3] = 0x00; //表地址
	g_tVar.ucaMETID[4] = 0x00; //表地址
	g_tVar.ucaMETID[5] = 0x00; //表地址 

    g_tVar.ucaDTUSN[0] = 0x00;//设备地址(BCD)
    g_tVar.ucaDTUSN[1] = 0x00;//设备地址(BCD)    
    g_tVar.ucaDTUSN[2] = 0x00;//设备地址(BCD) 
    g_tVar.ucaDTUSN[3] = 0x00;//设备地址(BCD)    
    g_tVar.ucaDTUSN[4] = 0x00;//设备地址(BCD)
    g_tVar.ucaDTUSN[5] = 0x00;//设备地址(BCD)

	
    g_tVar.usTPITV = 7200; //采样间隔 
    g_tVar.usSPITV = 480; //上报间隔
   
    g_tVar.ucUPRTUEN= 1;//主动上报RTU数据  
    g_tVar.ucUPUMTEN = 0;//主动上报大用户表数据  
    g_tVar.ucSLEEPEN = 1;//休眠使能 

	//  8---自定义模式参数
	//--->第一路	
    g_tVar.ucAI1MODE = 0; //信号类型
    g_tVar.usRGL1 = 0; //最小量程
    g_tVar.usRGB1 = 1000; //最大量程 
    g_tVar.usUPL1 =1000; //上限
    g_tVar.usDOL1 = 0; //下限  
	//--->第二路		
    g_tVar.ucAI2MODE = 0; //信号类型
    g_tVar.usRGL2 = 0; //最小量程
    g_tVar.usRGB2= 1000; //最大量程 
    g_tVar.usUPL2 = 1000; //上限
    g_tVar.usDOL2 = 0; //下限
	//--->第三路		
    g_tVar.ucAI3MODE = 0; //信号类型
    g_tVar.usRGL3 = 0; //最小量程
    g_tVar.usRGB3= 1000; //最大量程 
    g_tVar.usUPL3 = 1000; //上限
    g_tVar.usDOL3 = 0; //下限	
	
	//9---脉冲采集设置
    g_tVar.uiPlsCntP = 0; //正向脉冲 
    g_tVar.uiPlsCntN = 0; //反向脉冲 
    g_tVar.ucALARM = 0; //报警标志
    g_tVar.ucPSMD = 3;    //脉冲模式	
    g_tVar.ucDEVADDR = 8; //外部设备地址 
    g_tVar.ucDEVADDR2 = 1; //外部备用设备地址 	
    g_tVar.ucFSTYPE = HYCL; //流量计型号   
    g_tVar.ucSRCHN = 1; //压力传感器通道 
    g_tVar.ucSRFMEN = 0; //使能外接串口流量计  
#ifdef __RS485__	
    g_tVar.ucRTUPLS = 0; //板载脉冲计数使能	
#else
    g_tVar.ucRTUPLS = 1; //板载脉冲计数使能	
#endif
    g_tVar.ucPULRATIO = 2; //脉冲板变比 	
    g_tVar.ucSENSTIME = 2; //传感器预热时间 	
    g_tVar.ucDTUDEV = 16; //厂商代码
    g_tVar.usPLSCJ = 10; //每10立方脉冲数
	memset(g_tVar.caUPTIME, 5, 16);
    sprintf(g_tVar.caUPTIME, "0024"); //上报时间区间		
	memset(g_tVar.caIMEI, 0, 16);
    sprintf(g_tVar.caIMEI, "000000000000000"); //IMEI号	
	
    sprintf(g_tVar.caCCID, "00000000000000000000"); //CCID号		

	memset(g_tVar.caDFOTP, 0, 34);
    sprintf(g_tVar.caDFOTP, "");  //自定义心跳包	
	memset(g_tVar.caDFRGP, 0, 72);
    sprintf(g_tVar.caDFRGP, "");  //自定义注册包	
    g_tVar.ucTMSMD = 0;    //传输协议
    g_tVar.IDLDT = 0;    //空闲下线时间	 


    g_tVar.ucDEVADDR2 = 0; //外部设备地址 	
    g_tVar.ucSVTYPE = 0; //服务器类型 
#ifdef __SL6200N__
	#ifdef __SLM130__
		g_tVar.ucMODULE = MD_SLM130; //模组
	#else
		g_tVar.ucMODULE = MD_BC26; //模组
	#endif
#else
		g_tVar.ucMODULE = MD_EC20; //模组
#endif	
	memset(g_tVar.caFileName, 0, 65);	
	g_tVar.uiFileSize = 0; //文件大小	
	g_tVar.uiFileOffset = 0; //文件偏移
	
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
			g_tVar.uiTPIDX = 0;   //采样索引	
			g_tVar.uiSPIDX = 0;   //上报索引		
			var_load();	
		}			
	}
	
	s_usIdxMax = sizeof(s_taCfg)/sizeof(CFG_T);	
	
    g_tVar.uiMAXIDX = 4096; //最大索引  MX25L12835F 扇区数量  	
	
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

