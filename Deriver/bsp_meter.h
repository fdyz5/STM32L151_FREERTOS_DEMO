#ifndef _BSP_METER_H
#define _BSP_METER_H

int16_t bsp_meter_get_addr(void);
int16_t bsp_meter_get_data(uint8_t addr,uint8_t fuc,uint16_t reg,uint16_t regs,uint8_t *pBuf);
int16_t bsp_meter_set_data(uint8_t addr,uint8_t fuc,uint16_t reg,uint16_t regs,uint8_t *pBuf,uint8_t len);

#endif
