#include "bsp.h"

#define PARA_DISP_TIME	1000

PARA_TYPEDEF para_t;

const uint8_t Month_Table[12][3] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

static uint8_t bsp_StrHex2Int8(char* str_dec);
//static uint16_t bsp_StrToIntArray(char* str_buf,char* array);
static uint32_t  bsp_StrDecToHex(char* str_dec, uint8_t cnt);
static int16_t bsp_asc_to_bcd(const uint8_t *asc, uint8_t *bcd, uint16_t len);

static char * bsp_GetDateIDE(void);
static void bsp_disp_version(void);
static void bsp_strsubr(char* str);

static void bsp_para_write(uint32_t addr,uint8_t *buf,uint16_t size);
static void bsp_para_read(uint32_t addr,uint8_t *buf,uint16_t size);

/* 参数类型 */
typedef enum 
{ 
	PARA_STRING = 1, 
	PARA_HEX, 
	PARA_INT 
} PARA_MODE;      


struct CONFIG_CMD_T
{
	PARA_MODE mode; 		//参数类型
    const char* describe;	//描述
    const char* cmd;		//命令
    const char* def; 		//默认参数
    uint16_t addr;		  	//存储地址
    uint8_t size;			//数据长度
};


#define CONFIG_NUM 		25

const struct CONFIG_CMD_T Setup_pro[CONFIG_NUM] =
{
	{ PARA_INT,	    "工作模式",		"+MODE=",    	"3",              1,   1 },
	{ PARA_INT,		"上报时间",	    "+RPT=", 		"120",            2,   2 },
	{ PARA_STRING,  "设备编号",    	"+DEV=",  	 	"123456789000",	  4,   12},
    { PARA_STRING,	"服务器IP",    	"+IP=",  	 	"101.37.22.223",  16,  16},
    { PARA_INT,	    "服务器端口",   "+PORT=",  	 	"5009",	          32,  2 },
    { PARA_INT,	    "通道1上限",    "+CHL1UP=",     "4096",           34,  2 },
    { PARA_INT,	    "通道1下限",    "+CHL1DN=",     "0",              36,  1 },
    { PARA_INT,	    "通道2上限",    "+CHL2UP=",     "4096",           37,  2 },
    { PARA_INT,	    "通道2下限",    "+CHL2DN=",     "0",              39,  1 },
    { PARA_INT,	    "通道3上限",    "+CHL3UP=",     "4096",           40,  2 },
    { PARA_INT,	    "通道3下限",    "+CHL3DN=",     "0",              42,  1 },
    { PARA_INT,	    "通道1部件地址","+CHL1ADDR=",   "1",              43,  1 },
    { PARA_INT,	    "通道2部件地址","+CHL2ADDR=",   "2",              44,  1 },
    { PARA_INT,	    "通道3部件地址","+CHL3ADDR=",   "3",              45,  1 },
    { PARA_INT,	    "开关量部件地址","+DIADDR=",    "1",              46,  1 },
	{ PARA_INT,	    "通道1使能",		"+AI1=",        "1",              48,  1 },
	{ PARA_INT,	    "通道2使能",		"+AI2=",        "1",              49,  1 },
	{ PARA_INT,	    "通道3使能",		"+AI3=",        "1",              50,  1 },
	{ PARA_INT,	    "设备子号",		"+DEVID=",    	"1",              51,  1 },
	{ PARA_INT,	    "波特率",     	"+BAUD=",  		"4",	     	  52,  1 },
	{ PARA_INT,	    "数据位",     	"+DABT=",  		"8",	     	  53,  1 },
	{ PARA_INT,	    "校验位",     	"+CHCK=",  		"0",	     	  54,  1 },
	{ PARA_INT,	    "停止位",     	"+STOP=",  		"1",	     	  55,  1 },
	{ PARA_INT,	    "子号数量",		"+DEVIDS=",    	"1",              56,  1 },
};


