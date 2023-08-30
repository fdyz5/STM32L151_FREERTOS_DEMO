#ifndef _BSP_H
#define _BSP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include "stm32l1xx.h"


typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

//#include "bsp.h"
//#include "bsp_timer.h"
#include "bsp_led.h"
#include "bsp_power.h"
#include "bsp_uart_fifo.h"
//#include "bsp_rtc.h"
//#include "bsp_iwdg.h"
//#include "bsp_i2c_gpio.h"
//#include "bsp_cpu_adc.h"
//#include "bsp_eeprom_24xx.h"
#include "bsp_trigger.h"
//#include "bsp_spi_bus.h"
//#include "bsp_spi_flash.h"
//#include "bsp_sleep.h"
#include "bsp_beep.h"  

void bsp_init(void);



#endif
