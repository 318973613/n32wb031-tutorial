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
 * @file ns_dfu_serial.c
 * @author NSING Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/** @addtogroup 
 * @{
 */

 /* Includes ------------------------------------------------------------------*/
#include "ns_dfu_serial.h"
#include "ns_scheduler.h"
#include "ns_dfu_boot.h"
#include "dfu_usart.h"
#include "n32wb03x.h"
#include "ns_error.h"
#include "dfu_delay.h"
#include "dfu_crc.h"
/* Private typedef -----------------------------------------------------------*/
static struct
{
    uint8_t buffer[4096];
    uint32_t offset;
}m_pkt;

typedef struct 
{
    uint32_t crc;
    uint32_t app_start_address;
    uint32_t app_size;
    uint32_t app_crc;
    uint32_t app_version;
    uint32_t reserve[10];
}_init_pkt;
static _init_pkt m_init_pkt;
typedef struct 
{
    uint32_t offset;
    uint32_t size;
    uint32_t crc;
}_pkt_header;

/* Private define ------------------------------------------------------------*/
#define  DFU_SERIAL_HEADER                      0xAA
#define  DFU_SERIAL_CMD_Ping                    0x01
#define  DFU_SERIAL_CMD_InitPkt                 0x02
#define  DFU_SERIAL_CMD_Pkt_header              0x03
#define  DFU_SERIAL_CMD_Pkt                     0x04
#define  DFU_SERIAL_CMD_PostValidate            0x05
#define  DFU_SERIAL_CMD_ActivateReset           0x06
#define  DFU_SERIAL_CMD_JumpToMasterBoot        0x07

#define SCHED_EVT_RX_DATA            1
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static _pkt_header m_pkt_header;
static NS_Bootsetting_t m_ns_bootsetting;
/* Private function prototypes -----------------------------------------------*/
static void sched_evt(void * p_event_data, uint16_t event_size);
static uint32_t serial_send_data(uint8_t *p_data, uint32_t length);
static uint8_t m_buffer[256];
static void dfu_serial_cmd_ping(void);
static void dfu_serial_cmd_init_pkt(void);
static void dfu_serial_cmd_pkt_header(void);
static void dfu_serial_cmd_pkt(void);
static void dfu_serial_cmd_postvalidate(void);
static void dfu_serial_cmd_activate_reset(void);
static void dfu_serial_cmd_jump_to_master_boot(void);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief Initialize serial dfu.
 * @param[in] none.
 * @return none
 */
void ns_dfu_serial_init(void)
{
    Qflash_Init();
    if(ns_bootsetting.crc == dfu_crc32((uint8_t *)&ns_bootsetting.crc + 4, sizeof(NS_Bootsetting_t) - 4))
    {
        if(ns_bootsetting.master_boot_force_update == NS_BOOTSETTING_MASTER_BOOT_FORCE_UPDATE_YES)
        {
            NS_Bootsetting_t ns_bootsetting_tmp;
            do{
                memcpy(&ns_bootsetting_tmp,&ns_bootsetting,sizeof(NS_Bootsetting_t));
                ns_bootsetting_tmp.master_boot_force_update = NS_BOOTSETTING_MASTER_BOOT_FORCE_UPDATE_NO;
                ns_bootsetting_tmp.crc = dfu_crc32((uint8_t *)&ns_bootsetting_tmp.crc + 4, sizeof(NS_Bootsetting_t) - 4);
                Qflash_Erase_Sector(NS_BOOTSETTING_START_ADDRESS);
                Qflash_Write(NS_BOOTSETTING_START_ADDRESS, (uint8_t *)&ns_bootsetting_tmp, sizeof(NS_Bootsetting_t));            
            }while(ns_bootsetting_tmp.crc != dfu_crc32((uint8_t *)((uint32_t *)(NS_BOOTSETTING_START_ADDRESS + 4)), sizeof(NS_Bootsetting_t) - 4));
        }    
    }
    
    
    dfu_usart1_interrupt_config();
    dfu_usart1_enable();
}



/**
 * @brief Process data received from serial port.
 * @param[in] p_event_data event type.
 * @param[in] event_size event size.
 * @return none
 */
