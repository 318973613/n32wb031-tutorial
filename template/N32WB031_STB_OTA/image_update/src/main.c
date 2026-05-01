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
 * @version v1.0.2
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/** @addtogroup 
 * @{
 */

 /* Includes ------------------------------------------------------------------*/
#include "n32wb03x.h"
#include "dfu_crc.h"
#include "ble_stack_common.h"
#include "ns_ble.h"
#include "prf.h"
#include "ns_dfu_boot.h"


/**
 * @brief Set the bootsetting to default value, so next time boot up will not goin to imageupdate again.
 * @param[in] none.
 * @return none
 */
static void bootsetting_reset(void)
{
    NS_Bootsetting_t m_ns_bootsetting;
    memcpy(&m_ns_bootsetting,&ns_bootsetting,sizeof(NS_Bootsetting_t));    
    Qflash_Init();
    if(ns_bootsetting.app1.crc == dfu_crc32((uint8_t *)((uint32_t *)ns_bootsetting.app1.start_address), ns_bootsetting.app1.size) )
    {
        m_ns_bootsetting.app1.activation = NS_BOOTSETTING_ACTIVATION_YES;
        m_ns_bootsetting.ImageUpdate.activation = NS_BOOTSETTING_ACTIVATION_NO;
    }
    else if(ns_bootsetting.app2.crc == dfu_crc32((uint8_t *)((uint32_t *)ns_bootsetting.app2.start_address), ns_bootsetting.app2.size) )
    {
        m_ns_bootsetting.app2.activation = NS_BOOTSETTING_ACTIVATION_YES;
        m_ns_bootsetting.ImageUpdate.activation = NS_BOOTSETTING_ACTIVATION_NO;
    }
    if(m_ns_bootsetting.ImageUpdate.activation == NS_BOOTSETTING_ACTIVATION_NO)
    {
        m_ns_bootsetting.crc = dfu_crc32((uint8_t *)&m_ns_bootsetting.crc + 4, sizeof(NS_Bootsetting_t) - 4);
        Qflash_Erase_Sector(NS_BOOTSETTING_START_ADDRESS);
        Qflash_Write(NS_BOOTSETTING_START_ADDRESS, (uint8_t *)&m_ns_bootsetting, sizeof(NS_Bootsetting_t));        
    }
}

int main(void)
{
    bootsetting_reset();
    
    app_init();
    prf_init(RWIP_INIT);

    while(1)
    {
        rwip_schedule();        
    }
}


