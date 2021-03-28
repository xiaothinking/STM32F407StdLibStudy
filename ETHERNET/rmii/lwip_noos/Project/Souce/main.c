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
#include "delay.h"
#include "uart.h"
#include "key.h"
#include "lwip_comm.h"
#include "LAN8720.h"
#include "usmart.h"
#include "timer.h"
#include "lcd.h"
#include "sram.h"
#include "malloc.h"
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"

void show_address(u8 mode)
{
	u8 buf[30];
	if(mode==1)
	{
		sprintf((char*)buf,"MAC    :%d.%d.%d.%d.%d.%d",lwipdev.mac[0],lwipdev.mac[1],lwipdev.mac[2],lwipdev.mac[3],lwipdev.mac[4],lwipdev.mac[5]);//打印MAC地址
		LCD_ShowString(30,130,210,16,16,buf); 
		sprintf((char*)buf,"DHCP IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
		LCD_ShowString(30,150,210,16,16,buf); 
		sprintf((char*)buf,"DHCP GW:%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
		LCD_ShowString(30,170,210,16,16,buf); 
		sprintf((char*)buf,"DHCP IP:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
		LCD_ShowString(30,190,210,16,16,buf); 
	}
	else 
	{
		sprintf((char*)buf,"MAC      :%d.%d.%d.%d.%d.%d",lwipdev.mac[0],lwipdev.mac[1],lwipdev.mac[2],lwipdev.mac[3],lwipdev.mac[4],lwipdev.mac[5]);//打印MAC地址
		LCD_ShowString(30,130,210,16,16,buf); 
		sprintf((char*)buf,"Static IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
		LCD_ShowString(30,150,210,16,16,buf); 
		sprintf((char*)buf,"Static GW:%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
		LCD_ShowString(30,170,210,16,16,buf); 
		sprintf((char*)buf,"Static IP:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
		LCD_ShowString(30,190,210,16,16,buf); 
	}	
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	delay_init(168);       	//延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);   	//串口波特率设置
	usmart_dev.init(84); 	//初始化USMART
//	LED_Init();  			//LED初始化
	key_init();  			//按键初始化
	LCD_Init(); 			//LCD初始化
	FSMC_SRAM_Init();		//初始化外部SRAM  
	
	my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMEX);		//初始化外部内存池
	my_mem_init(SRAMCCM);	//初始化CCM内存池
	
	POINT_COLOR = RED; 		
	LCD_ShowString(30,30,200,16,16,"Explorer STM32F4");
	LCD_ShowString(30,50,200,16,16,"Ethernet lwIP Test");
	LCD_ShowString(30,70,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,90,200,16,16,"2014/8/15"); 	
	TIM3_Int_Init(999,839); //100hz的频率
	while(lwip_comm_init()) //lwip初始化
	{
		LCD_ShowString(30,110,200,20,16,"LWIP Init Falied!");
		delay_ms(1200);
		LCD_Fill(30,110,230,130,WHITE); //清除显示
		LCD_ShowString(30,110,200,16,16,"Retrying...");  
	}
	LCD_ShowString(30,110,200,20,16,"LWIP Init Success!");
 	LCD_ShowString(30,130,200,16,16,"DHCP IP configing...");  //等待DHCP获取 
#if LWIP_DHCP   //使用DHCP
	while((lwipdev.dhcpstatus!=2)&&(lwipdev.dhcpstatus!=0XFF))//等待DHCP获取成功/超时溢出
	{
		lwip_periodic_handle();	//LWIP内核需要定时处理的函数
	}
#endif
	show_address(lwipdev.dhcpstatus);	//显示地址信息
	while(1)
	{
		lwip_periodic_handle();	//LWIP内核需要定时处理的函数
	}
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
