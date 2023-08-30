#ifndef _BSP_HT1622_H
#define _BSP_HT1622_H

// 命令定义
#define SYS_DIS 0x00 // 系统振荡器关
#define SYS_EN 0x01 // 系统振荡器开
#define LCD_OFF 0x02 // 关闭LCD显示
#define LCD_ON 0x03 // 开启LCD显示
#define TIMER_DIS 0x04 // 时基输出失效
#define WDT_DIS 0x05 // WDT 溢出标志输出失效
#define TIMER_EN 0x06 // 使能时基输出
#define WDT_EN 0x07 // 使能 WDT 溢出标志失效
#define TONE_OFF 0x08 // 关闭声音输出
#define CLR_TIMER 0x0D // 时基发生器清零
#define CLR_WDT 0x0F // 清除 WDT 状态
#define RCosc 0x18 // 系统时钟源，片内 RC 振荡器
#define NORMAL 0xE3 // 系统时钟源，片内 RC 振荡器
 
extern uint8_t g_ucLcdSta;

void bsp_ht1622_init(void);

void bsp_ht1622_send_command(uint8_t cmd);
void bsp_ht1622_send_data(uint8_t addr, uint8_t data);
void bsp_ht1622_send_datas(uint8_t addr, uint8_t *ptr, uint8_t len);

void bsp_ht1622_turn_off(void);
void bsp_ht1622_turn_on(void);

int lcd_process(void);

#endif
