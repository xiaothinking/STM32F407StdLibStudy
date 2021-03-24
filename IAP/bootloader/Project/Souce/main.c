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
#include "iap.h" 


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	uint8_t m_key;
	uint16_t oldcount=0;	//老的串口接收数据值
	uint16_t applenth=0;	//接收到的app代码长度
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);
	uart_init(115200);
	key_init();
	printf("按下KEY0: Copy APP2Flash!\r\n");
	printf("按下KEY1: 查询固件版本号!\r\n");
	printf("按下KEY2: Run Flash APP!\r\n");
	printf("按下WK_UP: Run SRAM APP!\r\n");
  while (1)
  {		
	 	if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				printf("用户程序接收完成!\r\n");
				printf("代码长度:%dBytes\r\n",applenth);
			}else oldcount=USART_RX_CNT;			
		}
		delay_ms(10);  //10ms一个等待周期

		m_key=key_scan(0);
		if(m_key==1)	//KEY0按键按下
		{	
			if(applenth)
			{
				printf("开始更新固件...\r\n");				
 				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
				{	 
					iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//更新FLASH代码   				
					printf("固件更新完成!\r\n");	
				}
				else 
				{					   
					printf("非FLASH应用程序!\r\n");
				}
 			}
			else 
			{
				printf("没有可以更新的固件!\r\n");
			}									 
		}
		if(m_key==2)	//KEY1按下
		{
				printf("固件版本号：V1.0.0.210321 !\r\n");										 
		}
		if(m_key==3)	//KEY2按下
		{
			printf("开始执行FLASH用户代码!!\r\n");
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
			}
			else 
			{
				printf("非FLASH应用程序,无法执行!\r\n");   
			}									   
		}
		if(m_key==4)	//WK_UP按下
		{
			printf("开始执行SRAM用户代码!!\r\n");
			if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x20000000)//判断是否为0X20XXXXXX.
			{	 
				iap_load_app(0X20001000);//SRAM地址
			}
			else 
			{
				printf("非SRAM应用程序,无法执行!\r\n"); 
			}									  
		}		
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
