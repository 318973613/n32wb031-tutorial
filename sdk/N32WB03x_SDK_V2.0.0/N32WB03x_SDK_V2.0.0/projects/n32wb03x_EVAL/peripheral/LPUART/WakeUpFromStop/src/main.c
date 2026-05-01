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
 * @version v1.0.2
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "main.h"

/** @addtogroup N32WB03X_StdPeriph_Examples
 * @{
 */

/** @addtogroup LPUART_WakeUpFromStop
 * @{
 */

LPUART_InitType LPUART_InitStructure;

/* Buffer used for confirmation messages transmission */
uint8_t TxBuffer1[] = "Start bit detection wake-up successful";
uint8_t TxBuffer2[] = "RXNE detection wake-up successful";
uint8_t TxBuffer3[] = "A configurable received byte match wake-up successful";
uint8_t TxBuffer4[] = "A programmed 4-Byte frame match wake-up successful";
uint32_t WakeUpTrigger[] = {0x5A, 0x1E2D3C4B};

uint8_t RxIndex = 0;
uint8_t RxBuffer[32]	= {0};

/**
 * @brief  Main program
 */
int main(void)
{
	/* please reset for next flash, and you have do flash it bfore the MCU into sleep mode*/
    uint32_t dealy = 0x1E84800/2;
    while(dealy--){} //delay few secoonds for SWD
		
    /* Configure the GPIO ports */
    GPIO_Configuration();

    /* System Clocks Configuration */
    RCC_Configuration(RCC_LPUARTCLK_SRC_LSI_LSE);

    /* Configure EXTI Line 10 */
    EXTI_Configuration();
	

	/* LPUART configuration ------------------------------------------------------*/
	LPUART_DeInit(LPUARTx);
	LPUART_InitStructure.BaudRate            	= 9600;
	LPUART_InitStructure.Parity              	= LPUART_PE_NO;
	LPUART_InitStructure.RtsThreshold        	= LPUART_RTSTH_FIFOFU;
	LPUART_InitStructure.HardwareFlowControl 	= LPUART_HFCTRL_NONE;
	LPUART_InitStructure.Mode                	= LPUART_MODE_RX | LPUART_MODE_TX;
	
	LPUART_ConfigInt(LPUARTx, LPUART_INT_FIFO_NE, ENABLE);
	
	/* Configure LPUARTx */
	LPUART_Init(LPUARTx, &LPUART_InitStructure);
    
    /*##-1- Wake Up second step  ###############################################*/
    printf("\n\r##-1-Start Bit Detection Wake-Up##\n\r");
    /* Set the Wake-Up Event: specify wake-up on start bit */
    LPUART_ConfigWakeUpMethod(LPUARTx, LPUART_WUSTP_STARTBIT);
    /* Enable the LPUARTx Wake UP from STOP Mode Interrupt */
    LPUART_ConfigInt(LPUARTx, LPUART_INT_WUF, ENABLE);
    /* Enable MCU Wake-up by LPUARTx */
    LPUART_EnableWakeUpStop(LPUARTx, ENABLE);
    
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);
    /* Enter STOP Mode */
    PWR_EnterSLEEPMode(PWR_SLEEPENTRY_WFI);
    /* ... STOP Mode ... */
    /* Wake Up based on Start Bit Detection successful */
    LPUART_EnableWakeUpStop(LPUARTx, DISABLE);
    /* Output a message on Hyperterminal using printf function */
    printf("\n\rStart bit detection wake-up successful\n\r");     
   
    /*##-2- Wake Up second step  ###############################################*/
    printf("\n\r##-2-RXNE Detection Wake-Up##\n\r");
    /* Set the Wake-Up Event: specify wake-up on RXNE flag */
    LPUART_ConfigWakeUpMethod(LPUARTx, LPUART_WUSTP_RXNE);
    /* Enable the LPUART Wake UP from STOP Mode Interrupt */
    LPUART_ConfigInt(LPUARTx, LPUART_INT_WUF, ENABLE);
    /* Enable MCU Wake-Up by LPUARTx */
    LPUART_EnableWakeUpStop(LPUARTx, ENABLE);
    
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);
    /* Enter STOP Mode */
    PWR_EnterSLEEPMode(PWR_SLEEPENTRY_WFI);
    /* ... STOP Mode ... */
    /* Wake Up based on RXNE Detection successful */
    LPUART_EnableWakeUpStop(LPUARTx, DISABLE);
    /* Output a message on Hyperterminal using printf function */
    printf("\n\rRXNE detection wake-up successful\n\r");

    /*##-3- Wake Up second step  ###############################################*/
    printf("\n\r##-3-A Configurable Received Byte Match Wake-Up##\n\r");
    /* Set the Wake-Up Event: specify wake-up on a configurable received byte match */
    LPUART_ConfigWakeUpMethod(LPUARTx, LPUART_WUSTP_BYTE);
    /* Set the Wake-Up Data */
    WakeUpTrigger[0] = 0xAA;
    LPUART_ConfigWakeUpData(LPUARTx, WakeUpTrigger[0]);
    /* Enable the LPUARTx Wake UP from STOP mode Interrupt */
    LPUART_ConfigInt(LPUARTx, LPUART_INT_WUF, ENABLE);
    /* Enable MCU Wake-Up by LPUARTx */
    LPUART_EnableWakeUpStop(LPUARTx, ENABLE);
    
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);
    /* Enter STOP Mode */
    PWR_EnterSLEEPMode(PWR_SLEEPENTRY_WFI);
    /* ... STOP Mode ... */
    /* Wake Up based on a Configurable Received Byte Match successful */
    LPUART_EnableWakeUpStop(LPUARTx, DISABLE);
    /* Output a message on Hyperterminal using printf function */
    printf("\n\rA configurable received byte match wake-up successful\n\r");

    /*##-4- Wake Up second step  ###############################################*/
    printf("\n\r##-4-A Programmed 4-Byte Frame Match Wake-Up##\n\r");
    /* Set the Wake-Up Event: specify wake-up on a programmed 4-Byte frame */
    LPUART_ConfigWakeUpMethod(LPUARTx, LPUART_WUSTP_FRAME);
    /* Set the Wake-Up Data */
    WakeUpTrigger[1] = 0x1E2D3C4B;
    LPUART_ConfigWakeUpData(LPUARTx, WakeUpTrigger[1]);
    /* Enable the LPUARTx Wake UP from STOP mode Interrupt */
    LPUART_ConfigInt(LPUARTx, LPUART_INT_WUF, ENABLE);
    /* Enable MCU Wake-Up by LPUARTx */
    LPUART_EnableWakeUpStop(LPUARTx, ENABLE);
    
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);
    /* Enter STOP Mode */
    PWR_EnterSLEEPMode(PWR_SLEEPENTRY_WFI);
    /* ... STOP Mode ... */
    /* Wake Up based on a Programmed 4-Byte Frame Match successful */
    LPUART_EnableWakeUpStop(LPUARTx, DISABLE);    
    /* Output a message on Hyperterminal using printf function */
    printf("\n\rA programmed 4-Byte frame match wake-up successful\n\r");

}

