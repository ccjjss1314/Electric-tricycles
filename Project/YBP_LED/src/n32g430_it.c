/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
*\*\file n32g430_it.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
**/

#include "n32g430_it.h"
#include "main.h"

uint8_t Flag_1ms,Flag_5ms,Flag_10ms,Flag_50ms,Flag_150ms,Flag_500ms,Flag_1s= 0;

volatile uint16_t PB1_ReadValueL   = 0,PB1_ReadValueH = 0;
volatile uint16_t PB11_ReadValueL   = 0,PB11_ReadValueH = 0;

volatile uint16_t CaptureNumber_TIM3,CaptureNumber_TIM2 = 0;

volatile uint32_t PB1_Capture         = 0, PB1_CaptureH         = 0, PB1_CaptureL         = 0;
volatile uint32_t PB11_Capture         = 0, PB11_CaptureH         = 0, PB11_CaptureL         = 0;

volatile float  TIM2Freq,TIM3Freq = 0;
volatile uint32_t GPTIMClockFrequency = 32000000;
/** N32G430_StdPeriph_Template
*\*\ 
**/

/**
*\*\name    NMI_Handler.
*\*\fun     This function handles NMI exception.
*\*\return  none
**/
void NMI_Handler(void)
{
}
/**
*\*\name    HardFault_Handler.
*\*\fun     This function handles Hard Fault exception.
*\*\return  none
**/
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}
/**
*\*\name    MemManage_Handler.
*\*\fun     This function handles Memory Manage exception.
*\*\return  none
**/
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}
/**
*\*\name    BusFault_Handler.
*\*\fun     This function handles Bus Fault exception.
*\*\return  none
**/
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}
/**
*\*\name    UsageFault_Handler.
*\*\fun     This function handles Usage Fault exception.
*\*\return  none
**/
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}
/**
*\*\name    SVC_Handler.
*\*\fun     This function handles SVCall exception.
*\*\return  none
**/
//void SVC_Handler(void)
//{
//}
/**
*\*\name    DebugMon_Handler.
*\*\fun     This function handles Debug Monitor exception.
*\*\return  none
**/
void DebugMon_Handler(void)
{
}
/**
*\*\name    SysTick_Handler.
*\*\fun     This function handles SysTick Handler.
*\*\return  none
**/
void SysTick_Handler(void)
{
	    //在中断中将time_delay递减。实现延时
	    if(time_delay)
        time_delay--;
}

void TIM6_IRQHandler(void)
{
	  static uint16_t Cnt1ms;
    if (TIM_Interrupt_Status_Get(TIM6, TIM_INT_UPDATE) != RESET)
    {
        TIM_Interrupt_Status_Clear(TIM6, TIM_INT_UPDATE);
	      Flag_1ms = ~Flag_1ms;
			  if(Cnt1ms++>=4)
        {
           Cnt1ms = 0;
           Flag_5ms=1;                       
        }		
    }
}



void USART1_IRQHandler(void)
{
	uint8_t Res;
	int value;
//	if(USART_Interrupt_Status_Get(USART1, USART_INT_RXDNE) != RESET)
//	{
//		Res =USART_Data_Receive(USART1);	        		//读取接收到的数据,USART_INT_RXDNE自动清零
//		if(Uart1ReadBuffCnt < Uart1ReadBuffLen-1) 		//防止溢出
//		{
//			Uart1ReadBuff[Uart1ReadBuffCnt] = Res;
//			Uart1ReadBuffCnt++;
//		}
//	}
 if(USART_Interrupt_Status_Get(USART1, USART_INT_IDLEF)!= RESET)   //空闲中断,此时dma已经把数据存入数组
	{
		USART1->STS;
		USART1->DAT;       //先读STS,在读DAT清中断
		
		//DMA方式接收，减少中断
		DMA_Channel_Disable(DMA_CH3);
		Uart1ReadBuffCnt = Uart1ReadBuffLen - DMA_Current_Data_Transfer_Number_Get(DMA_CH3);   //计算接收的数据个数（数组长度(20)-DMA缓存剩余长度(0-20)）
		BufferManageWrite(&buff_manage_struct_test,Uart1ReadBuff,Uart1ReadBuffCnt,&value);    //存入一帧数据到缓存队列
		DMA_Current_Data_Transfer_Number_Set(DMA_CH3,Uart1ReadBuffLen);
		DMA_Channel_Enable(DMA_CH3);                                                          //开启DMA将数据送入Uart1ReadBuff
		if(DMA_Current_Data_Transfer_Number_Get(DMA_CH3) != 20)                               //解决数据超出规定字节数bug
		{
			DMA_Channel_Disable(DMA_CH3);
			DMA_Memory_Address_Config(DMA_CH3,(uint32_t)Uart1ReadBuff);
			DMA_Current_Data_Transfer_Number_Set(DMA_CH3,Uart1ReadBuffLen);
			DMA_Channel_Enable(DMA_CH3);
		}
		Flag_usart1_recive = 1;
		//Uart1ReadBuffCntCopy = Uart1ReadBuffCnt;
		//Uart1ReadBuffCnt = 0;                                                               //一帧数据清零，重新开始接收下一帧接收数据
	}
}


void TIM4_IRQHandler(void)                  
{
	static uint16_t Cnt1us;
    if (TIM_Interrupt_Status_Get(TIM4, TIM_INT_UPDATE) != RESET)
    {
        TIM_Interrupt_Status_Clear(TIM4, TIM_INT_UPDATE);
			 
    }
}


/*******************************************************************EXTI*********************************************************************/
void EXTI1_IRQHandler(void)
{

}

void EXTI0_IRQHandler(void)
{

}

void EXTI2_IRQHandler(void)
{

}

void EXTI3_IRQHandler(void)
{

}

void EXTI4_IRQHandler(void)
{
	
}


void EXTI9_5_IRQHandler(void)
{

}

void EXTI15_10_IRQHandler(void)
{

}
/**
*\*\name    PPP_IRQHandler.
*\*\fun     This function handles PPP interrupt request.
*\*\return  none
**/
/*void PPP_IRQHandler(void)
{
}*/




