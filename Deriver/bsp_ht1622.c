#include "app.h"

#define GPIO_RCC_CS 		RCC_AHBPeriph_GPIOC
#define GPIO_PORT_CS		GPIOC
#define GPIO_PIN_CS			GPIO_Pin_8
#define TM1622_CS_1()	    GPIO_PORT_CS->BSRRL  = GPIO_PIN_CS			
#define TM1622_CS_0()	    GPIO_PORT_CS->BSRRH  = GPIO_PIN_CS	

#define GPIO_RCC_WR 		RCC_AHBPeriph_GPIOC
#define GPIO_PORT_WR		GPIOC
#define GPIO_PIN_WR			GPIO_Pin_1
#define TM1622_WR_1()	    GPIO_PORT_WR->BSRRL   = GPIO_PIN_WR			
#define TM1622_WR_0()	    GPIO_PORT_WR->BSRRH   = GPIO_PIN_WR	

#define GPIO_RCC_DATA 		RCC_AHBPeriph_GPIOC
#define GPIO_PORT_DATA		GPIOC
#define GPIO_PIN_DATA		GPIO_Pin_3
#define TM1622_DATA_1()		GPIO_PORT_DATA->BSRRL = GPIO_PIN_DATA			
#define TM1622_DATA_0()		GPIO_PORT_DATA->BSRRH = GPIO_PIN_DATA	

#define GPIO_RCC_RD 		RCC_AHBPeriph_GPIOC
#define GPIO_PORT_RD		GPIOC
#define GPIO_PIN_RD			GPIO_Pin_0
#define TM1622_RD_1()	    GPIO_PORT_RD->BSRRL   = GPIO_PIN_RD			
#define TM1622_RD_0()	    GPIO_PORT_RD->BSRRH   = GPIO_PIN_RD	


//                           0     1    2    3    4    5    6     7   8    9    -    A    E    r    d   n    _    U    P    L    C    y    t     F
const uint8_t SegHTbl1[] = {0x0a,0x0a,0x0c,0x0e,0x0e,0x06,0x06,0x0a,0x0e,0x0e,0x04,0x0e,0x04,0x04,0x0e,0x06,0x00,0x0a,0x0c,0x00,0x00,0x0e,0x04,0x04};
const uint8_t SegLTbl1[] = {0x0f,0x00,0x0b,0x09,0x04,0x0d,0x0f,0x08,0x0f,0x0d,0x00,0x0e,0x0f,0x06,0x03,0x02,0x01,0x07,0x0e,0x07,0x0F,0x05,0x07,0x0e};

const uint8_t SegHTbl2[] = {0x05,0x05,0x03,0x07,0x07,0x06,0x06,0x05,0x07,0x07,0x02,0x07,0x02,0x02,0x07,0x06,0x00,0x07,0x03,0x00,0x00,0x07,0x02,0x02};
const uint8_t SegLTbl2[] = {0x0f,0x00,0x0d,0x09,0x02,0x0b,0x0f,0x01,0x0f,0x0b,0x00,0x07,0x0f,0x06,0x0C,0x0C,0x08,0x0E,0x07,0x01,0x0F,0x0A,0x0E,0x07};

static void bsp_ht1622_send_bit_low(uint8_t data, uint8_t bits);
static void bsp_ht1622_send_bit_high(uint8_t data, uint8_t bits);


void bsp_ht1622_delay(void)
{
	uint32_t i;
	
	for(i=0; i<50; i++)
	{

	}
}

void bsp_ht1622_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开GPIO的时钟 */
	RCC_APB2PeriphClockCmd(GPIO_RCC_CS, ENABLE);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;		
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_CS;
	GPIO_Init(GPIO_PORT_CS, &GPIO_InitStructure);
	
	/* 打开GPIO的时钟 */
	RCC_APB2PeriphClockCmd(GPIO_RCC_DATA, ENABLE);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_DATA;
	GPIO_Init(GPIO_PORT_DATA, &GPIO_InitStructure);
	
	/* 打开GPIO的时钟 */
	RCC_APB2PeriphClockCmd(GPIO_RCC_WR, ENABLE);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_WR;
	GPIO_Init(GPIO_PORT_WR, &GPIO_InitStructure);
	
	/* 打开GPIO的时钟 */
	RCC_APB2PeriphClockCmd(GPIO_RCC_RD, ENABLE);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_RD;
	GPIO_Init(GPIO_PORT_RD, &GPIO_InitStructure);
    
	TM1622_CS_1();
	TM1622_WR_1();
	TM1622_DATA_1();
	TM1622_RD_1();
	bsp_ht1622_send_command(SYS_EN);
