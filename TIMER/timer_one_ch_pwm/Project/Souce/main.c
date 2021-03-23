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
#include "timer.h"
#include "delay.h"

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	uint16_t led0pwmval=0;    
	uint8_t dir=1;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	tim14_pwm_init(100-1,84-1); //84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.
	delay_init(168);
  while (1)
  {
    if(dir)led0pwmval++;//dir==1 led0pwmval����
		else led0pwmval--;	//dir==0 led0pwmval�ݼ� 
 		if(led0pwmval>90)dir=0;//led0pwmval����300�󣬷���Ϊ�ݼ�
		if(led0pwmval==0)dir=1;	//led0pwmval�ݼ���0�󣬷����Ϊ���� 
		TIM_SetCompare1(TIM14,led0pwmval);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		delay_ms(10);	
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
