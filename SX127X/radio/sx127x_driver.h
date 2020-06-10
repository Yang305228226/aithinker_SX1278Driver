#ifndef __SX127X_DRIVER_H__
#define __SX127X_DRIVER_H__
#include "stdbool.h"
#include "stdint.h"

/*
 * LoRa sx1278/76����
 *
 * \version    1.0.0
 * \date       Dec 17 2019
 * \author     Specter
 */

/* ���ļ���Ҫ��Semtech sx1278/76�������ļ������ڶԽ�Ӳ���� */

#define VERSION "2.0.0"

typedef struct _sLoRaSettings
{
    uint32_t RFFrequency;               // Ƶ��,��λHz
    int8_t Power;                       // ���书��2~20
    uint8_t SignalBw;                   // ���� [0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz, 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]  ����
    uint8_t SpreadingFactor;            // ��Ƶ���� [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips] ��Ƶ����
    uint8_t ErrorCoding;                // ��������� [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]  ������
    uint16_t PreambleLength;            // ǰ���볤��
} tLoRaSettings;						/* LORA���ƽ���������ýṹ�� */

typedef enum
{
    RF_IDLE,    		//����״̬
    RF_BUSY,    		//ģ��ִ��������
    RF_RX_DONE, 		//�������
    RF_RX_TIMEOUT,  	//���ճ�ʱ
    RF_TX_DONE, 		//�������
    RF_TX_TIMEOUT,  	//���ͳ�ʱ
    RF_CAD_DETECTED,    //CAD��⵽ǰ����
    RF_CAD_EMPTY,   	//CAD�����ɣ�û�м�⵽ǰ����
    RF_CAD_TIMEOUT, 	//CAD��ʱ
    RF_UNKNOW_STATUS    //�쳣״̬��
} tRFProcessReturnCodes;				/* ģ��״̬�� ö��    */

typedef enum
{
    LORA_OPMODE_SLEEP = 0,				/* ˯�� */
    LORA_OPMODE_STANDBY,				/* ���� */
    LORA_OPMODE_SYNTHESIZER_TX,			/* Ƶ�ʺϳɷ��� */
    LORA_OPMODE_TRANSMITTER,			/* ���� */
    LORA_OPMODE_SYNTHESIZER_RX,			/* Ƶ�ʺϳɽ��� */
    LORA_OPMODE_RECEIVER,				/* �������� */
    LORA_OPMODE_RECIVER_SINGLE,			/* ���ν��� */
    LORA_OPMODE_CAD,					/* �ŵ����� */
} LoRaOpModeType;						/* Ӳ������ģʽ */

//�����ṹ�壬����������ز����ĺ���
typedef struct sRadioDriver
{
    void (*Init)(tLoRaSettings *stting);
    void (*Reset)(void);
    void (*StartRx)(uint32_t timeoutSystick);
    void (*GetRxPacket)(void *buffer, uint16_t *size);
    void (*SetTxPacket)(const void *buffer, uint16_t size, uint32_t timeoutSystick);
    void (*StartCAD)(void);
    tRFProcessReturnCodes(*Process)(void);
} tRadioDriver;

extern tRadioDriver g_Radio;			/* �ṩ��������� */

void sx127xInit(tLoRaSettings *stting);						/* Ӳ����ʼ�� */
void Sx127xRestart(void);									/* Ӳ�������� */
void SX127xSetLoRaMode(void);								/* ���ó�LORA���ƽ��ģʽ */
void Write127xReg(uint8_t addr, uint8_t data);				/* д�Ĵ��� */
uint8_t Read127xReg(uint8_t addr);							/* ���Ĵ��� */
void SX127xWriteFifo(uint8_t *buffer, uint8_t size);		/* дbuffer */
void SX127xReadFifo(uint8_t *buffer, uint8_t size);			/* ��buffer */
void SX127xSetOpMode(LoRaOpModeType opMode);				/* ���õ��ƽ��ģʽ     FSK/LORA    */
LoRaOpModeType SX127xGetOpMode(void);						/* ��ȡ���Խ��ģʽFSK/LORA */
void SX127xSetFrf(uint32_t fr);								/* ����RF�ز�Ƶ�� */
uint8_t sx127xSend(uint8_t *data, uint8_t len, uint32_t timeoutMs);		/* �������� */
uint8_t sx127xRx(uint8_t *buf, uint8_t *len, uint32_t timeoutMs);		/* �������� */

tRFProcessReturnCodes SX127xProcess(void);

#endif  //end of __SX127X_DRIVER_H__
