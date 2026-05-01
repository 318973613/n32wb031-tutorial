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
 * @file main.c
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include <stdio.h>
#include "main.h"
#include "log.h"

/** @addtogroup N32WB03X_StdPeriph_Examples
 * @{
 */

/** @addtogroup LPUART_DMA_Interrupt
 * @{
 */

#define TxBufferSize1 (countof(TxBuffer1))
#define TxBufferSize2 (countof(TxBuffer2))

#define countof(a) (sizeof(a) / sizeof(*(a)))

LPUART_InitType LPUART_InitStructure;
USART_InitType USART_InitStructure;
uint8_t TxBuffer1[] = "LPUARTy DMA Interrupt: LPUARTy -> USARTz using DMA Tx and Rx Flag";
uint8_t TxBuffer2[] = "LPUARTy DMA Interrupt: USARTz -> LPUARTy using DMA Tx and Rx Interrupt";
uint8_t RxBuffer1[TxBufferSize2];
uint8_t RxBuffer2[TxBufferSize1];
__IO uint8_t RxCounter              = 0x00;
uint8_t NbrOfDataToRead             = TxBufferSize2;
uint32_t indexFlag                  = 0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

/**
 * @brief  Main program
 */
int main(void)
{  
    log_init();
    printf("\n This is lpuart dma interrupt demo\r\n");
    
    /* System Clocks Configuration */
    RCC_Configuration(RCC_LPUARTCLK_SRC_APB1);

    /* NVIC configuration */
    NVIC_Configuration();

    /* Configure the GPIO ports */
    GPIO_Configuration();

    /* Configure the DMA */
    DMA_Configuration();

    /* LPUARTy and USARTz configuration ------------------------------------------------------*/

    USART_DeInit(USARTz);
    USART_InitStructure.BaudRate            = 9600;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
    /* Configure USARTz */
    USART_Init(USARTz, &USART_InitStructure);
    /* Enable the USARTz */
    USART_Enable(USARTz, ENABLE);

    Delay(0x2FFFF);
    
    LPUART_DeInit(LPUARTy);
    LPUART_InitStructure.BaudRate            = 9600;
    LPUART_InitStructure.Parity              = LPUART_PE_NO;
    LPUART_InitStructure.RtsThreshold        = LPUART_RTSTH_FIFOFU;
    LPUART_InitStructure.HardwareFlowControl = LPUART_HFCTRL_NONE;
    LPUART_InitStructure.Mode                = LPUART_MODE_RX | LPUART_MODE_TX;
    /* Configure LPUARTy */
    LPUART_Init(LPUARTy, &LPUART_InitStructure);

    /* Flush LPUARTy RX FIFO */
    LPUART_FlushRxFifo(LPUARTy);

    /* Enable LPUARTy DMA Rx and TX request */
    LPUART_EnableDMA(LPUARTy, LPUART_DMAREQ_TX, ENABLE);
    /* Enable USARTz DMA Rx and TX request */
    USART_EnableDMA(USARTz, USART_DMAREQ_TX, ENABLE);

    /* Enable the LPUARTy Receive Interrupt */
    LPUART_ConfigInt(LPUARTy, LPUART_INT_FIFO_NE, ENABLE);

    /* Enable LPUARTy TX DMA Channel */
    DMA_EnableChannel(LPUARTy_Tx_DMA_Channel, ENABLE);
    /* Enable USARTz TX DMA Channel */
    DMA_EnableChannel(USARTz_Tx_DMA_Channel, ENABLE);   

    /* Receive the TxBuffer2 */
    while (indexFlag < TxBufferSize1)
    {
        while (USART_GetFlagStatus(USARTz, USART_FLAG_RXDNE) == RESET)
        {
        }
        RxBuffer2[indexFlag++] = USART_ReceiveData(USARTz);
    }

    /* Wait until USARTz TX DMA Channel Transfer Complete */
    while (DMA_GetFlagStatus(USARTz_Tx_DMA_FLAG, DMA) == RESET)
    {
    }
    
    /* Wait until end of transmission from USARTz to LPUARTy */
    while (RxCounter < TxBufferSize2)
    {
       
    }  

    /* Check the received data with the send ones */
    TransferStatus1 = Buffercmp(TxBuffer2, RxBuffer1, TxBufferSize2);
    /* TransferStatus1 = PASSED, if the data transmitted from USARTz and
       received by LPUARTy are the same */
    /* TransferStatus1 = FAILED, if the data transmitted from USARTz and
       received by LPUARTy are different */
    TransferStatus2 = Buffercmp(TxBuffer1, RxBuffer2, TxBufferSize1);
    /* TransferStatus2 = PASSED, if the data transmitted from LPUARTy and
       received by USARTz are the same */
    /* TransferStatus2 = FAILED, if the data transmitted from LPUARTy and
       received by USARTz are different */
    
    printf("test result TransferStatus1:%d, TransferStatus2:%d\r\n", TransferStatus1, TransferStatus2);
    printf("RxBuffer1:%s\r\n", RxBuffer1);
    printf("RxBuffer2:%s\r\n", RxBuffer2);
    
    while (1)
    {
    }
}

/**
 * @brief  Configures the different system clocks.
 * @param  LPUART_CLK_SRC: specifies the LPUARTy clock source.
 */
