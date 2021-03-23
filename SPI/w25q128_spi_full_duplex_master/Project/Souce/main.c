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
#include "w25qxx.h" 

#define  EEP_Firstpage      0x00
//要写入到W25Q16的字符串数组
const uint8_t TEXT_Buffer[]={"spi flash test"};
#define SIZE sizeof(TEXT_Buffer)	 
uint8_t	datatemp[SIZE];
uint8_t g_key=255;

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
  W25QXX_Init();			//W25QXX初始化
  while (1)
  {
	   g_key=key_scan();
		 if(g_key==0)
     {
			 g_key=255;
			   //将I2c_Buf_Write中顺序递增的数据写入EERPOM中 
	     W25QXX_Write((uint8_t*)TEXT_Buffer,0x00000100,SIZE);		//从倒数第100个地址处开始,写入SIZE长度的数据
			 printf("spi write data:%s\r\n",TEXT_Buffer);
		 }
		 if(g_key==1)
     {
			 g_key=255;
			 //将EEPROM读出数据顺序保持到I2c_Buf_Read中
	      W25QXX_Read(datatemp,0x00000100,SIZE);					//从倒数第100个地址处开始,读出SIZE个字节
			 printf("spi read data:%s\r\n",datatemp);
		 }
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
