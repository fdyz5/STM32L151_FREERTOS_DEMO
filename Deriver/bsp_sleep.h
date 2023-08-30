#ifndef _BSP_SLEEP_H
#define _BSP_SLEEP_H

void bsp_sleep_init(void);
uint8_t bsp_sleep(void);

extern uint8_t g_ucWkpTrig;
#endif
