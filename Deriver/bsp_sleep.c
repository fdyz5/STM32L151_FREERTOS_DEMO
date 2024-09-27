#include "app.h"

static void bsp_sleep_handle(void);
static void bsp_sleep_quit_sleep(void);
static void bsp_sleep_io_close(void);

uint8_t g_ucWkpTrig = 0;

void bsp_sleep_init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RTC_WakeUpCmd(DISABLE);
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits); 
	RTC_SetWakeUpCounter(12);         //间隔12s
	RTC_ClearFlag(RTC_FLAG_WUTF);
    RTC_ClearITPendingBit(RTC_IT_WUT);
    RTC_ITConfig(RTC_IT_WUT,DISABLE);    //需要使能中断，不需要中断函数
	
	EXTI_ClearITPendingBit(EXTI_Line20);//配置唤醒中断
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line20;
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel					 = RTC_WKUP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority    	 = 10;
	NVIC_InitStructure.NVIC_IRQChannelCmd            	 = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

uint8_t bsp_sleep(void)
{
	uint8_t wkpType;
	
	//io关闭配置
	bsp_sleep_io_close();
	
	DBGMCU_Config(DBGMCU_STOP, DISABLE);
	
	//清除看门狗
	bsp_iwdg_clear();
	bsp_sleep_handle();

	while(1)
	{
		if(g_ucRptAuto||g_ucRptTrig||g_ucAqtAuto||g_ucAqtTrig||g_ucCfgTrig||g_usPlsTime)
		{
			break;
		}		
		//进入休眠
		PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
		bsp_iwdg_clear();		
	}
	
	bsp_sleep_quit_sleep();
	
	return wkpType;
}

//唤醒中断
void RTC_WKUP_IRQHandler(void)
{
    if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
    {
		g_ucWkpTrig = 1;
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line20);
    }
}

static void bsp_sleep_handle(void)
{
	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	RCC_HSICmd(DISABLE);

	/* Allow access to RTC */
	PWR_RTCAccessCmd(ENABLE);

//	RCC_LSEConfig(RCC_LSE_ON);
//	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
//	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
//	RCC_RTCCLKCmd(ENABLE);
//	/* Wait for RTC APB registers synchronisation */
//	RTC_WaitForSynchro();
	
	RTC_ClearFlag(RTC_FLAG_WUTF);
    RTC_ClearITPendingBit(RTC_IT_WUT);
	EXTI_ClearITPendingBit(EXTI_Line20);
	
	RTC_ITConfig(RTC_IT_WUT,ENABLE);    //需要使能中断，不需要中断函数
	RTC_WakeUpCmd(ENABLE);
	
	CLEAR_BIT(SysTick->CTRL,SysTick_CTRL_TICKINT_Msk);
	PWR_UltraLowPowerCmd(ENABLE);
	PWR_FastWakeUpCmd(ENABLE);
}

static void bsp_sleep_quit_sleep(void)
{	
	#if HSI__EN
					RTC_ITConfig(RTC_IT_WUT,DISABLE);    //需要使能中断，不需要中断函数
					RTC_WakeUpCmd(DISABLE);
					
					PWR_UltraLowPowerCmd(DISABLE);
					PWR_FastWakeUpCmd(DISABLE);
					
					DBGMCU_Config(DBGMCU_STOP, ENABLE);
					
					//重新配置系统时钟
					RCC_HSICmd(ENABLE);
					/* Wait till HSE is ready */
						while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
						{}
						
						/* Enable PLL */
						RCC_PLLCmd(ENABLE);
						
						/* Wait till PLL is ready */
						while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
						{}
						
						/* Select PLL as system clock source */
						RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
						
						/* Wait till PLL is used as system clock source */
						while (RCC_GetSYSCLKSource() != 0x0C)
						{}
					
					SET_BIT(SysTick->CTRL,SysTick_CTRL_TICKINT_Msk);				
					bsp_power_init();
					bsp_led_init();
					/******************蜂鸣器***************/
	        BEEP_GPIO_Config();	
					bsp_uart_fifo_init();
					bsp_InitSPIBus();
					bsp_cpu_adc_init();
					bsp_i2c_init();
					sf_ConfigGPIO();							
					bsp_timer_star_auto(TMR_RUN, TM_1S);		
	#else

					RTC_ITConfig(RTC_IT_WUT,DISABLE);    //需要使能中断，不需要中断函数
					RTC_WakeUpCmd(DISABLE);
//					
					PWR_UltraLowPowerCmd(DISABLE);
					PWR_FastWakeUpCmd(DISABLE);
					
					DBGMCU_Config(DBGMCU_STOP, ENABLE);
					
					//重新配置系统时钟————使用外部高速晶振
				  	RCC_HSEConfig(RCC_HSE_ON);
					/* Wait till HSE is ready */
						while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
						{}
						
						/* Enable PLL */
						RCC_PLLCmd(ENABLE);
						
						/* Wait till PLL is ready */
						while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
						{}
						
						/* Select PLL as system clock source */
						RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
						
						/* Wait till PLL is used as system clock source */
						while (RCC_GetSYSCLKSource() != 0x0C)
						{}


	/***************************************************/				
					SET_BIT(SysTick->CTRL,SysTick_CTRL_TICKINT_Msk);				
					bsp_power_init();
					bsp_led_init();
					/******************蜂鸣器***************/
	        BEEP_GPIO_Config();	
					bsp_uart_fifo_init();
					bsp_InitSPIBus();
					bsp_cpu_adc_init();
					bsp_i2c_init();
//					TIM3_Int_Init((1000-1),(36-1));//设置定时器 1ms中断一次
					sf_ConfigGPIO();							
					bsp_timer_star_auto(TMR_RUN, TM_1S);	
//					vGetSysclockDebug();
	/***************************************************/	

	#endif
	
}

void bsp_sleep_io_close(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, DISABLE);
	SPI_Cmd(SPI2, DISABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);
	USART_Cmd(USART1, DISABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);
	USART_Cmd(USART2, DISABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, DISABLE);
	USART_Cmd(USART3, DISABLE);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);
	ADC_Cmd(ADC1, DISABLE);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_All;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
//	GPIO_Init(GPIOC,&GPIO_InitStruct);
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	GPIO_Init(GPIOH,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
//    if(g_tVar.ucRTUPLS) //采集脉冲
	{	
		GPIO_InitStruct.GPIO_Pin   = 0xFF78;
	}
//	else
//	{
//		GPIO_InitStruct.GPIO_Pin   = 0xFF7F;		
//	}
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
#if MPU6050_EN
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = 0xFFF7;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
#else
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_All;
  GPIO_Init(GPIOC,&GPIO_InitStruct);
#endif
	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	//蓝牙开关
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIOB->BSRRL = GPIO_Pin_8|GPIO_Pin_9;	
	

}