/**
 * @brief  Enable the lsi and calibration it.
 * @param  
 */
uint32_t calib_lsi_clk(void)
{
    extern void system_delay_n_10us(uint32_t value);
//    const uint32_t  g_lsi_20_syscle_cal_value     = 20000; //32000
//    const uint32_t  g_lsi_20_syscle_cal_value     = 22222; //28800
    const uint32_t  g_lsi_20_syscle_cal_value     = 19531; //32768
    
    const uint32_t  g_lsi_accuracy                = 0;       
    const uint32_t g_lsi_count_n_syscle           = 20;
    const uint32_t g_timeoutcnt                   = 1000;   
    uint32_t  lsi_timeoutcnt = 0; 
    uint32_t  min = 0;
    uint32_t  max = 2047; 
    uint32_t  mid = (RCC->LSCTRL >> 8) & 0x7FF;
    uint32_t  delta = 0;
    uint32_t  tmp_trim ;
    uint32_t  temp;
    uint32_t  count_value = 0;
    
    RCC->OSCFCCR &= ~(0xFF<< 8);
    RCC->OSCFCCR |= (g_lsi_count_n_syscle <<8);  //write count n syscle  
    
    do{  
        temp  = RCC->LSCTRL & (~(0x7FF << 8));  //32K TRIM 8-18 bit
        RCC->LSCTRL = temp | (mid << 8);                 // clear and set TRIM value 
        RCC->OSCFCCR |= 0x1 ;                                //start lsi   
         
        while(1)
        {
            system_delay_n_10us(1);        
            if((RCC->OSCFCSR & 0x01))
            {
                break;
            }    
            if(lsi_timeoutcnt++ > g_timeoutcnt)
            {    
                return 0;
            }    
        }  
        count_value = RCC->OSCFCLSICNT; //ready cnt value       
        
        delta = abs(count_value - g_lsi_20_syscle_cal_value); 
        if(count_value >=  g_lsi_20_syscle_cal_value)
        {
            min = mid;
        }
        else
        {
            max = mid;
        }       
        tmp_trim = (min + max)/2;  
        if(tmp_trim == mid )                //0 and 127 if not used        
        {
            break;
        }    
        mid = tmp_trim;
    }while(delta > g_lsi_accuracy);   

    return count_value;
} 
/**
 * @brief  Configures the different system clocks.
 * @param  LPUART_CLK_SRC: specifies the LPUARTx clock source.
 */
