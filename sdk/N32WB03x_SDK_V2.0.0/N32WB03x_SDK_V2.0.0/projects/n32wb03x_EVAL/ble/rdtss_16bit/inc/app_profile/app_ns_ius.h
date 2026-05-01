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
 * @file app_ns_ius.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */


#ifndef __APP_NS_IUS_H__
#define __APP_NS_IUS_H__


/* Includes ------------------------------------------------------------------*/

#include "rwip_config.h"     // SW configuration

#include "prf_types.h"
#include "prf.h"


#if (BLE_APP_NS_IUS)
/* Public typedef -----------------------------------------------------------*/
enum
{
    NS_IUS_IDX_SVC,
        
    NS_IUS_IDX_RC_CHAR,
    NS_IUS_IDX_RC_VAL,
      NS_IUS_IDX_RC_CFG,
        
      NS_IUS_IDX_CC_CHAR,
    NS_IUS_IDX_CC_VAL,
    NS_IUS_IDX_CC_CFG,
        
    NS_IUS_IDX_NB,
};

/* Public define ------------------------------------------------------------*/
#define SERVICE_NS_IUS       {0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,  0x01,0x00,  0x11,0x11}        
#define CHAR_NS_IUS_RC       {0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11  ,0x02,0x00,  0x11,0x11}     
#define CHAR_NS_IUS_CC       {0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11  ,0x03,0x00,  0x11,0x11}        


/* Public macro -------------------------------------------------------------*/
/* Public variables ---------------------------------------------------------*/
extern struct attm_desc_128 ns_ius_att_db[NS_IUS_IDX_NB];
/* Public function prototypes -----------------------------------------------*/
extern const struct app_subtask_handlers ns_ius_app_handlers;

void ns_ble_ius_app_cc_send(uint8_t *p_data, uint16_t length);
void app_ns_ius_add_ns_ius(void);
void app_ns_ius_init(void);

#endif //BLE_APP_NS_IUS

#endif //__APP_NS_IUS_H__
