#ifndef BUFFMANAGE_H_
#define BUFFMANAGE_H_

#include "n32g430.h"
#include "LoopList.h"


#define Manage_Data_Buff_Len 21
#define Manage_num_Buff_Len 21
#define Manage_Buff_Copy_Len 21

typedef struct{
	signed int  Count;
	signed int  Cnt;
	unsigned char ReadFlage;
	unsigned char SendFlage;
	
	signed int  ReadLen;
	signed int  SendLen;	//�û�������ʹ�����ϱ���
		
	int32_t value;    //�ڲ�ʹ��,�û�����ʹ��
	signed int  Len;	//�ڲ�ʹ��,�û�����ʹ��
	rb_t Buff;        //����:������������
	rb_t ManageBuff;  //����:ÿ�λ������ݵ�����
}buff_manage_struct;

extern uint8_t    Manage_Data_Buff[Manage_Data_Buff_Len];                                  //��������
extern uint32_t   Manage_num_Buff[Manage_num_Buff_Len];                                    //��¼�������ݵĸ���
extern uint8_t    Manage_Buff_Copy[Manage_Buff_Copy_Len];                                  //��ȡ���������

extern buff_manage_struct buff_manage_struct_test;


void BufferManageCreate(buff_manage_struct *bms,void *buff,uint32_t BuffLen,void *ManageBuff,uint32_t ManageBuffLen);
void BufferManageWrite(buff_manage_struct *bms,void *buff,uint32_t BuffLen,int *DataLen);
void BufferManageRead(buff_manage_struct *bms,void *buff,int *DataLen);

#endif

