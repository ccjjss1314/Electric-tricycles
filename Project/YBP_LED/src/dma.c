#include "main.h"

RTC_InitType RTC_InitStructure;
RTC_TimeType RTC_TimeDefault;
RTC_DateType RTC_DateDefault;

uint32_t SynchPrediv, AsynchPrediv;

uint16_t ADCConvertedValue[adc_BufSize] = {0};

void ADC_DMA_CH1_Init(void)
{
	  DMA_InitType DMA_InitStructure;
	
    DMA_Reset(DMA_CH1);
    DMA_InitStructure.PeriphAddr     = (uint32_t)&ADC->DAT;                     //�����ַ
    DMA_InitStructure.MemAddr        = (uint32_t)&ADCConvertedValue;            //�ڴ��ַ
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_SRC;                      //����Ϊ����Դ
    DMA_InitStructure.BufSize        = adc_BufSize;                             //DMAͨ�������С������Ҫ���ټ����ڴ�ռ�
    DMA_InitStructure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;                 //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.CircularMode   = DMA_CIRCULAR_MODE_DISABLE;                //ʹ��ѭ������ģʽ
    DMA_InitStructure.Mem2Mem        = DMA_MEM2MEM_DISABLE;                     //�ر��ڴ浽�ڴ洫��
    DMA_InitStructure.MemDataSize    = DMA_MEM_DATA_WIDTH_HALFWORD;             //�ڴ����ݿ��Ϊ16λ
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;             //�����ַ�Ĵ���������,ֻ��һ��
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_HALFWORD;          //�������ݿ��Ϊ16λ
    DMA_Initializes(DMA_CH1, &DMA_InitStructure);
	
    DMA_Channel_Request_Remap(DMA_CH1 ,DMA_REMAP_ADC);                           //DMA�ض���
	  DMA_InitStructure.Priority = DMA_CH_PRIORITY_LOW;                            //���ȼ�����
    DMA_Channel_Enable(DMA_CH1);
}



void USART1_DMA_CH3_Init(void)
{
	  DMA_InitType DMA_InitStructure;
	
    DMA_Reset(DMA_CH3);
    DMA_InitStructure.PeriphAddr     = (uint32_t)&USART1->DAT;                  //�����ַ
    DMA_InitStructure.MemAddr        = (uint32_t)Uart1ReadBuff;                 //�ڴ��ַ
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_SRC;                      //����Ϊ����Դ
    DMA_InitStructure.BufSize        = Uart1ReadBuffLen;                        //DMAͨ�������С������Ҫ���ټ����ڴ�ռ�
    DMA_InitStructure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;                 //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.CircularMode   = DMA_CIRCULAR_MODE_DISABLE;               //ʹ��ѭ������ģʽ
    DMA_InitStructure.Mem2Mem        = DMA_MEM2MEM_DISABLE;                     //�ر��ڴ浽�ڴ洫��
    DMA_InitStructure.MemDataSize    = DMA_MEM_DATA_WIDTH_BYTE;                 //�ڴ����ݿ��Ϊ8λ
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;             //�����ַ�Ĵ���������,ֻ��һ��
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_BYTE;              //�������ݿ��Ϊ8λ
    DMA_Initializes(DMA_CH3, &DMA_InitStructure);
	
    DMA_Channel_Request_Remap(DMA_CH3 ,DMA_REMAP_USART1_RX);                     //DMA�ض���
	  DMA_InitStructure.Priority = DMA_CH_PRIORITY_HIGH;                           //���ȼ�����
    DMA_Channel_Enable(DMA_CH3);
}


