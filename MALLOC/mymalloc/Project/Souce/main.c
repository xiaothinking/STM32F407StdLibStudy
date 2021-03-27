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
#include "timer.h"
#include "lcd.h"
#include "usmart.h"
#include "sram.h"
#include "key.h"
#include "malloc.h"

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	uint8_t key;		 
 	uint8_t i=0;	    
	uint8_t *p=0;
	uint8_t *tp=0;
	uint8_t paddr[18];				//���P Addr:+p��ַ��ASCIIֵ
	uint8_t sramx=0;					//Ĭ��Ϊ�ڲ�sram
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
  uart_init(9600);
	delay_init(168);
  tim_init(10-1,8400-1);	
	usmart_dev.init(84); 	//��ʼ��USMART		
  LCD_Init();           //��ʼ��LCD FSMC�ӿ�	
	key_init();
	FSMC_SRAM_Init();
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		//��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ��
	POINT_COLOR=RED;      //������ɫ����ɫ
	LCD_ShowString(30,40,210,24,24,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"MALLOC TEST");
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");     					 
	LCD_ShowString(30,110,200,16,16,"2021/3/27");   
	LCD_ShowString(30,130,200,16,16,"KEY0:Malloc  KEY2:Free");
	LCD_ShowString(30,150,200,16,16,"KEY_UP:SRAMx KEY1:Read"); 	
	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
	LCD_ShowString(30,170,200,16,16,"SRAMIN");
	LCD_ShowString(30,190,200,16,16,"SRAMIN  USED:   %");
	LCD_ShowString(30,210,200,16,16,"SRAMEX  USED:   %");
	LCD_ShowString(30,230,200,16,16,"SRAMCCM USED:   %");
  while (1)
  {
     		key=key_scan(0);//��֧������	
		switch(key)
		{
			case 0://û�а�������	
				break;
			case 1:	//KEY0����
				p=mymalloc(sramx,2048);//����2K�ֽ�
				if(p!=NULL)sprintf((char*)p,"Memory Malloc Test%03d",i);//��pд��һЩ����
				break;
			case 2:	//KEY1����	   
				if(p!=NULL)
				{
					sprintf((char*)p,"Memory Malloc Test%03d",i);//������ʾ���� 	 
					LCD_ShowString(30,270,200,16,16,p);			 //��ʾP������
				}
				break;
			case 3:	//KEY2����	  
				myfree(sramx,p);//�ͷ��ڴ�
				p=0;			//ָ��յ�ַ
				break;
			case 4:	//KEY UP���� 
				sramx++; 
				if(sramx>2)sramx=0;
				if(sramx==0)LCD_ShowString(30,170,200,16,16,"SRAMIN ");
				else if(sramx==1)LCD_ShowString(30,170,200,16,16,"SRAMEX ");
				else LCD_ShowString(30,170,200,16,16,"SRAMCCM");
				break;
		}
		if(tp!=p)
		{
			tp=p;
			sprintf((char*)paddr,"P Addr:0X%08X",(uint32_t)tp);
			LCD_ShowString(30,250,200,16,16,paddr);	//��ʾp�ĵ�ַ
			if(p)LCD_ShowString(30,270,200,16,16,p);//��ʾP������
		    else LCD_Fill(30,270,239,266,WHITE);	//p=0,�����ʾ
		}
		delay_ms(10);   
		i++;
		if((i%20)==0)//DS0��˸.
		{
			LCD_ShowNum(30+104,190,my_mem_perused(SRAMIN),3,16);//��ʾ�ڲ��ڴ�ʹ����
			LCD_ShowNum(30+104,210,my_mem_perused(SRAMEX),3,16);//��ʾ�ⲿ�ڴ�ʹ����
			LCD_ShowNum(30+104,230,my_mem_perused(SRAMCCM),3,16);//��ʾCCM�ڴ�ʹ����
 		}
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
