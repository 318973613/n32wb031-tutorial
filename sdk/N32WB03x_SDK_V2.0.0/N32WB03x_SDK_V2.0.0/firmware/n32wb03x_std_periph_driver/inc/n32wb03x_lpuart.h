/**
*     Copyright (c) 2025, NSING Technologies Inc.
* 
*     All rights reserved.
*
*     This software is the exclusive property of NSING Technologies Inc. (Hereinafter 
* referred to as NSING). This software, and the product of NSING described herein 
* (Hereinafter referred to as the Product) are owned by NSING under the laws and treaties
* of the People's Republic of China and other applicable jurisdictions worldwide.
*
*     NSING does not grant any license under its patents, copyrights, trademarks, or other 
* intellectual property rights. Names and brands of third party may be mentioned or referred 
* thereto (if any) for identification purposes only.
*
*     NSING reserves the right to make changes, corrections, enhancements, modifications, and 
* improvements to this software at any time without notice. Please contact NSING and obtain 
* the latest version of this software before placing orders.

*     Although NSING has attempted to provide accurate and reliable information, NSING assumes 
* no responsibility for the accuracy and reliability of this software.
* 
*     It is the responsibility of the user of this software to properly design, program, and test 
* the functionality and safety of any application made of this information and any resulting product. 
* In no event shall NSING be liable for any direct, indirect, incidental, special,exemplary, or 
* consequential damages arising in any way out of the use of this software or the Product.
*
*     NSING Products are neither intended nor warranted for usage in systems or equipment, any
* malfunction or failure of which may cause loss of human life, bodily injury or severe property 
* damage. Such applications are deemed, "Insecure Usage".
*
*     All Insecure Usage shall be made at user's risk. User shall indemnify NSING and hold NSING 
* harmless from and against all claims, costs, damages, and other liabilities, arising from or related 
* to any customer's Insecure Usage.

*     Any express or implied warranty with regard to this software or the Product, including,but not 
* limited to, the warranties of merchantability, fitness for a particular purpose and non-infringement
* are disclaimed to the fullest extent permitted by law.

*     Unless otherwise explicitly permitted by NSING, anyone may not duplicate, modify, transcribe
* or otherwise distribute this software for any purposes, in whole or in part.
*
*     NSING products and technologies shall not be used for or incorporated into any products or systems
* whose manufacture, use, or sale is prohibited under any applicable domestic or foreign laws or regulations. 
* User shall comply with any applicable export control laws and regulations promulgated and administered by 
* the governments of any countries asserting jurisdiction over the parties or transactions.
**/


/**
 * @file n32wb03x_lpuart.h
 * @author NSING Firmware Team
 * @version v1.0.4
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#ifndef __N32WB03X_LPUART_H__
#define __N32WB03X_LPUART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32wb03x.h"

/** @addtogroup N32WB03X_StdPeriph_Driver
 * @{
 */

/** @addtogroup LPUART
 * @{
 */

/** @addtogroup LPUART_Exported_Types
 * @{
 */

/**
 * @brief  LPUART Init Structure definition
 */

typedef struct
{
    uint32_t BaudRate; /*!< This member configures the LPUART communication baud rate.
                                  The baud rate is computed using the following formula:
                                   - IntegerDivider = ((CLK) / (LPUART_InitStruct->BaudRate)))
                                   - FractionalDivider */

    uint16_t Parity; /*!< Specifies the parity mode.
                                This parameter can be a value of @ref Parity
                                @note When parity is enabled, the computed parity is inserted
                                      at the MSB position of the transmitted data (only support
                                      8 data bits). */ 

    uint16_t Mode; /*!< Specifies wether the Receive or Transmit mode is enabled or disabled.
                              This parameter can be a value of @ref Mode */

    uint16_t RtsThreshold; /* Specifies RTS Threshold.
                                    This parameter can be a value of @ref RtsThreshold */                          

    uint16_t HardwareFlowControl; /*!< Specifies wether the hardware flow control mode is enabled
                                             or disabled.
                                             This parameter can be a value of @ref LPUART_Hardware_Flow_Control */
} LPUART_InitType;

/**
 * @}
 */

/** @addtogroup LPUART_Exported_Constants
 * @{
 */

#define IS_LPUART_ALL_PERIPH(PERIPH) ((PERIPH) == LPUART1)

/** @addtogroup Parity
 * @{
 */

#define LPUART_PE_NO             ((uint16_t)0x0008)
#define LPUART_PE_EVEN           ((uint16_t)0x0000)
#define LPUART_PE_ODD            ((uint16_t)0x0001)
#define IS_LPUART_PARITY(PARITY) (((PARITY) == LPUART_PE_NO) || ((PARITY) == LPUART_PE_EVEN) || ((PARITY) == LPUART_PE_ODD))
/**
 * @}
 */

