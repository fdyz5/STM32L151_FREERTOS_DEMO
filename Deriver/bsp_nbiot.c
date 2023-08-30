#include "bsp.h"

#define DBUG_NBIOT_EN  1

#define COM_NBIOT	    COM2		/* ѡ�񴮿� */

#define NBIOT_TMR_ID	(TMR1)

#if DBUG_NBIOT_EN
	#define DBUG_NBIOT(fmt,...)  printf(fmt,##__VA_ARGS__)
#else
	#define DBUG_NBIOT(fmt,...) 
#endif


#define INT_TO_BCD(x)	(x + ( x / 10) * 6)
#define BCD_TO_INT(x)	(x - (x >> 4) * 6)

static int8_t bsp_nbiot_WaitResponse(char *_Cmd, char *_pAckStr, uint16_t _usTimeOut);
static uint8_t bsp_nbiot_GetResponse(char *_Cmd, char *_pAckStr, uint16_t _usTimeOut);

static void bsp_nbiot_HexStrToByte(const char* source, unsigned char* dest, int sourceLen);

int8_t bsp_nbiot_link(void)
{
    uint8_t times  = 0;
	uint8_t step   = 0;
    uint8_t buf[250];
    char cmd[50];
    uint8_t len;
	char *ptr  = NULL;
	char *ptr1 = NULL;
	uint16_t temp;
    
	while(1)
	{
		switch(step)
		{
            case 0:
			{
				//����ģ��
				bsp_power_nb_off();
				bsp_timer_delay_ms(100);
				bsp_power_nb_on();
				bsp_timer_delay_ms(1000);
				
				//����ģ��
				bsp_nbiot_WaitResponse((char*)"AT+NRB",(char*)"OK\r\n",1000);
				
                step  = 1;
                times = 0;
			}
			break;
            
            case 1:
			{
				//����
				if(bsp_nbiot_WaitResponse((char*)"AT",(char*)"OK\r\n",1000) != -1)
				{
					step   = 3;
					times  = 0;
                    bsp_timer_delay_ms(100);
				}
				else
				{
                    bsp_timer_delay_ms(1000);
					times++;
					if(times >= 5)
					{
                        return -1;
					}
				}
			}
			break;
			
			case 2:
			{
				//����
				if(bsp_nbiot_WaitResponse((char*)"ATE0",(char*)"OK\r\n",1000) != -1)
				{
					step   = 3;
					times  = 0;
                    bsp_timer_delay_ms(100);
				}
				else
				{
                    bsp_timer_delay_ms(1000);
					times++;
					if(times >= 3)
					{
                        return -1;
					}
				}
			}
			break;
            
            case 3:
			{
				//��ȡ�̼��汾
				if(bsp_nbiot_WaitResponse((char*)"AT+CIMI",(char*)"OK\r\n",1000) != -1)
				{
					step  = 4;
					times = 0;
                    bsp_timer_delay_ms(1000);
				}
				else
				{
                    bsp_timer_delay_ms(1000);
					times++;
					if(times >= 5)
					{
						return -1;
					}
				}
			}
			break;
            
            case 4:
			{
				//��ȡ�̼��汾
				if(bsp_nbiot_WaitResponse((char*)"AT+CIMI",(char*)"OK\r\n",1000) != -1)
				{
					step  = 5;
					times = 0;
                    bsp_timer_delay_ms(1000);
				}
				else
				{
                    bsp_timer_delay_ms(1000);
					times++;
					if(times >= 5)
					{
						return -1;
					}
				}
			}
			break;
			
			case 5:
			{
				//��ȡ�̼��汾
				if(bsp_nbiot_WaitResponse((char*)"ATI",(char*)"OK\r\n",1000) != -1)
				{
					step  = 6;
					times = 0;
                    bsp_timer_delay_ms(1000);
				}
				else
				{
                    bsp_timer_delay_ms(1000);
					times++;
					if(times >= 5)
					{
						return -1;
					}
				}
			}
			break;
			
			case 6:
			{
                memset(buf,0,sizeof(buf));
				//��ѯ�̼��汾(����),ע��656SP2�汾��Ӧ�Ļ�վ�İ汾��Ҫ�����빦��
                len = bsp_nbiot_GetResponse("AT+CGSN=1",(char *)buf,1000);
                if(len > 0)
                {
                    DBUG_NBIOT("%s",buf);
                }
                step  = 7;
                times = 0;
                bsp_timer_delay_ms(100);
			}
			break;       
            
			case 7:
			{
				//�鿴Ƶ����Ϣ
                memset(buf,0,sizeof(buf));
                len = bsp_nbiot_GetResponse((char*)"AT+NBAND?",(char *)buf,1000);
                if(len > 0)
                {
                    DBUG_NBIOT("%s",buf);
                }
                step  = 8;
                times = 0;
                bsp_timer_delay_ms(100);             
			}
			break;

			case 8:
			{
                memset(buf,0,sizeof(buf));
				//��ѯ�̼��汾(����),ע��656SP2�汾��Ӧ�Ļ�վ�İ汾��Ҫ�����빦��
                len = bsp_nbiot_GetResponse("AT+CGMR",(char *)buf,1000);
                if(len > 0)
                {
                    DBUG_NBIOT("%s",buf);
                }
                step  = 9;
                times = 0;
                bsp_timer_delay_ms(100);
			}
			break;       
			
			case 9:
			{
				//��ȡCSQ
                memset(buf,0,sizeof(buf));
                len = bsp_nbiot_GetResponse("AT+CSQ",(char *)buf,2000);
                if(len > 0)
                {
                    DBUG_NBIOT("%s",buf);
					ptr = strstr((char *)buf,"+CSQ:");
					if(ptr != NULL)
					{
						ptr += 5;
						ptr1 = strstr((char *)ptr,",");
						if(ptr1 != NULL)
						{
							memset(cmd,0,sizeof(cmd));
							memcpy(cmd,ptr,ptr1-ptr);
							temp = atoi(cmd);
							if((temp < 99) && (temp >= 10))
							{
								step  = 10;
								times = 0;
								bsp_timer_delay_ms(1000);
							}
							else
							{
								bsp_timer_delay_ms(3000);
								times++;
								if(times >= 10)
								{
                                    step  = 13;
                                    times = 0;
								}
							}
						}
						else
						{
							bsp_timer_delay_ms(3000);
							times++;
							if(times >= 10)
							{
                                step  = 13;
                                times = 0;
							}
						}
						
					}
					else
					{
						bsp_timer_delay_ms(3000);
						times++;
						if(times >= 10)
						{
                            step  = 13;
                            times = 0;
						}
					}
                }
				else
				{
					bsp_timer_delay_ms(3000);
					times++;
					if(times >= 10)
					{
                        step  = 13;
                        times = 0;
					}
				}
                
			}
			break;       
            
            case 10:
			{
                if(bsp_nbiot_WaitResponse((char*)"AT+CGATT?",(char*)"+CGATT:1\r\n\r\nOK\r\n",3000) != -1)
                {
                    step   = 11;
                    times  = 0;
                    bsp_timer_delay_ms(1000);
                }
                else
                {
                    bsp_timer_delay_ms(5000);
                    times++;
                    if(times >= 10)
                    {
                        return -1;
                    }
                }
			}
			break;
            
            case 11:
			{
				//��ȡ��Ϣ
				if(bsp_nbiot_WaitResponse((char*)"AT+NUESTATS",(char*)"OK\r\n",5000) != -1)
				{
					times = 0;
                    step  = 12;
                    bsp_timer_delay_ms(2000);
				}
				else
				{
                    bsp_timer_delay_ms(1000);
                    
					times++;
					if(times >= 3)
					{
						step  = 13;
                        times = 0;
					}
				}
			}
			break;
            
            case 12:
            {
                //�����Զ���������
                if(bsp_nbiot_WaitResponse((char*)"AT+NPSMR=1",(char*)"\r\nOK\r\n",1000) != -1)
                {
                    step  = 0;
                    times = 0;
                    bsp_timer_delay_ms(1000);
                    
                    return 0;
                }
                else
                {
                    bsp_timer_delay_ms(1000);
                    times++;
                    if(times >= 3)
                    {
                        return -1;
                    }
                }
            }
            break;
            
            case 13:
			{
                if(bsp_nbiot_WaitResponse((char*)"AT+CFUN=0",(char*)"\r\nOK\r\n",3000) != -1)
                {
                    step   = 14;
                    times  = 0;
                    bsp_timer_delay_ms(1000);
                }
                else
                {
                    bsp_timer_delay_ms(5000);
                    times++;
                    if(times >= 3)
                    {
                        return -1;
                    }
                }
			}
			break;
			
			case 14:
			{
                if(bsp_nbiot_WaitResponse((char*)"AT+NCSEARFCN",(char*)"\r\nOK\r\n",3000) != -1)
                {
                    step   = 15;
                    times  = 0;
                    bsp_timer_delay_ms(1000);
                }
                else
                {
                    bsp_timer_delay_ms(5000);
                    times++;
                    if(times >= 10)
                    {
                        return -1;
                    }
                }
			}
			break;
            
            case 15:
			{
                if(bsp_nbiot_WaitResponse((char*)"AT+CFUN=1",(char*)"\r\nOK\r\n",3000) != -1)
                {
                    step   = 0;
                    times  = 0;
                    bsp_timer_delay_ms(1000);
                    
                    return -1;
                }
                else
                {
                    bsp_timer_delay_ms(5000);
                    times++;
                    if(times >= 10)
                    {
                        return -1;
                    }
                }
			}
			break;
		}
	}
}

