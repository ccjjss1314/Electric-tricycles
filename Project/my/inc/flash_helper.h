#ifndef FLASH_HELPER_H_
#define FLASH_HELPER_H_

#ifndef FLASH_HELPER_C_//如果没有定义
#define FLASH_HELPER_Cx_ extern
#else
#define FLASH_HELPER_Cx_
#endif

#include "main.h"


#define FlashHelperDebug 0 //0:关闭 1:打开

#define FlashHelperDataLen 1       //最大存储的数据(u32类型)个数,根据用户实际修改.
#define FlashHelperUUID 0x55555555 //最后一位的标识符，数组最后一位存储的为0x55555555，用户不能使用最后一位数组

typedef struct flash_helper
{
	/*存储用户数据的数组*/
	u32 FlashHelperData[FlashHelperDataLen];
	/*数组可以保存的字节数*/
	u32 FlashHelperDataSize;
	/*扇区的开始地址*/
	u32 FlashHelperStartAddr;
	/*扇区的结束地址*/
	u32 FlashHelperEndAddr;
	/*记录当前操作的地址*/	
	u32 FlashHelperSaveAddr;
} flash_helper_typedef;

FLASH_HELPER_Cx_ flash_helper_typedef flash_helper_struct;


/**
* @brief  flash_helper_init
* @param  sectors_addr  使用哪一个扇区保存数据
* @param  None
* @param  None
* @retval 0:成功
* @example 
* @waing  
**/
int flash_helper_init(u32 sectors_addr);

/**
* @brief  更改完数组里面的值,调用此函数把数组数据写入flash
* @param  None
* @param  None
* @param  None
* @retval None
* @example 
* @waing  
**/
void flash_helper_update(void);

/**
* @brief   获取缓存的数据
* @param   index 从哪一位数据开始获取(0-)
* @param   data  返回的数据
* @param   datalen 获取的个数
* @retval  0:成功 1:没有缓存的数据
* @warning 
* @example 
**/
int flash_helper_get(u32 index,u32 *data, u32 datalen);

void flash_data_set_count(u32 data);
u32 flash_data_get_count(char mode);
void FlashHelperReadDataEx(u32 addr,u32 *data, u32 datalen);
char FlashHelperEraseSectorEx(u32 sector_start_addr,u32 sector_stop_addr);
#endif













