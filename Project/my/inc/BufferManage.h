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
	signed int  SendLen;	//用户可自由使用以上变量
		
	int32_t value;    //内部使用,用户不可使用
	signed int  Len;	//内部使用,用户不可使用
	rb_t Buff;        //管理:缓存数据数组
	rb_t ManageBuff;  //管理:每次缓存数据的数组
}buff_manage_struct;

extern uint8_t    Manage_Data_Buff[Manage_Data_Buff_Len];                                  //缓存数据
extern uint32_t   Manage_num_Buff[Manage_num_Buff_Len];                                    //记录缓存数据的个数
extern uint8_t    Manage_Buff_Copy[Manage_Buff_Copy_Len];                                  //提取缓存的数据

extern buff_manage_struct buff_manage_struct_test;


void BufferManageCreate(buff_manage_struct *bms,void *buff,uint32_t BuffLen,void *ManageBuff,uint32_t ManageBuffLen);
void BufferManageWrite(buff_manage_struct *bms,void *buff,uint32_t BuffLen,int *DataLen);
void BufferManageRead(buff_manage_struct *bms,void *buff,int *DataLen);

#endif

