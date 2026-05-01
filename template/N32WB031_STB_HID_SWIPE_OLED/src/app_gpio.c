/**
*     Copyright (c) 2025, NSING Technologies Inc.
*
*     All rights reserved.
**/

#include "app_gpio.h"
#include <stdio.h>
#include "global_func.h"
#include "app_hid.h"
#include "app_oled.h"
#include "ns_log.h"
#include "ns_timer.h"
#include "ns_delay.h"
#include "app_ble.h"

#define KEY_PRESS_DELAY 100
#define HID_KEY_VOL_UP      0x10
#define HID_KEY_VOL_DOWN    0x08

uint8_t key1_irq_actived = 0;
uint8_t key2_irq_actived = 0;

static void app_hid_camera_shot(void);
static void app_hid_volume_down(void);
static void app_hid_send_consumer_key(uint8_t key_value);

void LedInit(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_InitType GPIO_InitStructure;

    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    if (GPIOx == GPIOA)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
    }
    else if (GPIOx == GPIOB)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    }
    else
    {
        return;
    }

    if (Pin <= GPIO_PIN_ALL)
    {
        GPIO_InitStruct(&GPIO_InitStructure);
        GPIO_InitStructure.Pin = Pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitPeripheral(GPIOx, &GPIO_InitStructure);
    }
}

void LedOn(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_SetBits(GPIOx, Pin);
}

void LedOff(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_ResetBits(GPIOx, Pin);
}

void LedBlink(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_TogglePin(GPIOx, Pin);
}

void app_key_configuration(void)
{
    GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);

    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin = KEY1_INPUT_PIN;
    GPIO_InitStructure.GPIO_Pull = GPIO_PULL_UP;
    GPIO_InitPeripheral(KEY1_INPUT_PORT, &GPIO_InitStructure);

    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin = KEY2_INPUT_PIN;
    GPIO_InitStructure.GPIO_Pull = GPIO_PULL_UP;
    GPIO_InitPeripheral(KEY2_INPUT_PORT, &GPIO_InitStructure);

    GPIO_ConfigEXTILine(KEY1_INPUT_PORT_SOURCE, KEY1_INPUT_PIN_SOURCE);
    GPIO_ConfigEXTILine(KEY2_INPUT_PORT_SOURCE, KEY2_INPUT_PIN_SOURCE);

    EXTI_InitStructure.EXTI_Line = KEY1_INPUT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = KEY1_INPUT_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    EXTI_InitStructure.EXTI_Line = KEY2_INPUT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = KEY2_INPUT_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void app_key_reinit_after_sleep(void)
{
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);
}

static void app_hid_send_consumer_key(uint8_t key_value)
{
    if (!is_app_hid_ready())
    {
        oled_show_status("HID CAMERA DEMO", "STATE: NOT READY", "ACTION: BLOCKED", "WAIT CONNECT");
        return;
    }

    app_hid_send_consumer_report(&key_value);
    delay_n_10us(1000);
    key_value = 0x00;
    app_hid_send_consumer_report(&key_value);
}

static void app_hid_camera_shot(void)
{
    LedBlink(LED1_PORT, LED1_PIN);
    oled_show_status("HID CAMERA DEMO", "STATE: CONNECTED", "ACTION: SHOT", "KEY: VOL+");
    NS_LOG_INFO("KEY1 -> HID camera shot (VOL+)\r\n");
    app_hid_send_consumer_key(HID_KEY_VOL_UP);
}

static void app_hid_volume_down(void)
{
    LedBlink(LED1_PORT, LED1_PIN);
    oled_show_status("HID CAMERA DEMO", "STATE: CONNECTED", "ACTION: AUX", "KEY: VOL-");
    NS_LOG_INFO("KEY2 -> HID volume down\r\n");
    app_hid_send_consumer_key(HID_KEY_VOL_DOWN);
}

void app_key_press_timeout_handler(void)
{
    if (key1_irq_actived == 1)
    {
        app_hid_camera_shot();
        key1_irq_actived = 0;
    }
    else if (key1_irq_actived == 2)
    {
        ns_timer_create(KEY_PRESS_DELAY, app_key_press_timeout_handler);
        key1_irq_actived = 1;
    }

    if (key2_irq_actived == 1)
    {
        app_hid_volume_down();
        key2_irq_actived = 0;
    }
    else if (key2_irq_actived == 2)
    {
        ns_timer_create(KEY_PRESS_DELAY, app_key_press_timeout_handler);
        key2_irq_actived = 1;
    }
}

void EXTI0_1_IRQHandler(void)
{
    if (EXTI_GetITStatus(KEY1_INPUT_EXTI_LINE) != RESET)
    {
        if (key1_irq_actived == 0)
        {
            ke_msg_send_basic(APP_KEY_DETECTED, TASK_APP, TASK_APP);
            key1_irq_actived = 1;
        }
        else if (key1_irq_actived == 1)
        {
            key1_irq_actived = 2;
        }
        EXTI_ClrITPendBit(KEY1_INPUT_EXTI_LINE);
    }
}

void EXTI2_3_IRQHandler(void)
{
    if (EXTI_GetITStatus(KEY2_INPUT_EXTI_LINE) != RESET)
    {
        if (key2_irq_actived == 0)
        {
            ke_msg_send_basic(APP_KEY_DETECTED, TASK_APP, TASK_APP);
            key2_irq_actived = 1;
        }
        else if (key2_irq_actived == 1)
        {
            key2_irq_actived = 2;
        }
        EXTI_ClrITPendBit(KEY2_INPUT_EXTI_LINE);
    }
}
