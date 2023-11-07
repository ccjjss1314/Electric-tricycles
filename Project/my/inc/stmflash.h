#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#include "main.h"

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
 

//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//扇区0起始地址, 2 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08000800) 	//扇区1起始地址, 2 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08001000) 	//扇区2起始地址, 2 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x08001800) 	//扇区3起始地址, 2 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08002000) 	//扇区4起始地址, 2 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08002800) 	//扇区5起始地址, 2 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08003000) 	//扇区6起始地址, 2 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08003800) 	//扇区7起始地址, 2 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((u32)0x08004000) 	//扇区8起始地址, 2 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x08004800) 	//扇区9起始地址, 2 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x08005000) 	//扇区10起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((u32)0x08005800) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_12    ((u32)0x08006000) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_13    ((u32)0x08006800) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_14    ((u32)0x08007000) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_15    ((u32)0x08007800) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_16    ((u32)0x08008000) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_17    ((u32)0x08008800) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_18    ((u32)0x08009000) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_19    ((u32)0x08009800) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_20    ((u32)0x0800A000) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_21    ((u32)0x0800A800) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_22    ((u32)0x0800B000) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_23    ((u32)0x0800B800) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_24    ((u32)0x0800C000) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_25    ((u32)0x0800C800) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_26    ((u32)0x0800D000) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_27    ((u32)0x0800D800) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_28    ((u32)0x0800E000) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_29    ((u32)0x0800E800) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_30    ((u32)0x0800F000) 	//扇区11起始地址,2 Kbytes  
#define ADDR_FLASH_SECTOR_31    ((u32)0x0800F800) 	//扇区11起始地址,2 Kbytes  


/**
* @brief  擦除某一个扇区
* @param  sector_start_addr  要擦除的扇区起始地址
* @param  sector_stop_addr   要擦除的扇区结束地址
* @param  None
* @retval 0:成功
* @example 
* @waing  
**/
int FlashEraseSector(u32 sector_start_addr,u32 sector_stop_addr);

/**
* @brief  指定地址写入指定长度的字(需要事先擦除才可使用此函数)
* @param  WriteAddr  开始写入的地址
* @param  pBuffer    写入的数据
* @param  NumToWrite 写入的数据个数
* @retval 0:成功
* @example 
* @waing  
**/
int FlashWriteWords(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);

/**
* @brief  指定地址读取一个字
* @param  faddr  读取的地址
* @param  none
* @param  none
* @retval 读取的字
* @example 
* @waing  
**/
u32 STMFLASH_ReadWord(u32 faddr);


/**
* @brief  指定地址读取指定长度的字
* @param  ReadAddr:起始地址
* @param  pBuffer:数据指针
* @param  NumToRead:读取的个数
* @retval 读取的字
* @example 
* @waing  
**/
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);   		//从指定地址开始读出指定长度的数据


uint16_t STMFLASH_GetFlashSector(u32 addr);
#endif

















