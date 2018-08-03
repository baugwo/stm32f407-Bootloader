#include "stm32f4xx.h"
#include "usb_bsp.h"
#include "ff.h"			/* FatFS文件系统模块*/
#include "usbh_usr.h"
#include "string.h"
extern void bsp_GetRunTimeRun(void);
u32 xxfag=0;
int main()
{
    SystemInit();
	  GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);   //使能端口PORTC时钟                                   
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;					//对应管脚  GPIOx_Pinx0~15  
    GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_2MHz;		//所选管脚的响应速率 
  	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;			//所选管脚的模式：输入IN、输出OUT、复用AF、模拟AN=AIN   
    GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;         //所选管脚的开漏OD、推挽PP  （IN模式下无效）
  	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL;			//所选管脚  上UP/下拉DOWN/无NOPULL
    GPIO_Init(GPIOC, &GPIO_InitStructure);                  //配置引脚
    GPIO_SetBits(GPIOC,GPIO_Pin_2);  
	
		USBH_Init(&USB_OTG_Core,USB_OTG_HS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_cb);
		xxfag=0;	
		while(1)
		{
			USBH_Process(&USB_OTG_Core, &USB_Host);
			if(xxfag)
			{
				
				
				
				xxfag=0;
			}
		}
}
