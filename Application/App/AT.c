#include "app.h"
#include "stdlib.h"

static uint8_t s_ucATSta = 0;  //AT状态
static uint8_t s_ucaBuf[2048];
static uint16_t s_usSize = 0;
static uint8_t s_ucTimeout = 0;  //超时
static uint8_t s_ucTryCnt = 0;  //超时
static uint8_t s_ucUpdataSta = 0;

uint8_t g_ucRssi = 0;
uint8_t g_ucSocket = 0;
uint8_t g_ucTimeout = 0;  //超时
uint32_t g_uiTime = 0;

extern uint8_t g_ucTimeFast;
/*************************************************************************************************/

//计算累加和
unsigned int add8_caculate(unsigned char *buf, unsigned int len)
{
	unsigned int i;
	unsigned char sum;

	sum = 0;
	for(i=0; i<len; i++)
	{
	   sum += buf[i];
	}

	return sum;
}

//追加累加和, len不包含校验码
unsigned int add8_append(unsigned char *buf, unsigned int len)
{
	unsigned char sum;

	sum = add8_caculate(buf, len);
	buf[len] = sum;
	return (len+1);
}

//检查累加和，len包含校验码
unsigned int add8_check(unsigned char *buf, unsigned int len)
{
	unsigned char sum;

	sum = add8_caculate(buf, len-1);
	if(sum == buf[len-1]) return 1;
	else return 0;
}

unsigned long int crc32_flash ( unsigned int addr, unsigned long int len )
{
	int j;
	unsigned int crc;
	unsigned long int crc_table;
	unsigned char data;

	crc = 0xFFFFFFFF;
	while ( len )
	{
		data = *(__IO uint8_t*)addr;
		crc_table = ( ( crc & 0xFF ) ^data );
		for ( j = 0; j < 8; j++ ) // Do eight times.
		{
			if ( crc_table & 1 )
			{
				crc_table = ( crc_table >> 1 ) ^ 0xEDB88320;
			}
			else 
			{
				crc_table = crc_table >> 1;
			}
		}

		crc = ( crc >> 8 ) ^crc_table;
		addr += 1;
		len -= 1;
	}
	return ( crc ^ 0xffffffff );
}

/*************************************************************************************************/
//int AT_ACK(char *_Cmd, char *_Ack, char *_buf, uint16_t _usLen, uint32_t _usTimeout, uint8_t times)
//{	
//	uint16_t len;

//	if(s_ucATSta == 0) //初始化
//	{		
//		comClearRxFifo(COM_RPT);
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta&0x01)  //发送指令
//	{
//		if(_Cmd != NULL)
//		{			
//			comSendBuf(COM_RPT, (uint8_t *)_Cmd, strlen(_Cmd));
//			comSendBuf(COM_DEBUG, (uint8_t *)_Cmd, strlen(_Cmd));				
//		}
//		bsp_timer_star_once(TMR_RPT, _usTimeout);
//		s_ucTimeout = 0;
//		g_ucTimeout = 0,		
//		s_ucATSta++;		
//		
//	}
//	else //接收应答 
//	{		
//		if(bsp_timer_check(TMR_RPT)) 
//		{
//			if(_Ack != NULL)
//			{				
//				if((s_ucATSta>>1) >= times) return 0; //失败
//				else	s_ucTimeout = 1, g_ucTimeout = 1, s_ucATSta++;
//			}
//			else
//			{
//				return 1; //成功
//			}				
//		}
//		else if(comGetRxTimeOut(COM_RPT))
//		{
//			s_ucTimeout = 1,
//			g_ucTimeout = 1, 			
//			len = comGetBytes(COM_RPT, (uint8_t *)_buf, _usLen);			
//			comSendBuf(COM_DEBUG, (uint8_t *)_buf, len);			
//			if(_Ack != NULL)
//			{				
//				if(strstr(_buf, _Ack))
//				{
//					return 1; //成功				
//				}
//			}		
//		}		
//	}	

//	return -1;
//}

///******************************************************************************/
//void AT_ResetSta(void)	
//{
//	s_ucATSta = 0;	
//}

//void AT_SendCmd(char *_Cmd)
//{	
//	comClearRxFifo(COM_RPT);
//	
//	comSendBuf(COM_RPT, (uint8_t *)_Cmd, strlen(_Cmd));	
//	comSendBuf(COM_RPT, (uint8_t *)"\r\n", 2);	
//	
//	comSendBuf(COM_DEBUG, (uint8_t *)_Cmd, strlen(_Cmd));	
//	comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//			
//}

//int AT_DELAY(uint32_t _period)
//{
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;
//		bsp_timer_star_once(TMR_RPT, _period);		
//	}	
//	else if(bsp_timer_check(TMR_RPT))
//	{
//		return 1;
//	}
//	
//	return -1;	
//}

int AT_START(void)
{
	uint8_t ch;
	uint16_t len, i;
	uint8_t buf[256],*p;	
	static OS_TimeStruct stAT_AckTim;
	if(s_ucATSta == 0)
	{
		
		bsp_power_rpt_off();
		bsp_power_pwk_off();			
		stAT_AckTim=GetOSRunTimeNow();//获取当前时间
		s_ucATSta++;	
		printf("\r\ns_ucATSta:%d\r\n",s_ucATSta);	
	}	
	else if(s_ucATSta == 1)
	{		
		if( TimeOut==SingleTimeoutCheck(stAT_AckTim,0,0,50))
		{			
			bsp_power_rpt_on();
			printf("\r\nVBAT ON\r\n");
			//bsp_timer_star_once(TMR_RPT, TM_500MS);	
      stAT_AckTim=GetOSRunTimeNow();//获取当前时间			
			s_ucATSta++;		
      printf("\r\ns_ucATSta:%d\r\n",s_ucATSta);				
		}	
	}	
	else if(s_ucATSta == 2)	
	{		
		if( TimeOut==SingleTimeoutCheck(stAT_AckTim,0,0,500))
		{
			
				bsp_power_pwk_on();
				printf("\r\nPWRKEY ON\r\n");		
			// bsp_timer_star_once(TMR_RPT, TM_3S);	
      stAT_AckTim=GetOSRunTimeNow();//获取当前时间				
			s_ucATSta++;	
      printf("\r\ns_ucATSta:%d\r\n",s_ucATSta);					
		}			
	}
	else if(s_ucATSta == 3)
	{		
		if( TimeOut==SingleTimeoutCheck(stAT_AckTim,0,3,0))
		{

				bsp_power_pwk_off();
				printf("\r\nPWRKEY OFF\r\n");		
			//bsp_timer_star_once(TMR_RPT, TM_100MS);		
      stAT_AckTim=GetOSRunTimeNow();//获取当前时间				
			s_ucATSta++;	
      printf("\r\ns_ucATSta:%d\r\n",s_ucATSta);		
      //Uart2_SendStr("AT+IPR=9600\r\n");			
     return 1;			
		}
	}	
//	else if(s_ucATSta == 4)
//	{		
//		if( TimeOut==SingleTimeoutCheck(stAT_AckTim,0,0,100))
//		{
//			bsp_power_psm_off();
//			//bsp_timer_star_once(TMR_RPT, TM_100MS);	
//			stAT_AckTim=GetOSRunTimeNow();//获取当前时间	
//			s_ucATSta++;	
//printf("\r\ns_ucATSta:%d\r\n",s_ucATSta);				
//		}
//	}
//	else if(s_ucATSta == 5)	
//	{		
//		if( TimeOut==SingleTimeoutCheck(stAT_AckTim,0,0,100))
//		{
//			bsp_power_psm_on();
//			//bsp_timer_star_once(TMR_RPT, TM_100MS);		
//      stAT_AckTim=GetOSRunTimeNow();//获取当前时间				
//			s_ucATSta++;			
//			printf("\r\ns_ucATSta:%d\r\n",s_ucATSta);	
//		}			
//	}	
//	else if(s_ucATSta == 6)
//	{		
//		if( TimeOut==SingleTimeoutCheck(stAT_AckTim,0,0,100))
//		{
//			bsp_power_psm_off();	
//			//bsp_timer_star_once(TMR_RPT, TM_100MS);	
//      stAT_AckTim=GetOSRunTimeNow();//获取当前时间				
//			s_ucATSta++;			
//			printf("\r\ns_ucATSta:%d\r\n",s_ucATSta);	
//		}
//	}	
//	else if(s_ucATSta == 7)
//	{	
//		if( TimeOut==SingleTimeoutCheck(stAT_AckTim,0,0,100))
//		{					
//			return 1;
//			printf("\r\ns_ucATSta:%d\r\n",s_ucATSta);	
//		}
//	}

//	if(comGetRxTimeOut(COM_RPT))
//	{			
//		len = comGetRxCount(COM_RPT);
//		memset(buf, 0, sizeof(buf));
//		if(len)
//		{
//			for(i=0; i<len; i++)			
//			{
//				comGetChar(COM_RPT, buf+i);				
//			}
//			
//			comSendBuf(COM_DEBUG, buf, len);				
//			p = (uint8_t *)strstr((char *)buf,"RDY");	
//			if(p != NULL)
//			{
//				return 1;				
//			}			
//		}
//		
//	}			
	
	return -1;	
}

//int AT_OK(char *_Cmd)
//{
//	uint16_t len, i;
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 30)
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		AT_SendCmd(_Cmd);
//		bsp_timer_star_once(TMR_RPT, TM_1S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT))
//		{
//			s_ucATSta++;			
//		}
//		else if(comGetRxTimeOut(COM_RPT))
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);				
//				p = (uint8_t *)strstr((char *)buf,"OK\r\n");	
//				if(p != NULL)
//				{
//					return 1;               
//				}
//			}			
//	
//		}
//	}

//	return -1;	
//}

////S:设置 Q:查询
//int AT_CFUN(char op)
//{
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)    //重发次数限制 /2
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		if(op == 'S') AT_SendCmd("AT+CFUN=1");
//		else  		  AT_SendCmd("AT+CFUN?");
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));
//				if(op == 'S') p = (uint8_t *)strstr((char *)buf,"\r\nOK\r\n");
//				else 		  p = (uint8_t *)strstr((char *)buf,"+CFUN: 1\r\n\r\nOK\r\n");
//				if(p != NULL)
//				{
//					return 1;   
//				}			
//			}							
//		}
//	}
//		
//	return -1;	
//}

//int AT_CPIN(char op)
//{
//	uint16_t len, i;
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 10)   //重发次数限制 /2
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		AT_SendCmd("AT+CPIN?");
//		bsp_timer_star_once(TMR_RPT, TM_5S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理	
//		{								
//			s_ucATSta++;	
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);				
//				p = (uint8_t *)strstr((char *)buf,"+CPIN: READY");	
//				if(p != NULL)
//				{
//					s_ucATSta = 0;				
//					return 1;   
//				}
//				else if(strstr((char *)buf,"ERROR"))
//				{
//					s_ucATSta = 0;				
//					return 0;  					
//				}
//				
//			}
//		}
//	}	
//						
//	return -1;	
//}

//int AT_CGSN(char op)
//{
//    uint32_t addr;	
//	uint16_t len, i;
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)   //重发次数限制 /2
//	{
//		return 0;
//	}
//	else if(s_ucATSta&0x01)
//	{
//		if(g_tVar.ucMODULE == MD_BC26) AT_SendCmd("AT+CGSN=1");		
//		else 						   AT_SendCmd("AT+CGSN");	
//		
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT))  //超时处理	
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);	
//				p = (uint8_t *)strstr((char *)buf,"\r\n\r\nOK\r\n");
//				if(p != NULL)			
//				{
//					p -= 15;
//					if(memcmp(g_tVar.caIMEI, p, 15) != 0)
//					{						
//						memcpy(g_tVar.caIMEI, p, 15);
//						g_tVar.caIMEI[15] = 0;
//						addr = OFFSET_OF(VAR_T, caIMEI);
//						var_WriteBytes((uint8_t *)g_tVar.caIMEI, addr, 16);						
//					}
//					return 1;
//				}
//			}
//							
//		}
//	}
//	
//	return -1;	
//}

//int AT_CIMI(char op)
//{
//	uint32_t addr;
//	uint16_t len, i;
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)   //重发次数限制 /2
//	{
//		return 0;
//	}
//	else if(s_ucATSta&0x01)
//	{
//		AT_SendCmd("AT+CIMI");
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);	
//				p = (uint8_t *)strstr((char *)buf,"\r\n\r\nOK\r\n");
//				if(p != NULL)			
//				{
//					p -= 15;
//					if(memcmp(g_tVar.caPHO, p, 15) != 0)
//					{						
//						memcpy(g_tVar.caPHO, p, 15);
//						g_tVar.caPHO[15] = 0;
//						addr = OFFSET_OF(VAR_T, caPHO);
//						var_WriteBytes((uint8_t *)g_tVar.caPHO, addr, 16);						
//					}					
//					return 1;
//				}
//			}
//		}						
//	}
//	
//	return -1;	
//}

//int AT_CCID(char op)
//{
//	uint32_t addr;
//	uint16_t len, i;
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)   //重发次数限制 /2
//	{
//		return 0;
//	}
//	else if(s_ucATSta&0x01)
//	{
//	
//		if((g_tVar.ucMODULE == MD_M26) || (g_tVar.ucMODULE == MD_L610))
//		{
//			AT_SendCmd("AT+CCID");
//		}
//		else 						  
//		{
//			AT_SendCmd("AT+QCCID");	
//		}			
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);	
//				p = (uint8_t *)strstr((char *)buf,"\r\n\r\nOK\r\n");
//				if(p != NULL)			
//				{
//					if(g_tVar.ucMODULE == MD_M26)
//					{
//						p -= 1; //去掉引号
//					}

