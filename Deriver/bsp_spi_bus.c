/*
*********************************************************************************************************
*
*	模块名称 : SPI总线驱动
*	文件名称 : bsp_spi_bus.h
*	版    本 : V1.1
*	说    明 : SPI总线底层驱动。提供SPI配置、收发数据、多设备共享SPI支持。通过宏切换是硬件SPI还是软件模拟
*	修改记录 :
*		版本号  日期        作者    说明
*       v1.0    2015-05-18 armfly  首版。将串行FLASH、TSC2046、VS1053、AD7705、ADS1256等SPI设备的配置
*									和收发数据的函数进行汇总分类。并解决不同速度的设备间的共享问题。
*		V1.1	2015-05-21 armfly  硬件SPI时，没有开启GPIOB时钟，已解决。
*
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "stm32l1xx.h"
#include "bsp_spi_bus.h"

#if SPI1_BUS_EN == 1
	/* 定义SPI总线的 GPIO端口 */
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
	/* 定义SPI总线的 GPIO端口 */
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
	/* 定义SPI总线的 GPIO端口 */
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
*	函 数 名: bsp_InitSPIBus
*	功能说明: 配置SPI总线。 只包括 SCK、 MOSI、 MISO口线的配置。不包括片选CS，也不包括外设芯片特有的INT、BUSY等
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitSPIBus(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;	
	
#if SPI1_BUS_EN == 1		/* 硬件SPI */
	/* 开启 SPI 时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/* 使能 GPIO 时钟 */
	RCC_APB2PeriphClockCmd(RCC_SCK1 | RCC_MOSI1 | RCC_MISO1, ENABLE);	

	/* 配置 SPI引脚SCK、MISO 和 MOSI为复用推挽模式 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_SCK1;	
	GPIO_Init(PORT_SCK1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_MISO1;	
	GPIO_Init(PORT_MISO1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_MOSI1;	
	GPIO_Init(PORT_MOSI1, &GPIO_InitStructure);

	/* 配置SPI硬件参数 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	/* 数据方向：2线全双工 */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		/* STM32的SPI工作模式 ：主机模式 */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	/* 数据位长度 ： 8位 */
	/* SPI_CPOL和SPI_CPHA结合使用决定时钟和数据采样点的相位关系、
	   本例配置: 总线空闲是高电平,第2个边沿（上升沿采样数据)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;			/* 时钟下降沿采样数据 */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		/* 时钟的第1个边沿采样数据 */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			/* 片选控制方式：软件控制 */

	/* 设置波特率预分频系数 SPI_BaudRatePrescaler_8 ，实测SCK周期 96ns, 10.4MHz */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	/* 数据位传输次序：高位先传 */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			/* CRC多项式寄存器，复位后为7。本例程不用 */
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);				/* 使能SPI  */
#endif

