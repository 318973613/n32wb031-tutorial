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
 * @file dfu_crc.c
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/** @addtogroup 
 * @{
 */

 /* Includes ------------------------------------------------------------------*/
#include "dfu_crc.h"
#include "n32wb03x_crc.h"

/* Public define ------------------------------------------------------------*/
/* Public typedef -----------------------------------------------------------*/
/* Public define ------------------------------------------------------------*/  

#define LITTLE_TO_BIG(u32)    \
(uint32_t) ((u32 & 0x000000FF) << 24) | \
(uint32_t) ((u32 & 0x0000FF00) << 8) | \
(uint32_t) ((u32 & 0x00FF0000) >> 8) | \
(uint32_t) ((u32 & 0xFF000000) >> 24)

/**
 * @brief Calculate crc32 for given data, using hardware crc32.
 * @param[in] p_data raw data.
 * @param[in] len raw data len. 
 * @return crc32 result.
 */
uint32_t dfu_crc32(uint8_t * p_data, uint32_t len)
{
    if ((uint32_t)p_data == 0xFFFFFFFF) return 0;
    uint32_t u32p_data;
    RCC->AHBPCLKEN |= RCC_AHB_PERIPH_CRC;
    CRC->CRC32CTRL = CRC32_CTRL_RESET;
    uint32_t index = 0;

    //for if led%4 > 0
    switch (len&0x3)
    {
    	case 1:
            CRC->CRC32DAT = 0x6AA59E9D; 
            u32p_data = p_data[index];
            CRC->CRC32DAT = u32p_data;
            index += 1;
    		break;
        case 2:
            CRC->CRC32DAT = 0x9746CD0A; 
            u32p_data = (uint32_t)(p_data[index]<<8)| p_data[index + 1];
            CRC->CRC32DAT = u32p_data;
            index += 2;
    		break;
    	case 3:
            CRC->CRC32DAT = 0xCC6021D0; 
            u32p_data = (uint32_t)(p_data[index]<<16)| (p_data[index + 1]<<8)|(p_data[index + 2]);
            CRC->CRC32DAT = u32p_data;
            index += 3;
    		break;
    	default:
    		break;
    }
    
    while (index < len)
    {
        u32p_data = p_data[index]; 
        index++;
        u32p_data = (u32p_data<<8)|p_data[index]; 
        index++;
        u32p_data = (u32p_data<<8)|p_data[index]; 
        index++;
        u32p_data = (u32p_data<<8)|p_data[index]; 
        index++;
        CRC->CRC32DAT = u32p_data;
    }

    return CRC->CRC32DAT;
}

/**
 * @}
 */



