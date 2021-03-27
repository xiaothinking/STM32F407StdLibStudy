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
	uint8_t paddr[18];				//存放P Addr:+p地址的ASCII值
	uint8_t sramx=0;					//默认为内部sram
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
  uart_init(9600);
	delay_init(168);
  tim_init(10-1,8400-1);	
	usmart_dev.init(84); 	//初始化USMART		
  LCD_Init();           //初始化LCD FSMC接口	
	key_init();
	FSMC_SRAM_Init();
	my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMEX);		//初始化外部内存池
	my_mem_init(SRAMCCM);		//初始化CCM内存池
	POINT_COLOR=RED;      //画笔颜色：红色
	LCD_ShowString(30,40,210,24,24,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"MALLOC TEST");
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");     					 
	LCD_ShowString(30,110,200,16,16,"2021/3/27");   
	LCD_ShowString(30,130,200,16,16,"KEY0:Malloc  KEY2:Free");
	LCD_ShowString(30,150,200,16,16,"KEY_UP:SRAMx KEY1:Read"); 	
	POINT_COLOR=BLUE;//设置字体为蓝色 
	LCD_ShowString(30,170,200,16,16,"SRAMIN");
	LCD_ShowString(30,190,200,16,16,"SRAMIN  USED:   %");
	LCD_ShowString(30,210,200,16,16,"SRAMEX  USED:   %");
	LCD_ShowString(30,230,200,16,16,"SRAMCCM USED:   %");
  while (1)
  {
     		key=key_scan(0);//不支持连按	
		switch(key)
		{
			case 0://没有按键按下	
				break;
			case 1:	//KEY0按下
				p=mymalloc(sramx,2048);//申请2K字节
				if(p!=NULL)sprintf((char*)p,"Memory Malloc Test%03d",i);//向p写入一些内容
				break;
			case 2:	//KEY1按下	   
				if(p!=NULL)
				{
					sprintf((char*)p,"Memory Malloc Test%03d",i);//更新显示内容 	 
					LCD_ShowString(30,270,200,16,16,p);			 //显示P的内容
				}
				break;
			case 3:	//KEY2按下	  
				myfree(sramx,p);//释放内存
				p=0;			//指向空地址
				break;
			case 4:	//KEY UP按下 
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
			LCD_ShowString(30,250,200,16,16,paddr);	//显示p的地址
			if(p)LCD_ShowString(30,270,200,16,16,p);//显示P的内容
		    else LCD_Fill(30,270,239,266,WHITE);	//p=0,清除显示
		}
		delay_ms(10);   
		i++;
		if((i%20)==0)//DS0闪烁.
		{
			LCD_ShowNum(30+104,190,my_mem_perused(SRAMIN),3,16);//显示内部内存使用率
			LCD_ShowNum(30+104,210,my_mem_perused(SRAMEX),3,16);//显示外部内存使用率
			LCD_ShowNum(30+104,230,my_mem_perused(SRAMCCM),3,16);//显示CCM内存使用率
 		}
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
