#ifndef _BSP_LED_H
#define _BSP_LED_H

#include <stdint.h>
#define GPIO_RCC_LED1					RCC_AHBPeriph_GPIOA  
#define LED1_GPIO_PORT             		GPIOA
#define LED1_PIN                  		GPIO_Pin_4
         
#define GPIO_RCC_LED2					RCC_AHBPeriph_GPIOC  
#define LED2_GPIO_PORT             		GPIOC
#define LED2_PIN                  		GPIO_Pin_5
         
#define GPIO_RCC_LED5					RCC_AHBPeriph_GPIOC 
#define LED5_GPIO_PORT             		GPIOC
#define LED5_PIN                  		GPIO_Pin_11

#define GPIO_RCC_LED6					RCC_AHBPeriph_GPIOC 
#define LED6_GPIO_PORT             		GPIOC
#define LED6_PIN                  		GPIO_Pin_12

#define GPIO_RCC_LED7					RCC_AHBPeriph_GPIOC 
#define LED7_GPIO_PORT             		GPIOC
#define LED7_PIN                  		GPIO_Pin_10

// 枚举类型定义LED作用
typedef enum 
{
    NO_ALARM = 0,                      //无报警
    SHIFT_ALARM_LED,                      //位移报警
    VOLTAGE_LOSS_ALARM_LED ,             //失压报警
    LEVEL_ALARM_LED,                    //液位报警
    HIGH_PRESSURE_ALARM_LED           //高压报警
} AlarmStatusLED;

extern uint16_t g_usLed1Time;
extern uint16_t g_usLed2Time;
extern uint16_t g_usLed5Time;
extern uint16_t g_usLed6Time;
extern uint16_t g_usLed7Time;
extern uint16_t g_usBeepTime;

void bsp_led_init(void);
void bsp_led_on(uint8_t n);
void bsp_led_off(uint8_t n);
void bsp_led_toggle(uint8_t n);
uint8_t bsp_led_Is_On(uint8_t n);

#endif