void RCC_Configuration(uint32_t LPUART_CLK_SRC)
{
    /* get trim value */
    trim_stored_t *p_trim = SystemTrimValueGet();
    switch(LPUART_CLK_SRC)
    {
        case RCC_LPUARTCLK_SRC_LSI_LSE:
        {  
#if 1			
            /* Configures the External Low Speed oscillator (LSE) */
            RCC_ConfigLSXSEL(RCC_RTCCLK_SRC_LSE);
            RCC_ConfigLse(RCC_LSE_ENABLE);		//32.768KHz
            while (RCC_GetFlagStatus(RCC_LSCTRL_FLAG_LSERD) == RESET)
            {
            }
#else
            //calibration LSI 
            calib_lsi_clk();

			RCC_ConfigLSXSEL(RCC_RTCCLK_SRC_LSI);
			RCC_EnableLsi(ENABLE);		//32.768KHz
            while (RCC_GetFlagStatus(RCC_LSCTRL_FLAG_LSIRD) == RESET)
            {
            }
#endif
            /* Specifies the LPUARTx clock source, LSE selected as LPUARTx clock */
            RCC_ConfigLpuartClk(LPUARTx_CLK, RCC_LPUARTCLK_SRC_LSI_LSE);
        }
        break;        
        
        default:
        {
            /* Specifies the LPUARTx clock source, APB1 selected as LPUARTx clock */
            RCC_ConfigLpuartClk(LPUARTx_CLK, RCC_LPUARTCLK_SRC_APB1);
        }
        break;
    }    
    
    /* Enable LPUARTx Clock */
	RCC_EnableLpuartClk(ENABLE);
}

/**
 * @brief  Configures the different GPIO ports.
 */
void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Enable GPIO clock */
    RCC_EnableAPB2PeriphClk(LPUARTx_GPIO_CLK | RCC_APB2_PERIPH_AFIO, ENABLE);       

    /* Initialize GPIO_InitStructure */
    GPIO_InitStruct(&GPIO_InitStructure);

    /* Configure LPUARTx Tx as alternate function push-pull */
    GPIO_InitStructure.Pin            = LPUARTx_TxPin;
	GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = LPUARTx_Tx_GPIO_AF;
    GPIO_InitPeripheral(LPUARTx_GPIO, &GPIO_InitStructure);

    /* Configure LPAURTx Rx as alternate function push-pull and pull-up */
    GPIO_InitStructure.Pin            = LPUARTx_RxPin;  
    GPIO_InitStructure.GPIO_Alternate = LPUARTx_Rx_GPIO_AF;    
    GPIO_InitPeripheral(LPUARTx_GPIO, &GPIO_InitStructure);
}

/**
 * @brief  Configures the nested vectored interrupt controller.
 */
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;

    /* Enable the LPUARTx Wakeup Interrupt through EXTI line 10 */
    NVIC_InitStructure.NVIC_IRQChannel                   = LPUART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority           = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the NRST Wakeup Interrupt through EXTI line 12 */
	NVIC_InitStructure.NVIC_IRQChannel                   = EXTI4_12_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}


/**
 * @brief  Configure EXTI_LINE23 as EXTI_Line and set the Priority.
 */
void EXTI_Configuration(void)
{    
    EXTI_InitType EXTI_InitStructure;

    /* Configure NVIC */
    NVIC_Configuration();
	
    /* Enable the LPUARTx Wakeup Interrupt through EXTI line 10 */
    EXTI_InitStructure.EXTI_Line	= EXTI_LINE10;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);
	
	/* Enable the NRST Wakeup Interrupt through EXTI line 12 */
	EXTI_InitStructure.EXTI_Line	= EXTI_LINE12;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);
}

/**
 * @brief  Inserts a delay time.
 * @param nCount specifies the delay time length.
 */
void Delay(__IO uint32_t nCount)
{
    /* Decrement nCount value */
    for (; nCount != 0; nCount--)
        ;
}

/* retarget the C library printf function to the LPUART */
int fputc(int ch, FILE* f)
{
    LPUART_SendData(LPUARTx, (uint8_t)ch);
    while (LPUART_GetFlagStatus(LPUARTx, LPUART_FLAG_TXC) == RESET)
        ;
    LPUART_ClrFlag(LPUARTx, LPUART_FLAG_TXC);      

    return (ch);
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

    /* Infinite loop */
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