static void sched_evt(void * p_event_data, uint16_t event_size)
{
    switch(*(uint8_t *)p_event_data)
    {
        case SCHED_EVT_RX_DATA:{
            if(m_buffer[0] == DFU_SERIAL_HEADER)
            {
                switch(m_buffer[1]){
                
                    case DFU_SERIAL_CMD_Ping:{
                        dfu_serial_cmd_ping();
                    }break;
                    case DFU_SERIAL_CMD_InitPkt:{
                        dfu_serial_cmd_init_pkt();
                    }break;
                    case DFU_SERIAL_CMD_Pkt_header:{
                        dfu_serial_cmd_pkt_header();
                    }break;
                    case DFU_SERIAL_CMD_Pkt:{
                        dfu_serial_cmd_pkt();
                    }break;
                    case DFU_SERIAL_CMD_PostValidate:{
                        dfu_serial_cmd_postvalidate();
                    }break;
                    case DFU_SERIAL_CMD_ActivateReset:{
                        dfu_serial_cmd_activate_reset();
                    }break;                    
                    case DFU_SERIAL_CMD_JumpToMasterBoot:{
                        dfu_serial_cmd_jump_to_master_boot();
                    }break;            
                }
            }
        }break;
    }
}
/**
 * @brief Ping the board.
 * @param[in] none.
 * @return none
 */
static void dfu_serial_cmd_ping(void)
{
    uint8_t cmd[] = {DFU_SERIAL_HEADER,DFU_SERIAL_CMD_Ping};
    serial_send_data(cmd, sizeof(cmd));
}
/**
 * @brief Process init packet.
 * @param[in] none.
 * @return none
 */
static void dfu_serial_cmd_init_pkt(void)
{
    uint8_t error = 0;
    memcpy(&m_init_pkt,m_buffer+2,sizeof(_init_pkt));
    if(m_init_pkt.crc != dfu_crc32((uint8_t *)&m_init_pkt.crc + 4, sizeof(_init_pkt) - 4))
    {
        error = 1;
    }
    if(m_init_pkt.app_start_address < NS_BOOTSETTING_START_ADDRESS)
    {
        error = 1;
    }
    uint8_t cmd[] = {DFU_SERIAL_HEADER,DFU_SERIAL_CMD_InitPkt,error};
    serial_send_data(cmd, sizeof(cmd));
}
/**
 * @brief Process packet header.
 * @param[in] none.
 * @return none
 */
static void dfu_serial_cmd_pkt_header(void)
{
    m_pkt.offset = 0;
    uint8_t error = 0;
    memcpy(&m_pkt_header,m_buffer+2,sizeof(_pkt_header));
    uint8_t cmd[] = {DFU_SERIAL_HEADER,DFU_SERIAL_CMD_Pkt_header,error};
    serial_send_data(cmd, sizeof(cmd));    
}
/**
 * @brief Process packet data.
 * @param[in] none.
 * @return none
 */
static void dfu_serial_cmd_pkt(void)
{    
    uint8_t error = 0;
    memcpy(m_pkt.buffer+m_pkt.offset,m_buffer+3,m_buffer[2]);    
    m_pkt.offset += m_buffer[2];
    
    if(m_pkt.offset >= m_pkt_header.size)
    {
        if(m_pkt_header.crc == dfu_crc32(m_pkt.buffer, m_pkt_header.size))
        {
            Qflash_Erase_Sector(m_init_pkt.app_start_address + m_pkt_header.offset);
            Qflash_Write(m_init_pkt.app_start_address + m_pkt_header.offset, m_pkt.buffer, m_pkt_header.size);
            if(m_pkt_header.crc != dfu_crc32((uint8_t *)((uint32_t *)(m_init_pkt.app_start_address + m_pkt_header.offset)), m_pkt_header.size))
            {
                error = 1;
            }
        }
        else
        {
            error = 1;
        }
    }
    uint8_t cmd[] = {DFU_SERIAL_HEADER,DFU_SERIAL_CMD_Pkt,error};
    serial_send_data(cmd, sizeof(cmd));    
}
/**
 * @brief Validate receviced data.
 * @param[in] none.
 * @return none
 */