//					p -= 20;
//					if(memcmp(g_tVar.caCCID, p, 20) != 0)
//					{						
//						memcpy(g_tVar.caCCID, p, 20);
//						g_tVar.caCCID[20] = 0;
//						addr = OFFSET_OF(VAR_T, caCCID);
//						var_WriteBytes((uint8_t *)g_tVar.caCCID, addr, 21);						
//					}					
//					return 1;
//				}
//			}
//		}						
//	}
//	
//	return -1;	
//}

//int AT_ATI(char op)
//{
//	uint32_t addr;
//	uint16_t len, i,module;
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)   //重发次数限制 /2
//	{
//		return 0;
//	}
//	else if(s_ucATSta&0x01)
//	{
//		AT_SendCmd("ATI");
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);	
//				p = (uint8_t *)strstr((char *)buf,"\r\n\r\nOK\r\n");
//				if(p != NULL)			
//				{
//					p = (uint8_t *)strstr((char *)buf,"EG25");
//					if(p != NULL) module = MD_EC20,printf("\r\nMODULE:EG25\r\n");					
//					
//					p = (uint8_t *)strstr((char *)buf,"EC20");
//					if(p != NULL) module = MD_EC20,printf("\r\nMODULE:EC20\r\n");
//					
//					p = (uint8_t *)strstr((char *)buf,"M26");
//					if(p != NULL) module = MD_M26,printf("\r\nMODULE:M26\r\n");	
//					
//					p = (uint8_t *)strstr((char *)buf,"BC26");
//					if(p != NULL) module = MD_BC26,printf("\r\nMODULE:BC26\r\n");

//					p = (uint8_t *)strstr((char *)buf,"SLM130");
//					if(p != NULL) module = MD_SLM130,printf("\r\nMODULE:SLM130\r\n");

//					p = (uint8_t *)strstr((char *)buf,"L610");
//					if(p != NULL) module = MD_L610,printf("\r\nMODULE:L610\r\n");	
//					
//					if(g_tVar.ucMODULE != module)
//					{					
//						g_tVar.ucMODULE = module;
//						addr = OFFSET_OF(VAR_T, ucMODULE);						
//						var_WriteBytes(&g_tVar.ucMODULE, addr, 1);
////						printf("\r\ng_tVar.ucMODULE=%d\r\n",g_tVar.ucMODULE);						
//					}
//																	
//					return 1;
//				}
//			}
//		}						
//	}
//	
//	return -1;	
//}
///******************************************************************************/
//int AT_CSQ(char op)
//{
//	uint16_t len, i;
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 120)    //重发次数限制 /2
//	{
//		return 0;
//	}
//	else if(s_ucATSta&0x01)
//	{
//		AT_SendCmd("AT+CSQ");
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);	
//				p = (uint8_t *)strstr((char *)buf,"OK\r\n");
//				if(p != NULL)			
//				{			
//					p = (uint8_t *)strstr((char *)buf,"+CSQ:");	
//					if(p != NULL)
//					{
//						if((p[6]>'0')&&(p[6]<='9')&&(p[7]>='0')&&(p[7]<='9'))
//						{
//						  g_ucRssi = (p[6]-0x30)*10 + p[7]-0x30; 
//						  if((g_ucRssi >= 10)&&(g_ucRssi <= 31))
//						  {
//								g_tDat.signal = g_ucRssi;	//信号量	
//								return g_ucRssi;   
//						  }
//						}					           
//					}
//				}
//			}						
//		}
//	}
//	
//	return -1;	
//}

//int AT_CREG(char op)
//{
//	uint16_t len, i;
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 40)    //重发次数限制 /2
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		AT_SendCmd("AT+CREG?");
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);				
//				p = (uint8_t *)strstr((char *)buf,"+CREG: 0,1\r\n\r\nOK\r\n");	
//				if(p != NULL)
//				{
//						return 1;   
//				}
//				p = (uint8_t *)strstr((char *)buf,"+CREG: 0,5\r\n\r\nOK\r\n");	
//				if(p != NULL)
//				{
//						return 1;   
//				}			
//			}							
//		}
//	}
//		
//	return -1;	
//}

//int AT_CGATT(char op)
//{
//	uint16_t len, i;
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 120)  //重发次数限制 /2
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		AT_SendCmd("AT+CGATT?");
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else 	
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;
//		}
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);				
//				p = (uint8_t *)strstr((char *)buf,"+CGATT: 1\r\n\r\nOK\r\n");	
//				if(p != NULL)
//				{
//						return 1;   
//				}
//				p = (uint8_t *)strstr((char *)buf,"+CGATT: 5\r\n\r\nOK\r\n");	
//				if(p != NULL)
//				{
//						return 1;   
//				}			
//			}
//		}			
//	}
//	
//	return -1;	
//}

//int AT_QICLOSE(char op)
//{
//	uint16_t len, i;
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 2)  //重发次数限制 /2
//	{
//		return 1;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		AT_SendCmd("AT+QICLOSE");
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else 	
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);				
//				p = (uint8_t *)strstr((char *)buf,"CLOSE OK");	
//				if(p != NULL)
//				{
//						return 1;   
//				}				
//			}			
//		}
//	}
//	
//	return -1;	
//}

//int AT_QIOPEN(char op)
//{
//	uint16_t len, i;
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)  //重发次数限制 /3
//	{
//		return 0;		
//	}	
//	else if((s_ucATSta == 1) || (s_ucATSta == 5))
//	{
//		memset(buf,0,sizeof(buf));
//		sprintf((char *)buf, "AT+QIOPEN=\042UDP\042,\042%s\042,\042%d\042", g_tVar.caIP,g_tVar.usPORT);	
//		AT_SendCmd((char *)buf);		
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else if((s_ucATSta == 2) || (s_ucATSta == 6))
//	{
//		if(bsp_timer_check(TMR_RPT))
//		{
//			s_ucATSta++;
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);				
//				p = (uint8_t *)strstr((char *)buf,"CONNECT OK");	
//				if(p != NULL)
//				{
//					return 1;
//				}
//				p = (uint8_t *)strstr((char *)buf,"ALREADY CONNECT");	
//				if(p != NULL)
//				{
//					return 1;
//				}								
//			}			
//		}				
//	}
//	else if(s_ucATSta == 3)			
//	{
//		AT_SendCmd("AT+QICLOSE");
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;
//	}
//	else if(s_ucATSta == 4)
//	{
//		if(bsp_timer_check(TMR_RPT)) // 超时处理
//		{
//			s_ucATSta++;
//		}		
//		else if(comGetRxTimeOut(COM_RPT))	
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);				
//				p = (uint8_t *)strstr((char *)buf,"CLOSE OK");	
//				if(p != NULL)
//				{
//					s_ucATSta++;   
//				}
//				
//			}			
//		}				
//	}
//	
//	return -1;	
//}	

//// R:接收平台配置命令并应答
//// D:发送数据包并等待平台应答
//// H:发送心跳包并等待平台应答
//// L:发送登陆包并等待平台应答
//// T:触发上报当前数据
//int AT_M26_UDP(char op)
//{
//	char cmd[64];	
//	uint16_t len, i;
//	uint8_t buf[3072],*p, ch;
//	
//	if(s_ucATSta == 0)
//	{
//		if(op == 'R')
//		{			
//			bsp_timer_star_once(TMR_RPT, TM_6S);
//			s_ucATSta++;			
//		}
//		else
//		{
//			s_ucATSta += 2;			
//		}		
//	}
//	else if(s_ucATSta == 1) //接收平台命令
//	{
//		if(comGetRxTimeOut(COM_RPT))
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);				
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);
//					sprintf((char *)cmd,"%02X", buf[i]);					
//					comSendBuf(COM_DEBUG, (uint8_t *)cmd, strlen(cmd));						
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				if(g_tVar.ucSVTYPE == 0)
//				{					
//					s_usSize = up_cfg(s_ucaBuf, buf, len);
//				}
//				else if(g_tVar.ucSVTYPE == 1)
//				{
//					s_usSize = up1_cfg(s_ucaBuf, buf, len);					
//				}
//				if(s_usSize)
//				{					
//					s_ucATSta++;
//				}
//			}			
//		}
//		
//		if(bsp_timer_check(TMR_RPT))
//		{
//			return 1; 
//		}
//	}
//	else if(s_ucATSta > 20) //重发次数限制 /10
//	{
//		return 0;
//	}
//	else if((s_ucATSta == 2) || (s_ucATSta == 12))//指定发送数据长度
//	{
//		if(g_tVar.ucSVTYPE == 0)
//		{		
//			if(op == 'L')
//			{			
//				s_usSize = up_login(s_ucaBuf);			
//			}		
//			else if(op == 'D')
//			{				
//				s_usSize = up_data(s_ucaBuf);			
//			}
//			else if(op == 'T')
//			{			
//				s_usSize = up_trig(s_ucaBuf);			
//			}
//			else if(op == 'H')
//			{			
//				s_usSize = up_heart(s_ucaBuf);			
//			}
//		}
//		else if(g_tVar.ucSVTYPE == 1)
//		{		
//			if(op == 'L')
//			{			
//				s_usSize = up1_login(s_ucaBuf);			
//			}		
//			else if(op == 'D')
//			{				
//				s_usSize = up1_data(s_ucaBuf);			
//			}
//			else if(op == 'T')
//			{			
//				s_usSize = up1_trig(s_ucaBuf);			
//			}
//			else if(op == 'H')
//			{			
//				s_usSize = up1_heart(s_ucaBuf);			
//			}
//		}

//		memset(cmd,0,sizeof(cmd));
//		sprintf((char *)cmd, "AT+QISEND=%d", s_usSize);
//		AT_SendCmd(cmd);
//		bsp_timer_star_once(TMR_RPT, TM_3S);		
//		s_ucATSta++;		
//	}	
//	else if((s_ucATSta == 3) || (s_ucATSta == 13))//等待输入发送数据
//	{
//		if(comGetRxTimeOut(COM_RPT))
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);				
//				p = (uint8_t *)strstr((char *)buf,">");	
//				if(p != NULL)
//				{
//					s_ucATSta++; 
//				}			
//			}			
//		}
//		
//		if(bsp_timer_check(TMR_RPT)) // 超时处理
//		{
//			if(s_ucATSta < 10)
//			{
//				s_ucATSta = 12;
//			}
//			else if(s_ucATSta < 20)
//			{
//				s_ucATSta = 22;					
//			}
//			else if(s_ucATSta < 30)
//			{
//				s_ucATSta = 32;					
//			}				
//		}			
//	}
//	else if((s_ucATSta == 4) || (s_ucATSta == 14))//发送数据
//	{
//		memset(buf,0,sizeof(buf));
//		for(i=0; i<s_usSize; i++)
//		{
//			sprintf((char *)buf+i*2,"%02X", *(s_ucaBuf+i));					
//		}
//		comSendBuf(COM_DEBUG, buf, strlen((char *)buf));
//		comClearRxFifo(COM_RPT);
//		comSendBuf(COM_RPT, s_ucaBuf, s_usSize);		
//		bsp_timer_star_once(TMR_RPT, TM_3S);

//		if(op == 'R')
//		{
//			s_ucATSta = 0; 	
//		}
//		else
//		{
//			s_ucATSta++;
//		}						
//	}
//	else if((s_ucATSta == 5) || (s_ucATSta == 15))//发送是否成功
//	{
//		if(comGetRxTimeOut(COM_RPT))
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);				
//				p = (uint8_t *)strstr((char *)buf,"SEND OK");	
//				if(p != NULL)
//				{
//					comClearRxFifo(COM_RPT);
//					bsp_timer_star_once(TMR_RPT, TM_6S);					
//					s_ucATSta++; 			
//				}			
//			}			
//		}
//		
//		if(bsp_timer_check(TMR_RPT)) // 超时处理 
//		{
//			if(s_ucATSta < 10)
//			{
//				s_ucATSta = 12;
//			}
//			else if(s_ucATSta < 20)
//			{
//				s_ucATSta = 22;					
//			}
//			else if(s_ucATSta < 30)
//			{
//				s_ucATSta = 32;					
//			}							
//		}		
//	}
//	else if((s_ucATSta == 6) || (s_ucATSta == 16)) //平台应答
//	{
//		if(comGetRxTimeOut(COM_RPT))
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, &ch);
//					sprintf((char *)buf+i*2,"%02X", ch);										
//				}								
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);					
//				
//				//判断平台应答
//				if((len == 15) || (len == 13) || (len == 14))
//				{
//					if(op == 'D')  
//					{
//						uint32_t addr;
//						//上报数据包成功,修改并保存上报索引						
//						addr = OFFSET_OF(VAR_T, uiSPIDX);
//						var_WriteBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//						var_ReadBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//						printf("\r\n");		
//						printf("TPIDX:%u\r\n", g_tVar.uiTPIDX);			
//						printf("SPIDX:%u\r\n", g_tVar.uiSPIDX);
//						printf("\r\n");	
//						
//						if(g_tVar.uiSPIDX == g_tVar.uiTPIDX) return 1;
//						else s_ucATSta = 2;												
//					}
//					else
//					{
//						return 1;
//					}
//				}					
//			}			
//		}
//		
//		if((op == 'H') && (g_tVar.ucSVTYPE == 1))//大数据云平台特殊处理
//		{
//			return 1;
//		}
//		
//		if(bsp_timer_check(TMR_RPT)) // 超时处理  
//		{
//			if((op == 'L') || (op == 'H'))
//			{
//				return 1;
//			}				
//			
//			if(s_ucATSta < 10)
//			{
//				s_ucATSta = 12;
//			}
//			else if(s_ucATSta < 20)
//			{
//				s_ucATSta = 22;					
//			}
//			else if(s_ucATSta < 30)
//			{
//				s_ucATSta = 32;					
//			}						
//		}		
//	}	
//	
//	return -1;			
//}

