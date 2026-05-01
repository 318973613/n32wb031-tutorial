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
 * @file ns_sec.h
 * @author NSING Firmware Team
 * @version v1.0.4
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/** @addtogroup NS_SEC
 * @{
 */

#ifndef NS_SEC_H_
#define NS_SEC_H_

/* Includes ------------------------------------------------------------------*/
#include "rwip_config.h"

#if (BLE_APP_SEC)
#include "global_func.h"
/* Public define ------------------------------------------------------------*/
#ifndef MAX_BOND_PEER
#define MAX_BOND_PEER                       8
#endif
/* Public typedef -----------------------------------------------------------*/


enum sec_msg_id
{
    NS_SEC_NULL_MSG = 0,
    NS_SEC_BOND_STATE,
    NS_SEC_PAIRING_RSP,
    NS_SEC_PINCODE_DISPLAY,
    NS_SEC_PINCODE_ENTER,
    NS_SEC_PAIR_SUCCEED,
    NS_SEC_PAIR_FAILED,
    NS_SEC_LTK_FOUND,
    NS_SEC_LTK_MISSING,
    NS_SEC_ENC_SUCCEED,
    
};

struct sec_msg_t
{
    enum sec_msg_id msg_id;
    //income message
    union 
    {
        uint8_t const*  peer_num;
        uint32_t const* pincode;
        struct gapc_bond_cfm* cfm;
    }msg;
    
};


/// Application Security Bond Data environment structure
struct app_sec_bond_data_env_tag
{
    // LTK
    struct gap_sec_key ltk;
    
    // Random Number
    rand_nb_t rand_nb;
    
    // EDIV
    uint16_t ediv;
    
    // Remote IRK
    struct gapc_irk irk;

    // LTK key size
    uint8_t key_size;

    // Last paired peer address type
    uint8_t peer_addr_type;
    
    // Last paired peer address
    bd_addr_t peer_addr;

    // authentication level
    uint8_t auth;
};

/* bonding structure
 * --------------------------------------------------------------------------------
 * |         ADDR            |                 FIELD                   |   SIZE   |
 * --------------------------|-----------------------------------------|----------|
 * | BOND_DATA_BASE_ADDR+0   | Bond space valid flag                   |    2     |
 * --------------------------|-----------------------------------------|----------|
 * | BOND_DATA_BASE_ADDR+2   | Bonded peer device numbers              |    1     |
 * --------------------------------------------------------------------------------
 * | BOND_DATA_BASE_ADDR+4   | Bond data0(@ app_sec_bond_data_env_tag) |          |
 * --------------------------------------------------------------------------------
 * |                         | Bond data1(@ app_sec_bond_data_env_tag) |          |
 * --------------------------------------------------------------------------------
 * |                         |               ......                    |          |
 * --------------------------------------------------------------------------------
 */  
struct local_device_bond_data_tag
{
    uint16_t valid_flag;
    uint8_t num;
    struct app_sec_bond_data_env_tag single_peer_bond_data[MAX_BOND_PEER];
};



struct ns_sec_init_t
{
    //Pairing Features 
    struct gapc_pairing pairing_feat;
    
    //pin code for pairing
    uint32_t pin_code;
    bool     rand_pin_enable;
    
    //config bond
    uint8_t  bond_enable;
    uint8_t  bond_max_peer;
    uint16_t bond_sync_delay;
    uint32_t bond_db_addr;
    
    //sec msg hanlder
    void (*ns_sec_msg_handler)(struct sec_msg_t const*);    
    
};

struct app_sec_env_tag
{
    // Bond status
    uint8_t  bonded;
    uint16_t store_latency;
    //Pairing Features 
    struct ns_sec_init_t sec_init;
};

/* Public define ------------------------------------------------------------*/ 
/* Public constants ---------------------------------------------------------*/
/* Public variables ---------------------------------------------------------*/
extern const struct app_subtask_handlers app_sec_handlers;
/* Public function prototypes -----------------------------------------------*/

/**
 * @brief  Initialize the Application Security Module
 */
void ns_sec_init(struct ns_sec_init_t const* init);

/**
 * @brief Send a security request to the peer device. This function is used to require the
 * central to start the encryption with a LTK that would have shared during a previous
 * bond procedure.
 *
 * @param[in]   - conidx: Connection Index
 */
void ns_sec_send_security_req(uint8_t conidx);

/**
 * @brief  start bond by central 
 */
void ns_sec_send_bond_start(uint8_t conidx);

/**
 * @brief  start encrypt request by central
 */
void ns_sec_send_encrypt_req(uint8_t conidx);

/**
 * @brief  respond the pincode to remote when get GAPC_TK_EXCH message 
 *         and the tk_type is GAP_TK_KEY_ENTRY.
 */
void ns_sec_pincode_respond(uint8_t conidx, uint32_t pincode);
/**
 * @brief  get the bond status
 */
bool ns_sec_get_bond_status(void);
/**
 * @brief  erase all bond information
 */
void ns_sec_bond_db_erase_all(void);

/**
 * @brief  store bond information event hander
 */
int  ns_sec_bond_store_evt_handler(ke_msg_id_t const msgid, void *p_param, \
                                  ke_task_id_t const dest_id, ke_task_id_t const src_id);

uint8_t ns_sec_get_iocap(void);

void ns_bond_last_bonded_addr(struct gap_bdaddr *p_addr);
void ns_bond_last_bonded_peer_id(struct gap_bdaddr *p_addr);
void ns_bond_last_bonded_ral_info(struct gap_ral_dev_info *ral_list);
void ns_bond_resolv_addr_start(struct bd_addr* addr, uint8_t idx);
void ns_bond_search_addr_irk(struct bd_addr* addr,uint8_t addr_type);
#endif //(BLE_APP_SEC)

#endif // APP_SEC_H_

/// @} NS_SEC
