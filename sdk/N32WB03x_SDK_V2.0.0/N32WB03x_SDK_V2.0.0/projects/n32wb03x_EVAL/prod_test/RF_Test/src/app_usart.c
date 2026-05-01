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
 * @file app_usart.c
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

#include "global_func.h"
#include "app_usart.h"
#include "ns_log.h"
#include "ns_delay.h"
#include "app_ble.h"
/** @addtogroup 
 * @{
 */
/* Private typedef -----------------------------------------------------------*/
typedef enum
{
    STATE_UNINITIALIZED,
    STATE_IDLE,
    STATE_TRANSMITTER_TEST,
    STATE_CARRIER_TEST,
    STATE_RECEIVER_TEST
} state_t;
/* Private define ------------------------------------------------------------*/

//#define LE_PACKET_REPORTING_EVENT       0x8000                             
#define RF_TEST_STATUS_EVENT_SUCCESS    0x0000                             
#define RF_TEST_STATUS_EVENT_ERROR      0x0001                             

#define CMD_RSP_SUCCESS                     0x00                               
#define CMD_RSP_ERROR_ILLEGAL_CHANNEL       0x01                               
#define CMD_RSP_ERROR_INVALID_STATE         0x02                               
#define CMD_RSP_ERROR_ILLEGAL_LENGTH        0x03                               
#define CMD_RSP_ERROR_ILLEGAL_CONFIGURATION 0x04                               
#define CMD_RSP_ERROR_UNINITIALIZED         0x05    

#define RF_TEST_SETUP                   0                                  
#define RF_RECEIVER_TEST                1                                  
#define RF_TRANSMITTER_TEST             2                                  
#define RF_TEST_END                     3

#define RF_TEST_SETUP_RESET             0                                  
#define RF_TEST_SETUP_SET_UPPER         1                                  
#define RF_TEST_SETUP_SET_PHY           2                                  
#define RF_TEST_SETUP_SELECT_MODULATION 3                                  
#define RF_TEST_SETUP_READ_SUPPORTED    4                                  
#define RF_TEST_SETUP_READ_MAX          5                                  
                              
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t USART_rx_dma_buf[USART_RX_DMA_SIZE] = {0};
uint16_t rx_old_pos = 0;

uint8_t  usart_rx_fifo_buf[USART_RX_FIFO_SIZE] = {0};
uint32_t usart_rx_fifo_in = 0;
uint32_t usart_rx_fifo_out = 0;


uint8_t  usart_tx_fifo_buf[USART_TX_FIFO_SIZE] = {0};
uint32_t usart_tx_fifo_in = 0;
uint32_t usart_tx_fifo_out = 0;

uint8_t usart_sending = false;
uint8_t ble_sending   = false;
uint16_t ble_att_mtu = (ATT_DEFAULT_MTU-3);

//rf test
struct gapm_le_test_mode_ctrl_cmd   m_rf_params;
static uint16_t          m_event;
static bool              m_new_event; 
static uint16_t          m_state = STATE_UNINITIALIZED;
uint8_t                  m_stop_rx_rsp = false;

static uint8_t uart_cmd[2]; 
/* Private function prototypes -----------------------------------------------*/
void app_usart_tx_process(void);
extern void cwtx_test(uint8_t channel);
/* Private functions ---------------------------------------------------------*/


/**
 * @brief  Configures the different system clocks.
 */
void RCC_Configuration_USART(void)
{
    /* Enable GPIO clock */
    GPIO_APBxClkCmd(USARTx_GPIO_CLK, ENABLE);
    /* Enable USARTx Clock */
    USART_APBxClkCmd(USARTx_CLK, ENABLE);
        /* DMA clock enable */
    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA, ENABLE);
}

/**
 * @brief  Configures the different GPIO ports.
 */
void GPIO_Configuration_USART(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Initialize GPIO_InitStructure */
    GPIO_InitStruct(&GPIO_InitStructure);    

    /* Configure USARTx Tx as alternate function push-pull */
    GPIO_InitStructure.Pin            = USARTx_TxPin;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = USARTx_Tx_GPIO_AF;
    GPIO_InitPeripheral(USARTx_GPIO, &GPIO_InitStructure);   

    /* Configure USARTx Rx as alternate function push-pull */
    GPIO_InitStructure.Pin            = USARTx_RxPin;
    GPIO_InitStructure.GPIO_Alternate = USARTx_Rx_GPIO_AF;
    GPIO_InitPeripheral(USARTx_GPIO, &GPIO_InitStructure); 
}

