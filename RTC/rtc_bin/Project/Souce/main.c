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
#include "rtc.h"

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	uint8_t t=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
  uart_init(9600);
  My_RTC_Init();
	delay_init(168);
  while (1)
  {
    t++;
		delay_ms(10);
		if((t%10)==0)	//每100ms更新一次显示数据
		{
			RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
			printf("hour=%d\r\n",RTC_TimeStruct.RTC_Hours);
		  printf("Minutes=%d\r\n",RTC_TimeStruct.RTC_Minutes);
      printf("Seconds=%d\r\n",RTC_TimeStruct.RTC_Seconds);
			
			RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
      printf("year=%d\r\n",RTC_DateStruct.RTC_Year);
			printf("week=%d\r\n",RTC_DateStruct.RTC_WeekDay);
			printf("month=%d\r\n",RTC_DateStruct.RTC_Month);
			printf("date=%d\r\n",RTC_DateStruct.RTC_Date);
		} 

  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
