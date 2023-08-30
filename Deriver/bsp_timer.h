#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H


/*
	在此定义若干个软件定时器全局变量
	注意，必须增加__IO 即 volatile，因为这个变量在中断和主程序中同时被访问，有可能造成编译器错误优化。
*/

#define TMR_COUNT	13		/* 软件定时器的个数 （定时器ID范围 0 - 3) */


typedef enum
{
	TMR0 = 0,		
	TMR1,	
	TMR2,	
	TMR3,	
	TMR4,
}TMR_E;

/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
typedef enum
{
	TMR_ONCE_MODE = 0,		/* 一次工作模式 */
	TMR_AUTO_MODE = 1		/* 自动定时工作模式 */
}TMR_MODE_E;

/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
typedef struct
{
	volatile uint8_t Mode;		/* 计数器模式，1次性 */
	volatile uint8_t Flag;		/* 定时到达标志  */
	volatile uint32_t Count;	/* 计数器 */
	volatile uint32_t PreLoad;	/* 计数器预装值 */
}SOFT_TMR;

void bsp_timer_init(void);

int32_t bsp_timer_get_tick(void);

uint8_t bsp_timer_1s(void);
void bsp_timer_delay_us(uint32_t n);
void bsp_timer_delay_ms(uint32_t n);

uint8_t bsp_timer_check(uint8_t _id);
void bsp_timer_stop(uint8_t _id);
void bsp_timer_star_auto(uint8_t _id, uint32_t _period);
void bsp_timer_star_once(uint8_t _id, uint32_t _period);
uint32_t bsp_timer_get_count(uint8_t _id);

#endif
