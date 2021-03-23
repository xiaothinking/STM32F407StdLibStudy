#ifndef _WWDT_H_
#define _WWDT_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "uart.h"

void wwdt_init(uint8_t tr,uint8_t wr,uint32_t fprer);

#endif

