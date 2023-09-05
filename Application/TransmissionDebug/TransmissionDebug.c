/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： ledtask.c
* 文件标识：
* 内容摘要： ledtask相关函数定义
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
#include "app.h"
TaskHandle_t TRANSMISS_Task_Handler;    //任务句柄



void  vTransmissionTask(void *pvParameters)
{
	while(AT_START() == -1);

  while(1) 
	{	
	#if Trans_DEBUG
		if(buf_uart1.index != 0)
		{
		   Uart2_SendStr(buf_uart1.buf);
		}
		if(buf_uart2.index != 0)
		{
		   Uart1_SendStr(buf_uart2.buf);
		}		
		Clear_Buffer();
	  vTaskDelay(pdMS_TO_TICKS(50));

	#else
		vTaskDelay(pdMS_TO_TICKS(50));
  #endif
	}
}






void Creat_Transmission_Task(void)
{
    //创建任务  透传调试
    xTaskCreate((TaskFunction_t	)vTransmissionTask,		    //任务函数
                (const char* 	)"vTransmissionTask",	      //任务名称
                (uint16_t 		)TRANSMISS_Task_STK_SIZE,	  //任务栈大小
                (void* 		  	)NULL,				              //传递给任务函数的参数
                (UBaseType_t 	)TRANSMISS_Task_PRIO, 	    //任务优先级
                (TaskHandle_t*  )&TRANSMISS_Task_Handler);  //任务句柄
}

