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
 * @file ns_ble_task.h
 * @author NSING Firmware Team
 * @version v1.0.4
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */


#ifndef _NS_BLE_TASK_H_
#define _NS_BLE_TASK_H_

/**
 * @addtogroup APPTASK Task
 * @ingroup APP
 * @brief Routes ALL messages to/from APP block.
 *
 * The APPTASK is the block responsible for bridging the final application with the
 * RWBLE software host stack. It communicates with the different modules of the BLE host,
 * i.e. @ref SMP, @ref GAP and @ref GATT.
 *
 * @{
 **/

/* Includes ------------------------------------------------------------------*/

#include "rwip_config.h"             // SW configuration

#if (BLE_APP_PRESENT)
#include "global_func.h"

/* Define ------------------------------------------------------------*/

/// Number of APP Task Instances
#define APP_IDX_MAX                 (1)


#ifndef NS_LSI_CALIB_EN
#define NS_LSI_CALIB_EN 1
#endif
#define LSI_CLOCK_CNT_CYCLES        (126)
#define LSI_CLOCK_EVENT_INTV        (5)  //1000/(32000/126)
#define LSI_CLOCK_CALIB_TIMES       (500)
/* Typedef -----------------------------------------------------------*/
 
 /// Process event response
enum process_event_response
{
    /// Handled
    PR_EVENT_HANDLED = 0,

    /// Unhandled
    PR_EVENT_UNHANDLED
};


/// States of APP task
enum app_state
{
    /// Initialization state
    APP_INIT,
    /// Database create state
    APP_CREATE_DB,
    /// Ready State
    APP_READY,
    /// Connected state
    APP_CONNECTED,

    /// Number of defined states.
    APP_STATE_MAX
};


/// APP Task messages
/*@TRACE*/
enum app_msg_id
{
    APP_DUMMY_MSG = TASK_FIRST_MSG(TASK_ID_APP),
    APP_LSI_CALIB_EVT,
    APP_PARAMS_UPDATE_EVT,
    APP_INIT_TIMEOUT_EVT,
    APP_BOND_STORE_EVT,
    #if (BLE_APP_NS_IUS)    
    APP_DFU_BLE_RESET_TIMER,
    #endif
    #if (NS_TIMER_ENABLE)
    /*ns timer*/
    APP_CANCEL_TIMER,
    APP_MODIFY_TIMER,
    //Do not alter the order of the next messages
    //they are considered a range
    NS_TIMER_API_MES0,
    NS_TIMER_API_MES1=NS_TIMER_API_MES0+1,
    NS_TIMER_API_MES2=NS_TIMER_API_MES0+2,
    NS_TIMER_API_MES3=NS_TIMER_API_MES0+3,
    NS_TIMER_API_MES4=NS_TIMER_API_MES0+4,
    NS_TIMER_API_MES5=NS_TIMER_API_MES0+5,
    NS_TIMER_API_MES6=NS_TIMER_API_MES0+6,
    NS_TIMER_API_MES7=NS_TIMER_API_MES0+7,
    NS_TIMER_API_MES8=NS_TIMER_API_MES0+8,
    NS_TIMER_API_MES9=NS_TIMER_API_MES0+9,
    NS_TIMER_API_LAST_MES=NS_TIMER_API_MES9,
    #endif //NS_TIMER_ENABLE
    
    //keep APP_FREE_EVE_FOR_USER as last event
    APP_FREE_EVE_FOR_USER,
        
};


/* Public variables ---------------------------------------------------------*/

/// @} APPTASK

#endif //(BLE_APP_PRESENT)

#endif // _NS_BLE_TASK_H_
