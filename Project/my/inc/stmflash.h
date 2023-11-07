#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#include "main.h"

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
 

//FLASH ��������ʼ��ַ
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//����0��ʼ��ַ, 2 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08000800) 	//����1��ʼ��ַ, 2 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08001000) 	//����2��ʼ��ַ, 2 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x08001800) 	//����3��ʼ��ַ, 2 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08002000) 	//����4��ʼ��ַ, 2 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08002800) 	//����5��ʼ��ַ, 2 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08003000) 	//����6��ʼ��ַ, 2 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08003800) 	//����7��ʼ��ַ, 2 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((u32)0x08004000) 	//����8��ʼ��ַ, 2 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x08004800) 	//����9��ʼ��ַ, 2 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x08005000) 	//����10��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((u32)0x08005800) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_12    ((u32)0x08006000) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_13    ((u32)0x08006800) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_14    ((u32)0x08007000) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_15    ((u32)0x08007800) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_16    ((u32)0x08008000) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_17    ((u32)0x08008800) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_18    ((u32)0x08009000) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_19    ((u32)0x08009800) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_20    ((u32)0x0800A000) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_21    ((u32)0x0800A800) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_22    ((u32)0x0800B000) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_23    ((u32)0x0800B800) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_24    ((u32)0x0800C000) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_25    ((u32)0x0800C800) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_26    ((u32)0x0800D000) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_27    ((u32)0x0800D800) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_28    ((u32)0x0800E000) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_29    ((u32)0x0800E800) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_30    ((u32)0x0800F000) 	//����11��ʼ��ַ,2 Kbytes  
#define ADDR_FLASH_SECTOR_31    ((u32)0x0800F800) 	//����11��ʼ��ַ,2 Kbytes  


/**
* @brief  ����ĳһ������
* @param  sector_start_addr  Ҫ������������ʼ��ַ
* @param  sector_stop_addr   Ҫ����������������ַ
* @param  None
* @retval 0:�ɹ�
* @example 
* @waing  
**/
int FlashEraseSector(u32 sector_start_addr,u32 sector_stop_addr);

/**
* @brief  ָ����ַд��ָ�����ȵ���(��Ҫ���Ȳ����ſ�ʹ�ô˺���)
* @param  WriteAddr  ��ʼд��ĵ�ַ
* @param  pBuffer    д�������
* @param  NumToWrite д������ݸ���
* @retval 0:�ɹ�
* @example 
* @waing  
**/
int FlashWriteWords(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);

/**
* @brief  ָ����ַ��ȡһ����
* @param  faddr  ��ȡ�ĵ�ַ
* @param  none
* @param  none
* @retval ��ȡ����
* @example 
* @waing  
**/
u32 STMFLASH_ReadWord(u32 faddr);


/**
* @brief  ָ����ַ��ȡָ�����ȵ���
* @param  ReadAddr:��ʼ��ַ
* @param  pBuffer:����ָ��
* @param  NumToRead:��ȡ�ĸ���
* @retval ��ȡ����
* @example 
* @waing  
**/
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����


uint16_t STMFLASH_GetFlashSector(u32 addr);
#endif

















