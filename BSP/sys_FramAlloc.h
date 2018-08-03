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
#define  FMAddr_InitFlag                 (0x0000)   /*!< 0x55AA, �����ϵ�ʱд�룬֮���ϵ��ȡ�� */
#define  FMAddr_ATT1_CalData             (0x0002)   /*!< ATT1У׼��������׵�ַ */
#define  FMAddr_ATT2_CalData             (0x0078)   /*!< ATT2У׼������ŵ�ַ */
#define  FMAddr_EP_RealTime              (0x0100)   /*!< ĸ��ʵʱ���ܴ���׵�ַ */
#define  FMAddr_OptimSet                 (0x0200)   /*!< ���ò�������׵�ַ,+50Ϊ֧·�������� */
#define  FMAddr_IAP                      (0x0300)   /*!< IAP��ر�־����׵�ַ,+0:��ʼ����־�� +4����λ��־   +8�����سɹ���־ */
#define  FMAddr_PastAlm                  (0x0400)   /*!< ��ʷ�澯��¼��ŵ�ַ:0-2��������Ϣ��4��ʼ��ż�¼ */
#define  FMAddr_EP_PastTime              (0x1500)   /*!< ��ʷ����������ݴ�ŵ�ַ */


/* Expanded Variables -----------------------------------------------------------------------*/
/* function declarations --------------------------------------------------------------------*/

#endif /* end of _BSP_FRAM_H */



