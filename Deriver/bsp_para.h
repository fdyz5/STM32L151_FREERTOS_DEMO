#ifndef _BSP_PARA_H_
#define _BSP_PARA_H_

typedef struct
{
	uint8_t mode; 				//工作模式
	uint8_t devAddr;			//设备子号
	uint8_t devNum;				//设备子号数量
	uint8_t devAddrAsc[12];		//设备编号
	
	uint16_t ai1Up;				//模拟量1上限
	uint16_t ai1Down;			//模拟量1下限
	
	uint16_t ai2Up;				//模拟量2上限
	uint16_t ai2Down;			//模拟量2下限
	
	uint16_t ai3Up;				//模拟量3上限
	uint16_t ai3Down;			//模拟量3下限
	
	uint8_t ai1Addr;			//模拟量1部件地址
	uint8_t ai2Addr;			//模拟量2部件地址
	uint8_t ai3Addr;			//模拟量3部件地址
	
	uint8_t ai1Enable;			//模拟量1使能
	uint8_t ai2Enable;			//模拟量2使能
	uint8_t ai3Enable;			//模拟量3使能
	
	uint8_t diAddr;				//开关量部件地址
	
	uint16_t rptTime;			//上报时间
	
	uint64_t baudRate;			//串口波特率
	uint64_t dataBit;			//串口停止位
	uint64_t parity;			//串口校验位
	uint64_t stopBit;			//串口停止位
	
	uint8_t ip[16];				//服务器IP地址
	uint16_t port;				//服务器端口号
	
	uint8_t di1;
	uint8_t di2;

}PARA_TYPEDEF;

extern PARA_TYPEDEF para_t;

void bsp_para_init(void);

void bsp_para_default(void);
uint8_t bsp_para_set(const char *cmd, uint8_t* buf_out);
uint8_t bsp_para_get(const char* describe, const char *cmd, uint8_t* buf_out);

#endif
