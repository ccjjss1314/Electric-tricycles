#include "stmflash.h"
#include <stdio.h>

 
/**
* @brief  根据地址获取所在的扇区编号
* @param  addr:flash地址
* @param  none
* @param  none
* @retval addr所在的扇区编号
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
* @brief  指定地址读取一个字
* @param  faddr  读取的地址
* @param  none
* @param  none
* @retval 读取的字
* @example 
* @waing  
**/
u32 STMFLASH_ReadWord(u32 faddr)
{
//	printf("RE=%x:%d ",faddr,*(vu32*)faddr);
	return *(vu32*)faddr; 
} 


/**
* @brief  指定地址读取指定长度的字
* @param  ReadAddr:起始地址
* @param  pBuffer:数据指针
* @param  NumToRead:读取的个数
* @retval 读取的字
* @example 
* @waing  
**/
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
		ReadAddr+=4;//偏移4个字节.	
	}
}




/**
* @brief  擦除一个或者多个扇区
* @param  sector_start_addr  要擦除的扇区起始地址
* @param  sector_stop_addr   要擦除的扇区结束地址
* @param  None
* @retval 0:成功
* @example 
1.擦除第3个扇区 FlashEraseSector(ADDR_FLASH_SECTOR_3,ADDR_FLASH_SECTOR_3);
2.擦除第3个扇区和第4个扇区 FlashEraseSector(ADDR_FLASH_SECTOR_3,ADDR_FLASH_SECTOR_4);
* @waing  
**/
int FlashEraseSector(u32 sector_start_addr,u32 sector_stop_addr)
{
	u32 i=0;
	char FlashStatus=0;
	u32 SectorStart=0;
	u32 SectorStop=0;
	
	/*操作的地址在flash范围之内*/
	if(sector_start_addr<ADDR_FLASH_SECTOR_0) return 2;
	if(sector_start_addr>0x0800FFFF) return 3;
	if(sector_stop_addr<ADDR_FLASH_SECTOR_0) return 4;
	if(sector_stop_addr>0x0800FFFF) return 5;
	
	/*获取编号*/
	SectorStart = STMFLASH_GetFlashSector(sector_start_addr);
	SectorStop = STMFLASH_GetFlashSector(sector_stop_addr);
	if(SectorStop>=SectorStart)
	{
		if(FLASH_Status_Get() != FLASH_BUSY)//没有正在进行闪存操作，可以操作Flash
		{
			FLASH_Unlock();
//			FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
			for(i = SectorStart; i <= SectorStop; i++)
			{
				if(FLASH_EOP != FLASH_One_Page_Erase((u32)i*0x800+(u32)0x08000000))
				{
					FlashStatus = i;//返回哪一个编号的flash擦除失败
					if(FlashStatus==0){FlashStatus=1;}//(编号0返回1,区分出来擦除失败)
				}
			}
//			FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
			 FLASH_Lock();//上锁
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
* @brief  指定地址写入指定长度的字(需要事先擦除才可使用此函数)
* @param  WriteAddr  开始写入的地址
* @param  pBuffer    写入的数据
* @param  NumToWrite 写入的数据个数
* @retval 0:成功
* @example 
假设事先已经擦除第四扇区
写255个u32数据到第四扇区
u32 data[255];
STMFLASH_Write(ADDR_FLASH_SECTOR_4,data,255);
* @waing  
**/
int FlashWriteWords(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
{
	char FlashStatus=0;
	u32 EndAddr=0;
	
	/*操作的地址在flash范围之内*/
	if(WriteAddr<ADDR_FLASH_SECTOR_0) return 1;
	if(WriteAddr>0x0800FFFF) return 2;
	
//	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)printf("非法地址\r\n");;
	
	EndAddr = WriteAddr+NumToWrite*4;	//写入的结束地址
	if(EndAddr>0x0800FFFF) return 3;
	
	if(FLASH_Status_Get() != FLASH_BUSY)//可以操作Flash
	{
		FLASH_Unlock();
		
		while(WriteAddr<EndAddr)//写数据
		{
			if(FLASH_EOP != FLASH_Word_Program(WriteAddr,*pBuffer))//写入数据
			{ 
				FlashStatus = 4;
				break;	//写入异常
			}
			else
			{
//				printf("OK=%x:%d ",WriteAddr,*pBuffer);
			}
			WriteAddr+=4;
			pBuffer++;
		}
		FLASH_Lock();//上锁
	}
	else
	{
		return 5;
	}
	return FlashStatus;
}





