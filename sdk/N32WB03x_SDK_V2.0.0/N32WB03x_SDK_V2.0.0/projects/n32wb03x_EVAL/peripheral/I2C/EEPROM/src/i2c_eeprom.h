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
 * @file i2c_eeprom.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#ifndef __I2C_EEPROM_H__
#define __I2C_EEPROM_H__

#include "n32wb03x.h"
#include <stdio.h>

typedef enum i2c_state
{
    COMM_DONE       = 0, /// done successfully
    COMM_PRE        = 1,
    COMM_IN_PROCESS = 2,
    COMM_EXIT       = 3 /// exit since failure
} I2C_STATE;

typedef enum i2c_direction
{
    Transmitter = 0x00,
    Receiver    = 0x01
} I2C_DIRECTION;

/**
 * PROCESS MODE
 * 0=polling
 * 1=interrupt
 * 2=DMA
 */

#define PROCESS_MODE     (0)

#define I2C1_TEST
//#define I2C1_REMAP

#ifdef I2C1_TEST
#define I2Cx            I2C1
#define I2Cx_SCL_PIN    GPIO_PIN_7
#define I2Cx_SDA_PIN    GPIO_PIN_6
#define GPIOx           GPIOB
#define I2Cx_GPIO_AF    GPIO_AF3_I2C
#define I2Cx_IRQ        I2C1_IRQn

#define I2Cx_peripheral_clk_en() RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_I2C1, ENABLE)
#define I2Cx_scl_clk_en()        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE)
#define I2Cx_sda_clk_en()        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE)
#endif

#define TEST_EEPROM_SIZE 256
#define TEST_EEPROM_ADDR 0x00
#define I2C_Speed        100000
#define EEPROM_ADDRESS   0xA0
#define I2C_PageSize     16//32 /// eeprom IC type M24C32
#define sEE_FLAG_TIMEOUT ((uint32_t)0x1000)
#define sEE_LONG_TIMEOUT ((uint32_t)(100 * sEE_FLAG_TIMEOUT))

/** Maximum number of trials for sEE_WaitEepromStandbyState() function */
#define sEE_MAX_TRIALS_NUMBER 150
#define FALSE                 0
#define TRUE                  1

void I2C_EE_Init(void);
void I2C_EE_WriteBuffer(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
void I2C_EE_WriteOnePage(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
void I2C_EE_PageWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
void I2C_EE_WriteOnePageCompleted(void);

void I2C_EE_ReadBuffer(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead);
void I2C_EE_WaitOperationIsCompleted(void);
void I2C_EE_WaitEepromStandbyState(void);

void i2c1_evt_handle(void);
void i2c1_err_handle(void);
void i2c1_send_dma_handle(void);
void i2c1_receive_dma_handle(void);

#endif /* __I2C_EEPROM_H__ */
