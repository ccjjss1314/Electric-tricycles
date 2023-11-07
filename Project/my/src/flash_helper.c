#define FLASH_HELPER_C_

#include "flash_helper.h"
#include "stmflash.h"


flash_helper_typedef flash_helper_struct;

/**Flash操作函数,根据自己的函数修改函数体**/

//指定地址读取一定个数的u32型数据
void FlashHelperReadDataEx(u32 addr,u32 *data, u32 datalen)
{
	STMFLASH_Read(addr,data,datalen);
}
//指定地址写入一定个数的u32型数据
int FlashHelperWriteWordsEx(u32 addr,u32 *pBuffer, u32 NumToWrite)
{
	 return FlashWriteWords(addr,pBuffer,NumToWrite);
}
//擦除某一个扇区
char FlashHelperEraseSectorEx(u32 sector_start_addr,u32 sector_stop_addr)
{
	return FlashEraseSector(sector_start_addr,sector_stop_addr);
}


/**
* @brief  flash_helper_init
* @param  sectors_addr  使用哪一个扇区保存数据
* @param  None
* @param  None
* @retval 0:成功
* @example 
* @waing   
**/
int flash_helper_init(u32 sectors_addr)
{
	u32 i=0;
	
	flash_helper_struct.FlashHelperDataSize = FlashHelperDataLen*4;          //数组可以保存的字节数
	/*扇区的开始地址*/
	flash_helper_struct.FlashHelperStartAddr=0;
	/*扇区的结束地址*/
	flash_helper_struct.FlashHelperEndAddr=0;
	/*记录当前操作的地址*/	
	flash_helper_struct.FlashHelperSaveAddr=0;
	
		//缓存的数组超过了此扇区,2KB
		if(FlashHelperDataLen>2*1024/4) {return 1;}
		//扇区结束地址
		flash_helper_struct.FlashHelperEndAddr = sectors_addr+2*1024;
		
		#if FlashHelperDebug
		printf("\r\nsectors_addr_start: 0x%x\r\n",sectors_addr);
		printf("\r\nsectors_addr_stop: 0x%x\r\n",flash_helper_struct.FlashHelperEndAddr);
		#endif
	
  if(sectors_addr < 0x08000000 || sectors_addr > 0x0800FFFF)
	{
		#if FlashHelperDebug
		printf("\r\nsectors_addr_err=4\r\n");
		#endif
		return 4;//不是使用的某个扇区的首地址
	}
	
	//扇区开始地址
	flash_helper_struct.FlashHelperStartAddr = sectors_addr;
	
	
	//找到当前缓存到了哪个位置
	for(i=flash_helper_struct.FlashHelperStartAddr;i<=flash_helper_struct.FlashHelperEndAddr-flash_helper_struct.FlashHelperDataSize;i+=flash_helper_struct.FlashHelperDataSize)
	{
		//获取缓存的数据
		FlashHelperReadDataEx(i,flash_helper_struct.FlashHelperData,FlashHelperDataLen);
		
		//有标识
//		if(flash_helper_struct.FlashHelperData[FlashHelperDataLen-1] == FlashHelperUUID)
	  if(flash_helper_struct.FlashHelperData[FlashHelperDataLen-1] != 0xffffffff )
		{
			//当前数据保存的地址
			flash_helper_struct.FlashHelperSaveAddr=i;
			
			#if FlashHelperDebug
			printf("\r\ndata:0x55555555 addr: 0x0%x\r\n",flash_helper_struct.FlashHelperSaveAddr);
			#endif
		}
		else if(flash_helper_struct.FlashHelperData[FlashHelperDataLen-1] == 0xffffffff)
		{
			//没有缓存数据
			if(i == flash_helper_struct.FlashHelperStartAddr)
			{
				//当前数据保存的地址
				flash_helper_struct.FlashHelperSaveAddr =  flash_helper_struct.FlashHelperStartAddr;
//				flash_helper_struct.FlashHelperSaveAddr=i;
				//设置结尾标识符       //第一个字写0
//				 flash_data_set_count(111);
				memset(flash_helper_struct.FlashHelperData,0,sizeof(flash_helper_struct.FlashHelperData));
//				flash_helper_struct.FlashHelperData[FlashHelperDataLen-1] = FlashHelperUUID;
				FlashHelperWriteWordsEx(flash_helper_struct.FlashHelperSaveAddr,flash_helper_struct.FlashHelperData,FlashHelperDataLen);
			}
			else
			{
				//读出上一次存储的数据到数组
				FlashHelperReadDataEx(i-flash_helper_struct.FlashHelperDataSize,flash_helper_struct.FlashHelperData,FlashHelperDataLen);
			}
			
			#if FlashHelperDebug
			printf("\r\ndata:0xFFFFFFFF addr: 0x0%x\r\n",flash_helper_struct.FlashHelperSaveAddr);
			#endif
			break;
		}
		else
		{
			//擦除这个扇区
			FlashHelperEraseSectorEx(sectors_addr,sectors_addr);
			//当前数据保存的地址
			flash_helper_struct.FlashHelperSaveAddr =  flash_helper_struct.FlashHelperStartAddr;
			
			#if FlashHelperDebug
			printf("\r\ndata:err addr: 0x0%x\r\n",flash_helper_struct.FlashHelperSaveAddr);
			#endif
			break;
		}
	}
	return 0;
}