//	bsp_ht1622_send_command(0x24);
	bsp_ht1622_send_command(RCosc);
	bsp_ht1622_send_command(SYS_EN);
	bsp_ht1622_send_command(LCD_ON);
	bsp_ht1622_send_command(NORMAL);

}


static void bsp_ht1622_send_bit_high(uint8_t data, uint8_t bits)
{
	uint8_t i;

	for(i=0; i<bits; i++)
	{
		if(data & 0x80)
		{
			TM1622_DATA_1();
		}
		else
		{
			TM1622_DATA_0();
		}
		
		bsp_ht1622_delay();
		TM1622_WR_0();
		bsp_ht1622_delay();
		TM1622_WR_1();
		bsp_ht1622_delay();
		data <<= 1;
		bsp_ht1622_delay();
	}
}

static void bsp_ht1622_send_bit_low(uint8_t data, uint8_t bits)
{
	uint8_t i;

	for(i=0; i<bits; i++) 
	{ 
		if (data & 0x01) 
		{
			TM1622_DATA_1(); 
		}
		else
		{
			TM1622_DATA_0(); 
		}
		bsp_ht1622_delay();
		TM1622_WR_0(); 
		bsp_ht1622_delay();		
		TM1622_WR_1(); 
		bsp_ht1622_delay();
		data >>= 1;
		bsp_ht1622_delay();
	}
}

void bsp_ht1622_send_command(uint8_t cmd)
{
	TM1622_CS_0();
	bsp_ht1622_send_bit_high(0x80, 3);
	bsp_ht1622_send_bit_high(cmd, 8);
	bsp_ht1622_send_bit_high(0x00, 1);
	TM1622_CS_1();
}

void bsp_ht1622_send_data(uint8_t addr, uint8_t data)
{
	TM1622_CS_0();
	bsp_ht1622_send_bit_high(0xA0, 3);
	bsp_ht1622_send_bit_high(addr << 2, 6);
	bsp_ht1622_send_bit_low(data, 4);
	TM1622_CS_1();
}

void bsp_ht1622_send_datas(uint8_t addr, uint8_t *ptr, uint8_t len)
{
	uint8_t i;
    
	TM1622_CS_0();
	bsp_ht1622_send_bit_high(0xA0, 3);
	bsp_ht1622_send_bit_high(addr << 2, 6);
	for(i=0; i<len; i++)
	{
		bsp_ht1622_send_bit_low(*ptr, 8);
		ptr++;
	}
	TM1622_CS_1();
}

void bsp_ht1622_turn_off(void)
{
	uint8_t i;

	TM1622_CS_0();

	bsp_ht1622_send_bit_high(0xA0, 3);
	bsp_ht1622_send_bit_high(0, 6);
	for(i=0; i<64; i++)
	{
		bsp_ht1622_send_bit_low(0x00, 4);
	}
	TM1622_CS_1();
}

void bsp_ht1622_turn_on(void)
{
	uint8_t u8Count;

	TM1622_CS_0();

	bsp_ht1622_send_bit_high(0xA0, 3);
	bsp_ht1622_send_bit_high(0, 6);
	for(u8Count=0; u8Count<64; u8Count++)
	{
		bsp_ht1622_send_bit_low(0xFF, 4);
	}

	TM1622_CS_1();
}

#define UNIT_P1  		0x00000001
#define UNIT_P2  		0x00000002
#define UNIT_P3  		0x00000004
#define UNIT_P4  		0x00000008
#define UNIT_P5  		0x00000010
#define UNIT_P6  		0x00000020
#define UNIT_P7  		0x00000040
#define UNIT_P8  		0x00000080
#define UNIT_P9  		0x00000100
#define UNIT_P10  		0x00000200
#define UNIT_P11  		0x00000400
#define UNIT_M3_H   	0x00000800
#define UNIT_L_M   		0x00001000
#define UNIT_FLW  		0x00002000
#define UNIT_F  		0x00004000
#define UNIT_R  		0x00008000
#define UNIT_M3  		0x00010000
#define UNIT_Mpa  		0x00020000
#define UNIT_V  		0x00040000
#define UNIT_C  		0x00080000
#define UNIT_RTC    	0x00100000

