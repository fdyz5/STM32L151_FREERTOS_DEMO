/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： usart.c
* 文件标识：
* 内容摘要： usart相关函数定义
* 其它说明：
* 当前版本：
* 作    者： woody
* 完成日期： 2020.09.07
*
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
* 修改记录2：…
************************************************************************/
#include "usart.h"
#include "MainConfig.h"


volatile char receivedData;

#if DEBUG_LOG_PRINTF
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE {
    int handle;

};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while((USART1->SR & 0X40) == 0); //循环发送,直到发送完毕
    USART1->DR = (uint8_t) ch;
    return ch;
}
#endif

/**************************************************************************
* 函数名称: USART1_Init
* 功能描述: USART1_Init 初始化函数，(串口1功能与USB功能冲突)
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/

#if EN_USART1_RX   //Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
//×¢Òâ,¶ÁÈ¡USARTx->SRÄÜ±ÜÃâÄªÃûÆäÃîµÄ´íÎó   	
u8 USART_RX_BUF[USART_REC_LEN];     //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.
//½ÓÊÕ×´Ì¬
//bit15£¬	½ÓÊÕÍê³É±êÖ¾
//bit14£¬	½ÓÊÕµ½0x0d
//bit13~0£¬	½ÓÊÕµ½µÄÓÐÐ§×Ö½ÚÊýÄ¿
u16 USART_RX_STA=0;       //½ÓÊÕ×´Ì¬±ê¼Ç	  

void uart1_init(uint32_t BaudRate)
{
	  #if 0
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);			//使能USART1
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);				//使能GPIOA时钟

    /* Connect PA9 to USART1_Tx */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); 		//PA.9 复用
    /* Connect PA10 to USART1_Rx */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); 		//PA10 复用

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = BaudRate; //串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	  //收发模式

    USART_Init(USART1, &USART_InitStructure);//初始化串口1
    USART_Cmd(USART1, ENABLE);//使能串口1
		#else
		    //GPIO¶Ë¿ÚÉèÖÃ
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA,ENABLE);//GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//Ê¹ÄÜUSART1
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
    /*
	*  USART1_TX -> PA9 , USART1_RX -> PA10
	*/				
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //ÍÆÍì¸´ÓÃÊä³ö
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //ÉÏÀ­
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //Usart1 NVIC ÅäÖÃ
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//ÇÀÕ¼ÓÅÏÈ¼¶3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//×ÓÓÅÏÈ¼¶3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
    NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷

    //USART ³õÊ¼»¯ÉèÖÃ

    USART_InitStructure.USART_BaudRate = BaudRate;//Ò»°ãÉèÖÃÎª9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
    USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½

    USART_Init(USART1, &USART_InitStructure); //³õÊ¼»¯´®¿Ú
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//¿ªÆôÖÐ¶Ï
    USART_Cmd(USART1, ENABLE);                    //Ê¹ÄÜ´®¿Ú 

		#endif
}
#endif
//³õÊ¼»¯IO ´®¿Ú3 
//bound:²¨ÌØÂÊ
void uart2_init(u32 bound)
{

    //GPIO¶Ë¿ÚÉèÖÃ
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

    //USART2_TX -> PA2 , USART2_RX -> PA3

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //ÍÆÍì¸´ÓÃÊä³ö
    //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //ÉÏÀ­
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //Usart3 NVIC ÅäÖÃ

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//ÇÀÕ¼ÓÅÏÈ¼¶1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//×ÓÓÅÏÈ¼¶1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
    NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷

    //USART ³õÊ¼»¯ÉèÖÃ

    USART_InitStructure.USART_BaudRate = bound;//Ò»°ãÉèÖÃÎª9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
    USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½

    USART_Init(USART2, &USART_InitStructure); //³õÊ¼»¯´®¿Ú
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//¿ªÆôÖÐ¶Ï
    USART_Cmd(USART2, ENABLE);                    //Ê¹ÄÜ´®¿Ú

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

}


