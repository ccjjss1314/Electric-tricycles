#include "System_Init.h"
RCC_ClocksType RCC_ClockFreq;

ErrorStatus HSEStartUpStatus;
ErrorStatus HSIStartUpStatus;
unsigned long time_delay = 0;   //�ж���ʱ
uint8_t Updata = 0;

Car_indicators       car_led;
OneWire_indicators   OneWire_led;
Bluetooth_prompt     car_bluetooth;
//����
uint8_t battery_value= 0;
uint16_t Dianping_Voltage = 0; 

//��˸��־
uint8_t battery_icon,battery_frame,
        battery_icon_blink,battery_frame_blink ,
				led_left_blink,led_right_blink = 0; 

//�߿����
uint8_t flow_water,border_mode = 0,
                 border_colour = 1;

//������
float kilometer_test = 0;
uint32_t temp_kilo = 0;
uint32_t kilometer,kilometer_sub = 0;
uint8_t  kilometer_units = 1,
              trip_swit = 0;

//�������
uint8_t guide,guidance_units = 0;
uint16_t guide_delay_time= 0;
uint32_t Guide_distance = 0;
//�ٶ����
uint8_t Speed_units,speed= 1;

//flash���
uint8_t Sector_flag = 0;

/**
*\*\name    SetSysClockToHSI.
*\*\fun     Selects HSI as System clock source and configure HCLK, PCLK2
*\*\         and PCLK1 prescalers.
*\*\param   none
*\*\return  none 
**/
void SetSysClockToHSI(void)
{
    RCC_Reset();

    RCC_HSI_Enable();

    /* Wait till HSI is ready */
    HSIStartUpStatus = RCC_HSI_Stable_Wait();

    if (HSIStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
        FLASH_Prefetch_Buffer_Enable();

        /* Flash 0 wait state */
        FLASH_Latency_Set(FLASH_LATENCY_0);

        /* HCLK = SYSCLK */
        RCC_Hclk_Config(RCC_SYSCLK_DIV1);

        /* PCLK2 = HCLK */
        RCC_Pclk2_Config(RCC_HCLK_DIV1);

        /* PCLK1 = HCLK */
        RCC_Pclk1_Config(RCC_HCLK_DIV1);

        /* Select HSI as system clock source */
        RCC_Sysclk_Config(RCC_SYSCLK_SRC_HSI);
           
        /* Wait till HSI is used as system clock source */
        while (RCC_Sysclk_Source_Get() != RCC_CFG_SCLKSTS_HSI)
        {
        }
    }
    else
    {
        /* If HSI fails to start-up, the application will have wrong clock
           configuration. User can add here some code to deal with this error */

        /* Go to infinite loop */
        while (1)
        {
        }
    }
}



/**
*\*\name    SetSysClockToHSE.
*\*\fun     Selects HSE as System clock source and configure HCLK, PCLK2
*\*\          and PCLK1 prescalers.
*\*\param   none
*\*\return  none 
**/
void SetSysClockToHSE(void)
{
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration
     * -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_Reset();

    /* Enable HSE */
    RCC_HSE_Config(RCC_HSE_ENABLE);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_HSE_Stable_Wait();

//	   RCC_PLL_Enable();
//	   RCC_PLL_Config(RCC_PLL_SRC_HSE_DIV1,RCC_PLL_MUL_16);
	
    if (HSEStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
        FLASH_Prefetch_Buffer_Enable();

        if (HSE_Value <= 18000000)
        {
            /* Flash 0 wait state */
            FLASH_Latency_Set(FLASH_LATENCY_0);
        }
        else
        {
            /* Flash 1 wait state */
            FLASH_Latency_Set(FLASH_LATENCY_1);
        }

        /* HCLK = SYSCLK */
        RCC_Hclk_Config(RCC_SYSCLK_DIV1);

        /* PCLK2 = HCLK */
        RCC_Pclk2_Config(RCC_HCLK_DIV1);

        /* PCLK1 = HCLK */
        RCC_Pclk1_Config(RCC_HCLK_DIV1);

        /* Select HSE as system clock source */
        RCC_Sysclk_Config(RCC_SYSCLK_SRC_HSE);
       
        /* Wait till HSE is used as system clock source */
        while (RCC_Sysclk_Source_Get() != RCC_CFG_SCLKSTS_HSE)
        {
        }
    }
    else
    {
        /* If HSE fails to start-up, the application will have wrong clock
           configuration. User can add here some code to deal with this error */

        /* Go to infinite loop */
        while (1)
        {
        }
    }
}


