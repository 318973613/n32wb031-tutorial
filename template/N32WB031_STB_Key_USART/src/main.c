/**
*     Copyright (c) 2025, NSING Technologies Inc.
*
*     All rights reserved.
**/

#include "main.h"
#include <stdio.h>
#include <stdint.h>

typedef enum
{
    DEMO_MODE_HEARTBEAT = 0,
    DEMO_MODE_PINGPONG,
    DEMO_MODE_SYNC,
    DEMO_MODE_MAX
} DemoMode;

static DemoMode g_mode = DEMO_MODE_HEARTBEAT;
static uint8_t g_paused = 0U;
static uint32_t g_key1_press_count = 0U;
static uint32_t g_key2_press_count = 0U;

static const char* DemoModeName(DemoMode mode);
static void ServiceTasks(void);
static uint8_t KeyPressed(GPIO_Module* GPIOx, uint16_t Pin);
static void SetMode(DemoMode mode, const char* source);
static void TogglePause(const char* source);
static void HandleKeyInput(void);
static void HandleSerialInput(void);
static void RunCurrentMode(void);
static void Pulse(GPIO_Module* GPIOx, uint16_t Pin, uint32_t onDelay, uint32_t offDelay);
static void FlashBoth(uint32_t repeat, uint32_t onDelay, uint32_t offDelay);
static void StartupAnimation(void);
static void RunHeartbeatFrame(void);
static void RunPingPongFrame(void);
static void RunSyncFrame(void);
static void LedAllOff(void);

int main(void)
{
    LedInit(LED1_PORT, LED1_PIN);
    LedInit(LED2_PORT, LED2_PIN);
    KeyInit(KEY1_PORT, KEY1_PIN);
    KeyInit(KEY2_PORT, KEY2_PIN);
    UsartInit();

    LedAllOff();
    PrintBanner();
    PrintHelp();
    PrintStatus();
    StartupAnimation();

    while (1)
    {
        ServiceTasks();
        RunCurrentMode();
    }
}

void Delay(volatile uint32_t count)
{
    while (count-- > 0U)
    {
        __NOP();
    }
}

void DelayWithService(uint32_t count)
{
    while (count > 0U)
    {
        uint32_t slice = (count > 0x2000U) ? 0x2000U : count;
        Delay(slice);
        count -= slice;
        ServiceTasks();
    }
}

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

void LedToggle(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_TogglePin(GPIOx, Pin);
}

void KeyInit(GPIO_Module* GPIOx, uint16_t Pin)
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

    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin = Pin;
    GPIO_InitStructure.GPIO_Pull = GPIO_PULL_UP;
    GPIO_InitPeripheral(GPIOx, &GPIO_InitStructure);
}

void UsartInit(void)
{
    GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;

    GPIO_APBxClkCmd(USARTx_GPIO_CLK, ENABLE);
    USART_APBxClkCmd(USARTx_CLK, ENABLE);

    GPIO_InitStruct(&GPIO_InitStructure);

    GPIO_InitStructure.Pin = USARTx_TxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = USARTx_Tx_GPIO_AF;
    GPIO_InitPeripheral(USARTx_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = USARTx_RxPin;
    GPIO_InitStructure.GPIO_Alternate = USARTx_Rx_GPIO_AF;
    GPIO_InitPeripheral(USARTx_GPIO, &GPIO_InitStructure);

    USART_InitStructure.BaudRate = 115200;
    USART_InitStructure.WordLength = USART_WL_8B;
    USART_InitStructure.StopBits = USART_STPB_1;
    USART_InitStructure.Parity = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode = USART_MODE_RX | USART_MODE_TX;

    USART_Init(USARTx, &USART_InitStructure);
    USART_Enable(USARTx, ENABLE);
}

void PrintBanner(void)
{
    printf("\r\n");
    printf("========================================\r\n");
    printf(" N32WB031-STB Key + USART Basic Demo\r\n");
    printf(" USART1: PB6(TX) / PB7(RX) @ 115200\r\n");
    printf(" KEY1: PB1, KEY2: PB2\r\n");
    printf("========================================\r\n");
}

void PrintHelp(void)
{
    printf("Commands:\r\n");
    printf("  1 -> heartbeat mode\r\n");
    printf("  2 -> ping-pong mode\r\n");
    printf("  3 -> sync flash mode\r\n");
    printf("  m -> next mode\r\n");
    printf("  p -> pause/resume animation\r\n");
    printf("  s -> print status\r\n");
    printf("  h -> print help\r\n");
    printf("Buttons:\r\n");
    printf("  KEY1(PB1) -> next mode\r\n");
    printf("  KEY2(PB2) -> pause/resume + status\r\n");
}

void PrintStatus(void)
{
    printf("[STATUS] mode=%s, paused=%s, key1_count=%lu, key2_count=%lu\r\n",
           DemoModeName(g_mode),
           g_paused ? "YES" : "NO",
           (unsigned long)g_key1_press_count,
           (unsigned long)g_key2_press_count);
}

int fputc(int ch, FILE* f)
{
    (void)f;
    USART_SendData(USARTx, (uint8_t)ch);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET)
    {
    }
    return ch;
}

