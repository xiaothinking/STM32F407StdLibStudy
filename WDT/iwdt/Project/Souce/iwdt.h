#ifndef _IWDT_H_
#define _IWDT_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "uart.h"

void iwdt_init(uint8_t prer,uint16_t rlr);

#endif

