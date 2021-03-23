#include "uart.h"
 
/*uart2 ���ջ������飬256�ֽ�*/
uint8_t ar_uart1_buffer[MAX_UART_RX_BUFFER];
/*uart2 ���ա��������ݶ�λָ��*/
Uart_pos uart_tmp1 = {INIT_ZERO};
/*uart2 ���ݽ������ʱ����*/
volatile uint8_t g_uart1_tick = INIT_ZERO;
/*uart2 ���ݽ�����ɱ�־λ*/
volatile uint8_t g_uart1_receive_finish = INIT_ZERO;
uint8_t ar_uart1_protocol[MAX_UART_RX_BUFFER];

void uart_init(uint32_t bound)
{
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

}


void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
  {  			 
     ar_uart1_buffer[uart_tmp1.writepos] = USART_ReceiveData(USART1); 	 
     uart_tmp1.writepos++;		
     g_uart1_tick = 10;
     g_uart1_receive_finish = 1;
	}
}


void uart1_receive_handle(void)
{ 
	  uint8_t m_length = INIT_ZERO;
	  uint8_t i;
    /*һ֡���ݽ������*/
    if((g_uart1_receive_finish == 1) && (g_uart1_tick == 0))
    {
			  g_uart1_receive_finish = 0;
			
			   /*�жϽ��ա��������ݵ�ָ���Ƿ�һ�£��������յ������ݱ��浽tmp������*/
				if(uart_tmp1.readpos <= uart_tmp1.writepos)
				{
						m_length = uart_tmp1.writepos - uart_tmp1.readpos;
						memcpy(ar_uart1_protocol, &ar_uart1_buffer[uart_tmp1.readpos], m_length);
				}
				else
				{
						m_length = uart_tmp1.writepos + MAX_UART_RX_BUFFER - uart_tmp1.readpos;
						memcpy(ar_uart1_protocol, &ar_uart1_buffer[uart_tmp1.readpos], MAX_UART_RX_BUFFER - uart_tmp1.readpos);
						memcpy(&ar_uart1_protocol[MAX_UART_RX_BUFFER - uart_tmp1.readpos], ar_uart1_buffer, uart_tmp1.writepos);
				}
				uart_tmp1.readpos = uart_tmp1.writepos;
				
		    for(i=0;i<m_length;i++)
        {
						USART_SendData(USART1, ar_uart1_protocol[i]);         //�򴮿�1��������
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���			
				}
		}
}

