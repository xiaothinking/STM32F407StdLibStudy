#ifndef _KEY_H
#define _KEY_H

#include "stm32f4xx.h" 
#include "delay.h" 

void key_init(void);
uint8_t key_scan(void);
#endif

