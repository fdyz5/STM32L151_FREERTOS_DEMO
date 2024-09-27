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

#include "usart.h"
#include "bsp.h"

TaskHandle_t Runing_State_Handler;     //任务句柄
TaskHandle_t LED2_Task_Handler;     //任务句柄


__IO uint32_t LsiFreq = 0;

__IO uint32_t CaptureNumber = 0, PeriodValue = 0;
uint32_t IC1ReadValue1 = 0, IC1ReadValue2 =0;

/**
  * @brief  Configures TIM10 to measure the LSI oscillator frequency. 
  * @param  None
  * @retval LSI Frequency
  */
uint32_t GetLSIFrequency(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  RCC_ClocksTypeDef  RCC_ClockFreq;

  /* Enable the LSI oscillator ************************************************/
  RCC_LSICmd(ENABLE);
  
  /* Wait till LSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}

  /* TIM10 configuration *******************************************************/ 
  /* Enable TIM10 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
  
  /* Reset TIM10 registers */
  TIM_DeInit(TIM10);

  /* Configure TIM10 prescaler */
  TIM_PrescalerConfig(TIM10, 0, TIM_PSCReloadMode_Immediate);

  /* Connect LSI clock to TIM10 Input Capture 1 */
  TIM_RemapConfig(TIM10, TIM10_LSI);

  /* TIM10 configuration: Input Capture mode ---------------------
     The reference clock(LSE or external) is connected to TIM10 CH1
     The Rising edge is used as active edge,
     The TIM10 CCR1 is used to compute the frequency value 
  ------------------------------------------------------------ */
  TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM10, &TIM_ICInitStructure);

  /* Enable the TIM10 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable TIM10 counter */
  TIM_Cmd(TIM10, ENABLE);

  /* Reset the flags */
  TIM10->SR = 0;
    
  /* Enable the CC4 Interrupt Request */  
  TIM_ITConfig(TIM10, TIM_IT_CC1, ENABLE);


  /* Wait until the TIM10 get 2 LSI edges (refer to TIM10_IRQHandler() in 
    stm32l1xx_it.c file) ******************************************************/
  while(CaptureNumber != 2)
  {
  }
  /* Deinitialize the TIM10 peripheral registers to their default reset values */
  TIM_DeInit(TIM10);


  /* Compute the LSI frequency, depending on TIM10 input clock frequency (PCLK1)*/
  /* Get SYSCLK, HCLK and PCLKx frequency */
  RCC_GetClocksFreq(&RCC_ClockFreq);

  /* Get PCLK1 prescaler */
  if ((RCC->CFGR & RCC_CFGR_PPRE1) == 0)
  { 
    /* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
    return ((RCC_ClockFreq.PCLK1_Frequency / PeriodValue) * 8);
  }
  else
  { /* PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1 */
    return (((2 * RCC_ClockFreq.PCLK1_Frequency) / PeriodValue) * 8) ;
  }
}

/**
  * @brief  This function handles TIM10 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM10_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM10, TIM_IT_CC1) != RESET)
  {
    /* Clear TIM10 Capture Compare 1 interrupt pending bit */
    TIM_ClearITPendingBit(TIM10, TIM_IT_CC1);
    
    if(CaptureNumber == 0)
    {
      /* Get the Input Capture value */
      IC1ReadValue1 = TIM_GetCapture1(TIM10);
      CaptureNumber = 1;
    }
    else if(CaptureNumber == 1)
    {
       /* Get the Input Capture value */
       IC1ReadValue2 = TIM_GetCapture1(TIM10); 
       TIM_ITConfig(TIM10, TIM_IT_CC1, DISABLE);

       /* Capture computation */
       if (IC1ReadValue2 > IC1ReadValue1)
       {
         PeriodValue = (IC1ReadValue2 - IC1ReadValue1);
       }
       else
       {
         PeriodValue = ((0xFFFF - IC1ReadValue1) + IC1ReadValue2);
       }
       /* capture of two values is done */
       CaptureNumber = 2;
    }
  }
}


