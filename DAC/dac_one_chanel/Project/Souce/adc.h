#ifndef __ADC_H
#define __ADC_H	
#include "stm32f4xx.h"
 
 							   
void adc_init(void); 				//ADCͨ����ʼ��
uint16_t  get_adc(uint8_t ch); 				//���ĳ��ͨ��ֵ 
uint16_t get_adc_average(uint8_t ch,uint8_t times);//�õ�ĳ��ͨ����������������ƽ��ֵ  
#endif 