uint32_t s_ucUnit = 0;
uint8_t  bsp_seg_uint(uint8_t addr, uint32_t uint)
{
	uint8_t i, ch = 0;
	
	for(i=0; i<21; i++)
	{
		switch(uint&((uint32_t)1<<i))
		{	
		case UNIT_P1:
			if(addr == 59) ch |= 0x01;		
			break;
		case UNIT_P2:
			if(addr == 55) ch |= 0x01;		
			break;
		case UNIT_P3:
			if(addr == 51) ch |= 0x01;		
			break;
		case UNIT_P4:
			if(addr == 43) ch |= 0x01;		
			break;
		case UNIT_P5:
			if(addr == 39) ch |= 0x01;		
			break;
		case UNIT_P6:
			if(addr == 35) ch |= 0x01;		
			break;
		case UNIT_P7:
			if(addr == 9)  ch |= 0x08;		
			break;
		case UNIT_P8:
			if(addr == 13) ch |= 0x08;		
			break;
		case UNIT_P9:
			if(addr == 17) ch |= 0x08;		
			break;
		case UNIT_P10:
			if(addr == 21) ch |= 0x08;		
			break;
		case UNIT_P11:
			if(addr == 25) ch |= 0x08;		
			break;
		case UNIT_M3_H: //T1
			if(addr == 31) ch |= 0x01;		
			break;
		case UNIT_L_M: //T2
			if(addr == 47) ch |= 0x08;	
			break;
		case UNIT_FLW: //T3
			if(addr == 32) ch |= 0x02;	
			break;
		case UNIT_F:  //T4
			if(addr == 32) ch |= 0x04;	
			break;	
		case UNIT_R:  //T5
			if(addr == 32) ch |= 0x08;	
			break;
		case UNIT_M3:  //T6
			if(addr == 29) ch |= 0x08;	
			break;	
		case UNIT_Mpa:
			if(addr == 30) ch |= 0x08;		
			break;		
		case UNIT_V:
			if(addr == 30) ch |= 0x04;		
			break;		
		case UNIT_C:
			if(addr == 30) ch |= 0x02;		
			break;
		case UNIT_RTC:
			if((addr == 12)|(addr == 20)|(addr == 58)|(addr == 50)|(addr == 42)) ch |= 0x08;		
			break;
		}	
	}
	
	return ch;
}

int  bsp_seg_char(uint8_t seg, char ch)
{
	uint8_t i, addr1, addr2, ch1, ch2;
	
	if(seg) seg--;
	else return 0;
	
	if((ch>='0')&&(ch<='9')) ch -= 0x30;
	else if(ch == '-') ch = 10;
	else if(ch == 'a') ch = 11;
	else if(ch == 'e') ch = 12;
	else if(ch == 'r') ch = 13;
	else if(ch == 'd') ch = 14;
	else if(ch == 'n') ch = 15;	
	else if(ch == '_') ch = 16;
	else if(ch == 'u') ch = 17;
	else if(ch == 'p') ch = 18;
	else if(ch == 'l') ch = 19;
	else if(ch == 'c') ch = 20;
	else if(ch == 'y') ch = 21;
	else if(ch == 't') ch = 22;
	else if(ch == 'f') ch = 23;	
	else ch = 'z'+1;
			
	if(seg <= 7)
	{
		addr1 = 59 - seg*4;
		addr2 = addr1 + 2; 
		ch1 = SegHTbl1[ch];
		ch2 = SegLTbl1[ch];							
	}
	else if(seg == 8)
	{
		addr1 = 62;
		addr2 = 0;
		ch1 = SegLTbl2[ch];
		ch2 = SegHTbl2[ch];								
	}
	else if(seg <= 14)
	{
		addr1 = 2 + (seg-9)*4;
		addr2 = addr1 + 2; 
		ch1 = SegLTbl2[ch];
		ch2 = SegHTbl2[ch];			
	}	
	else if(seg <= 21)
	{
		addr1 = 60 - (seg-15)*4;
		addr2 = addr1 - 2; 
		ch1 = SegLTbl2[ch];
		ch2 = SegHTbl2[ch];									
	}
	else if(seg <= 22)
	{
		addr1 = 63;
		addr2 = 1;
		ch1 = SegLTbl2[ch];
		ch2 = SegHTbl2[ch];			
	}
	else 
	{
		addr1 = 3 + (seg-23)*4;
		addr2 = addr1 + 2; 
		ch1 = SegLTbl2[ch];
		ch2 = SegHTbl2[ch];		
	}	
	
	if(ch > 'z') ch1 = ch2 = 0;
	
	ch1 |= bsp_seg_uint(addr1, s_ucUnit);
	ch2 |= bsp_seg_uint(addr2, s_ucUnit);	
	
	bsp_ht1622_send_data(addr1, ch1);
	bsp_ht1622_send_data(addr2, ch2);
}

