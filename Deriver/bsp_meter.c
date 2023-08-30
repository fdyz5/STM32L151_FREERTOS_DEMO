#include "bsp.h"


#define METER_COM		 COM3
#define METER_TMR_ID	(TMR2)

static uint8_t steps     = 0x00;
static uint8_t bytes     = 0x00;
static uint8_t first     = 0x00;
static uint8_t overTimes = 0x00;

static uint8_t rBuffer[256];

static void bsp_meter_write(uint8_t addr,uint8_t fuc,uint16_t reg,uint16_t regs,uint8_t *pBuf,uint8_t len);
static void bsp_meter_read(uint8_t addr,uint8_t fuc,uint16_t reg,uint16_t regs);
static uint16_t bsp_meter_modbus_crc16(uint8_t *pData,uint16_t nLen);

int16_t bsp_meter_set_data(uint8_t addr,uint8_t fuc,uint16_t reg,uint16_t regs,uint8_t *pBuf,uint8_t len)
{
	uint8_t ucData;
	uint16_t i;
	uint16_t crc;
	uint16_t crc1;
	
	while(1)
	{
		switch(steps)
		{
			//初始化参数
			case 0:
			{
				memset(rBuffer,0,sizeof(rBuffer));
				overTimes = 0;
				steps      = 1;
			}
			break;
			
			//发送指令
			case 1:
			{
				bsp_meter_write(addr,fuc,reg,regs,pBuf,len);
				
				//开启定时器
				bsp_timer_star_once(METER_TMR_ID, 3000);
				first = 0;
				steps = 2;
				bytes = 0;
			}
			break;
			
			//等待接收数据
			case 2:
			{
				if (bsp_timer_check(METER_TMR_ID))
				{
					if(first == 0)
					{
						overTimes++;
						/* 超时 */
						printf("METER_COM recieve overtime:%d\r\n",overTimes);
						if(overTimes >= 3)
						{
							steps = 0;
							
							return -1;
						}
						else
						{
							steps = 1;
						}
					}
					else
					{
						steps = 0;
						
						printf("\r\nMETER_COM recieve data[%d]:",bytes);
						for(i=0; i<bytes; i++)
						{
							printf("%02X ",rBuffer[i]);
						}
						printf("\r\n");
						
						//设备地址
						if(rBuffer[0] == addr)
						{	
							//功能码
							if(rBuffer[1] == fuc)
							{
								crc   = bsp_meter_modbus_crc16(rBuffer,bytes-2);
								crc1  = rBuffer[bytes-1] << 8 | rBuffer[bytes-2];
								if(crc == crc1)
								{
									steps = 0;
									
									//接收完成
									return 0;
								}
								else
								{
									printf("CRC16 err，value:%04X\r\n",crc);
									overTimes++;
									if(overTimes >= 3)
									{
										steps = 0;
										return -1;
									}
									else
									{
										steps = 1;
									}
								}
							}
							else
							{
								printf("function err，value:%02X\r\n",fuc);
							}
						}
						else
						{
							printf("device addr err，value:%02X\r\n",addr);
						}
					}
				}
				
				if(comGetChar(METER_COM, &ucData))
				{
					//开启定时功能，字节与字节之间的时间为20ms；
					//超出20ms，认为已接收满一帧数据
				  
					rBuffer[bytes++] = ucData;
					bsp_timer_star_once(METER_TMR_ID, 20);
					first = 1;
				}
			}
			break;
		}
		
		bsp_iwdg_clear();
	}
}

int16_t bsp_meter_get_data(uint8_t addr,uint8_t fuc,uint16_t reg,uint16_t regs,uint8_t *pBuf)
{
	uint8_t ucData;
	uint16_t i;
	uint16_t crc;
	uint16_t crc1;
	uint8_t len;
	
	while(1)
	{
		switch(steps)
		{
			//初始化参数
			case 0:
			{
				memset(rBuffer,0,sizeof(rBuffer));
				overTimes = 0;
				steps      = 1;
			}
			break;
			
			//发送指令
			case 1:
			{
				bsp_meter_read(addr,fuc,reg,regs);
				
				//开启定时器
				bsp_timer_star_once(METER_TMR_ID, 3000);
				first = 0;
				steps = 2;
				bytes = 0;
			}
			break;
			
			//等待接收数据
			case 2:
			{
				if (bsp_timer_check(METER_TMR_ID))
				{
					if(first == 0)
					{
						overTimes++;
						/* 超时 */
						printf("METER_COM recieve overtime:%d\r\n",overTimes);
						if(overTimes >= 3)
						{
							steps = 0;
							
							return -1;
						}
						else
						{
							steps = 1;
						}
					}
					else
					{
						steps = 0;
						
						printf("\r\nMETER_COM recieve data[%d]:",bytes);
						for(i=0; i<bytes; i++)
						{
							printf("%02X ",rBuffer[i]);
						}
						printf("\r\n");
						
						//设备地址
						if(rBuffer[0] == addr)
						{	
							//功能码
							if(rBuffer[1] == fuc)
							{
								crc   = bsp_meter_modbus_crc16(rBuffer,bytes-2);
								crc1  = rBuffer[bytes-1] << 8 | rBuffer[bytes-2];
								if(crc == crc1)
								{
									steps = 0;

									len = rBuffer[2];
									for(i=0; i<len; i++)
									{
										pBuf[i] = rBuffer[3+i];
									}					
									//接收完成
									return len;
								}
								else
								{
									printf("CRC16 err，value:%04X\r\n",crc);
									overTimes++;
									if(overTimes >= 3)
									{
										steps = 0;
										return -1;
									}
									else
									{
										steps = 1;
									}
								}
							}
							else
							{
								printf("function err，value:%02X\r\n",fuc);
							}
						}
						else
						{
							printf("device addr err，value:%02X\r\n",addr);
						}
					}
				}
				
				if(comGetChar(METER_COM, &ucData))
				{
					//开启定时功能，字节与字节之间的时间为20ms；
					//超出20ms，认为已接收满一帧数据
				  
					rBuffer[bytes++] = ucData;
					bsp_timer_star_once(METER_TMR_ID, 20);
					first = 1;
				}
			}
			break;
		}
		
		bsp_iwdg_clear();
	}
}