/**
*\*\name    SetSysClockToPLL.
*\*\fun     Selects PLL clock as System clock source and configure HCLK, PCLK2
*\*\         and PCLK1 prescalers.
*\*\param   none
*\*\return  none 
**/
void SetSysClockToPLL(uint32_t freq, uint8_t src)
{
    uint32_t pllmul;
    uint32_t latency;
    uint32_t pclk1div, pclk2div;

    if (HSE_VALUE != 8000000)
    {
        /* HSE_VALUE == 8000000 is needed in this project! */
        while (1);
    }

    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration
     * -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_Reset();

    if (src == SYSCLK_USE_HSIDIV2_PLL) 
    {
        /* Enable HSI */
        RCC_HSI_Enable();

        /* Wait till HSI is ready */
        HSIStartUpStatus = RCC_HSI_Stable_Wait();

        if (HSIStartUpStatus != SUCCESS)
        {
            /* If HSI fails to start-up, the application will have wrong clock
               configuration. User can add here some code to deal with this
               error */

            /* Go to infinite loop */
            while (1);
        }

    }
    else if (src == SYSCLK_USE_HSEDIV2_PLL) 
    {
        /* Enable HSE */
        RCC_HSE_Config(RCC_HSE_ENABLE);

        /* Wait till HSE is ready */
        HSEStartUpStatus = RCC_HSE_Stable_Wait();

        if (HSEStartUpStatus != SUCCESS)
        {
            /* If HSE fails to start-up, the application will have wrong clock
               configuration. User can add here some code to deal with this
               error */

            /* Go to infinite loop */
            while (1);
        }

    }

    switch (freq)
    {
        case 32000000:
            latency  = FLASH_LATENCY_1;
            if(src == SYSCLK_USE_HSIDIV2_PLL)
            {
                pllmul = RCC_PLL_MUL_16;
            }
            else if(src == SYSCLK_USE_HSEDIV2_PLL)
            {
                pllmul = RCC_PLL_MUL_16;
            }
            pclk1div = RCC_HCLK_DIV2;
            pclk2div = RCC_HCLK_DIV1;
            break;
        case 48000000:
            latency  = FLASH_LATENCY_1;
            if(src == SYSCLK_USE_HSIDIV2_PLL)
            {
              pllmul = RCC_PLL_MUL_12;
            }
            else if(src == SYSCLK_USE_HSEDIV2_PLL)
            {
               pllmul = RCC_PLL_MUL_12;
            }
            pclk1div = RCC_HCLK_DIV2;
            pclk2div = RCC_HCLK_DIV1;
            break;
        default:
            while (1);
    }

    FLASH_Latency_Set(latency);

    /* HCLK = SYSCLK */
    RCC_Hclk_Config(RCC_SYSCLK_DIV1);

    /* PCLK2 = HCLK */
    RCC_Pclk2_Config(pclk2div);

    /* PCLK1 = HCLK */
    RCC_Pclk1_Config(pclk1div);
    
    if(src == SYSCLK_USE_HSEDIV2_PLL)
    {
        RCC_PLL_Config(RCC_PLL_SRC_HSE_DIV1,pllmul);
    }
    else
    {
        RCC_PLL_Config(RCC_PLL_SRC_HSE_DIV1,pllmul);
    }

    /* Enable PLL */
    RCC_PLL_Enable();

    /* Wait till PLL is ready */
   // while (RCC_Flag_Status_Get(RCC_FLAG_PLLRD) == RESET);
     /* Wait till PLL is ready */
    while ((RCC->CTRL & RCC_CTRL_PLLRDF) == 0)
    {
    }
    /* Select PLL as system clock source */
    RCC_Sysclk_Config(RCC_SYSCLK_SRC_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_Sysclk_Source_Get() != RCC_CFG_SCLKSTS_PLL);
}



