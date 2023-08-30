#include "app.h"


void bsp_init(void)
{
			/********************************************************************/	
  //��������ϵͳʱ�ӡ�������ʹ���ⲿ���پ���
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
	/******************������***************/
	BEEP_GPIO_Config();	
	bsp_uart_fifo_init();
	bsp_i2c_init();
	bsp_InitSPIBus();	/* ����SPI���� */
		
	bsp_led_on(1);bsp_led_on(2);bsp_led_on(5);bsp_led_on(6);bsp_led_on(7);		
	bsp_timer_delay_ms(100);
	bsp_led_off(1);bsp_led_off(2);bsp_led_off(5);bsp_led_off(6);bsp_led_off(7);
	
	if(ee_CheckOk() == 0)
	{
		/* û�м�⵽EEPROM */
		printf("û�м�⵽����EEPROM!\r\n");

//		while (1);	/* ͣ�� */
	}
	bsp_InitSFlash();	/* ��ʼ������Flash. �ú�����ʶ����FLASH�ͺ� */	
	
	/* ��⴮��Flash */
	if(g_tSF.ChipID != MX25L12835E_ID)
	{
		printf("û�м�⵽����Flash!\r\n");		
	}

	var_init();
	#ifndef __RS485__
//    if(g_tVar.ucRTUPLS) //�ɼ�����
	{		
		pls_init();	
	}
	#endif
#if MPU6050_EN
	atk_ms6050_interruptEN();
#endif
	bsp_trigger_init(); //�����жϳ�ʼ��	
	bsp_rtc_init(); //RTC��ʼ��	
	bsp_rtc_alarm_init(); //RTC���ӳ�ʼ��	
	bsp_sleep_init(); //���߲�����ʼ��	
	bsp_cpu_adc_init(); //ADC��ʼ��
	
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
														bsp_rtc_get_time();//��ȡϵͳʱ��
														bsp_rtc_set_alarm();			
														bsp_rtc_get_alarm(NULL);//��ȡ����ʱ��	
														bsp_rtc_alarm_clr();//�������ӻ���			
														g_ucRtcCali = 0;				
														printf("\r\n");			
														printf("TPIDX:%u\r\n", g_tVar.uiTPIDX);			
														printf("SPIDX:%u\r\n", g_tVar.uiSPIDX);	
														printf("\r\n");	
														
														if(g_tVar.ucRTUPLS) //�ɼ�����
														{
//															printf("\r\n");			
//															printf("P:%u\r\n", g_tVar.uiPlsCntP);			
//															printf("N:%u\r\n", g_tVar.uiPlsCntN);
//															printf("A:%d\r\n", Sensor.Alarm);				
//															printf("\r\n");
														}				
														
														printf("mcu go to sleep\r\n");
														
														comWaitTxFifo(COM1);			

														bsp_sleep(); //���������Ҷ�ʱ����ι��
														
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
										bsp_rtc_get_time();//��ȡϵͳʱ��
										bsp_rtc_set_alarm();			
										bsp_rtc_get_alarm(NULL);//��ȡ����ʱ��
										bsp_rtc_alarm_clr();//�������ӻ���	
										g_ucRtcCali = 0;		
									}
	#else 
	
                 if(g_ucRtcCali)										
									{
										bsp_rtc_get_time();//��ȡϵͳʱ��
										bsp_rtc_set_alarm();			
										bsp_rtc_get_alarm(NULL);//��ȡ����ʱ��
										bsp_rtc_alarm_clr();//�������ӻ���	
										g_ucRtcCali = 0;		
									}
	#endif
	
}


