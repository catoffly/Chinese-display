#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "sdio_sdcard.h" 
#include "ff.h"
#include "exfuns.h" 
#include "text.h"	
#include "usart1.h"
#include "BH1750.h"
#include "ds18b20.h"
u8 xianshi[40];
u8 jjj[20]="��� ������";
int dis_data;
	char xia[10];
		int  m;
int main(void)
{	 
	u8 t=0,i;
  int temp;                                                                                                                                                      
	u16 ret;
	
	delay_init();	    							 //��ʱ������ʼ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE); //ʱ��ѡ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�	
	LCD_Init();				//LCD��ʼ��	 	
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 		//����SD�� 
	Init_BH1750(); 
	GPIOConfig();
	DS18B20_Init();
	USART1_Config();
	Show_Str(150,60,144,16,jjj,16,1);
  	while(1) 
	{			
			for(m=16;m>-((USART_RX-2)/2);m--)
		{	
			Show_Str(150,60,144,16,jjj,16,1);
			Show_Str(m*20,100,144,16,xianshi,16,1);
			Show_Str(90,140,144,16,"�¶ȣ�",16,1);
			Show_Str(150,140,144,16,xia,16,1);
			Single_Write_BH1750(0x01);   // power on
      Single_Write_BH1750(0x10);   // H- resolution mode
			delay_ms(500);
			//LCD_Clear(WHITE);
			mread();       //�����������ݣ��洢��BUF��
			dis_data=BUF[0];
      dis_data=(dis_data<<8)+BUF[1]; //�ϳ����� 
			temp=DS18B20_Get_Temp();
			sprintf(xia,"%4d",temp);
			if(dis_data>200)
			LCD_Clear(WHITE);
			else if(200>dis_data>100)
				LCD_Clear(GRAYBLUE);
			else if(dis_data<100)
				LCD_Clear(DARKBLUE);
		}
			//for(i=0;)
			//USART_SendData(USART1, '1');
			//delay_ms(500);
	}			
 }

