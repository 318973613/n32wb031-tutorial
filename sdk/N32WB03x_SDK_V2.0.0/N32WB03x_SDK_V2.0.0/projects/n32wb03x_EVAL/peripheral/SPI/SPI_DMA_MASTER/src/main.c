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
#include "main.h"
#include "n32wb03x.h"
#include "log.h"

#include <string.h>

/** @addtogroup N32WB03X_StdPeriph_Examples
 * @{
 */

/** @addtogroup DMA_SPI_RAM
 * @{
 */

typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} Status;

#define BUFFER_SIZE    32
#define CRC_POLYNOMIAL 7

SPI_InitType SPI_InitStructure;
DMA_InitType DMA_InitStructure;
uint8_t SPI_Master_Rx_Buffer[BUFFER_SIZE], SPI_Slave_Rx_Buffer[BUFFER_SIZE];
volatile uint8_t SPI_MASTERCRCValue = 0, SPI_SLAVECRCValue = 0;
volatile Status TransferStatus1 = PASSED, TransferStatus2 = PASSED;

uint8_t SPI_Master_Tx_Buffer[BUFFER_SIZE] = {0xf1, 0x2e, 0xf1, 0xaf, 0x08, 0xa2, 0x41, 0xb4, 0xc8, 0x19, 0x38,
                                             0xb7, 0xd9, 0xdb, 0x9a, 0x64, 0x1f, 0xd6, 0x99, 0x9c, 0x0f, 0xae,
                                             0x84, 0xab, 0xda, 0x12, 0x95, 0x6c, 0xdb, 0xec, 0x06, 0x08};

uint8_t SPI_Slave_Tx_Buffer[BUFFER_SIZE] = {0x2c, 0x99, 0xd7, 0x26, 0xb0, 0xe5, 0xb2, 0xfc, 0xee, 0x88, 0x3f,
                                            0xde, 0xa4, 0x37, 0x87, 0xc9, 0xb2, 0x9c, 0xce, 0xc8, 0x2c, 0x22,
                                            0x6b, 0xfe, 0xba, 0x49, 0x94, 0x0a, 0x47, 0x5a, 0xb7, 0x89};

void RCC_Configuration(void);
void GPIO_Configuration(void);
Status Buffercmp(uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength);

/**
 * @brief  Main program
 */
