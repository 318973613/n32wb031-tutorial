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
 * @file spi_flash.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32wb03x.h"

/** @addtogroup Utilities
 * @{
 */

/** @addtogroup
 * @{
 */

/** @addtogroup Common
 * @{
 */

/** @addtogroup SPI_FLASH
 * @{
 */

/** @addtogroup SPI_FLASH_Exported_Types
 * @{
 */
/**
 * @}
 */

/** @addtogroup SPI_FLASH_Exported_Constants
 * @{
 */
/**
 * @brief  SPI Flash supported commands
 */
#define sFLASH_CMD_WRITE 0x02 /*!< Write to Memory instruction */
#define sFLASH_CMD_WRSR  0x01 /*!< Write Status Register instruction */
#define sFLASH_CMD_WREN  0x06 /*!< Write enable instruction */
#define sFLASH_CMD_READ  0x03 /*!< Read from Memory instruction */
#define sFLASH_CMD_RDSR  0x05 /*!< Read Status Register instruction  */
#define sFLASH_CMD_RDID  0x9F /*!< Read identification */
#define sFLASH_CMD_SE    0x20 /*!< Sector Erase instruction */
#define sFLASH_CMD_BE    0xC7 /*!< Bulk Erase instruction */

#define sFLASH_WIP_FLAG 0x01 /*!< Write In Progress (WIP) flag */

#define sFLASH_DUMMY_BYTE   0xA5
#define sFLASH_SPI_PAGESIZE 0x100

#define sFLASH_W25Q128_ID 0x00EF4014
#define sFLASH_M25P64_ID  0x202017

#if 1
#define sFLASH_SPI                SPI1
#define sFLASH_SPI_CLK            RCC_APB2_PERIPH_SPI1
#define sFLASH_SPI_AF             GPIO_AF1_SPI1
#define sFLASH_SPI_SCK_PIN        GPIO_PIN_1 /* PA.01 */
#define sFLASH_SPI_SCK_GPIO_PORT  GPIOA      /* GPIOA */
#define sFLASH_SPI_SCK_GPIO_CLK   RCC_APB2_PERIPH_GPIOA
#define sFLASH_SPI_MISO_PIN       GPIO_PIN_3 /* PA.03 */
#define sFLASH_SPI_MISO_GPIO_PORT GPIOA      /* GPIOA */
#define sFLASH_SPI_MISO_GPIO_CLK  RCC_APB2_PERIPH_GPIOA
#define sFLASH_SPI_MOSI_PIN       GPIO_PIN_2 /* PA.02 */
#define sFLASH_SPI_MOSI_GPIO_PORT GPIOA      /* GPIOA */
#define sFLASH_SPI_MOSI_GPIO_CLK  RCC_APB2_PERIPH_GPIOA
#define sFLASH_CS_PIN             GPIO_PIN_0 /* PA.00 */
#define sFLASH_CS_GPIO_PORT       GPIOA      /* GPIOA */
#define sFLASH_CS_GPIO_CLK        RCC_APB2_PERIPH_GPIOA
#else
#define sFLASH_SPI                SPI2
#define sFLASH_SPI_CLK            RCC_APB2_PERIPH_SPI2
#define sFLASH_SPI_AF              GPIO_AF2_SPI2
#define sFLASH_SPI_SCK_PIN        GPIO_PIN_1 /* PB.01 */
#define sFLASH_SPI_SCK_GPIO_PORT  GPIOB      /* GPIOB */
#define sFLASH_SPI_SCK_GPIO_CLK   RCC_APB2_PERIPH_GPIOB
#define sFLASH_SPI_MISO_PIN       GPIO_PIN_3 /* PB.03 */
#define sFLASH_SPI_MISO_GPIO_PORT GPIOB      /* GPIOB */
#define sFLASH_SPI_MISO_GPIO_CLK  RCC_APB2_PERIPH_GPIOB
#define sFLASH_SPI_MOSI_PIN       GPIO_PIN_2 /* PB.02 */
#define sFLASH_SPI_MOSI_GPIO_PORT GPIOB      /* GPIOB */
#define sFLASH_SPI_MOSI_GPIO_CLK  RCC_APB2_PERIPH_GPIOB
#define sFLASH_CS_PIN             GPIO_PIN_0 /* PB.00 */
#define sFLASH_CS_GPIO_PORT       GPIOB      /* GPIOB */
#define sFLASH_CS_GPIO_CLK        RCC_APB2_PERIPH_GPIOB
#endif


/**
 * @}
 */

/** @addtogroup SPI_FLASH_Exported_Macros
 * @{
 */
/**
 * @brief  Select sFLASH: Chip Select pin low
 */
#define sFLASH_CS_LOW() GPIO_ResetBits(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN)
/**
 * @brief  Deselect sFLASH: Chip Select pin high
 */
#define sFLASH_CS_HIGH() GPIO_SetBits(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN)
/**
 * @}
 */

/** @addtogroup SPI_FLASH_Exported_Functions
 * @{
 */
/**
 * @brief  High layer functions
 */
void sFLASH_DeInit(void);
void sFLASH_Init(void);
void sFLASH_EraseSector(uint32_t SectorAddr);
void sFLASH_EraseBulk(void);
void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t sFLASH_ReadID(void);
void sFLASH_StartReadSequence(uint32_t ReadAddr);

/**
 * @brief  Low layer functions
 */
uint8_t sFLASH_ReadByte(void);
uint8_t sFLASH_SendByte(uint8_t byte);
uint16_t sFLASH_SendHalfWord(uint16_t HalfWord);
void sFLASH_WriteEnable(void);
void sFLASH_WaitForWriteEnd(void);

#ifdef __cplusplus
}
#endif

#endif
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
