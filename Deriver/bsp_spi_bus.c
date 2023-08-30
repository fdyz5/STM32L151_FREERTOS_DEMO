/*
*********************************************************************************************************
*
*	ģ������ : SPI��������
*	�ļ����� : bsp_spi_bus.h
*	��    �� : V1.1
*	˵    �� : SPI���ߵײ��������ṩSPI���á��շ����ݡ����豸����SPI֧�֡�ͨ�����л���Ӳ��SPI�������ģ��
*	�޸ļ�¼ :
*		�汾��  ����        ����    ˵��
*       v1.0    2015-05-18 armfly  �װ档������FLASH��TSC2046��VS1053��AD7705��ADS1256��SPI�豸������
*									���շ����ݵĺ������л��ܷ��ࡣ�������ͬ�ٶȵ��豸��Ĺ������⡣
*		V1.1	2015-05-21 armfly  Ӳ��SPIʱ��û�п���GPIOBʱ�ӣ��ѽ����
*
*	Copyright (C), 2015-2016, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "stm32l1xx.h"
#include "bsp_spi_bus.h"

#if SPI1_BUS_EN == 1
	/* ����SPI���ߵ� GPIO�˿� */
	#define RCC_SCK1 	RCC_APB2Periph_GPIOA
	#define PORT_SCK1	GPIOA
	#define PIN_SCK1	GPIO_Pin_5

	#define RCC_MISO1 	RCC_APB2Periph_GPIOA
	#define PORT_MISO1	GPIOA
	#define PIN_MISO1	GPIO_Pin_6

	#define RCC_MOSI1 	RCC_APB2Periph_GPIOA
	#define PORT_MOSI1	GPIOA
	#define PIN_MOSI1	GPIO_Pin_7
#endif

#if SPI2_BUS_EN == 1
	/* ����SPI���ߵ� GPIO�˿� */
	#define RCC_SCK2 	RCC_AHBPeriph_GPIOB
	#define PORT_SCK2	GPIOB
	#define PIN_SCK2	GPIO_Pin_13

	#define RCC_MISO2 	RCC_AHBPeriph_GPIOB
	#define PORT_MISO2	GPIOB
	#define PIN_MISO2	GPIO_Pin_14

	#define RCC_MOSI2 	RCC_AHBPeriph_GPIOB
	#define PORT_MOSI2	GPIOB
	#define PIN_MOSI2	GPIO_Pin_15
#endif

#if SPI3_BUS_EN == 1
	/* ����SPI���ߵ� GPIO�˿� */
	#define RCC_SCK3 	RCC_APB2Periph_GPIOB
	#define PORT_SCK3	GPIOB
	#define PIN_SCK3	GPIO_Pin_3

	#define RCC_MISO3 	RCC_APB2Periph_GPIOB
	#define PORT_MISO3	GPIOB
	#define PIN_MISO3	GPIO_Pin_4

	#define RCC_MOSI3 	RCC_APB2Periph_GPIOB
	#define PORT_MOSI3	GPIOB
	#define PIN_MOSI3	GPIO_Pin_5
