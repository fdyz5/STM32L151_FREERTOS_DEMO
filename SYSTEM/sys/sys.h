#ifndef __SYS_H
#define __SYS_H	
#include "stm32l1xx.h"
#include "stdbool.h"
#include "stdio.h"
#include "bsp.h"

//0,不支持os
//1,支持os
#define SYSTEM_SUPPORT_OS		1		//定义系统文件夹是否支持OS
																	    
	 

//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址

#endif
