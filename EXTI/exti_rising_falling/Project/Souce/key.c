#include "key.h"

void key_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* GPIOG Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOA, ENABLE);

  /* Configure PG6 and PG8 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 |GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

uint8_t key_scan(void)
{
	uint8_t key=255;
	if(0==GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2))
    {
			  delay_ms(10);
				if(0==GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2))
          {						
						key=2; 
					}
		}
  else if(0==GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3))
    {
			  delay_ms(10);
				if(0==GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3))
          {						
						key=1; 
					}
		}
	else if(0==GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4))
    {
			  delay_ms(10);
				if(0==GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4))
          {						
						key=0; 
					}
		}
	else if(1==GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
    {
			  delay_ms(10);
				if(1==GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
          {						
						key=3; 
					}
		}
	return key;
}

