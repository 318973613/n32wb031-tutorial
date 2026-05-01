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
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
 
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "log.h"
#include "smartcard.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/


/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t gTestResult;
uint8_t looptime = 0;
uint8_t F_HotReset = 0;

uint8_t AppletAID[8] = {0xA0, 0x00, 0x00, 0x00,  0x03, 0x00, 0x00, 0x00};

/* Data: Send to card */
uint8_t TxData[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
                    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
/* Data: Receive from card */
uint8_t RxData[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 
                    0xaa, 0x55, 0xff, 0x00, 0x11, 0x22, 0x33, 0x44};   

char gJsonObject[512];
/* Private function prototypes -----------------------------------------------*/
void Test_PSAM(void);
/* Private functions ---------------------------------------------------------*/



/** @addtogroup N32WB03X_StdPeriph_Examples
 * @{
 */

/** @addtogroup SmartCard
 * @{
 */



/**
 * @brief  Main program
 */
int main(void)
{
 
    log_init();
    /* Output a message on Hyperterminal using printf function */
    printf("\n\rSmartCard 7816 demo\n\r");

    while (1)
    {
        Test_PSAM();
        printf("PSAM_1 test finish\r\n");
        
        SC1_DeInit();
        Delay_ms(1000);

//        Uart_Init(115200);
        printf("System Init Finish!\r\n");
    }
}

/**
 * @brief  smart card test
 * @param   
 * @return 
 * @note   Note
 */
void Test_PSAM(void)
{
    uint16_t i;
    uint8_t timeout =0;
    SC_InitStructure SC_InitCfg;
    SC_State SCState = SC_POWER_OFF;
    SC_ADPU_Commands SC_ADPU;
    SC_ADPU_Responce SC_Responce;
    
    printf("USARTx Smart_Card_1 Test Start\r\n");
    
    SC_InitCfg.Clk_Div   =  10;    
    SC_InitCfg.GT        =  16; 
    SC_InitCfg.StopBits  =  3;  
    SC_InitCfg.Parity    =  1;        
    SC_InitCfg.NackEn    =  0;  
    looptime             =  10;
    

    SCState = SC_POWER_ON;  
    
    SC_ADPU.Header.CLA  = 0x00;  
    SC_ADPU.Header.INS  = SC_GET_A2R;  
    SC_ADPU.Header.P1   = 0x00;  
    SC_ADPU.Header.P2   = 0x00;  
    SC_ADPU.Body.LC     = 0x00;
    
    while(SCState != SC_ACTIVE_ON_T0)   
    {  
        SC_Handler(&SCState, &SC_ADPU, &SC_Responce, &SC_InitCfg);  
        
        timeout++;
        Delay_ms(10);
        if(timeout>10)
        {
             break;
        }
    }  

    /* Apply the Procedure Type Selection (PTS)  */
    SC_PTSConfig();
    //
    Delay_ms(10);
    // Select Applet
    SC_ADPU.Header.CLA  = 0x00;
    SC_ADPU.Header.INS  = 0x84;
    SC_ADPU.Header.P1   = 0x00;
    SC_ADPU.Header.P2   = 0x00;
    SC_ADPU.Body.LC     = 0x00;
    SC_ADPU.Body.LE     = 0x08;

    for(i = 0; i < SC_ADPU.Body.LC; i++)
    {
        SC_ADPU.Body.Data[i] = AppletAID[i];
    }
    while(i < LC_MAX)
    {
        SC_ADPU.Body.Data[i++] = 0;
    }
    
    SC_Handler(&SCState, &SC_ADPU, &SC_Responce, &SC_InitCfg);
    Delay_ms(10);
    if((SC_Responce.SW1 == 0x90)&&(SC_Responce.SW2 == 0x00))
    {
        printf("SmartCard Read Random success! \r\n");
        for(i = 0;i<8;i++)
        {
            printf("Random_Table[%d] = %x \r\n",i,SC_Responce.Data[i]);
        }
        
    }
    
    F_HotReset = 1;
    printf("USARTx Smart_Card Hot_Reset\r\n");

    SC1_Reset(Bit_RESET);
    Delay_ms(10);
    SC1_Reset(Bit_SET);

    SC_InitCfg.Clk_Div   =  10;    
    SC_InitCfg.GT        =  16; 
    SC_InitCfg.StopBits  =  3;  
    SC_InitCfg.Parity    =  1;        
    SC_InitCfg.NackEn    =  0;  
    looptime             =  10;
    
    SCState = SC_POWER_ON;  
    
    SC_ADPU.Header.CLA  = 0x00;  
    SC_ADPU.Header.INS  = SC_GET_A2R;  
    SC_ADPU.Header.P1   = 0x00;  
    SC_ADPU.Header.P2   = 0x00;  
    SC_ADPU.Body.LC     = 0x00;
    
    while(SCState != SC_ACTIVE_ON_T0)   
    {  
        SC_Handler(&SCState, &SC_ADPU, &SC_Responce, &SC_InitCfg);  
    }  
    printf("USARTx Smart_Card Hot_Reset Finish\r\n");
    F_HotReset = 0;
}

/**
 * @}
 */

/**
 * @}
 */