int8_t bsp_nbiot_get_socket(void)
{
    char buf[250];
    uint8_t len;
    uint8_t n = 0;
    char *ptr;
	
	while(n < 3)
    {
        if(bsp_nbiot_WaitResponse("AT",(char*)"\r\nOK\r\n",1000) != -1)
        {
           break;
        }
        else
        {
            n++;
            if(n >= 3)
            {
                return -1;
            }
            bsp_timer_delay_ms(1000);
        }
    }
    
    bsp_nbiot_WaitResponse("AT+NSOCL=0",(char*)"\r\nOK\r\n",1000);
    
	n = 0;
    while(n < 3)
    {
        memset(buf,0,sizeof(buf));
        len = bsp_nbiot_GetResponse("AT+NSOCR=DGRAM,17,3005,1",buf,1000);
        if(len > 0)
        {
            printf("%s",buf);
            ptr = strstr(buf,"\r\n");
            if(ptr != NULL)
            {
                return (*(ptr+2)-'0');
            }
        }
        
        n++;
        
        bsp_timer_delay_ms(1000);
    }
    
    return -1;
}

int8_t bsp_nbiot_send(uint8_t n,uint8_t *sbuf,uint8_t len)
{
    char dBuf[1024];
    char tBuf[1024];
    uint8_t times = 0;
    uint8_t i;
    
    memset(dBuf,0,sizeof(dBuf));
    memset(tBuf,0,sizeof(tBuf));
    
    for(i=0; i<len; i++)
    {
        sprintf(dBuf+i*2,"%02X",*(sbuf+i));
    }
    
//    sprintf((char*)tBuf,"AT+NSOST=%d,%d.%d.%d.%d,%d,%d,%s",\
//            n,ip[0],ip[1],ip[2],ip[3],port,len,dBuf);  
    
    sprintf((char*)tBuf,"AT+NSOSTF=%d,%s,%d,0x200,%d,%s",\
            n,para_t.ip,para_t.port,len,dBuf);  
    
    while(times < 3)
    {
        if(bsp_nbiot_WaitResponse((char*)tBuf,(char*)"OK\r\n",3000) != -1)
        {
            return 0;
        }
        times++;
    }
    
    return -1;
}