/**
* @brief  更改完数组里面的值,调用此函数把数组数据写入flash
* @param  None
* @param  None
* @param  None
* @retval None
* @example 
* @waing  
**/
void flash_helper_update(void)
{
	//先前保存数据的首地址 + 存数据的个数 = 这次保存数据的首地址
		flash_helper_struct.FlashHelperSaveAddr = flash_helper_struct.FlashHelperSaveAddr + flash_helper_struct.FlashHelperDataSize;
	
	#if FlashHelperDebug
	printf("SaveAddr:%x\r\n",flash_helper_struct.FlashHelperSaveAddr);
	#endif
	
	//这次保存数据的尾地址大于了该扇区尾地址
	//超出范围,清除整个扇区,重新开始存储
	if(flash_helper_struct.FlashHelperSaveAddr + flash_helper_struct.FlashHelperDataSize -1 > flash_helper_struct.FlashHelperEndAddr)
	{
		#if FlashHelperDebug
		printf("EraseSector\r\n");
		#endif
		//擦除这个扇区
		FlashHelperEraseSectorEx(flash_helper_struct.FlashHelperStartAddr,flash_helper_struct.FlashHelperStartAddr);
		//设置当前保存的地址为扇区首地址
		flash_helper_struct.FlashHelperSaveAddr =  flash_helper_struct.FlashHelperStartAddr;
	}
	//再次设置一下结尾标识符,防止用户操作不当戳改.
//	flash_helper_struct.FlashHelperData[FlashHelperDataLen-1] = FlashHelperUUID;
	FlashHelperWriteWordsEx(flash_helper_struct.FlashHelperSaveAddr,flash_helper_struct.FlashHelperData,FlashHelperDataLen);
}


/**
* @brief   获取缓存的数据
* @param   index 和数组的下标对应
* @param   data  返回的数据
* @param   datalen 获取的个数
* @retval  0:成功
* @warning 
* @example 
**/
int flash_helper_get(u32 index,u32 *data, u32 datalen)
{
	//最大获取的个数不能超出实际最大能返回的个数
	if(datalen > FlashHelperDataLen - index) datalen = FlashHelperDataLen - index;
	FlashHelperReadDataEx(flash_helper_struct.FlashHelperSaveAddr+(index<<2),data,datalen);
	return 0;
}



/*******************************************************flash_data*****************************************************/
//设置计数值
void flash_data_set_count(u32 data)
{
	flash_helper_struct.FlashHelperData[0] = data;
}

//获取设置的计数值
u32 flash_data_get_count(char mode)
{
	u32 value;
	if(mode){//从falsh里面获取
		flash_helper_get(0,&value,1);
	}
	else{
		value = flash_helper_struct.FlashHelperData[0];
	}
	return value;
}