/** @addtogroup Mode
 * @{
 */

#define LPUART_MODE_RX       ((uint16_t)0x0000)
#define LPUART_MODE_TX       ((uint16_t)0x0002)
#define IS_LPUART_MODE(MODE) (((MODE) == LPUART_MODE_RX) || ((MODE) == LPUART_MODE_TX))
/**
 * @}
 */

/** @addtogroup RtsThreshold
 * @{
 */

#define LPUART_RTSTH_FIFOHF       ((uint16_t)0x0000)
#define LPUART_RTSTH_FIFO3QF      ((uint16_t)0x0100)
#define LPUART_RTSTH_FIFOFU       ((uint16_t)0x0200)
#define IS_LPUART_RTSTHRESHOLD(RTSTHRESHOLD)                                                                                \
    (((RTSTHRESHOLD) == LPUART_RTSTH_FIFOHF) || ((RTSTHRESHOLD) == LPUART_RTSTH_FIFO3QF) || ((RTSTHRESHOLD) == LPUART_RTSTH_FIFOFU))
/**
 * @}
 */

/** @addtogroup Hardware_Flow_Control
 * @{
 */
#define LPUART_HFCTRL_NONE    ((uint16_t)0x0000)
#define LPUART_HFCTRL_CTS     ((uint16_t)0x0400)
#define LPUART_HFCTRL_RTS     ((uint16_t)0x0800)
#define LPUART_HFCTRL_RTS_CTS ((uint16_t)0x0C00)
#define IS_LPUART_HARDWARE_FLOW_CONTROL(CONTROL)                                                                        \
    (((CONTROL) == LPUART_HFCTRL_NONE) || ((CONTROL) == LPUART_HFCTRL_RTS) || ((CONTROL) == LPUART_HFCTRL_CTS)          \
     || ((CONTROL) == LPUART_HFCTRL_RTS_CTS))
/**
 * @}
 */

/** @addtogroup LPUART_Interrupt_definition
 * @{
 */
#define LPUART_INT_PE      ((uint16_t)0x0001)
#define LPUART_INT_TXC     ((uint16_t)0x0102)
#define LPUART_INT_FIFO_OV ((uint16_t)0x0204)
#define LPUART_INT_FIFO_FU ((uint16_t)0x0308)
#define LPUART_INT_FIFO_HF ((uint16_t)0x0410)
#define LPUART_INT_FIFO_NE ((uint16_t)0x0520)
#define LPUART_INT_WUF     ((uint16_t)0x0680)

#define IS_LPUART_CFG_INT(IT)                                                                                           \
    (((IT) == LPUART_INT_PE) || ((IT) == LPUART_INT_TXC) || ((IT) == LPUART_INT_FIFO_OV) || ((IT) == LPUART_INT_FIFO_FU)       \
     || ((IT) == LPUART_INT_FIFO_HF) || ((IT) == LPUART_INT_FIFO_NE) || ((IT) == LPUART_INT_WUF))
#define IS_LPUART_GET_INT(IT)                                                                                           \
    (((IT) == LPUART_INT_PE) || ((IT) == LPUART_INT_TXC) || ((IT) == LPUART_INT_FIFO_OV) || ((IT) == LPUART_INT_FIFO_FU)       \
     || ((IT) == LPUART_INT_FIFO_HF) || ((IT) == LPUART_INT_FIFO_NE) || ((IT) == LPUART_INT_WUF))
#define IS_LPUART_CLR_INT(IT)                                                                                           \
    (((IT) == LPUART_INT_PE) || ((IT) == LPUART_INT_TXC) || ((IT) == LPUART_INT_FIFO_OV) || ((IT) == LPUART_INT_FIFO_FU)       \
     || ((IT) == LPUART_INT_FIFO_HF) || ((IT) == LPUART_INT_FIFO_NE) || ((IT) == LPUART_INT_WUF))   
/**
 * @}
 */

/** @addtogroup LPUART_DMA_Requests
 * @{
 */

#define LPUART_DMAREQ_TX         ((uint16_t)0x0020)
#define LPUART_DMAREQ_RX         ((uint16_t)0x0040)
#define IS_LPUART_DMAREQ(DMAREQ) ((((DMAREQ) & (uint16_t)0xFF9F) == (uint16_t)0x00) && ((DMAREQ) != (uint16_t)0x00))

/**
 * @}
 */

/** @addtogroup LPUART_WakeUp_methods
 * @{
 */

