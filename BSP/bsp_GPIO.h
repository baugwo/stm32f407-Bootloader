/**
  ******************************************************************************
  * @file    bsp_GPIO.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    19-May-2018
  * @brief   Interface of source file bsp_Fram.c     
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* type definitions -------------------------------------------------------------------------*/
/* macro definitions ------------------------------------------------------------------------*/
#define RUN_LED_Pin              GPIO_Pin_2
#define RUN_LED_GPIO_Port        GPIOE

#define RUN_LED_ON()             GPIO_ResetBits(RUN_LED_GPIO_Port, RUN_LED_Pin);
#define RUN_LED_OFF()            GPIO_SetBits(RUN_LED_GPIO_Port, RUN_LED_Pin);
#define RUN_LED_Twinkle()        GPIO_WriteBit(RUN_LED_GPIO_Port , RUN_LED_Pin, \
                                 (BitAction)(1 - GPIO_ReadOutputDataBit(RUN_LED_GPIO_Port, RUN_LED_Pin)))\
																 
																 
#define ALM_LED_PIN              GPIO_Pin_4
#define ALM_LED_GPIO_PORT        GPIOE

#define ALM_LED_ON()             GPIO_ResetBits(ALM_LED_GPIO_PORT, ALM_LED_PIN );
#define ALM_LED_OFF()            GPIO_SetBits(ALM_LED_GPIO_PORT, ALM_LED_PIN );
#define ALM_LED_Twinkle()        GPIO_WriteBit(ALM_LED_GPIO_PORT , ALM_LED_PIN , \
                                 (BitAction)(1 - GPIO_ReadOutputDataBit(ALM_LED_GPIO_PORT, ALM_LED_PIN )))\


#define ALM_AURAL_Pin            GPIO_Pin_3
#define ALM_AURAL_GPIO_Port      GPIOD
#define ALM_VISUAL_Pin           GPIO_Pin_2
#define ALM_VISUAL_GPIO_Port     GPIOD

#define ALM_AURAL_ON()           GPIO_SetBits(ALM_AURAL_GPIO_Port, ALM_AURAL_Pin);
#define ALM_AURAL_OFF()          GPIO_ResetBits(ALM_AURAL_GPIO_Port, ALM_AURAL_Pin);
#define ALM_VISUAL_ON()          GPIO_SetBits(ALM_VISUAL_GPIO_Port, ALM_VISUAL_Pin);
#define ALM_VISUAL_OFF()         GPIO_ResetBits(ALM_VISUAL_GPIO_Port, ALM_VISUAL_Pin);

/* Expanded Variables -----------------------------------------------------------------------*/
/* function declarations --------------------------------------------------------------------*/
void GPIOInit(void);





#endif /* end of _BSP_GPIO_H */





