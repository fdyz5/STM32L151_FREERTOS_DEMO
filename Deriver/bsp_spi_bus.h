/*
*********************************************************************************************************
*
*	模块名称 : SPI总线驱动
*	文件名称 : bsp_spi_bus.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2014-2015, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_SPI_BUS_H
#define __BSP_SPI_BUS_H
#include <stdint.h>

#define	SPI1_BUS_EN	0
#define	SPI2_BUS_EN	1
#define	SPI3_BUS_EN	0


/*
	【SPI时钟最快是2分频，不支持不分频】
	如果是SPI1，2分频时SCK时钟 = 42M，4分频时SCK时钟 = 21M
	如果是SPI3, 2分频时SCK时钟 = 21M
*/
#define SPI_SPEED_42M		SPI_BaudRatePrescaler_2
#define SPI_SPEED_21M		SPI_BaudRatePrescaler_4
#define SPI_SPEED_5_2M		SPI_BaudRatePrescaler_8
#define SPI_SPEED_2_6M		SPI_BaudRatePrescaler_16
#define SPI_SPEED_1_3M		SPI_BaudRatePrescaler_32
#define SPI_SPEED_0_6M		SPI_BaudRatePrescaler_64

void bsp_InitSPIBus(void);

void bsp_spiWrite0(SPI_TypeDef* SPIx,uint8_t _ucByte);
uint8_t bsp_spiRead0(SPI_TypeDef* SPIx);
void bsp_spiWrite1(SPI_TypeDef* SPIx,uint8_t _ucByte);
uint8_t bsp_spiRead1(SPI_TypeDef* SPIx);


#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