int  bsp_seg_rtc(RTC_T tRTC)
{
	uint8_t seg;
	char buf[32];
	
	sprintf(buf,"%04d%02d%02d%02d%02d%02d", tRTC.Year,tRTC.Mon,tRTC.Day,tRTC.Hour,tRTC.Min,tRTC.Sec);
	s_ucUnit |= UNIT_RTC;
	for(seg=9; seg<=22; seg++)
	{
		bsp_seg_char(seg, buf[seg-9]);
	}
	
	return 1;
}	

int  bsp_seg_flow(void)
{
	uint8_t seg,offset;
	char buf[32],*p;
	union
	{
		uint32_t temp;
		float    f;
	}fTol;	
	
	fTol.temp = g_tDat.flow;
	sprintf(buf,"%f", fTol.f);
	
	if(buf[1] == '.')
	{	
		if((buf[2] == '0')&&(buf[3] == '0')&&(buf[4] == '0')) 
		{
			buf[4]=buf[0]; 
			buf[0]=buf[2]=buf[3]=0;
		}
		else if((buf[3] == '0')&&(buf[4] == '0')) 
		{			
			buf[3]=buf[0]; 
			buf[4]=buf[2];
			buf[0]=buf[2]=0;
			s_ucUnit |= UNIT_P3;
		}
		else if(buf[4] == '0') 
		{			
			buf[4]=buf[3]; 
			buf[3]=buf[2];
			buf[2]=buf[0];
			buf[0]=0;			
			s_ucUnit |= UNIT_P2;
		}
		else
		{
			s_ucUnit |= UNIT_P1;			
		}
		
	}
	else if(buf[2] == '.') 
	{
		if((buf[3] == '0')&&(buf[4] == '0')) 
		{			
			buf[3]=buf[0]; 
			buf[4]=buf[1];
			buf[0]=buf[1]=0;
		}
		else if(buf[4] == '0') 
		{			
			buf[4]=buf[3]; 
			buf[3]=buf[1];
			buf[1]=buf[0];
			buf[0]=0;			
			s_ucUnit |= UNIT_P3;
		}
		else
		{
			s_ucUnit |= UNIT_P2;			
		}		
	}
	else if(buf[3] == '.') 
	{
		if(buf[4] == '0') 
		{			
			buf[4]=buf[2]; 
			buf[2]=buf[1];
			buf[1]=buf[0];
			buf[0]=0;			
		}
		else
		{
			s_ucUnit |= UNIT_P3;			
		}
	}		
		
	s_ucUnit |= UNIT_M3_H;
	
	p = buf;
	for(seg=1; seg<=4; seg++,p++)
	{
		if(*p == '.') p++;
		bsp_seg_char(seg, *p);
	}
}

int  bsp_seg_Accumulation(char ch)
{
	uint8_t seg,len,offset;
	char *p,buf[32] = {0};
	char buffer[32] = {0}; 
	union
	{
		uint32_t temp;
		float    f;
	}fTol;	
	
	if(ch == 'F')
	{
		fTol.temp = g_tDat.flow_total_decimal;
		fTol.f += g_tDat.flow_total;	
	}
	else
	{
		fTol.temp = g_tDat.flow_reverse_decimal;
		fTol.f += g_tDat.flow_reverse;			
	}

	sprintf(buf,"%.3f", fTol.f);
	
	len = strlen(buf);
	if(len < 9)
	{
		offset = 9 - len;
		memcpy(buffer+offset, buf, len);
	}
	else
	{
		offset = len - 9;		
		memcpy(buffer, buf+offset, 9);		
	}
	
	s_ucUnit |= UNIT_P9;		
	s_ucUnit |= UNIT_M3;
	
	p = buffer;
	for(seg=23; seg<=30; seg++,p++)
	{
		if(*p == '.') p++;
		bsp_seg_char(seg, *p);
	}
	
	if(ch == 'F')
	{
		bsp_ht1622_send_data(32, 0x0A);	
	}
	else
	{
		bsp_ht1622_send_data(32, 0x06);			
	}

	return 1;
}

int  bsp_seg_string(char *str)
{
	uint8_t seg;
	char *p,buf[32] = {0};

	sprintf(buf,"%s",str);	
	
	s_ucUnit &= ~UNIT_P9;		
	s_ucUnit &= ~UNIT_M3;	
	
	p = buf;
	for(seg=23; seg<=30; seg++,p++)
	{
		bsp_seg_char(seg, *p);
	}
	
	return 1;
}

