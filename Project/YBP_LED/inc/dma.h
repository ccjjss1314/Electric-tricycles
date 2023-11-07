#ifndef __DMA_H__
#define __DMA_H__


#include "adc.h"

#define adc_BufSize  10


extern RTC_TimeType RTC_TimeDefault;
extern uint16_t ADCConvertedValue[adc_BufSize];
void DMA_Init(void);
void USART1_DMA_CH3_Init(void);
void ADC_DMA_CH1_Init(void);
void RTC_Time_Init(void);
#endif /* __DMA_H__ */




