/**
*     Copyright (c) 2025, NSING Technologies Inc.
*
*     All rights reserved.
**/

#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32wb03x.h"

#define LED1_PORT GPIOB
#define LED1_PIN  GPIO_PIN_0

#define LED2_PORT GPIOA
#define LED2_PIN  GPIO_PIN_6

#define KEY1_PORT GPIOB
#define KEY1_PIN  GPIO_PIN_1

#define KEY2_PORT GPIOB
#define KEY2_PIN  GPIO_PIN_2

#define USARTx              USART1
#define USARTx_CLK          RCC_APB2_PERIPH_USART1
#define USARTx_GPIO         GPIOB
#define USARTx_GPIO_CLK     RCC_APB2_PERIPH_GPIOB
#define USARTx_RxPin        GPIO_PIN_7
#define USARTx_TxPin        GPIO_PIN_6
#define USARTx_Rx_GPIO_AF   GPIO_AF4_USART1
#define USARTx_Tx_GPIO_AF   GPIO_AF4_USART1
#define GPIO_APBxClkCmd     RCC_EnableAPB2PeriphClk
#define USART_APBxClkCmd    RCC_EnableAPB2PeriphClk

void Delay(volatile uint32_t count);
void DelayWithService(uint32_t count);
void LedInit(GPIO_Module* GPIOx, uint16_t Pin);
void LedOn(GPIO_Module* GPIOx, uint16_t Pin);
void LedOff(GPIO_Module* GPIOx, uint16_t Pin);
void LedToggle(GPIO_Module* GPIOx, uint16_t Pin);
void KeyInit(GPIO_Module* GPIOx, uint16_t Pin);
void UsartInit(void);
void PrintBanner(void);
void PrintHelp(void);
void PrintStatus(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */
