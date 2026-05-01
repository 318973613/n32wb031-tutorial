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
 * @file global_func.h
 * @author NSING Firmware Team
 * @version v1.0.4
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "n32wb03x.h"
#include "global_var.h"
#include "Typedefine.h"

#include "rwip_config.h"     // SW configuration
#include "compiler.h"
#include "co_version.h"      // version information
#include "co_utils.h"
#include "co_bt.h"           // Common BT Definitions
#include "co_math.h"         // Common Maths Definition
#include "co_bt_defines.h"


#include "arch.h"            // Platform architecture definition
#include "em_map.h"
#include "rf.h"

#include "rwip.h"            // RW definitions
#include "rwip_int.h"        // RW internal definitions
#include "rwip_task.h"       // Task definitions

#include "sch_alarm.h"       // for the half slot target ISR
#include "sch_arb.h"         // for the half us target ISR
#include "sch_prog.h"        // for the fifo/clock ISRs
#include "reg_ipcore.h"
#include "reg_blecore.h"
#include "aes.h"             // AES result function
#include "rwble.h"           // for sleep and wake-up specific functions
#include "lld.h"             // for AES encryption handler

#include "ke.h"              // kernel definition
#include "ke_event.h"        // kernel event
#include "ke_timer.h"        // definitions for timer
#include "ke_mem.h"          // kernel memory manager
#include "ke_task.h"         // Kernel Task


#include "rwble_hl.h"        // BLE HL definitions
#include "l2cc.h"
#include "llm_int.h"
#include "ahi_task.h"
#include "co_hci.h"
#include "hci.h"
#include "attm.h"
#include "atts.h"
#include "prf.h"
#include "gap.h"
#include "gapm.h"
#include "gapm_int.h"
#include "gapm_task.h"      // GAP Manager Task API
#include "gapc.h"
#include "gapc_task.h"      // GAP Controller Task API Definition
#include "gattm_int.h"
#include "gattc.h" 
#include "gatt.h"

#include "ble_stack_common.h"
#include "ns_adv_data_def.h"

#ifndef _GLOBAL_FUNC_H_
#define _GLOBAL_FUNC_H_

/* Public typedef -----------------------------------------------------------*/
typedef void (*IRQ_HANNDLE_FUN) (void);

/* Public define ------------------------------------------------------------*/ 
#define GLOBAL_INT_DISABLE()        \
uint32_t ui32IntStatus = 0;         \
do{                                 \
    ui32IntStatus = __get_PRIMASK();\
    __set_PRIMASK(1);               \
}while(0)

#define GLOBAL_INT_RESTORE()     \
do{                              \
    __set_PRIMASK(ui32IntStatus);\
}while(0)


/* Public constants ---------------------------------------------------------*/
/* Public function prototypes -----------------------------------------------*/

extern uint32_t calib_lsi_clk(void);
extern void EXTI_PA11_Configuration(void);
extern uint32_t ModuleIrqRegister(IRQn_Type irqn,IRQ_HANNDLE_FUN fun);
extern uint32_t ModuleIrqRemoval(IRQn_Type irqn);

extern void rwip_slp_isr(void);
extern void rf_tx_power_set(rf_tx_power_t pwr);
#endif    //_GLOBAL_FUNC_H_
