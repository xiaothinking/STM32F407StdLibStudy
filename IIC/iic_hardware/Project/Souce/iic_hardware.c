#include "iic_hardware.h"
#include "uart.h"

uint16_t EEPROM_ADDRESS;

static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;   

static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);


/**
  * @brief  I2C1 I/O����
  * @param  ��
  * @retval ��
  */
static void I2C_GPIO_Config(void)
{

  GPIO_InitTypeDef  GPIO_InitStructure; 
   
  /*!< EEPROM_I2C Periph clock enable */
  EEPROM_I2C_CLK_INIT(EEPROM_I2C_CLK, ENABLE);
  
  /*!< EEPROM_I2C_SCL_GPIO_CLK and EEPROM_I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_AHB1PeriphClockCmd(EEPROM_I2C_SCL_GPIO_CLK | EEPROM_I2C_SDA_GPIO_CLK, ENABLE);

  /*!< GPIO configuration */
  /* Connect PXx to I2C_SCL*/
  GPIO_PinAFConfig(EEPROM_I2C_SCL_GPIO_PORT, EEPROM_I2C_SCL_SOURCE, EEPROM_I2C_SCL_AF);
  /* Connect PXx to I2C_SDA*/
  GPIO_PinAFConfig(EEPROM_I2C_SDA_GPIO_PORT, EEPROM_I2C_SDA_SOURCE, EEPROM_I2C_SDA_AF);  
  
  /*!< Configure EEPROM_I2C pins: SCL */   
  GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure EEPROM_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_PIN;
  GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
 
}

/**
  * @brief  I2C ����ģʽ����
  * @param  ��
  * @retval ��
  */
static void I2C_Mode_Configu(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C ���� */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;	
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;		                    /* �ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ� */
  I2C_InitStructure.I2C_OwnAddress1 =I2C_OWN_ADDRESS7; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;	
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	/* I2C��Ѱַģʽ */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;	                            /* ͨ������ */
  I2C_Init(EEPROM_I2C, &I2C_InitStructure);	                                      /* I2C1 ��ʼ�� */
  I2C_Cmd(EEPROM_I2C, ENABLE);  	                                                /* ʹ�� I2C1 */

  I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);  
}

/**
  * @brief  I2C ����(EEPROM)��ʼ��
  * @param  ��
  * @retval ��
  */
void I2C_EE_Init(void)
{
  I2C_GPIO_Config(); 
 
  I2C_Mode_Configu();


/* ����ͷ�ļ�i2c_ee.h�еĶ�����ѡ��EEPROMҪд��ĵ�ַ */
#ifdef EEPROM_Block0_ADDRESS
  /* ѡ�� EEPROM Block0 ��д�� */
  EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
#endif

#ifdef EEPROM_Block1_ADDRESS  
	/* ѡ�� EEPROM Block1 ��д�� */
  EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
#endif

#ifdef EEPROM_Block2_ADDRESS  
	/* ѡ�� EEPROM Block2 ��д�� */
  EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;
#endif

#ifdef EEPROM_Block3_ADDRESS  
	/* ѡ�� EEPROM Block3 ��д�� */
  EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;
#endif
}

/**
  * @brief   ���������е�����д��I2C EEPROM��
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ
  *     @arg NumByteToWrite:д���ֽ���
  * @retval  ��
  */
void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % I2C_PageSize;
  count = I2C_PageSize - Addr;
  NumOfPage =  NumByteToWrite / I2C_PageSize;
  NumOfSingle = NumByteToWrite % I2C_PageSize;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
}

/**
  * @brief   дһ���ֽڵ�I2C EEPROM��
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ 
  * @retval  ��
  */
uint32_t I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr)
{
  /* Send STRAT condition */
  I2C_GenerateSTART(EEPROM_I2C, ENABLE);

  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
  }    

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
  }    
      
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(EEPROM_I2C, WriteAddr);
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  } 
  /* Send the byte to be written */
  I2C_SendData(EEPROM_I2C, *pBuffer); 
   
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
  } 
  
  /* Send STOP condition */
  I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
  
  return 1;
}

/**
  * @brief   ��EEPROM��һ��дѭ���п���д����ֽڣ���һ��д����ֽ���
  *          ���ܳ���EEPROMҳ�Ĵ�С��AT24C02ÿҳ��8���ֽ�
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ
  *     @arg NumByteToWrite:д���ֽ���
  * @retval  ��
  */
uint32_t I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
{
  I2CTimeout = I2CT_LONG_TIMEOUT;

  while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))  
   {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(4);
  } 
  
  /* Send START condition */
  I2C_GenerateSTART(EEPROM_I2C, ENABLE);
  
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(5);
  } 
  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(6);
  } 
  /* Send the EEPROM's internal address to write to */    
  I2C_SendData(EEPROM_I2C, WriteAddr);  

  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(! I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) 
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(7);
  } 
  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(EEPROM_I2C, *pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 
  
    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(8);
    } 
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
  
  return 1;
}

/**
  * @brief   ��EEPROM�����ȡһ������ 
  * @param   
  *		@arg pBuffer:��Ŵ�EEPROM��ȡ�����ݵĻ�����ָ��
  *		@arg WriteAddr:�������ݵ�EEPROM�ĵ�ַ
  *     @arg NumByteToWrite:Ҫ��EEPROM��ȡ���ֽ���
  * @retval  ��
  */
uint32_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
    I2CTimeout = I2CT_LONG_TIMEOUT;

  //*((uint8_t *)0x4001080c) |=0x80; 
    while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))   
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);
    }
  /* Send START condition */
  I2C_GenerateSTART(EEPROM_I2C, ENABLE);
  //*((uint8_t *)0x4001080c) &=~0x80;
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(10);
   }

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  I2CTimeout = I2CT_FLAG_TIMEOUT;
 
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(11);
   }
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(EEPROM_I2C, ENABLE);

  /* Send the EEPROM's internal address to write to */
  I2C_SendData(EEPROM_I2C, ReadAddr);  

     I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(12);
   }
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(EEPROM_I2C, ENABLE);
  
     I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(13);
   }
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Receiver);
  
     I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(14);
   }
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(EEPROM_I2C, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
    }

		
		I2CTimeout = I2CT_LONG_TIMEOUT;
		while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)  
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
		} 	
		{
		  /* Read a byte from the device */
      *pBuffer = I2C_ReceiveData(EEPROM_I2C);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;
		}			
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);
  
  return 1;
}

/**
  * @brief  Wait for EEPROM Standby state 
  * @param  ��
  * @retval ��
  */
void I2C_EE_WaitEepromStandbyState(void)      
{
  volatile uint16_t SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(EEPROM_I2C, ENABLE);
    /* Read EEPROM_I2C SR1 register */
    SR1_Tmp = I2C_ReadRegister(EEPROM_I2C, I2C_Register_SR1);
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(EEPROM_I2C, I2C_Register_SR1) & 0x0002));
  
  /* Clear AF flag */
  I2C_ClearFlag(EEPROM_I2C, I2C_FLAG_AF);
  /* STOP condition */    
  I2C_GenerateSTOP(EEPROM_I2C, ENABLE); 
}



/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode��������룬����������λ���ĸ����ڳ���.
  * @retval ����0����ʾIIC��ȡʧ��.
  */
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  printf("I2C �ȴ���ʱ!errorCode = %d",errorCode);
  
  return 0;
}
/*********************************************END OF FILE**********************/