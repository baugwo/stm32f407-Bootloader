#include "stm32f4xx.h"
#include "usb_bsp.h"
#include "ff.h"			/* FatFS�ļ�ϵͳģ��*/
#include "usbh_usr.h"
#include "string.h"
extern void bsp_GetRunTimeRun(void);
u32 xxfag=0;
int main()
{
    SystemInit();
	  GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);   //ʹ�ܶ˿�PORTCʱ��                                   
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;					//��Ӧ�ܽ�  GPIOx_Pinx0~15  
    GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_2MHz;		//��ѡ�ܽŵ���Ӧ���� 
  	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;			//��ѡ�ܽŵ�ģʽ������IN�����OUT������AF��ģ��AN=AIN   
    GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;         //��ѡ�ܽŵĿ�©OD������PP  ��INģʽ����Ч��
  	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL;			//��ѡ�ܽ�  ��UP/����DOWN/��NOPULL
    GPIO_Init(GPIOC, &GPIO_InitStructure);                  //��������
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
