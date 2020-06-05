#include "example/example_CAD.h"
#include "radio/sx127x_driver.h"
#include "hal/sx127x_hal.h"
#include "string.h"

 
//��Ҫ��ѯ���ã���Ҫͬʱ�ܶ��demo��������ɻ��ң�
void exampleRx(void){
	tRFProcessReturnCodes processRet=RF_IDLE;	/* ��Ϊ����̬ */
	uint8_t rxBuf[128]={0};
	uint16_t rxCount=128;
	
	processRet=g_Radio.Process();
	
	switch(processRet){
		case RF_IDLE:			//����״̬
			g_Radio.StartRx(3000);
			break;
    case RF_RX_DONE:	//�������
			memset(rxBuf,0,128);
			g_Radio.GetRxPacket(rxBuf,&rxCount);
			DEBUG("[DEBUG %s()-%d]RF_RX_DONE (%d):%s\r\n",__func__,__LINE__,rxCount,rxBuf);
			g_Radio.StartRx(3000);
			break;
    case RF_RX_TIMEOUT:	//���ճ�ʱ
			DEBUG("[DEBUG %s()-%d]RF_RX_TIMEOUT\r\n",__func__,__LINE__);
			g_Radio.StartRx(3000);
			break;
		case RF_BUSY:	//ģ��ִ��������
			break;
		case RF_UNKNOW_STATUS:	//�쳣״̬��
		default:
			DEBUG("[ERROR %s()-%d]unknow status:%d\r\n",__func__,__LINE__,processRet);
			break;
	}
}