#if SPI2_BUS_EN == 1		/* 硬件SPI */
	/* 开启 SPI 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* 使能 GPIO 时钟 */
	RCC_AHBPeriphClockCmd(RCC_SCK2 | RCC_MOSI2 | RCC_MISO2, ENABLE);	
	
	
	GPIO_PinAFConfig(PORT_SCK2, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(PORT_MISO2, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(PORT_MOSI2, GPIO_PinSource15, GPIO_AF_SPI2);

	/* 配置 SPI引脚SCK、MISO 和 MOSI为复用推挽模式 */
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
		
	/* 配置SPI硬件参数 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	/* 数据方向：2线全双工 */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		/* STM32的SPI工作模式 ：主机模式 */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	/* 数据位长度 ： 8位 */
	/* SPI_CPOL和SPI_CPHA结合使用决定时钟和数据采样点的相位关系、
	   本例配置: 总线空闲是高电平,第2个边沿（上升沿采样数据)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;			/* 时钟上升沿采样数据 */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		/* 时钟的第2个边沿采样数据 */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			/* 片选控制方式：软件控制 */

	/* 设置波特率预分频系数 SPI_BaudRatePrescaler_8 ，实测SCK周期 96ns, 10.4MHz */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	/* 数据位传输次序：高位先传 */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			/* CRC多项式寄存器，复位后为7。本例程不用 */
	SPI_Init(SPI2, &SPI_InitStructure);

	SPI_Cmd(SPI2, ENABLE);				/* 使能SPI  */
#endif

#if SPI3_BUS_EN == 1		/* 硬件SPI */
	/* 开启 SPI 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	/* 使能 GPIO 时钟 */
	RCC_APB2PeriphClockCmd(RCC_SCK3 | RCC_MOSI3 | RCC_MISO3, ENABLE);	
	
	/* 禁用jtag */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 

	/* 使能复用时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO,ENABLE);

	/* 配置 SPI引脚SCK、MISO 和 MOSI为复用推挽模式 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_SCK3;	
	GPIO_Init(PORT_SCK3, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_MISO3;	
	GPIO_Init(PORT_MISO3, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_MOSI3;	
	GPIO_Init(PORT_MOSI3, &GPIO_InitStructure);
		
	/* 配置SPI硬件参数 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	/* 数据方向：2线全双工 */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		/* STM32的SPI工作模式 ：主机模式 */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	/* 数据位长度 ： 8位 */
	/* SPI_CPOL和SPI_CPHA结合使用决定时钟和数据采样点的相位关系、
	   本例配置: 总线空闲是高电平,第2个边沿（上升沿采样数据)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;			/* 时钟上升沿采样数据 */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;		/* 时钟的第2个边沿采样数据 */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			/* 片选控制方式：软件控制 */

	/* 设置波特率预分频系数 SPI_BaudRatePrescaler_8 ，实测SCK周期 96ns, 10.4MHz */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	/* 数据位传输次序：高位先传 */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			/* CRC多项式寄存器，复位后为7。本例程不用 */
	SPI_Init(SPI3, &SPI_InitStructure);

	SPI_Cmd(SPI3, ENABLE);				/* 使能SPI  */
#endif
}


/*
*********************************************************************************************************
*	函 数 名: bsp_spiWrite0
*	功能说明: 向SPI总线发送一个字节。SCK上升沿采集数据, SCK空闲时为低电平。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_spiWrite0(SPI_TypeDef* SPIx,uint8_t _ucByte)
{	
	/* 等待发送缓冲区空 */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	/* 发送一个字节 */
	SPI_I2S_SendData(SPIx, _ucByte);

	/* 等待数据接收完毕 */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

	/* 读取接收到的数据 */
	SPI_I2S_ReceiveData(SPIx);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_spiRead0
*	功能说明: 从SPI总线接收8个bit数据。 SCK上升沿采集数据, SCK空闲时为低电平。
*	形    参: 无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t bsp_spiRead0(SPI_TypeDef* SPIx)
{
	uint8_t read;

	/* 等待发送缓冲区空 */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	/* 发送一个字节 */
	SPI_I2S_SendData(SPIx, 0);

	/* 等待数据接收完毕 */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

	/* 读取接收到的数据 */
	read = SPI_I2S_ReceiveData(SPIx);

	/* 返回读到的数据 */
	return read;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_spiWrite1
*	功能说明: 向SPI总线发送一个字节。  SCK上升沿采集数据, SCK空闲时为高电平
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_spiWrite1(SPI_TypeDef* SPIx,uint8_t _ucByte)
{
	/* 等待发送缓冲区空 */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	/* 发送一个字节 */
	SPI_I2S_SendData(SPIx, _ucByte);

	/* 等待数据接收完毕 */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

	/* 读取接收到的数据 */
	SPI_I2S_ReceiveData(SPIx);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_spiRead1
*	功能说明: 从SPI总线接收8个bit数据。  SCK上升沿采集数据, SCK空闲时为高电平
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t bsp_spiRead1(SPI_TypeDef* SPIx)
{
	uint8_t read;

	/* 等待发送缓冲区空 */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	/* 发送一个字节 */
	SPI_I2S_SendData(SPIx, 0);

	/* 等待数据接收完毕 */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

	/* 读取接收到的数据 */
	read = SPI_I2S_ReceiveData(SPIx);

	/* 返回读到的数据 */
	return read;
}



/********************************************* YMH *****************************************************/
