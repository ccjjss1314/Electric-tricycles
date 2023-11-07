#ifndef __USART_H__
#define __USART_H__

#include "main.h"

#define Uart1ReadBuffLen 20


#define USARTy            USART1
#define USARTy_CLK        RCC_APB2_PERIPH_USART1
#define USARTy_GPIO       GPIOA
#define USARTy_RxPin      GPIO_PIN_10
#define USARTy_TxPin      GPIO_PIN_9
#define USARTy_Rx_GPIO_AF GPIO_AF5_USART1
#define USARTy_Tx_GPIO_AF GPIO_AF5_USART1
#define USARTy_APBxClkCmd RCC_APB2_Peripheral_Clock_Enable
#define USARTy_IRQn       USART1_IRQn
#define USARTy_IRQHandler USART1_IRQHandler



typedef struct
{
        uint8_t mark;
        uint8_t dstAddr[6];
        uint8_t id;
        uint8_t cmd;
        uint8_t cmdExt;
        uint8_t datLen;
        uint8_t datLenH;
        uint8_t data[130];

        
}BlePkt_TypeDef;




extern unsigned char Uart1ReadBuff[Uart1ReadBuffLen];
extern u32 Uart1ReadBuffCnt;//缓存数据累加
extern u32 Uart1ReadBuffCntCopy;//串口接收的数据个数
extern uint8_t Flag_bluetooth_Connecting,Flag_usart1_recive;
extern uint8_t Guide_On_Flag;




void Usart1_Init(void);
uint8_t Usart1_Receive_Deal(void);
void UsartOutStr(USART_Module* USARTx, uint8_t *c,uint32_t cnt);
void PrintfClockInfo(void);
void Guide_Data_Deal(uint8_t index);
void Bluetooth_connection(uint8_t index);
#endif /* __USART_H__ */








