#ifndef __ADC_H
#define __ADC_H	
#include "stm32f4xx.h"
 
 							   
void adc_init(void); 				//ADC通道初始化
uint16_t  get_adc(uint8_t ch); 				//获得某个通道值 
uint16_t get_adc_average(uint8_t ch,uint8_t times);//得到某个通道给定次数采样的平均值  
#endif 















