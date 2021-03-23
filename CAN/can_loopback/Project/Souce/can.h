#ifndef __CAN_H
#define __CAN_H	

#include "stm32f4xx.h"
#include "uart.h"
 
void can_init(void);
uint8_t CAN1_Send_Msg(uint8_t* msg,uint8_t len);
uint8_t CAN1_Receive_Msg(uint8_t *buf);

#endif 