///******************************************************************************/
//int AT_CESQ(char op)
//{
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 40)    //重发次数限制 /2
//	{
//		return 1;
//	}
//	else if(s_ucATSta&0x01)
//	{
//		AT_SendCmd("AT+CESQ");
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));	
//				p = (uint8_t *)strstr((char *)buf,"OK\r\n");
//				if(p != NULL)			
//				{			
//					p = (uint8_t *)strstr((char *)buf,"+CESQ:");	
//					if(p != NULL)
//					{
//						if((p[7]>'0')&&(p[7]<='9')&&(p[8]>='0')&&(p[8]<='9'))
//						{
//						  g_ucRssi = (p[7]-0x30)*10 + p[8]-0x30; 
//						  if((g_ucRssi >= 10)&&(g_ucRssi <= 63))
//						  {

//								return g_ucRssi;   
//						  }
//						}					           
//					}
//				}
//			}						
//		}
//	}
//	
//	return -1;	
//}

//int AT_CEREG(char op)
//{
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)    //重发次数限制 /2
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		if(op == 'S') AT_SendCmd("AT+CEREG=1");
//		else 		  AT_SendCmd("AT+CEREG?"); 
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));
//				if(op == 'S') p = (uint8_t *)strstr((char *)buf,"\r\nOK\r\n");	
//				else 		  p = (uint8_t *)strstr((char *)buf,"+CEREG: 1,1\r\n\r\nOK\r\n");				
//				if(p != NULL)
//				{
//						return 1;   
//				}
//				if(op == 'S') p = (uint8_t *)strstr((char *)buf,"\r\nOK\r\n");	
//				else 		  p = (uint8_t *)strstr((char *)buf,"+CEREG: 1,5\r\n\r\nOK\r\n");
//				if(p != NULL)
//				{
//						return 1;   
//				}			
//			}							
//		}
//	}
//		
//	return -1;	
//}


//int AT_BC260Y_CEREG(void)
//{
//	uint16_t len, i;
//	uint8_t buf[128],*p;
//	uint32_t reg;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 40)    //重发次数限制 /2
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		AT_SendCmd("AT+CEREG?");
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);	

//                //+CEREG: 0,1
//                p = (uint8_t *)strstr((char *)buf,"+CEREG:");    
//                if(p != NULL)
//                {
//                    if(sscanf((char *)p,"%*[^ ] %*d,%d",&reg)>0)
//                    {
//                        if((reg == 1)||(reg == 5))
//                        {
//                            return 1;   
//                        }
//                    }                              
//                }			
//			}							
//		}
//	}
//	return -1;	
//}



//int AT_QGACT(char op)
//{
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)    //重发次数限制 /2
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		AT_SendCmd("AT+QGACT=1,1,\042ctnb\042,\042\042,\042\042");
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));				
//				p = (uint8_t *)strstr((char *)buf,"\r\nOK\r\n");	
//				if(p != NULL)
//				{
//					return 1;   
//				}			
//			}							
//		}
//	}
//		
//	return -1;	
//}

//int AT_CSCON(char op)
//{
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)    //重发次数限制 /2
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		if(op == 'S') AT_SendCmd("AT+CSCON=1");
//		else  		  AT_SendCmd("AT+CSCON?");
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));				
//				if(op == 'S') p = (uint8_t *)strstr((char *)buf,"\r\nOK\r\n");
//				else  		  p = (uint8_t *)strstr((char *)buf,"+CSCON: 1,1\r\n\r\nOK\r\n");
//				if(p != NULL)
//				{
//					return 1;   
//				}			
//			}							
//		}
//	}
//		
//	return -1;	
//}

//int AT_QSOC(char op)
//{
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)    //重发次数限制 /2
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		AT_SendCmd("AT+QSOC=1,2,1");
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));				
//				p = (uint8_t *)strstr((char *)buf,"\r\n\r\nOK\r\n");	
//				if(p != NULL)
//				{
//					p -= 1;					
//					g_ucSocket = *p;
//					return 1;   
//				}			
//			}							
//		}
//	}
//		
//	return -1;	
//}

//int AT_CCLK(char op)
//{
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)    //重发次数限制 /2
//	{
//		return 1;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		if(g_tVar.ucMODULE == MD_M26)
//		{
//			AT_SendCmd("AT+QLTS");			
//		}
//		else
//		{
//			AT_SendCmd("AT+CCLK?");			
//		}

//		bsp_timer_star_once(TMR_RPT, TM_1S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));				

//				if(g_tVar.ucMODULE == MD_BC26)
//				{
//					p = (uint8_t *)strstr((char *)buf,"GMT");					
//				}
//				else
//				{
//					p = (uint8_t *)strstr((char *)buf,"OK");					
//				}

//				if(p != NULL) 
//				{
//					if(g_tVar.ucMODULE == MD_M26)
//					{
//						p = (uint8_t *)strstr((char *)buf,"+QLTS:");							
//					}
//					else 
//					{
//						p = (uint8_t *)strstr((char *)buf,"+CCLK:");						
//					}
//					if(p != NULL) 
//					{					
//						struct tm tRTC;
//						int yy,mm,dd,hh,nn,ss;

//						if(sscanf((char *)p+8, "%d/%02d/%02d,%02d:%02d:%02d",&yy,&mm,&dd,&hh,&nn,&ss) == 6)
//						{
//							struct tm* ptm;
//							uint32_t time, now, t;
//							
//							tRTC.tm_year = 2000+yy-1900; 
//							tRTC.tm_mon = mm - 1;
//							tRTC.tm_mday = dd;
//							tRTC.tm_hour = hh;
//							tRTC.tm_min = nn;
//							tRTC.tm_sec = ss;
//							tRTC.tm_isdst = -1; 
//							
//							time = mktime(&tRTC) + 28800;
//							
//							if((time > 1648569600)&&(time < 1956499200)) //2022-03-30 00:00:00
//							{
//								ptm = localtime((const time_t *)&time);
//								g_tRTC.Sec = ptm->tm_sec;
//								g_tRTC.Min = ptm->tm_min;
//								g_tRTC.Hour = ptm->tm_hour;
//								g_tRTC.Day = ptm->tm_mday;
//								g_tRTC.Mon = ptm->tm_mon+1;
//								g_tRTC.Year = ptm->tm_year-100;	

//								now = bsp_rtc_get_unix();
//								if(abs((int)(time-now)) >= 60)
//								{
//									int i;
//									
//									__set_PRIMASK(1);	

//								
//									for(i=0; i<3; i++)
//									{
//										bsp_rtc_set_time(g_tRTC.Year,g_tRTC.Mon,g_tRTC.Day,g_tRTC.Hour,g_tRTC.Min,g_tRTC.Sec);
//										t = bsp_rtc_get_unix();
//										if(abs((int)(time-t)) < 60)
//										{
//											break;
//										}
//									}
//									__set_PRIMASK(0);	

//									if(i == 3)
//									{
//										g_usErr |= 0x0010;
//										printf("\r\nbsp_rtc_set_time error\r\n");
//									}
//									else
//									{
//										g_ucRtcCali = 1;
//										g_tDat.time = time;
//										bsp_rtc_get_time();
//										printf("\r\n[RTC: %04d-%02d-%02d %02d:%02d:%02d]\r\n",
//												g_tRTC.Year,g_tRTC.Mon,g_tRTC.Day,g_tRTC.Hour,g_tRTC.Min,g_tRTC.Sec);										
//										if(now > time)
//										{
//											if((g_ucRptAuto == 1) && (g_ucRptTrig == 0))
//											{
//												g_usErr |= 0x0020;
//												g_ucRptAuto = 0;															
//												g_ucTimeFast = 1;
//												printf("\r\nThe clock is running fast\r\n");															
//											}
//										}
//									}
//								}
//								 
//								return 1;
//							}							
//							
//						}								  
//							                       				
//					}
//				}					
//			}							
//		}
//	}
//		
//	return -1;	
//	
//}

//int AT_BC26_EC20_QIOPEN(char op)
//{
//	uint16_t len, i;
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)  //重发次数限制 /3
//	{
//		return 0;		
//	}	
//	else if((s_ucATSta == 1) || (s_ucATSta == 5))
//	{
//		memset(buf,0,sizeof(buf));
//		if(g_tVar.ucCNTP == 1)
//		{			
//			sprintf((char *)buf, "AT+QIOPEN=1,0,\042TCP\042,\042%s\042,%d,0,1", g_tVar.caIP,g_tVar.usPORT);	
//		}
//		else
//		{
//			sprintf((char *)buf, "AT+QIOPEN=1,0,\042UDP\042,\042%s\042,%d,0,1", g_tVar.caIP,g_tVar.usPORT);				
//		}
//		AT_SendCmd((char *)buf);		
//		bsp_timer_star_once(TMR_RPT, TM_30S);
//		s_ucATSta++;		
//	}
//	else if((s_ucATSta == 2) || (s_ucATSta == 6))
//	{
//		if(bsp_timer_check(TMR_RPT))
//		{
//			s_ucATSta++;
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);	
//				p = (uint8_t *)strstr((char *)buf,"+QIOPEN: 0,0");				
////				p = (uint8_t *)strstr((char *)buf,"CONNECT");	
//				if(p != NULL)
//				{
//					return 1;
//				}
//								
//			}			
//		}				
//	}
//	else if(s_ucATSta == 3)			
//	{
//		AT_SendCmd("AT+QICLOSE=0");
//		bsp_timer_star_once(TMR_RPT, TM_10S);
//		s_ucATSta++;
//	}
//	else if(s_ucATSta == 4)
//	{
//		if(bsp_timer_check(TMR_RPT)) // 超时处理
//		{
//			s_ucATSta++;
//		}		
//		else if(comGetRxTimeOut(COM_RPT))	
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);
//				p = (uint8_t *)strstr((char *)buf,"+QIOPEN: 0,0");					
////				p = (uint8_t *)strstr((char *)buf,"CLOSE");	
//				if(p != NULL)
//				{
//					s_ucATSta++;   
//				}
//				
//			}			
//		}				
//	}
//	
//	return -1;	
//}

//int AT_L610_MIPOPEN(char op)
//{
//	uint16_t len, i;
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)  //重发次数限制 /3
//	{
//		return 0;		
//	}	
//	else if((s_ucATSta == 1) || (s_ucATSta == 5))
//	{
//		memset(buf,0,sizeof(buf));
//		if(g_tVar.ucCNTP == 1)
//		{			
//			sprintf((char *)buf, "AT+MIPOPEN=1,,\042%s\042,%d,0", g_tVar.caIP,g_tVar.usPORT);	
//		}
//		else
//		{
//			sprintf((char *)buf, "AT+MIPOPEN=1,,\042%s\042,%d,1", g_tVar.caIP,g_tVar.usPORT);				
//		}
//		AT_SendCmd((char *)buf);		
//		bsp_timer_star_once(TMR_RPT, TM_30S);
//		s_ucATSta++;		
//	}
//	else if((s_ucATSta == 2) || (s_ucATSta == 6))
//	{
//		if(bsp_timer_check(TMR_RPT))
//		{
//			s_ucATSta++;
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);	
//				p = (uint8_t *)strstr((char *)buf,"+MIPOPEN: 1,1");					
//				if(p != NULL)
//				{
//					return 1;
//				}
//								
//			}			
//		}				
//	}
//	else if(s_ucATSta == 3)			
//	{
//		AT_SendCmd("AT+MIPCLOSE=1");
//		bsp_timer_star_once(TMR_RPT, TM_10S);
//		s_ucATSta++;
//	}
//	else if(s_ucATSta == 4)
//	{
//		if(bsp_timer_check(TMR_RPT)) // 超时处理
//		{
//			s_ucATSta++;
//		}		
//		else if(comGetRxTimeOut(COM_RPT))	
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);
//				p = (uint8_t *)strstr((char *)buf,"+MIPCLOSE: 1,0");						
//				if(p != NULL)
//				{
//					s_ucATSta++;   
//				}
//				
//			}			
//		}				
//	}
//	
//	return -1;	
//}


