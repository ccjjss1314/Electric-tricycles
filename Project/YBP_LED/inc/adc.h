#ifndef __ADC_H__
#define __ADC_H__

#include "main.h"

#define RT_NUM 160
#define MINTEMP 0

/**********************48V电池电压定义**************************/
#define Value_48V_1_2                       440
#define Value_48V_2_3                       450            
#define Value_48V_3_4                       465            
#define Value_48V_4_5                       480            
#define Value_48V_charge                    440     
#define Value_48V_poor                      423

/**********************60V电池电压定义**************************/
#define Value_60V_1_2                       545
#define Value_60V_2_3                       562            
#define Value_60V_3_4                       580            
#define Value_60V_4_5                       600            
#define Value_60V_charge                    550     
#define Value_60V_poor                      525

/**********************72V电池电压定义**************************/
#define Value_72V_1_2                       645
#define Value_72V_2_3                       670            
#define Value_72V_3_4                       694            
#define Value_72V_4_5                       717            
#define Value_72V_charge                    650     
#define Value_72V_poor                      625

/**********************48V-72V判断电压**************************/
#define DP_48_72V_change                    590



uint8_t Battery_Voltage_get(void);
uint8_t Speed_get(void);

uint8_t Voltage_Oil(void);
void ADC_Init(void);
void bubble_sort(uint16_t arr[], uint8_t len);
uint16_t ADC_GetData(uint8_t ADC_Channel);
void FindAdcTemp(uint16_t wAdcVal, uint16_t wTempData);
uint8_t Voltage_temperature(void);
#endif /* __ADC_H__ */


