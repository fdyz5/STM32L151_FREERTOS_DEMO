#include "app.h"


#define INT_TO_BCD(x)	(x + ( x / 10) * 6)

RTC_T g_tRTC;

static uint8_t alarmWkp = 0x00;
uint8_t g_ucRtcCali = 0; //RTC校准
uint8_t g_ucAlarmSet = 0; 
uint32_t g_uiRptTime = 0;
uint32_t g_uiAqtTime = 0;

void bsp_rtc_init(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	

	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to RTC */
	PWR_RTCAccessCmd(ENABLE);
	
	/*!< Reset RTC Domain */
	RCC_RTCResetCmd(ENABLE);
	RCC_RTCResetCmd(DISABLE);

	RCC_LSICmd(ENABLE);
	
	/* Wait till LSE is ready */ 
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{

	}

	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

	/* Enable the RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC APB registers synchronisation */
	RTC_WaitForSynchro();

	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;		
	RTC_InitStructure.RTC_SynchPrediv = 0x137;		
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);
	
	bsp_rtc_set_time(20,11,11,16,00,00);	
}

void bsp_rtc_set_time(uint8_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	RTC_TimeStruct.RTC_Hours   = hour;
	RTC_TimeStruct.RTC_Minutes = min;
	RTC_TimeStruct.RTC_Seconds = sec;
	RTC_TimeStruct.RTC_H12     = RTC_H12_AM;
	RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct);
	
	RTC_DateStruct.RTC_Year    = year;
	RTC_DateStruct.RTC_Month   = month;
	RTC_DateStruct.RTC_Date    = day;
	RTC_DateStruct.RTC_WeekDay = 1;
	RTC_SetDate(RTC_Format_BIN,&RTC_DateStruct);
}

void bsp_rtc_set_time_bcd(uint8_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	RTC_TimeStruct.RTC_Hours   = hour;
	RTC_TimeStruct.RTC_Minutes = min;
	RTC_TimeStruct.RTC_Seconds = sec;
	RTC_TimeStruct.RTC_H12     = RTC_H12_AM;
	RTC_SetTime(RTC_Format_BCD,&RTC_TimeStruct);
	
	RTC_DateStruct.RTC_Year    = year;
	RTC_DateStruct.RTC_Month   = month;
	RTC_DateStruct.RTC_Date    = day;
	RTC_DateStruct.RTC_WeekDay = 1;
	RTC_SetDate(RTC_Format_BCD,&RTC_DateStruct);
}

void bsp_rtc_get_time(void)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
	
	g_tRTC.Hour = RTC_TimeStruct.RTC_Hours;
	g_tRTC.Min  = RTC_TimeStruct.RTC_Minutes;
	g_tRTC.Sec  = RTC_TimeStruct.RTC_Seconds ;
	
	g_tRTC.Year = RTC_DateStruct.RTC_Year + 2000;
	g_tRTC.Mon  = RTC_DateStruct.RTC_Month ;
	g_tRTC.Day  = RTC_DateStruct.RTC_Date;
	
	g_tRTC.Week = RTC_DateStruct.RTC_WeekDay ;
	
//	printf("\r\nTIME:%04d-%02d-%02d %02d:%02d:%02d\r\n", 
//				  g_tRTC.Year,g_tRTC.Mon,g_tRTC.Day,g_tRTC.Hour,g_tRTC.Min,g_tRTC.Sec);	
}

uint32_t bsp_rtc_get_unix(void)
{
	struct tm rtc;
		
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
	
	rtc.tm_hour = RTC_TimeStruct.RTC_Hours;
	rtc.tm_min  = RTC_TimeStruct.RTC_Minutes;
	rtc.tm_sec  = RTC_TimeStruct.RTC_Seconds;
	
	rtc.tm_year = RTC_DateStruct.RTC_Year+100;
	rtc.tm_mon  = RTC_DateStruct.RTC_Month-1;
	rtc.tm_mday = RTC_DateStruct.RTC_Date;
	
	rtc.tm_wday = RTC_DateStruct.RTC_WeekDay ;
	
	return mktime(&rtc);     
}

