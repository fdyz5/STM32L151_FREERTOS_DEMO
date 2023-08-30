#ifndef _BSP_TRIGGER_H
#define _BSP_TRIGGER_H

/*外部中断EXIT相关宏定义*/
#if MPU6050_EN
			#define             RCC_GPIO_TRIG1 		                   RCC_AHBPeriph_GPIOB
			#define             EXTI_GPIO_CLK                        RCC_APB2Periph_SYSCFG     
			#define             EXTI_GPIO_PORT                       GPIOB   
			#define             EXTI_GPIO_PIN                        GPIO_Pin_6
			#define             EXTI_SOURCE_PORT                     EXTI_PortSourceGPIOB
			#define             EXTI_SOURCE_PIN                      EXTI_PinSource6
			#define             EXTI_LINE                            EXTI_Line6
			#define             EXTI_IRQ                             EXTI9_5_IRQn
			#define             EXTI_INT_FUNCTION                    EXTI9_5_IRQHandler
#else
			
#endif






void bsp_trigger_init(void);
void bsp_trigger_clr(void);

uint8_t bsp_trigger_get(void);

#endif
