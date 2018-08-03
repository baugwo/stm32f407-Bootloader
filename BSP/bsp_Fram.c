/**
  ******************************************************************************
  * @file    bsp_Fram.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    19-May-2018
  * @brief   soft I2C and FM24W256 options   
  * @endverbatim
  ******************************************************************************
  */
	
/* Includes -----------------------------------------------------------------------------------------*/
#include "bsp_Fram.h"

/* private type definitions -------------------------------------------------------------------------*/
/* private macro definitions ------------------------------------------------------------------------*/	
#define FRAM_SCL_Pin                 GPIO_Pin_6
#define FRAM_SCL_GPIO_Port           GPIOB
#define FRAM_SDA_Pin                 GPIO_Pin_7
#define FRAM_SDA_GPIO_Port           GPIOB	

#define FRAM_SCL_HIGH()              GPIO_SetBits(FRAM_SCL_GPIO_Port,FRAM_SCL_Pin)
#define FRAM_SCL_LOW()               GPIO_ResetBits(FRAM_SCL_GPIO_Port,FRAM_SCL_Pin)	
#define FRAM_SDA_HIGH()              GPIO_SetBits(FRAM_SDA_GPIO_Port,FRAM_SDA_Pin)
#define FRAM_SDA_LOW()               GPIO_ResetBits(FRAM_SDA_GPIO_Port,FRAM_SDA_Pin)
#define FRAM_I2C_READ()              GPIO_ReadInputDataBit(FRAM_SDA_GPIO_Port, FRAM_SDA_Pin)

#define FRAM_DEV_ADDR                (0xA0)  /*!< 设备地址 */
#define FRAM_I2C_WR	                 (0)		 /*!< 写控制bit */
#define FRAM_I2C_RD	                 (1)		 /*!< 读控制bit */

/* private variables ---------------------------------------------------------------------------------*/
/* Expanded Variables --------------------------------------------------------------------------------*/
/* private functions prototype -----------------------------------------------------------------------*/
static void Fram_I2C_Stop(void);
/* function body -------------------------------------------------------------------------------------*/

/**
  *@breif  Fram GPIO init
  *@param  NONE
  *@retval NONE
  *@note   
  */
void Fram_GPIO_init(void)
{
   GPIO_InitTypeDef   GPIO_InitStruct;
	 
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	    
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	 GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	 GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
   
   GPIO_InitStruct.GPIO_Pin = FRAM_SCL_Pin;
   GPIO_Init(FRAM_SCL_GPIO_Port, &GPIO_InitStruct);
    
   GPIO_InitStruct.GPIO_Pin = FRAM_SDA_Pin;
   GPIO_Init(FRAM_SDA_GPIO_Port, &GPIO_InitStruct);
	
	 Fram_I2C_Stop();
}


/**
  *@breif  Fram I2C delay
  *@param  NONE
  *@retval NONE
  *@note   
  */
static void Fram_I2C_Delay(void)
{
   uint8_t i;
   
   for(i = 0; i<80;i++)
	 {;}
}


/**
  *@brief  Fram I2C start
  *@param  NONE
  *@retval NONE
  *@note  当SCL高电平时，SDA出现一个下降沿表示I2C总线启动信号  
  */
static void Fram_I2C_Start(void)
{
   FRAM_SDA_HIGH();
	 FRAM_SCL_HIGH();
	 Fram_I2C_Delay();
	 FRAM_SDA_LOW();
	 Fram_I2C_Delay();
	 FRAM_SCL_LOW();
	 Fram_I2C_Delay();
}

/**
  *@breif  Fram I2C stop
  *@param  NONE
  *@retval NONE
  *@note   当SCL高电平时，SDA出现一个上升沿表示I2C总线停止信号
  */
static void Fram_I2C_Stop(void)
{
   FRAM_SDA_LOW();
   FRAM_SCL_HIGH();
   Fram_I2C_Delay();
   FRAM_SDA_HIGH();
}


/**
  *@brief CPU send 8bits data to I2C
  *@param Byte: data to send
  *@retval NONE
  *@note   NONE
  */
static void Fram_I2C_SendByte(uint8_t ucByte)
{
   uint8_t i;
	
	 for(i=0; i<8; i++)
	 {
		  if(ucByte & 0x80)
			{
			    FRAM_SDA_HIGH();
			}
	    else
			{
			    FRAM_SDA_LOW();
			}
      Fram_I2C_Delay();
      FRAM_SCL_HIGH();
      Fram_I2C_Delay();
      FRAM_SCL_LOW();
      if(7 == i)
			{
			   FRAM_SDA_HIGH();   //释放总线
			}				
			ucByte <<= 1; //左移1个bit
			Fram_I2C_Delay();		
	 } 
}

/**
  *@brief  CPU read 8bits from I2C
  *@param  NONE
  *@retval NONE
  *@note 
  */
static uint8_t Fram_I2C_ReadByte(void)
{
   uint8_t i;
	 uint8_t ucValue = 0;
	
	 for(i = 0; i < 8; i++ )
	 {
	    ucValue <<= 1;
		  FRAM_SCL_HIGH();
		  Fram_I2C_Delay();
		  if(FRAM_I2C_READ())
			{
			   ucValue++;
			}
			FRAM_SCL_LOW();
			Fram_I2C_Delay();
	 }
	 return ucValue;
}

