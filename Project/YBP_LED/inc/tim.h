#ifndef __TIM_H__
#define __TIM_H__


#include "main.h"



void TIME6_Init(void);
extern uint32_t BSTIMClockFrequency;
void Common_TIM_NVIC_Initialize(IRQn_Type IRQ_number, FunctionalState command);
uint32_t Common_BSTIM_RCC_Initialize(TIM_Module *TIMx, uint32_t hclk_division);
void Common_TIM_Base_Initialize(TIM_Module *TIMx, uint16_t period, uint16_t prescaler);
void TIME3_Init(void);
void TIME4_Init(void);
void TIME2_Init(void);
#endif /* __TIM_H__ */