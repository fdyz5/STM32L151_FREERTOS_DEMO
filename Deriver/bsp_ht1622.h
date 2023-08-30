#ifndef _BSP_HT1622_H
#define _BSP_HT1622_H

// �����
#define SYS_DIS 0x00 // ϵͳ������
#define SYS_EN 0x01 // ϵͳ������
#define LCD_OFF 0x02 // �ر�LCD��ʾ
#define LCD_ON 0x03 // ����LCD��ʾ
#define TIMER_DIS 0x04 // ʱ�����ʧЧ
#define WDT_DIS 0x05 // WDT �����־���ʧЧ
#define TIMER_EN 0x06 // ʹ��ʱ�����
#define WDT_EN 0x07 // ʹ�� WDT �����־ʧЧ
#define TONE_OFF 0x08 // �ر��������
#define CLR_TIMER 0x0D // ʱ������������
#define CLR_WDT 0x0F // ��� WDT ״̬
#define RCosc 0x18 // ϵͳʱ��Դ��Ƭ�� RC ����
#define NORMAL 0xE3 // ϵͳʱ��Դ��Ƭ�� RC ����
 
extern uint8_t g_ucLcdSta;

void bsp_ht1622_init(void);

void bsp_ht1622_send_command(uint8_t cmd);
void bsp_ht1622_send_data(uint8_t addr, uint8_t data);
void bsp_ht1622_send_datas(uint8_t addr, uint8_t *ptr, uint8_t len);

void bsp_ht1622_turn_off(void);
void bsp_ht1622_turn_on(void);

int lcd_process(void);

#endif