static const char* DemoModeName(DemoMode mode)
{
    switch (mode)
    {
        case DEMO_MODE_HEARTBEAT:
            return "heartbeat";
        case DEMO_MODE_PINGPONG:
            return "pingpong";
        case DEMO_MODE_SYNC:
            return "sync";
        default:
            return "unknown";
    }
}

static void ServiceTasks(void)
{
    HandleSerialInput();
    HandleKeyInput();
}

static uint8_t KeyPressed(GPIO_Module* GPIOx, uint16_t Pin)
{
    if (GPIO_ReadInputDataBit(GPIOx, Pin) == RESET)
    {
        Delay(0x5000U);
        if (GPIO_ReadInputDataBit(GPIOx, Pin) == RESET)
        {
            while (GPIO_ReadInputDataBit(GPIOx, Pin) == RESET)
            {
            }
            Delay(0x5000U);
            return 1U;
        }
    }

    return 0U;
}

static void SetMode(DemoMode mode, const char* source)
{
    g_mode = mode;
    g_paused = 0U;
    printf("[%s] mode -> %s\r\n", source, DemoModeName(g_mode));
    FlashBoth(1U, 0x10000U, 0x08000U);
}

static void TogglePause(const char* source)
{
    g_paused = (uint8_t)!g_paused;
    printf("[%s] animation %s\r\n", source, g_paused ? "paused" : "resumed");
}

static void HandleKeyInput(void)
{
    if (KeyPressed(KEY1_PORT, KEY1_PIN) != 0U)
    {
        g_key1_press_count++;
        SetMode((DemoMode)((g_mode + 1U) % DEMO_MODE_MAX), "KEY1");
    }

    if (KeyPressed(KEY2_PORT, KEY2_PIN) != 0U)
    {
        g_key2_press_count++;
        TogglePause("KEY2");
        PrintStatus();
    }
}

static void HandleSerialInput(void)
{
    if (USART_GetFlagStatus(USARTx, USART_FLAG_RXDNE) == RESET)
    {
        return;
    }

    switch ((char)USART_ReceiveData(USARTx))
    {
        case '1':
            SetMode(DEMO_MODE_HEARTBEAT, "UART");
            break;
        case '2':
            SetMode(DEMO_MODE_PINGPONG, "UART");
            break;
        case '3':
            SetMode(DEMO_MODE_SYNC, "UART");
            break;
        case 'm':
        case 'M':
            SetMode((DemoMode)((g_mode + 1U) % DEMO_MODE_MAX), "UART");
            break;
        case 'p':
        case 'P':
            TogglePause("UART");
            break;
        case 's':
        case 'S':
            PrintStatus();
            break;
        case 'h':
        case 'H':
        case '?':
            PrintHelp();
            break;
        case '\r':
        case '\n':
            break;
        default:
            printf("[UART] unknown command\r\n");
            PrintHelp();
            break;
    }
}

static void RunCurrentMode(void)
{
    if (g_paused != 0U)
    {
        LedAllOff();
        DelayWithService(0x18000U);
        return;
    }

    switch (g_mode)
    {
        case DEMO_MODE_HEARTBEAT:
            RunHeartbeatFrame();
            break;
        case DEMO_MODE_PINGPONG:
            RunPingPongFrame();
            break;
        case DEMO_MODE_SYNC:
            RunSyncFrame();
            break;
        default:
            RunHeartbeatFrame();
            break;
    }
}

static void Pulse(GPIO_Module* GPIOx, uint16_t Pin, uint32_t onDelay, uint32_t offDelay)
{
    LedOn(GPIOx, Pin);
    DelayWithService(onDelay);
    LedOff(GPIOx, Pin);
    DelayWithService(offDelay);
}

static void FlashBoth(uint32_t repeat, uint32_t onDelay, uint32_t offDelay)
{
    while (repeat-- > 0U)
    {
        LedOn(LED1_PORT, LED1_PIN);
        LedOn(LED2_PORT, LED2_PIN);
        Delay(onDelay);
        LedAllOff();
        Delay(offDelay);
    }
}

static void StartupAnimation(void)
{
    Pulse(LED1_PORT, LED1_PIN, 0x10000U, 0x08000U);
    Pulse(LED2_PORT, LED2_PIN, 0x10000U, 0x08000U);
    FlashBoth(2U, 0x14000U, 0x0A000U);
}

static void RunHeartbeatFrame(void)
{
    Pulse(LED1_PORT, LED1_PIN, 0x08000U, 0x04000U);
    Pulse(LED1_PORT, LED1_PIN, 0x08000U, 0x12000U);
    Pulse(LED2_PORT, LED2_PIN, 0x08000U, 0x04000U);
    Pulse(LED2_PORT, LED2_PIN, 0x08000U, 0x20000U);
}

static void RunPingPongFrame(void)
{
    Pulse(LED1_PORT, LED1_PIN, 0x0C000U, 0x05000U);
    Pulse(LED2_PORT, LED2_PIN, 0x0C000U, 0x05000U);
}

static void RunSyncFrame(void)
{
    FlashBoth(2U, 0x0A000U, 0x06000U);
    DelayWithService(0x20000U);
}

static void LedAllOff(void)
{
    LedOff(LED1_PORT, LED1_PIN);
    LedOff(LED2_PORT, LED2_PIN);
}

#ifdef USE_FULL_ASSERT
void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    (void)expr;
    (void)file;
    (void)line;
    while (1)
    {
    }
}
#endif