//// R:接收平台配置命令并应答
//// D:发送数据包并等待平台应答
//// H:发送心跳包并等待平台应答
//// L:发送登陆包并等待平台应答
//// T:触发上报当前数据
//int AT_BC26_UDP(char op)
//{
//	char cmd[64];	
//	uint16_t len, i, offset;
//	uint8_t buf[3072],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		if(op == 'R')
//		{
//			bsp_timer_star_once(TMR_RPT, TM_6S);												
//			s_ucATSta++; 				
//		}
//		else
//		{
//			s_ucATSta += 2;			
//		}		
//	}
//	else if(s_ucATSta == 1) //接收平台命令
//	{
//		if(comGetRxTimeOut(COM_RPT))
//		{
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));				
//				p = (uint8_t *)strstr((char *)buf, "+QIURC: \"recv\",0,");	
//				if(p != NULL)
//				{
//					
//					p += 17; 
//					if((p[1]==',') || (p[2]==','))
//					{
//					  if(p[1]==',')
//					  {
//						len = (p[0]-0x30)*2;                            
//						p += 2;                     
//					  }
//					  else if(p[2]==',')
//					  {
//						len = ((p[0]-0x30)*10 + p[1]-0x30)*2;                            
//						p += 3;
//					  }					
//					
//						str_to_byte((char *)p, s_ucaBuf, len*2);
//						memset(buf, 0, sizeof(buf));
//						memcpy(buf, s_ucaBuf, len);
//						s_usSize = up_cfg(s_ucaBuf, buf, len);
//						if(s_usSize)
//						{					
//							s_ucATSta++;
//						}
//					}
//				}				
//			}			
//		}
//		
//		if(bsp_timer_check(TMR_RPT))
//		{
//			return 1; 
//		}
//	}
//	else if(s_ucATSta > 20) //重发次数限制 /10
//	{
//		return 0;
//	}
//	else if((s_ucATSta == 2) || (s_ucATSta == 12))//整理发送数据
//	{
//		if(op == 'L')
//		{
//			s_usSize = up_login(s_ucaBuf);
//		}			
//		else if(op == 'D')
//		{
//			s_usSize = up_data(s_ucaBuf);			
//		}
//		else if(op == 'T')
//		{
//			s_usSize = up_trig(s_ucaBuf);			
//		}
//		else if(op == 'H')
//		{
//			s_usSize = up_heart(s_ucaBuf);			
//		}
//		
//		s_ucATSta++;		
//	}	
//	else if((s_ucATSta == 3) || (s_ucATSta == 13))//发送数据
//	{	
//		memset(buf,0,sizeof(buf));		
//		sprintf((char *)buf, "AT+QISENDEX=0,%d,", s_usSize);
//		offset = strlen((char *)buf);		
//		for(i=0; i<s_usSize; i++)
//		{
//			sprintf((char *)buf+offset+i*2,"%02X", *(s_ucaBuf+i));					
//		}
//		strcat((char *)buf, "\r\n");		
//		comSendBuf(COM_DEBUG, buf, strlen((char *)buf));
//		comClearRxFifo(COM_RPT);         
//		comSendBuf(COM_RPT, buf, strlen((char *)buf));		
//		bsp_timer_star_once(TMR_RPT, TM_6S);

//		if(op == 'R')
//		{
//			s_ucATSta = 0; 	
//		}
//		else
//		{
//			s_ucATSta++;
//		}						
//	}	
//	else if((s_ucATSta == 4) || (s_ucATSta == 14)) //平台应答
//	{
//		if(comGetRxTimeOut(COM_RPT))
//		{
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{								
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));				
//				
//				//判断平台应答
//				p = (uint8_t *)strstr((char *)buf,"16\r\n");	
//				if(p != NULL)
//				{
//					bsp_timer_star_once(TMR_RPT, TM_10S); //开启接收计时							
//					
//					if(op == 'D')  
//					{
//						uint32_t addr;
//						//上报数据包成功,修改并保存上报索引						
//						addr = OFFSET_OF(VAR_T, uiSPIDX);
//						var_WriteBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//						var_ReadBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//						printf("\r\n");		
//						printf("TPIDX:%u\r\n", g_tVar.uiTPIDX);			
//						printf("SPIDX:%u\r\n", g_tVar.uiSPIDX);
//						printf("\r\n");	
//						
//						if(g_tVar.uiSPIDX == g_tVar.uiTPIDX) return 1;
//						else s_ucATSta = 2;												
//					}
//					else
//					{					
//						return 1;
//					}
//				
//				}								
//					
//			}			
//		}
//		
//		if(bsp_timer_check(TMR_RPT)) // 超时处理  
//		{
////			if((op == 'L') || (op == 'H'))
////			{
////				return 1;
////			}

//			if(op == 'D')  
//			{
//				uint32_t addr;
//				//上报数据包成功,修改并保存上报索引						
//				addr = OFFSET_OF(VAR_T, uiSPIDX);
//				var_WriteBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//				var_ReadBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//				printf("\r\n");		
//				printf("TPIDX:%u\r\n", g_tVar.uiTPIDX);			
//				printf("SPIDX:%u\r\n", g_tVar.uiSPIDX);
//				printf("\r\n");	
//				
//				if(g_tVar.uiSPIDX == g_tVar.uiTPIDX) return 1;
//				else s_ucATSta = 2;												
//			}
//			else
//			{					
//				return 1;
//			}
//			
//			
//			if(s_ucATSta < 10)
//			{
//				s_ucATSta = 12;
//			}
//			else if(s_ucATSta < 20)
//			{
//				s_ucATSta = 22;					
//			}
//			else if(s_ucATSta < 30)
//			{
//				s_ucATSta = 32;					
//			}						
//		}		
//	}	
//	
//	return -1;			
//}
///******************************************************************************/
//int AT_QLWSERV(char op)
//{
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)    //重发次数限制 /2
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		memset(buf,0,sizeof(buf));
////		sprintf((char *)buf, "AT+QLWSERV=\042180.101.147.115\042,5683");
//		if(g_tVar.ucCNTP == 5) //AEP
//		{
//			sprintf((char *)buf, "AT+QLWSERV=\042221.229.214.202\042,5683");
//		}
//		else
//		{
//			sprintf((char *)buf, "AT+QLWSERV=\042117.60.157.137\042,5683");	
//		}
//			
//		AT_SendCmd((char *)buf);
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));				
//				p = (uint8_t *)strstr((char *)buf,"\r\nOK\r\n");	
//				if(p != NULL)
//				{
//					return 1;   
//				}			
//			}							
//		}
//	}
//		
//	return -1;	
//}

//int AT_QLWCONF(char op)
//{
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)    //重发次数限制 /2
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		memset(buf,0,sizeof(buf));
//		sprintf((char *)buf, "AT+QLWCONF=\"%s\"", g_tVar.caIMEI);		
//		AT_SendCmd((char *)buf);
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));				
//				p = (uint8_t *)strstr((char *)buf,"\r\nOK\r\n");	
//				if(p != NULL)
//				{
//					return 1;   
//				}			
//			}							
//		}
//	}
//		
//	return -1;	
//}

//int AT_QLWADDOBJ(char op)
//{
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 20)    //重发次数限制 /10
//	{
//		return 0;
//	}	
//	else if((s_ucATSta == 1) || (s_ucATSta == 11))
//	{
//		memset(buf,0,sizeof(buf));
//		sprintf((char *)buf, "AT+QLWADDOBJ=19,0,1,\0420\042");		
//		AT_SendCmd((char *)buf);
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else  if((s_ucATSta == 2) || (s_ucATSta == 12))
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta = s_ucATSta/10 + 11;			
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));				
//				p = (uint8_t *)strstr((char *)buf,"\r\nOK\r\n");	
//				if(p != NULL)
//				{
//					s_ucATSta++;   
//				}			
//			}							
//		}
//	}
//	else if((s_ucATSta == 3) || (s_ucATSta == 13))
//	{
//		memset(buf,0,sizeof(buf));
//		sprintf((char *)buf, "AT+QLWADDOBJ=19,1,1,\0420\042");		
//		AT_SendCmd((char *)buf);
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else  if((s_ucATSta == 4) || (s_ucATSta == 14))
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta = s_ucATSta/10 + 13;			
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));				
//				p = (uint8_t *)strstr((char *)buf,"\r\nOK\r\n");	
//				if(p != NULL)
//				{
//					return 1;   
//				}			
//			}							
//		}
//	}
//			
//	return -1;	
//}

//int AT_QLWOPEN(char op)
//{
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{	
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 2)    //重发次数限制 /2
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{	
//		AT_SendCmd("AT+QLWOPEN=0");
//		bsp_timer_star_once(TMR_RPT, TM_128S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;	
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));				
////				p = (uint8_t *)strstr((char *)buf,"\r\nCONNECT OK\r\n");	
//				p = (uint8_t *)strstr((char *)buf,"+QLWOBSERVE: 0,19,0,0");
//				if(p != NULL)
//				{
//					return 1;   
//				}
//				p = (uint8_t *)strstr((char *)buf,"\r\nCONNECT FAIL\r\n");	
//				if(p != NULL)
//				{
//					s_ucATSta++;
//				}				
//			}							
//		}
//	}
//		
//	return -1;	
//}

//int AT_QLWCFG(char op)
//{
//	static uint8_t ret;
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		ret = 0;
//		s_ucATSta++;		
//	}
//	if(s_ucATSta == 1)
//	{
//		bsp_timer_star_once(TMR_RPT, TM_1S);
//		s_ucATSta++;		
//	}
//	if(s_ucATSta == 2)
//	{
//		if(bsp_timer_check(TMR_RPT))
//		{
//			s_ucATSta++;
//		}			
//	}	
//	else if(s_ucATSta > 6)    //重发次数限制 /2
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{	
//		AT_SendCmd("AT+QLWCFG=\042dataformat\042,1,1");
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
////			if(ret) return 1;
////			else s_ucATSta++;	
//			s_ucATSta++;			
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));				
//				p = (uint8_t *)strstr((char *)buf,"\r\nOK\r\n");	
//				if(p != NULL)
//				{
////					ret	= 1;				
//					return 1;   
//				}			
//			}							
//		}
//	}
//		
//	return -1;	
//}

//// R:接收平台配置命令并应答
//// D:发送数据包并等待平台应答
//// H:发送心跳包并等待平台应答
//// L:发送登陆包并等待平台应答
//// T:触发上报当前数据
//int AT_BC26_LwM2M(char op)
//{
//	char cmd[64];	
//	uint16_t len, i, offset;
//	uint8_t buf[3072],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		if(op == 'R')
//		{			
//			bsp_timer_star_once(TMR_RPT, TM_6S);
//			s_ucATSta++;			
//		}
//		else
//		{
//			s_ucATSta += 2;			
//		}		
//	}
//	else if(s_ucATSta == 1) //接收平台命令
//	{
//		if(comGetRxTimeOut(COM_RPT))
//		{
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));
//				memset(cmd,0,sizeof(cmd));
//				sprintf((char *)cmd, "+QLWDATARECV: 19,1,0,");					
//				p = (uint8_t *)strstr((char *)buf, cmd);	
//				if(p != NULL)
//				{
//					p += strlen(cmd);
//					len = atoi((const char *)p);
//					printf("\r\nlen: %d", len);					
//					p = (uint8_t *)strstr((char *)p, ",");
//					if(p != NULL)
//					{
//						p += 1;
//						str_to_byte((char *)p, s_ucaBuf, len*2);
//						memset(buf, 0, sizeof(buf));
//						memcpy(buf, s_ucaBuf, len);
//						s_usSize = var_cfg((char *)buf, (char *)s_ucaBuf);
//						if(s_usSize == 0)
//						{
//							s_usSize = up_cfg(s_ucaBuf, buf, len);
//						}
//						if(s_usSize)
//						{
//							s_ucATSta++;
//						}
//					}
//				}				
//			}			
//		}
//		
//		if(bsp_timer_check(TMR_RPT))
//		{
//			return 1; 
//		}
//	}
//	else if(s_ucATSta > 20) //重发次数限制 /10
//	{
//		return 0;
//	}
//	else if((s_ucATSta == 2) || (s_ucATSta == 12))//整理发送数据
//	{
//		if(op == 'L')
//		{
//			s_usSize = up_login(s_ucaBuf);
//		}			
//		else if(op == 'D')
//		{
//			s_usSize = up_data(s_ucaBuf);			
//		}
//		else if(op == 'T')
//		{
//			s_usSize = up_trig(s_ucaBuf);			
//		}
//		else if(op == 'H')
//		{
//			s_usSize = up_heart(s_ucaBuf);			
//		}
//		
//		s_ucATSta++;		
//	}	
//	else if((s_ucATSta == 3) || (s_ucATSta == 13))//发送数据
//	{
//		memset(buf,0,sizeof(buf));			
//		sprintf((char *)buf, "AT+QLWDATASEND=19,0,0,%d,", s_usSize);
//		offset = strlen((char *)buf);		
//		for(i=0; i<s_usSize; i++)
//		{
//			sprintf((char *)buf+offset+i*2,"%02X", *(s_ucaBuf+i));					
//		}
//		strcat((char *)buf, ",0x0000\r\n");		
//		comSendBuf(COM_DEBUG, buf, strlen((char *)buf));
//		comClearRxFifo(COM_RPT);         
//		comSendBuf(COM_RPT, buf, strlen((char *)buf));		
//		bsp_timer_star_once(TMR_RPT, TM_6S);

//		if(op == 'R')
//		{
//			s_ucATSta = 0; 	
//		}
//		else
//		{
//			s_ucATSta++;
//		}						
//	}	
//	else if((s_ucATSta == 4) || (s_ucATSta == 14)) //平台应答
//	{
//		if(comGetRxTimeOut(COM_RPT))
//		{
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{								
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));	
//				
//				s_usSize = var_cfg((char *)buf, (char *)s_ucaBuf);
//				if(s_usSize)
//				{
//					s_ucATSta--;
//				}
//				else
//				{
//					//判断平台应答
//					p = (uint8_t *)strstr((char *)buf,"+QLWDATARECV: 19,1,0,15");	
//					if(p != NULL)
//					{
//						bsp_timer_star_once(TMR_RPT, TM_10S); //开启接收计时							
//						
//						if(op == 'D')  
//						{
//							uint32_t addr;
//							//上报数据包成功,修改并保存上报索引						
//							addr = OFFSET_OF(VAR_T, uiSPIDX);
//							var_WriteBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//							var_ReadBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//							printf("\r\n");		
//							printf("TPIDX:%u\r\n", g_tVar.uiTPIDX);			
//							printf("SPIDX:%u\r\n", g_tVar.uiSPIDX);
//							printf("\r\n");	
//							
//							if(g_tVar.uiSPIDX == g_tVar.uiTPIDX) return 1;
//							else s_ucATSta = 2;												
//						}
//						else
//						{					
//							return 1;
//						}				
//					}					
//					
//				}
//																
//			}			
//		}
//		
//		if(bsp_timer_check(TMR_RPT)) // 超时处理  
//		{
//			if((op == 'L') || (op == 'H'))
//			{
//				return 1;
//			}
//			
//			if(s_ucATSta < 10)
//			{
//				s_ucATSta = 12;
//			}
//			else if(s_ucATSta < 20)
//			{
//				s_ucATSta = 22;					
//			}
//						
//		}		
//	}	
//	
//	return -1;			
//}

