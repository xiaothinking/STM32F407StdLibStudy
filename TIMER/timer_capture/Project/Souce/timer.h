#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

extern uint8_t   TIM5CH1_CAPTURE_STA;	//���벶��״̬		    				
extern uint32_t	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)

void TIM5_CH1_Cap_Init(uint32_t arr,uint16_t psc);

#endif

