#include "Instruction.h"
#include "sdio_sdcard.h"   
#include "ff.h"
#include "string.h"			//�ַ�������
#include "rs485.h"
#include "compare.h"
#include "rtc.h" 
//#include "stdlib.h"
#include "my_math.h"


extern LinkInfoTypeDef LinkInfo[MaxUserNum];	//�û���Ϣ����
extern u16 USER_SUM;
extern Union_info Client;					//������Ա��Ϣ�洢������
extern FIL fdsts;      		 //�洢�ļ���Ϣ
u16 SD_NUM_OVER;	//SD����Ա��ϢOVER����λ
extern FIL fdsts_recive;  
extern UINT readnum;

u8 data[50];
u8 sendbuf[100];

UINT bws;        	 // File R/W count
UINT bws_p;
FIL fds_pho;
u16 USER_SUM=0;	
FRESULT res;
DIR dirp;
FILINFO fileinfop;


/*********************************************************************
*��    �ܣ��洢���˽�������Ϣ
*��ڲ�����
*���ڲ�����
*********************************************************************/
void Save_access(void)
{
	u32 IO_num=0;				//�洢�ĳ�����Ϣ����
	u32 IO_SD_SATE=0;			//ָ�򱾴δ洢��λ��
	char IO_num_array[9]={0};	//�洢�ĳ�����Ϣ����
	char time_array[16]={0};	//�洢ʱ��
	char time_temp[6]={0};		//ʱ�仺����
	if(f_open(&fdsts,"SaveIO.txt",FA_READ)==FR_OK)               
	{	 	
		f_lseek(&fdsts,0);                         		 //�ƶ��ļ�ָ��
		f_read(&fdsts,IO_num_array,8,&readnum);			 //��ȡ�Ѵ���Ϣ����
		f_close(&fdsts);	  							
	}
	IO_num=my_atoi(IO_num_array);						 //���ַ���ת��������
	IO_SD_SATE=IO_num*36+8;								 //ָ�򱾴δ洢��λ��
	IO_num++;											 //�洢����+1
	sprintf(IO_num_array,"%08d",IO_num); 				 //������ת�����ַ���	
	sprintf(time_array, "%04d",calendar.w_year);			 //��ʽ����� ʱ��	
	sprintf(time_temp, "%02d",calendar.w_month);
	strcat(time_array,time_temp);						 //�ַ���ƴ��
	sprintf(time_temp, "%02d",calendar.w_date);
	strcat(time_array,time_temp);
	sprintf(time_temp, "%02d",calendar.hour);
	strcat(time_array,time_temp);
	sprintf(time_temp, "%02d",calendar.min);
	strcat(time_array,time_temp);
	sprintf(time_temp, "%02d",calendar.sec);
	strcat(time_array,time_temp);

	if(f_open(&fdsts,"SaveIO.txt",FA_WRITE)==FR_OK)
    {			
        f_lseek(&fdsts,0);                         		 //�ƶ��ļ�ָ��
		f_write(&fdsts,IO_num_array,8,&readnum);		 //�����Ѵ������Ϣ����
		f_lseek(&fdsts,IO_SD_SATE); 
        f_write(&fdsts,Client.Student_Information[0].order,4,&readnum);		 //�洢�û���	
		f_write(&fdsts,Client.Student_Information[0].StudentNum,11,&readnum);//�洢ѧ��
        f_write(&fdsts,Client.Student_Information[0].Name,6,&readnum);		 //�洢�û���
        f_write(&fdsts,Client.Student_Information[0].state,1,&readnum);	   	 //�洢����״̬  
		f_write(&fdsts,time_array,14,&readnum);               				 //�洢ʱ��
        f_close(&fdsts);	  							
    }	
}

/*********************************************************************
*��    �ܣ��ϴ����˽�������Ϣ
*��ڲ�����
*���ڲ�����
*********************************************************************/
void Upload_access(void)
{
	u8 ret=0;
	char IO_num_array[9]={0};		//�洢������Ϣ����
	u16 IO_num=0;					//������Ϣ����	
	u16 i;
	u8 clean_IO_num[9]={"00000000"};		//���ڽ�����������
	char seed_num[9]={0};
	for(i=0;i<30;i++)
	{
		data[i]=0;
	}
	if(f_open(&fdsts,"SaveIO.txt",FA_READ)==FR_OK)
	{	 	
		f_lseek(&fdsts,0);                          	//�ƶ��ļ�ָ��
		f_read(&fdsts,data,8,&readnum);					//��ȡ�洢������Ϣ����
		f_close(&fdsts);	  							
	}
	IO_num=my_atoi(data);								//�Ѵ������Ϣ����

	while(IO_num>0)										//���������Ϣ��������10
	{
		if(f_open(&fdsts,"SaveIO.txt",FA_READ)==FR_OK)
		{	 	
			f_lseek(&fdsts,(IO_num-1)*36+8);         //�ƶ��ļ�ָ��
			f_read(&fdsts,&data[0],36,&readnum);		//��ȡ10��������Ϣ
			f_close(&fdsts);
		}		
		//ret=sendNByte(data,CMD_GTE_I_O,UPPERADDRESS,376,500	);//���ͽ�����Ϣ
	  //void RS485_Send_Data(u8 *buf,u8 len)
		//memcpy(&photo_name[4],jpg,4);
		//0003���û��������2009��12��02��10:01:12ˢ������
		memcpy(sendbuf,data,4);
		memcpy(&sendbuf[4],"���û�",6);
		memcpy(&sendbuf[10],&data[15],6);
		memcpy(&sendbuf[16],"��",2);
		memcpy(&sendbuf[18],&data[22],4);
		memcpy(&sendbuf[22],"��",2);
		memcpy(&sendbuf[24],&data[26],2);
		memcpy(&sendbuf[26],"��",2);
		memcpy(&sendbuf[28],&data[28],2);
		memcpy(&sendbuf[30],"��",2);
		memcpy(&sendbuf[32],&data[30],2);
		memcpy(&sendbuf[34],":",1);
		memcpy(&sendbuf[35],&data[32],2);
		memcpy(&sendbuf[37],":",1);
		memcpy(&sendbuf[38],&data[30],2);
		if(data[21]=='0')
		{
			memcpy(&sendbuf[40],"ˢ������",8);
		}
		else
		{
			memcpy(&sendbuf[40],"ˢ������",8);
		}
		sendbuf[48]='\r';
		sendbuf[49]='\n';
		RS485_Send_Data(sendbuf,50);
		IO_num-=1;										//��Ϣ����-1
		sprintf(IO_num_array, "%08d",IO_num); 			//ת�����ַ���
		if(f_open(&fdsts,"SaveIO.txt",FA_WRITE)==FR_OK)
		{	 	
			f_lseek(&fdsts,0); 
			f_write(&fdsts,IO_num_array,8,&readnum);	//���´洢��Ϣ����			
			f_lseek(&fdsts,IO_num*36+8);                        		
			f_truncate(&fdsts);							//�ض��ļ�
			f_close(&fdsts);			
		}		
	}
}















