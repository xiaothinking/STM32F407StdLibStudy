/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "uart.h"
#include "delay.h"
#include "key.h"
#include "adc.h"

uint8_t g_key=255;
// 局部变量，用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal[RHEOSTAT_NOFCHANEL]={0}; 
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
  uart_init(9600);
	delay_init(168);	 
	key_init();
  adc_init();
  while (1)
  {
	   g_key=key_scan();
		 if(g_key==0)
     {
			  g_key=255;
       
				ADC_ConvertedValueLocal[0] =(float) ADC_ConvertedValue[0]/4096*(float)3.3;
				ADC_ConvertedValueLocal[1] =(float) ADC_ConvertedValue[1]/4096*(float)3.3;
				ADC_ConvertedValueLocal[2] =(float) ADC_ConvertedValue[2]/4096*(float)3.3;
				
				printf("CH1_PA0 value = %f V \r\n",ADC_ConvertedValueLocal[0]);
				printf("CH2_PA5 value = %f V \r\n",ADC_ConvertedValueLocal[1]);
				printf("CH3_PA6 value = %f V \r\n",ADC_ConvertedValueLocal[2]);
		 }
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
