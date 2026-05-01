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
 * @file ns_ble.h
 * @author NSING Firmware Team
 * @version v1.0.2
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */


#ifndef APP_H_
#define APP_H_

/**
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Application entry point.
 *
 * @{
 **/

/* Includes ------------------------------------------------------------------*/

#include "rwip_config.h"     // SW configuration

#if (BLE_APP_PRESENT)

#include <stdint.h>          // Standard Integer Definition
#include "arch.h"            // Platform Definitions
#include <co_bt.h>           // Common BT Definitions


#include "gapc_task.h"       // GAPC Definitions
#include "gattc_task.h"       // GATTC Definitions

#if (NVDS_SUPPORT)
#include "nvds.h"
#endif // (NVDS_SUPPORT)


/* Define ------------------------------------------------------------*/




#define APP_ADV_DURATION_MAX           (655)

#define SECS_UNIT_1_25_MS              (800)
#define SECS_UNIT_10MS                 (100)
#define SECS_TO_UNIT(sec,uint)         ((sec)*(uint))

#define MSECS_UNIT_1_25_MS              (1250)
#define MSECS_UNIT_10_MS                (10000)
#define MSECS_TO_UNIT(msec,uint)        ((msec*1000)/(uint))

/// Maximal length of the Device Name value
#define APP_DEVICE_NAME_MAX_LEN      (18)
#define APP_MESH_DEMO_TYPE_LEN        (1)

/*
 * MACROS
 **/

#define APP_HANDLERS(subtask)    {&subtask##_msg_handler_list[0], ARRAY_LEN(subtask##_msg_handler_list)}

/* Typedef -----------------------------------------------------------*/

#if (NVDS_SUPPORT)
/// List of Application NVDS TAG identifiers
enum app_nvds_tag
{
    /// Device Name
    NVDS_TAG_DEVICE_NAME                = 0x02,
    NVDS_LEN_DEVICE_NAME                = 62,

    /// BD Address
    NVDS_TAG_BD_ADDRESS                 = 0x01,
    NVDS_LEN_BD_ADDRESS                 = 6,

    /// Local device Identity resolving key
    NVDS_TAG_LOC_IRK                    = 0xA0,
    NVDS_LEN_LOC_IRK                    = KEY_LEN,



    #if (BLE_APP_PRF)
    /// BLE Application Advertising data
    NVDS_TAG_APP_BLE_ADV_DATA           = 0x0B,
    NVDS_LEN_APP_BLE_ADV_DATA           = 32,

    /// BLE Application Scan response data
    NVDS_TAG_APP_BLE_SCAN_RESP_DATA     = 0x0C,
    NVDS_LEN_APP_BLE_SCAN_RESP_DATA     = 32,

    /// Mouse Sample Rate
    NVDS_TAG_MOUSE_SAMPLE_RATE          = 0x38,
    NVDS_LEN_MOUSE_SAMPLE_RATE          = 1,

    /// Peripheral Bonded
    NVDS_TAG_PERIPH_BONDED              = 0x39,
    NVDS_LEN_PERIPH_BONDED              = 1,

    /// Mouse NTF Cfg
    NVDS_TAG_MOUSE_NTF_CFG              = 0x3A,
    NVDS_LEN_MOUSE_NTF_CFG              = 2,

    /// Mouse Timeout value
    NVDS_TAG_MOUSE_TIMEOUT              = 0x3B,
    NVDS_LEN_MOUSE_TIMEOUT              = 2,

    /// Peer Device BD Address
    NVDS_TAG_PEER_BD_ADDRESS            = 0x3C,
    NVDS_LEN_PEER_BD_ADDRESS            = 7,

    /// Mouse Energy Safe
    NVDS_TAG_MOUSE_ENERGY_SAFE          = 0x3D,
    NVDS_LEN_MOUSE_SAFE_ENERGY          = 2,

    /// EDIV (2bytes), RAND NB (8bytes),  LTK (16 bytes), Key Size (1 byte)
    NVDS_TAG_LTK                        = 0x3E,
    NVDS_LEN_LTK                        = 28,

    /// PAIRING
    NVDS_TAG_PAIRING                    = 0x3F,
    NVDS_LEN_PAIRING                    = 54,

    /// Audio mode 0 task
    NVDS_TAG_AM0_FIRST                  = 0x90,
    NVDS_TAG_AM0_LAST                   = 0x9F,

    /// Peer device Resolving identity key (+identity address)
    NVDS_TAG_PEER_IRK                   = 0xA1,
    NVDS_LEN_PEER_IRK                   = sizeof(struct gapc_irk),
    
    //TODO
#endif //(BLE_APP_PRF)
};
#endif // (NVDS_SUPPORT)


/// Advertising state machine
enum app_adv_state
{
    /// Advertising activity does not exists
    APP_ADV_STATE_IDLE = 0,
    #if BLE_APP_PRF
    /// Creating advertising activity
    APP_ADV_STATE_CREATING,
    /// Setting advertising data
    APP_ADV_STATE_SETTING_ADV_DATA,
    /// Setting scan response data
    APP_ADV_STATE_SETTING_SCAN_RSP_DATA,