/*******************************************************************************
**�������ƣ�void RccInit(void)
**����������ϵͳʱ�ӳ�ʼ��
**��ڲ�����
**�������
*******************************************************************************/
void RccInit(void)
{
	#if SYSCLK_SOURCE_SELECT == SYSCLK_SOURCE_HSI
    
    SetSysClockToHSI();
    PrintfClockInfo("HSI, 8MHz");
    
#elif SYSCLK_SOURCE_SELECT == SYSCLK_SOURCE_HSE  
    
    SetSysClockToHSE();
//    printf("HSE, 8MHz");
    
#elif SYSCLK_SOURCE_SELECT == SYSCLK_SOURCE_HSI_PLL 
    
    SetSysClockToPLL(48000000, SYSCLK_USE_HSIDIV2_PLL);
    PrintfClockInfo("HSIDIV2->PLL, 48M");
    
    
#elif SYSCLK_SOURCE_SELECT == SYSCLK_SOURCE_HSE_PLL 
   
    SetSysClockToPLL(32000000, SYSCLK_USE_HSEDIV2_PLL);
//    printf("HSEDIV2->PLL, 32M");
    
#endif
}


__STATIC_INLINE uint32_t SysTick_Configing(uint32_t ticks)
{
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
  {
    return (1UL);                                                   /* Reload value impossible */
  }

  SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
  return (0UL);                                                     /* Function successful */
}


//������systic�ж�
//void delay_us(u32 nus)
//{
// u32 temp;
// SysTick->LOAD = 16*nus;
// SysTick->VAL=0X00;//��ռ�����
// SysTick->CTRL=0X01;//ʹ�ܣ����������޶����������ⲿʱ��Դ
// do
// {
//  temp=SysTick->CTRL;//��ȡ��ǰ������ֵ
// }while((temp&0x01)&&(!(temp&(1<<16))));//�ȴ�ʱ�䵽��
//     SysTick->CTRL=0x00; //�رռ�����
//    SysTick->VAL =0X00; //��ռ�����
//}
//void delay_ms(u16 nms)
//{
// u32 temp;
// SysTick->LOAD = 16000*nms;
// SysTick->VAL=0X00;//��ռ�����
// SysTick->CTRL=0X01;//ʹ�ܣ����������޶����������ⲿʱ��Դ
// do
// {
//  temp=SysTick->CTRL;//��ȡ��ǰ������ֵ
// }while((temp&0x01)&&(!(temp&(1<<16))));//�ȴ�ʱ�䵽��
//    SysTick->CTRL=0x00; //�رռ�����
//    SysTick->VAL =0X00; //��ռ�����
//}
 

//ms��ʱ����
void delay_ms(volatile unsigned long nms)
{
    //SYSTICK��Ƶ--1ms��ϵͳʱ���ж�
    if (SysTick_Config((SystemCoreClock) / 1000))
    {
        while (1);
    }
    time_delay=nms;//��ȡ��ʱʱ��
    while(time_delay);
    SysTick->CTRL=0x00; //�رռ�����
    SysTick->VAL =0X00; //��ռ�����
}


//us��ʱ����
void delay_us(volatile unsigned long nus)
{
 //SYSTICK��Ƶ--1us��ϵͳʱ���ж�
    if (SysTick_Config((SystemCoreClock) / 1000000))
    {
        while (1);
    }
    time_delay=nus;//��ȡ��ʱʱ��
    while(time_delay);
    SysTick->CTRL=0x00; //�رռ�����
    SysTick->VAL =0X00; //��ռ�����
}