int8_t bsp_nbiot_get_time(uint8_t *pbuf)
{
    uint8_t len;
    uint8_t buf[100];
    char *ptr;
    uint8_t n = 0;
    
    while(n < 3)
    {
        memset(buf,0,sizeof(buf));
        len = bsp_nbiot_GetResponse("AT+CCLK?",(char *)buf,1000);
        if(len > 0)
        {
            DBUG_NBIOT("%s",buf);
            if(strstr((char *)buf,"ERROR") == NULL)
            {
                ptr  = strstr((char *)buf,":");
                if(ptr != NULL)
                {
                    ptr += 1;
                    pbuf[0] = (*(ptr+0) - '0') * 10 + (*(ptr+1) - '0');
                    ptr += 3;
                    pbuf[1] = (*(ptr+0) - '0') * 10 + (*(ptr+1) - '0');
                    ptr += 3;
                    pbuf[2] = (*(ptr+0) - '0') * 10 + (*(ptr+1) - '0');
                    
                    ptr += 3;
                    pbuf[3] = (*(ptr+0) - '0') * 10 + (*(ptr+1) - '0');
                    
                    if(pbuf[3] <= 15)
                    {
                        pbuf[3] += 8;
                    }
                    else
                    {
                        pbuf[3] = pbuf[3] - 16;
                        switch(pbuf[1])
                        {
                            case 1:
                            case 3:
                            case 5:
                            case 7:
                            case 8:
                            case 10:
                            case 12:
                            {
                                if(pbuf[2] != 31)
                                {
                                    pbuf[2]++;
                                }
                                else
                                {
                                    pbuf[2] = 1;
                                    if(pbuf[1] == 12)
                                    {
                                        pbuf[1] = 1;
                                        pbuf[0]++;
                                    }
                                }
                            }
                            break;
                            
                            case 4:
                            case 6:
                            case 9:
                            case 11:
                            {
                                if(pbuf[2] != 30)
                                {
                                    pbuf[2]++;
                                }
                                else
                                {
                                    pbuf[2] = 1;
                                    if(pbuf[1] == 12)
                                    {
                                        pbuf[1] = 1;
                                        pbuf[0]++;
                                    }
                                }
                            }
                            break;
                            
                            case 2:
                            {
                                if(((pbuf[0]/4 == 0) && (pbuf[0]/100 != 0)) || (pbuf[0]/400 == 0))
                                {
                                    if(pbuf[2] != 29)
                                    {
                                        pbuf[2]++;
                                    }
                                    else
                                    {
                                        pbuf[2] = 1;
                                        if(pbuf[1] == 12)
                                        {
                                            pbuf[1] = 1;
                                            pbuf[0]++;
                                        }
                                    }
                                }
                                else
                                {
                                    if(pbuf[2] != 28)
                                    {
                                        pbuf[2]++;
                                    }
                                    else
                                    {
                                        pbuf[2] = 1;
                                        if(pbuf[1] == 12)
                                        {
                                            pbuf[1] = 1;
                                            pbuf[0]++;
                                        }
                                    }
                                }
                            }
                            break;
                        }    
                    }
                    
                    ptr += 3;
                    pbuf[4] = (*(ptr+0) - '0') * 10 + (*(ptr+1) - '0');
                    ptr += 3;
                    pbuf[5] = (*(ptr+0) - '0') * 10 + (*(ptr+1) - '0');
                    
//                    pbuf[0] = INT_TO_BCD(pbuf[0]);
//                    pbuf[1] = INT_TO_BCD(pbuf[1]);
//                    pbuf[2] = INT_TO_BCD(pbuf[2]);
//                    pbuf[3] = INT_TO_BCD(pbuf[3]);
//                    pbuf[4] = INT_TO_BCD(pbuf[4]);
//                    pbuf[5] = INT_TO_BCD(pbuf[5]);
                }
                return len;
            }
        }
        
        n++;
        bsp_timer_delay_ms(1000);
    }
    
    return -1;
}