/**
 * @brief  Configures the DMA.
 */
void DMA_Configuration(void)
{
    DMA_InitType DMA_InitStructure;

    /* USARTx_Tx_DMA_Channel (triggered by USARTx Tx event) Config */
    DMA_DeInit(USARTx_Tx_DMA_Channel);
    DMA_RequestRemap(USARTx_Tx_DMA_REMAP, DMA, USARTx_Tx_DMA_Channel, ENABLE);    
    DMA_ConfigInt(USARTx_Tx_DMA_Channel,DMA_INT_TXC ,ENABLE);

    /* USARTx RX DMA1 Channel (triggered by USARTx Rx event) Config */
    DMA_DeInit(USARTx_Rx_DMA_Channel);
    DMA_RequestRemap(USARTx_Rx_DMA_REMAP, DMA, USARTx_Rx_DMA_Channel, ENABLE);        
    DMA_ConfigInt(USARTx_Rx_DMA_Channel,DMA_INT_TXC|DMA_INT_HTX ,ENABLE);
        
    DMA_InitStructure.PeriphAddr     = USARTx_DAT_Base;
    DMA_InitStructure.MemAddr        = (uint32_t)USART_rx_dma_buf;
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_SRC;
    DMA_InitStructure.BufSize        = USART_RX_DMA_SIZE;
    DMA_InitStructure.CircularMode   = DMA_MODE_CIRCULAR;
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc  = DMA_MEM_INC_ENABLE;
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_SIZE_BYTE;
    DMA_InitStructure.MemDataSize    = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.Priority       = DMA_PRIORITY_VERY_HIGH;
    DMA_InitStructure.Mem2Mem        = DMA_M2M_DISABLE;
    DMA_Init(USARTx_Rx_DMA_Channel, &DMA_InitStructure);

}


/**
 * @brief  Configures the nested vectored interrupt controller.
 */
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;

    NVIC_DisableIRQ(DMA_Channel1_2_3_4_IRQn);
    /* Enable the DMA Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = DMA_Channel1_2_3_4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority           = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_DisableIRQ(USARTx_IRQn);
    /* Enable the DMA Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = USARTx_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority           = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/**
 * @brief  Configures the USART as 115200 8n1.
 */
void app_usart_configuration(void)
{

    USART_InitType USART_InitStructure;
         
    /* Configure and enable RCC */
    RCC_Configuration_USART();
    /* Configure GPIO for USART */
    GPIO_Configuration_USART();

    /* Configure the DMA */
    DMA_Configuration();

    NVIC_Configuration();
    
    /* USARTx and USARTz configuration ------------------------------------------------------*/
    USART_InitStructure.BaudRate            = 19200;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;

    /* Configure USARTx */
    USART_Init(USARTx, &USART_InitStructure);
        
    /* Enable USARTz Receive and Transmit interrupts */
    USART_ConfigInt(USARTx, USART_INT_IDLEF, ENABLE);

    /* Enable USARTx DMA Rx and TX request */
    USART_EnableDMA(USARTx, USART_DMAREQ_RX | USART_DMAREQ_TX, ENABLE);
    
    DMA_EnableChannel(USARTx_Rx_DMA_Channel, ENABLE);
    
    /* Enable the USARTx */
    USART_Enable(USARTx, ENABLE);
    
}

/**
 * @brief  enable or disable the usart dma
 */
void app_usart_dma_enable(FunctionalState Cmd)
{
    if(Cmd == ENABLE)
    {
        rx_old_pos = 0;
        usart_tx_fifo_in = usart_tx_fifo_out = 0; //clean fifo
        usart_rx_fifo_in = usart_rx_fifo_out = 0; //clean fifo
        app_usart_configuration();
        m_state         = STATE_IDLE;
    }
    else{

        USART_Enable(USARTx, DISABLE);
        DMA_EnableChannel(USARTx_Rx_DMA_Channel, DISABLE);
        DMA_EnableChannel(USARTx_Tx_DMA_Channel, DISABLE);
        /* Deinit IO of USART to save power */
        GPIO_InitType GPIO_InitStructure;
        /* Initialize GPIO_InitStructure */
        GPIO_InitStruct(&GPIO_InitStructure);    
        /* Configure USARTx Tx as alternate function push-pull */
        GPIO_InitStructure.Pin            = USARTx_TxPin|USARTx_RxPin;
        GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_ANALOG;
        GPIO_InitStructure.GPIO_Alternate = GPIO_AF0;
        GPIO_InitPeripheral(USARTx_GPIO, &GPIO_InitStructure);  
    }
}

