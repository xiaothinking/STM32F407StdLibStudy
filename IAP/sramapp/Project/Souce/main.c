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
	uint16_t oldcount=0;	//�ϵĴ��ڽ�������ֵ
	uint16_t applenth=0;	//���յ���app���볤��
		SCB->VTOR = SRAM_BASE | 0x10000;//����ƫ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);
	key_init();
	led_init();
	uart_init(115200);
	printf("����KEY0: ��תLED0��LED1!\r\n");
	printf("����KEY1: ��ѯ�̼��汾��!\r\n");
	printf("����KEY2: LED0��LED1��!\r\n");
	printf("����WK_UP: LED0��LED1Ϩ��!\r\n");
  while (1)
  {
		g_key=key_scan(0);
		if(g_key==1)	//KEY0��������
		{
			g_key=0;
			GPIO_ToggleBits(GPIOF,GPIO_Pin_9);		
      GPIO_ToggleBits(GPIOF,GPIO_Pin_10);				
		}
		if(g_key==2)	//KEY1����
		{
			  g_key=0;
				printf("�̼��汾�ţ�V1.0.2.210321 !\r\n");										 
		}
		if(g_key==3)	//KEY2����
		{
			g_key=0;
      GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_RESET);	
      GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_RESET);				
		}
		if(g_key==4)	//WK_UP����
		{
			g_key=0;
      GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_SET);	
      GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_SET);			
		}		
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
