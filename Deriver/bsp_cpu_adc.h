#ifndef _BSP_CPU_ADC_H
#define _BSP_CPU_ADC_H

void bsp_cpu_adc_init(void);

uint16_t bsp_cpu_adc_get_battery(void) ;

uint16_t bsp_cpu_adc_get_channel1(uint16_t vref) ;
uint16_t bsp_cpu_adc_get_channel2(uint16_t vref) ;
uint16_t bsp_cpu_adc_get_channel3(uint16_t vref) ;

#endif
