#ifndef _DMA_H_
#define _DMA_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "uart.h"

extern uint16_t rx_len;
extern uint8_t SendBuffer[256];
extern uint8_t ReceiveBuffer[256];
extern uint16_t BufferLen;
extern uint8_t rx_flag; 

extern GPIO_InitTypeDef GPIO_InitStructure;                       //定义·IO初始化结构体
extern USART_InitTypeDef USART_InitStructure;                     //定义串口结构体
extern DMA_InitTypeDef DMA_InitStructure;                         //定义DMA结构体
extern NVIC_InitTypeDef NVIC_InitStructure;                       //定义中高结构体

extern void dma_user_init(void);
extern void Init_Dma_Tx_Config(USART_TypeDef *Usartx,uint32_t Par, DMA_Stream_TypeDef *DMA_Streamx_Tx, uint32_t DMA_Chx,uint32_t Tx_Buf, uint16_t Tx_Size, uint32_t DMA_Streamx_Irqn);
extern void Init_Dma_Rx_Config(USART_TypeDef *Usartx, uint32_t Par, DMA_Stream_TypeDef *DMA_Streamx_Rx, uint32_t DMA_Chx,uint32_t Rx_Buf, uint16_t Rx_Size);
extern void Dma_Send_Enable(DMA_Stream_TypeDef *DMA_Streamx_tx,uint16_t ndtr);
extern void DMA2_Stream7_IRQHandler(void);
extern void USART1_IRQHandler(void);


#endif

