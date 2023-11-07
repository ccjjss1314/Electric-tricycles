#include "BufferManage.h"
#include "LoopList.h"
#include <stdio.h>

uint8_t    Manage_Data_Buff[Manage_Data_Buff_Len];//��������
uint32_t   Manage_num_Buff[Manage_num_Buff_Len];//��¼�������ݵĸ���
uint8_t    Manage_Buff_Copy[Manage_Buff_Copy_Len];//��ȡ���������

buff_manage_struct buff_manage_struct_test;//���建���������

/**
* @brief   �������ݻ������
* @param   bms			     ��������ṹ�����
* @param   buff          ���ڻ������ݵ�����
* @param   BuffLen       ���ڻ������ݵ�����ĳ���
* @param   ManageBuff    ���ڼ�¼ÿ�λ�������ֽڵ�����
* @param   ManageBuffLen ���ڼ�¼ÿ�λ�������ֽڵ����鳤��
* @retval  None
* @warning None
* @example 
**/
void BufferManageCreate(buff_manage_struct *bms,void *buff,uint32_t BuffLen,void *ManageBuff,uint32_t ManageBuffLen)
{
	__disable_irq();
	rbCreate(&(bms->Buff),buff,BuffLen);
	rbCreate(&(bms->ManageBuff),ManageBuff,ManageBuffLen);
	
	bms->Count=0;
	bms->Cnt=0;
	bms->ReadFlage=0;
	bms->ReadLen=0;
	bms->SendFlage=0;
	bms->SendLen=0;
	bms->value=0;
	__enable_irq();
}


/**
* @brief   д�뻺������
* @param   bms			��������ṹ�����
* @param   buff     д�������
* @param   BuffLen  д������ݸ���
* @param   DataLen  ����: 0 Success;1:����������;2:���ݻ�����
* @retval  None
* @warning None
* @example 
**/
void BufferManageWrite(buff_manage_struct *bms,void *buff,uint32_t BuffLen,int *DataLen)
{
	__disable_irq();
	if(rbCanWrite(&(bms->Buff))>BuffLen)//����д������
	{
		if(rbCanWrite(&(bms->ManageBuff))>4)//���Լ�¼���ݸ���
		{			
			PutData(&(bms->Buff) ,buff, BuffLen);
			PutData(&(bms->ManageBuff) ,&BuffLen, 4);
			*DataLen = 0;
		}
		else{*DataLen = -1;}
	}
	else {*DataLen = -2;}
	__enable_irq();
}


/**
* @brief   �ӻ����ж�ȡ����
* @param   bms			��������ṹ�����
* @param   buff     ���ص����ݵ�ַ
* @param   DataLen  ��ȡ�����ݸ���
* @retval  ȡ�������ݸ���
* @warning None
* @example 
**/
void BufferManageRead(buff_manage_struct *bms,void *buff,int *DataLen)
{
	__disable_irq();
	*DataLen=0;
	if(rbCanRead(&(bms->ManageBuff))>=4)
	{
		rbRead(&(bms->ManageBuff), &(bms->Len) , 4);//��������������ݸ���
		if(bms->Len>0)
		{
			*DataLen = rbRead(&(bms->Buff),buff, bms->Len); 
		}
	}
	__enable_irq();
}







