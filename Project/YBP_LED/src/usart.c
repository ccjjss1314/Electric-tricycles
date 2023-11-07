#include "usart.h"


uint8_t  Flag_usart1_recive = 0;
uint8_t  Uart1ReadBuff[Uart1ReadBuffLen];//串口接收缓存数据
uint32_t Uart1ReadBuffCnt = 0;//缓存数据累加
uint32_t Uart1ReadBuffCntCopy = 0;//串口接收的数据个数

uint8_t Flag_bluetooth_Connecting = 0;   // 1:连接中 2:连接成功 3:断开连接

uint8_t Guide_On_Flag = 0;              //0:导航关闭    1:导航打开


//CRC16校验
uint16_t Ymodem_Protocol_Crc16(uint8_t *data, uint16_t len)
{
    uint32_t i;
    uint16_t crc = 0x0000;

    while(len--)
    {
        crc ^= (unsigned short)(*data++) << 8;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}


void Usart1_Init(void)
{
	  //USART1:  PA9:TX  PA10:RX
	  GPIO_InitType GPIO_InitStructure;
	  USART_InitType USART_InitStructure;
	  NVIC_InitType NVIC_InitStructure;
	
	  BufferManageCreate(&buff_manage_struct_test,Manage_Data_Buff,Manage_Data_Buff_Len,Manage_num_Buff,Manage_num_Buff_Len*4);   //创建缓存,处理串口数据
	 
	  RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_USART1);        //开启USART1外设时钟
	  
	  //GPIO初始化
    GPIO_Structure_Initialize(&GPIO_InitStructure);                   //默认值填充
    /* Configure USARTx Tx as alternate function push-pull */
    GPIO_InitStructure.Pin            = USARTy_TxPin;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;              //推挽复用输出（查手册，RX同）             
    GPIO_InitStructure.GPIO_Alternate = USARTy_Tx_GPIO_AF;
    GPIO_Peripheral_Initialize(USARTy_GPIO, &GPIO_InitStructure);   
    /* Configure USARTx Rx as alternate function push-pull */
    GPIO_InitStructure.Pin            = USARTy_RxPin;
    GPIO_InitStructure.GPIO_Alternate = USARTy_Rx_GPIO_AF;
    GPIO_Peripheral_Initialize(USARTy_GPIO, &GPIO_InitStructure); 
	  
	  //中断配置
	  /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = USARTy_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
	
	  //USART初始化
    USART_InitStructure.BaudRate            = 115200;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
    USART_Initializes(USARTy, &USART_InitStructure);

		USART_Interrput_Enable(USARTy,USART_INT_IDLEF);
		USART_DMA_Transfer_Enable(USART1,USART_DMAREQ_RX);
    //   USART_Interrput_Enable(USARTy, USART_INT_TXDE);
		//   USART_Interrput_Enable(USARTy, USART_INT_RXDNE);

    USART_Enable(USARTy);
    printf("\n\r串口初始化成功\n\r");
}


/**
* @brief  串口发送字符串数据
* @param  *c:发送的数据指针  cnt:数据个数
* @param  None
* @param  None
* @retval None
* @example 
**/
void UsartOutStr(USART_Module* USARTx, uint8_t *c,uint32_t cnt)
{
	while(cnt--)
	{
		USART_Data_Send(USARTx, *c++);
		while(USART_Flag_Status_Get(USART1, USART_FLAG_TXDE) == RESET );
	}
}

/**
* @brief  串口发送一个字节
* @param  *c:发送的数据指针  cnt:数据个数
* @param  None
* @param  None
* @retval None
* @example 
**/
void UsartOutChar(USART_Module* USARTx, unsigned char c)
{
	USART_Data_Send(USARTx,c);
	while(USART_Flag_Status_Get(USART1, USART_FLAG_TXDE) == RESET );
}



void Guide_Data_Deal(uint8_t index)
{
		Guide_distance = (Manage_Buff_Copy[index+16]<<24) + (Manage_Buff_Copy[index+15]<<16) + (Manage_Buff_Copy[index+14]<<8)+Manage_Buff_Copy[index+13];
		if(Guide_distance > 99)       
			guidance_units = 2; //单位切换为km 
    else
      guidance_units = 1;	//小于99m,单位切换为m		
//		if(m > 100)
//		Guide_distance = m/100;
		switch(Manage_Buff_Copy[index+12])               //下一路口方向 byte13
		{
			case 1: guide = 4;break;       //左转
			case 2: guide = 5;break;       //右转
			case 3: guide = 6;break;       //左前
			case 4: guide = 7;break;       //右前
			case 5: break;       //左后
			case 6: guide = 3;break;       //右后
			case 7: guide = 2;break;       //左掉头
			case 8: guide = 1;break;       //直行
			default:break;
		}
		guide_delay_time = 0;
}


