#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H


#include "sys.h"

//��λ��������λ��
#define CMD_ADD_USER                  0X01          //�����Ա
#define CMD_DELETE_USER          	  0X02          //ɾ����Ա
#define CMD_ONLINE                    0X03          //�������
#define CMD_GET_USER_LIST     		  0X04          //��ȡ��λ���û��б�
#define CMD_GET_ALL_LIST         	  0X05          //��ȡ��λ��ȫ����Ա��Ϣ
#define CMD_GET_USER_NUM			  0X06			//��ȡ��λ��������
#define CMD_SET_TIME				  0X07			//������λ��ʱ��
#define CMD_GTE_I_O					  0X08			//��ȡ��Ա������Ϣ
#define CMD_SAVE_PHOTO				  0X09			//�洢��Ƭ
#define CMD_DELETE_PHOTO			  0X0A			//ɾ����Ƭ
#define CMD_MEMBER			 		  0X0B			//�´���Ա��Ϣ��
#define CMD_CHECK_PHOTO				  0X0C			//�鿴ͼƬ�Ƿ����


typedef struct
{
	char order[4];						//˳���
	char CardNum[8];					//ID����
	char StudentNum[11];				//ѧ��
	char Name[6];						//����
	char Jurisdiction[1];				//Ȩ��
	char state[1];						//��Ա����״̬		1����ˢ�����ѽ��� �� 0����ˢ�������		
	char Fingerprint[193];				//ָ��
    char Totalnum[4];
}TransferTypeDef;						//227byte


typedef union 
{ 
	TransferTypeDef userinfo;
	char userinfo_arrary[227];
}Union_userinfo;



void Save_access(void);		 //�洢���˽�������Ϣ
void Upload_access(void);

#endif

