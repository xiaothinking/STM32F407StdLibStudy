#ifndef _UART_H_
#define _UART_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#define USART_REC_LEN  			120*1024 //�����������ֽ��� 120K
	  	
extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint32_t USART_RX_CNT;				//���յ��ֽ���	


void uart_init(uint32_t bound);
void uart1_receive_handle(void);

#endif

