#ifndef _UART_H_
#define _UART_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#define USART_REC_LEN  			120*1024 //定义最大接收字节数 120K
	  	
extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern uint32_t USART_RX_CNT;				//接收的字节数	


void uart_init(uint32_t bound);
void uart1_receive_handle(void);

#endif

