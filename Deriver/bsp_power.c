#include "bsp.h"
#include "var.h"
VAR_T  g_tVar;//临时定义在这个位置，后续做调整
//VDD
#define GPIO_RCC_VDD					RCC_AHBPeriph_GPIOB 
#define VDD_GPIO_PORT             		GPIOB 
#define VDD_PIN                  		GPIO_Pin_5 

//VOUT
#define GPIO_RCC_VOUT					RCC_AHBPeriph_GPIOC  
#define VOUT_GPIO_PORT             		GPIOC
#define VOUT_PIN                  		GPIO_Pin_2 

//BLE
#define GPIO_RCC_BLE					RCC_AHBPeriph_GPIOB  
#define BLE_GPIO_PORT             		GPIOB
#define BLE_PIN                  		GPIO_Pin_9          

//RPT
#define GPIO_RCC_RPT					RCC_AHBPeriph_GPIOB  
#define RPT_GPIO_PORT             		GPIOB
#define RPT_PIN                  		GPIO_Pin_8  

//PWK
#define GPIO_RCC_PWK					RCC_AHBPeriph_GPIOC  
#define PWK_GPIO_PORT             		GPIOC
#define PWK_PIN                  		GPIO_Pin_9 

//PSM
#define GPIO_RCC_PSM					RCC_AHBPeriph_GPIOA  
#define PSM_GPIO_PORT             		GPIOA
#define PSM_PIN                  		GPIO_Pin_8

//VCC
#define GPIO_RCC_VCC					RCC_AHBPeriph_GPIOA  
#define VCC_GPIO_PORT             		GPIOA
#define VCC_PIN                  		GPIO_Pin_1

void bsp_power_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	RCC_AHBPeriphClockCmd(GPIO_RCC_VDD, ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = VDD_PIN;
	GPIO_Init(VDD_GPIO_PORT,&GPIO_InitStruct);
	
	RCC_AHBPeriphClockCmd(GPIO_RCC_VOUT, ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = VOUT_PIN;
	GPIO_Init(VOUT_GPIO_PORT,&GPIO_InitStruct);
	
	RCC_AHBPeriphClockCmd(GPIO_RCC_BLE, ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = BLE_PIN;
	GPIO_Init(BLE_GPIO_PORT,&GPIO_InitStruct);
	
	RCC_AHBPeriphClockCmd(GPIO_RCC_RPT, ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = RPT_PIN;
	GPIO_Init(RPT_GPIO_PORT,&GPIO_InitStruct);

	RCC_AHBPeriphClockCmd(GPIO_RCC_PWK, ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = PWK_PIN;
	GPIO_Init(PWK_GPIO_PORT,&GPIO_InitStruct);
	
	RCC_AHBPeriphClockCmd(GPIO_RCC_PSM, ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = PSM_PIN;
	GPIO_Init(PSM_GPIO_PORT,&GPIO_InitStruct);	

	RCC_AHBPeriphClockCmd(GPIO_RCC_PWK, ENABLE);	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = PWK_PIN;
	GPIO_Init(PWK_GPIO_PORT,&GPIO_InitStruct);
	
	RCC_AHBPeriphClockCmd(GPIO_RCC_PSM, ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = PSM_PIN;
	GPIO_Init(PSM_GPIO_PORT,&GPIO_InitStruct);
	
	RCC_AHBPeriphClockCmd(GPIO_RCC_VCC, ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = VCC_PIN;
	GPIO_Init(VCC_GPIO_PORT,&GPIO_InitStruct);	

	bsp_power_vdd_on();
	bsp_power_ble_on();
	
	bsp_power_rpt_off();
	
	bsp_power_pwk_off();
//	bsp_power_psm_off();	
	bsp_power_vout_off();
	bsp_power_vcc_on();
	bsp_power_reset_off();	
	
	

}

void bsp_power_vdd_on(void)
{
//	GPIO_InitTypeDef  GPIO_InitStruct;
//	
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
//	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
//	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
//	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	VDD_GPIO_PORT->BSRRL = VDD_PIN;
}

void bsp_power_vdd_off(void)
{
//	GPIO_InitTypeDef  GPIO_InitStruct;
//	
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
//	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
//	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
//	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	VDD_GPIO_PORT->BSRRH = VDD_PIN;
}

void bsp_power_vcc_on(void)	
{
	VCC_GPIO_PORT->BSRRL = VCC_PIN;
}

void bsp_power_vcc_off(void)
{
	VCC_GPIO_PORT->BSRRH = VCC_PIN;
}

void bsp_power_vout_on(void)
{	
	GPIO_InitTypeDef  GPIO_InitStruct;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2;
	GPIO_Init(GPIOD,&GPIO_InitStruct);		

	GPIOD->BSRRH = GPIO_Pin_2;	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
	
	GPIOB->BSRRH = GPIO_Pin_3 | GPIO_Pin_4;	
	
	
	if(g_tVar.ucPWRDY == 1) //VOUT_24V
	{
	#ifdef __SL6200N__
		GPIOB->BSRRL = GPIO_Pin_4;
	#else
		GPIOB->BSRRL = GPIO_Pin_3;		
	#endif
				
	}
	else if(g_tVar.ucPWRDY == 2) //VOUT_12V	
	{
	#ifdef __SL6200N__
		GPIOB->BSRRL = GPIO_Pin_3;
	#else
		GPIOB->BSRRL = GPIO_Pin_4;		
	#endif			
	}
	else
	{
		GPIOD->BSRRL = GPIO_Pin_2;	//VOUT_5V		
	}			
	
	VOUT_GPIO_PORT->BSRRL = VOUT_PIN;
}

void bsp_power_vout_off(void)
{	
	VOUT_GPIO_PORT->BSRRH = VOUT_PIN;
	
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	
	GPIOD->BSRRH = GPIO_Pin_2;	
	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
	
	GPIOB->BSRRH = GPIO_Pin_3 | GPIO_Pin_4;	
}

void bsp_power_ble_on(void)
{	
	BLE_GPIO_PORT->BSRRH = BLE_PIN;
}

void bsp_power_ble_off(void)
{
	BLE_GPIO_PORT->BSRRL = BLE_PIN;
}

void bsp_power_rpt_on(void)
{
	RPT_GPIO_PORT->BSRRH = RPT_PIN;
}

void bsp_power_rpt_off(void)
{
	RPT_GPIO_PORT->BSRRL = RPT_PIN;
}

void bsp_power_pwk_on(void)
{
	
	PWK_GPIO_PORT->BSRRL = PWK_PIN;
}

void bsp_power_pwk_off(void)
{		
	PWK_GPIO_PORT->BSRRH = PWK_PIN;
}

void bsp_power_psm_on(void)
{
	PSM_GPIO_PORT->BSRRL = PSM_PIN;
}

void bsp_power_psm_off(void)
{
	PSM_GPIO_PORT->BSRRH = PSM_PIN;
}

void bsp_power_reset_on(void)
{
	PSM_GPIO_PORT->BSRRL = PSM_PIN;
}

void bsp_power_reset_off(void)
{
	PSM_GPIO_PORT->BSRRH = PSM_PIN;
}