//void SPI2_DMA_CH2_Init(void)
//{
//	  DMA_InitType DMA_InitStructure;
//	
//    DMA_Reset(DMA_CH2);
//    DMA_InitStructure.PeriphAddr     = (uint32_t)&SPI_SLAVE->DAT;                //�����ַ
//    DMA_InitStructure.MemAddr        = (uint32_t)&Temporary_cache[0][0];         //�ڴ��ַ
//    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_DST;                       //����ΪĿ�ĵ�
//    DMA_InitStructure.BufSize        = 6;                                       //DMAͨ�������С������Ҫ���ټ����ڴ�ռ�
//    DMA_InitStructure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;                  //�ڴ��ַ�Ĵ�������
//    DMA_InitStructure.CircularMode   = DMA_CIRCULAR_MODE_DISABLE;                 //ʹ��ѭ������ģʽ
//    DMA_InitStructure.Mem2Mem        = DMA_MEM2MEM_DISABLE;                      //�ر��ڴ浽�ڴ洫��
//    DMA_InitStructure.MemDataSize    = DMA_MEM_DATA_WIDTH_HALFWORD;              //�ڴ����ݿ��Ϊ16λ
//    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;              //�����ַ�Ĵ���������,ֻ��һ��
//    DMA_InitStructure.Priority       = DMA_CH_PRIORITY_HIGH;                     //�趨DMAѡ����ͨ����������ȼ�
//    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_HALFWORD;           //�������ݿ��Ϊ16λ
//    DMA_Initializes(DMA_CH2, &DMA_InitStructure);
//	
//    DMA_Channel_Request_Remap(DMA_CH2 ,DMA_REMAP_SPI2_TX);                       //DMA�ض��壬SPI2_TX DMA����
//		DMA_InitStructure.Priority = DMA_CH_PRIORITY_HIGHEST;                        //���ȼ������
//    DMA_Channel_Enable(DMA_CH2);
//}







/*
*\*\name    RTC_CLKSource_Config.
*\*\fun     Configure the RTC peripheral by selecting the clock source.
*\*\param   ClkSrc    
*\*\            - RTC_CLK_HSE128    clock source select HSE/128
*\*\            - RTC_CLK_LSE       clock source select LSE
*\*\            - RTC_CLK_LSI       clock source select LSI
*\*\param   FirstLastCfg
*\*\            - RTC_CLK_FIRST_CONFIG
*\*\            - RTC_CLK_LAST_CONFIG
*\*\return  none
**/
void RTC_CLKSource_Config(uint8_t ClkSrc, uint8_t FirstLastCfg)
{
    /* Enable the PWR clock */
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_PWR);
    RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);
    /* Allow access to RTC */
    PWR_RTC_Backup_Access_Enable();
    
    RCC_Backup_Reset();
    
    /* Disable RTC clock */
    RCC_RTC_Clock_Disable();

    if (ClkSrc == RTC_CLK_HSE128)
    {
        log_info("\r\n RTC_ClkSrc Is Set HSE128! \r\n");
        if (FirstLastCfg == RTC_CLK_FIRST_CONFIG)
        {
            /* Enable HSE */
            RCC_LSI_Disable();
            RCC_HSE_Config(RCC_HSE_ENABLE);
            while (RCC_HSE_Stable_Wait() == ERROR)
            {
            }
            RCC_RTC_Clock_Config(RCC_RTCCLK_SRC_HSE_DIV128);
        }
        else
        {
            RCC_LSI_Disable();
            RCC_RTC_Clock_Config(RCC_RTCCLK_SRC_HSE_DIV128);

            /* Enable HSE */
            RCC_HSE_Config(RCC_HSE_ENABLE);

            while (RCC_HSE_Stable_Wait() == ERROR)
            {
            }
        }

        SynchPrediv  = 0x1E8; // 8M/128 = 62.5KHz
//				SynchPrediv  = 0x4E2; // 8M/128 = 62.5KHz
//        AsynchPrediv = 0x1F;  // value range: 0-7F
				AsynchPrediv = 0x7F;  // value range: 0-7F
    }
    else if (ClkSrc == RTC_CLK_LSE)
    {
        log_info("\r\n RTC_ClkSrc Is Set LSE! \r\n");

        if (FirstLastCfg == RTC_CLK_FIRST_CONFIG)
        {
            /* Enable the LSE OSC32_IN PC14 */
            RCC_LSI_Disable(); // LSI is turned off here to ensure that only one clock is turned on

#if (_LSE_BYPASS_)
            RCC_LSE_Config(RCC_LSE_BYPASS, 0x141);
#else
            RCC_LSE_Config(RCC_LSE_ENABLE, 0x141);
#endif

            while (RCC_Flag_Status_Get(RCC_FLAG_LSERD) == RESET)
            {
            }

            RCC_RTC_Clock_Config(RCC_RTCCLK_SRC_LSE);
        }
        else
        {
            /* Enable the LSE OSC32_IN PC14 */
            RCC_LSI_Disable();
            RCC_RTC_Clock_Config(RCC_RTCCLK_SRC_LSE);

#if (_LSE_BYPASS_)
            RCC_LSE_Config(RCC_LSE_BYPASS, 0x141);
#else
            RCC_LSE_Config(RCC_LSE_ENABLE, 0x141);
#endif

            while (RCC_Flag_Status_Get(RCC_FLAG_LSERD) == RESET)
            {
            }
        }

        SynchPrediv  = 0xFF; // 32.768KHz
        AsynchPrediv = 0x7F; // value range: 0-7F
    }
    else if (ClkSrc == RTC_CLK_LSI)
    {
        log_info("\r\n RTC_ClkSrc Is Set LSI! \r\n");
        if (FirstLastCfg == RTC_CLK_FIRST_CONFIG)
        {
            /* Enable the LSI OSC */
            RCC_LSI_Enable();

            while (RCC_Flag_Status_Get(RCC_FLAG_LSIRD) == RESET)
            {
            }

            RCC_RTC_Clock_Config(RCC_RTCCLK_SRC_LSI);
        }
        else
        {
            RCC_RTC_Clock_Config(RCC_RTCCLK_SRC_LSI);

            /* Enable the LSI OSC */
            RCC_LSI_Enable();

            while (RCC_Flag_Status_Get(RCC_FLAG_LSIRD) == RESET)
            {
            }
        }

        SynchPrediv  = 0x13B; // 39.64928KHz
        AsynchPrediv = 0x7F;  // value range: 0-7F
    }
    else
    {
        log_info("\r\n RTC_ClkSrc Value is error! \r\n");
    }

    /* Enable the RTC Clock */
    RCC_RTC_Clock_Enable();
    RTC_Wait_For_Synchronization();
}