void bsp_para_init(void)
{
	uint8_t first;
	uint8_t buf[30];
	uint64_t temp;
	uint8_t i;
    
	printf("\r\n/***********************************/\r\n");
	bsp_disp_version();
	printf("****************************************/\r\n");
	
    bsp_para_read(0,&first,1);
    if(first != 0xaa)
    {
        bsp_para_default();
        first = 0xaa;
        bsp_para_write(0,&first,1);
    }
	
	memset(buf,0,sizeof(buf));
    bsp_para_get("-->work mode","+MODE=",(uint8_t *)&para_t.mode);
	bsp_timer_delay_ms(PARA_DISP_TIME);
	
	switch(para_t.mode)
	{
		//模式1-国标
		case 1:
		{
			printf("---------Chinese Standard\r\n");
			
			memset(buf,0,sizeof(buf));
			bsp_para_get("-->device number","+DEV=",(uint8_t *)buf);
			temp = atoll((char *)&buf[0]);
			for(i=0; i<6; i++)
			{
				para_t.devAddrAsc[i] = (temp >> (i*8)) & 0xff;
			}
			bsp_timer_delay_ms(PARA_DISP_TIME);
			
			memset(buf,0,sizeof(buf));
			bsp_para_get("-->ai1 addr","+CHL1ADDR=",(uint8_t *)&para_t.ai1Addr);
			bsp_timer_delay_ms(PARA_DISP_TIME);
			
			memset(buf,0,sizeof(buf));
			bsp_para_get("-->ai2 addr","+CHL2ADDR=",(uint8_t *)&para_t.ai2Addr);
			bsp_timer_delay_ms(PARA_DISP_TIME);
			
			memset(buf,0,sizeof(buf));
			bsp_para_get("-->ai3 addr","+CHL3ADDR=",(uint8_t *)&para_t.ai3Addr);
			bsp_timer_delay_ms(PARA_DISP_TIME);
			
			memset(buf,0,sizeof(buf));
			bsp_para_get("-->di addr","+DIADDR=",(uint8_t *)&para_t.diAddr);
			bsp_timer_delay_ms(PARA_DISP_TIME);
		}
		break;
		
		//模式2-自定义【RS485】
		case 2:
		{
			printf("---------user-defined for RS485\r\n");
			
			memset(buf,0,sizeof(buf));
			bsp_para_get("-->device number","+DEV=",(uint8_t *)para_t.devAddrAsc);
			bsp_timer_delay_ms(PARA_DISP_TIME);
			
			bsp_para_get("-->device id","+DEVID=",(uint8_t *)&para_t.devAddr);
			if(para_t.devAddr <= 1)
			{
				para_t.devAddr = 1;
			}
			bsp_timer_delay_ms(PARA_DISP_TIME);
			
			bsp_para_get("-->device ids","+DEVIDS=",(uint8_t *)&para_t.devNum);
			if((para_t.devNum <= 1) || (para_t.devNum > 32))
			{
				para_t.devNum = 1;
			}
			bsp_timer_delay_ms(PARA_DISP_TIME);
			
			bsp_para_get("-->baudrate","+BAUD=",(uint8_t *)&buf[0]);
			switch(buf[0])
			{
				case 0:para_t.baudRate = 1200;break;
				case 1:para_t.baudRate = 2400;break;
				case 2:para_t.baudRate = 3600;break;
				case 3:para_t.baudRate = 4800;break;
				case 4:para_t.baudRate = 9600;break;
				case 5:para_t.baudRate = 19200;break;
				case 6:para_t.baudRate = 38400;break;
				case 7:para_t.baudRate = 57600;break;
				case 8:para_t.baudRate = 115200;break;
				default:para_t.baudRate = 9600;break;
			}
			printf("Explain:0-1200  1-2400  2-3600  3-4800   4-9600\r\n \
			                5-19200 6-38400 7-57600 8-115200\r\n");
			bsp_timer_delay_ms(PARA_DISP_TIME);
			
			bsp_para_get("-->data bit","+DABT=",(uint8_t *)&buf[0]);
			switch(buf[0])
			{
				case 8:para_t.dataBit = USART_WordLength_8b;break;
				case 9:para_t.dataBit = USART_WordLength_9b;break;
				default:para_t.dataBit = USART_WordLength_8b;break;
			}
			printf("Explain:8-8bit 9-9bit\r\n");
			bsp_timer_delay_ms(PARA_DISP_TIME);
			
			bsp_para_get("-->parity","+CHCK=",(uint8_t *)&buf[0]);
			switch(buf[0])
			{
				case 0:para_t.parity  = USART_Parity_No;break;
				case 1:para_t.parity  = USART_Parity_Even;break;
				case 2:para_t.parity  = USART_Parity_Odd;break;
				default:para_t.parity = USART_Parity_No;break;
			}
			printf("Explain:0-none	1-even 2-odd\r\n");
			bsp_timer_delay_ms(PARA_DISP_TIME);
			
			bsp_para_get("-->stop bit","+STOP=",(uint8_t *)&buf[0]);
			switch(buf[0])
			{
				case 1:para_t.stopBit  = USART_StopBits_1;break;
				case 0:para_t.stopBit  = USART_StopBits_0_5;break;
				case 2:para_t.stopBit  = USART_StopBits_2;break;
				case 3:para_t.stopBit  = USART_StopBits_1_5;break;
				default:para_t.stopBit = USART_StopBits_1;break;
			}
			printf("Explain:0-0.5bit 1-1bit 2-2bit 3-1.5bit\r\n");
			bsp_timer_delay_ms(PARA_DISP_TIME);
		}
		break;
		
		//模式3-自定义【ADC】
		case 3:
		{
			printf("---------user-defined for ADC\r\n");
			
			memset(buf,0,sizeof(buf));
			bsp_para_get("-->device number","+DEV=",(uint8_t *)para_t.devAddrAsc);
			bsp_timer_delay_ms(PARA_DISP_TIME);
			
			bsp_para_get("-->device id","+DEVID=",(uint8_t *)&para_t.devAddr);
			if(para_t.devAddr <= 1)
			{
				para_t.devAddr = 1;
			}
			bsp_timer_delay_ms(PARA_DISP_TIME);
		}
		break;
		
		default:
		{
			printf("-->mode err\r\n");
		}
		break;
	}
	
	memset(buf,0,sizeof(buf));
	bsp_para_get("-->ai1 enable","+AI1=",(uint8_t *)&para_t.ai1Enable);
	bsp_timer_delay_ms(PARA_DISP_TIME);
	if(para_t.ai1Enable == 1)
	{
		memset(buf,0,sizeof(buf));
		bsp_para_get("-->ai1 up value","+CHL1UP=",(uint8_t *)buf);
		para_t.ai1Up = buf[1] << 8 | buf[0];
		bsp_timer_delay_ms(PARA_DISP_TIME);
		
		memset(buf,0,sizeof(buf));
		bsp_para_get("-->ai1 down value","+CHL1DOWN=",(uint8_t *)buf);
		para_t.ai1Down = buf[1] << 8 | buf[0];
		bsp_timer_delay_ms(PARA_DISP_TIME);
	}
	
	memset(buf,0,sizeof(buf));
	bsp_para_get("-->ai2 enable","+AI2=",(uint8_t *)&para_t.ai2Enable);
	bsp_timer_delay_ms(PARA_DISP_TIME);
	if(para_t.ai2Enable == 1)
	{
		memset(buf,0,sizeof(buf));
		bsp_para_get("-->ai2 up value","+CHL2UP=",(uint8_t *)buf);
		para_t.ai2Up = buf[1] << 8 | buf[0];
		bsp_timer_delay_ms(PARA_DISP_TIME);
		
		memset(buf,0,sizeof(buf));
		bsp_para_get("-->ai2 down value","+CHL2DOWN=",(uint8_t *)buf);
		para_t.ai2Down = buf[1] << 8 | buf[0];
		bsp_timer_delay_ms(PARA_DISP_TIME);
	}
	
	memset(buf,0,sizeof(buf));
	bsp_para_get("-->ai3 enable","+AI3=",(uint8_t *)&para_t.ai3Enable);
	bsp_timer_delay_ms(PARA_DISP_TIME);
	if(para_t.ai3Enable == 1)
	{
		memset(buf,0,sizeof(buf));
		bsp_para_get("-->ai3 up value","+CHL3UP=",(uint8_t *)buf);
		para_t.ai3Up = buf[1] << 8 | buf[0];
		bsp_timer_delay_ms(PARA_DISP_TIME);
		
		memset(buf,0,sizeof(buf));
		bsp_para_get("-->ai3 down value","+CHL3DOWN=",(uint8_t *)buf);
		para_t.ai3Down = buf[1] << 8 | buf[0];
		bsp_timer_delay_ms(PARA_DISP_TIME);
	}

    memset(buf,0,sizeof(buf));
    bsp_para_get("-->report time","+RPT=",(uint8_t *)&buf);
	para_t.rptTime = buf[1] << 8 | buf[0];
	bsp_timer_delay_ms(PARA_DISP_TIME);
    
    memset(buf,0,sizeof(buf));
    bsp_para_get("-->service ip","+IP=",(uint8_t *)buf);
    memcpy(para_t.ip,buf,strlen((char *)buf));
	bsp_timer_delay_ms(PARA_DISP_TIME);
	
    memset(buf,0,sizeof(buf));
    bsp_para_get("-->service port","+PORT=",(uint8_t *)buf);
    para_t.port = (buf[1] << 8) | buf[0];
	bsp_timer_delay_ms(PARA_DISP_TIME);

	printf("/***********************************/\r\n");
}

