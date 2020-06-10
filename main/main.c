#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "stm32f10x_it.h"
#include "radio/sx127x_driver.h"
#include "example/example_TX.h"
#include "example/example_RX.h"
#include "example/example_CAD.h"


#define TEST_MOD    1   //����ģʽ 1��ʾ������ʽģʽ��0��ʾ����ʽģʽ
#if !TEST_MOD
    #define IS_TX_MOD 1 //1��ʾ����ģʽ��0��ʾ����ģʽ(������ģʽ��Ч)
#endif

#pragma import(__use_no_semihosting)
//��׼����Ҫ֧�ֵĺ���
struct __FILE
{
    int handle;
};

FILE __stdout;
//����_sys_exit()�Ա��⹤���ڰ�����״̬
void _sys_exit(int x)
{
    x = x;
}
//�ض���fputc����
//�����Ҫ����MCU������ϣ��printf���ĸ����������ȷ�� __WAIT_TODO__
int fputc(int ch, FILE *f)
{
    //ע�⣺USART_FLAG_TXE�Ǽ�鷢�ͻ������Ƿ�Ϊ�գ����Ҫ�ڷ���ǰ��飬������������߷���Ч�ʣ����������ߵ�ʱ����ܵ������һ���ַ���ʧ
    //USART_FLAG_TC�Ǽ�鷢����ɱ�־������ڷ��ͺ��飬����������˯�߶�ʧ�ַ����⣬����Ч�ʵͣ����͹����з��ͻ������Ѿ�Ϊ���ˣ����Խ�����һ�������ˣ�������ΪҪ�ȴ�������ɣ�����Ч�ʵͣ�
    //��Ҫ����һ���ã�һ����Ч�����

    //ѭ���ȴ�ֱ�����ͻ�����Ϊ��(TX Empty)��ʱ���Է������ݵ�������
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
    {}
    USART_SendData(USART2, (uint8_t) ch);

    /* ѭ���ȴ�ֱ�����ͽ���*/
    //while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
    //{}

    return ch;
}

void uartInit()
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    USART_DeInit(USART2);  //��λ����

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);      // RCC_APB1Periph_USART2 ʱ��ʹ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);       // RCC_APB2Periph_GPIOA  ʱ��ʹ��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           // GPIO ��ת�ٶ�Ϊ50MHz

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);                  	// ��ʼ��GPIO

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);                  	// ��ʼ��GPIO

    //USART����
    USART_InitStructure.USART_BaudRate = 115200;                                    //���ô��ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     				//�ֳ�Ϊ8
    USART_InitStructure.USART_StopBits = USART_StopBits_1;              			//1��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                     		//����żУ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 				//�շ�ģʽ
    USART_Init(USART2, &USART_InitStructure);                                       //����USART����
    USART_Cmd(USART2, ENABLE);                                                      //ʹ��USART
}

void delayMsBySystick(uint32_t timeoutMs)
{
    uint32_t systickBak, currTick;
    systickBak = GetTick();
    while (1)
    {
        currTick = GetTick();
        if (currTick >= systickBak)
        {
            if (currTick - systickBak > timeoutMs)
            {
                return;
            }
        }
        else
        {
            //currTick�����
            if (currTick + (~systickBak) > timeoutMs)
            {
                return;
            }
        }
    }
}

/* ��������� */
int main(void)
{
	
	/* -------------------------------------------------------------------- */
    tLoRaSettings setting =
    {
        435000000,      		/* Ƶ������ 435MHZ */
        20,                     /* �������� 20dBm*/
        7,                      /* �������� 125kHz */
        12,                     /* ��Ƶ���� 12 */
        1,                      /* ������� 4/5*/
        0x0005              	/* ǰ���� 0x0005 */
    };


	/* -------------------------------------------------------------------- */

    SysTick_Config(SystemCoreClock / 1000); /* ʱ�ӵδ����ó�1MS */

    uartInit();     /* UART2 INIT */

    printf("aithinker Lora-version:%s\r\n", VERSION);

    g_Radio.Init(&setting); //��ʼ������

    printf("aithinker Lora-init OK\r\n");

    while (1)
    {
#if TEST_MOD
        //����������
        //exampleTx();        	//���Ͳ��ԣ�������ɵȴ�1s���ٴη���
        exampleRx();  			//ѭ������
        //exampleCAD(); 		//CADѭ��������
#else   //else of #if IS_TX_MOD
        //�����շ�����
#if IS_TX_MOD
        if (0 == sx127xSend((uint8_t *)"hello", strlen("hello"), 1000)) //�������ݣ������ʱʱ��1000Ms
        {
            printf("send ok\r\n");
        }
        else
        {
            printf("send timeout\r\n");
        }
        delayMsBySystick(10000);
#else   //else of #if IS_TX_MOD
        rxCount = 255;
        memset(rxBuf, 0, 255);
        printf("start rx\r\n");

        u8_ret = sx127xRx(rxBuf, &rxCount, 1000); //�������ݣ�rxBuf�洢�������ݣ�rxCount�洢���յ������ݳ��ȣ������ʱ1000Ms
        switch (u8_ret)
        {
        case 0:
            printf("rx done\r\n  len:%d\r\n  playload:%s\r\n", rxCount, rxBuf);
            break;
        case 1:
        case 2:
            printf("rx timeout ret:%d\r\n", u8_ret);
            break;
        default:
            printf("unknow ret:%d\r\n", u8_ret);
            break;
        }
#endif  //end of #if IS_TX_MOD
#endif  //end of #if IS_TX_MOD
    }
}