#endif

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitSPIBus
*	����˵��: ����SPI���ߡ� ֻ���� SCK�� MOSI�� MISO���ߵ����á�������ƬѡCS��Ҳ����������оƬ���е�INT��BUSY��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitSPIBus(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;	
	
#if SPI1_BUS_EN == 1		/* Ӳ��SPI */
	/* ���� SPI ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/* ʹ�� GPIO ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_SCK1 | RCC_MOSI1 | RCC_MISO1, ENABLE);	

	/* ���� SPI����SCK��MISO �� MOSIΪ��������ģʽ */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_SCK1;	
	GPIO_Init(PORT_SCK1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_MISO1;	
	GPIO_Init(PORT_MISO1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_MOSI1;	
	GPIO_Init(PORT_MOSI1, &GPIO_InitStructure);

	/* ����SPIӲ������ */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	/* ���ݷ���2��ȫ˫�� */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		/* STM32��SPI����ģʽ ������ģʽ */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	/* ����λ���� �� 8λ */
	/* SPI_CPOL��SPI_CPHA���ʹ�þ���ʱ�Ӻ����ݲ��������λ��ϵ��
	   ��������: ���߿����Ǹߵ�ƽ,��2�����أ������ز�������)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;			/* ʱ���½��ز������� */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		/* ʱ�ӵĵ�1�����ز������� */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			/* Ƭѡ���Ʒ�ʽ��������� */

	/* ���ò�����Ԥ��Ƶϵ�� SPI_BaudRatePrescaler_8 ��ʵ��SCK���� 96ns, 10.4MHz */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	/* ����λ������򣺸�λ�ȴ� */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			/* CRC����ʽ�Ĵ�������λ��Ϊ7�������̲��� */
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);				/* ʹ��SPI  */
#endif

#if SPI2_BUS_EN == 1		/* Ӳ��SPI */
	/* ���� SPI ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* ʹ�� GPIO ʱ�� */
	RCC_AHBPeriphClockCmd(RCC_SCK2 | RCC_MOSI2 | RCC_MISO2, ENABLE);	
	
	
	GPIO_PinAFConfig(PORT_SCK2, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(PORT_MISO2, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(PORT_MOSI2, GPIO_PinSource15, GPIO_AF_SPI2);

	/* ���� SPI����SCK��MISO �� MOSIΪ��������ģʽ */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_SCK2;	
	GPIO_Init(PORT_SCK2, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_MISO2;	
	GPIO_Init(PORT_MISO2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_MOSI2;	
	GPIO_Init(PORT_MOSI2, &GPIO_InitStructure);
		
	/* ����SPIӲ������ */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	/* ���ݷ���2��ȫ˫�� */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		/* STM32��SPI����ģʽ ������ģʽ */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	/* ����λ���� �� 8λ */
	/* SPI_CPOL��SPI_CPHA���ʹ�þ���ʱ�Ӻ����ݲ��������λ��ϵ��
	   ��������: ���߿����Ǹߵ�ƽ,��2�����أ������ز�������)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;			/* ʱ�������ز������� */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		/* ʱ�ӵĵ�2�����ز������� */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			/* Ƭѡ���Ʒ�ʽ��������� */

	/* ���ò�����Ԥ��Ƶϵ�� SPI_BaudRatePrescaler_8 ��ʵ��SCK���� 96ns, 10.4MHz */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	/* ����λ������򣺸�λ�ȴ� */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			/* CRC����ʽ�Ĵ�������λ��Ϊ7�������̲��� */
	SPI_Init(SPI2, &SPI_InitStructure);

	SPI_Cmd(SPI2, ENABLE);				/* ʹ��SPI  */
#endif

#if SPI3_BUS_EN == 1		/* Ӳ��SPI */
	/* ���� SPI ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	/* ʹ�� GPIO ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_SCK3 | RCC_MOSI3 | RCC_MISO3, ENABLE);	
	
	/* ����jtag */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 

	/* ʹ�ܸ���ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO,ENABLE);

	/* ���� SPI����SCK��MISO �� MOSIΪ��������ģʽ */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_SCK3;	
	GPIO_Init(PORT_SCK3, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_MISO3;	
	GPIO_Init(PORT_MISO3, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_MOSI3;	
	GPIO_Init(PORT_MOSI3, &GPIO_InitStructure);
		
	/* ����SPIӲ������ */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	/* ���ݷ���2��ȫ˫�� */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		/* STM32��SPI����ģʽ ������ģʽ */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	/* ����λ���� �� 8λ */
	/* SPI_CPOL��SPI_CPHA���ʹ�þ���ʱ�Ӻ����ݲ��������λ��ϵ��
	   ��������: ���߿����Ǹߵ�ƽ,��2�����أ������ز�������)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;			/* ʱ�������ز������� */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;		/* ʱ�ӵĵ�2�����ز������� */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			/* Ƭѡ���Ʒ�ʽ��������� */

	/* ���ò�����Ԥ��Ƶϵ�� SPI_BaudRatePrescaler_8 ��ʵ��SCK���� 96ns, 10.4MHz */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	/* ����λ������򣺸�λ�ȴ� */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			/* CRC����ʽ�Ĵ�������λ��Ϊ7�������̲��� */
	SPI_Init(SPI3, &SPI_InitStructure);

	SPI_Cmd(SPI3, ENABLE);				/* ʹ��SPI  */
#endif
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_spiWrite0
*	����˵��: ��SPI���߷���һ���ֽڡ�SCK�����زɼ�����, SCK����ʱΪ�͵�ƽ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_spiWrite0(SPI_TypeDef* SPIx,uint8_t _ucByte)
{	
	/* �ȴ����ͻ������� */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	/* ����һ���ֽ� */
	SPI_I2S_SendData(SPIx, _ucByte);

	/* �ȴ����ݽ������ */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

	/* ��ȡ���յ������� */
	SPI_I2S_ReceiveData(SPIx);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_spiRead0
*	����˵��: ��SPI���߽���8��bit���ݡ� SCK�����زɼ�����, SCK����ʱΪ�͵�ƽ��
*	��    ��: ��
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t bsp_spiRead0(SPI_TypeDef* SPIx)
{
	uint8_t read;

	/* �ȴ����ͻ������� */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	/* ����һ���ֽ� */
	SPI_I2S_SendData(SPIx, 0);

	/* �ȴ����ݽ������ */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

	/* ��ȡ���յ������� */
	read = SPI_I2S_ReceiveData(SPIx);

	/* ���ض��������� */
	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_spiWrite1
*	����˵��: ��SPI���߷���һ���ֽڡ�  SCK�����زɼ�����, SCK����ʱΪ�ߵ�ƽ
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_spiWrite1(SPI_TypeDef* SPIx,uint8_t _ucByte)
{
	/* �ȴ����ͻ������� */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	/* ����һ���ֽ� */
	SPI_I2S_SendData(SPIx, _ucByte);

	/* �ȴ����ݽ������ */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

	/* ��ȡ���յ������� */
	SPI_I2S_ReceiveData(SPIx);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_spiRead1
*	����˵��: ��SPI���߽���8��bit���ݡ�  SCK�����زɼ�����, SCK����ʱΪ�ߵ�ƽ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t bsp_spiRead1(SPI_TypeDef* SPIx)
{
	uint8_t read;

	/* �ȴ����ͻ������� */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	/* ����һ���ֽ� */
	SPI_I2S_SendData(SPIx, 0);

	/* �ȴ����ݽ������ */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

	/* ��ȡ���յ������� */
	read = SPI_I2S_ReceiveData(SPIx);

	/* ���ض��������� */
	return read;
}



/********************************************* YMH *****************************************************/