/*******************************************************************************
**函数名称：void Usart1_Receive_Deal(void)
**功能描述：串口1接收处理函数
**入口参数：
**输出：
*******************************************************************************/
uint8_t Usart1_Receive_Deal(void)
{
		uint8_t      i=0;
		uint16_t     tempdata;//一个灵活的临时变量
	
	  BufferManageRead(&buff_manage_struct_test,Manage_Buff_Copy,&buff_manage_struct_test.ReadLen);//读取缓存的数据
	
		if(buff_manage_struct_test.ReadLen>0)                                                        //有一帧数据
		{
			if(strstr((char *)Manage_Buff_Copy,"Connect")!= NULL)
				Flag_bluetooth_Connecting = 1;
			else if(strstr((char *)Manage_Buff_Copy,"Disconnect")!= NULL)
			{
				Flag_bluetooth_Connecting = 0;
				guide = 0;
			}
      
		  if(Flag_bluetooth_Connecting)
			{
				for(i=0;i<20;i++)
			  {
					if(Manage_Buff_Copy[i]==0x24 && Manage_Buff_Copy[i+1]==0x24)      //找到报头和结束位
					{
						tempdata = (uint16_t)(Manage_Buff_Copy[i+18]<<8) + (uint16_t)Manage_Buff_Copy[i+17];
						break;
					}
			  }
				if(i > 0) {printf("串口1接收到错误数据"); return 1;}
				else if(Ymodem_Protocol_Crc16(Manage_Buff_Copy,17)== tempdata)                    //crc16校验
				Guide_Data_Deal(i);
			}
			memset(Manage_Buff_Copy,0x00,20);                                                      //处理完一帧数据后，清除buff内容            
			Flag_usart1_recive = 0;                                                                //处理完一帧数据后，等待下一帧数据置1
			//			UsartOutStr(USART1,Manage_Buff_Copy,buff_manage_struct_test.ReadLen);          //回复数据
			return 0;	
		}
}



/**
*\*\name    Buffercmp.
*\*\fun     Compares two buffers.
*\*\param   pBuffer1
*\*\param   pBuffer2
*\*\param   BufferLength
*\*\return  PASSED or FAILED
**/
Status Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
}



/**
* @brief  使用microLib的方法,使用printf
* @warningg 勾选 microLib
* @param  None
* @param  None
* @param  None
* @retval None
* @example 
**/

static int is_lr_sent = 0;

int fputc(int ch, FILE* f)
{
    if (ch == '\r')
    {
        is_lr_sent = 1;
    }
    else if (ch == '\n')
    {
        if (!is_lr_sent)
        {
            USART_Data_Send(USART1, (uint8_t)'\r');
            /* Loop until the end of transmission */
            while (USART_Flag_Status_Get(USART1, USART_FLAG_TXC) == RESET)
            {
            }
        }
        is_lr_sent = 0;
    }
    else
    {
        is_lr_sent = 0;
    }
    USART_Data_Send(USART1, (uint8_t)ch);
    /* Loop until the end of transmission */
    while (USART_Flag_Status_Get(USART1, USART_FLAG_TXDE) == RESET)
    {
    }
    return ch;
}






//int fputc(int ch, FILE *f)
//{
//	static char flage=0;
//	uint8_t data = ch;
//	if(!flage)
//	{
//	  flage=1;
//		USART_Interrupt_Status_Clear(USART1,USART_INT_TXC);//解决printf丢失第一个字节的问题
//	}
//	UsartOutChar(USART1, (uint8_t) ch);
//  return ch;
//}


void PrintfClockInfo(void)
{
    printf("--------------------------------\n");
    RCC_Clocks_Frequencies_Value_Get(&RCC_ClockFreq);
    printf("SYSCLK: %d\n", RCC_ClockFreq.SysclkFreq);
    printf("HCLK: %d\n", RCC_ClockFreq.HclkFreq);
    printf("PCLK1: %d\n", RCC_ClockFreq.Pclk1Freq);
    printf("PCLK2: %d\n", RCC_ClockFreq.Pclk2Freq);
}





/**
*\*\name    fputc.
*\*\fun     retarget the C library printf function to the USART
*\*\param   ch
*\*\param   f
*\*\return  none
**/
//int fputc(int ch, FILE* f)
//{
//    USART_Data_Send(USARTy, (uint8_t)ch);
//    while (USART_Flag_Status_Get(USARTy, USART_FLAG_TXDE) == RESET)
//        ;

//    return (ch);
//}
