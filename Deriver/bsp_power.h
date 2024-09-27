#ifndef _BSP_POWER_H
#define _BSP_POWER_H

void bsp_power_init(void);

void bsp_power_vdd_on(void);
void bsp_power_vdd_off(void);
void bsp_power_vcc_on(void);	
void bsp_power_vcc_off(void);
void bsp_power_vout_on(void);
void bsp_power_vout_off(void);
void bsp_power_ble_on(void);
void bsp_power_ble_off(void);
void bsp_power_rpt_on(void);
void bsp_power_rpt_off(void);
void bsp_power_pwk_on(void);
void bsp_power_pwk_off(void);
void bsp_power_psm_on(void);
void bsp_power_psm_off(void);
void bsp_power_reset_on(void);
void bsp_power_reset_off(void);
#endif