///******************************************************************************/

//// R:接收平台配置命令并应答
//// D:发送数据包并等待平台应答
//// H:发送心跳包并等待平台应答
//// L:发送登陆包并等待平台应答
//// T:触发上报当前数据
//int AT_EC20_UDP(char op)
//{
//	char cmd[64];	
//	uint16_t len, i;
//	uint8_t buf[3072],*p, ch;
//	
//	if(s_ucATSta == 0)
//	{
//		if(op == 'R')
//		{			
//			bsp_timer_star_once(TMR_RPT, TM_6S);			
//			s_ucATSta++;			
//		}
//		else
//		{
//			s_ucATSta += 2;			
//		}		
//	}
//	else if(s_ucATSta == 1) //接收平台命令
//	{
//		if(comGetRxTimeOut(COM_RPT))
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				memset(cmd,0,sizeof(cmd));
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);
//					sprintf((char *)cmd,"%02X", buf[i]);					
//					comSendBuf(COM_DEBUG, (uint8_t *)cmd, strlen(cmd));						
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);				
//				if(g_tVar.ucSVTYPE == 0)
//				{					
//					s_usSize = up_cfg(s_ucaBuf, buf, len);
//				}
//				else if(g_tVar.ucSVTYPE == 1)
//				{
//					s_usSize = up1_cfg(s_ucaBuf, buf, len);					
//				}
//				if(s_usSize)
//				{					
//					s_ucATSta++;
//				}
//			}			
//		}
//		
//		if(bsp_timer_check(TMR_RPT))
//		{
//			return 1; 
//		}
//	}
//	else if(s_ucATSta > 20) //重发次数限制 /10
//	{
//		return 0;
//	}
//	else if((s_ucATSta == 2) || (s_ucATSta == 12))//指定发送数据长度
//	{		
//		if(g_tVar.ucSVTYPE == 0)
//		{		
//			if(op == 'L')
//			{			
//				s_usSize = up_login(s_ucaBuf);			
//			}		
//			else if(op == 'D')
//			{				
//				s_usSize = up_data(s_ucaBuf);			
//			}
//			else if(op == 'T')
//			{			
//				s_usSize = up_trig(s_ucaBuf);			
//			}
//			else if(op == 'H')
//			{			
//				s_usSize = up_heart(s_ucaBuf);			
//			}
//		}
//#ifndef __SL_XF01__	
//		else if(g_tVar.ucSVTYPE == 1)//物联网大数据云平台
//		{		
//			if(op == 'L')
//			{			
//				s_usSize = up1_login(s_ucaBuf);			
//			}		
//			else if(op == 'D')
//			{				
//				s_usSize = up1_data(s_ucaBuf);			
//			}
//			else if(op == 'T')
//			{			
//				s_usSize = up1_trig(s_ucaBuf);			
//			}
//			else if(op == 'H')
//			{			
//				s_usSize = up1_heart(s_ucaBuf);			
//			}
//		}
//#else
//		else if(g_tVar.ucSVTYPE == 1)//物联网大数据云平台
//		{		
//			if(op == 'L')
//			{			
//				s_usSize = up1_login(s_ucaBuf);			
//			}		
//			else if(op == 'D')
//			{				
//				s_usSize = up1_data(s_ucaBuf);			
//			}
//			else if(op == 'T')
//			{			
//				s_usSize = up1_trig(s_ucaBuf);			
//			}
//			else if(op == 'H')
//			{			
//				s_usSize = up1_heart(s_ucaBuf);			
//			}
//		}
//#endif		
//		comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//		memset(cmd,0,sizeof(cmd));
//		sprintf((char *)cmd, "AT+QISEND=0,%d", s_usSize);
//		AT_SendCmd(cmd);
//		bsp_timer_star_once(TMR_RPT, TM_3S);		
//		s_ucATSta++;		
//	}	
//	else if((s_ucATSta == 3) || (s_ucATSta == 13))//等待输入发送数据
//	{		
//		if(comGetRxTimeOut(COM_RPT))
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);				
//				p = (uint8_t *)strstr((char *)buf,">");	
//				if(p != NULL)
//				{
//					s_ucATSta++; 
//				}			
//			}			
//		}
//		
//		if(bsp_timer_check(TMR_RPT)) // 超时处理
//		{
//			if(s_ucATSta < 10)
//			{
//				s_ucATSta = 12;
//			}
//			else if(s_ucATSta < 20)
//			{
//				s_ucATSta = 22;					
//			}
//					
//		}			
//	}
//	else if((s_ucATSta == 4) || (s_ucATSta == 14))//发送数据
//	{
//		memset(buf, 0, sizeof(buf));
//		for(i=0; i<s_usSize; i++)
//		{
//			sprintf((char *)buf+i*2,"%02X", *(s_ucaBuf+i));		//转换成字符形式发送出来			
//		}
//		strcat((char *)buf, "\r\n");				
//		comSendBuf(COM_DEBUG, buf, strlen((char *)buf));	
//		comClearRxFifo(COM_RPT);
//		comSendBuf(COM_RPT, s_ucaBuf, s_usSize);		
//		bsp_timer_star_once(TMR_RPT, TM_6S);

//		if(op == 'R')
//		{
//			s_ucATSta = 0; 	
//		}
//		else
//		{
//			s_ucATSta++;			
//		}						
//	}
//	else if((s_ucATSta == 5) || (s_ucATSta == 15)) //平台应答
//	{
//		if(comGetRxTimeOut(COM_RPT))
//		{		
//			len = comGetBytes(COM_RPT, buf, sizeof(buf));
//			if(len)
//			{
//				comSendBuf(COM_DEBUG, buf, len);
//				
//				p = (uint8_t *)strstr((char *)buf,"+QIURC: \"recv\",0,");		
//				if(p != NULL)
//				{										
//					if(op == 'D')  
//					{
//						uint32_t addr;
//						//上报数据包成功,修改并保存上报索引						
//						addr = OFFSET_OF(VAR_T, uiSPIDX);
//						var_WriteBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//						var_ReadBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//						printf("\r\n");		
//						printf("TPIDX:%u\r\n", g_tVar.uiTPIDX);			
//						printf("SPIDX:%u\r\n", g_tVar.uiSPIDX);
//						printf("\r\n");	
//					
//						if(g_tVar.uiSPIDX == g_tVar.uiTPIDX) return 1;
//						else s_ucATSta = 2;												
//					}
//					else
//					{							
//						return 1;
//					}
//					
//				}				
//			}
//		}			
//		
//		if(bsp_timer_check(TMR_RPT)) // 超时处理  
//		{
//			if((op == 'L')  || (op == 'T')|| (op == 'H'))
//			{
//				return 1;
//			}
//			
//			if(op == 'D')  
//			{
//				if(g_tVar.uiSPIDX == g_tVar.uiTPIDX) 
//				{
//					uint32_t addr;
//					//上报数据包成功,修改并保存上报索引						
//					addr = OFFSET_OF(VAR_T, uiSPIDX);
//					var_WriteBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//					var_ReadBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//					printf("\r\n");		
//					printf("TPIDX:%u\r\n", g_tVar.uiTPIDX);			
//					printf("SPIDX:%u\r\n", g_tVar.uiSPIDX);
//					printf("\r\n");	

//					return 1;
//				}
//				else s_ucATSta = 2;												
//			}
//			else
//			{						
//				if(s_ucATSta < 10)
//				{
//					s_ucATSta = 12;
//				}
//				else if(s_ucATSta < 20)
//				{
//					s_ucATSta = 22;					
//				}
//			}			
//		}		
//	}	
//	
//	return -1;			
//}

//int AT_L610_UDP(char op)
//{
//	char cmd[64];	
//	uint16_t len, i;
//	uint8_t buf[3072],*p, ch;
//	
//	if(s_ucATSta == 0)
//	{
//		if(op == 'R')
//		{			
//			bsp_timer_star_once(TMR_RPT, TM_6S);			
//			s_ucATSta++;			
//		}
//		else
//		{
//			s_ucATSta += 2;			
//		}		
//	}
//	else if(s_ucATSta == 1) //接收平台命令
//	{
//		if(comGetRxTimeOut(COM_RPT))
//		{			
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				memset(cmd,0,sizeof(cmd));
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);
//					if(g_tVar.ucMODULE == MD_L610)
//					{
//						comGetChar(COM_DEBUG, buf+i);						
//					}
//					else
//					{
//						sprintf((char *)cmd,"%02X", buf[i]);					
//						comSendBuf(COM_DEBUG, (uint8_t *)cmd, strlen(cmd));	
//					}						
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);	

//				if(g_tVar.ucSVTYPE == 0)
//				{					
//					s_usSize = up_cfg(s_ucaBuf, buf, len);
//				}
//				else if(g_tVar.ucSVTYPE == 1)
//				{
//					s_usSize = up1_cfg(s_ucaBuf, buf, len);					
//				}
//				if(s_usSize)
//				{					
//					s_ucATSta++;
//				}
//			}			
//		}
//		
//		if(bsp_timer_check(TMR_RPT))
//		{
//			return 1; 
//		}
//	}
//	else if(s_ucATSta > 20) //重发次数限制 /10
//	{
//		return 0;
//	}
//	else if((s_ucATSta == 2) || (s_ucATSta == 12))//指定发送数据长度
//	{		
//		if(g_tVar.ucSVTYPE == 0)
//		{		
//			if(op == 'L')
//			{			
//				s_usSize = up_login(s_ucaBuf);			
//			}		
//			else if(op == 'D')
//			{				
//				s_usSize = up_data(s_ucaBuf);			
//			}
//			else if(op == 'T')
//			{			
//				s_usSize = up_trig(s_ucaBuf);			
//			}
//			else if(op == 'H')
//			{			
//				s_usSize = up_heart(s_ucaBuf);			
//			}
//		}
//		else if(g_tVar.ucSVTYPE == 1)
//		{		
//			if(op == 'L')
//			{			
//				s_usSize = up1_login(s_ucaBuf);			
//			}		
//			else if(op == 'D')
//			{				
//				s_usSize = up1_data(s_ucaBuf);			
//			}
//			else if(op == 'T')
//			{			
//				s_usSize = up1_trig(s_ucaBuf);			
//			}
//			else if(op == 'H')
//			{			
//				s_usSize = up1_heart(s_ucaBuf);			
//			}
//		}
//		
//		comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//		memset(cmd,0,sizeof(cmd));
//		sprintf((char *)cmd, "AT+MIPSEND=1,%d", s_usSize);
//		AT_SendCmd(cmd);
//		bsp_timer_star_once(TMR_RPT, TM_3S);		
//		s_ucATSta++;		
//	}	
//	else if((s_ucATSta == 3) || (s_ucATSta == 13))//等待输入发送数据
//	{		
//		if(comGetRxTimeOut(COM_RPT))
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);				
//				p = (uint8_t *)strstr((char *)buf,">");	
//				if(p != NULL)
//				{
//					s_ucATSta++; 
//				}			
//			}			
//		}
//		
//		if(bsp_timer_check(TMR_RPT)) // 超时处理
//		{
//			if(s_ucATSta < 10)
//			{
//				s_ucATSta = 12;
//			}
//			else if(s_ucATSta < 20)
//			{
//				s_ucATSta = 22;					
//			}
//						
//		}			
//	}
//	else if((s_ucATSta == 4) || (s_ucATSta == 14))//发送数据
//	{
//		memset(buf, 0, sizeof(buf));
//		for(i=0; i<s_usSize; i++)
//		{
//			sprintf((char *)buf+i*2,"%02X", *(s_ucaBuf+i));					
//		}
//		strcat((char *)buf, "\r\n");				
//		comSendBuf(COM_DEBUG, buf, strlen((char *)buf));	
//		comClearRxFifo(COM_RPT);
//		comSendBuf(COM_RPT, s_ucaBuf, s_usSize);		
//		bsp_timer_star_once(TMR_RPT, TM_6S);

