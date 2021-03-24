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
	uint16_t oldcount=0;	//�ϵĴ��ڽ�������ֵ
	uint16_t applenth=0;	//���յ���app���볤��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);
	uart_init(115200);
	key_init();
	printf("����KEY0: Copy APP2Flash!\r\n");
	printf("����KEY1: ��ѯ�̼��汾��!\r\n");
	printf("����KEY2: Run Flash APP!\r\n");
	printf("����WK_UP: Run SRAM APP!\r\n");
  while (1)
  {		
	 	if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				printf("�û�����������!\r\n");
				printf("���볤��:%dBytes\r\n",applenth);
			}else oldcount=USART_RX_CNT;			
		}
		delay_ms(10);  //10msһ���ȴ�����

		m_key=key_scan(0);
		if(m_key==1)	//KEY0��������
		{	
			if(applenth)
			{
				printf("��ʼ���¹̼�...\r\n");				
 				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
				{	 
					iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//����FLASH����   				
					printf("�̼��������!\r\n");	
				}
				else 
				{					   
					printf("��FLASHӦ�ó���!\r\n");
				}
 			}
			else 
			{
				printf("û�п��Ը��µĹ̼�!\r\n");
			}									 
		}
		if(m_key==2)	//KEY1����
		{
				printf("�̼��汾�ţ�V1.0.0.210321 !\r\n");										 
		}
		if(m_key==3)	//KEY2����
		{
			printf("��ʼִ��FLASH�û�����!!\r\n");
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
			}
			else 
			{
				printf("��FLASHӦ�ó���,�޷�ִ��!\r\n");   
			}									   
		}
		if(m_key==4)	//WK_UP����
		{
			printf("��ʼִ��SRAM�û�����!!\r\n");
			if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x20000000)//�ж��Ƿ�Ϊ0X20XXXXXX.
			{	 
				iap_load_app(0X20001000);//SRAM��ַ
			}
			else 
			{
				printf("��SRAMӦ�ó���,�޷�ִ��!\r\n"); 
			}									  
		}		
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
