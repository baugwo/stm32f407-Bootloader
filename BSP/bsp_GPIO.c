/**
  ******************************************************************************
  * @file    bsp_GPIO.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    19-May-2018
  * @brief   this file configs GPIO clock and I/O PIN    
  * @endverbatim
  ******************************************************************************
  */
	
/* Includes -----------------------------------------------------------------------------------------*/
#include "bsp_GPIO.h"
	
/* private type definitions -------------------------------------------------------------------------*/
/* private macro definitions ------------------------------------------------------------------------*/	
/* private variables ---------------------------------------------------------------------------------*/
/* Expanded Variables --------------------------------------------------------------------------------*/
/* private functions prototype -----------------------------------------------------------------------*/
/* function body -------------------------------------------------------------------------------------*/

/**
  *@brief  GPIO init
  *@param  NONE
  *@retval NONE
  *@note   此处打开所有用到的GPIO端口的时钟
  */
void GPIOInit(void)
{
    
    GPIO_InitTypeDef   GPIO_InitStruct;
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);
  
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
        
    GPIO_InitStruct.GPIO_Pin  = RUN_LED_Pin;
    GPIO_Init(RUN_LED_GPIO_Port, &GPIO_InitStruct);
	
	  GPIO_InitStruct.GPIO_Pin  = ALM_LED_PIN;
    GPIO_Init(ALM_LED_GPIO_PORT, &GPIO_InitStruct);
	
	  GPIO_InitStruct.GPIO_Pin  = ALM_AURAL_Pin;
    GPIO_Init(ALM_AURAL_GPIO_Port, &GPIO_InitStruct);
		
		GPIO_InitStruct.GPIO_Pin  = ALM_VISUAL_Pin;
    GPIO_Init(ALM_VISUAL_GPIO_Port, &GPIO_InitStruct);
		
		RUN_LED_OFF();
		ALM_LED_OFF();
		ALM_AURAL_OFF();
		ALM_VISUAL_OFF();  
}


	
/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE****/




