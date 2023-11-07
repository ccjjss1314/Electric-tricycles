#ifndef __SYSTEM_INIT_H__
#define __SYSTEM_INIT_H__
#include "main.h"

#define OFF 0
#define ON  1
#define SYSCLK_SOURCE_HSI 1
#define SYSCLK_SOURCE_HSE 2
#define SYSCLK_SOURCE_HSI_PLL 3
#define SYSCLK_SOURCE_HSE_PLL 4

#define SYSCLK_SOURCE_SELECT SYSCLK_SOURCE_HSE_PLL

#ifndef SYSCLK_SOURCE_SELECT
#define SYSCLK_SOURCE_SELECT SYSCLK_SOURCE_HSE_PLL /*select sysclk source */
#endif


#define SYSCLK_USE_HSIDIV2_PLL 0
#define SYSCLK_USE_HSEDIV2_PLL 1
#define HSE_Value   (8000000)

/***************************************RTC CLK*******************************************/
#define RTC_CLK_HSE128          1
#define RTC_CLK_LSE             2
#define RTC_CLK_LSI             3

#define RTC_CLK_FIRST_CONFIG    0
#define RTC_CLK_LAST_CONFIG     1
/*****************************************************************************************/
#define ONE_WIRE_ENABLE 1            //定义有无一线通功能

#define rb_t_Usart1_receive_buff_len 100

typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} Status;



typedef struct           
{                                                      //0：关闭，1：开启
    uint8_t Left;
    uint8_t Right;
    uint8_t High_beam;
    uint8_t Width_indicator;
    uint8_t Rear_fog_light;
    uint8_t Double_flash;
	  uint8_t Ready;
}Car_indicators;

typedef struct           
{                                                      //0：关闭，1：开启
	  uint8_t reserve_erro;
	  uint8_t motor_erro;
	  uint8_t brake_erro;
	  uint8_t handle_erro;
	  uint8_t stop_erro;
	  uint8_t ECU_erro;
	  uint8_t gear; 
}OneWire_indicators;

typedef struct 
{
	uint8_t message;
	uint8_t phone;
	uint8_t tunnel;
	uint8_t spend_station;
	uint8_t Service;
	uint8_t bluetooth;
}Bluetooth_prompt;


extern Car_indicators       car_led;
extern OneWire_indicators   OneWire_led;
extern Bluetooth_prompt     car_bluetooth;

extern uint8_t battery_icon,battery_frame,battery_icon_blink,battery_frame_blink,led_left_blink,led_right_blink;
extern uint8_t flow_water,border_mode,border_colour;
extern uint8_t battery_value;
extern uint8_t trip_swit;

void Border_detect(void);

//extern char temp[temp_len];
extern unsigned long time_delay;
extern uint32_t Guide_distance,kilometer,temp_kilo,kilometer_sub;
extern float kilometer_test;
extern uint8_t guide,guidance_units,speed;
extern uint16_t Dianping_Voltage,guide_delay_time;

extern RCC_ClocksType RCC_ClockFreq;
void delay_ms(volatile unsigned long nms);
void delay_us(volatile unsigned long nus);
//void delay_us(u32 nus);
//void delay_ms(u16 nms);
void RccInit(void);
void UserSystemInit(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void SysTick_Configuration(void);
__STATIC_INLINE uint32_t SysTick_Configing(uint32_t ticks);

void Pro_1s(void);
void Pro_5ms(void);
void Pro_50ms(void);
void Pro_500ms(void);
void Pro_150ms(void);
void Pro_10ms(void);

void Start_Screen(uint8_t colour);
void Polling_Car_Status(void);

void Kilo_storage(void);
void Sector_select_init();
//extern volatile unsigned long time_delay; // 延时时间，注意定义为全局变量
#endif /* __MAIN_H__ */


