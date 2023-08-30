#include "bsp.h"
#include <stdio.h>
#include <stdbool.h>
#define RCC_GPIO_TRIG1 		RCC_AHBPeriph_GPIOB
#define GPIO_PORT_TRIG1    	GPIOB
#define GPIO_PIN_TRIG1	    GPIO_Pin_7




///****************���ſ��ȼ��*******************/


volatile uint8_t g_ucMpuTriggerCnt = 0x00;
uint8_t mpu6050_CollFlag;//����MPU6050�ɼ����ݱ�־
uint8_t trigger = 0x00;
void bsp_trigger_init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��1������GPIOʱ�� */
	RCC_AHBPeriphClockCmd(RCC_GPIO_TRIG1, ENABLE);

	/* ��2�����������еİ���GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	/* �������������� */
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_TRIG1;
	GPIO_Init(GPIO_PORT_TRIG1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	/* �������������� */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	/* ʹ��SYSCFGʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource7);
	EXTI_InitStructure.EXTI_Line                         = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode                         = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger                      = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd                      = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel                   = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x00;   //�����ȼ��������ã�������Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI->IMR |=(1<<7);
	
#ifndef __RS485__	
	{		
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);
		EXTI_InitStructure.EXTI_Line                         = EXTI_Line0;
		EXTI_InitStructure.EXTI_Mode                         = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger 					 = EXTI_Trigger_Falling;			
		
		EXTI_InitStructure.EXTI_LineCmd                      = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel                   = EXTI0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x01;
		NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		EXTI->IMR |= (1<<0);	

		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);
		EXTI_InitStructure.EXTI_Line                         = EXTI_Line1;
		EXTI_InitStructure.EXTI_Mode                         = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger 					 = EXTI_Trigger_Falling;			
	
		EXTI_InitStructure.EXTI_LineCmd                      = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel                   = EXTI1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x01;
		NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		EXTI->IMR |= (1<<1);

		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);
		EXTI_InitStructure.EXTI_Line                         = EXTI_Line2;
		EXTI_InitStructure.EXTI_Mode                         = EXTI_Mode_Interrupt;

		EXTI_InitStructure.EXTI_Trigger 					 = EXTI_Trigger_Falling;			
	
		EXTI_InitStructure.EXTI_LineCmd                      = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel                   = EXTI2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x01;
		NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		EXTI->IMR |= (1<<2);
	}
#endif
	
#if  MPU6050_EN  
	
	  	/* ��1������GPIOʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	/* ��2�����������еİ���GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	/* �������������� */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	


	/* ʹ��SYSCFGʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource3);
	EXTI_InitStructure.EXTI_Line                         = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode                         = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger                      = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd                      = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel                   = EXTI3_IRQn  ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x00;   //�����ȼ��������ã�������Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI->IMR |=(1<<3);
	
	
	
	
 //   SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);//Ī�������жϣ�����������
		EXTI_InitStructure.EXTI_Line                         = EXTI_Line0;
		EXTI_InitStructure.EXTI_Mode                         = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger                      = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd                      = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel                   = EXTI0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		EXTI->IMR |= (1<<0);	

		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);
		EXTI_InitStructure.EXTI_Line                         = EXTI_Line1;
		EXTI_InitStructure.EXTI_Mode                         = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger                      = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd                      = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel                   = EXTI1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		EXTI->IMR |= (1<<1);

		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);
		EXTI_InitStructure.EXTI_Line                         = EXTI_Line2;
		EXTI_InitStructure.EXTI_Mode                         = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger                      = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd                      = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel                   = EXTI2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		EXTI->IMR |= (1<<2);
#endif			
}

uint8_t bsp_trigger_get(void)
{
	return trigger;
}

void bsp_trigger_clr(void)
{
	EXTI->IMR |=(1<<0);
	EXTI->IMR |=(1<<1);	 
	EXTI->IMR |=(1<<2);
	EXTI->IMR |=(1<<3);
//	EXTI->IMR |=(1<<7);	
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
    printf("\r\n EXTI_Line Power�����ж�\r\n"); 		
		EXTI_ClearITPendingBit(EXTI_Line7); /* ����жϱ�־λ */
	}			
}




	void EXTI0_IRQHandler(void)
			{		
				if(EXTI_GetITStatus(EXTI_Line0) != RESET)
				{			
					//sensor0StatesCheck();		
					  printf("\r\n EXTI0_IRQHandler�����ж�\r\n"); 
					 buzzerUpTime(100);
	  			EXTI_ClearITPendingBit(EXTI_Line0); /* ����жϱ�־λ */
				}			
			}

			void EXTI1_IRQHandler(void)
			{
				if(EXTI_GetITStatus(EXTI_Line1) != RESET)
				{			
					//sensor1StatesCheck();		
					  printf("\r\n EXTI1_IRQHandler�����ж�\r\n");
            buzzerUpTime(100);					
					EXTI_ClearITPendingBit(EXTI_Line1); /* ����жϱ�־λ */
				}			
			}

			void EXTI2_IRQHandler(void)
			{
				if(EXTI_GetITStatus(EXTI_Line2) != RESET)
				{				
				//	sensor2StatesCheck();
          printf("\r\n EXTI2_IRQHandler�����ж�\r\n"); 
          buzzerUpTime(100);					
					EXTI_ClearITPendingBit(EXTI_Line2); /* ����жϱ�־λ */
				}			
			}  
			void EXTI3_IRQHandler(void)
			{
				if(EXTI_GetITStatus(EXTI_Line3) != RESET)
				{	
          g_ucMpuTriggerCnt++;
					if(g_ucMpuTriggerCnt >=3)
					{
						g_ucMpuTriggerCnt = 0;
						mpu6050_CollFlag = 1;
//						g_ucRptTrig = 1;
//						g_ucAqtTrig = 1;
					}
	      	printf("g_ucMpu_triggerCnt = %d!\r\n",g_ucMpuTriggerCnt);
					EXTI_ClearITPendingBit(EXTI_Line3); /* ����жϱ�־λ */
				}			
			} 			

	

