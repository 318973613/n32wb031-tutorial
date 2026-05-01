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
 * @file ns_timer.h
 * @author NSING Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/** @addtogroup 
 * @{
 */

#ifndef _NS_TIMER_H_
#define _NS_TIMER_H_

/* Includes ------------------------------------------------------------------*/
#include "ke_msg.h"
/* Public typedef -----------------------------------------------------------*/
/// Timer handler type
typedef uint8_t timer_hnd_t;
/// Timer callback function type definition
typedef void (* timer_callback_t)(void);
/* Public define ------------------------------------------------------------*/    
/// Max timer delay 41943sec (41943000ms)
#define KE_TIMER_DELAY_MAX          (41943000)
/// Value indicating an invalide timer operation
#define NS_TIMER_INVALID_HANDLER    (0xFF)
/* Public constants ---------------------------------------------------------*/
/* Public function prototypes -----------------------------------------------*/


/**
 * @brief     Process handler for the Timer messages.
 * @param msgid   Id of the message received
 * @param param   Pointer to the parameters of the message
 * @param dest_id ID of the receiving task instance (probably unused)
 * @param src_id  ID of the sending task instance
 * @param msg_ret Result of the message handler
 * @return    Returns if the message is handled by the process handler
 * @note   
 */

enum process_event_response ns_timer_api_process_handler(ke_msg_id_t const msgid,
                                                          void const *param,
                                                          ke_task_id_t const dest_id,
                                                          ke_task_id_t const src_id,
                                                          enum ke_msg_status_tag *msg_ret);


/**
 * @brief  Create a new timer.
 * @param  Param input
 * @brief Create a new timer.
 * @param delay The amount of timer value to wait (time resolution is 1ms)
 * @param fn    The callback to be called when the timer expires
 * @return The handler of the timer for future reference. If there are not timers available
 *         NS_TIMER_INVALID_HANDLER will be returned
 */
timer_hnd_t ns_timer_create(const uint32_t delay, timer_callback_t fn);

/**
 * @brief Cancel an active timer.
 * @param timer_id The timer handler to cancel
 */
void ns_timer_cancel(const timer_hnd_t timer_id);

/**
 * @brief Modify the delay of an existing timer.
 * @param timer_id The timer handler to modify
 * @param delay    The new delay value (time resolution is 1ms)
 * @return The timer handler if everything is ok
 */
timer_hnd_t ns_timer_modify(const timer_hnd_t timer_id, const uint32_t delay);

/**
 * @brief Cancel all the active timers.
 */
void ns_timer_cancel_all(void);

#endif // _NS_TIMER_H_

/**
 * @}
 */