/**************************************************************************
* 函数名称: Runing_State
* 功能描述: 系统正常运行指示灯任务 一秒亮一次
* 输入参数: pvParameters 是在创建该任务时传递的形参
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void  Runing_State(void *pvParameters)
{
//	__IO uint32_t LsiFreq = 0;
	RCC_ClocksTypeDef stRCC_Clocks;
	uint8_t ucSYSCLKSource;
    while(1) 
	{
		bsp_led_toggle(SHIFT_ALARM_LED);   
		vTaskDelay(pdMS_TO_TICKS(500));//延时1s
		bsp_led_toggle(VOLTAGE_LOSS_ALARM_LED);  
		vTaskDelay(pdMS_TO_TICKS(500));//延时1s
	#if GETSYSCLK
		RCC_GetClocksFreq(&stRCC_Clocks);
		printf("\r\nSYSCLK_Frequency: %d\r\n",stRCC_Clocks.SYSCLK_Frequency);	
		printf("\r\n HCLK_Frequency: %d\r\n",stRCC_Clocks.HCLK_Frequency);
		printf("\r\n PCLK1_Frequency: %d\r\n",stRCC_Clocks.PCLK1_Frequency);
		printf("\r\n PCLK2_Frequency: %d\r\n",stRCC_Clocks.PCLK2_Frequency);		
		ucSYSCLKSource = RCC_GetSYSCLKSource();
		printf("\r\n ucSYSCLKSource: %d\r\n",ucSYSCLKSource);
	#endif
	       // printf("\r\nLsiFreq: %u\r\n",LsiFreq);	
//#if DEBUG_LOG_PRINTF  
//		/* 检测任务堆栈使用大小的一种方式，通过设置FreeRTOS的宏INCLUDE_uxTaskGetStackHighWaterMark开启
//		 * uxTaskGetStackHighWaterMark()主要用来查询指定任务的运行历史中， 其栈空间还差多少就要溢出。
//		 * 这个值被称为栈空间的“高水线(High Water Mark)”。需要先设置大堆栈，然后合理化设置实际堆栈。*/
//		LOG_D("Reference task stack size = %d", 
//			((int )Runing_State_STK_SIZE-(int)uxTaskGetStackHighWaterMark(NULL))*2);
//#endif
	}
}

// 定义一个计数信号量句柄
SemaphoreHandle_t xCountingSemaphore;
char *strx;
/**************************************************************************/
void  LED2_Task(void *pvParameters)
{ 
	char *p = "Read these notes carefully and keep them for future reference.\r\n";
	uint16_t  usRunningCount = 0;
	    // 创建计数信号量，初始值为0，最大值为5
   // xCountingSemaphore = xSemaphoreCreateCounting(5, 0);
    while(1) 
	{
       Uart1_SendStr(p);
	   vTaskDelay(pdMS_TO_TICKS(2000));
//		    Uart1_SendStr("AT\r\n");
        Uart2_SendStr("AT\r\n");
        vTaskDelay(pdMS_TO_TICKS(3000));
		Uart1_SendStr(buf_uart2.buf);
		Clear_Buffer();
	}
}
/**************************************************************************/




/**************************************************************************
* 函数名称: LEDTaskSuspend
* 功能描述: 挂起运行指示灯任务
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void LEDTaskSuspend(void)
{
    //LED_Control(0);		//挂起LED任务的时候需要设置一下LED灯的状态
    vTaskSuspend(Runing_State_Handler);  //挂起运行指示灯任务
}

/**************************************************************************
* 函数名称: LEDTaskResume
* 功能描述: 挂起运行指示灯任务
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void LEDTaskResume(void)
{
    //LED_Control(0);		//恢复LED任务的时候需要设置一下LED灯的状态
    vTaskResume(Runing_State_Handler); //恢复运行指示灯任务
}

/**************************************************************************
* 函数名称: Creat_LED_Task
* 功能描述: 创建任务函数
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void Creat_LED_Task(void)
{
    //动态方式创建运行状态灯任务
    xTaskCreate((TaskFunction_t	)Runing_State,		      //任务函数
                (const char* 	)"Runing_State",	      //任务名称
                (uint16_t 		)Runing_State_STK_SIZE,	  //任务栈大小
                (void* 		  	)NULL,				      //传递给任务函数的参数
                (UBaseType_t 	)Runing_State_PRIO, 	  //任务优先级
                (TaskHandle_t*  )&Runing_State_Handler);  //任务句柄
}


void Creat_LED2_Task(void)
{
    //动态方式创建运行状态灯任务
    xTaskCreate((TaskFunction_t	)LED2_Task,		      //任务函数
                (const char* 	)"LED2_Task",	      //任务名称
                (uint16_t 		)LED2_Task_STK_SIZE,	  //任务栈大小
                (void* 		  	)NULL,				      //传递给任务函数的参数
                (UBaseType_t 	)LED2_Task_PRIO, 	  //任务优先级
                (TaskHandle_t*  )&LED2_Task_Handler);  //任务句柄
}








