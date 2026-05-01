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
 
 /* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_rtc.h"
#include "app_tim.h"
#include "app_gpio.h"
#include "app_usart.h"
#include "app_adc.h"

/** @addtogroup Demo
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PWM_TEST_ENABLE    
#define RTC_TEST_ENABLE

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t EnterSleep = 0;
uint8_t key1_flag = 0;
uint8_t key2_flag = 0;
/* Private function prototypes -----------------------------------------------*/
void app_user_key_handler(void);
extern void system_delay_n_10us(uint32_t value);
/* Private functions ---------------------------------------------------------*/


void init_peripheral(void)
{

    /*Initialize USART */
    USART_Configuration();
#ifdef PWM_TEST_ENABLE    
    /*Initialize TIM3 as PWM mode */
    TIM3_PWM_Configuration();
#endif
    /*Initialize Led1 and Led2 as output pushpull mode*/
    LedInit(LED1_PORT, LED1_PIN);
    LedInit(LED2_PORT, LED2_PIN);
    /*Turn on Led1 and LED2 */
    LedOn(LED1_PORT, LED1_PIN);
    LedOn(LED2_PORT, LED2_PIN);
    /*Initialize ADC and read CH1 */
    ADC_Configuration();
    
    EnterSleep = 0;   
    key1_flag  = 0;
    key2_flag  = 0; 
    system_delay_n_10us(100*500); //500ms, prevent enter sleep with the wakeup press 
    

}

void deinit_peripheral(void)
{
    /*before enter sleep */
    LedOff(LED1_PORT, LED1_PIN);
    LedOff(LED2_PORT, LED2_PIN);  
    /*Deinitializes USART to save power*/
    USART_Deinitializes();
    
}



/**
 * @brief  Main program
 */
int main(void)
{
#ifdef RUN_FROM_RAM
    //rum from RAW
    PWR->VTOR_REG = 0xA0000000;
#endif    

    /*Initialize peripheral */
    init_peripheral();       
    /*Initialize key as external line interrupt */
    KeyInputExtiInit();        
#ifdef RTC_TEST_ENABLE            
    /*Initialize RTC  */
    RTC_Configuration();    
#endif
#ifdef TEST_200MS_WAKEUP        
    system_delay_n_10us(200*1000); //delay few secoonds for SWD 
    /*before enter sleep */
    printf("Enter sleep and wait RTC auto wakeup in 200ms loop!\r\n");

    /* Enable auto wakeup */
    RTC_EnableWakeUp(ENABLE);
    /*Deinitializes peripheral to save power*/
    deinit_peripheral();
     
    while(1)
    {
        /* Enable PWR Clock */
        RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);
        PWR_EnterSLEEPMode(PWR_SLEEPENTRY_WFI);

    }
#endif

    /* Output a message on Hyperterminal using printf function */
    printf("\r\n************************** N32WB03x DEMO Start **************************\r\n");
    printf("1. LED1(PB0) and LED2(PA6) should turnn on after powoer on.\r\n");
    printf("2. Short press Key1(PB1) will Toggles LED1 and enter sleep mode, press any key will wakeup\r\n");
    printf("3. Long press Key1(PB1) will Toggles LED1 annd enter sleep and will wakeup form RTC after 10 second \r\n");   
    printf("4. Short press Key2(PB2) will read and print ADC CH1(PB10) \r\n");        
    printf("5. TIM3 PWM CH1(PB4) is run in 40 KHz and 75%% (300/400).\r\n");
    printf("6. TIM3 PWM CH2(PB5) is run in 40 KHz and 50%% (200/400).\r\n");    
    printf("7. USART1(Tx:PB6, Rx:PB7, 115200 8N1) is enable and running echo task.\r\n");        
    printf("***************************************************************************\r\n");
    printf("Now you can enter some charaters(<256 bytes one time) to test USART1:\r\n");

    while (1)
    {
        /*Run the USART echo task*/
        USART_EchoTask();
        /*Run the user key task*/
        app_user_key_handler();
        
        /*sleep task*/
        if(EnterSleep == 1)
        {
            GLOBAL_INT_DISABLE();
            printf("Enter sleep \r\n");
            /* deinit peripheral before sleep*/
            deinit_peripheral();
            
            /* enter sleep */
            RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);
            PWR_EnterSLEEPMode(PWR_SLEEPENTRY_WFI); 

            /* after wakeup */
            init_peripheral();
            GLOBAL_INT_RESTORE();

            printf("Out of sleep \r\n");    
            RTC_TimeShow();                
          
        }
        else if(EnterSleep == 2)
        {
            /* Enable auto wakeup */
            RTC_EnableWakeUp(ENABLE);
            printf("Enter sleep and wait RTC auto wakeup in 10 seconds!\r\n");
            /* deinit peripheral before sleep*/
            deinit_peripheral();
            
            /* enter sleep */
            RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);
            PWR_EnterSLEEPMode(PWR_SLEEPENTRY_WFI);

            /* after wakeup */
            init_peripheral();

            printf("Out of sleep with RTC auto wakeup!\r\n");    
            RTC_TimeShow();
            /*stop auto wake up*/
            RTC_EnableWakeUp(DISABLE);

        }
            
    }
}

void app_user_key_handler(void)
{
    uint16_t ADCConvertedValue[2];
    uint32_t voltage[2] = {0}; 
    uint32_t keyPressTimer = 0;
    if(key1_flag)
    {

        if(GPIO_ReadInputDataBit(KEY1_INPUT_PORT,KEY1_INPUT_PIN) == RESET)
        {
            LedBlink(LED1_PORT, LED1_PIN);
            while(GPIO_ReadInputDataBit(KEY1_INPUT_PORT,KEY1_INPUT_PIN) == RESET)
            {
                if(keyPressTimer < 0xffffff)
                {
                    keyPressTimer++;
                }
                system_delay_n_10us(80);
            }
            /* Set sleep flag */
            if(keyPressTimer > 3000)
            {
                EnterSleep = 2;
            }
            else if(keyPressTimer > 200)
            {
                EnterSleep = 1;
            }
            else{
                EnterSleep = 0;
            }
        }
        key1_flag  = 0;
    }
    if(key2_flag)
    {
        if(GPIO_ReadInputDataBit(KEY2_INPUT_PORT,KEY2_INPUT_PIN) == RESET)
        {
            LedBlink(LED2_PORT, LED2_PIN);
            while(GPIO_ReadInputDataBit(KEY2_INPUT_PORT,KEY2_INPUT_PIN) == RESET)
            {}

            ADCConvertedValue[0] = ADC_GetDataBlocking(ADC_CTRL_CH_1);
                
            voltage[0] = ADC_ConverValueToVoltage(ADCConvertedValue[0], ADC_CTRL_CH_1);
            printf("ADC_CH1 value: %4d  |  ADC_CH1 vol_mV: %4d .\r\n",ADCConvertedValue[0],voltage[0]);    

        }
    
    }
    
}



#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param file pointer to the source file name
 * @param line assert_param error line source number
 */
void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    while (1)
    {
    }
}

#endif

/**
 * @}
 */

/**
 * @}
 */

/*************** (C) COPYRIGHT NSING Technologies Inc *****END OF FILE***************/