static uint32_t app_rf_tx_power_cmd(uint32_t cmd)
{
    uint8_t new_power8;
    new_power8 = (uint8_t)(cmd & 0xFF);
    new_power8 = (new_power8 & 0x30) != 0 ? (new_power8 | 0xC0) : new_power8;
    switch (new_power8)
    {
        case 0:
            rf_tx_power_set(TX_POWER_0_DBM);
            break;
        case 1:  case 2:
            rf_tx_power_set(TX_POWER_Pos2_DBM);
            break;
        case 3:
            rf_tx_power_set(TX_POWER_Pos3_DBM);
            break;
        case 4:
            rf_tx_power_set(TX_POWER_Pos4_DBM);
            break;
        case 5: case 6: case 7: case 8: 
            rf_tx_power_set(TX_POWER_Pos6_DBM);                 
            break;
        case 0xfc: //-4
            rf_tx_power_set(TX_POWER_Neg2_DBM);   
            break;
        case 0xf8: //-8
            rf_tx_power_set(TX_POWER_Neg8_DBM); 
            break;
        case 0xf4: //-12
        case 0xf0: //-16
            rf_tx_power_set(TX_POWER_Neg15_DBM); 
            break;
        case 0xec: //-20
        case 0xd8: //-40
             rf_tx_power_set(TX_POWER_Neg20_DBM);
            break;
        default:
            m_event = RF_TEST_STATUS_EVENT_ERROR;
            return CMD_RSP_ERROR_ILLEGAL_CONFIGURATION;
    }
    return CMD_RSP_SUCCESS;
}


/**
 * @brief  rf test command handler
 */
