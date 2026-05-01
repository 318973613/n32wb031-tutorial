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
 * @file main.c
 * @author NSING Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include "n32wb03x.h"
#include "log.h"

#include <string.h>

#define LOG_USARTx              USART2
#define LOG_PERIPH              RCC_APB1_PERIPH_USART2
#define LOG_GPIO                GPIOB
#define LOG_PERIPH_GPIO         RCC_APB2_PERIPH_GPIOB
#define LOG_TX_PIN              GPIO_PIN_4
#define LOG_RX_PIN              GPIO_PIN_5
#define LOG_GPIO_AF             GPIO_AF3_USART2            //GPIO_AF4_USART1

typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} Status;

#define I2C1_MASTER_ADDRESS7    0x30
#define I2C2_SLAVE_ADDRESS7     0xA0
#define BUFFER_SIZE             32
#define I2C_CLK_SPEED           100000

I2C_InitType I2C_InitStructure;
DMA_InitType DMA_InitStructure;
uint8_t I2C1_Tx_Buffer[BUFFER_SIZE];
uint8_t I2C2_Rx_Buffer[BUFFER_SIZE];
uint8_t Tx_Idx = 0, Rx_Idx = 0;
void RCC_Configuration(void);
void GPIO_Configuration(void);

int main(void)
{
    int i = 0;
    
    log_init();
    log_info("\n this is a DMA I2C Demo.\n");
    
    /* System Clocks Configuration */
    RCC_Configuration();
    
    /* Configure the GPIO ports */
    GPIO_Configuration();
    
    /* Fill the buffer to send */
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        I2C1_Tx_Buffer[i] = i;
    }
    memset(I2C2_Rx_Buffer, 0, BUFFER_SIZE);
    
    /* DMA channel5 configuration ----------------------------------------------*/
    DMA_DeInit(DMA_CH5);
    DMA_InitStructure.PeriphAddr     = (uint32_t)&I2C1->DAT;
    DMA_InitStructure.MemAddr        = (uint32_t)I2C1_Tx_Buffer;
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_DST;
    DMA_InitStructure.BufSize        = BUFFER_SIZE;
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc  = DMA_MEM_INC_ENABLE;
    DMA_InitStructure.PeriphDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.MemDataSize    = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.CircularMode   = DMA_MODE_NORMAL;
    DMA_InitStructure.Priority       = DMA_PRIORITY_VERY_HIGH;
    DMA_InitStructure.Mem2Mem        = DMA_M2M_DISABLE;
    DMA_Init(DMA_CH5, &DMA_InitStructure);
    DMA_RequestRemap(DMA_REMAP_I2C_TX, DMA, DMA_CH5, ENABLE);
    
    /* DMA channel4 configuration ----------------------------------------------*/
    DMA_DeInit(DMA_CH4);
    DMA_InitStructure.PeriphAddr     = (uint32_t)&I2C1->DAT;
    DMA_InitStructure.MemAddr        = (uint32_t)I2C2_Rx_Buffer;
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_SRC;
    DMA_InitStructure.Priority       = DMA_PRIORITY_HIGH;
    DMA_InitStructure.BufSize        = BUFFER_SIZE;
    DMA_Init(DMA_CH4, &DMA_InitStructure);
    DMA_RequestRemap(DMA_REMAP_I2C_RX, DMA, DMA_CH4, ENABLE);
    
    I2C_DeInit(I2C1);
    /* I2C1 configuration ------------------------------------------------------*/
    I2C_InitStructure.BusMode     = I2C_BUSMODE_I2C;
    I2C_InitStructure.FmDutyCycle = I2C_FMDUTYCYCLE_1;
    I2C_InitStructure.OwnAddr1    = I2C1_MASTER_ADDRESS7;
    I2C_InitStructure.AckEnable   = I2C_ACKEN;
    I2C_InitStructure.AddrMode    = I2C_ADDR_MODE_7BIT;
    I2C_InitStructure.ClkSpeed    = I2C_CLK_SPEED;
    I2C_Init(I2C1, &I2C_InitStructure);

    /* Enable I2C1 and I2C2 ----------------------------------------------------*/
    I2C_Enable(I2C1, ENABLE);
    
    
    /*----- Transmission Phase -----*/
    while (I2C_GetFlag(I2C1, I2C_FLAG_BUSY))
        ;
    /* Send I2C1 START condition */
    I2C_GenerateStart(I2C1, ENABLE);
    /* Test on I2C1 EV5 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVT_MASTER_MODE_FLAG))
        ;
    
    /* Send I2C2 slave Address for write */
    I2C_SendAddr7bit(I2C1, I2C2_SLAVE_ADDRESS7, I2C_DIRECTION_SEND);
    /* Test on I2C1 EV6 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVT_MASTER_TXMODE_FLAG))
        ;

    /* Enable I2C1 Send DMA */
    I2C_EnableDMA(I2C1, ENABLE);
    /* Enable DMA Channel5 */
    DMA_EnableChannel(DMA_CH5, ENABLE);
    
    
    /* DMA Channel5 transfer complete test */
    while (!DMA_GetFlagStatus(DMA_FLAG_TC5, DMA))
        ;
    
    Delay_ms(5);
    /* Send I2C1 STOP Condition */
    I2C_GenerateStop(I2C1, ENABLE);
    
    /*I2C read*/
    while (I2C_GetFlag(I2C1, I2C_FLAG_BUSY))
        ;
    /** Send START condition */
    I2C_GenerateStart(I2C1, ENABLE);
    /* Test on I2C1 EV5 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVT_MASTER_MODE_FLAG))
        ;
    
    // send addr
    I2C_SendAddr7bit(I2C1, I2C2_SLAVE_ADDRESS7, I2C_DIRECTION_RECV);
    while (!I2C_CheckEvent(I2C1, I2C_EVT_MASTER_RXMODE_FLAG)) // EV6
        ;
    
    /* Enable I2C1 Recv DMA */
    I2C_EnableDMA(I2C1, ENABLE);
    /* Enable DMA Channel5 */
    DMA_EnableChannel(DMA_CH4, ENABLE);
    
    while(1)
    {
        if(DMA_GetCurrDataCounter(DMA_CH4) == 1)
        {
            I2C_ConfigAck(I2C1, DISABLE);
            I2C_GenerateStop(I2C1, ENABLE);
            break;
        }
    }
    
    /* DMA Channel5 transfer complete test */
    while (!DMA_GetFlagStatus(DMA_FLAG_TC4, DMA))
        ;
    
    /* Send I2C1 STOP Condition */
    //I2C_GenerateStop(I2C1, ENABLE);
    
    log_buff(I2C1_Tx_Buffer, BUFFER_SIZE);
    log_buff(I2C2_Rx_Buffer, BUFFER_SIZE);
    printf("test end\r\n");

    while (1)
    {
    }
}

