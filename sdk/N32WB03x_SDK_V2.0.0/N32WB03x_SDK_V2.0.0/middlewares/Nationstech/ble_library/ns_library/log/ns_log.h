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
 * @file ns_log.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

#ifndef __NS_LOG_H__
#define __NS_LOG_H__


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_user_config.h"

#if   (NS_LOG_LPUART_ENABLE)
#include "ns_log_lpuart.h" 
#elif (NS_LOG_USART_ENABLE)
#include "ns_log_usart.h"
#elif (NS_LOG_RTT_ENABLE)
#include "ns_log_rtt.h"       
#endif

/* Private define ------------------------------------------------------------*/
#ifndef NS_LOG_ERROR_ENABLE
#define NS_LOG_ERROR_ENABLE      0
#endif
#ifndef NS_LOG_WARNING_ENABLE
#define NS_LOG_WARNING_ENABLE    0
#endif
#ifndef NS_LOG_INFO_ENABLE
#define NS_LOG_INFO_ENABLE       0
#endif
#ifndef NS_LOG_DEBUG_ENABLE
#define NS_LOG_DEBUG_ENABLE      0
#endif
#ifndef NS_LOG_LPUART_ENABLE
#define NS_LOG_LPUART_ENABLE     0
#endif
#ifndef NS_LOG_USART_ENABLE
#define NS_LOG_USART_ENABLE      0
#endif
#ifndef NS_LOG_RTT_ENABLE
#define NS_LOG_RTT_ENABLE        0
#endif

#ifndef PRINTF_COLOR_ENABLE
#define PRINTF_COLOR_ENABLE      0
#endif
#define LOG_COLOR_RED            "\033[0;31m"
#define LOG_COLOR_YELLOW         "\033[0;33m"
#define LOG_COLOR_CYAN           "\033[0;36m"
#define LOG_COLOR_GREEN          "\033[0;32m"


#if   (NS_LOG_LPUART_ENABLE)
#define NS_LOG_INTERNAL_OUTPUT(color, ...)  NS_LOG_LPUART_OUTPUT(color, __VA_ARGS__)
#define NS_LOG_INTERNAL_INIT()              NS_LOG_LPUART_INIT() 
#define NS_LOG_INTERNAL_DEINIT()           
#elif (NS_LOG_USART_ENABLE)
#define NS_LOG_INTERNAL_OUTPUT(color, ...)  NS_LOG_USART_OUTPUT(color, __VA_ARGS__)
#define NS_LOG_INTERNAL_INIT()              NS_LOG_USART_INIT()
#define NS_LOG_INTERNAL_DEINIT()            NS_LOG_USART_DEINIT()
#elif (NS_LOG_RTT_ENABLE)
#define NS_LOG_INTERNAL_OUTPUT(color, ...)  NS_LOG_RTT_OUTPUT(color, __VA_ARGS__)
#define NS_LOG_INTERNAL_INIT()              NS_LOG_RTT_INIT()
#define NS_LOG_INTERNAL_DEINIT()            NS_LOG_RTT_DEINIT()
#else
#define NS_LOG_INTERNAL_OUTPUT(color, ...)  
#define NS_LOG_INTERNAL_INIT() 
#define NS_LOG_INTERNAL_DEINIT()
#endif
/* Public typedef -----------------------------------------------------------*/
/* Public define ------------------------------------------------------------*/
#if  NS_LOG_ERROR_ENABLE
#define NS_LOG_ERROR(...)        NS_LOG_INTERNAL_OUTPUT(LOG_COLOR_RED, __VA_ARGS__)
#else
#define NS_LOG_ERROR( ...) 
#endif

#if NS_LOG_WARNING_ENABLE
#define NS_LOG_WARNING(...)      NS_LOG_INTERNAL_OUTPUT(LOG_COLOR_YELLOW, __VA_ARGS__)
#else
#define NS_LOG_WARNING( ...) 
#endif

#if  NS_LOG_INFO_ENABLE
#define NS_LOG_INFO(...)         NS_LOG_INTERNAL_OUTPUT(LOG_COLOR_CYAN, __VA_ARGS__)
#else
#define NS_LOG_INFO( ...) 
#endif

#if  NS_LOG_DEBUG_ENABLE
#define NS_LOG_DEBUG(...)        NS_LOG_INTERNAL_OUTPUT(LOG_COLOR_GREEN, __VA_ARGS__)
#else
#define NS_LOG_DEBUG( ...) 
#endif


/**
 * @brief   Initialization the log module
 * @param  
 * @return 
 * @note   
 */
#define NS_LOG_INIT()              NS_LOG_INTERNAL_INIT()

/**
 * @brief   Deinitialize the log module
 * @param  
 * @return 
 * @note   
 */
#define NS_LOG_DEINIT()            NS_LOG_INTERNAL_DEINIT()
/* Public constants ---------------------------------------------------------*/
/* Public function prototypes -----------------------------------------------*/



#ifdef __cplusplus
}
#endif

#endif /* __NS_LOG_H__ */
/**
 * @}
 */
