#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H


/*
	�ڴ˶������ɸ������ʱ��ȫ�ֱ���
	ע�⣬��������__IO �� volatile����Ϊ����������жϺ���������ͬʱ�����ʣ��п�����ɱ����������Ż���
*/

#define TMR_COUNT	13		/* �����ʱ���ĸ��� ����ʱ��ID��Χ 0 - 3) */


typedef enum
{
	TMR0 = 0,		
	TMR1,	
	TMR2,	
	TMR3,	
	TMR4,
}TMR_E;

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef enum
{
	TMR_ONCE_MODE = 0,		/* һ�ι���ģʽ */
	TMR_AUTO_MODE = 1		/* �Զ���ʱ����ģʽ */
}TMR_MODE_E;

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
	volatile uint8_t Mode;		/* ������ģʽ��1���� */
	volatile uint8_t Flag;		/* ��ʱ�����־  */
	volatile uint32_t Count;	/* ������ */
	volatile uint32_t PreLoad;	/* ������Ԥװֵ */
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