/**
 * @brief  Configures the different system clocks.
 */
void RCC_Configuration(void)
{
    /* Enable DMA clock */
    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA, ENABLE);
    /* Enable GPIO clock */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_AFIO, ENABLE);
    /* Enable I2C1 and I2C2 clock */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_I2C1, ENABLE);
}

/**
 * @brief  Configures the different GPIO ports.
 */
void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;

    GPIO_InitStruct(&GPIO_InitStructure);
    /* Configure I2C1 pins: SCL and SDA */
    GPIO_InitStructure.Pin                = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStructure.GPIO_Speed         = GPIO_SPEED_LOW;
    GPIO_InitStructure.GPIO_Mode          = GPIO_MODE_AF_OD;
    GPIO_InitStructure.GPIO_Alternate     = GPIO_AF3_I2C;
    GPIO_InitStructure.GPIO_Pull          = GPIO_PULL_UP;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
}

void gpio_starup_deinit(void)
{
    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);
    
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_I2C1, ENABLE);
    RCC_EnableAPB1PeriphReset(RCC_APB1_PERIPH_I2C1, ENABLE);
    RCC_EnableAPB1PeriphReset(RCC_APB1_PERIPH_I2C1, DISABLE);
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_I2C1, DISABLE);
    RCC_EnableAPB2PeriphClk( RCC_APB2_PERIPH_AFIO, DISABLE);
    RCC_EnableAPB2PeriphClk (RCC_APB2_PERIPH_GPIOB, DISABLE );
    RCC_EnableAPB1PeriphReset(RCC_APB1_PERIPH_I2C1, ENABLE);
    RCC_EnableAPB1PeriphReset(RCC_APB1_PERIPH_I2C1, DISABLE);
    
    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOB);
    
    GPIO_AFIOInitDefault();
    
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, DISABLE);
    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA, DISABLE);
}

void log_init(void)
{
    GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;
    
    gpio_starup_deinit();
    
    GPIO_InitStruct(&GPIO_InitStructure);
    
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO | LOG_PERIPH_GPIO, ENABLE);
    
#if (LOG_USART == 1)
    RCC_EnableAPB2PeriphClk(LOG_PERIPH, ENABLE);
#else
    RCC_EnableAPB1PeriphClk(LOG_PERIPH, ENABLE);
#endif
    
    GPIO_InitStructure.Pin            = LOG_TX_PIN;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = LOG_GPIO_AF;
    GPIO_InitPeripheral(LOG_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.Pin             = LOG_RX_PIN;
    GPIO_InitStructure.GPIO_Alternate  = LOG_GPIO_AF;
    GPIO_InitPeripheral(LOG_GPIO, &GPIO_InitStructure);

    USART_InitStructure.BaudRate            = 115200;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;

    // init uart
    USART_Init(LOG_USARTx, &USART_InitStructure);

    // enable uart
    USART_Enable(LOG_USARTx, ENABLE);
}

static int is_lr_sent = 0;

int fputc(int ch, FILE* f)
{
    if (ch == '\r')
    {
        is_lr_sent = 1;
    }
    else if (ch == '\n')
    {
        if (!is_lr_sent)
        {
            USART_SendData(LOG_USARTx, (uint8_t)'\r');
            /* Loop until the end of transmission */
            while (USART_GetFlagStatus(LOG_USARTx, USART_FLAG_TXC) == RESET)
            {
            }
        }
        is_lr_sent = 0;
    }
    else
    {
        is_lr_sent = 0;
    }
    USART_SendData(LOG_USARTx, (uint8_t)ch);
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(LOG_USARTx, USART_FLAG_TXC) == RESET)
    {
    }
    return ch;
}

void log_buff(uint8_t *data, int len)
{
    int i = 0;
    for(i=0; i<len; i++)
    {
        printf("0x%02x, ", data[i]);
        
        if((i != 0) && ((i+1) % 16 == 0))
        {
            printf("\n");
        }
    }
    
    printf("\n");
}

void Delay(uint32_t count)
{
    int i = 0;
    for (; count > 0; count--)
    {
        i++;
    }
        
}

extern void system_delay_n_10us(uint32_t value);
void Delay_ms(uint32_t count)
{
    system_delay_n_10us(100*count);    
}

void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    log_error("assertion failed: `%s` at %s:%d", expr, file, line);
    while (1)
    {
    }
}