uint32_t app_rf_test_cmd(uint32_t cmd, uint32_t freq, uint32_t length, uint32_t payload)
{
    m_rf_params.tx_data_length   = (m_rf_params.tx_data_length&0xC0) |((uint8_t)length & 0x3F);
    m_rf_params.tx_pkt_payload   = payload;
    m_rf_params.channel          = freq;

    if ((m_rf_params.phy == 1 || m_rf_params.phy == 2) && payload == 3)
    {
        m_rf_params.tx_pkt_payload = 0xff;
    }
    
    m_new_event     = true;
    m_event         = RF_TEST_STATUS_EVENT_SUCCESS;

    if (cmd == RF_TEST_SETUP)
    {

        if (freq == RF_TEST_SETUP_RESET)
        {
            if (length != 0x00) 
            {
                m_event = RF_TEST_STATUS_EVENT_ERROR;
                return CMD_RSP_ERROR_ILLEGAL_CONFIGURATION;
            }
            m_rf_params.tx_data_length = 0;
            
            m_rf_params.phy        = 1;
            m_rf_params.slot_dur         = 1;  
            m_rf_params.switching_pattern_len = MIN_SWITCHING_PATTERN_LEN;            
            m_rf_params.operation = GAPM_LE_TEST_STOP;
            ns_ble_prod_test_cmd_send(&m_rf_params,false);//stop, reset
            m_state = STATE_IDLE;
        }
        else if (freq == RF_TEST_SETUP_SET_UPPER)
        {
            if (length > 0x03)
            {
                m_event = RF_TEST_STATUS_EVENT_ERROR;
                return CMD_RSP_ERROR_ILLEGAL_CONFIGURATION;
            }
            m_rf_params.tx_data_length = length << 6;
        }
        else if (freq == RF_TEST_SETUP_SET_PHY)
        {
            switch (length)
            {
                case 1:
                case 2:
                case 3:
                case 4:
                    m_rf_params.phy = length;
                    return CMD_RSP_SUCCESS;

                default:
                    m_event = RF_TEST_STATUS_EVENT_ERROR;
                    return CMD_RSP_ERROR_ILLEGAL_CONFIGURATION;
            }
        }
        else if (freq == RF_TEST_SETUP_SELECT_MODULATION)
        {
            if (length > 0x01) 
            {
                m_event = RF_TEST_STATUS_EVENT_ERROR;
                return CMD_RSP_ERROR_ILLEGAL_CONFIGURATION;
            }
            else{
                m_rf_params.modulation_idx = length;
            }
        }
        else if (freq == RF_TEST_SETUP_READ_SUPPORTED)
        {
            if (length != 0x00) 
            {
                m_event = RF_TEST_STATUS_EVENT_ERROR;
                return CMD_RSP_ERROR_ILLEGAL_CONFIGURATION;
            }
            m_event = 0x0006;
        }
        else if (freq == RF_TEST_SETUP_READ_MAX)
        {
            switch (length)
            {
                case 0x00:
                    // Read supportedMaxTxOctets
                    m_event = 0x01FE;
                    break;
                case 0x01:
                    // Read supportedMaxTxTime
                    m_event = 0x4290;
                    break;
                case 0x02:
                    // Read supportedMaxRxOctets
                    m_event = 0x01FE;
                    break;
                case 0x03:
                    // Read supportedMaxRxTime
                    m_event = 0x4290;
                    break;
                default:
                    m_event = RF_TEST_STATUS_EVENT_ERROR;
                    return CMD_RSP_ERROR_ILLEGAL_CONFIGURATION;
            }
        }
        else
        {
            m_event = RF_TEST_STATUS_EVENT_ERROR;
            return CMD_RSP_ERROR_ILLEGAL_CONFIGURATION;
        }
        return CMD_RSP_SUCCESS;
    }

    if (cmd == RF_TEST_END)
    {
        if (m_state == STATE_IDLE)
        {
            m_event = RF_TEST_STATUS_EVENT_ERROR;
            return CMD_RSP_ERROR_INVALID_STATE;
        }
        else if(m_state == STATE_CARRIER_TEST)
        {
            uart_cmd[0] = 0;
            uart_cmd[1] = 0;
            usart_tx_dma_send(uart_cmd,2);
            delay_n_ms(5);
            NVIC_SystemReset();
        }
        //rx respond in gapm_le_test_end_ind_handler
        m_rf_params.operation = GAPM_LE_TEST_STOP;
        ns_ble_prod_test_cmd_send(&m_rf_params,false);//stop
        //
        if(m_state ==STATE_RECEIVER_TEST)
        {
            m_new_event     = false;
            
            m_stop_rx_rsp = false;
            ke_timer_set(APP_TIMER0, TASK_APP, 30);
        }
        
        m_state = STATE_IDLE;

        return CMD_RSP_SUCCESS;
    }
    
    if (m_state != STATE_IDLE)
    {
        m_event = RF_TEST_STATUS_EVENT_ERROR;
        return CMD_RSP_ERROR_INVALID_STATE;
    }

    if (payload != 3 && m_rf_params.channel > 39)
    {
        m_event = RF_TEST_STATUS_EVENT_ERROR;

        return CMD_RSP_ERROR_ILLEGAL_CHANNEL;
    }

    if (cmd == RF_RECEIVER_TEST)
    {
       
        m_rf_params.operation = GAPM_LE_TEST_RX_START;
        m_rf_params.slot_dur  = 1;
        m_rf_params.switching_pattern_len = MIN_SWITCHING_PATTERN_LEN;
        ns_ble_prod_test_cmd_send(&m_rf_params,false);//rx
        m_state = STATE_RECEIVER_TEST;
        return CMD_RSP_SUCCESS;
    }
    
    if (cmd == RF_TRANSMITTER_TEST)
    {
        switch (m_rf_params.tx_pkt_payload)
        {
            case 0://PRBS9
            case 1://0x0F
            case 2://0x55
                break;
            case 3:
                m_rf_params.tx_pkt_payload = GAP_PKT_PLD_REPEATED_11111111;//4
                break;
            case 0xff:
                if(length == 0 || length ==1)
                {
                    //continuous carrier signal(CW test)
                    m_rf_params.tx_pkt_payload = 0;
                    m_rf_params.operation = GAPM_LE_TEST_TX_START;
                    ns_ble_prod_test_cmd_send(&m_rf_params,true);//tx

                    m_state = STATE_CARRIER_TEST;
                    return CMD_RSP_SUCCESS;
                }else
                if(length == 2)// set tx power command
                {
                    return app_rf_tx_power_cmd(freq);
                }
                else{
                    m_event = RF_TEST_STATUS_EVENT_ERROR;
                    return CMD_RSP_ERROR_ILLEGAL_CONFIGURATION;
                }
                
            default:
                // Parameter error
                m_event = RF_TEST_STATUS_EVENT_ERROR;
                return CMD_RSP_ERROR_ILLEGAL_CONFIGURATION;
        }
        
        m_rf_params.operation = GAPM_LE_TEST_TX_START;
        ns_ble_prod_test_cmd_send(&m_rf_params,false);//tx
        m_state            = STATE_TRANSMITTER_TEST;
    }
    return CMD_RSP_SUCCESS;
}
//uint16_t command;
/**
 * @brief  usart rx data enter fifo and active ble send first package if not active yet
 */