//³õÊ¼»¯IO ´®¿Ú3 
//bound:²¨ÌØÂÊ
void uart3_init(u32 bound)
{
    //GPIO¶Ë¿ÚÉèÖÃ
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
    /*
	*  USART3_TX -> PB10 , USART3_RX -> PB11
	*/				
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //ÍÆÍì¸´ÓÃÊä³ö
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //ÉÏÀ­
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //Usart3 NVIC ÅäÖÃ

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//ÇÀÕ¼ÓÅÏÈ¼¶1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//×ÓÓÅÏÈ¼¶1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
    NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷

    //USART ³õÊ¼»¯ÉèÖÃ

    USART_InitStructure.USART_BaudRate = bound;//Ò»°ãÉèÖÃÎª9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
    USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½

    USART_Init(USART3, &USART_InitStructure); //³õÊ¼»¯´®¿Ú
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//¿ªÆôÖÐ¶Ï
    USART_Cmd(USART3, ENABLE);                    //Ê¹ÄÜ´®¿Ú

}


void Uart1_SendStr(char*SendBuf)//´®¿Ú1´òÓ¡Êý¾Ý´úÂë
{
    while(*SendBuf)
    {
        while((USART1->SR&0X40)==0)
        {
        }//µÈ´ý·¢ËÍÍê³É
        USART1->DR = (u8) *SendBuf; 
        SendBuf++;
    }
}


void Uart2_SendStr(char*SendBuf)//´®¿Ú1´òÓ¡Êý¾Ý´úÂë
{
    while(*SendBuf)
    {
        while((USART2->SR&0X40)==0)
        {
        }//µÈ´ý·¢ËÍÍê³É
        USART2->DR = (u8) *SendBuf; 
        SendBuf++;
    }
}

void Uart3_SendStr(char*SendBuf)//´®¿Ú1´òÓ¡Êý¾Ý´úÂë
{
    while(*SendBuf)
    {
        while((USART3->SR&0X40)==0)
        {
        }//µÈ´ý·¢ËÍÍê³É
        USART3->DR = (u8) *SendBuf; 
        SendBuf++;
    }
}


UART_BUF buf_uart1;     //CH340
UART_BUF buf_uart2;     //NBIOT
UART_BUF buf_uart3;     //TTL


void USART1_IRQHandler(void)                	//串口1中断服务函数
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
    {
        //buf_uart1.buf[buf_uart1.index++] =USART_ReceiveData(USART1);//接收模块的数据
		if(buf_uart1.index >= BUFLEN)
		{
			buf_uart1.index = 0 ;
		}
		else
		{
			buf_uart1.buf[buf_uart1.index++] = USART_ReceiveData(USART1);
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
   if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
    {
        if(buf_uart1.index >= BUFLEN)
		{
			buf_uart1.index = 0 ;
		}
		else
		{
			buf_uart1.buf[buf_uart1.index++] = USART_ReceiveData(USART1);
		}
        USART_ClearFlag(USART1,USART_FLAG_ORE);
    }
} 
//void USART1_IRQHandler(void)
//{
//    if (USART1->SR & USART_SR_RXNE)
//    {
//        receivedData = USART1->DR; // 读取接收缓冲区的数据

//        // 发送接收到的数据
//        while (!(USART1->SR & USART_SR_TXE))
//        {
//            // 等待发送缓冲区为空
//        }
//        USART1->DR = receivedData;

//        // 清除接收中断标志位
//        USART1->SR &= ~USART_SR_RXNE;
//    }
//}

void nbiot_receive_process_event(unsigned char ch )     //´®¿Ú2¸ønbiotÓÃ
{
    if(buf_uart2.index >= BUFLEN)
    {
        buf_uart2.index = 0 ;
    }
    else
    {
        buf_uart2.buf[buf_uart2.index++] = ch;
    }
}

void USART2_IRQHandler(void)                            
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE)==SET)
    {
        nbiot_receive_process_event(USART_ReceiveData(USART2));
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    }

    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
    {
        nbiot_receive_process_event(USART_ReceiveData(USART2));
        USART_ClearFlag(USART2,USART_FLAG_ORE);
    }
}
#endif	


void Clear_Buffer(void)
{
	  buf_uart1.index=0;
    memset(buf_uart1.buf,0,BUFLEN);
    buf_uart2.index=0;
    memset(buf_uart2.buf,0,BUFLEN);
}











