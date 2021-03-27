#ifndef __SRAM_H
#define __SRAM_H															    
#include "stm32f4xx.h"


											  
void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t NumHalfwordToRead);

void fsmc_sram_test(void);

#endif

