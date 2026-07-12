/******************************************************************************
 * File        : main.c
 * Project     : STM32 Driver Development Framework
 * MCU         : STM32F407VG
 * Description : Main Application Entry
 ******************************************************************************/

#include "drivers.h"

/*==========================================================
                Global Objects
==========================================================*/
static UART_HandleTypeDef huart2;

static GPIO_Config_t ledConfig;

/*==========================================================
                Main
==========================================================*/
int main(void)
{
    /* System Initialization */
    System_Init();

    /* Enable Peripheral Clocks */
    RCC_EnableGPIOClock(GPIOA);
    RCC_EnableUSARTClock(USART2);

    /* Configure PA5 (LED) */
    ledConfig.pin = GPIO_PIN_5;
    ledConfig.mode = GPIO_MODE_OUTPUT;
    ledConfig.outputType = GPIO_PUSH_PULL;
    ledConfig.speed = GPIO_SPEED_HIGH;
    ledConfig.pull = GPIO_NOPULL;

    GPIO_Init(GPIOA, &ledConfig);

    /* Configure UART */
    huart2.Instance = USART2;

    huart2.Init.baudRate = UART_BAUD_115200;
    huart2.Init.wordLength = UART_WORDLENGTH_8B;
    huart2.Init.stopBits = UART_STOPBITS_1;
    huart2.Init.parity = UART_PARITY_NONE;
    huart2.Init.mode = UART_MODE_TX_RX;

    UART_Init(&huart2);

    UART_SendString(&huart2,
                    "\r\n====================================\r\n");

    UART_SendString(&huart2,
                    " STM32 Driver Development Framework\r\n");

    UART_SendString(&huart2,
                    " Build Successful\r\n");

    UART_SendString(&huart2,
                    " UART Driver Running\r\n");

    UART_SendString(&huart2,
                    "====================================\r\n");

    while (1)
    {
        GPIO_TogglePin(GPIOA, GPIO_PIN_5);

        UART_SendString(&huart2,
                        "Framework Running...\r\n");

        for (volatile uint32_t delay = 0;
             delay < 500000;
             delay++)
        {

        }
    }
}