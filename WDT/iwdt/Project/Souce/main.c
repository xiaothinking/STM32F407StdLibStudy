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
#include "iwdt.h"
#include "key.h"
#include "led.h"
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
	iwdt_init(IWDG_Prescaler_64,500);//与分频数为64,重载值为500,溢出时间为1s	
	key_init();
	led_init();
	delay_ms(100);
	GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_SET);
	GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_SET);
  while (1)
  {
   	if(0==GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2))
    {
			  delay_ms(10);
				if(0==GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2))
          {						
						IWDG_ReloadCounter(); 
					}
		}
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
