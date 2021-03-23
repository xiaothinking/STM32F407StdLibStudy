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
#include "iic_hardware.h"

#define  EEP_Firstpage      0x00
uint8_t I2c_Buf_Write[18]={"iic hardware test"};
uint8_t I2c_Buf_Read[18];
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
  I2C_EE_Init();
  while (1)
  {
	   g_key=key_scan();
		 if(g_key==0)
     {
			 g_key=255;
			   //将I2c_Buf_Write中顺序递增的数据写入EERPOM中 
	     I2C_EE_BufferWrite( I2c_Buf_Write, EEP_Firstpage, sizeof(I2c_Buf_Write));
			 printf("iic write data:%s\r\n",I2c_Buf_Write);
		 }
		 if(g_key==1)
     {
			 g_key=255;
			 //将EEPROM读出数据顺序保持到I2c_Buf_Read中
	     I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, sizeof(I2c_Buf_Read)); 
			 printf("iic read data:%s\r\n",I2c_Buf_Read);
		 }
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
