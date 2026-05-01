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
 * @file n32wb03x_crc.c
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include "n32wb03x_crc.h"

/** @addtogroup N32WB03X_StdPeriph_Driver
 * @{
 */

/** @addtogroup CRC
 * @brief CRC driver modules
 * @{
 */

/** @addtogroup CRC_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup CRC_Private_Defines
 * @{
 */

/**
 * @}
 */

/** @addtogroup CRC_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup CRC_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup CRC_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup CRC_Private_Functions
 * @{
 */

/**
 * @brief  Resets the CRC Data register (DAT).
 */
void CRC32_ResetCrc(void)
{
    /* Reset CRC generator */
    CRC->CRC32CTRL = CRC32_CTRL_RESET;
}

/**
 * @brief  Computes the 32-bit CRC of a given data word(32-bit).
 * @param Data data word(32-bit) to compute its CRC
 * @return 32-bit CRC
 */
uint32_t CRC32_CalcCrc(uint32_t Data)
{
    CRC->CRC32DAT = Data;

    return (CRC->CRC32DAT);
}

/**
 * @brief  Computes the 32-bit CRC of a given buffer of data word(32-bit).
 * @param pBuffer pointer to the buffer containing the data to be computed
 * @param BufferLength length of the buffer to be computed
 * @return 32-bit CRC
 */
uint32_t CRC32_CalcBufCrc(uint32_t pBuffer[], uint32_t BufferLength)
{
    uint32_t index = 0;

    for (index = 0; index < BufferLength; index++)
    {
        CRC->CRC32DAT = pBuffer[index];
    }
    return (CRC->CRC32DAT);
}

/**
 * @brief  Returns the current CRC value.
 * @return 32-bit CRC
 */
uint32_t CRC32_GetCrc(void)
{
    return (CRC->CRC32DAT);
}

/**
 * @brief  Stores a 8-bit data in the Independent Data(ID) register.
 * @param IDValue 8-bit value to be stored in the ID register
 */
void CRC32_SetIDat(uint8_t IDValue)
{
    CRC->CRC32IDAT = IDValue;
}

/**
 * @brief  Returns the 8-bit data stored in the Independent Data(ID) register
 * @return 8-bit value of the ID register
 */
uint8_t CRC32_GetIDat(void)
{
    return (CRC->CRC32IDAT);
}

// CRC16 add
void __CRC16_SetLittleEndianFmt(void)
{
    CRC->CRC16CTRL = CRC16_CTRL_LITTLE | CRC->CRC16CTRL;
}
void __CRC16_SetBigEndianFmt(void)
{
    CRC->CRC16CTRL = CRC16_CTRL_BIG & CRC->CRC16CTRL;
}
void __CRC16_SetCleanEnable(void)
{
    CRC->CRC16CTRL = CRC16_CTRL_RESET | CRC->CRC16CTRL;
}
void __CRC16_SetCleanDisable(void)
{
    CRC->CRC16CTRL = CRC16_CTRL_NO_RESET & CRC->CRC16CTRL;
}

uint16_t __CRC16_CalcCrc(uint8_t Data)
{
    CRC->CRC16DAT = Data;
    return (CRC->CRC16D);
}

void __CRC16_SetCrc(uint8_t Data)
{
    CRC->CRC16DAT = Data;
}

uint16_t __CRC16_GetCrc(void)
{
    return (CRC->CRC16D);
}

void __CRC16_SetLRC(uint8_t Data)
{
    CRC->LRC = Data;
}

uint8_t __CRC16_GetLRC(void)
{
    return (CRC->LRC);
}

uint16_t CRC16_CalcBufCrc(uint8_t pBuffer[], uint32_t BufferLength)
{
    uint32_t index = 0;

    CRC->CRC16D = 0x00;
    for (index = 0; index < BufferLength; index++)
    {
        CRC->CRC16DAT = pBuffer[index];
    }
    return (CRC->CRC16D);
}

uint16_t CRC16_CalcCRC(uint8_t Data)
{
    CRC->CRC16DAT = Data;

    return (CRC->CRC16D);
}
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
