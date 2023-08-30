/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： usart.h
* 文件标识：
* 内容摘要： usart相关函数声明
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
#ifndef __USART_H
#define __USART_H
#include "stm32l1xx.h"
#include "stdio.h"
#include "stdint.h"

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

#define USART_REC_LEN  			200  	//定义最大接受字节数200
#define EN_USART1_RX 			1		//1：使能 0：禁止
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲，最大USART_REC_LEN个字节，未字节为换行符
extern u16 USART_RX_STA;         		//接收标记符号	
void uart_init(u32 bound);


#define BUFLEN 256      //Êý×é»º´æ´óÐ¡
typedef struct _UART_BUF
{
    char buf [BUFLEN+1];               
    unsigned int index ;
}UART_BUF;
void uart_init(uint32_t BaudRate);
void uart1_init(u32 bound);     //´®¿Ú³õÊ¼»¯
void uart2_init(u32 bound);
void uart3_init(u32 bound);
void Uart1_SendStr(char*SendBuf);   //×Ö·û´®·¢ËÍ
void Uart2_SendStr(char*SendBuf);
void Uart3_SendStr(char*SendBuf);
void Clear_Buffer(void);
extern UART_BUF buf_uart1;     //CH340
extern UART_BUF buf_uart2;     //NBIOT
extern UART_BUF buf_uart3;     //TTL


#endif   /*  __USART_H  */
 

























