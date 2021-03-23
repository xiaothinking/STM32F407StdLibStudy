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
#include "dma.h"
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	dma_user_init();
  uart_init(9600);
	delay_init(168);
	//DMA 接收数据流初始化
	Init_Dma_Rx_Config(USART1, (uint32_t)(&USART1->DR), DMA2_Stream5, DMA_Channel_4, (uint32_t)ReceiveBuffer, BufferLen);
  while (1)
  {
			 //接收到一帧数据
			if(rx_flag)    
			{
				//数据帧接收标志清零
					rx_flag = 0;
					//将接受到的数据发送到设备
//					Dma_Send_Enable(DMA2_Stream7, rx_len);                      
          Init_Dma_Tx_Config(USART1, (uint32_t)(&USART1->DR), DMA2_Stream7, DMA_Channel_4, (uint32_t)ReceiveBuffer, BufferLen, DMA2_Stream7_IRQn);
					Dma_Send_Enable(DMA2_Stream7, rx_len);   

			}
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
