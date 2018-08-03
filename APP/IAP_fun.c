#include "IAP_config.h"
#include "usb_bsp.h"
#include "ff.h"			/* FatFS文件系统模块*/
#include "usbh_usr.h"
#include "bsp_GPIO.h"
#include "string.h"

static      u8 RunAPPflag=0;
static      u32 RunWait=0;
static      FATFS fs;         /* Work area (filesystem object) for logical drives */
static      FIL fsrc;    /* File objects */
static      BYTE buffer[4096];   /* File copy buffer */
static      FRESULT fr;          /* FatFs function common result code */
static      UINT br;         /* File read/write count */
static      u8 IAP_run_step=0;
void IAP_FlashWritSome(void *buf,u32 len,u32 addr);
void  IAP_Init(void);

		

void IAP_fun_run(void)
{	
	  u16 temp;
	
    GPIOInit();
		IAP_Init();
	
		USBH_Init(&USB_OTG_Core,USB_OTG_HS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_cb);
	  IAP_run_step=0;
		br=0;
		RunAPPflag=0;
		RunWait=0;
		while(1)
		{
			USBH_Process(&USB_OTG_Core, &USB_Host);
			
			RUN_LED_Twinkle();
			
			if(RunAPPflag)
			{		

          ALM_AURAL_ON();
          ALM_VISUAL_ON();
				  ALM_LED_ON();
 				
					__set_PRIMASK(1);
					__disable_irq();          // 关闭所有中断
					NVIC_SystemReset();       // 复位
				  RunWait=0;
				  RunAPPflag=0;
				  IAP_run_step=0;
			}
			else if(RunWait>=0xfffff)
			{						
				
					__set_PRIMASK(1);
					__disable_irq();          // 关闭所有中断
					NVIC_SystemReset();       // 复位
				  RunWait=0;
			}
			else
				RunWait++;
		}
}



u32 filesize=0;
void USB_IAP_APP(void)
{
	RunWait=0;
	switch(IAP_run_step)
	{
			case 0:fr=f_mount(&fs, "0:", 0);IAP_run_step=1;break;//挂载U盘
			case 1:if(fr)IAP_run_step=10;else {IAP_run_step=2;} break;//判断挂载成功
			case 2:fr = f_open(&fsrc, FILE_NAME, FA_READ);IAP_run_step=3;break;//只读打开文件
			case 3:if(fr)IAP_run_step=2;else {IAP_run_step=4;filesize=0;}break;//判断打开成功
			case 4:
				          br=0;
									f_lseek(&fsrc,filesize);
									fr=f_read(&fsrc,buffer,256,&br);
									if(br==256)
									IAP_run_step=5;
									else
									IAP_run_step=7;
									
							break;//读取 文件内容 从br开始，运行结束后返回长度值在br内。
			case 5:if(fr){IAP_run_step=0;}else {IAP_run_step=6;}break;//
			case 6:IAP_FlashWritSome(buffer,256,ApplicationAddressOne+filesize);filesize+=br;IAP_run_step=4; break;//读取成功 写入连续FLASH
			case 7:IAP_FlashWritSome(buffer,br,ApplicationAddressOne+filesize);filesize=0;IAP_run_step=8; break;//读取成功 写入最后FLASH
			case 8:f_close(&fsrc);IAP_run_step=9;break;
			case 9:f_mount(NULL, "0:", 0);IAP_run_step=10;break;
			case 10:
				      RunAPPflag=0xff;
              
			        break;  //运行APP程序
			default :break;
	}
}



//*==============================================CRC===========================================================================*//
const u8 chCRCHTalbe[256] =                                 // CRC 高位字节值表 
{ 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 }; 
 
const u8 chCRCLTalbe[256] =                                 // CRC 低位字节值表 
{ 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE,
	0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 
	0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33,
	0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 
	0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 
	0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6,
	0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78,
	0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76,
	0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94,
	0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 
0x81, 0x80, 0x40  }; 

u16 IAP_CRC16(u8* pchMsg, u32 wDataLen) {    
volatile	u8 chCRCHi = 0xFF; // 高CRC字节初始化 
volatile	u8 chCRCLo = 0xFF; // 低CRC字节初始化 
volatile	u16  wIndex;            // CRC循环中的索引 
  while (wDataLen--)
        {
                // 计算CRC
                wIndex = chCRCLo ^ *pchMsg++ ;
                chCRCLo = chCRCHi ^ chCRCHTalbe[wIndex]; 
                chCRCHi = chCRCLTalbe[wIndex] ;
        }

        return ((chCRCHi << 8) | chCRCLo) ;
}

void IAP_FlashReadSome(void *buf,u32 len,u32 addr)
{
volatile uint32_t i;
volatile u8 *data,*fdata;
	  data=buf;
		fdata=(u8 *)addr;
		for(i=0;i<len;i++)
		data[i]=fdata[i];
}

/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */
/****************************************************************************
* 功    能: 获取扇区编号
* 入口参数：addr：地址
* 出口参数：扇区号
* 说    明：无
* 调用方法：无
****************************************************************************/
uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
  {
    sector = FLASH_Sector_11;  
  }

  return sector;
}

/****************************************************************************
* 功    能: 写入长度为length的16位数据
* 入口参数：addr：地址
leng： 数据长度
buf：要写入的数据指针
* 出口参数：无
* 说    明：无
* 调用方法：无
****************************************************************************/
void IAP_FlashWritSome(void *buf,u32 len,u32 addr)
{
volatile uint32_t i,e_sector;
volatile u16 *data;
const u16 *rdata;
	data=buf;
	len>>=1;
FLASH_Unlock(); //解锁FLASH后才能向FLASH中写数据。
FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);

//是否需要擦除
	rdata=(const u16 *)addr;
		for(i=0;i<len;i++)
			{
				if((rdata[i]&0xffff)!=0xffff)
				{
					e_sector=GetSector(addr);
					FLASH_EraseSector(e_sector,VoltageRange_2);
					break;
				}
			}
		for(i=0; i<len; i++)
		{
			if (FLASH_ProgramHalfWord(addr, data[i]) == FLASH_COMPLETE)   
			{
				 addr = addr + 2;
			}
		}
FLASH_Lock();  //读FLASH不需要FLASH处于解锁状态。
}

void IAP_FlashReadSome(void *buf,u32 len,u32 addr);
typedef void(*pFunction)(void);
void  IAP_Init(void)
{
	
	__IO uint32_t JumpAddress;
	pFunction   Jump_To_Application;
	__set_PRIMASK(1);
	__disable_irq();      // 关闭所有中断
	
		
	if(RCC_GetFlagStatus(RCC_FLAG_SFTRST))//如果下载完成或不是U盘挂载软复位则跳转
	{
			RCC_ClearFlag();//清除复位标志
			if(((*(volatile vu32*)ApplicationAddressOne)&0x1FFE0000)==0x10000000) 
			{ 
					//跳转至用户代码  One
					JumpAddress = *(volatile uint32_t *)(ApplicationAddressOne + 4);  					
					Jump_To_Application = (pFunction)JumpAddress;  
				 //初始化用户程序的堆栈指针  
					__set_MSP(*(volatile uint32_t*)ApplicationAddressOne); 
					Jump_To_Application();//执行 用户程序 						
			}	
	}
	__set_PRIMASK(0);
	__enable_irq();      // 打开所有中断
}


