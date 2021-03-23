#ifndef _IIC_HARDWARE_H_
#define _IIC_HARDWARE_H_

#include "stm32f4xx.h"



/* AT24C01/02ÿҳ��8���ֽ� */
#define I2C_PageSize           8

/* AT24C04/08A/16Aÿҳ��16���ֽ� */
//#define I2C_PageSize           16			



/* STM32 I2C ����ģʽ */
#define I2C_Speed              400000

/* �����ַֻҪ��STM32��ҵ�I2C������ַ��һ������ */
#define I2C_OWN_ADDRESS7      0XA0   

/*I2C�ӿ�*/
#define EEPROM_I2C                          I2C1
#define EEPROM_I2C_CLK                      RCC_APB1Periph_I2C1
#define EEPROM_I2C_CLK_INIT								RCC_APB1PeriphClockCmd

#define EEPROM_I2C_SCL_PIN                  GPIO_Pin_8                 
#define EEPROM_I2C_SCL_GPIO_PORT            GPIOB                       
#define EEPROM_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define EEPROM_I2C_SCL_SOURCE               GPIO_PinSource8
#define EEPROM_I2C_SCL_AF                   GPIO_AF_I2C1

#define EEPROM_I2C_SDA_PIN                  GPIO_Pin_9                  
#define EEPROM_I2C_SDA_GPIO_PORT            GPIOB                       
#define EEPROM_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define EEPROM_I2C_SDA_SOURCE               GPIO_PinSource9
#define EEPROM_I2C_SDA_AF                   GPIO_AF_I2C1

/*�ȴ���ʱʱ��*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))



/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

/* EEPROM Addresses defines */
#define EEPROM_Block0_ADDRESS 0xA0   /* E2 = 0 */
//#define EEPROM_Block1_ADDRESS 0xA2 /* E2 = 0 */
//#define EEPROM_Block2_ADDRESS 0xA4 /* E2 = 0 */
//#define EEPROM_Block3_ADDRESS 0xA6 /* E2 = 0 */



void I2C_EE_Init(void);
void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
uint32_t I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr);
uint32_t I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite);
uint32_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
void I2C_EE_WaitEepromStandbyState(void);



#endif