/**
*\*\name    RCC_Configuration.
*\*\fun     Configures the different system clocks.
*\*\return  none
**/
void RCC_Configuration(void)
{
	
	 /* PCLK2 = HCLK/2 */
   RCC_Pclk2_Config(RCC_HCLK_DIV2);
	 /* Enable Clock Security System(CSS): this will generate an NMI exception when HSE clock fails */
   RCC_Clock_Security_System_Enable();

	 RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA|RCC_AHB_PERIPH_GPIOB|RCC_AHB_PERIPH_GPIOD);	  //GPIOA��GPIOB��GPIOD ʱ��ʹ��
	 RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);                                            //����AFIO����ʱ��
	
	 RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_DMA);                                               //DMAʱ��ʹ��
}

/**
*\*\name    GPIO_Configuration.
*\*\fun     Configures the different GPIO ports.
*\*\return  none
**/
void GPIO_Configuration(void)
{
		GPIO_InitType GPIO_InitStructure;
	  //PA8 : STB1
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    GPIO_InitStructure.Pin        = GPIO_PIN_8;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;
	  GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
	  GPIO_Pins_Set(GPIOA, GPIO_PIN_8);
	
	 //GPIOA
		GPIO_InitStructure.Pin          = GPIO_PIN_5;         //PA5
	  GPIO_InitStructure.GPIO_Mode    = GPIO_MODE_INPUT;
		GPIO_InitStructure.GPIO_Pull    = GPIO_PULL_UP;
		GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
	
	#if ONE_WIRE_ENABLE==1
	  GPIO_InitStructure.Pin          = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;  //PA2��PA3��PA4��λ��һ��ͨ��
	  GPIO_InitStructure.GPIO_Pull    = GPIO_PULL_DOWN;
	  GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
	#endif
	
	 //GPIOB
	  GPIO_InitStructure.Pin          = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_7;  //PB3��PB4��PB7�ƹ���ʾ
   	GPIO_InitStructure.GPIO_Pull    = GPIO_PULL_DOWN;
	  GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
}


