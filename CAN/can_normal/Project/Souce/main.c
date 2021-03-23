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
#include "can.h"
uint8_t g_key=255;
/// 不精确的延时
static void can_delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
} 
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	uint8_t i;
	uint8_t canbuf[8];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
  uart_init(9600);
	delay_init(168);	 
	key_init();
  can_init();
  while (1)
  {
	   g_key=key_scan();
		 if(g_key==0)
     {		 
			  for(i=0;i<8;i++)
        {								
				   canbuf[i]=i;//填充发送缓冲区		 			
			     CAN1_Send_Msg(canbuf,8);//发送8个字节 
				}

		 }

  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
