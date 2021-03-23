#ifndef _UART_H_
#define _UART_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#define MAX_UART_RX_BUFFER 256
#define INIT_ZERO  0
typedef struct
{
    uint8_t writepos;
    uint8_t readpos;
}Uart_pos;

/*uart2 ���ջ������飬256�ֽ�*/
extern uint8_t ar_uart1_buffer[MAX_UART_RX_BUFFER];
/*uart2 ���ա��������ݶ�λָ��*/
extern Uart_pos uart_tmp1;
/*uart2 ���ݽ������ʱ����*/
extern volatile uint8_t g_uart1_tick;
/*uart2 ���ݽ�����ɱ�־λ*/
extern volatile uint8_t g_uart1_receive_finish;


void uart_init(uint32_t bound);
void uart1_receive_handle(void);

#endif