/*******************************************************************************
**�������ƣ�Pro_5ms
**����������5���봦���� 

*********************************************************************************/
void GPIO_ExtiInit(void)
{
	  GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;
    
	  //GPIOA
		GPIO_Structure_Initialize(&GPIO_InitStructure);
		GPIO_InitStructure.Pin          = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
		GPIO_InitStructure.GPIO_Pull    = GPIO_PULL_UP;
		GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
	
	  GPIO_InitStructure.Pin          = GPIO_PIN_15;
	  GPIO_InitStructure.GPIO_Pull    = GPIO_PULL_DOWN;
	  GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    
	  //GPIOB
	  GPIO_InitStructure.Pin          = GPIO_PIN_0;
   	GPIO_InitStructure.GPIO_Pull    = GPIO_PULL_UP;
	  GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
	  
    /* Configure EXTI Line to Pin */
		GPIO_EXTI_Line_Set(EXTI_LINE_SOURCE0, AFIO_EXTI_PB0);
		GPIO_EXTI_Line_Set(EXTI_LINE_SOURCE2, AFIO_EXTI_PA2);
    GPIO_EXTI_Line_Set(EXTI_LINE_SOURCE3, AFIO_EXTI_PA3);
		GPIO_EXTI_Line_Set(EXTI_LINE_SOURCE4, AFIO_EXTI_PA4);
		GPIO_EXTI_Line_Set(EXTI_LINE_SOURCE5, AFIO_EXTI_PA5);
		GPIO_EXTI_Line_Set(EXTI_LINE_SOURCE6, AFIO_EXTI_PA6);
		GPIO_EXTI_Line_Set(EXTI_LINE_SOURCE7, AFIO_EXTI_PA7);
		GPIO_EXTI_Line_Set(EXTI_LINE_SOURCE15, AFIO_EXTI_PA15);
		
    /* Configure EXTI line */
    EXTI_InitStructure.EXTI_Line    = EXTI_LINE2|EXTI_LINE3|EXTI_LINE4|EXTI_LINE5|EXTI_LINE6|EXTI_LINE7;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                     //�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Peripheral_Initializes(&EXTI_InitStructure);
		
		EXTI_InitStructure.EXTI_Line    = EXTI_LINE0|EXTI_LINE15;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;                       //�����ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Peripheral_Initializes(&EXTI_InitStructure);

    /* Set input interrupt priority */
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = NVIC_SUB_PRIORITY_1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel                   = EXTI15_10_IRQn;
    NVIC_Initializes(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel                   = EXTI0_IRQn;
    NVIC_Initializes(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel                   = EXTI1_IRQn;
    NVIC_Initializes(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel                   = EXTI2_IRQn;
    NVIC_Initializes(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel                   = EXTI3_IRQn;
    NVIC_Initializes(&NVIC_InitStructure);
		
	  NVIC_InitStructure.NVIC_IRQChannel                   = EXTI4_IRQn;
    NVIC_Initializes(&NVIC_InitStructure);
}


/**
*\*\name    NVIC_Configuration.
*\*\fun     Configures the nested vectored interrupt controller.
*\*\param   none
*\*\return  none
**/
void NVIC_Configuration(void)
{
	  NVIC_SetPriority(SysTick_IRQn, 0x0);
}



/*******************************************************************************
**�������ƣ�void Sector_select_init(void)
**������������̴洢flash
**��ڲ�����
**�������
*******************************************************************************/
void Sector_select_init()
{
	uint32_t sector;
	sector = flash_data_get_count(0);
//	FlashHelperReadDataEx(flash_helper_struct.FlashHelperSaveAddr,&sector,1);
	switch(sector)
	{
		case 0: flash_helper_init(ADDR_FLASH_SECTOR_22);break;
		case 22:flash_helper_init(ADDR_FLASH_SECTOR_22);break;
		case 23:flash_helper_init(ADDR_FLASH_SECTOR_23);break;
		case 24:flash_helper_init(ADDR_FLASH_SECTOR_24);break;
		case 25:flash_helper_init(ADDR_FLASH_SECTOR_25);break;
		case 26:flash_helper_init(ADDR_FLASH_SECTOR_26);break;
		case 27:flash_helper_init(ADDR_FLASH_SECTOR_27);break;
		case 28:flash_helper_init(ADDR_FLASH_SECTOR_28);break;
		case 29:flash_helper_init(ADDR_FLASH_SECTOR_29);break;
		case 30:flash_helper_init(ADDR_FLASH_SECTOR_30);break;
		case 31:flash_helper_init(ADDR_FLASH_SECTOR_31);break;
		default:break;
	}
	if(Sector_flag)
	{
			flash_data_set_count(temp_kilo);                    //���������������flash
	    flash_helper_update();                          //ˢ�����ݵ�flash
	}
	
	kilometer = flash_data_get_count(0)/1000;        //km
	Sector_flag = 0;
}


/*******************************************************************************
**�������ƣ�Pro_5ms
**����������5���봦���� 

*********************************************************************************/		
void Pro_5ms(void)
{
	static uint8_t cnt_5ms;	
  Flag_5ms=0;
	
	if(guide)
	guide_delay_time++;
	if(guide_delay_time > 400)
		guide = 0;
		
	if(Updata)
	{
		All_LED_Switch(All_LED_num);
		Send_AddrInc(Display_Cmd_On | Brightness_level14,showing_arr_Aip1,1);
		Send_AddrInc(Display_Cmd_On | Brightness_level14,showing_arr_Aip2,2);
		Clean_All_LED_num();
		Clean_show_LED_num();
		
		Updata = 0;
	}
	
	Polling_Car_Status();
	
	if(++cnt_5ms>1)
	{
		cnt_5ms=0;
		Flag_10ms = 1;
	}
}


/*******************************************************************************
**�������ƣ�Pro_5ms
**����������5���봦���� 

*********************************************************************************/		
void Pro_10ms(void)
{
	static uint8_t cnt_10ms;	
	static uint32_t Speed_cnt_10ms;
  Flag_10ms=0;
	
	
	if(Flag_bluetooth_Connecting)          //������ʾ
	car_bluetooth.bluetooth = 1;
	else
	car_bluetooth.bluetooth = 0;
	
	
	
	if(++cnt_10ms > 4)
	{
		cnt_10ms=0;
		Flag_50ms = 1;
	}
}



/*******************************************************************************
**�������ƣ�Pro_5ms
**����������5���봦���� 

*********************************************************************************/		
void Pro_50ms(void)
{
  static uint8_t cnt1_50ms,cnt2_50ms;	
  Flag_50ms=0;
	
	speed = Speed_get();   //ʵʱ�ٶȻ�ȡ
	
	if(++cnt1_50ms > 2)
	{
		cnt1_50ms=0;
		Flag_150ms = 1;
	}
	
	if(++cnt2_50ms > 9)
	{
		cnt2_50ms=0;
		Flag_500ms = 1;
	}
}



/*******************************************************************************
**�������ƣ�Pro_250ms
**����������100���봦���� 

*********************************************************************************/
void Pro_150ms(void)
{
	Flag_150ms=0;
	if(flow_water++ == 10)   //��ˮ�ٶ�
		  flow_water = 0;
	
	Border_detect();
	
	Transfer_CarLed_to_All_LED_num();
	Transfer_onewire_to_All_LED_num();
  Transfer_Trip_to_All_LED_num(kilometer,kilometer_units,trip_swit);
	Transfer_Speed_to_All_LED_num(speed,Speed_units);
	Transfer_battery_to_All_LED_num(battery_value);
	Transfer_guide_to_All_LED_num(guide,Guide_distance,guidance_units);
  Transfer_border_to_All_LED_num(border_colour,border_mode);
	Updata = 1;
}

/*******************************************************************************
**�������ƣ�Pro_500ms
**����������500���봦���� 

*********************************************************************************/		
void Pro_500ms(void)
{
	static uint8_t cnt_500ms;	
  Flag_500ms=0;
	
	if(car_led.Left)
		led_left_blink = ~led_left_blink;
	else
		led_left_blink = 0;
	if(car_led.Right)
		led_right_blink = ~led_right_blink;
	else
		led_right_blink = 0;
	
	
	//�����˸
  if(battery_icon)
		battery_icon_blink = ~battery_icon_blink;
	if(battery_frame)
		battery_frame_blink = ~battery_frame_blink;
	
	if(!battery_frame && !speed)
		car_led.Ready = 1;
	else
		car_led.Ready = 0;
	
	if(++cnt_500ms>1)
	{
		cnt_500ms=0;
		Flag_1s = 1;
	}
}



/*******************************************************************************
**�������ƣ�Pro_1s
**����������1�봦���� 

*********************************************************************************/		

void Pro_1s(void)
{
	static uint8_t cnt1s;
	Flag_1s=0;

	
	if(++cnt1s==2)
		trip_swit = 0; //չʾ�����
	if(cnt1s==6)
	{
		trip_swit = 1; //չʾС�����
		odotrip_show_Flag = 1; //�����չʾ���
	}
	
	battery_value = Battery_Voltage_get();          //��ȡ��ƽ��ѹ
	
	
//	if(Sector_flag)
//	Sector_select_init();
	
//	if(speed)
//	{
//		kilometer_test = speed*0.277778;
//		kilometer_sub += kilometer_test;
//	  Kilo_storage();
//	}
}





/*******************************************************************************
**�������ƣ�void Start_Screen(void)
**������������������ת�ٳ�ʼ��
**��ڲ�����
**�������
*******************************************************************************/
void Start_Screen(uint8_t colour)
{
	led_right_blink = led_left_blink = car_led.High_beam = car_led.Rear_fog_light = car_led.Width_indicator = car_led.Ready = 1;
	
	OneWire_led.brake_erro = OneWire_led.handle_erro = OneWire_led.motor_erro = OneWire_led.reserve_erro = OneWire_led.stop_erro = OneWire_led.ECU_erro = 1;
	OneWire_led.gear = 8;
	
	car_bluetooth.bluetooth = car_bluetooth.message = car_bluetooth.phone = car_bluetooth.spend_station = car_bluetooth.tunnel =1;
	
	Transfer_CarLed_to_All_LED_num();
	Transfer_onewire_to_All_LED_num();
  Transfer_Trip_to_All_LED_num(88888,0,0);
	Transfer_Speed_to_All_LED_num(88,0);
	Transfer_battery_to_All_LED_num(5);
	Transfer_guide_to_All_LED_num(8,88800,0);
  Transfer_border_to_All_LED_num(colour,0);        //��
	
	All_LED_Switch(All_LED_num);
	Send_AddrInc(Display_Cmd_On | Brightness_level14,showing_arr_Aip1,1);
	Send_AddrInc(Display_Cmd_On | Brightness_level14,showing_arr_Aip2,2);
	Clean_All_LED_num();
	Clean_show_LED_num();
}



void Polling_Car_Status(void)
{
	//�ƹ���ʾ��ѯ�Ƿ�Ϊ��
	if(GPIOB->PID & GPIO_PIN_7)       car_led.Width_indicator = 1; 
	else                              car_led.Width_indicator = 0;
	
	if(GPIOB->PID & GPIO_PIN_4)       car_led.Right = 1; 
	else                              car_led.Right = 0;
	
	if(GPIOB->PID & GPIO_PIN_3)       car_led.Left = 1; 
	else                              car_led.Left = 0;
	
	if(GPIOA->PID & GPIO_PIN_15)      car_led.High_beam = 1; 
	else                              car_led.High_beam = 0;
}


/*******************************************************************************
**�������ƣ�void Kilo_storage(void)
**������������̴洢flash
**��ڲ�����
**�������
*******************************************************************************/
void Kilo_storage(void)    //ÿ5�׸���һ��flash
{
	  uint32_t addr,erase_num;
	  uint8_t  sector_number;
	  
	  temp_kilo = flash_data_get_count(1) + kilometer_test;  //���¼��㵱ǰ���m
		kilometer_test = 0;
	  flash_data_set_count(temp_kilo);                       //���������������flash
	  flash_helper_update();                          //ˢ�����ݵ�flash
		
		kilometer = temp_kilo/1000;
	
	if(flash_helper_struct.FlashHelperSaveAddr > (flash_helper_struct.FlashHelperStartAddr + 2040))//�жϵ�ǰ����λ���Ƿ�Ҫ�Ѵﵱǰ����ĩβ
		{
			Sector_flag = 1;
			addr = flash_helper_struct.FlashHelperSaveAddr;      //���浱ǰ������ݴ洢��ַ
			FlashHelperEraseSectorEx(flash_helper_struct.FlashHelperStartAddr,flash_helper_struct.FlashHelperStartAddr);     //����������
			
			flash_helper_init(ADDR_FLASH_SECTOR_31);             //��ȡ��ǰ������Ų��洢��31������
      sector_number = STMFLASH_GetFlashSector(addr);	
			sector_number +=1;
			if(sector_number >30)
				sector_number = 22;
			flash_data_set_count(sector_number);
			flash_helper_update(); 
			
			Sector_select_init();
		}                
}



/*******************************************************************************
**�������ƣ�void border_detect(void)
**�����������߿���仯
**��ڲ�����
**�������
*******************************************************************************/
void Border_detect(void)
{
	if(speed && speed<60)
	{
		border_mode = 1;
		border_colour = 1;  //��ɫ
	}
	else if(speed>60)
	{
			border_mode = 1;
		  border_colour = 0;
	}
	else if(!speed)
	{
		border_mode = 0;
		border_colour = 1;
	}
}


/*******************************************************************************
**�������ƣ�void SystemInit(void)
**����������ϵͳ��ʼ��
**��ڲ�����
**�������
*******************************************************************************/
void UserSystemInit(void)
{	
	RccInit();                  //��ʼ���ⲿʱ��
	RCC_Configuration();       
	GPIO_Configuration();    
	NVIC_Configuration();
	TIME6_Init(); 
  Usart1_Init();
	USART1_DMA_CH3_Init();
  SPI2_Init();
  ADC_Init();   
//	RTC_Time_Init();
	PrintfClockInfo();              //ϵͳ��ǰʱ�Ӵ�ӡ
//	TIME3_Init();                   //�ٶȲɼ�
//	TIME2_Init();                   //ת�ٲɼ�
	//TIME4_Init();
	//  GPIO_ExtiInit();	
}