void bsp_rtc_set_alarm(void)
{
	RTC_AlarmTypeDef  RTC_AlarmStructure;
	uint32_t unix;
	struct tm* next;
	uint32_t temp;
	uint32_t delay;
	
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
	RTC_AlarmStructInit(&RTC_AlarmStructure);
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel   = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmMask             = RTC_AlarmMask_None;//RTC_AlarmMask_DateWeekDay;
	
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
	
	//获取当前时间
	unix  = bsp_rtc_get_unix();	
	//计算下一次采集时间戳
	temp  = (g_tVar.usTPITV<60)? 60: g_tVar.usTPITV; //强制采集间隔>=60s 
	temp  = (temp - unix%temp);
	g_uiAqtTime = temp/5;
	unix += temp;	
	//转换为时间
	next = localtime(&unix);
	
	RTC_AlarmStructure.RTC_AlarmDateWeekDay      = INT_TO_BCD(next->tm_mday);	
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = INT_TO_BCD(next->tm_hour);
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = INT_TO_BCD(next->tm_min);
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = INT_TO_BCD(next->tm_sec);
	
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);
	
	RTC_AlarmCmd(RTC_Alarm_B,DISABLE);
	RTC_AlarmStructInit(&RTC_AlarmStructure);
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel   = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmMask             = RTC_AlarmMask_None;//RTC_AlarmMask_DateWeekDay;
	
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;	
	
	//获取当前时间
	unix  = bsp_rtc_get_unix();	
		
	//计算下一次上报时间戳
	temp  = g_tVar.usSPITV*60;
	temp  = (temp - unix%temp);
	g_uiRptTime = temp/5;
	unix += temp;
	
	if(g_tVar.ucUPDELAYEN == 1) 
	{
		delay = (g_tVar.ucaDTUSN[0]&0x0F)*60;
		printf("\r\ndelay: %d\r\n", 	delay);	
	}
	else	
	{
		delay = 0;
	}		
	unix += delay;
	
	//转换为时间
	next = localtime(&unix);

	RTC_AlarmStructure.RTC_AlarmDateWeekDay      = INT_TO_BCD(next->tm_mday);
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = INT_TO_BCD(next->tm_hour);
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = INT_TO_BCD(next->tm_min);
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = INT_TO_BCD(next->tm_sec);
	
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_B, &RTC_AlarmStructure);	
}

void bsp_rtc_get_alarm(uint8_t *pBuf)
{
	RTC_AlarmTypeDef  RTC_AlarmStructure;
	
	RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);	
	printf("AQT: %04d-%02d-%02d ",g_tRTC.Year,g_tRTC.Mon,RTC_AlarmStructure.RTC_AlarmDateWeekDay);	
	printf("%02d:%02d:%02d\r\n",\
	       RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours,\
	       RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes,\
	       RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds);
	
	RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_B, &RTC_AlarmStructure);		  		
	printf("RPT: %04d-%02d-%02d ",g_tRTC.Year,g_tRTC.Mon,RTC_AlarmStructure.RTC_AlarmDateWeekDay);			
	printf("%02d:%02d:%02d\r\n",\
	       RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours,\
	       RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes,\
	       RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds);		
}

uint8_t bsp_rtc_alarm_wkp(void)
{
	return alarmWkp;
}

void bsp_rtc_alarm_clr(void)
{
	alarmWkp = 0x00;
	
	RTC_ClearITPendingBit(RTC_IT_ALRA);//清除中断标	
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	
	RTC_ClearITPendingBit(RTC_IT_ALRB);//清除中断标		
	RTC_ITConfig(RTC_IT_ALRB,ENABLE);
	RTC_AlarmCmd(RTC_Alarm_B,ENABLE);
	
	EXTI_ClearITPendingBit(EXTI_Line17);//清除中断标志	
}

void RTC_Alarm_IRQHandler(void)
{	
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	{
		g_ucAqtAuto	= 1;	
		RTC_ITConfig(RTC_IT_ALRA,DISABLE);
		RTC_AlarmCmd(RTC_Alarm_A,DISABLE);		
		RTC_ClearITPendingBit(RTC_IT_ALRA);//清除中断标志
		g_ucRtcCali	= 1;		
	}
	
	if(RTC_GetITStatus(RTC_IT_ALRB) != RESET)
	{
		g_ucRptAuto	= 1;
		RTC_ITConfig(RTC_IT_ALRB,DISABLE);
		RTC_AlarmCmd(RTC_Alarm_B,DISABLE);		
		RTC_ClearITPendingBit(RTC_IT_ALRB);//清除中断标志
		g_ucRtcCali	= 1;		
	}


	EXTI_ClearITPendingBit(EXTI_Line17);	
}

void bsp_rtc_alarm_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_ClearITPendingBit(EXTI_Line17);
    EXTI_InitStructure.EXTI_Line    = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
    NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
    NVIC_Init(&NVIC_InitStructure); 
	
	RTC_ClearITPendingBit(RTC_IT_ALRA);//清除中断标志	
	RTC_ITConfig(RTC_IT_ALRA,DISABLE);
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
	
	RTC_ClearITPendingBit(RTC_IT_ALRB);//清除中断标志	
	RTC_ITConfig(RTC_IT_ALRB,DISABLE);
	RTC_AlarmCmd(RTC_Alarm_B,DISABLE);

	EXTI_ClearITPendingBit(EXTI_Line17);//清除中断标志
}