    /// Advertising activity created
    APP_ADV_STATE_CREATED,
    /// Starting advertising activity
    APP_ADV_STATE_STARTING,
    /// Advertising activity started
    APP_ADV_STATE_STARTED,
    /// Stopping advertising activity
    APP_ADV_STATE_STOPPING,
    #endif //(BLE_APP_PRF)
};

enum app_adv_mode
{
      APP_ADV_MODE_IDLE = 0,
        APP_ADV_MODE_DIRECTED_HIGH_DUTY,
      APP_ADV_MODE_DIRECTED,
        APP_ADV_MODE_FAST,
        APP_ADV_MODE_SLOW,
    
};
typedef enum
{
    BLE_LSC_LSI_32000HZ = 0,
    BLE_LSC_LSI_32768HZ,
    BLE_LSC_LSI_28800HZ,
    BLE_LSC_LSE_32768HZ
}ble_lsc_cfg_t;
/* Public define ------------------------------------------------------------*/

/// Structure containing information about the handlers for an application subtask
struct app_subtask_handlers
{
    /// Pointer to the message handler table
    const struct ke_msg_handler *p_msg_handler_tab;
    /// Number of messages handled
    uint16_t msg_cnt;
};

/// Application environment structure
struct app_env_tag
{
    /// Connection handle
    uint16_t conhdl;
    /// Connection Index
    uint8_t  conidx;

    /// Advertising activity index
    uint8_t adv_actv_idx;
    /// Current advertising state (@see enum app_adv_state)
    uint8_t adv_state;
    /// Next expected operation completed event
    uint8_t adv_op;

    /// Last initialized profile
    uint8_t next_svc;

    /// Bonding status
    bool bonded;

    /// Device Name length
    uint8_t dev_name_len;
    /// Device Name
    uint8_t dev_name[APP_DEVICE_NAME_MAX_LEN];

    /// Local device IRK
    uint8_t loc_irk[KEY_LEN];

    /// Secure Connections on current link
    bool sec_con_enabled;

    /// Counter used to generate IRK
    uint8_t rand_cnt;
        
        /// Advertising mode
    uint8_t adv_mode;
        /// Maximum device MTU size
    uint16_t max_mtu;

    /// Demonstration type length
    uint8_t demo_type_len;
    /// Demonstration type
    uint8_t demo_type;
        
    uint8_t manual_conn_param_update;
    uint8_t manual_mtu_update;
    ble_lsc_cfg_t lsc_cfg;
    
    uint8_t scan_actv_idx;
    uint8_t init_actv_idx;
};
struct ns_scan_params_t
{  
    /// Initiating type (@see enum gapm_init_type)
    uint8_t type;
    // if enable scan
    uint8_t scan_enable         :1;   
    // if connect the device if match
    uint8_t connect_enable      :1;    
    /// if use coded phy mode  
    uint8_t phy_coded_enable    :1;    
    /// Properties for the scan active mode, enable will require scan respond data 
    uint8_t prop_active_enable  :1;
    /// Duplicate packet filtering policy
    uint8_t dup_filt_pol        :2;
    /// Scan interval
    uint16_t scan_intv;
    /// Scan window
    uint16_t scan_wd;
    /// Scan duration (in unit of 10ms). 0 means that the controller will scan continuously until
    /// reception of a stop command from the application
    uint16_t duration;
    /// Scan period (in unit of 1.28s). Time interval betweem two consequent starts of a scan duration
    /// by the controller. 0 means that the scan procedure is not periodic
    uint16_t period;
    
//    void (*ble_scan_state_handler)(enum scan_state_t);
//    void (*ble_scan_data_handler)(struct gapm_ext_adv_report_ind const*);

//    enum scan_filter_type filter_type;
//    const uint8_t         *filter_data;
    
};
/* Public variables ---------------------------------------------------------*/

/// Application environment
extern struct app_env_tag app_env;
extern struct ns_scan_params_t scan_env;
/* Public function prototypes -----------------------------------------------*/

/**
 * @brief Initialize the BLE demo application.
 **/
void app_init(void);

/**
 * @brief Add a required service in the database
 **/
bool app_add_svc(void);

/**
 * @brief Retrieve device name
 *
 * @param[out] Pointer at which device name will be returned
 *
 * @return Name length
 **/
uint8_t app_get_dev_name(uint8_t* p_name);

#if (BLE_APP_PRF)
/**
 * @brief
 **/
void app_adv_fsm_next(void);

/**
 * @brief Send to request to update the connection parameters
 **/
void ns_ble_update_param(struct gapc_conn_param *p_conn_param);

/**
 * @brief Send a disconnection request
 **/
void app_disconnect(void);

/**
 * @brief Start/stop advertising
 *
 * @param[in] start     True if advertising has to be started, else false
 **/

void app_update_adv_state(bool start);

/**
 * @brief delete advertising
 *
 * @param[in] none
 **/

void app_delete_advertising(void);
/**
 * @brief Return if the device is currently bonded
 **/
bool app_sec_get_bond_status(void);

/**
 * @brief set mtu
 *
 * @param[in] none
 **/
void ns_ble_mtu_set(uint16_t mtu);

/// @} APP
///
#endif //(BLE_APP_PRF)
#endif //(BLE_APP_PRESENT)

#endif // APP_H_
