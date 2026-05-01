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
 * @file app_fifo.c
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include "app_fifo.h"
#include "app_rdtss.h"
#include "rwprf_config.h"
#include "string.h"
#include "ke_timer.h"
#include "rdtss_task.h"

/** @addtogroup 
 * @{
 */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

uint8_t  master_rx_fifo_buf[MASTER_RX_FIFO_SIZE] = {0};
uint32_t master_rx_fifo_in = 0;
uint32_t master_rx_fifo_out = 0;
bool sending_to_master;
uint16_t num_send_to_peer_master = 20;      //MTU-3

/**
 * @brief  pop own master RX data, send data to peer master
 */
void pop_matser_rx_to_ble(void)
{
    uint32_t in_temp;
    uint16_t ble_send_len;
    
    in_temp = master_rx_fifo_in;
    if(master_rx_fifo_out < in_temp)
    {
        ble_send_len = in_temp-master_rx_fifo_out;
    }
    else if(master_rx_fifo_out > in_temp){
        ble_send_len = MASTER_RX_FIFO_SIZE-master_rx_fifo_out;
    }
    else if(master_rx_fifo_out == in_temp){
        // fifo empty, stop send loop
        sending_to_master = false;
        return;
    }
    if(ble_send_len > num_send_to_peer_master)
    {
        ble_send_len = num_send_to_peer_master;
    }
    sending_to_master = true;    
    #if (BLE_RDTSS_SERVER)
    rdtss_send_notify(  &master_rx_fifo_buf[master_rx_fifo_out], ble_send_len);
    #endif
    master_rx_fifo_out = (master_rx_fifo_out+ble_send_len)%MASTER_RX_FIFO_SIZE;

    return;
}

/**
 * @brief  push master RX data to fifo and active ble send first package if not active yet
 */
uint8_t push_master_rx_data_to_fifo(const uint8_t *p_data, uint16_t len)
{
    uint32_t in_len;
    //store data in fifo
    while(len)
    {
        if(master_rx_fifo_in >= master_rx_fifo_out )
        {
            in_len = MASTER_RX_FIFO_SIZE-master_rx_fifo_in;
            if(in_len > len)
            {
                in_len = len;
            }
            memcpy(&master_rx_fifo_buf[master_rx_fifo_in],p_data,in_len);
            len = len-in_len;
            p_data += in_len;
            master_rx_fifo_in = (master_rx_fifo_in + in_len)%MASTER_RX_FIFO_SIZE;
            
        }
        else if(master_rx_fifo_in < master_rx_fifo_out )
        {
            in_len = master_rx_fifo_out-master_rx_fifo_in-1;
            if(in_len > len)
            {
                in_len = len;
            }
            memcpy(&master_rx_fifo_buf[master_rx_fifo_in],p_data,in_len);
            len = len-in_len;
            master_rx_fifo_in = (master_rx_fifo_in + in_len)%MASTER_RX_FIFO_SIZE;
            
            //fifo full,drop the rest data 
            if(len)
            {
                NS_LOG_WARNING("F:%d,%d,%d\r\n",len,master_rx_fifo_in,master_rx_fifo_out);
            }
            break;
        }
    }
    
    if(master_rx_fifo_in != master_rx_fifo_out)       //data need to send
    {
        if(!sending_to_master)
        {
            ke_timer_set(RDTSS_VAL_NTF_CFM, TASK_APP, 10);
        }
    }

    return len;
}



/**
 * @}
 */