/**
  *@brief CPU gives a clock and get ack from device
  *@param NONE
  *@retval 0--NO ACK, 0--Right ACK
  *@note  释放总线后，SCL高电平时，器件应答
  */
static uint8_t Fram_I2C_WaitAck(void)
{
	 uint8_t ucAck;
	
	 FRAM_SDA_HIGH(); //CPU释放中线
	 Fram_I2C_Delay();
	 FRAM_SCL_HIGH(); 
	 Fram_I2C_Delay();
	 if(FRAM_I2C_READ())
	 {
	    ucAck = 1;
	 }
	 else
	 {
	    ucAck = 0;
	 }
	 FRAM_SCL_LOW();
	 Fram_I2C_Delay();
	 
	 return ucAck;
}


/**
  *@brief CPU gives a acknomwledge signal
  *@param NONE
  *@retval NONE
  *@note 
  */
static void Fram_I2C_Ack(void)
{
   FRAM_SDA_LOW();
	 Fram_I2C_Delay();
	 FRAM_SCL_HIGH();
	 Fram_I2C_Delay();
	 FRAM_SCL_LOW();
	 Fram_I2C_Delay();
	 FRAM_SDA_HIGH(); 
}


/**
  *@brief  CPU gives a no acknowledge signal
  *@param  NONE
  *@retval NONE
  *@note  device 发送完最后一个字节后，CPU通知device 不用应答
  */
static void Fram_I2C_NAck(void)
{
   FRAM_SDA_HIGH();
	 Fram_I2C_Delay();
	 FRAM_SCL_HIGH();
	 Fram_I2C_Delay();
	 FRAM_SCL_LOW();
	 Fram_I2C_Delay();
}




/**
  *@brief  read datas from Fram in Seleective Read mode
  *@param  *ucBuff--buffer for read data
  *        usAddr --start Address
  *        usSize -- data length
  *@retval 0--fail , 1--suceed
  *@note   NONE
  */
uint8_t Fram_I2C_Read(uint8_t *ucBuff, uint16_t usAddr, uint16_t usSize)
{
    uint16_t i;

    /*发送写指令，写入地址*/	
	  Fram_I2C_Start();
	
	  Fram_I2C_SendByte(FRAM_DEV_ADDR | FRAM_I2C_WR);
	  if(Fram_I2C_WaitAck() != 0)
		{
		   goto cmd_fail;
		}
		
		Fram_I2C_SendByte((uint8_t)(usAddr>>8));
		if(Fram_I2C_WaitAck() != 0)
		{
		    goto cmd_fail;
		}
		
		Fram_I2C_SendByte((uint8_t)usAddr);
		if(Fram_I2C_WaitAck() != 0)
		{
		    goto cmd_fail;
		}
		
		
		 /*发送读指令，读取从写入地址开始的数据*/
		Fram_I2C_Start();
		
		Fram_I2C_SendByte(FRAM_DEV_ADDR | FRAM_I2C_RD);
		if(Fram_I2C_WaitAck() != 0)
		{
		    goto cmd_fail;
		}
		
		for(i = 0; i < usSize; i++)
		{
		   ucBuff[i] = Fram_I2C_ReadByte();
			/*非最后一个字节读完后，CPU产生ACK信息号（SDA = 0）,
			  最后一个字节读完后，CPU产生NACK信号（SDA = ） 
			 */
			 if( i!= usSize - 1)
			 {
			    Fram_I2C_Ack(); 
			 }
			 else
			 {
			    Fram_I2C_NAck(); 
			 }
		}
		Fram_I2C_Stop();
		return 1;
 
  cmd_fail:
    Fram_I2C_Stop();
		
    return 0;				
}


/**
  *@brief  Write datas to I2C device in Multiple Byte Write mode
  *@param  *ucBuff--buffer for read data
  *        usAddr --start Address
  *        usSize -- data length
  *@retval 0--fail , 1--suceed
  *@note   NONE 
  */
uint8_t Fram_I2C_Write(uint8_t *ucBuff, uint16_t usAddr, uint16_t usSize)
{
    uint16_t i;		
		
	  /* 发送写命令，写入地址 */
	  Fram_I2C_Start();
	  Fram_I2C_SendByte(FRAM_DEV_ADDR | FRAM_I2C_WR);
	  if(Fram_I2C_WaitAck() != 0)
		{
		    goto cmd_fail;
		}
		Fram_I2C_SendByte((uint8_t)(usAddr >> 8));
		if(Fram_I2C_WaitAck() != 0)
		{
		    goto cmd_fail;
		}
		Fram_I2C_SendByte((uint8_t)usAddr);
		if(Fram_I2C_WaitAck() != 0)
		{
		    goto cmd_fail;
		}
		/*写入数据*/
	  for(i=0; i<usSize; i++)
    {
		    Fram_I2C_SendByte(ucBuff[i]);
			  if(Fram_I2C_WaitAck() != 0)
		    {
		       goto cmd_fail;
		    }
		}
    Fram_I2C_Stop();
		return 1;

  cmd_fail:
    Fram_I2C_Stop();	

		return 0;
}


/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE****/
		
		
		


		
		
		