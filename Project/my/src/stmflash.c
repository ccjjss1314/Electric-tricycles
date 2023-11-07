#include "stmflash.h"
#include <stdio.h>

 
/**
* @brief  ���ݵ�ַ��ȡ���ڵ��������
* @param  addr:flash��ַ
* @param  none
* @param  none
* @retval addr���ڵ��������
* @example 
* @waing  
**/
uint16_t STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
	else if(addr<ADDR_FLASH_SECTOR_12)return FLASH_Sector_11;
	else if(addr<ADDR_FLASH_SECTOR_13)return FLASH_Sector_12;
	else if(addr<ADDR_FLASH_SECTOR_14)return FLASH_Sector_13;
	else if(addr<ADDR_FLASH_SECTOR_15)return FLASH_Sector_14;
	else if(addr<ADDR_FLASH_SECTOR_16)return FLASH_Sector_15;
	else if(addr<ADDR_FLASH_SECTOR_17)return FLASH_Sector_16;
	else if(addr<ADDR_FLASH_SECTOR_18)return FLASH_Sector_17;
	else if(addr<ADDR_FLASH_SECTOR_19)return FLASH_Sector_18;
	else if(addr<ADDR_FLASH_SECTOR_20)return FLASH_Sector_19;
	else if(addr<ADDR_FLASH_SECTOR_21)return FLASH_Sector_20;
	else if(addr<ADDR_FLASH_SECTOR_22)return FLASH_Sector_21;
	else if(addr<ADDR_FLASH_SECTOR_23)return FLASH_Sector_22;
	else if(addr<ADDR_FLASH_SECTOR_24)return FLASH_Sector_23;
	else if(addr<ADDR_FLASH_SECTOR_25)return FLASH_Sector_24;
	else if(addr<ADDR_FLASH_SECTOR_26)return FLASH_Sector_25;
	else if(addr<ADDR_FLASH_SECTOR_27)return FLASH_Sector_26;
	else if(addr<ADDR_FLASH_SECTOR_28)return FLASH_Sector_27;
	else if(addr<ADDR_FLASH_SECTOR_29)return FLASH_Sector_28;
	else if(addr<ADDR_FLASH_SECTOR_30)return FLASH_Sector_29;
	else if(addr<ADDR_FLASH_SECTOR_31)return FLASH_Sector_30;
	return FLASH_Sector_31;	
}

/**
* @brief  ָ����ַ��ȡһ����
* @param  faddr  ��ȡ�ĵ�ַ
* @param  none
* @param  none
* @retval ��ȡ����
* @example 
* @waing  
**/
u32 STMFLASH_ReadWord(u32 faddr)
{
//	printf("RE=%x:%d ",faddr,*(vu32*)faddr);
	return *(vu32*)faddr; 
} 


/**
* @brief  ָ����ַ��ȡָ�����ȵ���
* @param  ReadAddr:��ʼ��ַ
* @param  pBuffer:����ָ��
* @param  NumToRead:��ȡ�ĸ���
* @retval ��ȡ����
* @example 
* @waing  
**/
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//��ȡ4���ֽ�.
		ReadAddr+=4;//ƫ��4���ֽ�.	
	}
}




/**
* @brief  ����һ�����߶������
* @param  sector_start_addr  Ҫ������������ʼ��ַ
* @param  sector_stop_addr   Ҫ����������������ַ
* @param  None
* @retval 0:�ɹ�
* @example 
1.������3������ FlashEraseSector(ADDR_FLASH_SECTOR_3,ADDR_FLASH_SECTOR_3);
2.������3�������͵�4������ FlashEraseSector(ADDR_FLASH_SECTOR_3,ADDR_FLASH_SECTOR_4);
* @waing  
**/
int FlashEraseSector(u32 sector_start_addr,u32 sector_stop_addr)
{
	u32 i=0;
	char FlashStatus=0;
	u32 SectorStart=0;
	u32 SectorStop=0;
	
	/*�����ĵ�ַ��flash��Χ֮��*/
	if(sector_start_addr<ADDR_FLASH_SECTOR_0) return 2;
	if(sector_start_addr>0x0800FFFF) return 3;
	if(sector_stop_addr<ADDR_FLASH_SECTOR_0) return 4;
	if(sector_stop_addr>0x0800FFFF) return 5;
	
	/*��ȡ���*/
	SectorStart = STMFLASH_GetFlashSector(sector_start_addr);
	SectorStop = STMFLASH_GetFlashSector(sector_stop_addr);
	if(SectorStop>=SectorStart)
	{
		if(FLASH_Status_Get() != FLASH_BUSY)//û�����ڽ���������������Բ���Flash
		{
			FLASH_Unlock();
//			FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
			for(i = SectorStart; i <= SectorStop; i++)
			{
				if(FLASH_EOP != FLASH_One_Page_Erase((u32)i*0x800+(u32)0x08000000))
				{
					FlashStatus = i;//������һ����ŵ�flash����ʧ��
					if(FlashStatus==0){FlashStatus=1;}//(���0����1,���ֳ�������ʧ��)
				}
			}
//			FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
			 FLASH_Lock();//����
		}
		else
		{
			FlashStatus = 7;
		}
	}
	else
	{
		FlashStatus = 6;
	}
	return FlashStatus;
}



/**
* @brief  ָ����ַд��ָ�����ȵ���(��Ҫ���Ȳ����ſ�ʹ�ô˺���)
* @param  WriteAddr  ��ʼд��ĵ�ַ
* @param  pBuffer    д�������
* @param  NumToWrite д������ݸ���
* @retval 0:�ɹ�
* @example 
���������Ѿ�������������
д255��u32���ݵ���������
u32 data[255];
STMFLASH_Write(ADDR_FLASH_SECTOR_4,data,255);
* @waing  
**/
int FlashWriteWords(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
{
	char FlashStatus=0;
	u32 EndAddr=0;
	
	/*�����ĵ�ַ��flash��Χ֮��*/
	if(WriteAddr<ADDR_FLASH_SECTOR_0) return 1;
	if(WriteAddr>0x0800FFFF) return 2;
	
//	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)printf("�Ƿ���ַ\r\n");;
	
	EndAddr = WriteAddr+NumToWrite*4;	//д��Ľ�����ַ
	if(EndAddr>0x0800FFFF) return 3;
	
	if(FLASH_Status_Get() != FLASH_BUSY)//���Բ���Flash
	{
		FLASH_Unlock();
		
		while(WriteAddr<EndAddr)//д����
		{
			if(FLASH_EOP != FLASH_Word_Program(WriteAddr,*pBuffer))//д������
			{ 
				FlashStatus = 4;
				break;	//д���쳣
			}
			else
			{
//				printf("OK=%x:%d ",WriteAddr,*pBuffer);
			}
			WriteAddr+=4;
			pBuffer++;
		}
		FLASH_Lock();//����
	}
	else
	{
		return 5;
	}
	return FlashStatus;
}