int  bsp_seg_bat_vol(void)
{
	uint8_t seg;
	float    f;
	char buf[32],*p;
	union
	{
		uint32_t temp;
		float    f;
	}fTol;	
	
	fTol.f = g_tDat.battary/1000.0;
	
	sprintf(buf,"%1.3f", fTol.f);
	s_ucUnit |= UNIT_P4;	
	s_ucUnit |= UNIT_V;
	
	p = buf;
	for(seg=5; seg<=8; seg++,p++)
	{
		if(*p == '.') p++;
		bsp_seg_char(seg, *p);
	}
	bsp_ht1622_send_data(30, 0x04);	
	
	return 1;
}


uint8_t g_ucLcdSta = 0;
uint8_t s_ucLcdIdx = 0;
uint8_t s_ucLcdPage = 0;
uint8_t g_ucKey1Sta = 0;
uint8_t g_ucKey2Sta = 0;
uint8_t g_ucKey3Sta = 0;
int lcd_process(void)
{
	switch(g_ucLcdSta)
	{
	case 0:
		if(GPIOB->IDR & (1<<7)) //K1
		{
			g_ucLcdSta++;	
			g_ucKey1Sta = 0;			
		}
		break;

	case 1:
		if((GPIOB->IDR &(1<<7)) == 0) //K1
		{
			g_ucLcdSta++;
			g_ucKey1Sta = 0;				
		}
		break;

	case 2:		
		bsp_timer_star_once(TMR_LCD, TM_100MS);
		g_ucAqtTrig = 1;	
		s_ucLcdIdx = 0;	
		g_ucLcdSta++;	
		break;
	
	case 3:			
		if(bsp_timer_check(TMR_LCD)) //超时处理
		{										
			g_ucLcdSta++;				
		}	
		break;	
			
	case 4:			
		bsp_ht1622_init();
		bsp_ht1622_turn_off();	
		s_ucLcdPage = 0;
		g_ucLcdSta++;
		break;	
	
	case 5:						
		bsp_timer_star_auto(TMR_LCD, TM_10MS);			
		g_ucLcdSta++;					
		break;

	case 6:
		if(bsp_timer_check(TMR_LCD)) //超时处理
		{			
			s_ucLcdIdx++;
			if(s_ucLcdIdx >= 5) 
			{
				s_ucLcdIdx = 0;									
			}
			g_ucLcdSta++;				
		}					
		break;
		
	case 7:
		if(s_ucLcdIdx == 0)
		{
			bsp_seg_flow();
			bsp_seg_bat_vol();		
		}
		else if(s_ucLcdIdx == 1)
		{
			bsp_rtc_get_time();			
			bsp_seg_rtc(g_tRTC);			
		}
		else if(s_ucLcdIdx == 2)
		{
			if(aqt_idle())
			{			
				if(s_ucLcdPage == 0) bsp_seg_Accumulation('F');
				else if(s_ucLcdPage == 1) bsp_seg_Accumulation('R');
			}
			else
			{
				s_ucLcdPage = 0;				
				bsp_seg_string("read____");					
			}
						
		}		
		
		g_ucLcdSta--;		
		break;
		
	default:
		break;
	
	}

	switch(g_ucKey1Sta)
	{
	case 0:
		if(GPIOB->IDR & (1<<7)) //K1
		{
			g_ucKey1Sta++;			
		}
		break;

	case 1:
		if((GPIOB->IDR &(1<<7)) == 0) //K1
		{
			s_ucLcdPage++;
			if(s_ucLcdPage >= 2) s_ucLcdPage = 0;
			g_ucKey1Sta = 0;			
		}
		break;
		
	default:
		g_ucKey1Sta = 0;		
		break;	
	}
	
	switch(g_ucKey2Sta)
	{
	case 0:
		if(GPIOB->IDR & (1<<6)) //K2
		{
			g_ucKey2Sta++;			
		}
		break;

	case 1:
		if((GPIOB->IDR &(1<<6)) == 0) //K2
		{
			g_usErr |= 0x0010;
			g_ucAqtTrig = 1;
			g_ucKey2Sta = 0;			
		}
		break;
		
	default:
		g_ucKey2Sta = 0;		
		break;	
	}
	
	switch(g_ucKey3Sta)
	{
	case 0:
		if(GPIOC->IDR & (1<<13)) //K3
		{
			g_ucKey3Sta++;			
		}
		break;

	case 1:
		if((GPIOC->IDR &(1<<13)) == 0) //K3
		{

			g_ucRptTrig = 1;
			g_ucKey3Sta = 0;			
		}
		break;
		
	default:
		g_ucKey3Sta = 0;		
		break;	
	}	
	
	return 1;
}

