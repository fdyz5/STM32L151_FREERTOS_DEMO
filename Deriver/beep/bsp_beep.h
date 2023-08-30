#ifndef __BEEP_H
#define	__BEEP_H
#include <stdint.h>



/* ������������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶķ��������� */
#define BEEP_GPIO_PORT    	GPIOB			              /* GPIO�˿� */
#define BEEP_GPIO_CLK 	    RCC_AHBPeriph_GPIOB		/* GPIO�˿�ʱ�� */
#define BEEP_GPIO_PIN		    GPIO_Pin_6			        /* ���ӵ���������GPIO */

/* �ߵ�ƽʱ���������� */
#define ON  1
#define OFF 0

/* ���κ꣬��������������һ��ʹ�� */
#define BEEP(a)	if (a)	\
					GPIO_SetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);\
					else		\
					GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN)

void BEEP_GPIO_Config(void);
void buzzer_beep(uint16_t milliseconds);	
void buzzerUpTime(uint16_t milliseconds);						
#endif /* __BEEP_H */