uint8_t bsp_para_set(const char *cmd, uint8_t* buf_out)
{
    uint8_t i = 0;
	uint32_t temp;
	uint8_t buf[10];
	uint8_t stringlen;
	
	if((cmd == NULL) || (strlen(cmd) == 0))
    {
        return 0;
    }

    for(i=0; i<CONFIG_NUM; i++)
    {
		if(strcmp(cmd, Setup_pro[i].cmd) == 0)
        {
            switch(Setup_pro[i].mode)
			{
				case PARA_STRING:
				{
					if(strlen((char *)buf_out) < Setup_pro[i].size)
					{
						strcat((char *)buf_out, "\r");
					}
					
					stringlen = strlen((char *)buf_out);
					if((stringlen%2) != 0)
					{
						stringlen += 1;
					}
					bsp_para_write(Setup_pro[i].addr,(uint8_t *)buf_out,stringlen);
					printf("%s:%s\r\n", Setup_pro[i].describe, buf_out);
				}
				break;
				
				case PARA_HEX:
				{
					bsp_para_write(Setup_pro[i].addr,(uint8_t *)buf_out,Setup_pro[i].size);
					
					printf("%s:", Setup_pro[i].describe);
					for(i=0; i<Setup_pro[i].size; i++)
					{
						printf(" %X", buf_out[i]);
					}
					printf("\r\n");
				}
				break;
				
				case PARA_INT:
				{
					temp = atoi((char *)buf_out);
					
					switch(Setup_pro[i].size)
					{
						case 1:
						{
							buf[0] = (temp >> 0) & 0xff;
						}
						break;
						
						case 2:
						{
							buf[1] = (temp >> 8) & 0xff;
							buf[0] = (temp >> 0) & 0xff;
						}
						break;
						
						case 4:
						{
							buf[3] = (temp >> 24) & 0xff;
							buf[2] = (temp >> 16) & 0xff;
							buf[1] = (temp >> 8)  & 0xff;
							buf[0] = (temp >> 0)  & 0xff;
						}
						break;
						
						default:
							break;
					}
					
					bsp_para_write(Setup_pro[i].addr,(uint8_t *)buf,Setup_pro[i].size);
					printf("%s:%d\r\n", Setup_pro[i].describe, temp);
				}
				break;
			}

            return i+1;
        }
    }

    return 0;
}

