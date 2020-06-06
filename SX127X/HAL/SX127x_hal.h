#ifndef __SX127X_HAL_H__
#define __SX127X_HAL_H__
/*
 * LoRa sx1278/76����
 *
 * \version    1.0.0
 * \date       Dec 17 2019
 * \author     Specter
 */

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stdio.h"

#define DEBUG( format , ... )   printf( format , ##__VA_ARGS__ )
#define GET_TICK_COUNT() GetTick()  	/*��ȡ��ǰ�δ��ʱ����ֵ*/

void Soft_delay_ms(u16 time);        	/* ����ӳ��� */
void SX1276HALInit(void);               /* Ӳ����ʼ�� */
uint8_t SpiInOut(uint8_t outData);   	/* SPI����������� */
void SpiNSSEnable(uint8_t status);  	/* SPI NSS Ƭѡʹ�� */
void SX127X_ResetPinControl(uint8_t status);     /* ��λ���� */
uint8_t SX1276ReadDio0(void);           /* ��ȡDIO0��ƽ */
uint8_t SX1276ReadDio1(void);           /* ��ȡDIO1��ƽ */
uint8_t SX1276ReadDio2(void);           /* ��ȡDIO2��ƽ */
uint8_t SX1276ReadDio3(void);           /* ��ȡDIO3��ƽ */
uint8_t SX1276ReadDio4(void);           /* ��ȡDIO4��ƽ */
uint8_t SX1276ReadDio5(void);           /* ��ȡDIO5��ƽ */

#endif //end of __SX127X_HAL_H__