//		if(op == 'R')
//		{
//			s_ucATSta = 0; 	
//		}
//		else
//		{
//			s_ucATSta++;			
//		}						
//	}
//	else if((s_ucATSta == 5) || (s_ucATSta == 15)) //平台应答
//	{
//		if(comGetRxTimeOut(COM_RPT))
//		{		
//			len = comGetBytes(COM_RPT, buf, sizeof(buf));
//			if(len)
//			{
//				comSendBuf(COM_DEBUG, buf, len);
//				if(g_tVar.ucCNTP == 1)
//				{
//					p = (uint8_t *)strstr((char *)buf,"+MIPRTCP:");	
//				}
//				else
//				{
//					p = (uint8_t *)strstr((char *)buf,"+MIPRUDP:");					
//				}
//				if(p != NULL)
//				{										
//					if(op == 'D')  
//					{
//						uint32_t addr;
//						//上报数据包成功,修改并保存上报索引						
//						addr = OFFSET_OF(VAR_T, uiSPIDX);
//						var_WriteBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//						var_ReadBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//						printf("\r\n");		
//						printf("TPIDX:%u\r\n", g_tVar.uiTPIDX);			
//						printf("SPIDX:%u\r\n", g_tVar.uiSPIDX);
//						printf("\r\n");	
//						
//						if(g_tVar.uiSPIDX == g_tVar.uiTPIDX) return 1;
//						else s_ucATSta = 2;												
//					}
//					else
//					{							
//						return 1;
//					}
//					
//				}				
//			}
//		}			
//		
//		if(bsp_timer_check(TMR_RPT)) // 超时处理  
//		{
//			if((op == 'L')  || (op == 'T')|| (op == 'H'))
//			{
//				return 1;
//			}
//			
//			if(op == 'D')  
//			{
//				if(g_tVar.uiSPIDX == g_tVar.uiTPIDX) 
//				{
//					uint32_t addr;
//					//上报数据包成功,修改并保存上报索引						
//					addr = OFFSET_OF(VAR_T, uiSPIDX);
//					var_WriteBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//					var_ReadBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//					printf("\r\n");		
//					printf("TPIDX:%u\r\n", g_tVar.uiTPIDX);			
//					printf("SPIDX:%u\r\n", g_tVar.uiSPIDX);
//					printf("\r\n");	

//					return 1;
//				}
//				else s_ucATSta = 2;												
//			}
//			else
//			{						
//				if(s_ucATSta < 10)
//				{
//					s_ucATSta = 12;
//				}
//				else if(s_ucATSta < 20)
//				{
//					s_ucATSta = 22;					
//				}
//			}			
//		}		
//	}	
//	
//	return -1;			
//}

//void AT_ResetUpdataSta(void)
//{
//	s_ucUpdataSta = 0;	
//}

//int AT_UPDATA(char op)
//{
//	int ret = -1;
//	uint32_t i, len;
//	char time[64] = {0};	
//	uint8_t buf[3072] ={0};
//	char cmd[64] = {0};		
//	char dtusn[16] = {0};	
//	uint8_t sta, *p, *q, *ptr;
//	uint32_t temp, addr, crc32;
//	
//	switch(sta = s_ucUpdataSta)
//	{
//	case 0x00: 
//		if(g_tVar.ucSMSVEN1) 
//		{
//			s_ucUpdataSta++;	
//		}
//		else
//		{
//			s_ucUpdataSta = 0xFF;			
//		}
//		break;		

//	case 0x01: 
//		if(g_tVar.ucMODULE == MD_L610)
//		{
//			ret = AT_ACK((char *)"AT+MIPCLOSE=1,0\r\n", (char *)"+MIPCLOSE: 1,0",(char *)buf, sizeof(buf), TM_10S, 1);
//		}
//		else
//		{			
//			if(g_tVar.ucMODULE == MD_M26) sprintf((char *)buf, "AT+QICLOSE\r\n");
//			else 						  sprintf((char *)buf, "AT+QICLOSE=0\r\n");
//			ret = AT_ACK((char *)buf, (char *)"OK\r\n", (char *)buf, sizeof(buf), TM_5S, 1);
//		}
//		if(ret >= 0) s_ucUpdataSta++;		
//		break;

//	case 0x02: //连接设备管理平台
//		if(g_tVar.ucMODULE == MD_L610)
//		{
//			sprintf((char *)buf, "AT+MIPOPEN=1,,\042%s\042,%d,1\r\n", g_tVar.caSIP1,g_tVar.usSPORT1);
//			ret = AT_ACK((char *)buf, (char *)"+MIPOPEN: 1,1",(char *)buf, sizeof(buf), TM_30S, 1);
//		}		
//		else if(g_tVar.ucMODULE == MD_M26) 
//		{
//			sprintf((char *)buf, "AT+QIOPEN=\"UDP\",\"%s\",\"%d\"\r\n", g_tVar.caSIP1,g_tVar.usSPORT1);
//			ret = AT_ACK((char *)buf, (char *)"CONNECT OK",(char *)buf, sizeof(buf), TM_30S, 1);
//		}			
//		else 
//		{			
//			sprintf((char *)buf, "AT+QIOPEN=1,0,\"UDP\",\"%s\",%d,0,2\r\n", g_tVar.caSIP1,g_tVar.usSPORT1);
//			ret = AT_ACK((char *)buf, (char *)"CONNECT",(char *)buf, sizeof(buf), TM_30S, 1);			
//		}			
//		
//		if(ret == 1) s_ucUpdataSta ++;	
//		else if(ret == 0) s_ucUpdataSta++;	
//		else if(s_ucTimeout)
//		{
//			s_ucTimeout = 0;			
//			p = (uint8_t *)strstr((char *)buf,"ERROR");	
//			if(p != NULL) 
//			{
//				s_ucUpdataSta = 0xFF;
//			}				
//			else
//			{
//				p = (uint8_t *)strstr((char *)buf,"ALREADY CONNECT");	
//				if(p != NULL) s_ucUpdataSta++;	
//			}				
//		}	
//		break;	
//		
//	case 0x03: //登录包
//		memset(dtusn, 0, sizeof(dtusn));
//		bcd_to_str(g_tVar.ucaDTUSN, dtusn, 6);	
//		sprintf(time, "%02d-%02d-%02d %02d:%02d:%02d", g_tRTC.Year,g_tRTC.Mon,g_tRTC.Day,g_tRTC.Hour,g_tRTC.Min,g_tRTC.Sec);
//		sprintf((char *)s_ucaBuf, "{\"REG\":{\"IMEI\":\"%s\",\"ICCID\":\"%s\",\"VER_SF\":\"%s\",\"VER_HW\":\"SL-6200_V2.xx\",\"CSQ\":\"%d\",\"ERR_L\":\"%08X\",\"TS_RST\":\"%08X\",\"ADDR\":\"%s\",\"VCC\":\"%d\",\"TIME\":\"%s\"}}", 
//								g_tVar.caIMEI,g_tVar.caCCID,cpSTVS,g_ucRssi,(g_tVar.uiPlsCntP<<16)|g_usErr,RCC->CSR&0xFC000000,dtusn,g_tDat.battary, time);
//		g_usErr = 0, g_usRst = 0;
//		RCC->CSR |= RCC_CSR_RMVF;
//		s_usSize = strlen((char *)s_ucaBuf);
//		s_ucTryCnt = 0;
//		s_ucUpdataSta += 2;	
//		break;
//		
//	case 0x04: //上发文件下载请求
//		if((g_tVar.uiFileSize>=0x1E000) || (g_tVar.uiFileOffset>=0x1E000))//文件要小于120K
//		{			
//			printf("\r\nErr: FileSize=%ld FileOffset=%ld\r\n", (unsigned long)g_tVar.uiFileSize,(unsigned long)g_tVar.uiFileOffset);
//			
//			memset(g_tVar.caFileName, 0x00, sizeof(g_tVar.caFileName)); 
//			g_tVar.uiFileSize = 0;
//			g_tVar.uiFileOffset = 0;
//			
//			addr = OFFSET_OF(VAR_T, caFileName);
//			var_WriteBytes((uint8_t *)g_tVar.caFileName, addr, sizeof(g_tVar.caFileName));							

//			addr = OFFSET_OF(VAR_T, uiFileSize);
//			var_WriteBytes((uint8_t *)&g_tVar.uiFileSize, addr, 4);							

//			addr = OFFSET_OF(VAR_T, uiFileOffset);
//			var_WriteBytes((uint8_t *)&g_tVar.uiFileOffset, addr, 4);
//			
//			s_ucUpdataSta = 0xFF;		
//		}
//		else
//		{
//			sprintf((char *)s_ucaBuf, "UPDATA2,%s,", g_tVar.caFileName);
//			s_usSize = strlen((char *)s_ucaBuf);
//			memcpy(s_ucaBuf+s_usSize, (uint8_t *)&g_tVar.uiFileSize, 4); s_usSize += 4;		
//			s_ucaBuf[s_usSize++] = 0x07;
//			s_ucaBuf[s_usSize++] = 0x00; 
//			s_ucaBuf[s_usSize++] = 0x01;
//			memcpy(s_ucaBuf+s_usSize, (uint8_t *)&g_tVar.uiFileOffset, 4); s_usSize += 4;
//			s_ucaBuf[s_usSize++] = 0x00;
//			s_ucaBuf[s_usSize++] = 0x04; 		
//			s_ucaBuf[s_usSize] = add8_caculate(s_ucaBuf, s_usSize);	
//			s_usSize++;	
//			s_ucTryCnt++;	
//			s_ucUpdataSta++;
//		}			
//		break;

//	case 0x05: //数据包大小
//	    if(g_tVar.ucMODULE == MD_L610)	
//		{
//			sprintf((char *)cmd, "AT+MIPSEND=1,%d\r\n", s_usSize);
//			ret = AT_ACK((char *)cmd, (char *)">",(char *)buf, sizeof(buf), TM_3S, 1);
//			if(ret == 1)s_ucUpdataSta++;
//			else if(ret == 0)s_ucUpdataSta = 0xFF;
//		}			
//		else if(g_tVar.ucMODULE == MD_M26)
//		{
//			sprintf((char *)cmd, "AT+QISEND=%d\r\n", s_usSize);
//			ret = AT_ACK((char *)cmd, (char *)">",(char *)buf, sizeof(buf), TM_3S, 1);
//			if(ret == 1)s_ucUpdataSta++;
//			else if(ret == 0)s_ucUpdataSta = 0xFF;
//		}
//		else
//		{
//			s_ucUpdataSta++;		 
//		}		
//		break;

//	case 0x06: //发送数据
//		comClearRxFifo(COM_RPT);
//		comTxPrintfHex(COM_RPT, s_ucaBuf, s_usSize);	
//		bsp_timer_star_once(TMR_RPT, TM_1S);
//		if(g_tVar.ucMODULE == MD_M26) 
//		{
//			s_ucUpdataSta++;
//			bsp_timer_star_once(TMR_RPT, TM_3S);			
//		}
//		else 
//		{
//			bsp_timer_star_once(TMR_RPT, TM_15S);			
//			s_ucUpdataSta += 2;
//		}
//		break;

//	case 0x07: //发送完成		
//		if(bsp_timer_check(TMR_RPT))
//		{			
//			s_ucUpdataSta = 0xFF;			
//		}		
//		else if(comGetRxTimeOut(COM_RPT))					
//		{			
//			len = comRxPrintfHex(COM_RPT, (uint8_t *)buf, sizeof(buf));
//			q = (uint8_t *)strstr((char *)buf,"SEND OK");
//			if(q != NULL)
//			{
//				comClearRxFifo(COM_RPT);				
//				bsp_timer_star_once(TMR_RPT, TM_15S);
//				s_ucUpdataSta++;						
//			}									
//		}
//		break;		

//	case 0x08: 
//		if(bsp_timer_check(TMR_RPT))
//		{			
//			if(g_tVar.uiFileSize) //表示有未完成的文件
//			{				
//				if(s_ucTryCnt >= 3)
//				{
//					s_ucUpdataSta = 0xFF;
//				}
//				else
//				{
//					s_ucUpdataSta = 0x04;				
//				}
//			}
//			else
//			{
//					s_ucUpdataSta = 0xFF;				
//			}
//		}		
//		else if(comGetRxTimeOut(COM_RPT))					
//		{			
////			len = comRxPrintfHex(COM_RPT, (uint8_t *)buf, sizeof(buf));			
//			len = comGetBytes(COM_RPT, (uint8_t *)buf, sizeof(buf));
////			comSendBuf(COM_CFG, (uint8_t *)buf, len);
//			ptr = (uint8_t *)strstr((char *)buf, "UPDATA");
//			if(ptr != NULL)
//			{
//				len = len - (ptr - buf);
//				if(g_tVar.ucMODULE == MD_L610) len -= 2;
//				printf("\r\nlen=%d\r\n", len);				
//				if(add8_check(ptr, len))
//				{
//					if(ptr[6]=='1')
//					{
//						p = ptr+7+1;
//						q = (uint8_t *)strstr((char *)p,",");
//						if(q != NULL)
//						{
//							len = q - p;													
//							memset(g_tVar.caFileName, 0x00, sizeof(g_tVar.caFileName));
//							memcpy(g_tVar.caFileName, p, len);	
//							printf("\r\nFileName:%s\r\n", g_tVar.caFileName);								
//							addr = OFFSET_OF(VAR_T, caFileName);
//							var_WriteBytes((uint8_t *)g_tVar.caFileName, addr, sizeof(g_tVar.caFileName));							
//						}

//						p = q+1;					
//						memcpy((uint8_t *)&g_tVar.uiFileSize, p, 4);
//						printf("\r\nFileSize:%d\r\n", g_tVar.uiFileSize);
//						addr = OFFSET_OF(VAR_T, uiFileSize);
//						var_WriteBytes((uint8_t *)&g_tVar.uiFileSize, addr, 4);							