uint8_t app_usart_rx_data_fifo_enter(const uint8_t *p_data, uint16_t len)
{
        
    if(len == 2)
    {
        uint16_t    command      = ((uint16_t)p_data[0]<<8)|p_data[1];
        uint32_t    command_code = (command >> 14) & 0x03;
        uint32_t    freq         = (command >> 8) & 0x3F;
        uint32_t    length       = (command >> 2) & 0x3F;
        uint32_t    payload      = command & 0x03;

        app_rf_test_cmd(command_code, freq, length, payload);
        if(m_new_event)
        {
            m_new_event = false;
            uart_cmd[0] = (m_event>>8)& 0xFF;
            uart_cmd[1] = (m_event)& 0xFF;
            usart_tx_dma_send(uart_cmd,2);
        }
    }
    else{
        uart_cmd[0] = 0;
        uart_cmd[1] = 0;
        usart_tx_dma_send(uart_cmd,2);
        NS_LOG_INFO("cmd len err=%d",len);
    }

    return len;
}

/**
 * @brief  usart send data via dma
 */
uint8_t usart_tx_dma_send(uint8_t *p_data, uint16_t len)
{
    DMA_InitType DMA_InitStructure;

    if(usart_sending)
    {
        return false;
    }
    /* USARTx_Tx_DMA_Channel (triggered by USARTx Tx event) Config */
    DMA_InitStructure.PeriphAddr     = USARTx_DAT_Base;
    DMA_InitStructure.MemAddr        = (uint32_t)p_data;
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_DST;
    DMA_InitStructure.BufSize        = len;
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc  = DMA_MEM_INC_ENABLE;
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_SIZE_BYTE;
    DMA_InitStructure.MemDataSize    = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.CircularMode   = DMA_MODE_NORMAL; 
    DMA_InitStructure.Priority       = DMA_PRIORITY_VERY_HIGH;
    DMA_InitStructure.Mem2Mem        = DMA_M2M_DISABLE;
    DMA_Init(USARTx_Tx_DMA_Channel, &DMA_InitStructure);

    usart_sending = true;
    DMA_EnableChannel(USARTx_Tx_DMA_Channel, ENABLE);
    
    return true;
}

/**
 * @brief  usart send data in blocking mode
 */
void usart_tx_data_blocking(uint8_t *p_data, uint16_t len)
{
    while(len--)
    {
         USART_SendData(USARTx, *p_data);
         p_data++;
    }
}

/**
 * @brief  usart tx fifo enter and active dma send out
 */
