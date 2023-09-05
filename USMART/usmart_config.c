#include "usmart.h"
#include "usmart_str.h"
////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����) 
#include "delay.h"	
#include "usart.h"		
#include "bsp_cpu_adc.h"	
#include "bsp_spi_flash.h"
												 
extern void bsp_led_toggle(uint8_t n);
extern void buzzer_beep(uint16_t milliseconds);
extern void ee_Test(u8 inputnub);
//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif		   
	(void*)delay_ms,"void delay_ms(uint32_t nms)",
 	(void*)delay_us,"void delay_us(uint32_t nus)",	 
	(void*)bsp_led_toggle,"bsp_led_toggle(uint8_t n)",
	(uint16_t*)bsp_cpu_adc_get_channel1,"uint16_t bsp_cpu_adc_get_channel1(uint16_t vref)",
	(uint16_t*)bsp_cpu_adc_get_channel2,"uint16_t bsp_cpu_adc_get_channel2(uint16_t vref)",
	(uint16_t*)bsp_cpu_adc_get_battery,"uint16_t bsp_cpu_adc_get_battery(void)  ",	
	(void*)ee_Test,"void ee_Test(uint16_t _usAddress,u8 inputnumb)",
	(uint32_t*)sf_ReadID,"uint32_t sf_ReadID(void)",	
	
	(void*)buzzer_beep,"void buzzer_beep(uint16_t milliseconds)",
};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};   



