//						g_tVar.uiFileOffset = 0;
//						addr = OFFSET_OF(VAR_T, uiFileOffset);
//						var_WriteBytes((uint8_t *)&g_tVar.uiFileOffset, addr, 4);							
//						
//						s_ucUpdataSta = 0x04;						
//					}
//					else if(ptr[6]=='3') //服务器下发指定数据
//					{
//						p = ptr+7+1;
//						q = (uint8_t *)strstr((char *)p,",");
//						if(q != NULL)
//						{
//							p = q+1;
//							p += 4;	 //文件大小						
//							len = (p[1]<<8) | p[0]; //数据区数据的字节数
//							printf("\r\nlen:%d\r\n", len);
//							if(len)
//							{
//								p += 2;																
//								memcpy((uint8_t *)&temp, p, 4);//本帧数据在文件中的偏移
//								printf("\r\noffset:%d\r\n", temp);
//								if(len != 3) //"ESC"
//								{
//									if(temp == g_tVar.uiFileOffset)
//									{
//										p += 4;											
//										len -= 4;
//										
//										len = len / 4;
//										
//										FLASH_Unlock();										
//										for(i=0; i<len; i++,p+=4)
//										{
//											memcpy((uint8_t *)&temp, p, 4);
//											addr = 0x08020000 + g_tVar.uiFileOffset + (i<<2);
//											if((addr&0xFF) == 0)
//											{
//												if(FLASH_COMPLETE != FLASH_ErasePage(addr))
//												{
//													printf("\r\nErr:FLASH_ErasePage\r\n");		
//												}	
//											}
//											
//											if(FLASH_COMPLETE != FLASH_FastProgramWord(addr, temp))
//											{
//												printf("\r\nErr:FLASH_FastProgramWord\r\n");						
//											}											
//											
//										}
//										FLASH_Lock();
//										
//										s_ucTryCnt = 0;										
//										g_tVar.uiFileOffset += 1024;
//									    addr = OFFSET_OF(VAR_T, uiFileOffset);
//									    var_WriteBytes((uint8_t *)&g_tVar.uiFileOffset, addr, 4);
//									}
//									
//									if(g_tVar.uiFileOffset < g_tVar.uiFileSize)
//									{
//										s_ucUpdataSta = 0x04;
//									}
//									else
//									{	
//										temp = crc32_flash (0x08020000, g_tVar.uiFileSize);
//										printf("\r\nFlashCRC32:%04X\r\n", temp);
//							
//										p = (uint8_t *)g_tVar.caFileName;
//										q = (uint8_t *)strstr((char *)p,"(");
//										if(q != NULL)
//										{
//											p = q+1;
//											str_to_bcd((char *)p, (uint8_t *)&crc32, 8);
//											printf("\r\nFileCRC32:%04X\r\n", crc32);									
//										}
//											
//										if(temp  == crc32)
//										{
//											FLASH_Unlock();	
//											if(FLASH_COMPLETE != FLASH_ErasePage(0x0801FF00))
//											{
//												printf("\r\nErr:FLASH_ErasePage\r\n");		
//											}	
//											
//											if(FLASH_COMPLETE != FLASH_FastProgramWord(0x0801FF00, g_tVar.uiFileSize))
//											{
//												printf("\r\nErr:FLASH_FastProgramWord\r\n");						
//											}
//											else
//											{
//												NVIC_SystemReset();												
//											}
//											FLASH_Lock();
//										}
//				
//										g_tVar.uiFileSize = 0;
//										addr = OFFSET_OF(VAR_T, uiFileSize);
//										var_WriteBytes((uint8_t *)&g_tVar.uiFileSize, addr, 4);											
//										printf("\r\n下载完成\r\n");										
//									}
//								}
//								else
//								{
//									g_tVar.uiFileSize = 0;
//									addr = OFFSET_OF(VAR_T, uiFileSize);
//									var_WriteBytes((uint8_t *)&g_tVar.uiFileSize, addr, 4);																				
//									printf("\r\n取消下载\r\n");
//								}
//								
//							}
//							else
//							{
//								g_tVar.uiFileSize = 0;
//								addr = OFFSET_OF(VAR_T, uiFileSize);
//								var_WriteBytes((uint8_t *)&g_tVar.uiFileSize, addr, 4);																			
//								printf("\r\n下发完毕\r\n");
//							}
//							
//						}						
//					
//					}
//				}					
//			}
//			else
//			{
//				memset(s_ucaBuf, 0, sizeof(s_ucaBuf));
//				var_cfg((char *)buf, (char *)s_ucaBuf);
//				s_usSize = strlen((char *)s_ucaBuf);
//				if(s_usSize)
//				{
//					s_ucUpdataSta = 0x05;					
//				}
//				bsp_timer_star_once(TMR_RPT, TM_6S);
//			}
//			
//		}	
//		break;
//		
//	default: 
//		s_ucUpdataSta = 0x00;	
//		return 0;
//		break;			

//	}	
//	
//	if(sta != s_ucUpdataSta)
//	{
//		AT_ResetSta();		
//	}
//	
//	return -1;
//}


//int AT_OK_E(char *_Cmd,const char *resp, int sec, int times)
//{
//	uint16_t len, i;
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > times*2)
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		AT_SendCmd(_Cmd);
//		if(times==0)return 1;
//		bsp_timer_star_once(TMR_RPT, TM_1S * sec);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT))
//		{
//			s_ucATSta++;			
//		}
//		else if(comGetRxTimeOut(COM_RPT))
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);			
//				if(resp==NULL)
//				{
//                    p = (uint8_t *)strstr((char *)buf,"OK\r\n");    
//                    if(p != NULL)
//                    {
//                        return 1;               
//                    }
//				}
//				else
//				{
//                    p = (uint8_t *)strstr((char *)buf,resp);    
//                    if(p != NULL)
//                    {
//                        return 1;               
//                    }
//				}

//				if(strstr((char *)buf,"ERROR\r\n"))
//				{
//                    s_ucATSta++;    
//				}
//			}			
//	
//		}
//	}

//	return -1;	
//}

//int AT_BC260Y_CESQ(char op)
//{
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 40)    //重发次数限制 /2
//	{
//		return 1;
//	}
//	else if(s_ucATSta&0x01)
//	{
//		AT_SendCmd("AT+CESQ");
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));	
//				p = (uint8_t *)strstr((char *)buf,"OK\r\n");
//				if(p != NULL)			
//				{			
//                    //+CESQ: 99,99,255,255,19,45
//                    p = (uint8_t *)strstr((char *)buf,"+CESQ:");    
//                    if(p != NULL)
//                    {
//                        if(sscanf((char *)p,"%*[^ ] %*d,%*d,%*d,%*d,%d",(int *)&g_ucRssi)>0)
//                        {
//                            if((g_ucRssi >= 10)&&(g_ucRssi <= 34))
//                            {
//                                printf("CSQ:%d\r\n",g_ucRssi);
//                                return g_ucRssi;   
//                            }
//                        }                              
//                    }
//				}
//			}						
//		}
//	}
//	
//	return -1;	
//}

//int AT_BC260Y_CGPADDR(void)
//{
//	uint16_t len, i;
//	uint8_t buf[128],*p;
//	int cid,ip[4];
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 40)    //重发次数限制 /2
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		AT_SendCmd("AT+CGPADDR?");
//		bsp_timer_star_once(TMR_RPT, TM_1S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);	

//                //+CGPADDR: 0,"21.154.204.109","36.14.8.124.7.130.74.122.0.0.0.0.0.0.0.1"
//                p = (uint8_t *)strstr((char *)buf,"+CGPADDR:");    
//                if(p != NULL)
//                {
//					memset(ip, 0xff, sizeof(ip));
//					if(sscanf((char *)p,"%*[^,],%d.%d.%d.%d",&ip[0],&ip[1],&ip[2],&ip[3])==4)
//					{
//						if(((ip[0]>0)&&(ip[0]<255))&&((ip[1]>=0)&&(ip[1]<=255))&&
//						   ((ip[2]>=0)&&(ip[2]<=255))&&((ip[3]>=0)&&(ip[3]<=255)))
//						{
//							printf("Local IP:%d.%d.%d.%d\r\n",ip[0],ip[1],ip[2],ip[3]);
//							return 1;   
//						}
//					}
//					else if(sscanf((char *)p,"%*[^,],\"%d.%d.%d.%d",&ip[0],&ip[1],&ip[2],&ip[3])==4)
//					{
//						if(((ip[0]>0)&&(ip[0]<255))&&((ip[1]>=0)&&(ip[1]<=255))&&
//						   ((ip[2]>=0)&&(ip[2]<=255))&&((ip[3]>=0)&&(ip[3]<=255)))
//						{
//							printf("Local IP:%d.%d.%d.%d\r\n",ip[0],ip[1],ip[2],ip[3]);
//							return 1;   
//						}
//					}
//                }			
//			}							
//		}
//	}
//	return -1;	
//}

//int AT_BC260Y_QIOPEN(char op)
//{
//	uint16_t len, i;
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)  //重发次数限制 /3
//	{
//		return 0;		
//	}	
//	else if((s_ucATSta == 1) || (s_ucATSta == 5))
//	{
//		memset(buf,0,sizeof(buf));
//		if(g_tVar.ucCNTP == 1)
//		{			
//			sprintf((char *)buf, "AT+QIOPEN=0,0,\"TCP\",\"%s\",%d,0,1", g_tVar.caIP,g_tVar.usPORT);	
//		}
//		else
//		{
//			sprintf((char *)buf, "AT+QIOPEN=0,0,\"UDP\",\042%s\042,%d,0,1", g_tVar.caIP,g_tVar.usPORT);				
//		}
//		AT_SendCmd((char *)buf);		
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;		
//	}
//	else if((s_ucATSta == 2) || (s_ucATSta == 6))
//	{
//		if(bsp_timer_check(TMR_RPT))
//		{
//			s_ucATSta++;
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);	
//				p = (uint8_t *)strstr((char *)buf,"+QIOPEN: 0,0");				
//				if(p != NULL)
//				{
//					return 1;
//				}
//			}			
//		}				
//	}
//	else if(s_ucATSta == 3)			
//	{
//		AT_SendCmd("AT+QICLOSE=0");
//		bsp_timer_star_once(TMR_RPT, TM_3S);
//		s_ucATSta++;
//	}
//	else if(s_ucATSta == 4)
//	{
//		if(bsp_timer_check(TMR_RPT)) // 超时处理
//		{
//			s_ucATSta++;
//		}		
//		else if(comGetRxTimeOut(COM_RPT))	
//		{
//			len = comGetRxCount(COM_RPT);
//			memset(buf, 0, sizeof(buf));
//			if(len)
//			{
//				for(i=0; i<len; i++)			
//				{
//					comGetChar(COM_RPT, buf+i);				
//				}
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, len);
//				p = (uint8_t *)strstr((char *)buf,"+QIOPEN: 0,0");					
////				p = (uint8_t *)strstr((char *)buf,"CLOSE");	
//				if(p != NULL)
//				{
//					s_ucATSta++;   
//				}
//				
//			}			
//		}				
//	}
//	
//	return -1;	
//}



//int AT_BC260Y_NCDPOPEN(char op)
//{
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{	
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 2)    //重发次数限制 /2
//	{
//		return 0;
//	}	
//	else if(s_ucATSta&0x01)
//	{	
//        if(g_tVar.ucCNTP == 4)
//        {
//            sprintf((char *)buf, "AT+NCDPOPEN=\"117.60.157.137\",5683");
//        }
//        else if(g_tVar.ucCNTP == 5)
//        {
//            sprintf((char *)buf, "AT+NCDPOPEN=\"221.229.214.202\",5683");
//        }
//        else
//        {
//            sprintf((char *)buf, "AT+NCDPOPEN=\"%s\",5683",g_tVar.caIP);
//        }
//        AT_SendCmd((char *)buf);	
//		bsp_timer_star_once(TMR_RPT, TM_128S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;	
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));				
//				p = (uint8_t *)strstr((char *)buf,"+QLWEVTIND: 3");
//				if(p != NULL)
//				{
//					return 1;   
//				}
//				p = (uint8_t *)strstr((char *)buf,"+QLWEVTIND: 1");	
//				if(p != NULL)
//				{
//					s_ucATSta++;
//				}		
//				p = (uint8_t *)strstr((char *)buf,"ERROR");	
//				if(p != NULL)
//				{
//					s_ucATSta++;
//				}	
//			}							
//		}
//	}
//	return -1;	
//}

//// R:接收平台配置命令并应答
//// D:发送数据包并等待平台应答
//// H:发送心跳包并等待平台应答
//// L:发送登陆包并等待平台应答
//// T:触发上报当前数据
//int AT_BC260Y_AEP(char op)
//{
//	char cmd[64];	
//	uint16_t i, offset;
//	uint8_t buf[3072],*p;
//	uint32_t len;
//	
//	if(s_ucATSta == 0)
//	{
//		if(op == 'R')
//		{			
//			bsp_timer_star_once(TMR_RPT, TM_6S);
//			s_ucATSta++;			
//		}
//		else
//		{
//			s_ucATSta += 2;			
//		}		
//	}
//	else if(s_ucATSta == 1) //接收平台命令
//	{
//		if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));
//				//+NNMI: 4,AAAA0000
//				p = (uint8_t *)strstr((char *)buf, "+NNMI:");	
//				if(p != NULL)
//				{
//    				if((sscanf((char *)p,"%*[^ ] %d,",&len)==1) && len)
//    				{
//                        p = (uint8_t *)strstr((char *)p, ",");
//                        if(p != NULL)
//                        {
//                            p += 1;
//                            str_to_byte((char *)p, s_ucaBuf, len*2);
//                            memset(buf, 0, sizeof(buf));
//                            memcpy(buf, s_ucaBuf, len);
//                            for(i=0; i<len; i++) printf("%02X",buf[i]);
//                            s_usSize = var_cfg((char *)buf, (char *)s_ucaBuf);
//                            if(s_usSize == 0)
//                            {
//                                s_usSize = up_cfg(s_ucaBuf, buf, len);
//                            }
//                            if(s_usSize)
//                            {
//                                s_ucATSta++;
//                            }
//                        }
//    				}
//				}
//			}			
//		}
//		
//		if(bsp_timer_check(TMR_RPT))
//		{
//			return 1; 
//		}
//	}
//	else if(s_ucATSta > 20) //重发次数限制 /10
//	{
//		return 0;
//	}
//	else if((s_ucATSta == 2) || (s_ucATSta == 12))//整理发送数据
//	{
//		if(op == 'L')
//		{
//			s_usSize = up_login(s_ucaBuf);
//		}			
//		else if(op == 'D')
//		{
//			s_usSize = up_data(s_ucaBuf);			
//		}
//		else if(op == 'T')
//		{
//			s_usSize = up_trig(s_ucaBuf);			
//		}
//		else if(op == 'H')
//		{
//			s_usSize = up_heart(s_ucaBuf);			
//		}		
//		s_ucATSta++;		
//	}	
//	else if((s_ucATSta == 3) || (s_ucATSta == 13))//发送数据
//	{
//    	//AT+NMGS=5,00454c4c4f
//		memset(buf,0,sizeof(buf));			
//		sprintf((char *)buf, "AT+NMGS=%d,\"", s_usSize);
//		offset = strlen((char *)buf);		
//		for(i=0; i<s_usSize; i++)
//		{
//			sprintf((char *)buf+offset+i*2,"%02X", *(s_ucaBuf+i));					
//		}
//		strcat((char *)buf,"\"\r");
//		comSendBuf(COM_DEBUG, buf, strlen((char *)buf));
//		comClearRxFifo(COM_RPT);         
//		comSendBuf(COM_RPT, buf, strlen((char *)buf));		
//		bsp_timer_star_once(TMR_RPT, TM_6S);

