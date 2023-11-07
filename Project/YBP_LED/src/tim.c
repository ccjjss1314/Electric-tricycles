#include "tim.h"


uint32_t BSTIMClockFrequency = 0;

static TIM_TimeBaseInitType TIM_TimeBaseStructure;

/**
*\*\name    Common_BSTIM_RCC_Initialize.
*\*\param   TIMx :
*\*\          - TIM6
*\*\param   hclk_division
*\*\          - RCC_HCLK_DIV1
*\*\          - RCC_HCLK_DIV2
*\*\          - RCC_HCLK_DIV4
*\*\          - RCC_HCLK_DIV8
*\*\          - RCC_HCLK_DIV16
*\*\return  uint32_t
**/
uint32_t Common_BSTIM_RCC_Initialize(TIM_Module *TIMx, uint32_t hclk_division)
{
    uint32_t BSTIM_clock;

    RCC_ClocksType RCC_Clocks;
    
    RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);
    
    RCC_Pclk1_Config(hclk_division);
    if(TIM6 == TIMx)
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_TIM6);
		else if(TIM4 == TIMx)
		RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_TIM4);
		else if(TIM3 == TIMx)
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_TIM3);
		else if(TIM2 == TIMx)
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_TIM2);
		
    RCC_Clocks_Frequencies_Value_Get(&RCC_Clocks);

    if(RCC_Clocks.HclkFreq > RCC_Clocks.Pclk1Freq) 
    {
        BSTIM_clock = RCC_Clocks.Pclk1Freq * 2; 
    }
    else
    {
        BSTIM_clock = RCC_Clocks.Pclk1Freq;
    }
    return BSTIM_clock;
}


/**
*\*\name    Common_TIM_NVIC_Initialize.
*\*\param   IRQ_number :
*\*\          - TIM1_BRK_IRQn
*\*\          - TIM1_UP_IRQn
*\*\          - TIM1_TRG_COM_IRQn
*\*\          - TIM1_CC_IRQn
*\*\          - TIM2_IRQn
*\*\          - TIM3_IRQn
*\*\          - TIM4_IRQn
*\*\          - TIM8_BRK_IRQn
*\*\          - TIM8_UP_IRQn
*\*\          - TIM8_TRG_COM_IRQn
*\*\          - TIM8_CC_IRQn
*\*\          - TIM5_IRQn
*\*\          - TIM6_IRQ
*\*\param   command
*\*\          - ENABLE
*\*\          - DISABLE
*\*\return  none
**/
void Common_TIM_NVIC_Initialize(IRQn_Type IRQ_number, FunctionalState command)
{
    NVIC_InitType NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = IRQ_number;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    
    if(ENABLE == command)
    {
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    }
    else
    {
        NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    }
    
    NVIC_Initializes(&NVIC_InitStructure);
}




/**
*\*\name    Common_TIM_Base_Initialize.
*\*\param   TIMx :
*\*\          - TIM1
*\*\          - TIM2
*\*\          - TIM3
*\*\          - TIM4
*\*\          - TIM5
*\*\          - TIM6
*\*\param   period
*\*\          - [1, 0xffff]
*\*\param   prescaler
*\*\          - [0, 0xffff]
*\*\return  none
**/
void Common_TIM_Base_Initialize(TIM_Module *TIMx, uint16_t period, uint16_t prescaler)
{
    TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.Period    = period;
    TIM_TimeBaseStructure.Prescaler = prescaler;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;

    TIM_Base_Initialize(TIMx, &TIM_TimeBaseStructure);   
}


void TIME6_Init(void)
{
	//TIME6
	BSTIMClockFrequency = Common_BSTIM_RCC_Initialize(TIM6, RCC_HCLK_DIV4);
	
	//TIME6
  Common_TIM_NVIC_Initialize(TIM6_IRQn, ENABLE);
	
	 /* Time base configuration， period = 65535, prescaler = prescaler */
	Common_TIM_Base_Initialize(TIM6, 63999, 0);
	
	TIM_Base_Reload_Mode_Set(TIM6, TIM_PSC_RELOAD_MODE_IMMEDIATE);

  TIM_Interrupt_Enable(TIM6, TIM_INT_UPDATE);

  TIM_On(TIM6);
}


void TIME4_Init(void)
{
	GPIO_InitType   GPIO_InitStructure;
	EXTI_InitType   EXTI_InitStructure;
  NVIC_InitType   NVIC_InitStructure;
	
	GPIO_Structure_Initialize(&GPIO_InitStructure);
	GPIO_InitStructure.Pin            = GPIO_PIN_1;
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;
	GPIO_InitStructure.GPIO_Pull      = GPIO_NO_PULL;
	GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
	
	/* Configure key EXTI Line to PB1 input Pin */
  GPIO_EXTI_Line_Set(EXTI_LINE_SOURCE1, AFIO_EXTI_PB1);
	 /* Configure EXTI line */
	EXTI_InitStructure.EXTI_Line    = EXTI_LINE1;
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Peripheral_Initializes(&EXTI_InitStructure);
	/* Set interrupt priority */
	NVIC_InitStructure.NVIC_IRQChannel                   = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = NVIC_SUB_PRIORITY_1;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Initializes(&NVIC_InitStructure);
	
	
	
	BSTIMClockFrequency = Common_BSTIM_RCC_Initialize(TIM4, RCC_HCLK_DIV4);
	
  Common_TIM_NVIC_Initialize(TIM4_IRQn, ENABLE);
	
	 /* Time base configuration， period = 65535, prescaler = prescaler */
	Common_TIM_Base_Initialize(TIM4, 31999, 0);                                  //1ms
	
	TIM_Base_Reload_Mode_Set(TIM4, TIM_PSC_RELOAD_MODE_IMMEDIATE);

  TIM_Interrupt_Enable(TIM4, TIM_INT_UPDATE);

  TIM_On(TIM4);
}




