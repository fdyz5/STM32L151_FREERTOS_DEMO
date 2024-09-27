#include "bsp.h"
       
uint16_t g_usLed1Time = 0;
uint16_t g_usLed2Time = 0;
uint16_t g_usLed5Time = 0;
uint16_t g_usLed6Time = 0;
uint16_t g_usLed7Time = 0;

void bsp_led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	RCC_AHBPeriphClockCmd(GPIO_RCC_LED1, ENABLE);	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = LED1_PIN;
	GPIO_Init(LED1_GPIO_PORT,&GPIO_InitStruct);
	
	RCC_AHBPeriphClockCmd(GPIO_RCC_LED2, ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = LED2_PIN;
	GPIO_Init(LED2_GPIO_PORT,&GPIO_InitStruct);
	
	RCC_AHBPeriphClockCmd(GPIO_RCC_LED5, ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = LED5_PIN;
	GPIO_Init(LED5_GPIO_PORT,&GPIO_InitStruct);
	
	RCC_AHBPeriphClockCmd(GPIO_RCC_LED6, ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = LED6_PIN;
	GPIO_Init(LED6_GPIO_PORT,&GPIO_InitStruct);	
	
	RCC_AHBPeriphClockCmd(GPIO_RCC_LED7, ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = LED7_PIN;
	GPIO_Init(LED7_GPIO_PORT,&GPIO_InitStruct);		

}

void bsp_led_on(uint8_t n)
{
	switch(n)
	{
		case 1:LED1_GPIO_PORT->BSRRL = LED1_PIN;break;
		case 2:LED2_GPIO_PORT->BSRRL = LED2_PIN;break;
		case 5:LED5_GPIO_PORT->BSRRL = LED5_PIN;break;
		case 6:LED6_GPIO_PORT->BSRRL = LED6_PIN;break;
		case 7:LED7_GPIO_PORT->BSRRL = LED7_PIN;break;			
	}
}

void bsp_led_off(uint8_t n)
{
	switch(n)
	{
		case 1:LED1_GPIO_PORT->BSRRH = LED1_PIN;break;
		case 2:LED2_GPIO_PORT->BSRRH = LED2_PIN;break;
		case 5:LED5_GPIO_PORT->BSRRH = LED5_PIN;break;
		case 6:LED6_GPIO_PORT->BSRRH = LED6_PIN;break;
		case 7:LED7_GPIO_PORT->BSRRH = LED7_PIN;break;		
	}
}

void bsp_led_toggle(uint8_t n)
{
	switch(n)
	{
		case 1:LED1_GPIO_PORT->ODR ^= LED1_PIN;break;
		case 2:LED2_GPIO_PORT->ODR ^= LED2_PIN;break;
		case 5:LED5_GPIO_PORT->ODR ^= LED5_PIN;break;
		case 6:LED6_GPIO_PORT->ODR ^= LED6_PIN;break;
		case 7:LED7_GPIO_PORT->ODR ^= LED7_PIN;break;		
	}
}

uint8_t bsp_led_Is_On(uint8_t n)
{
	if (n == 1)
	{
		if ((LED1_GPIO_PORT->ODR & LED1_PIN) == 0)
		{
			return 1;
		}
		return 0;
	}
	else if (n == 2)
	{
		if ((LED2_GPIO_PORT->ODR & LED2_PIN) == 0)
		{
			return 1;
		}
		return 0;
	}
	else if (n == 5)
	{
		if ((LED5_GPIO_PORT->ODR & LED5_PIN) == 0)
		{
			return 1;
		}
		return 0;
	}
	else if (n == 6)
	{
		if ((LED6_GPIO_PORT->ODR & LED6_PIN) == 0)
		{
			return 1;
		}
		return 0;
	}	
	else if (n == 7)
	{
		if ((LED7_GPIO_PORT->ODR & LED7_PIN) == 0)
		{
			return 1;
		}
		return 0;
	}	

	return 0;
}
