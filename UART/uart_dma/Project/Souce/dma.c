#include "dma.h"
 
uint16_t rx_len = 0;                          //DMA�������ݵĳ���
uint8_t SendBuffer[256]={0};                   //DMA���ͻ�����
uint8_t ReceiveBuffer[256]={0};                //DMA���ջ�����
uint16_t BufferLen = 256;                      //DMA�������ĳ���
uint8_t rx_flag = 0;                         //����֡���ձ�־

GPIO_InitTypeDef GPIO_InitStructure;      //����IO��ʼ���ṹ��
USART_InitTypeDef USART_InitStructure;    //���崮�ڽṹ��
DMA_InitTypeDef DMA_InitStructure;        //����DMA�ṹ��
NVIC_InitTypeDef NVIC_InitStructure;      //�����жϽṹ��

/**************************************
@����              ��ʼ��ʱ������
@���ù�ϵ           ��
@�������           ��
@����ֵ                ��
@˵��            ʹ��GPIOA��USART1��DMA2
***************************************/
void dma_user_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);          //DMA2ʱ��ʹ�� 
}

/**************************************
@����              DMA�ķ��ͳ�ʼ������
@���ù�ϵ          ��
@�������            
    @Usartx             ָ���Ĵ��ڣ��� USART1
    @Par                DMA�����ַ���� (uint32_t)&USART1->DR
    @DMA_Streamx_tx     ����DMA1�ķ�����������DMA1_Stream0~DMA1_Stream7
    @DMA_Chx            ����DMA��ͨ��ѡ��DMA_Channel_0~DMA_Channel_7
    @Tx_Buf             ����DMA�ķ��ͻ�����
    @Tx_Size            ���������ݸ���
    @DMA_Streamx_Irqn   DMA���������жϣ��� DMA2_Stream7_IRQn
@����ֵ                ��
@˵��            ��
***************************************/
void Init_Dma_Tx_Config(USART_TypeDef *Usartx,uint32_t Par, DMA_Stream_TypeDef *DMA_Streamx_Tx, 
                        uint32_t DMA_Chx,uint32_t Tx_Buf, uint16_t Tx_Size, uint32_t DMA_Streamx_Irqn)
{
    DMA_DeInit(DMA_Streamx_Tx);                                                //?DMA???????????? 
    while (DMA_GetCmdStatus(DMA_Streamx_Tx) != DISABLE) ;                      //??DMA?????????                      

    //DMA�Ľṹ������
    DMA_InitStructure.DMA_Channel = DMA_Chx;                                   //????  
    DMA_InitStructure.DMA_PeripheralBaseAddr = Par;                            //DMA????  
    DMA_InitStructure.DMA_Memory0BaseAddr = Tx_Buf;                            //DMA ???0??  
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                    //????????  
    DMA_InitStructure.DMA_BufferSize = Tx_Size;                                //?????   
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;           //???????  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                    //???????  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    //??????:8?  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;            //???????:8?  
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                              // ??????   
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                      //?????  
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                     //???DMA_FIFOMode 
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;              //FIFO ?????
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                //?????????  
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;        //????????  
    DMA_Init(DMA_Streamx_Tx, &DMA_InitStructure);                              //???DMA Stream  

    //DMA �ж�����    
    NVIC_InitStructure.NVIC_IRQChannel = DMA_Streamx_Irqn;                     //DMA?????????
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                  //?????0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                         //????0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                            //IRQ???? 
    NVIC_Init(&NVIC_InitStructure);                                            //??????????NVIC???

    USART_DMACmd(Usartx,USART_DMAReq_Tx,ENABLE);                               //????1?DMA??    
    DMA_Cmd(DMA_Streamx_Tx, DISABLE);                                          //??DMA?? 
    DMA_ITConfig(DMA_Streamx_Tx,DMA_IT_TC,ENABLE);                             //??DMA????????
}