//定时器3初始化，PB1速度捕获
void TIME3_Init(void)
{
	GPIO_InitType   GPIO_InitStructure;
  TIM_ICInitType  TIM_ICInitStructure;
	
	//TIME3
	BSTIMClockFrequency = Common_BSTIM_RCC_Initialize(TIM3, RCC_HCLK_DIV4);         //MAX:32M HZ
	/* Time base configuration， period = 65535, prescaler = prescaler */
	Common_TIM_Base_Initialize(TIM3, 63999, 599);                                  //
  Common_TIM_NVIC_Initialize(TIM3_IRQn, ENABLE);
	
	//PB1脉冲捕获,TIM3_CH4
	GPIO_Structure_Initialize(&GPIO_InitStructure);
	GPIO_InitStructure.Pin            = GPIO_PIN_1;
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;
	GPIO_InitStructure.GPIO_Current   = GPIO_DS_4MA;
	GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM3;
	GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
	
	TIM_Input_Struct_Initialize(&TIM_ICInitStructure);
	TIM_ICInitStructure.Channel     = TIM_CH_4;
	TIM_ICInitStructure.IcPolarity  = TIM_IC_POLARITY_RISING;
	TIM_ICInitStructure.IcSelection = TIM_IC_SELECTION_DIRECTTI;
	TIM_ICInitStructure.IcPrescaler = TIM_IC_PSC_DIV1;
	TIM_ICInitStructure.IcFilter    = 0x0;
	TIM_Input_Channel_Initialize(TIM3, &TIM_ICInitStructure);
	
	TIM_Input_Struct_Initialize(&TIM_ICInitStructure);
	TIM_ICInitStructure.Channel     = TIM_CH_3;
	TIM_ICInitStructure.IcPolarity  = TIM_IC_POLARITY_FALLING;
	TIM_ICInitStructure.IcSelection = TIM_IC_SELECTION_INDIRECTTI;
	TIM_ICInitStructure.IcPrescaler = TIM_IC_PSC_DIV1;
	TIM_ICInitStructure.IcFilter    = 0x0;
	TIM_Input_Channel_Initialize(TIM3, &TIM_ICInitStructure);

	TIM_On(TIM3);
	TIM_Interrupt_Enable(TIM3, TIM_INT_CC3 | TIM_INT_CC4);
}



//定时器2初始化，PB11转速捕获
void TIME2_Init(void)
{
	GPIO_InitType   GPIO_InitStructure;
  TIM_ICInitType  TIM_ICInitStructure;
	
	//TIME2
	BSTIMClockFrequency = Common_BSTIM_RCC_Initialize(TIM2, RCC_HCLK_DIV4);         //MAX:32M HZ
	/* Time base configuration， period = 65535, prescaler = prescaler */
	Common_TIM_Base_Initialize(TIM2, 63999, 599);                                  //
  Common_TIM_NVIC_Initialize(TIM2_IRQn, ENABLE);
	
	//PB11脉冲捕获,TIM2_CH4
	GPIO_Structure_Initialize(&GPIO_InitStructure);
	GPIO_InitStructure.Pin            = GPIO_PIN_11;
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;
	GPIO_InitStructure.GPIO_Current   = GPIO_DS_4MA;
	GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM2;                    //GPIO_AF6_TIM2
	GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
	
	TIM_Input_Struct_Initialize(&TIM_ICInitStructure);
	TIM_ICInitStructure.Channel     = TIM_CH_4;
	TIM_ICInitStructure.IcPolarity  = TIM_IC_POLARITY_RISING;
	TIM_ICInitStructure.IcSelection = TIM_IC_SELECTION_DIRECTTI;
	TIM_ICInitStructure.IcPrescaler = TIM_IC_PSC_DIV1;
	TIM_ICInitStructure.IcFilter    = 0x0;
	TIM_Input_Channel_Initialize(TIM2, &TIM_ICInitStructure);
	
	TIM_Input_Struct_Initialize(&TIM_ICInitStructure);
	TIM_ICInitStructure.Channel     = TIM_CH_3;
	TIM_ICInitStructure.IcPolarity  = TIM_IC_POLARITY_FALLING;
	TIM_ICInitStructure.IcSelection = TIM_IC_SELECTION_INDIRECTTI;
	TIM_ICInitStructure.IcPrescaler = TIM_IC_PSC_DIV1;
	TIM_ICInitStructure.IcFilter    = 0x0;
	TIM_Input_Channel_Initialize(TIM2, &TIM_ICInitStructure);

	TIM_On(TIM2);
	TIM_Interrupt_Enable(TIM2, TIM_INT_CC3 | TIM_INT_CC4);
}










