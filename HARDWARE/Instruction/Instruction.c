#include "Instruction.h"
#include "sdio_sdcard.h"   
#include "ff.h"
#include "string.h"			//字符串操作
#include "rs485.h"
#include "compare.h"
#include "rtc.h" 
//#include "stdlib.h"
#include "my_math.h"


extern LinkInfoTypeDef LinkInfo[MaxUserNum];	//用户信息链表
extern u16 USER_SUM;
extern Union_info Client;					//单个人员信息存储联合体
extern FIL fdsts;      		 //存储文件信息
u16 SD_NUM_OVER;	//SD卡人员信息OVER结束位
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
*功    能：存储个人进出门信息
*入口参数：
*出口参数：
*********************************************************************/
void Save_access(void)
{
	u32 IO_num=0;				//存储的出入信息条数
	u32 IO_SD_SATE=0;			//指向本次存储的位置
	char IO_num_array[9]={0};	//存储的出入信息条数
	char time_array[16]={0};	//存储时间
	char time_temp[6]={0};		//时间缓冲区
	if(f_open(&fdsts,"SaveIO.txt",FA_READ)==FR_OK)               
	{	 	
		f_lseek(&fdsts,0);                         		 //移动文件指针
		f_read(&fdsts,IO_num_array,8,&readnum);			 //读取已存信息条数
		f_close(&fdsts);	  							
	}
	IO_num=my_atoi(IO_num_array);						 //将字符串转换成整数
	IO_SD_SATE=IO_num*36+8;								 //指向本次存储的位置
	IO_num++;											 //存储条数+1
	sprintf(IO_num_array,"%08d",IO_num); 				 //将整数转换成字符串	
	sprintf(time_array, "%04d",calendar.w_year);			 //格式化输出 时间	
	sprintf(time_temp, "%02d",calendar.w_month);
	strcat(time_array,time_temp);						 //字符串拼接
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
        f_lseek(&fdsts,0);                         		 //移动文件指针
		f_write(&fdsts,IO_num_array,8,&readnum);		 //更新已存出入信息条数
		f_lseek(&fdsts,IO_SD_SATE); 
        f_write(&fdsts,Client.Student_Information[0].order,4,&readnum);		 //存储用户号	
		f_write(&fdsts,Client.Student_Information[0].StudentNum,11,&readnum);//存储学号
        f_write(&fdsts,Client.Student_Information[0].Name,6,&readnum);		 //存储用户名
        f_write(&fdsts,Client.Student_Information[0].state,1,&readnum);	   	 //存储进出状态  
		f_write(&fdsts,time_array,14,&readnum);               				 //存储时间
        f_close(&fdsts);	  							
    }	
}

/*********************************************************************
*功    能：上传个人进出门信息
*入口参数：
*出口参数：
*********************************************************************/
void Upload_access(void)
{
	u8 ret=0;
	char IO_num_array[9]={0};		//存储进出信息条数
	u16 IO_num=0;					//进出信息条数	
	u16 i;
	u8 clean_IO_num[9]={"00000000"};		//用于进出条数归零
	char seed_num[9]={0};
	for(i=0;i<30;i++)
	{
		data[i]=0;
	}
	if(f_open(&fdsts,"SaveIO.txt",FA_READ)==FR_OK)
	{	 	
		f_lseek(&fdsts,0);                          	//移动文件指针
		f_read(&fdsts,data,8,&readnum);					//读取存储进出信息条数
		f_close(&fdsts);	  							
	}
	IO_num=my_atoi(data);								//已存进出信息条数

	while(IO_num>0)										//如果进出信息条数大于10
	{
		if(f_open(&fdsts,"SaveIO.txt",FA_READ)==FR_OK)
		{	 	
			f_lseek(&fdsts,(IO_num-1)*36+8);         //移动文件指针
			f_read(&fdsts,&data[0],36,&readnum);		//读取10条进出信息
			f_close(&fdsts);
		}		
		//ret=sendNByte(data,CMD_GTE_I_O,UPPERADDRESS,376,500	);//发送进出信息
	  //void RS485_Send_Data(u8 *buf,u8 len)
		//memcpy(&photo_name[4],jpg,4);
		//0003号用户焦朋辉在2009年12月02日10:01:12刷卡进门
		memcpy(sendbuf,data,4);
		memcpy(&sendbuf[4],"号用户",6);
		memcpy(&sendbuf[10],&data[15],6);
		memcpy(&sendbuf[16],"在",2);
		memcpy(&sendbuf[18],&data[22],4);
		memcpy(&sendbuf[22],"年",2);
		memcpy(&sendbuf[24],&data[26],2);
		memcpy(&sendbuf[26],"月",2);
		memcpy(&sendbuf[28],&data[28],2);
		memcpy(&sendbuf[30],"日",2);
		memcpy(&sendbuf[32],&data[30],2);
		memcpy(&sendbuf[34],":",1);
		memcpy(&sendbuf[35],&data[32],2);
		memcpy(&sendbuf[37],":",1);
		memcpy(&sendbuf[38],&data[30],2);
		if(data[21]=='0')
		{
			memcpy(&sendbuf[40],"刷卡进门",8);
		}
		else
		{
			memcpy(&sendbuf[40],"刷卡出门",8);
		}
		sendbuf[48]='\r';
		sendbuf[49]='\n';
		RS485_Send_Data(sendbuf,50);
		IO_num-=1;										//信息条数-1
		sprintf(IO_num_array, "%08d",IO_num); 			//转换成字符串
		if(f_open(&fdsts,"SaveIO.txt",FA_WRITE)==FR_OK)
		{	 	
			f_lseek(&fdsts,0); 
			f_write(&fdsts,IO_num_array,8,&readnum);	//更新存储信息条数			
			f_lseek(&fdsts,IO_num*36+8);                        		
			f_truncate(&fdsts);							//截断文件
			f_close(&fdsts);			
		}		
	}
}