uint8_t bsp_para_get(const char* describe, const char *cmd, uint8_t* buf_out)
{
    uint8_t i = 0;
	uint32_t temp;
    const char * idstr = NULL;
    char * des_string   = NULL;

    if((cmd == NULL) || (strlen(cmd) == 0))
    {
        idstr = describe;
    }
    else
    {
        idstr = cmd;
    }
 

    if((idstr == NULL) || (strlen(idstr) == 0))
    {
        return 0;
    }

    for(i=0; i<CONFIG_NUM; i++)
    {
        if((cmd == NULL ) || (strlen(cmd) == 0))
        {
            des_string = (char *) Setup_pro[i].describe;
        }
        else
        {
            des_string = (char *)Setup_pro[i].cmd;
        }

        if(strcmp( idstr, des_string) == 0)
        {
            switch(Setup_pro[i].mode)
			{
				case PARA_STRING:
				{
					bsp_para_read(Setup_pro[i].addr,(uint8_t *)buf_out,Setup_pro[i].size);
					bsp_strsubr((char *)buf_out);
					printf("%s:%s\r\n", describe, buf_out);
				}
				break;
				
				case PARA_HEX:
				{
					bsp_para_read(Setup_pro[i].addr,(uint8_t *)buf_out,Setup_pro[i].size);
					
					printf("%s:", describe);
					for(i=0; i<Setup_pro[i].size; i++)
					{
						printf(" %X", buf_out[i]);
					}
					printf("\r\n");
				}
				break;
				
				case PARA_INT:
				{
					bsp_para_read(Setup_pro[i].addr,(uint8_t *)buf_out,Setup_pro[i].size);
					switch(Setup_pro[i].size)
					{
						case 1:
						{
							temp = *buf_out;
						}
						break;
						
						case 2:
						{
							temp = (*(buf_out + 1) << 8) | *(buf_out + 0);
						}
						break;
						
						case 4:
						{
							temp = ((*(buf_out + 3) << 24) + (*(buf_out + 2) << 16) + (*(buf_out + 1) << 8) | *(buf_out + 0));
						}
						break;
						
						default:
							break;
					}

					printf("%s:%d\r\n",describe, temp);
				}
				break;
			}

            return i+1;
        }
    }

    return 0;
}

