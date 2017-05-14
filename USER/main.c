#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "sdio_sdcard.h" 
#include "ff.h"
#include "exfuns.h" 
#include "text.h"	
#include "usart1.h"
u8 xianshi[40];
u8 jjj[20]="��� ������";
int main(void)
{	 
	u8 t=0,i;
	
	u16 ret;
	delay_init();	    							 //��ʱ������ʼ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE); //ʱ��ѡ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�	
	LCD_Init();				//LCD��ʼ��	 	
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 		//����SD�� 
	USART1_Config();
	Show_Str(150,60,144,16,jjj,16,1);
  	while(1) 
	{		
			if(state_0x0a==2)
			{
				for(i=0;i<40;i++)
				{
					xianshi[i]=0;
				}
				for(i=1;i<USART_RX-1;i++)
				{
						xianshi[i-1]=USART_RX_buf[i];
				}
					
				Show_Str(100,100,144,16,xianshi,16,0);
				state_0x0a=0;
			}
			//USART_SendData(USART1, '1');
			//delay_ms(500);
	}			
 }