int main(void)
{
    uint8_t i;
    log_init();

    log_info("\n this is a SPI master DMA to RAM Demo.\n");
    
    /* System Clocks Configuration */
    RCC_Configuration();

    /* Configure the GPIO ports */
    GPIO_Configuration();
    
    SPI_I2S_DeInit(SPI_MASTER);
    /* SPI_MASTER configuration
     * ------------------------------------------------*/
    SPI_InitStructure.DataDirection = SPI_DIR_DOUBLELINE_FULLDUPLEX;
    SPI_InitStructure.SpiMode       = SPI_MODE_MASTER;
    SPI_InitStructure.DataLen       = SPI_DATA_SIZE_8BITS;
    SPI_InitStructure.CLKPOL        = SPI_CLKPOL_LOW;
    SPI_InitStructure.CLKPHA        = SPI_CLKPHA_SECOND_EDGE;
    SPI_InitStructure.NSS           = SPI_NSS_SOFT;
    SPI_InitStructure.BaudRatePres  = SPI_BR_PRESCALER_32;
    SPI_InitStructure.FirstBit      = SPI_FB_MSB;
    SPI_InitStructure.CRCPoly       = CRC_POLYNOMIAL;
    SPI_Init(SPI_MASTER, &SPI_InitStructure);
    
    /* SPI_MASTER_RX_DMA_CHANNEL configuration
     * ---------------------------------*/
    DMA_DeInit(SPI_MASTER_RX_DMA_CHANNEL);
    DMA_InitStructure.PeriphAddr     = (uint32_t)&SPI_MASTER->DAT;
    DMA_InitStructure.MemAddr        = (uint32_t)SPI_Master_Rx_Buffer;
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_SRC;
    DMA_InitStructure.BufSize        = BUFFER_SIZE;
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc  = DMA_MEM_INC_ENABLE;
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_SIZE_BYTE;
    DMA_InitStructure.MemDataSize    = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.CircularMode   = DMA_MODE_NORMAL;
    DMA_InitStructure.Priority       = DMA_PRIORITY_HIGH;
    DMA_InitStructure.Mem2Mem        = DMA_M2M_DISABLE;
    DMA_Init(SPI_MASTER_RX_DMA_CHANNEL, &DMA_InitStructure);
    DMA_RequestRemap(SPI_MASTER_RX_DMA_REMAP, DMA, SPI_MASTER_RX_DMA_CHANNEL, ENABLE);

    /* SPI_MASTER_TX_DMA_CHANNEL configuration
     * ---------------------------------*/
    DMA_DeInit(SPI_MASTER_TX_DMA_CHANNEL);
    DMA_InitStructure.PeriphAddr = (uint32_t)&SPI_MASTER->DAT;
    DMA_InitStructure.MemAddr    = (uint32_t)SPI_Master_Tx_Buffer;
    DMA_InitStructure.Direction  = DMA_DIR_PERIPH_DST;
    DMA_InitStructure.Priority   = DMA_PRIORITY_LOW;
    DMA_Init(SPI_MASTER_TX_DMA_CHANNEL, &DMA_InitStructure);
    DMA_RequestRemap(SPI_MASTER_TX_DMA_REMAP, DMA, SPI_MASTER_TX_DMA_CHANNEL, ENABLE);
    
    /* Enable SPI_MASTER DMA Tx request */
    SPI_I2S_EnableDma(SPI_MASTER, SPI_I2S_DMA_TX, ENABLE);
    /* Enable SPI_MASTER DMA Rx request */
    SPI_I2S_EnableDma(SPI_MASTER, SPI_I2S_DMA_RX, ENABLE);
        
    /* Enable SPI_MASTER CRC calculation */
    SPI_EnableCalculateCrc(SPI_MASTER, ENABLE);

    /* Enable SPI_MASTER */
    SPI_Enable(SPI_MASTER, ENABLE);

    /* Enable DMA channels */
    DMA_EnableChannel(SPI_MASTER_RX_DMA_CHANNEL, ENABLE);
    DMA_EnableChannel(SPI_MASTER_TX_DMA_CHANNEL, ENABLE);

    /* Transfer complete */
    while (!DMA_GetFlagStatus(SPI_MASTER_RX_DMA_FLAG, SPI_MASTER_DMA))
        ;
    while (!DMA_GetFlagStatus(SPI_MASTER_TX_DMA_FLAG, SPI_MASTER_DMA))
        ;

    /* Wait for SPI_MASTER data reception: CRC transmitted by SPI_SLAVE */
    while (SPI_I2S_GetStatus(SPI_MASTER, SPI_I2S_RNE_FLAG) == RESET)
        ;

    /* Check the correctness of written dada */
    TransferStatus1 = Buffercmp(SPI_Master_Rx_Buffer, SPI_Slave_Tx_Buffer, BUFFER_SIZE);

    /* Test on the SPI_MASTER CRCR ERROR flag */
    if ((SPI_I2S_GetStatus(SPI_MASTER, SPI_CRCERR_FLAG)) != RESET)
    {
        TransferStatus2 = FAILED;
    }

    /* Read SPI_MASTER received CRC value */
    SPI_MASTERCRCValue = SPI_I2S_ReceiveData(SPI_MASTER);
    
    printf("TransferStatus1:%d, TransferStatus2:%d, SPI_MASTERCRCValue:0x%02x\r\n", TransferStatus1, TransferStatus2, SPI_MASTERCRCValue);
    
    if (TransferStatus1 != FAILED)
    {
        log_info("SPI master passed.\n");
        log_info("master recv data = ");
        for(i = 0; i< BUFFER_SIZE; i++)
        {
           log_info("%02x", SPI_Master_Rx_Buffer[i]);
        }
        log_info("\r\n");
    }
    else
    {
        log_error("SPI master failed.\n");
    }
    
    
    while (1)
    {
    }
}

/**
 * @brief  Configures the different system clocks.
 */
void RCC_Configuration(void)
{
    /* Enable peripheral clocks
     * ------------------------------------------------*/
    /* Enable DMA clock */
    RCC_EnableAHBPeriphClk(SPI_MASTER_DMA_CLK, ENABLE);

    /* Enable SPI_MASTER clock and GPIO clock for SPI_MASTER */
    RCC_EnableAPB2PeriphClk(SPI_MASTER_GPIO_CLK | SPI_MASTER_CLK | RCC_APB2_PERIPH_AFIO, ENABLE);
}

/**
 * @brief  Configures the different GPIO ports.
 */
void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;

    GPIO_InitStruct(&GPIO_InitStructure);
    /* Configure SPI_MASTER pins: SCK ,MISO and MOSI */
    GPIO_InitStructure.Pin        = SPI_MASTER_PIN_SCK | SPI_MASTER_PIN_MOSI |SPI_MASTER_PIN_MISO;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Pull  = GPIO_NO_PULL;
    GPIO_InitStructure.GPIO_Alternate = SPI_MASTER_GPIO_AF;
    GPIO_InitPeripheral(SPI_MASTER_GPIO, &GPIO_InitStructure);
}

/**
 * @brief  Compares two buffers.
 * @param  pBuffer, pBuffer1: buffers to be compared.
 * @param BufferLength buffer's length
 * @return PASSED: pBuffer identical to pBuffer1
 *         FAILED: pBuffer differs from pBuffer1
 */
Status Buffercmp(uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer != *pBuffer1)
        {
            return FAILED;
        }

        pBuffer++;
        pBuffer1++;
    }

    return PASSED;
}

/**
 * @}
 */

/**
 * @}
 */
