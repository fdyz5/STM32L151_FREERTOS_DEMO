#ifndef _BSP_RTC_H
#define _BSP_RTC_H

typedef struct
{
	uint16_t Year;
	uint8_t Mon;
	uint8_t Day;	
	uint8_t Hour;		
	uint8_t Min;				
	uint8_t Sec;					
	uint8_t Week;	
}RTC_T;

extern RTC_T g_tRTC;
extern uint8_t g_ucRtcCali;
extern uint32_t g_uiRptTime;
extern uint32_t g_uiAqtTime;

void bsp_rtc_init(void);
void bsp_rtc_alarm_init(void);
void bsp_rtc_get_time(void);
void bsp_rtc_get_alarm(uint8_t *pBuf);
void bsp_rtc_set_alarm(void);
void bsp_rtc_set_time(uint8_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec);
void bsp_rtc_set_time_bcd(uint8_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec);
uint32_t bsp_rtc_get_unix(void);
uint8_t bsp_rtc_alarm_wkp(void);
void bsp_rtc_alarm_clr(void);

#endif
