/*
*********************************************************************************************************
*
*	ģ������ : SPI��������
*	�ļ����� : bsp_spi_bus.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2014-2015, ���������� www.armfly.com
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
	��SPIʱ�������2��Ƶ����֧�ֲ���Ƶ��
	�����SPI1��2��ƵʱSCKʱ�� = 42M��4��ƵʱSCKʱ�� = 21M
	�����SPI3, 2��ƵʱSCKʱ�� = 21M
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
