/******************** ��ѧ�Ƽ� **************************
 * ʵ��ƽ̨��������STM32������
 * ��汾  ��ST3.5.0
 * ����    ����ѧ�Ƽ��Ŷ� 
 * �Ա�    ��http://shop102218275.taobao.com/
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 *��Ȩ���У�����ؾ���
**********************************************************************************/
#include "usart1.h"
#include <stdarg.h>
#include "string.h"
#include "stdio.h"

#define MAX(x, y) (x) > (y) ? (x) : (y)
u8 data_us=0,USART_RX_buf[50],USART_RX=0,data_transfer=0,data_us1=0,USART_RX_buf1[50],USART_RX1;
char data_cu[100],state_0x0a,data_cu1[100],state_0x0a1;
u8 cun=0,cun1=0;
/*
 * ��������USART1_Config
 * ����  ��USART1 GPIO ����,����ģʽ���á�115200 8-N-1
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_DeInit(USART1);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);    
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ClearFlag(USART1, USART_IT_RXNE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);  
}

/*******************************************************************************
* Function Name  : None.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
					data_transfer=USART_ReceiveData(USART1);	//��ȡ���յ�������
					//USART_SendData(USART2, data_transfer);
					USART_RX_buf[USART_RX]=data_transfer;
					if(data_transfer=='{')
					{
						USART_RX=0;
						state_0x0a=1;
					}
					if(state_0x0a==1)
					{
						USART_RX++;
						if(data_transfer=='}')
						{
							//USART_RX_buf[USART_RX-1]=0x00;
							state_0x0a=2;
						}
					}
	 
     } 
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
} 



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
