/**
  ******************************************************************************
  * @file    bsp_Fram.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    19-May-2018
  * @brief   Interface of source file bsp_Fram.c    
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _BSP_FRAM_H
#define _BSP_FRAM_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* type definitions -------------------------------------------------------------------------*/
/* macro definitions ------------------------------------------------------------------------*/
/* Expanded Variables -----------------------------------------------------------------------*/
/* function declarations --------------------------------------------------------------------*/
void Fram_GPIO_init(void);
uint8_t Fram_I2C_Read(uint8_t *ucBuff, uint16_t usAddr, uint16_t usSize);
uint8_t Fram_I2C_Write(uint8_t *ucBuff, uint16_t usAddr, uint16_t usSize);




#endif /* end of _BSP_FRAM_H */




