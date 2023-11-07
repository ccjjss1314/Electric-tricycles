#ifndef __SPI_H__
#define __SPI_H__

#include "main.h"

/*数据命令设置*/
#define Data_Cmd_AddrInc           0x40            //地址自动增加
#define Data_Cmd_AddrNoset         0x44            //地址固定

/*显示命令设置*/
#define Display_Cmd_On         0x88
#define Display_Cmd_Off        0x80

#define Brightness_level1      0x00
#define Brightness_level2      0x01
#define Brightness_level4      0x02
#define Brightness_level10     0x03
#define Brightness_level11     0x04
#define Brightness_level12     0x05
#define Brightness_level13     0x06
#define Brightness_level14     0x07

/*地址命令设置*/
#define Addr_Cmd_00H           0xc0          
#define Addr_Cmd_01H           0xc1          
#define Addr_Cmd_02H           0xc2          
#define Addr_Cmd_03H           0xc3          
#define Addr_Cmd_04H           0xc4          
#define Addr_Cmd_05H           0xc5          
#define Addr_Cmd_06H           0xc6         
#define Addr_Cmd_07H           0xc7         
#define Addr_Cmd_08H           0xc8         
#define Addr_Cmd_09H           0xc9          
#define Addr_Cmd_0AH           0xca          
#define Addr_Cmd_0BH           0xcb         
#define Addr_Cmd_0CH           0xcc          
#define Addr_Cmd_0DH           0xcd         
#define Addr_Cmd_0EH           0xce         
#define Addr_Cmd_0FH           0xcf          

/*SPI端口*/
#define SPI_SLAVE                 SPI2
#define SPI_SLAVE_PERIPH          RCC_APB2_PERIPH_SPI2
#define SPI_SLAVE_PERIPH_GPIO     RCC_AHB_PERIPH_GPIOB
#define SPI_SLAVE_GPIO            GPIOB
#define SPI_SLAVE_GPIO_ALTERNATE  GPIO_AF1_SPI2
#define SPI_SLAVE_MISO_PIN        GPIO_PIN_14
#define SPI_SLAVE_MOSI_PIN        GPIO_PIN_15
#define SPI_SLAVE_CLK_PIN         GPIO_PIN_13
#define SPI_SLAVE_NSS_PIN         GPIO_PIN_12
#define SPI_SLAVE_DMA_TX_CH       DMA_REMAP_SPI2_TX
#define SPI_SLAVE_DMA_RX_CH       DMA_REMAP_SPI2_RX

#define ARR_LENGTH(addr)    sizeof(addr)/sizeof(addr[0])
	

extern uint8_t All_LED_num[256];
extern uint8_t showing_arr_Aip1[16];
extern uint8_t showing_arr_Aip2[16];


void SPI2_Init(void);

void All_LED_Switch(uint8_t arr[]);
void Clean_All_LED_num(void);
void Clean_show_LED_num(void);
void Send_AddrInc(uint8_t display_cmd,uint8_t data[16],uint8_t nss);

//发送函数
void Transfer_guide_to_All_LED_num(uint8_t guide,uint32_t guide_distance,uint8_t unit);
void Transfer_onewire_to_All_LED_num();
void Transfer_CarLed_to_All_LED_num();
void Transfer_battery_to_All_LED_num(uint8_t value);
void Transfer_Speed_to_All_LED_num(uint8_t speed,uint8_t speed_unit);
void Transfer_Trip_to_All_LED_num(uint32_t kilometer,uint8_t unit,uint8_t trip_swit);
void Transfer_border_to_All_LED_num(uint8_t colour,uint8_t mode);
#endif /* __SPI_H__ */