/**
*\*\name    RTC_Prescaler_Config.
*\*\fun     RTC prescaler config.
*\*\param   RTC_InitStruct: pointer to a RTC_InitType structure. 
*\*\            - RTC_HourFormat
*\*\                - RTC_24HOUR_FORMAT
*\*\                - RTC_12HOUR_FORMAT
*\*\            - RTC_AsynchPrediv  the value in the 0-0x7F range
*\*\            - RTC_SynchPrediv   the value in the 0-0x7FFF range
*\*\return  none
**/
static void RTC_Prescaler_Config(RTC_InitType *RTC_InitStruct)
{
    /* Configure the RTC data register and RTC prescaler */
    RTC_InitStruct->RTC_AsynchPrediv = AsynchPrediv;          //�첽
    RTC_InitStruct->RTC_SynchPrediv  = SynchPrediv;           //ͬ��
    RTC_InitStruct->RTC_HourFormat   = RTC_24HOUR_FORMAT;
}



/*******************************************************************************
**�������ƣ�void RTC_Time_Init(void)
**����������RTC��ʼ��
**��ڲ�����
**�������
*******************************************************************************/
void RTC_Time_Init(void)
{
	//ʱ������
	RTC_CLKSource_Config(RTC_CLK_HSE128, RTC_CLK_FIRST_CONFIG);
	RTC_Prescaler_Config(&RTC_InitStructure);
	
	RTC_DateDefault.Year   = 1;
	RTC_DateDefault.Month  = 1;
	RTC_DateDefault.Date   = 1;
	RTC_DateDefault.WeekDay = 1;
	
	// Time
	RTC_TimeDefault.H12     = RTC_AM_H12;
	RTC_TimeDefault.Hours   = 12;
	RTC_TimeDefault.Minutes = 30;
	RTC_TimeDefault.Seconds = 0;
	
	RTC_Calendar_Initializes(RTC_FORMAT_BIN,&RTC_InitStructure,&RTC_DateDefault, &RTC_TimeDefault,DISABLE);
}