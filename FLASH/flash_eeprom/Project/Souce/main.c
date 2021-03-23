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
#include "uart.h"

//要写入到STM32 FLASH的字符串数组
const uint8_t TEXT_Buffer[]={"STM32 FLASH TEST"};
#define TEXT_LENTH sizeof(TEXT_Buffer)	 		  	//数组长度	
#define SIZE TEXT_LENTH/4+((TEXT_LENTH%4)?1:0)

#define FLASH_SAVE_ADDR  0X0800C004 	//设置FLASH 保存地址(必须为偶数，且所在扇区,要大于本代码所占用到的扇区.
										//否则,写操作的时候,可能会导致擦除整个扇区,从而引起部分程序丢失.引起死机.
uint8_t g_key=255;
uint8_t datatemp[SIZE];	
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);
	key_init();
	uart_init(9600);
  while (1)
  {
		g_key=key_scan();
    if(g_key==0)
     {
			 g_key=255;
			 STMFLASH_Write(FLASH_SAVE_ADDR,(uint32_t*)TEXT_Buffer,SIZE);
			 printf("flash write data: %s",TEXT_Buffer);
		 }
		if(g_key==1)
     {
			 g_key=255;
			 STMFLASH_Read(FLASH_SAVE_ADDR,(uint32_t*)datatemp,SIZE);
			 printf("flash read data: %s",datatemp);
		 }
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
