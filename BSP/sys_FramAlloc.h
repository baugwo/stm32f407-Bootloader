/**
  ******************************************************************************
  * @file    sys_FramAlloc.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    21-May-2018
  * @brief   FM24W256 address allocation     
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _SYS_FRAMALLOC_H
#define _SYS_FRAMALLOC_H

/* Includes ---------------------------------------------------------------------------------*/
/* type definitions -------------------------------------------------------------------------*/
/* macro definitions ------------------------------------------------------------------------*/
#define  FMAddr_InitFlag                 (0x0000)   /*!< 0x55AA, 初次上电时写入，之后上电读取判 */
#define  FMAddr_ATT1_CalData             (0x0002)   /*!< ATT1校准参数存放首地址 */
#define  FMAddr_ATT2_CalData             (0x0078)   /*!< ATT2校准参数存放地址 */
#define  FMAddr_EP_RealTime              (0x0100)   /*!< 母线实时电能存放首地址 */
#define  FMAddr_OptimSet                 (0x0200)   /*!< 设置参数存放首地址,+50为支路开关设置 */
#define  FMAddr_IAP                      (0x0300)   /*!< IAP相关标志存放首地址,+0:初始化标志， +4：复位标志   +8：下载成功标志 */
#define  FMAddr_PastAlm                  (0x0400)   /*!< 历史告警记录存放地址:0-2存放相关信息，4开始存放记录 */
#define  FMAddr_EP_PastTime              (0x1500)   /*!< 历史电能相关数据存放地址 */


/* Expanded Variables -----------------------------------------------------------------------*/
/* function declarations --------------------------------------------------------------------*/

#endif /* end of _BSP_FRAM_H */



