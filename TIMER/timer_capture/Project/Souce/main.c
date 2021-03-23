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
#include "timer.h"
#include "delay.h"
#include "uart.h"
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	long long temp=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
  TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //以1Mhz的频率计数 ,每1us计数一次，最大计数时间（2^32-1）us
	uart_init(9600);
	delay_init(168);
  while (1)
  {
 		if(TIM5CH1_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 		         //溢出时间总和
			temp+=TIM5CH1_CAPTURE_VAL;		   //得到总的高电平时间
			printf("HIGH:%lld us\r\n",temp); //打印总的高点平时间
			TIM5CH1_CAPTURE_STA=0;			     //开启下一次捕获
		}
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
