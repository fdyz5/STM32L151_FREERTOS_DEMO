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
    H200B,//开封电磁流量计，H200B智能电磁流量计
    XT_803_V1,
    XT_803_V2,//新天803C新版协议
    XT_MBUS,
    SCL_61,
    SCL_70_1A,
    WT4200S,
    UR_1000, // 多声道超声流量计
    PRAIS_V18, // 浦瑞斯
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
	CJ188_LYG,//连云港RTU
	CJ188_DJ,//东贱RTU
	BJXR_HG702,	//旭日三合一监测仪
	BJXR_HARD,	//旭日硬度监测仪
	BJXR_WATER,//旭日水质监测仪
	MT,		//迈拓
	PRAIS, // 浦瑞斯
	CJ188_YG,	//RTU 甬港
	CJ188_XT,	//RTU新天
	NHKJ_TR,		//能慧科技土壤监测仪
	CJ188_YG_V1,//RTU 甬港
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
	V51C_ZJCY,//浙江超仪
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
	VC51_JLJLL,//净累计流量 武汉瑞兆丰
	VC51_WHRZF,//武汉瑞兆丰
	L_MAG, //北京京源
	UVC, //天健创新
	MQ_MODBUS, //超声波明渠流量计
	AM4_MODBUS, //ABB流量计
	SD123_I950,//SD123-I950 水表智能传感器	
	AFTU_2, //艾拓利尔流量计 深圳建恒
	DJ_WM01_NP, //DJ_WM01型超声波流量计， 不带内置压力  苏州东剑
	V51C_DAOYITECH, //大连道一科技   协议同V51_ZJCY
	MODBUS_NS, //宁波水表  南京智控自动化控制系统有限公司	
	DTFX1020, //迪纳生超声波流量计
	TP_MODBUS,
	MX39_34CR,
	BY_MODBUS,
	MLF_100H,
	V51C_DHZK,  //甘肃大河科技
	DYP_A07,	//液位传感器
	DJ_WM1034,	//超声波水表（带阀控）
	W803E,
	HG_NG,
	QDJC_MODBUS, //积成协议  客户：银润达定
	TDS100_MODBUS, //邢银超声波流量计
	CY_MFB,
	LXC_100,
	DTFX1020M,
	XJZD_MODBUS,
	XC_MODBUS,
	ULFM_MODBUS, //ULFM(A)超声波明渠流量计
	SM100B,
	MFE_S,
	SCL_60_HZ,
	SENSUS_HRI,
	TJZNC,
	SCL_60_HZ_YL,
	SCL_70_1A_YL,
	EMWM_MODBUS, //上海科洋  周星	
	FC002, //旋进涡轮流量计 【带温度+压力】
	EH6400A,//流量计电子计数器， 样表用的是通讯协议二
	WAVE_MODBUS, //超声波水表
	MODBUS_JYCS, //JYCS超声波水表，带反向
	CJ188_SZMT, //深圳敏泰水表
	SC_MODBUS, //三川流量计
	SCL_61D5_6, //汇中流量计+压力
	MT_CJ188, //边拓流量计
	MT_CJ188_YL, //边拓流量计+压力
	PRAIS_V18_YL,//浦瑞斯流量计+压力	
	CJ188_ZL,//真兰水表
	SGLDY_MODBUS,//上海上戈传感器技术有限公司
	L_MAG2,//
	XJZD2_MODBUS,//新疆中电科技【外置压力】
	MODBUS_HSAZ,//杭水爱知电磁水表
	HLY600_MODBUS,//上海汉研流量计转换器
	V51_MODBUS,//大连圣世援水表通讯协议
	MODBUS_TRQXY_D10,
	MODBUS_FLSD,//福罗思德电磁流量计	
	TLU_C_100_YL,//浙江天信仪表科技有限公司[外置压力]
	CJ188_DJ_HM05,//东剑超声波热量表, 北京同仁堂
	WT4300_MODBUS,
	WALLJ_MODBUS,
	MODBUS_WYYT,//温压一体变送器
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
	MODBUS_DPL, //多普勒流量计
	MODBUS_WWJ, //超声波物位计	
	DIEHL_AH_MODBUS,
	SCL_61D5_AH,
	MIK_P300, //压力变送器
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
    uint8_t *addr;  //地址    
    uint8_t size;   //字节 
    char    type;   //类型	
    char    *cmd;   //命令   
    char    *name;  //描述 
    void 	*list;  //列表    
} CFG_T;