int32_t bsp_nbiot_get_unix(void)
{
	uint8_t buf[6];
	struct tm tim;
	
	bsp_nbiot_get_time(buf);
	bsp_rtc_set_time(buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
	
	tim.tm_year  = (buf[0]) + 2000 - 1900;
	tim.tm_mon   = (buf[1]) - 1;
	tim.tm_mday  = (buf[2]);
	tim.tm_hour  = (buf[3]);
	tim.tm_min   = (buf[4]);
	tim.tm_sec   = (buf[5]);
	tim.tm_isdst = -1;
	
	return (int32_t)mktime(&tim);   
}

static int8_t bsp_nbiot_wait_info(uint8_t *pBuf)
{
    int8_t err     = -1;
    uint16_t bytes = 0x00;
    uint8_t ucData = 0x00;
    
    bsp_timer_star_once(NBIOT_TMR_ID, 20);
    
    while(1)
    {
        //��ȡ������Ϣ
        if(comGetChar(COM_NBIOT, &ucData))
        {
            //������ʱ���ܣ��ֽ����ֽ�֮���ʱ��Ϊ20ms��
            //����20ms����Ϊ�ѽ�����һ֡����
          
            pBuf[bytes++] = ucData;
			if(bytes >= 512)
			{
				return -1;
			}
            else
            {
                err = 0;
            }
            
            bsp_timer_star_once(NBIOT_TMR_ID, 20);
        }
        
        if(bsp_timer_check(NBIOT_TMR_ID))
        {
            return err;
        }
    }
} 

int8_t bsp_nbiot_recv(uint8_t socket,uint8_t tcmd)
{
    uint16_t len;
	uint8_t cmd[100];
    uint8_t dat[100];
    uint8_t RevBuf[256];
	char *ptr;
	char *ptr1;
    uint8_t i;
    
    if(bsp_nbiot_wait_info(&RevBuf[0]) != -1)
    {
        printf("%s",RevBuf);
        
        ptr = strstr((char *)RevBuf,"+NPSMR:0");
        if(ptr != NULL)
        {
            return -1;
        }
        
		ptr = strstr((char *)RevBuf,"+NSONMI:");
        if(ptr != NULL)
        {
            ptr += 8;
            ptr1 = strstr((char *)ptr,"\r\n");
            if(ptr1 != NULL)
            {
                ptr1 += 2;
                memset(cmd,0,sizeof(cmd));
                memcpy(cmd,ptr,ptr1-ptr);

				memset(dat,0,sizeof(dat));
                sprintf((char *)dat,"AT+NSORF=%s",cmd);

				memset(RevBuf,0,sizeof(RevBuf));
                len = bsp_nbiot_GetResponse((char *)dat, (char *)RevBuf,3000);
                if(len > 0)
                {
                    printf("%s",RevBuf);
					bsp_timer_delay_ms(1000);
					
                    ptr = strstr((char *)RevBuf,(char *)"7B");
                    if(ptr != NULL)
                    {
                        ptr1 = strstr((char *)ptr,",");
                        if(ptr != NULL)
                        {
                            memset(dat,0,sizeof(dat));
                            bsp_nbiot_HexStrToByte(ptr,dat,ptr1-ptr);
                            printf("\r\nanalysis data:");
                            for(i=0; i<(ptr1-ptr)/2; i++)
                            {
                                printf("%02X ",dat[i]);
                            }
                            printf("\r\n");
                            
                            bsp_timer_delay_ms(1000);
                            
							if(tcmd == dat[1])
							{
								return 0;
							}
							else
							{
								printf("-->error data formate 3\r\n");
							}
                        }
                        else
                        {
                            printf("-->error data formate 2\r\n");
                        }
                    }
                    else
                    {
                        printf("-->error data formate 1\r\n");
                    }
                }
            } 
        }
    }
    
	return -1;
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_nbiot_WaitResponse
*	����˵��: �ȴ�nbiot����ָ����Ӧ���ַ���, ���԰��������ַ���ֻҪ������ȫ���ɷ��ء�
*	��    ��:  _Cmd:��������
*             _pAckStr : Ӧ����ַ����� ���Ȳ��ó���255
*			  _usTimeOut : ����ִ�г�ʱ��0��ʾһֱ�ȴ�. >����ʾ��ʱʱ�䣬��λ1ms
*	�� �� ֵ: 1 ��ʾ�ɹ�  0 ��ʾʧ��
*********************************************************************************************************
*/
static int8_t bsp_nbiot_WaitResponse(char *_Cmd, char *_pAckStr, uint16_t _usTimeOut)
{
	uint8_t ucData;
	uint16_t pos = 0;
	uint32_t len;

	len = strlen(_pAckStr);
	if (len > 255)
	{
		return -1;
	}
    
    comClearRxFifo(COM_NBIOT);

    comSendBuf(COM_NBIOT, (uint8_t *)_Cmd, strlen(_Cmd));
	comSendBuf(COM_NBIOT, (uint8_t *)"\r\n", 2);
    
    DBUG_NBIOT("%s\r\n",_Cmd);
    
	/* _usTimeOut == 0 ��ʾ���޵ȴ� */
	if (_usTimeOut > 0)
	{
		bsp_timer_star_once(NBIOT_TMR_ID, _usTimeOut);		/* ʹ�������ʱ��3����Ϊ��ʱ���� */
	}
    
	while (1)
	{
		if (_usTimeOut > 0)
		{
			if (bsp_timer_check(NBIOT_TMR_ID))
			{
				/* ��ʱ */
              
                DBUG_NBIOT("\r\n");
                
				return -1;
			}
		}

		if (comGetChar(COM_NBIOT, &ucData))
		{
			DBUG_NBIOT("%c",ucData);
			if (ucData == _pAckStr[pos])
			{
				pos++;

				if (pos == len)
				{
					/* �յ�ָ����Ӧ�����ݣ����سɹ� */
                    DBUG_NBIOT("\r\n");
                    
					return 0;
				}
			}
			else
			{
				pos = 0;
			}
		}
		bsp_iwdg_clear();
	}
}

static uint8_t bsp_nbiot_GetResponse(char *_Cmd, char *_pAckStr, uint16_t _usTimeOut)
{
	uint8_t ucData;
	uint8_t s_len = 0;
    
    comClearRxFifo(COM_NBIOT);
    
    comSendBuf(COM_NBIOT, (uint8_t *)_Cmd, strlen(_Cmd));
	comSendBuf(COM_NBIOT, (uint8_t *)"\r\n", 2);
    
    DBUG_NBIOT("%s\r\n",_Cmd);
    
    /* _usTimeOut == 0 ��ʾ���޵ȴ� */
	if (_usTimeOut > 0)
	{
		bsp_timer_star_once(NBIOT_TMR_ID, _usTimeOut);		/* ʹ�������ʱ��3����Ϊ��ʱ���� */
	}
    
	while (1)
	{
		if (_usTimeOut > 0)
		{
			if (bsp_timer_check(NBIOT_TMR_ID))
			{
				/* ��ʱ */
                DBUG_NBIOT("\r\n");
                
                if(s_len > 0)
                {   
                    return s_len;
                }
                
				return 0;
			}
		}

        if(comGetChar(COM_NBIOT, &ucData))
        {
            if (s_len < 250)
            {
                _pAckStr[s_len++] = ucData;		/* ������յ������� */
            }
        }
		
		bsp_iwdg_clear();
    }
}

//ʮ�������ַ���ת��Ϊ�ֽ���    
static void bsp_nbiot_HexStrToByte(const char* source, unsigned char* dest, int sourceLen)  
{  
    short i;  
    unsigned char highByte, lowByte;  
  
    for (i=0; i<sourceLen; i+=2)  
    {  
        highByte = toupper(source[i]);  
        lowByte  = toupper(source[i + 1]);  
  
        if (highByte > 0x39)  
            highByte -= 0x37;  
        else  
            highByte -= 0x30;  
  
        if (lowByte > 0x39)  
            lowByte -= 0x37;  
        else  
            lowByte -= 0x30;  
  
        dest[i / 2] = (highByte << 4) | lowByte;  
    }  
    return;  
}  



