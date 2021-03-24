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
#include "flash_eeprom.h"
#include "delay.h"
#include "key.h"
#include "led.h"
#include "uart.h"
#include "iap.h" 

uint8_t g_key=0;

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	uint16_t oldcount=0;	//老的串口接收数据值
	uint16_t applenth=0;	//接收到的app代码长度
		SCB->VTOR = SRAM_BASE | 0x10000;//设置偏移量
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);
	key_init();
	led_init();
	uart_init(115200);
	printf("按下KEY0: 翻转LED0、LED1!\r\n");
	printf("按下KEY1: 查询固件版本号!\r\n");
	printf("按下KEY2: LED0、LED1亮!\r\n");
	printf("按下WK_UP: LED0、LED1熄灭!\r\n");
  while (1)
  {
		g_key=key_scan(0);
		if(g_key==1)	//KEY0按键按下
		{
			g_key=0;
			GPIO_ToggleBits(GPIOF,GPIO_Pin_9);		
      GPIO_ToggleBits(GPIOF,GPIO_Pin_10);				
		}
		if(g_key==2)	//KEY1按下
		{
			  g_key=0;
				printf("固件版本号：V1.0.2.210321 !\r\n");										 
		}
		if(g_key==3)	//KEY2按下
		{
			g_key=0;
      GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_RESET);	
      GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_RESET);				
		}
		if(g_key==4)	//WK_UP按下
		{
			g_key=0;
      GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_SET);	
      GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_SET);			
		}		
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
