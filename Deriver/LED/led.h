/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： led.h
* 文件标识：
* 内容摘要： led相关函数声明
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
#ifndef __LED_H
#define __LED_H
#include "MainConfig.h" 

#define  LED_AHBPeriph_PORT		RCC_AHBPeriph_GPIOA
#define  LED_GPIO_PORT			GPIOA
#define  LED_GPIO_PIN			GPIO_Pin_8

#define  LED2_AHBPeriph_PORT		RCC_AHBPeriph_GPIOC
#define  LED2_GPIO_PORT			GPIOC
#define  LED2_GPIO_PIN			GPIO_Pin_9

#define  LED3_AHBPeriph_PORT		RCC_AHBPeriph_GPIOC
#define  LED3_GPIO_PORT			GPIOC
#define  LED3_GPIO_PIN			GPIO_Pin_8


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态


/* 定义控制IO的宏 */
#define LED1_TOGGLE		 digitalToggle(LED_GPIO_PORT,LED_GPIO_PIN)
#define LED1_OFF		   digitalHi(LED_GPIO_PORT,LED_GPIO_PIN)
#define LED1_ON			   digitalLo(LED_GPIO_PORT,LED_GPIO_PIN)

#define LED2_TOGGLE		 digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF		   digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_ON			   digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3_TOGGLE		 digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_OFF		   digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_ON			   digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)


#define LED_RED PAout(0)// PA0	

void bsp_InitLED(void);//初始化
void LED_Control(uint8_t valed);
void Reversal_LED(void);

#endif  /* _LED_H */

























