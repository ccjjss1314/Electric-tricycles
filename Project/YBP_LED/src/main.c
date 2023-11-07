#include "main.h"
 
 
 uint8_t odotrip_show_Flag = 0;

int main(void)
{
		UserSystemInit();                               //系统初始化
	  //开机动画
	  Start_Screen(0);    
	  delay_ms(1000);
	  Start_Screen(1);
	  delay_ms(1000);
	  battery_value = Battery_Voltage_get();          //获取电平电压
    trip_swit = 2;  //电压显示
	  car_led.Rear_fog_light = led_right_blink = led_left_blink = 0;//后雾灯，暂时未接
	  car_bluetooth.bluetooth = car_bluetooth.message = car_bluetooth.phone = car_bluetooth.spend_station = car_bluetooth.tunnel =0;
		OneWire_led.brake_erro = OneWire_led.handle_erro = OneWire_led.motor_erro = OneWire_led.reserve_erro = OneWire_led.stop_erro = OneWire_led.ECU_erro = 0;
	  OneWire_led.gear = 10;
	
//    flash_helper_init(ADDR_FLASH_SECTOR_31);        //记录扇区编号和扇区擦除次数
//	  Sector_select_init();
    while (1)
    {   
			 if(Flag_usart1_recive)         Usart1_Receive_Deal();
			 if(Flag_5ms)                   Pro_5ms();
			 if(Flag_10ms)                  Pro_10ms();
			 if(Flag_50ms)                  Pro_50ms();
			 if(Flag_150ms)                 Pro_150ms();
			 if(Flag_500ms)                 Pro_500ms();
			 if(Flag_1s)                    Pro_1s();  
    }
}



#ifdef USE_FULL_ASSERT

/**
*\*\name    assert_failed.
*\*\fun     Reports the name of the source file and the source line number
*\*\        where the assert_param error has occurred.
*\*\param   file pointer to the source file name
*\*\param   line assert_param error line source number
*\*\return  none
**/
void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}

#endif