#define LPUART_WUSTP_STARTBIT    ((uint16_t)0x0000)
#define LPUART_WUSTP_RXNE        ((uint16_t)0x1000)
#define LPUART_WUSTP_BYTE        ((uint16_t)0x2000)
#define LPUART_WUSTP_FRAME       ((uint16_t)0x3000)
#define IS_LPUART_WAKEUP(WAKEUP)  \
    (((WAKEUP) == LPUART_WUSTP_STARTBIT) || ((WAKEUP) == LPUART_WUSTP_RXNE) || ((WAKEUP) == LPUART_WUSTP_BYTE) || ((WAKEUP) == LPUART_WUSTP_FRAME))
/**
 * @}
 */

/** @addtogroup LPUART_Sampling_methods
 * @{
 */

#define LPUART_SMPCNT_3B    ((uint16_t)0x0000)
#define LPUART_SMPCNT_1B    ((uint16_t)0x4000)
#define IS_LPUART_SAMPLING(SAMPLING) (((SAMPLING) == LPUART_SMPCNT_1B) || ((SAMPLING) == LPUART_SMPCNT_3B))
/**
 * @}
 */

/** @addtogroup LPUART_Flags
 * @{
 */

#define LPUART_FLAG_PEF      ((uint16_t)0x0001)
#define LPUART_FLAG_TXC      ((uint16_t)0x0002)
#define LPUART_FLAG_FIFO_OV  ((uint16_t)0x0004)
#define LPUART_FLAG_FIFO_FU  ((uint16_t)0x0008)
#define LPUART_FLAG_FIFO_HF  ((uint16_t)0x0010)
#define LPUART_FLAG_FIFO_NE  ((uint16_t)0x0020)
#define LPUART_FLAG_CTS      ((uint16_t)0x0040)
#define LPUART_FLAG_WUF      ((uint16_t)0x0080)
#define LPUART_FLAG_NF       ((uint16_t)0x0100)
#define IS_LPUART_FLAG(FLAG)                                                                                                                                            \
    (((FLAG) == LPUART_FLAG_PEF) || ((FLAG) == LPUART_FLAG_TXC) || ((FLAG) == LPUART_FLAG_FIFO_OV)                        \
     || ((FLAG) == LPUART_FLAG_FIFO_FU) || ((FLAG) == LPUART_FLAG_FIFO_HF) || ((FLAG) == LPUART_FLAG_FIFO_NE)    \
     || ((FLAG) == LPUART_FLAG_CTS) || ((FLAG) == LPUART_FLAG_WUF) || ((FLAG) == LPUART_FLAG_NF))

#define IS_LPUART_CLEAR_FLAG(FLAG) ((((FLAG) & (uint16_t)0xFE40) == 0x00) && ((FLAG) != (uint16_t)0x00))

#define IS_LPUART_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) < 2000001))
#define IS_LPUART_DATA(DATA) ((DATA < 0x0100))

/**
 * @}
 */

/**
 * @}
 */

/** @addtogroup LPUART_Exported_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup LPUART_Exported_Functions
 * @{
 */

void LPUART_DeInit(LPUART_Module* LPUARTx);
void LPUART_Init(LPUART_Module* LPUARTx, LPUART_InitType* LPUART_InitStruct);
void LPUART_StructInit(LPUART_InitType* LPUART_InitStruct);
void LPUART_FlushRxFifo(LPUART_Module* LPUARTx);
void LPUART_ConfigInt(LPUART_Module* LPUARTx, uint16_t LPUART_INT, FunctionalState Cmd);
void LPUART_EnableDMA(LPUART_Module* LPUARTx, uint16_t LPUART_DMAReq, FunctionalState Cmd);
void LPUART_ConfigWakeUpMethod(LPUART_Module* LPUARTx, uint16_t LPUART_WakeUpMethod);
void LPUART_EnableWakeUpStop(LPUART_Module* LPUARTx, FunctionalState Cmd);
void LPUART_ConfigSamplingMethod(LPUART_Module* LPUARTx, uint16_t LPUART_SamplingMethod);
void LPUART_EnableLoopBack(LPUART_Module* LPUARTx, FunctionalState Cmd);
void LPUART_SendData(LPUART_Module* LPUARTx, uint8_t Data);
uint8_t LPUART_ReceiveData(LPUART_Module* LPUARTx);
void LPUART_ConfigWakeUpData(LPUART_Module* LPUARTx, uint32_t LPUART_WakeUpData);
FlagStatus LPUART_GetFlagStatus(LPUART_Module* LPUARTx, uint16_t LPUART_FLAG);
void LPUART_ClrFlag(LPUART_Module* LPUARTx, uint16_t LPUART_FLAG);
INTStatus LPUART_GetIntStatus(LPUART_Module* LPUARTx, uint16_t LPUART_INT);
void LPUART_ClrIntPendingBit(LPUART_Module* LPUARTx, uint16_t LPART_INT);

#ifdef __cplusplus
}
#endif

#endif /* __N32WB03X_LPUART_H__ */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
