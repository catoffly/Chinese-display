#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H


#include "sys.h"

//上位机传至下位机
#define CMD_ADD_USER                  0X01          //添加人员
#define CMD_DELETE_USER          	  0X02          //删除人员
#define CMD_ONLINE                    0X03          //检查联机
#define CMD_GET_USER_LIST     		  0X04          //获取下位机用户列表
#define CMD_GET_ALL_LIST         	  0X05          //获取下位机全部人员信息
#define CMD_GET_USER_NUM			  0X06			//获取下位机总人数
#define CMD_SET_TIME				  0X07			//设置下位机时间
#define CMD_GTE_I_O					  0X08			//获取人员进出信息
#define CMD_SAVE_PHOTO				  0X09			//存储照片
#define CMD_DELETE_PHOTO			  0X0A			//删除照片
#define CMD_MEMBER			 		  0X0B			//下传人员信息表
#define CMD_CHECK_PHOTO				  0X0C			//查看图片是否存在


typedef struct
{
	char order[4];						//顺序号
	char CardNum[8];					//ID卡号
	char StudentNum[11];				//学号
	char Name[6];						//姓名
	char Jurisdiction[1];				//权限
	char state[1];						//人员进出状态		1门外刷卡，已进入 ； 0门内刷卡已外出		
	char Fingerprint[193];				//指纹
    char Totalnum[4];
}TransferTypeDef;						//227byte


typedef union 
{ 
	TransferTypeDef userinfo;
	char userinfo_arrary[227];
}Union_userinfo;



void Save_access(void);		 //存储个人进出门信息
void Upload_access(void);

#endif

