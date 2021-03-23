#include "dma.h"
 
uint16_t rx_len = 0;                          //DMA接收数据的长度
uint8_t SendBuffer[256]={0};                   //DMA发送缓冲区
uint8_t ReceiveBuffer[256]={0};                //DMA接收缓冲区
uint16_t BufferLen = 256;                      //DMA缓冲区的长度
uint8_t rx_flag = 0;                         //数据帧接收标志

GPIO_InitTypeDef GPIO_InitStructure;      //定义IO初始化结构体
USART_InitTypeDef USART_InitStructure;    //定义串口结构体
DMA_InitTypeDef DMA_InitStructure;        //定义DMA结构体
NVIC_InitTypeDef NVIC_InitStructure;      //定义中断结构体

/**************************************
@功能              初始化时钟配置
@调用关系           无
@输入参数           无
@返回值                无
@说明            使能GPIOA，USART1，DMA2
***************************************/
void dma_user_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);          //DMA2时钟使能 
}

/**************************************
@功能              DMA的发送初始化配置
@调用关系          无
@输入参数            
    @Usartx             指定的串口，如 USART1
    @Par                DMA外设地址，如 (uint32_t)&USART1->DR
    @DMA_Streamx_tx     代表DMA1的发送数据流，DMA1_Stream0~DMA1_Stream7
    @DMA_Chx            代表DMA的通道选择，DMA_Channel_0~DMA_Channel_7
    @Tx_Buf             代表DMA的发送缓冲区
    @Tx_Size            代表发送数据个数
    @DMA_Streamx_Irqn   DMA数据流的中断，如 DMA2_Stream7_IRQn
@返回值                无
@说明            无
***************************************/
void Init_Dma_Tx_Config(USART_TypeDef *Usartx,uint32_t Par, DMA_Stream_TypeDef *DMA_Streamx_Tx, 
                        uint32_t DMA_Chx,uint32_t Tx_Buf, uint16_t Tx_Size, uint32_t DMA_Streamx_Irqn)
{
    DMA_DeInit(DMA_Streamx_Tx);                                                //?DMA???????????? 
    while (DMA_GetCmdStatus(DMA_Streamx_Tx) != DISABLE) ;                      //??DMA?????????                      

    //DMA的结构体设置
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

    //DMA 中断设置    
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
@功能              DMA的接受初始化配置
@调用关系           无
@输入参数           
    @Usartx             指定的串口，如USART1
    @Par                DMA外设地址,如 (uint32_t)&USART1->DR
    @DMA_Streamx_rx     代表DMA1的接收数据流,DMA1_Stream0~DMA1_Stream7
    @DMA_Chx            代表DMA的通道选择,DMA_Channel_0~DMA_Channel_7
    @Rx_Buf             代表DMA的接收缓冲区
    @Rx_Size            代表接收数据个数
@返回值                无
@说明            无
***************************************/
void Init_Dma_Rx_Config(USART_TypeDef *Usartx, uint32_t Par, DMA_Stream_TypeDef *DMA_Streamx_Rx, 
                        uint32_t DMA_Chx,uint32_t Rx_Buf, uint16_t Rx_Size)
{
    DMA_DeInit(DMA_Streamx_Rx);                                              //?DMA????????????
    while (DMA_GetCmdStatus(DMA_Streamx_Rx) != DISABLE);                     //??DMA????????? 

    //DMA的结构体设置 
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
@功能              DMA的发送使能
@调用关系           无
@输入参数            
    @DMA_Streamx_tx     代表DMA的发送数据流,DMA1_Stream0~DMA1_Stream7 / DMA2_Stream0~DMA2_Stream7
    @ndtr               代表DMA的发送数据个数
@返回值               无
@说明            无
***************************************/
void Dma_Send_Enable(DMA_Stream_TypeDef *DMA_Streamx_tx,uint16_t ndtr)
{
    DMA_Cmd(DMA_Streamx_tx, DISABLE);                         //??DMA??     
    while (DMA_GetCmdStatus(DMA_Streamx_tx) != DISABLE);        //??DMA?????        
    DMA_SetCurrDataCounter(DMA_Streamx_tx,ndtr);              //?????   
    DMA_Cmd(DMA_Streamx_tx, ENABLE);                          //??DMA?? 
}

/**************************************
@功能             DMA的发送完成中断函数
@调用关系           无
@输入参数           无
@返回值           无
@说明             无
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
@功能              串口1的空闲中断函数
@调用关系          无
@输入参数           无
@返回值           无
@说明             无
***************************************/
void USART1_IRQHandler(void)  
{  
    uint16_t i;  
    if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)            //???????? 
    {
        DMA_Cmd(DMA2_Stream5, DISABLE);                             //??DMA,?????????  

        //先读SR,再读DR,是为了清除IDLE中断
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

        //清除DMA2_Steam5传输完成标志 
        DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5); 
        DMA_SetCurrDataCounter(DMA2_Stream5, BufferLen);            //??DMA???????
        DMA_Cmd(DMA2_Stream5, ENABLE);                              //??DMA 

    }  
}   





