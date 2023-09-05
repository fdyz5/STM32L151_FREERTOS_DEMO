#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32l1xx.h"
#include "bsp_cpu_adc.h"
#include "bsp_para.h"
#include "delay.h"

#include "FreeRTOS.h"
#include "task.h"

#define bsp_timer_delay_ms  delay_xms
#define GPIO_RCC_AI1					RCC_AHBPeriph_GPIOA  
#define AI1_GPIO_PORT             		GPIOA
#define AI1_PIN                  		GPIO_Pin_5           

#define GPIO_RCC_AI2					RCC_AHBPeriph_GPIOA  
#define AI2_GPIO_PORT             		GPIOA
#define AI2_PIN                  		GPIO_Pin_6 

#define GPIO_RCC_AI3					RCC_AHBPeriph_GPIOA  
#define AI3_GPIO_PORT             		GPIOA
#define AI3_PIN                  		GPIO_Pin_7 


#define SAMP_COUNT	24		/* 样本个数，表示200ms内的采样数据求平均值 */


#define VREF_CAL    *(__IO uint16_t *)(0x1FF800F8)

static uint32_t bsp_cpu_adc_filter(uint16_t *pBuf);

void bsp_cpu_adc_init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);
	
	RCC_AHBPeriphClockCmd(GPIO_RCC_AI1, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = AI1_PIN;
	GPIO_Init(AI1_GPIO_PORT,&GPIO_InitStruct);
	
	RCC_AHBPeriphClockCmd(GPIO_RCC_AI2, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = AI2_PIN;
	GPIO_Init(AI2_GPIO_PORT,&GPIO_InitStruct);
	
	RCC_AHBPeriphClockCmd(GPIO_RCC_AI3, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = AI3_PIN;
	GPIO_Init(AI3_GPIO_PORT,&GPIO_InitStruct);
	
	/* Enable the HSI */
	RCC_HSICmd(ENABLE);
	/* Wait until HSI oscillator is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Resolution           = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode         = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode   = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign            = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion      = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, DISABLE);
	
	ADC_BankSelection(ADC1, ADC_Bank_A);
}

uint16_t bsp_cpu_adc_get_battery(void)   
{
	uint8_t i;
	uint16_t buf[SAMP_COUNT];
	uint16_t temp;
	uint16_t voltage;
	uint16_t vref_cal;
	
	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
    {
    } 	
	
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_Vrefint, 1, ADC_SampleTime_192Cycles );	  	

	for(i=0; i<SAMP_COUNT; i++)
	{
		ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
		 
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
		//返回最近一次ADC1规则组的转换结果
		buf[i] = ADC_GetConversionValue(ADC1);
		
		bsp_timer_delay_ms(10);
	}
	
	ADC_TempSensorVrefintCmd(DISABLE);
	ADC_Cmd(ADC1, DISABLE);

	temp    = bsp_cpu_adc_filter(buf);
	
	vref_cal = VREF_CAL;
//	printf("\r\nvref_cal=%d\r\n",vref_cal);
	

	voltage = 3000*vref_cal/temp;	

	bsp_timer_delay_ms(10);
	
    return voltage;
}

uint16_t bsp_cpu_adc_get_channel1(uint16_t vref)   
{
	uint8_t i;
	uint16_t buf[SAMP_COUNT];
	uint16_t temp;
	uint16_t value;
	
//	if(para_t.ai1Enable != 1)
//	{
//		printf("->channel 1 is not enable\r\n");
//		return 0;
//	}

	ADC_Cmd(ADC1, ENABLE);
	
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
    {
    }    
	
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_192Cycles );	  	
	
	for(i=0; i<SAMP_COUNT; i++)
	{
		ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
		 
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
		//返回最近一次ADC1规则组的转换结果
		buf[i] = ADC_GetConversionValue(ADC1);
		
		bsp_timer_delay_ms(10);
	}
	
	ADC_Cmd(ADC1, DISABLE);

	temp    = bsp_cpu_adc_filter(buf);
//	value   = vref * temp / 4096;  
	value   = temp; 	
//	printf("VAI1:%d.%02dV\r\n",value/100,value%100);
	bsp_timer_delay_ms(10);
	
    return value;
}

uint16_t bsp_cpu_adc_get_channel2(uint16_t vref)   
{
	uint8_t i;
	uint16_t buf[SAMP_COUNT];
	uint16_t temp;
	uint16_t value;
	
//	if(para_t.ai2Enable != 1)
//	{
//		printf("->channel 2 is not enable\r\n");
//		return 0;
//	}
 // taskENTER_CRITICAL();//进入临界段，禁止任务切换
	ADC_Cmd(ADC1, ENABLE);
	
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
    {
    }    
	
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_192Cycles );	  	
	
	for(i=0; i<SAMP_COUNT; i++)
	{
		ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
		 
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
		//返回最近一次ADC1规则组的转换结果
		buf[i] = ADC_GetConversionValue(ADC1);
		
		bsp_timer_delay_ms(10);
	}
	
	ADC_Cmd(ADC1, DISABLE);

	temp    = bsp_cpu_adc_filter(buf);
//	value   = vref * temp / 4096; 
	value   = temp; 	
//	printf("->channel 2 voltage:%d.%02dV\r\n",value/100,value%100);
	bsp_timer_delay_ms(10);
	
    return value;
}

uint16_t bsp_cpu_adc_get_channel3(uint16_t vref)   
{
	uint8_t i;
	uint16_t buf[SAMP_COUNT];
	uint16_t temp;
	uint16_t value;
	
//	if(para_t.ai3Enable != 1)
//	{
//		printf("->channel 3 is not enable\r\n");
//		return 0;
//	}
	
	ADC_Cmd(ADC1, ENABLE);
	
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
    {
    }    
	
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_192Cycles );	  	
	
	for(i=0; i<SAMP_COUNT; i++)
	{
		ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
		 
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
		//返回最近一次ADC1规则组的转换结果
		buf[i] = ADC_GetConversionValue(ADC1);
		
		bsp_timer_delay_ms(10);
	}
	
	ADC_Cmd(ADC1, DISABLE);

	temp    = bsp_cpu_adc_filter(buf);
//	value   = vref * temp / 4096; 
	value   = temp; 	
//	printf("->channel 3 voltage:%d.%02dV\r\n",value/100,value%100);
	bsp_timer_delay_ms(10);
	
    return value;
}

static uint32_t bsp_cpu_adc_filter(uint16_t *_pBuf)
{
	uint8_t flag;
	uint8_t i;
	uint32_t usTemp;
	uint32_t uiSum;

	/* 升序排列 */
    do
	{
		flag = 0;
		for (i=0; i<SAMP_COUNT-1; i++)
		{
			if (_pBuf[i] > _pBuf[i+1])
			{
				usTemp     = _pBuf[i + 1];
				_pBuf[i+1] = _pBuf[i];
				_pBuf[i]   = usTemp;
				flag = 1;
			}
		}
	}while(flag);

	uiSum = 0;
	for (i=0; i<SAMP_COUNT/3; i++)
	{
		uiSum += _pBuf[SAMP_COUNT/3 + i];
	}
	usTemp = uiSum / (SAMP_COUNT/3);
	
	return usTemp;
}

