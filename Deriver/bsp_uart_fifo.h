/*
*********************************************************************************************************
*
*	模块名称 : 串口中断+FIFO驱动模块
*	文件名称 : bsp_uart_fifo.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_
#include "stm32l1xx.h"
#include <stdint.h>
/*
	如果需要更改串口对应的管脚，请自行修改 bsp_uart_fifo.c文件中的 static void InitHardUart(void)函数
*/

/* 定义使能的串口, 0 表示不使能（不增加代码大小）， 1表示使能 */
/* 安富莱STM32-X3开发板串口分配：
	PA9/USART1_TX	  --- 打印调试口
	PA10/USART1_RX
*/
#define	RS485_EN		1

#define	UART1_FIFO_EN	1
#define	UART2_FIFO_EN	1
#define	UART3_FIFO_EN	1
#define	UART4_FIFO_EN	0
#define	UART5_FIFO_EN	0


/* 定义端口号 */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 或  PB6, PB7*/
	COM2 = 1,	/* USART2, PD5,PD6 或 PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12, PD2 */
}COM_PORT_E;

/* 定义串口波特率和FIFO缓冲区大小，分为发送缓冲区和接收缓冲区, 支持全双工 */
#if UART1_FIFO_EN == 1
	#define UART1_BAUD			9600
	#define UART1_TX_BUF_SIZE	1*3072
	#define UART1_RX_BUF_SIZE	1*2048
#endif

#if UART2_FIFO_EN == 1
#ifdef __SL6200N__
	#ifdef __SLM130__
	#define UART2_BAUD			9600
	#else
	#define UART2_BAUD			115200
	#endif
#else
	#define UART2_BAUD			115200
#endif
	#define UART2_TX_BUF_SIZE	1*2048
	#define UART2_RX_BUF_SIZE	1*2048
#endif

#if UART3_FIFO_EN == 1
	#define UART3_BAUD			9600
	#define UART3_TX_BUF_SIZE	1*512
	#define UART3_RX_BUF_SIZE	1*512
#endif

#if UART4_FIFO_EN == 1
	#define UART4_BAUD			115200
	#define UART4_TX_BUF_SIZE	1*1024
	#define UART4_RX_BUF_SIZE	1*1024
#endif

#if UART5_FIFO_EN == 1
	#define UART5_BAUD			115200
	#define UART5_TX_BUF_SIZE	1*1024
	#define UART5_RX_BUF_SIZE	1*1024
#endif

/* 串口设备结构体 */
typedef struct
{
	USART_TypeDef *uart;		/* STM32内部串口设备指针 */
	uint8_t *pTxBuf;			/* 发送缓冲区 */
	uint8_t *pRxBuf;			/* 接收缓冲区 */
	uint16_t usTxBufSize;		/* 发送缓冲区大小 */
	uint16_t usRxBufSize;		/* 接收缓冲区大小 */
	uint16_t usTxWrite;			/* 发送缓冲区写指针 */
	uint16_t usTxRead;			/* 发送缓冲区读指针 */
	uint16_t usTxCount;			/* 等待发送的数据个数 */

	uint16_t usRxWrite;			/* 接收缓冲区写指针 */
	uint16_t usRxRead;			/* 接收缓冲区读指针 */
	uint16_t usRxCount;			/* 还未读取的新数据个数 */

	void (*SendBefor)(void); 	/* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
	void (*SendOver)(void); 	/* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
	void (*ReciveNew)(uint8_t _byte);	/* 串口收到数据的回调函数指针 */
}UART_T;

void bsp_uart_fifo_init(void);
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte);
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte);
uint16_t comGetBytes(COM_PORT_E _ucPort, uint8_t *_pBuf, uint16_t _usSize);
uint16_t comTxPrintfHex(COM_PORT_E _ucPort, uint8_t *_pBuf, uint16_t _usSize);
uint16_t comRxPrintfHex(COM_PORT_E _ucPort, uint8_t *_pBuf, uint16_t _usSize);
uint16_t comGetRxCount(COM_PORT_E _ucPort);
uint8_t comGetRxTimeOut(COM_PORT_E _ucPort);

void comClearTxFifo(COM_PORT_E _ucPort);
void comClearRxFifo(COM_PORT_E _ucPort);

int16_t comWaitTxFifo(COM_PORT_E _ucPort);

#if RS485_EN == 1
	void RS485_SendBuf(uint8_t *_ucaBuf, uint16_t _usLen);
#endif

#if RS485_EN == 1
	void RS485_SendStr(char *_pBuf);
#endif

#if RS485_EN == 1
	void bsp_Set485Baud(uint32_t _baud);
#endif

void InitHardUart3(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
