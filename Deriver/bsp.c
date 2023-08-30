#include "app.h"


void bsp_init(void)
{
			/********************************************************************/	
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

	/********************************************************************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	
	
	bsp_timer_init();
	bsp_power_init();
	bsp_led_init();
	/******************蜂鸣器***************/
	BEEP_GPIO_Config();	
	bsp_uart_fifo_init();
	bsp_i2c_init();
	bsp_InitSPIBus();	/* 配置SPI总线 */
		
	bsp_led_on(1);bsp_led_on(2);bsp_led_on(5);bsp_led_on(6);bsp_led_on(7);		
	bsp_timer_delay_ms(100);
	bsp_led_off(1);bsp_led_off(2);bsp_led_off(5);bsp_led_off(6);bsp_led_off(7);
	
	if(ee_CheckOk() == 0)
	{
		/* 没有检测到EEPROM */
		printf("没有检测到串行EEPROM!\r\n");

//		while (1);	/* 停机 */
	}
	bsp_InitSFlash();	/* 初始化串行Flash. 该函数会识别串行FLASH型号 */	
	
	/* 检测串行Flash */
	if(g_tSF.ChipID != MX25L12835E_ID)
	{
		printf("没有检测到串行Flash!\r\n");		
	}

	var_init();
	#ifndef __RS485__
//    if(g_tVar.ucRTUPLS) //采集脉冲
	{		
		pls_init();	
	}
	#endif
#if MPU6050_EN
	atk_ms6050_interruptEN();
#endif
	bsp_trigger_init(); //触发中断初始化	
	bsp_rtc_init(); //RTC初始化	
	bsp_rtc_alarm_init(); //RTC闹钟初始化	
	bsp_sleep_init(); //休眠参数初始化	
	bsp_cpu_adc_init(); //ADC初始化
	
//	printf("\r\n**********************\r\n");
//	bsp_rtc_get_time();
//	vbat = bsp_cpu_adc_get_battery();
//	printf("VBAT:%u\r\n", vbat);		
//	printf("TPIDX:%u\r\n", g_tVar.uiTPIDX);			
//	printf("SPIDX:%u\r\n", g_tVar.uiSPIDX);		

//	printf("\r\n**********************\r\n");
	#if IWDG_EN
	bsp_iwdg_init();
	#endif
	g_ucRtcCali = 1;
	bsp_timer_star_auto(TMR_RUN, TM_3S);	
}



void bsp_run1ms(void)
{

}

void bsp_run10ms(void)
{

}

void bsp_idle(void)
{		
	#if SLEEP_EN
									bsp_iwdg_clear();	
									if(bsp_timer_check(TMR_RUN))
									{
										bsp_led_on(1);
										g_usLed1Time = TM_5MS;		
									}								
										if((g_ucCfgMode == 0)&&(g_tVar.ucSLEEPEN)&&rpt_idle()&&pls_idle()&&cfg_idle()&&aqt_idle())
										{   
												if((g_ucRptAuto == 0)&&(g_ucRptTrig == 0)&&(g_ucAqtAuto == 0)&&(g_ucAqtTrig == 0)&&(g_ucCfgTrig == 0))
												{ 												
														bsp_rtc_get_time();//获取系统时钟
														bsp_rtc_set_alarm();			
														bsp_rtc_get_alarm(NULL);//获取闹钟时钟	
														bsp_rtc_alarm_clr();//开启闹钟唤醒			
														g_ucRtcCali = 0;				
														printf("\r\n");			
														printf("TPIDX:%u\r\n", g_tVar.uiTPIDX);			
														printf("SPIDX:%u\r\n", g_tVar.uiSPIDX);	
														printf("\r\n");	
														
														if(g_tVar.ucRTUPLS) //采集脉冲
														{
//															printf("\r\n");			
//															printf("P:%u\r\n", g_tVar.uiPlsCntP);			
//															printf("N:%u\r\n", g_tVar.uiPlsCntN);
//															printf("A:%d\r\n", Sensor.Alarm);				
//															printf("\r\n");
														}				
														
														printf("mcu go to sleep\r\n");
														
														comWaitTxFifo(COM1);			

														bsp_sleep(); //进入休眠且定时醒来喂狗
														
														printf("\r\nmcu wakeup by ");
														if(g_ucWkpTrig) printf("<wdg clr>"), g_usErr |= 0x0100; 
														if(g_ucRptAuto) printf("<rpt_auto>"),g_usErr |= 0x0200; 
														if(g_ucRptTrig) printf("<rpt_trig>"),g_usErr |= 0x0400; 
														if(g_ucAqtAuto) printf("<aqt_auto>"),g_usErr |= 0x0800; 
														if(g_ucAqtTrig) printf("<aqt_trig>"),g_usErr |= 0x1000; 
														if(g_ucCfgTrig) printf("<cfg_trig>"),g_usErr |= 0x2000;
														if(g_usPlsTime) printf("<pls_trig>"),g_usErr |= 0x4000;
													
														printf("\r\n");
														
														g_ucUpdata = 0;
																							
												}      
										}
									else if(g_ucRtcCali)		
									{
										bsp_rtc_get_time();//获取系统时钟
										bsp_rtc_set_alarm();			
										bsp_rtc_get_alarm(NULL);//获取闹钟时钟
										bsp_rtc_alarm_clr();//开启闹钟唤醒	
										g_ucRtcCali = 0;		
									}
	#else 
	
                 if(g_ucRtcCali)										
									{
										bsp_rtc_get_time();//获取系统时钟
										bsp_rtc_set_alarm();			
										bsp_rtc_get_alarm(NULL);//获取闹钟时钟
										bsp_rtc_alarm_clr();//开启闹钟唤醒	
										g_ucRtcCali = 0;		
									}
	#endif
	
}


