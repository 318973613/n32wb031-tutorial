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
 * @file ns_dfu_boot.c
 * @author NSING Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/** @addtogroup 
 * @{
 */

 /* Includes ------------------------------------------------------------------*/
#include "ns_dfu_boot.h"
#include "n32wb03x.h"
#include "dfu_crc.h"
#include "ns_error.h"
/* Private typedef -----------------------------------------------------------*/
typedef void (*func_ptr_t)(void);
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
NS_Bootsetting_t ns_bootsetting __attribute__((at(NS_BOOTSETTING_START_ADDRESS))) __attribute__((used));
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
 * @brief Inter program jump function.
 * @param[in] address program flash address.
 * @return none, function will not return
 */
void ns_dfu_boot_jump(uint32_t address)
{
    
    uint32_t JumpAddress;
    func_ptr_t JumpToApplication;
    JumpAddress = *(__IO uint32_t *)(address + 4);
    JumpToApplication = (func_ptr_t)JumpAddress;
    __set_MSP(*(__IO uint32_t *)address);
    JumpToApplication();

}


/**
 * @brief Write force usart image update variable in bootsetting.
 * @param[in] none.
 * @return none
 */
bool ns_dfu_boot_force_usart_dfu(void)
{
    NS_Bootsetting_t ns_bootsetting_tmp;
    memcpy(&ns_bootsetting_tmp,&ns_bootsetting,sizeof(NS_Bootsetting_t));
    ns_bootsetting_tmp.master_boot_force_update = NS_BOOTSETTING_MASTER_BOOT_FORCE_UPDATE_YES;
    ns_bootsetting_tmp.crc = dfu_crc32((uint8_t *)&ns_bootsetting_tmp.crc + 4, sizeof(NS_Bootsetting_t) - 4);
    Qflash_Erase_Sector(NS_BOOTSETTING_START_ADDRESS);
    Qflash_Write(NS_BOOTSETTING_START_ADDRESS, (uint8_t *)&ns_bootsetting_tmp, sizeof(NS_Bootsetting_t));            
    
    if(ns_bootsetting_tmp.crc == dfu_crc32((uint8_t *)((uint32_t *)(NS_BOOTSETTING_START_ADDRESS + 4)), sizeof(NS_Bootsetting_t) - 4))
    {
        NVIC_SystemReset();
        return ERROR_SUCCESS;
    }
    else
    {
        return ERROR_INTERNAL;
    }
}








