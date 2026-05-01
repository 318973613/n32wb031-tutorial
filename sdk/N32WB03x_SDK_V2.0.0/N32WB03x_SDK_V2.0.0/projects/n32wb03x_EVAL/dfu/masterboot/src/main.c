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
 * @version v1.0.4
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/** @addtogroup 
 * @{
 */

 /* Includes ------------------------------------------------------------------*/
#include "n32wb03x.h"
#include "dfu_led.h"
#include "ns_dfu_boot.h"
#include "dfu_crc.h"
#include "ns_scheduler.h"
#include "ns_dfu_serial.h"
#include "dfu_usart.h"

/**
 * @brief Loop up the bootsetting and decide which bank to jump to , or stay in masterboot for usart image update.
 * @param[in] none.
 * @return none
 */
static void masterboot(void)
{
    uint32_t jump_address = NULL;
    if(ns_bootsetting.crc == dfu_crc32((uint8_t *)&ns_bootsetting.crc + 4, sizeof(NS_Bootsetting_t) - 4))
    {
        if(ns_bootsetting.master_boot_force_update != NS_BOOTSETTING_MASTER_BOOT_FORCE_UPDATE_YES)
        {
            if(ns_bootsetting.app1.activation == NS_BOOTSETTING_ACTIVATION_YES && ns_bootsetting.app1.start_address == NS_APP1_START_ADDRESS)
            {
                if(ns_bootsetting.app1.crc == dfu_crc32((uint8_t *)((uint32_t *)ns_bootsetting.app1.start_address), ns_bootsetting.app1.size))
                {
                    jump_address = ns_bootsetting.app1.start_address;
                }
            }    
            else if(ns_bootsetting.app2.activation == NS_BOOTSETTING_ACTIVATION_YES && ns_bootsetting.app2.start_address == NS_APP2_START_ADDRESS)
            {
                if(ns_bootsetting.app2.crc == dfu_crc32((uint8_t *)((uint32_t *)ns_bootsetting.app2.start_address), ns_bootsetting.app2.size))
                {
                    jump_address = ns_bootsetting.app2.start_address;
                }
            }    
            else if(ns_bootsetting.ImageUpdate.activation == NS_BOOTSETTING_ACTIVATION_YES && ns_bootsetting.ImageUpdate.start_address == NS_IMAGE_UPDATE_START_ADDRESS)
            {
                if(ns_bootsetting.ImageUpdate.crc == dfu_crc32((uint8_t *)((uint32_t *)ns_bootsetting.ImageUpdate.start_address), ns_bootsetting.ImageUpdate.size))
                {
                    jump_address = ns_bootsetting.ImageUpdate.start_address;
                }
            }
						if(jump_address == NULL)
							{
                if(ns_bootsetting.app1.crc == dfu_crc32((uint8_t *)((uint32_t *)ns_bootsetting.app1.start_address), ns_bootsetting.app1.size))
                {
                    jump_address = ns_bootsetting.app1.start_address;
                }        
                else if(ns_bootsetting.app2.crc == dfu_crc32((uint8_t *)((uint32_t *)ns_bootsetting.app2.start_address), ns_bootsetting.app2.size))
                {
                    jump_address = ns_bootsetting.app2.start_address;
                }            
                else if(ns_bootsetting.ImageUpdate.crc == dfu_crc32((uint8_t *)((uint32_t *)ns_bootsetting.ImageUpdate.start_address), ns_bootsetting.ImageUpdate.size))
                {
                    jump_address = ns_bootsetting.ImageUpdate.start_address;
                }
							}
						if(jump_address != NULL)
							{
									dfu_usart1_default_config();
									ns_dfu_boot_jump(jump_address);
							}							
        }
    }
}

#define POLL_UART_CYCLE (0x6FFF) //20ms
/**
 * @brief  main function
 * @param   
 * @return 
 * @note   Note
 */
int main(void)
{
    /// swd -> uart
    dfu_usart1_poll_config(USART1_GPIO_PA45,1000000);
    if(dfu_usart1_poll_boot_byte(0x7F, POLL_UART_CYCLE) == false)
    { 
        dfu_usart1_poll_config(USART1_GPIO_PA45,115200);
        if(dfu_usart1_poll_boot_byte(0x7F, POLL_UART_CYCLE) == false)
        { 
            dfu_usart1_poll_config(USART1_GPIO_PB67,1000000);
            if(dfu_usart1_poll_boot_byte(0x7F, POLL_UART_CYCLE) == false)
            { 
                dfu_usart1_poll_config(USART1_GPIO_PB67,115200);
                if(dfu_usart1_poll_boot_byte(0x7F, POLL_UART_CYCLE) == false)
                {
                    //normal boot
                    masterboot();
                }
            }
        }
    }
    NS_SCHED_INIT(256, 16);    
    ns_dfu_serial_init();
    
    while(1)
    {
        app_sched_execute();
        __WFE();
        __SEV();
        __WFE();    
    }
}