int16_t bsp_meter_get_addr(void)
{
	static uint8_t buf[8];
	
	uint8_t ucData;
	uint16_t crc;
	uint16_t crc1;
	
	while(1)
	{
		switch(steps)
		{
			//初始化参数
			case 0:
			{
				memset(buf,0,sizeof(buf));
				overTimes = 0;
				steps     = 1;
			}
			break;
			
			//发送指令
			case 1:
			{
				bsp_meter_read(0x00,0x03,0x0000,0x0001);
				
				//开启定时器
				bsp_timer_star_once(METER_TMR_ID, 3000);
				first = 0;
				steps = 2;
				bytes = 0;
				memset(buf,0,sizeof(buf));
			}
			break;
			
			//等待接收数据
			case 2:
			{
				if (bsp_timer_check(METER_TMR_ID))
				{
					if(first == 0)
					{
						overTimes++;
						/* 超时 */
						printf("METER_COM recieve overtime:%d\r\n",overTimes);
						if(overTimes >= 3)
						{
							steps = 0;
							
							return -1;
						}
						else
						{
							steps = 1;
						}
					}
					else
					{
						//接收完成
						
						crc   = bsp_meter_modbus_crc16(buf,bytes-2);
						crc1  = buf[bytes-1] << 8 | buf[bytes-2];
						if(crc == crc1)
						{
							steps = 0;
							return buf[0];
						}
						else
						{
							printf("CRC16 err，value:%04X\r\n",crc);
							overTimes++;
							if(overTimes >= 3)
							{
								steps = 0;
								return -1;
							}
							else
							{
								steps = 1;
							}
						}
					}
				}
				
				if(comGetChar(METER_COM, &ucData))
				{
					//开启定时功能，字节与字节之间的时间为20ms；
					//超出20ms，认为已接收满一帧数据
				  
					buf[bytes++] = ucData;
					bsp_timer_star_once(METER_TMR_ID, 20);
					first = 1;
				}
			}
			break;
		}
		
		bsp_iwdg_clear();
	}
}

static void bsp_meter_read(uint8_t addr,uint8_t fuc,uint16_t reg,uint16_t regs)
{
	uint8_t cmd[8];
	uint16_t crc16;
	uint8_t i;
	
	//设备地址
	cmd[0] = addr;
	//功能码
	cmd[1] = fuc;
	//寄存器地址
	cmd[2] = (reg >> 8) & 0xff;
	cmd[3] = (reg >> 0) & 0xff;
	//寄存器数量
	cmd[4] = (regs >> 8) & 0xff;
	cmd[5] = (regs >> 0) & 0xff;
	//CRC16校验
	crc16  = bsp_meter_modbus_crc16(cmd,6);
	cmd[6] = (crc16 >> 0) & 0xff;
	cmd[7] = (crc16 >> 8) & 0xff;
	
	printf("\r\nMETER_COM send data:");
	for(i=0; i<8; i++)
	{
		printf("%02X ",cmd[i]);
	}
	printf("\r\n");
	
	comSendBuf(METER_COM,cmd,8);
}

static void bsp_meter_write(uint8_t addr,uint8_t fuc,uint16_t reg,uint16_t regs,uint8_t *pBuf,uint8_t len)
{
	uint8_t cmd[256];
	uint16_t crc16;
	uint8_t i;
	uint8_t index = 0;
	
	//设备地址
	cmd[index++] = addr;
	//功能码
	cmd[index++] = fuc;
	//寄存器地址
	cmd[index++] = (reg >> 8) & 0xff;
	cmd[index++] = (reg >> 0) & 0xff;
	//寄存器数量
	cmd[index++] = (regs >> 8) & 0xff;
	cmd[index++] = (regs >> 0) & 0xff;
	
	//字节数
	cmd[index++] = len;
	
	memcpy(&cmd[index],pBuf,len);
	index += len;
	//CRC16校验
	crc16  = bsp_meter_modbus_crc16(cmd,index);
	cmd[index++] = (crc16 >> 0) & 0xff;
	cmd[index++] = (crc16 >> 8) & 0xff;
	
	printf("\r\nMETER_COM send data:");
	for(i=0; i<index; i++)
	{
		printf("%02X ",cmd[i]);
	}
	printf("\r\n");
	
	comSendBuf(METER_COM,cmd,index);
}

static uint16_t bsp_meter_modbus_crc16(uint8_t *pData,uint16_t nLen)
{
	uint16_t temp  = 0xffff;
	uint16_t temp1 = 0;
	uint16_t i     = 0;
	uint16_t j     = 0;
	
	for(i=0; i<nLen; i++)
	{
		temp ^= *(pData+i);
		for(j=0; j<8; j++)
		{
			temp1  = temp;
			temp >>= 1;
			if(temp1&0x0001)
			{
				temp^=0xa001;
			}
		}
	}
	
	return(temp);
}