#pragma pack(1) //强制字节对齐
typedef struct
{ 
	uint8_t  ucInit;    //初始化标志
	uint32_t uiTPIDX;   //采样索引	
	uint32_t uiSPIDX;   //上报索引
	uint32_t uiMAXIDX;  //最大索引
	
	//  1---模块参数设置	
    char  	 caID[10]; //ID号
	char     caPHO[16]; //SIM卡号	
    uint8_t  ucWT; //工作方式 	
    uint8_t  ucAUTO; //工作方式自动切换开关
    uint8_t  ucRPST; //DTU工作状态报告
    uint8_t  ucSVMD; //服务器模式	
	
	//  2---用户串口设置	
	//串口1	
    uint8_t  ucBAD; //波特率 
    uint8_t  ucDAB; //数据位
    uint8_t  ucCHC; //校验位
    uint8_t  ucSTO; //停止位	
	//串口2	
    uint8_t  ucBAD2; //波特率 
    uint8_t  ucDAB2; //数据位
    uint8_t  ucCHC2; //校验位
    uint8_t  ucSTO2; //停止位 	

	//  3---数据中心设置	
	uint8_t  ucMSVEN;    //应用中心使能	
	uint8_t  ucSRTP;    //数据中心类型	
	char     caIP[16];  //IP地址
	char     caDNS[33];  //主域名
    uint16_t usPORT;    //端口	
    uint8_t  ucCNTP; //网络传输协议TCP or UDP
	char     caDNS1IP[16];  //DNS1IP地址	
	char     caDNS2IP[16];  //DNS2IP地址		
	
    //  4---扩展中心设置
	//    --->扩展中心1:
	uint8_t  ucSMSVEN1;    //应用中心使能	
	uint8_t  ucSSRTP1;    //数据中心类型	
	char     caSIP1[16];  //IP地址
	char     caSDNS1[33];  //主域名
    uint16_t usSPORT1;    //端口	
    uint8_t  ucCNTP1; //网络传输协议	
	//    --->扩展中心2:
	uint8_t  ucSMSVEN2;    //应用中心使能	
	uint8_t  ucSSRTP2;    //数据中心类型	
	char     caSIP2[16];  //IP地址
	char     caSDNS2[33];  //主域名
    uint16_t usSPORT2;    //端口	
    uint8_t  ucCNTP2; //网络传输协议	
	
	//  5---通信设置
    uint16_t usMTDN; //最大传输字节数 
    uint16_t usOHRT; //心跳报告时间间隔  
    uint16_t usRCTT; //重连时间间隔  
    uint16_t usLPIT; //最后包空闲时间  
    uint16_t usSWTT; //短信息工作时间
	
	//  6---服务中心设置 	
    char  	 caAPN[33]; //接入点名称
    char  	 caUN[17]; //用户名
    char  	 caUP[17]; //密码
    char  	 caSRCD[17]; //服务代码	
    char  	 caSMSC1[21]; //管理员号码

	//7---RTU参数
	uint8_t  ucaMETID[6]; //表地址 
	uint8_t  ucaDTUSN[6];//设备地址
	uint16_t usTPITV;   //采样间隔	
	uint16_t usSPITV;   //上报间隔
	uint8_t  ucUPRTUEN;//主动上报RTU数据  
	uint8_t  ucUPUMTEN;//主动上报大用户表数据  
	uint8_t  ucSLEEPEN;//休眠使能 

	//  8---自定义模式参数
	//--->第一路	
    uint8_t  ucAI1MODE; //信号类型
    uint16_t usRGL1; //最小量程
    uint16_t usRGB1; //最大量程 
    uint16_t usUPL1; //上限
    uint16_t usDOL1; //下限  
	//--->第二路		
    uint8_t  ucAI2MODE; //信号类型
    uint16_t usRGL2; //最小量程
    uint16_t usRGB2; //最大量程 
    uint16_t usUPL2; //上限
    uint16_t usDOL2; //下限
	//--->第三路		
    uint8_t  ucAI3MODE; //信号类型
    uint16_t usRGL3; //最小量程
    uint16_t usRGB3; //最大量程 
    uint16_t usUPL3; //上限
    uint16_t usDOL3; //下限

	//9---脉冲采集设置
	uint32_t uiPlsCntP; //正向脉冲 
	uint32_t uiPlsCntN; //反向脉冲 
	uint32_t ucALARM; //报警标志
	uint8_t  ucPSMD;    //脉冲模式	
    uint8_t  ucDEVADDR; //外部设备地址 
    uint8_t  ucFSTYPE; //流量计型号   
    uint8_t  ucSRCHN; //压力传感器通道 
    uint8_t  ucSRFMEN; //使能外接串口流量计  	
    uint8_t  ucRTUPLS; //板载脉冲计数使能	
    uint8_t  ucPULRATIO; //脉冲板变比 	
    uint8_t  ucSENSTIME; //传感器预热时间 	
    uint8_t  ucDTUDEV; //厂商代码
    uint16_t usPLSCJ; //每10立方脉冲数	
    char  	 caUPTIME[5]; //上报时间区间	
	char     caIMEI[16]; //IMEI号

    char  	 caDFOTP[34]; //自定义心跳包
    char  	 caDFRGP[72]; //自定义注册包	
	uint8_t  ucTMSMD;    //传输协议
	uint8_t  IDLDT;    //空闲下线时间
	
	char     caCCID[21]; //CCID号
    uint8_t  ucDEVADDR2; //外部设备地址 	
    uint8_t  ucSVTYPE; 	 //服务器类型 
    uint8_t  ucMODULE;  //模组	
	
	char     caFileName[65]; //文件名
	uint32_t uiFileSize; //文件大小	
	uint32_t uiFileOffset; //文件偏移
	
	uint8_t  ucUPDELAYEN; //上报延迟使能
	uint8_t  ucPWRDY;     //VOUT电压	
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