void bsp_para_default(void)
{
	uint8_t i;
	char para[100];
	uint32_t temp;
	uint8_t stringlen;
	
	for(i=0; i<CONFIG_NUM; i++)
	{
		switch(Setup_pro[i].mode)
		{
			case PARA_STRING:
			{
				memset(para, 0, sizeof(para));
				strncpy(para, Setup_pro[i].def, strlen(Setup_pro[i].def));

				stringlen = strlen(para);
				if((stringlen%2) != 0)
				{
					stringlen += 1;
				}
                bsp_para_write(Setup_pro[i].addr,(uint8_t *)para,stringlen);
				
				printf("%s:%s\r\n", Setup_pro[i].describe, para);
			}
			break;
			
			case PARA_HEX:
			{
				bsp_StrToIntArray((char*)Setup_pro[i].def, para);
				bsp_para_write(Setup_pro[i].addr,(uint8_t *)para,Setup_pro[i].size);
				
				printf("%s:", Setup_pro[i].describe);
				for(i=0; i<Setup_pro[i].size; i++)
				{
					printf(" %X", para[i]);
				}
				printf("\r\n");
			}
			break;
			
			case PARA_INT:
			{
				temp = bsp_StrDecToHex((char*)Setup_pro[i].def,sizeof(Setup_pro[i].def));
                switch(Setup_pro[i].size)
                {
                    case 1:
					{
                        *para = ((uint8_t)temp);
					}
					break;
					
                    case 2:
					{
                        *(para + 1) = ((uint8_t)(temp >> 8));
                        *(para + 0) = ((uint8_t)(temp));
					}
					break;
					
                    case 4:
					{
                        *(para + 3) = ((uint8_t)(temp >> 24));
                        *(para + 2) = ((uint8_t)(temp >> 16));
                        *(para + 1) = ((uint8_t)(temp >> 8));
                        *(para + 0) = ((uint8_t)(temp));
					}
					break;
					
                    default:
                        break;
                }
				
                bsp_para_write(Setup_pro[i].addr,(uint8_t *)para,Setup_pro[i].size);
				
				printf("%s:%d\r\n", Setup_pro[i].describe, temp);
			}
			break;
		}
	}
}