//		if(op == 'R')
//		{
//			s_ucATSta = 0; 	
//		}
//		else
//		{
//			s_ucATSta++;
//		}						
//	}	
//	else if((s_ucATSta == 4) || (s_ucATSta == 14)) //平台应答
//	{
//		if(comGetRxTimeOut(COM_RPT))
//		{
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{								
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));	
//				
//				s_usSize = var_cfg((char *)buf, (char *)s_ucaBuf);
//				if(s_usSize)
//				{
//					s_ucATSta--;
//				}
//				else
//				{
//					//判断平台应答
//					p = (uint8_t *)strstr((char *)buf,"+NNMI: 15");	
//					if(p != NULL)
//					{
//						bsp_timer_star_once(TMR_RPT, TM_6S); //开启接收计时							
//						
//						if(op == 'D')  
//						{
//							uint32_t addr;
//							//上报数据包成功,修改并保存上报索引						
//							addr = OFFSET_OF(VAR_T, uiSPIDX);
//							var_WriteBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//							var_ReadBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//							printf("\r\n");		
//							printf("TPIDX:%u\r\n", g_tVar.uiTPIDX);			
//							printf("SPIDX:%u\r\n", g_tVar.uiSPIDX);
//							printf("\r\n");	
//							
//							if(g_tVar.uiSPIDX == g_tVar.uiTPIDX) return 1;
//							else s_ucATSta = 2;												
//						}
//						else
//						{					
//							return 1;
//						}				
//					}					
//					
//				}
//																
//			}			
//		}
//		
//		if(bsp_timer_check(TMR_RPT)) // 超时处理  
//		{
//			if((op == 'L') || (op == 'H'))
//			{
//				return 1;
//			}
//			
//			if(s_ucATSta < 10)
//			{
//				s_ucATSta = 12;
//			}
//			else if(s_ucATSta < 20)
//			{
//				s_ucATSta = 22;					
//			}
//						
//		}		
//	}	
//	
//	return -1;			
//}


//// R:接收平台配置命令并应答
//// D:发送数据包并等待平台应答
//// H:发送心跳包并等待平台应答
//// L:发送登陆包并等待平台应答
//// T:触发上报当前数据
//int AT_BC260Y_UDP(char op)
//{
//	char cmd[64];	
//	uint16_t len, i, offset;
//	uint8_t buf[3072],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		if(op == 'R')
//		{
//			bsp_timer_star_once(TMR_RPT, TM_6S);												
//			s_ucATSta++; 				
//		}
//		else
//		{
//			s_ucATSta += 2;			
//		}		
//	}
//	else if(s_ucATSta == 1) //接收平台命令
//	{
//		if(comGetRxTimeOut(COM_RPT))
//		{
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));				
//				p = (uint8_t *)strstr((char *)buf, "+QIURC: \"recv\",0,");	
//				if(p != NULL)
//				{
//					
//					p += 17; 
//					if((p[1]==',') || (p[2]==','))
//					{
//					  if(p[1]==',')
//					  {
//						len = (p[0]-0x30)*2;                            
//						p += 2;                     
//					  }
//					  else if(p[2]==',')
//					  {
//						len = ((p[0]-0x30)*10 + p[1]-0x30)*2;                            
//						p += 3;
//					  }					
//					
//						str_to_byte((char *)p, s_ucaBuf, len*2);
//						memset(buf, 0, sizeof(buf));
//						memcpy(buf, s_ucaBuf, len);
//						s_usSize = up_cfg(s_ucaBuf, buf, len);
//						if(s_usSize)
//						{					
//							s_ucATSta++;
//						}
//					}
//				}				
//			}			
//		}
//		
//		if(bsp_timer_check(TMR_RPT))
//		{
//			return 1; 
//		}
//	}
//	else if(s_ucATSta > 20) //重发次数限制 /10
//	{
//		return 0;
//	}
//	else if((s_ucATSta == 2) || (s_ucATSta == 12))//整理发送数据
//	{
//		if(op == 'L')
//		{
//			s_usSize = up_login(s_ucaBuf);
//		}			
//		else if(op == 'D')
//		{
//			s_usSize = up_data(s_ucaBuf);			
//		}
//		else if(op == 'T')
//		{
//			s_usSize = up_trig(s_ucaBuf);			
//		}
//		else if(op == 'H')
//		{
//			s_usSize = up_heart(s_ucaBuf);			
//		}
//		
//		s_ucATSta++;		
//	}	
//	else if((s_ucATSta == 3) || (s_ucATSta == 13))//发送数据
//	{	
//		memset(buf,0,sizeof(buf));		
//		sprintf((char *)buf, "AT+QISEND=0,%d,\"", s_usSize);
//		offset = strlen((char *)buf);		
//		for(i=0; i<s_usSize; i++)
//		{
//			sprintf((char *)buf+offset+i*2,"%02X", *(s_ucaBuf+i));					
//		}
//		strcat((char *)buf, "\"\r\n");		
//		comSendBuf(COM_DEBUG, buf, strlen((char *)buf));
//		comClearRxFifo(COM_RPT);         
//		comSendBuf(COM_RPT, buf, strlen((char *)buf));		
//		bsp_timer_star_once(TMR_RPT, TM_6S);

//		if(op == 'R')
//		{
//			s_ucATSta = 0; 	
//		}
//		else
//		{
//			s_ucATSta++;
//		}						
//	}	
//	else if((s_ucATSta == 4) || (s_ucATSta == 14)) //平台应答
//	{
//		if(comGetRxTimeOut(COM_RPT))
//		{
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{								
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));				
//				
//				//判断平台应答
//				p = (uint8_t *)strstr((char *)buf,"16\r\n");	
//				if(p != NULL)
//				{
//					bsp_timer_star_once(TMR_RPT, TM_10S); //开启接收计时							
//					
//					if(op == 'D')  
//					{
//						uint32_t addr;
//						//上报数据包成功,修改并保存上报索引						
//						addr = OFFSET_OF(VAR_T, uiSPIDX);
//						var_WriteBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//						var_ReadBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//						printf("\r\n");		
//						printf("TPIDX:%u\r\n", g_tVar.uiTPIDX);			
//						printf("SPIDX:%u\r\n", g_tVar.uiSPIDX);
//						printf("\r\n");	
//						
//						if(g_tVar.uiSPIDX == g_tVar.uiTPIDX) return 1;
//						else s_ucATSta = 2;												
//					}
//					else
//					{					
//						return 1;
//					}
//				
//				}								
//					
//			}			
//		}
//		
//		if(bsp_timer_check(TMR_RPT)) // 超时处理  
//		{
////			if((op == 'L') || (op == 'H'))
////			{
////				return 1;
////			}

//			if(op == 'D')  
//			{
//				uint32_t addr;
//				//上报数据包成功,修改并保存上报索引						
//				addr = OFFSET_OF(VAR_T, uiSPIDX);
//				var_WriteBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//				var_ReadBytes((uint8_t *)&g_tVar.uiSPIDX, addr, 4); 	
//				printf("\r\n");		
//				printf("TPIDX:%u\r\n", g_tVar.uiTPIDX);			
//				printf("SPIDX:%u\r\n", g_tVar.uiSPIDX);
//				printf("\r\n");	
//				
//				if(g_tVar.uiSPIDX == g_tVar.uiTPIDX) return 1;
//				else s_ucATSta = 2;												
//			}
//			else
//			{					
//				return 1;
//			}
//			
//			
//			if(s_ucATSta < 10)
//			{
//				s_ucATSta = 12;
//			}
//			else if(s_ucATSta < 20)
//			{
//				s_ucATSta = 22;					
//			}
//			else if(s_ucATSta < 30)
//			{
//				s_ucATSta = 32;					
//			}						
//		}		
//	}	
//	
//	return -1;			
//}

//int AT_BC260Y_CCLK(char op)
//{
//	uint8_t buf[128],*p;
//	
//	if(s_ucATSta == 0)
//	{
//		s_ucATSta++;		
//	}
//	else if(s_ucATSta > 6)    //重发次数限制 /2
//	{
//		return 1;
//	}	
//	else if(s_ucATSta&0x01)
//	{
//		AT_SendCmd("AT+CCLK?");			
//		bsp_timer_star_once(TMR_RPT, TM_1S);
//		s_ucATSta++;		
//	}
//	else 
//	{
//		if(bsp_timer_check(TMR_RPT)) //超时处理
//		{
//			s_ucATSta++;		
//		}		
//		else if(comGetRxTimeOut(COM_RPT))
//		{			
//			if(comGetBytes(COM_RPT, buf, sizeof(buf)))
//			{
//				comSendBuf(COM_DEBUG, (uint8_t *)"\r\n", 2);
//				comSendBuf(COM_DEBUG, buf, strlen((char *)buf));				

//                //+CCLK: "2022/05/31,06:39:52+32"
//				p = (uint8_t *)strstr((char *)buf,"+CCLK: ");						
//				if(p != NULL) 
//				{
//					struct tm tRTC;
//					int yy,mm,dd,hh,nn,ss;
//					
//					p += 7;
//					if(*p == '\"') p += 1;
//						
//					if(sscanf((char *)p, "%d/%d/%d,%d:%d:%d",&yy,&mm,&dd,&hh,&nn,&ss) == 6)
//					{
//						struct tm* ptm;
//						uint32_t time, now, t;
//						
//						tRTC.tm_year = yy-1900; 
//						tRTC.tm_mon = mm - 1;
//						tRTC.tm_mday = dd;
//						tRTC.tm_hour = hh;
//						tRTC.tm_min = nn;
//						tRTC.tm_sec = ss;
//						tRTC.tm_isdst = -1; 
//						
//                        printf("\r\n[MD RTC: %04d-%02d-%02d %02d:%02d:%02d]\r\n",
//                                tRTC.tm_year,tRTC.tm_mon,tRTC.tm_mday,tRTC.tm_hour,tRTC.tm_min,tRTC.tm_sec);

//						
//						time = mktime(&tRTC) + 28800;
//						
//						if(time > 1648569600) //2022-03-30 00:00:00
//						{
//							ptm = localtime((const time_t *)&time);
//							g_tRTC.Sec = ptm->tm_sec;
//							g_tRTC.Min = ptm->tm_min;
//							g_tRTC.Hour = ptm->tm_hour;
//							g_tRTC.Day = ptm->tm_mday;
//							g_tRTC.Mon = ptm->tm_mon+1;
//							g_tRTC.Year = ptm->tm_year-100;	

//							now = bsp_rtc_get_unix();
//							if(abs((int)(time-now)) >= 60)
//							{
//								int i;
//								
//								__set_PRIMASK(1);	
//								g_usErr |= 0x0040;
//							
//								for(i=0; i<3; i++)
//								{
//									bsp_rtc_set_time(g_tRTC.Year,g_tRTC.Mon,g_tRTC.Day,g_tRTC.Hour,g_tRTC.Min,g_tRTC.Sec);
//									t = bsp_rtc_get_unix();
//									if(abs((int)(time-t)) < 60)
//									{
//										break;
//									}
//								}
//								__set_PRIMASK(0);	

//								if(i == 3)
//								{
//									g_usErr |= 0x0010;
//									printf("\r\nbsp_rtc_set_time error\r\n");
//								}
//								else
//								{
//									g_ucRtcCali = 1;
//									g_tDat.time = time;
//									bsp_rtc_get_time();
//									printf("\r\n[RTC: %04d-%02d-%02d %02d:%02d:%02d]\r\n",
//											g_tRTC.Year,g_tRTC.Mon,g_tRTC.Day,g_tRTC.Hour,g_tRTC.Min,g_tRTC.Sec);										
//									if(now > time)
//									{
//										if((g_ucRptAuto == 1) && (g_ucRptTrig == 0))
//										{
//											g_usErr |= 0x0020;
//											g_ucRptAuto = 0;															
//											g_ucTimeFast = 1;
//											printf("\r\nThe clock is running fast\r\n");															
//										}
//									}
//								}
//							}
//							 
//							return 1;
//						}							
//						
//					}								  
//				}
//			}							
//		}
//	}
//		
//	return -1;	
//	
//}


