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
 * @file ns_dfu_boot.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

 /** @addtogroup 
 * @{
 */
#ifndef __NS_DFU_BOOT_H__
#define __NS_DFU_BOOT_H__


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Public define ------------------------------------------------------------*/
/* Public typedef -----------------------------------------------------------*/
typedef struct
{
    uint32_t start_address;
    uint32_t size;
    uint32_t crc;
    uint32_t version;
    uint32_t activation;
    uint32_t reserve[5];
}NS_Bank_t;

typedef struct
{
    uint32_t crc;
    uint32_t master_boot_force_update;
    NS_Bank_t app1;
    NS_Bank_t app2;
    NS_Bank_t ImageUpdate;
    uint8_t public_key[64];
}NS_Bootsetting_t;
/* Public define ------------------------------------------------------------*/  
extern int Image$$ER_IROM1$$Base;
#define CURRENT_APP_START_ADDRESS                      (uint32_t)&Image$$ER_IROM1$$Base

#define NS_MASTERBOOT_START_ADDRESS                    (0x01000000)
#define NS_MASTERBOOT_SIZE                             (0x2000)                                                                 
#define NS_BOOTSETTING_START_ADDRESS                   (0x01002000)
#define NS_BOOTSETTING_SIZE                            (0x1000)
#define NS_APP_DATA_START_ADDRESS                      (0x01003000)
#define NS_APP_DATA_SIZE                               (0x1000)
#define NS_APP1_START_ADDRESS                          (0x01004000)
#define NS_APP1_DEFAULT_SIZE                           (0x1C000)
#define NS_APP2_START_ADDRESS                          (0x01020000)
#define NS_APP2_DEFAULT_SIZE                           (0x1C000)
#define NS_IMAGE_UPDATE_START_ADDRESS                  (0x0103C000)
#define NS_IMAGE_UPDATE_SIZE                           (0x4000)

#define NS_BOOTSETTING_ACTIVATION_YES                       (0x00000001)
#define NS_BOOTSETTING_ACTIVATION_NO                        (0xFFFFFFFF)
#define NS_BOOTSETTING_MASTER_BOOT_FORCE_UPDATE_YES         (0x00000001)
#define NS_BOOTSETTING_MASTER_BOOT_FORCE_UPDATE_NO          (0xFFFFFFFF)

/* Public variables ---------------------------------------------------------*/
extern NS_Bootsetting_t ns_bootsetting;
/* Public function prototypes -----------------------------------------------*/
void ns_dfu_boot_jump(uint32_t address);
bool ns_dfu_boot_force_usart_dfu(void);


#ifdef __cplusplus
}
#endif


#endif //__NS_DFU_BOOT_H__