static void dfu_serial_cmd_postvalidate(void)
{
    uint8_t error = 0;
    
    if(m_init_pkt.app_crc != dfu_crc32((uint8_t *)((uint32_t *)m_init_pkt.app_start_address), m_init_pkt.app_size))
    {
        error = 1;
    }
    else{
        memcpy(&m_ns_bootsetting,&ns_bootsetting,sizeof(NS_Bootsetting_t));
        
        m_ns_bootsetting.app1.activation = NS_BOOTSETTING_ACTIVATION_NO;
        m_ns_bootsetting.app2.activation = NS_BOOTSETTING_ACTIVATION_NO;
        m_ns_bootsetting.ImageUpdate.activation = NS_BOOTSETTING_ACTIVATION_NO;
        m_ns_bootsetting.master_boot_force_update = NS_BOOTSETTING_MASTER_BOOT_FORCE_UPDATE_NO;
        if(m_init_pkt.app_start_address == NS_APP1_START_ADDRESS)
        {
            m_ns_bootsetting.app1.start_address = NS_APP1_START_ADDRESS;
            m_ns_bootsetting.app1.size = m_init_pkt.app_size;
            m_ns_bootsetting.app1.version = m_init_pkt.app_version;
            m_ns_bootsetting.app1.crc = m_init_pkt.app_crc;
            m_ns_bootsetting.app1.activation = NS_BOOTSETTING_ACTIVATION_YES;
        }
        else if(m_init_pkt.app_start_address == NS_APP2_START_ADDRESS)
        {
            m_ns_bootsetting.app2.start_address = NS_APP2_START_ADDRESS;
            m_ns_bootsetting.app2.size = m_init_pkt.app_size;
            m_ns_bootsetting.app2.version = m_init_pkt.app_version;
            m_ns_bootsetting.app2.crc = m_init_pkt.app_crc;
            m_ns_bootsetting.app2.activation = NS_BOOTSETTING_ACTIVATION_YES;        
        }
        else if(m_init_pkt.app_start_address == NS_IMAGE_UPDATE_START_ADDRESS)
        {
            m_ns_bootsetting.ImageUpdate.start_address = NS_IMAGE_UPDATE_START_ADDRESS;
            m_ns_bootsetting.ImageUpdate.size = m_init_pkt.app_size;
            m_ns_bootsetting.ImageUpdate.version = m_init_pkt.app_version;
            m_ns_bootsetting.ImageUpdate.crc = m_init_pkt.app_crc;
            m_ns_bootsetting.ImageUpdate.activation = NS_BOOTSETTING_ACTIVATION_YES;        
        }  
        else
        {
            error = 1;
        }
        
        if(error == 0){
            m_ns_bootsetting.crc = dfu_crc32((uint8_t *)&m_ns_bootsetting.crc + 4, sizeof(NS_Bootsetting_t) - 4);
            Qflash_Erase_Sector(NS_BOOTSETTING_START_ADDRESS);
            Qflash_Write(NS_BOOTSETTING_START_ADDRESS, (uint8_t *)&m_ns_bootsetting, sizeof(NS_Bootsetting_t));
            if(m_ns_bootsetting.crc != dfu_crc32((uint8_t *)((uint32_t *)(NS_BOOTSETTING_START_ADDRESS + 4)), sizeof(NS_Bootsetting_t) - 4))
            {
                error = 2;
            }        
        }
        
    }
    uint8_t cmd[] = {DFU_SERIAL_HEADER,DFU_SERIAL_CMD_PostValidate,error};
    serial_send_data(cmd, sizeof(cmd));        
}



/**
 * @brief Reset System.
 * @param[in] none.
 * @return none
 */
static void dfu_serial_cmd_activate_reset(void)
{
    uint8_t error = 0;

    uint8_t cmd[] = {DFU_SERIAL_HEADER,DFU_SERIAL_CMD_ActivateReset,error};
    serial_send_data(cmd, sizeof(cmd));        
    
    if(error == 0)
    {
        dfu_delay_ms(100);
        NVIC_SystemReset();
        
    }
    
    
}





static void dfu_serial_cmd_jump_to_master_boot(void)
{
    uint8_t cmd[] = {DFU_SERIAL_HEADER,DFU_SERIAL_CMD_JumpToMasterBoot};
    serial_send_data(cmd, sizeof(cmd));
}






void USART1_IRQHandler(void)
{
    static uint32_t index = 0;
    static uint8_t buffer[256];
    
    if(USART_GetFlagStatus(USART1, USART_FLAG_RXDNE) != RESET)
    {
        buffer[index] = USART_ReceiveData(USART1);
        
        if(buffer[0] == DFU_SERIAL_HEADER)
        {
            index++;
            if(index >= 256)
            {
                index = 0;    
                memset(m_buffer,0,sizeof(m_buffer));
                memcpy(m_buffer,buffer, 256);
                
                uint8_t event = SCHED_EVT_RX_DATA;
                uint32_t    err_code = app_sched_event_put(&event ,sizeof(uint8_t),sched_evt);
                ERROR_CHECK(err_code);
            }                            
        }
    }    
}



static uint32_t serial_send_data(uint8_t *p_data, uint32_t length)
{
    uint8_t cmd[256];
    memset(cmd,0,sizeof(cmd));
    memcpy(cmd,p_data,length);
    dfu_usart1_send(cmd,sizeof(cmd));
    return 0;
}