static uint8_t bsp_StrHex2Int8(char* str_dec)
{
	uint8_t temp = 0;
	char buf_tmp[3];

	if(str_dec == NULL)
	{
		return 0;
	}

	buf_tmp[2] = '\0';
	buf_tmp[0] = str_dec[0];
	buf_tmp[1] = str_dec[1];
	temp       = strtol(buf_tmp, NULL, 16); 	

	return temp;
}

static uint32_t  bsp_StrDecToHex(char* str_dec, uint8_t cnt)
{
	uint8_t i     = 0;
	uint32_t temp = 0;

	if(str_dec == NULL)
	{
		return 0;
	}
	
	for(i=0; i<cnt; i++)
	{
		if((*(str_dec + i) > '9') || (*(str_dec + i) < '0'))
		{
			break;
		}
		
		temp *= 10;
		temp += (*(str_dec + i ) - '0');
	}

	return temp;
}


static void bsp_disp_version(void)
{
    char buf[27];
    
    memset(buf,0,sizeof(buf));
    
    strcpy(buf,"YN510L_");
    strcpy(buf+7,bsp_GetDateIDE());
    
    printf("Software compilation date:%s\r\n",buf);
}

static char * bsp_GetDateIDE(void)
{
    static char date[12];
    char* now = __DATE__; //格式为: Jul 7 2012
    uint8_t i = 0;

	strncpy(date, now + 7, 4); //year

	for(i = 0; i < 12; i++) //month
	{
		if(strncmp((char*)Month_Table[i], now, 3) == 0)
		{
			sprintf(date + 4, "%02d", i + 1);
		}
	}
	if(*(now + 4) == 0x20) //day
	{
		sprintf(date + 6, "0%c", *(now + 5));
	}
	else
	{
		sprintf(date + 6, "%c%c", *(now + 4), *(now + 5));
	}

    return date;
}

static uint16_t bsp_StrToIntArray(char* str_buf,char* array)
{
	uint16_t len=0;

	while(*str_buf)
	{
		*array++ = bsp_StrHex2Int8(str_buf);
		str_buf +=2;
		len ++;
		
	}

	return len;
}


static void bsp_strsubr(char* str)
{
    char* ptr;

    if(( ptr = strchr( str, '\r')) != NULL)
    {
        *ptr = '\0';
    }
}

static const uint8_t ascii2bcd1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};  
static const uint8_t ascii2bcd2[6]  = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};  

static int16_t bsp_asc_to_bcd(const uint8_t *asc, uint8_t *bcd, uint16_t len)  
{  
    uint8_t c = 0;  
    uint8_t index = 0;  
    uint8_t i = 0;    
      
    len >>= 1;      
  
    for(; i < len; i++) 
	{  
        //first BCD  
        if(*asc >= 'A' && *asc <= 'F') 
		{  
            index = *asc - 'A';   
            c     = ascii2bcd2[index] << 4;  
        } 
		else if(*asc >= '0' && *asc <= '9') 
		{  
            index = *asc - '0';  
            c     = ascii2bcd1[index] << 4;  
        }  
        asc++;  
  
        //second BCD  
        if(*asc >= 'A' && *asc <= 'F') 
		{  
            index = *asc - 'A';   
            c    |= ascii2bcd2[index];  
        } 
		else if(*asc >= '0' && *asc <= '9') 
		{  
            index = *asc - '0';  
            c    |= ascii2bcd1[index];  
        }  
        asc++;  
  
        *bcd++ = c;  
    }     
  
    return 0;  
}  

static void bsp_para_write(uint32_t addr,uint8_t *buf,uint16_t size)
{ 
	uint8_t ret;
	uint8_t times = 0;
	
	while(1)
	{
		ret = ee_WriteBytes(buf,addr,size);
		if(ret == 1)
		{
			break;
		}
		else
		{
			times++;
			if(times >= 3)
			{
				times = 0;
				break;
			}
		}
	}
}

static void bsp_para_read(uint32_t addr,uint8_t *buf,uint16_t size)
{
	uint8_t ret;
	uint8_t times = 0;
	
	while(1)
	{
		ret = ee_ReadBytes(buf,addr,size);
		if(ret == 1)
		{
			break;
		}
		else
		{
			times++;
			if(times >= 3)
			{
				times = 0;
				break;
			}
		}
	}
}