void RCC_Configuration(uint32_t LPUART_CLK_SRC)
{
    USARTz_APBxClkCmd(USARTz_CLK, ENABLE);
	
    switch(LPUART_CLK_SRC)
    {
        case RCC_LPUARTCLK_SRC_LSI_LSE:
        {  
#if 0            
            RCC_ConfigLSXSEL(RCC_RTCCLK_SRC_LSE);
            /* Configures the External Low Speed oscillator (LSE) */
            RCC_ConfigLse(RCC_LSE_ENABLE);        //32.768KHz
            while (RCC_GetFlagStatus(RCC_LSCTRL_FLAG_LSERD) == RESET)
            {
            }
#else            
            RCC_ConfigLSXSEL(RCC_RTCCLK_SRC_LSI);
            RCC_EnableLsi(ENABLE);        //32KHz
            while (RCC_GetFlagStatus(RCC_LSCTRL_FLAG_LSIRD) == RESET)
            {
            }
#endif
            /* Specifies the LPUARTy clock source, LSE selected as LPUARTy clock */
            RCC_ConfigLpuartClk(LPUARTy_CLK, RCC_LPUARTCLK_SRC_LSI_LSE);
        }
        break;        
        
        default:
        {
            /* Specifies the LPUARTy clock source, APB1 selected as LPUARTy clock */
            RCC_ConfigLpuartClk(LPUARTy_CLK, RCC_LPUARTCLK_SRC_APB1);
        }
        break;
    }    
    
    /* Enable LPUARTy Clock */
    RCC_EnableLpuartClk(ENABLE);    
}

/**
 * @brief  Configures the different GPIO ports.
 */
void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;
    
    /* Enable GPIO clock */
    RCC_EnableAPB2PeriphClk(LPUARTy_GPIO_CLK | USARTz_GPIO_CLK | RCC_APB2_PERIPH_AFIO, ENABLE);

    /* Initialize GPIO_InitStructure */
    GPIO_InitStruct(&GPIO_InitStructure);    

    /* Configure LPUARTy Tx as alternate function push-pull */
    GPIO_InitStructure.Pin            = LPUARTy_TxPin;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = LPUARTy_Tx_GPIO_AF;
    GPIO_InitPeripheral(LPUARTy_GPIO, &GPIO_InitStructure);

    /* Configure USARTz Tx as alternate function push-pull */
    GPIO_InitStructure.Pin            = USARTz_TxPin;
    GPIO_InitStructure.GPIO_Alternate = USARTz_Tx_GPIO_AF;
    GPIO_InitPeripheral(USARTz_GPIO, &GPIO_InitStructure);  

    /* Configure LPUARTy Rx as alternate function push-pull and pull-up */
    GPIO_InitStructure.Pin            = LPUARTy_RxPin;  
    GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP;  
    GPIO_InitStructure.GPIO_Alternate = LPUARTy_Rx_GPIO_AF;
    GPIO_InitPeripheral(LPUARTy_GPIO, &GPIO_InitStructure);

    /* Configure USARTz Rx as alternate function push-pull and pull-up */
    GPIO_InitStructure.Pin            = USARTz_RxPin;
    GPIO_InitStructure.GPIO_Alternate = USARTz_Rx_GPIO_AF;
    GPIO_InitPeripheral(USARTz_GPIO, &GPIO_InitStructure); 
}

/**
 * @brief  Configures the nested vectored interrupt controller.
 */
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;

    /* Enable the USARTz Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = LPUARTy_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority           = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  Configures the DMA.
 */
void DMA_Configuration(void)
{
    DMA_InitType DMA_InitStructure;

    /* DMA clock enable */
    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA, ENABLE);

    /* LPUART_Tx_DMA_Channel (triggered by LPUART Tx event) Config */
    DMA_DeInit(LPUARTy_Tx_DMA_Channel);
    DMA_InitStructure.PeriphAddr     = LPUARTy_DAT_Base;
    DMA_InitStructure.MemAddr        = (uint32_t)TxBuffer1;
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_DST;
    DMA_InitStructure.BufSize        = TxBufferSize1;
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc  = DMA_MEM_INC_ENABLE;
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_SIZE_BYTE;
    DMA_InitStructure.MemDataSize    = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.CircularMode   = DMA_MODE_NORMAL;
    DMA_InitStructure.Priority       = DMA_PRIORITY_VERY_HIGH;
    DMA_InitStructure.Mem2Mem        = DMA_M2M_DISABLE;
    DMA_Init(LPUARTy_Tx_DMA_Channel, &DMA_InitStructure);
    DMA_RequestRemap(LPUARTy_Tx_DMA_REMAP, DMA, LPUARTy_Tx_DMA_Channel, ENABLE);

    /* USARTz_Tx_DMA_Channel (triggered by USARTz Tx event) Config */
    DMA_DeInit(USARTz_Tx_DMA_Channel);
    DMA_InitStructure.PeriphAddr = USARTz_DAT_Base;
    DMA_InitStructure.MemAddr    = (uint32_t)TxBuffer2;
    DMA_InitStructure.Direction  = DMA_DIR_PERIPH_DST;
    DMA_InitStructure.BufSize    = TxBufferSize2;
    DMA_Init(USARTz_Tx_DMA_Channel, &DMA_InitStructure);
    DMA_RequestRemap(USARTz_Tx_DMA_REMAP, DMA, USARTz_Tx_DMA_Channel, ENABLE);
}

/**
 * @brief  Compares two buffers.
 * @param  pBuffer1, pBuffer2: buffers to be compared.
 * @param BufferLength buffer's length
 * @return PASSED: pBuffer1 identical to pBuffer2
 *         FAILED: pBuffer1 differs from pBuffer2
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
}

/**
 * @}
 */

/**
 * @}
 */
