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
 * @file ns_scheduler.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

 /** @addtogroup 
 * @{
 */
#ifndef __NS_SCHEDULER_H__
#define __NS_SCHEDULER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/



#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#define APP_SCHED_EVENT_HEADER_SIZE 8       /**< Size of app_scheduler.event_header_t (only for use inside APP_SCHED_BUF_SIZE()). */


/**@brief Compute number of bytes required to hold the scheduler buffer.
 *
 * @param[in] EVENT_SIZE   Maximum size of events to be passed through the scheduler.
 * @param[in] QUEUE_SIZE   Number of entries in scheduler queue (i.e. the maximum number of events
 *                         that can be scheduled for execution).
 *
 * @return    Required scheduler buffer size (in bytes).
 */
#define APP_SCHED_BUF_SIZE(EVENT_SIZE, QUEUE_SIZE)                                                 \
            (((EVENT_SIZE) + APP_SCHED_EVENT_HEADER_SIZE) * ((QUEUE_SIZE) + 1))

/**@brief Scheduler event handler type. */
typedef void (*app_sched_event_handler_t)(void * p_event_data, uint16_t event_size);

/**@brief Macro for performing integer division, making sure the result is rounded up.
 *
 * @details One typical use for this is to compute the number of objects with size B is needed to
 *          hold A number of bytes.
 *
 * @param[in]   A   Numerator.
 * @param[in]   B   Denominator.
 *
 * @return      Integer result of dividing A by B, rounded up.
 */
#define CEIL_DIV(A, B)      \
    (((A) + (B) - 1) / (B))
/**@brief Macro for initializing the event scheduler.
 *
 * @details It will also handle dimensioning and allocation of the memory buffer required by the
 *          scheduler, making sure the buffer is correctly aligned.
 *
 * @param[in] EVENT_SIZE   Maximum size of events to be passed through the scheduler.
 * @param[in] QUEUE_SIZE   Number of entries in scheduler queue (i.e. the maximum number of events
 *                         that can be scheduled for execution).
 *
 * @note Since this macro allocates a buffer, it must only be called once (it is OK to call it
 *       several times as long as it is from the same location, e.g. to do a reinitialization).
 */
#define NS_SCHED_INIT(EVENT_SIZE, QUEUE_SIZE)                                                     \
    do                                                                                             \
    {                                                                                              \
        static uint32_t APP_SCHED_BUF[CEIL_DIV(APP_SCHED_BUF_SIZE((EVENT_SIZE), (QUEUE_SIZE)),     \
                                               sizeof(uint32_t))];                                 \
        uint32_t ERR_CODE = app_sched_init((EVENT_SIZE), (QUEUE_SIZE), APP_SCHED_BUF);             \
                                                                        \
    } while (0)


/**@brief Function for initializing the Scheduler.
 *
 * @details It must be called before entering the main loop.
 *
 * @param[in]   max_event_size   Maximum size of events to be passed through the scheduler.
 * @param[in]   queue_size       Number of entries in scheduler queue (i.e. the maximum number of
 *                               events that can be scheduled for execution).
 * @param[in]   p_evt_buffer   Pointer to memory buffer for holding the scheduler queue. It must
 *                               be dimensioned using the APP_SCHED_BUFFER_SIZE() macro. The buffer
 *                               must be aligned to a 4 byte boundary.
 *
 * @note Normally initialization should be done using the APP_SCHED_INIT() macro, as that will both
 *       allocate the scheduler buffer, and also align the buffer correctly.
 *
 * @retval      NRF_SUCCESS               Successful initialization.
 * @retval      NRF_ERROR_INVALID_PARAM   Invalid parameter (buffer not aligned to a 4 byte
 *                                        boundary).
 */
uint32_t app_sched_init(uint16_t max_event_size, uint16_t queue_size, void * p_evt_buffer);

/**@brief Function for executing all scheduled events.
 *
 * @details This function must be called from within the main loop. It will execute all events
 *          scheduled since the last time it was called.
 */
void app_sched_execute(void);

/**@brief Function for scheduling an event.
 *
 * @details Puts an event into the event queue.
 *
 * @param[in]   p_event_data   Pointer to event data to be scheduled.
 * @param[in]   event_size     Size of event data to be scheduled.
 * @param[in]   handler        Event handler to receive the event.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t app_sched_event_put(void const *              p_event_data,
                             uint16_t                  event_size,
                             app_sched_event_handler_t handler);


/**@brief Function for getting the current amount of free space in the queue.
 *
 * @details The real amount of free space may be less if entries are being added from an interrupt.
 *          To get the sxact value, this function should be called from the critical section.
 *
 * @return Amount of free space in the queue.
 */
uint16_t app_sched_queue_space_get(void);


#ifdef __cplusplus
}
#endif







#endif //__NS_SCHEDULER_H__
/**
 * @}
 */