/**************************************
@����              DMA�Ľ��ܳ�ʼ������
@���ù�ϵ           ��
@�������           
    @Usartx             ָ���Ĵ��ڣ���USART1
    @Par                DMA�����ַ,�� (uint32_t)&USART1->DR
    @DMA_Streamx_rx     ����DMA1�Ľ���������,DMA1_Stream0~DMA1_Stream7
    @DMA_Chx            ����DMA��ͨ��ѡ��,DMA_Channel_0~DMA_Channel_7
    @Rx_Buf             ����DMA�Ľ��ջ�����
    @Rx_Size            ����������ݸ���
@����ֵ                ��
@˵��            ��
***************************************/
void Init_Dma_Rx_Config(USART_TypeDef *Usartx, uint32_t Par, DMA_Stream_TypeDef *DMA_Streamx_Rx, 
                        uint32_t DMA_Chx,uint32_t Rx_Buf, uint16_t Rx_Size)
{
    DMA_DeInit(DMA_Streamx_Rx);                                              //?DMA????????????
    while (DMA_GetCmdStatus(DMA_Streamx_Rx) != DISABLE);                     //??DMA????????? 

    //DMA�Ľṹ������ 
    DMA_InitStructure.DMA_Channel = DMA_Chx;                                 //????  
    DMA_InitStructure.DMA_PeripheralBaseAddr = Par;                          //DMA????  
    DMA_InitStructure.DMA_Memory0BaseAddr = Rx_Buf;                          //DMA ???0??  
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;                 //????????  
    DMA_InitStructure.DMA_BufferSize = Rx_Size;                              //?????   
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;         //???????  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                  //???????  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //??????:8?  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;          //???????:8?  
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                            // ??????   
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                    //?????  
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                   //???DMA_FIFOMode 
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;            //FIFO ?????
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;              //?????????  
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;      //????????  
    DMA_Init(DMA_Streamx_Rx, &DMA_InitStructure);                            //???DMA Stream  

    USART_DMACmd(Usartx,USART_DMAReq_Rx,ENABLE);                             //?????DMA??   
    DMA_Cmd(DMA_Streamx_Rx, ENABLE);                                         //??DMA??    
}

/**************************************
@����              DMA�ķ���ʹ��
@���ù�ϵ           ��
@�������            
    @DMA_Streamx_tx     ����DMA�ķ���������,DMA1_Stream0~DMA1_Stream7 / DMA2_Stream0~DMA2_Stream7
    @ndtr               ����DMA�ķ������ݸ���
@����ֵ               ��
@˵��            ��
***************************************/
void Dma_Send_Enable(DMA_Stream_TypeDef *DMA_Streamx_tx,uint16_t ndtr)
{
    DMA_Cmd(DMA_Streamx_tx, DISABLE);                         //??DMA??     
    while (DMA_GetCmdStatus(DMA_Streamx_tx) != DISABLE);        //??DMA?????        
    DMA_SetCurrDataCounter(DMA_Streamx_tx,ndtr);              //?????   
    DMA_Cmd(DMA_Streamx_tx, ENABLE);                          //??DMA?? 
}

/**************************************
@����             DMA�ķ�������жϺ���
@���ù�ϵ           ��
@�������           ��
@����ֵ           ��
@˵��             ��
***************************************/

void DMA2_Stream7_IRQHandler(void)  
{
    if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)     //??DMA2_Steam7????  
    { 
        DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);           //??DMA2_Steam7?????? 
        DMA_Cmd(DMA2_Stream7,DISABLE);                        //??DMA           
        DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);    //???????     
    }  
}  

/**************************************
@����              ����1�Ŀ����жϺ���
@���ù�ϵ          ��
@�������           ��
@����ֵ           ��
@˵��             ��
***************************************/
void USART1_IRQHandler(void)  
{  
    uint16_t i;  
    if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)            //???????? 
    {
        DMA_Cmd(DMA2_Stream5, DISABLE);                             //??DMA,?????????  

        //�ȶ�SR,�ٶ�DR,��Ϊ�����IDLE�ж�
        i = USART1->SR;                                             //?SR???
        i = USART1->DR;                                             //?DR???

        rx_len = BufferLen - DMA_GetCurrDataCounter(DMA2_Stream5);  //??????????
//        if(rx_len !=0)                                              //?????????0
//        { 
            rx_flag = 1;                                       //????????1
//            for(i=0;i<rx_len;i++)
//            {
//                SendBuffer[i]= ReceiveBuffer[i];                    //????????
//            }
//            for(i=0;i<rx_len;i++)                                   //????????
//            {
//                ReceiveBuffer[i]=0;
//            }
//        }  

        //���DMA2_Steam5������ɱ�־ 
        DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5); 
        DMA_SetCurrDataCounter(DMA2_Stream5, BufferLen);            //??DMA???????
        DMA_Cmd(DMA2_Stream5, ENABLE);                              //??DMA 

    }  
}   