uint8_t app_usart_tx_fifo_enter(const uint8_t *p_data, uint16_t len)
{
    uint32_t in_len, out_temp;
    //store data in fifo
    NS_LOG_DEBUG("%d,%d,%d\r\n",len,usart_tx_fifo_in,usart_tx_fifo_out);
    out_temp = usart_tx_fifo_out;
    while(len)
    {
        if(usart_tx_fifo_in >= out_temp )
        {
            in_len = USART_TX_FIFO_SIZE-usart_tx_fifo_in;
            if(in_len > len)
            {
                in_len = len;
            }
            memcpy(&usart_tx_fifo_buf[usart_tx_fifo_in],p_data,in_len);
            len = len-in_len;
            p_data +=  in_len;
            usart_tx_fifo_in = (usart_tx_fifo_in + in_len)%USART_TX_FIFO_SIZE;
            
        }
        else if(usart_tx_fifo_in < out_temp )
        {
            in_len = out_temp-usart_tx_fifo_in-1;
            if(in_len > len)
            {
                in_len = len;
            }
            memcpy(&usart_tx_fifo_buf[usart_tx_fifo_in],p_data,in_len);
            len = len-in_len;
            usart_tx_fifo_in = (usart_tx_fifo_in + in_len)%USART_TX_FIFO_SIZE;
            
            //fifo full,drop the rest data 
            if(len)
            {
                NS_LOG_WARNING("F:%d,%d,%d\r\n",len,usart_tx_fifo_in,out_temp);
            }
            
            break;
        }
    }
    
    //  ble treaming cut timer, active usart send after it.
    if(!usart_sending)
    {
        app_usart_tx_process();
    }
    return len;
}

/**
 * @brief  usart tx data from fifo
 */
void app_usart_tx_process(void)
{
    uint32_t in_temp,len;
    uint8_t *p_data;
    
    in_temp = usart_tx_fifo_in;
    if(usart_tx_fifo_out < in_temp)
    {
        len = in_temp-usart_tx_fifo_out;
        p_data = &usart_tx_fifo_buf[usart_tx_fifo_out];
        if(usart_tx_dma_send(p_data,len) == true)
        {
            usart_tx_fifo_out = in_temp;
        }

    }
    else if(usart_tx_fifo_out > in_temp)
    {
        len = USART_TX_FIFO_SIZE-usart_tx_fifo_out;
        p_data = &usart_tx_fifo_buf[usart_tx_fifo_out];
        if(usart_tx_dma_send(p_data,len) == true)
        {
            usart_tx_fifo_out = 0;
        }
       
    }
}


/**
 * @brief  check the dma buffer which has been received 
 */
void usart_rx_check_in_irq(void)
{
    uint16_t rx_pos;

    rx_pos = USART_RX_DMA_SIZE - DMA_GetCurrDataCounter(USARTx_Rx_DMA_Channel);
    if(rx_pos <  rx_old_pos)
    {
        app_usart_rx_data_fifo_enter(&USART_rx_dma_buf[rx_old_pos],(USART_RX_DMA_SIZE - rx_old_pos));
        
        if(rx_pos > 0)
        {
            app_usart_rx_data_fifo_enter(&USART_rx_dma_buf[0],rx_pos);
        }
        
        rx_old_pos = rx_pos;
    }
    else if(rx_pos >  rx_old_pos)
    {
        app_usart_rx_data_fifo_enter(&USART_rx_dma_buf[rx_old_pos],(rx_pos - rx_old_pos));
        rx_old_pos = rx_pos;
    }
    else if(rx_pos != rx_old_pos)
    {
        //error
        rx_old_pos = rx_pos;
    }

}


/**
 * @brief  dma irq handler
 */
void DMA_Channel1_2_3_4_IRQHandler(void)
{
    //TX
    if(DMA_GetFlagStatus(DMA_FLAG_TC1, DMA))
    {
        //TX Transfer complete interrupt
        usart_sending = false;

        DMA_ClearFlag(DMA_FLAG_TC1, DMA);
    }
    //RX 
    if(DMA_GetFlagStatus(DMA_FLAG_TC2, DMA))
    {
        //RX Transfer complete interrupt
        DMA_ClearFlag(DMA_FLAG_TC2, DMA);
        usart_rx_check_in_irq();
        
    }
    if(DMA_GetFlagStatus(DMA_FLAG_HT2, DMA))
    {
        //RX Half transfer interrupt 
        DMA_ClearFlag(DMA_FLAG_HT2, DMA);
        usart_rx_check_in_irq();
    }


}

/**
 * @brief  usart irq handler
 */
void USARTx_IRQHandler(void)
{
    uint8_t temp;
    //usart idlle interrupt
    if(USART_GetFlagStatus(USARTx,USART_FLAG_IDLEF))
    {
        //read sts and data will clear rx idle interrupt
        temp = USARTx->DAT;
        usart_rx_check_in_irq();
        (void)temp;
    }

}



/**
 * @}
 */

