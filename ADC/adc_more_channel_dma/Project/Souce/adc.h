#ifndef __ADC_H
#define __ADC_H	
#include "stm32f4xx.h"
 
#define RHEOSTAT_NOFCHANEL      3

extern volatile uint16_t ADC_ConvertedValue[RHEOSTAT_NOFCHANEL];

void adc_init(void); 				//ADC通道初始化

#endif 

