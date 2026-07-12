#include "drivers.h"

UART_HandleTypeDef huart2;

void MyUARTCallback(UART_Event_t event)
{
    switch(event)
    {
        case UART_EVENT_RX_COMPLETE:
            break;

        case UART_EVENT_TX_COMPLETE:
            break;

        case UART_EVENT_ERROR:
            break;
    }
}

int main(void)
{
    RCC_EnableGPIOClock(GPIOA);
    RCC_EnableUSARTClock(USART2);

    huart2.Instance = USART2;

    huart2.Init.baudRate = UART_BAUD_115200;
    huart2.Init.wordLength = UART_WORDLENGTH_8B;
    huart2.Init.stopBits = UART_STOPBITS_1;
    huart2.Init.parity = UART_PARITY_NONE;
    huart2.Init.mode = UART_MODE_TX_RX;

    huart2.Callback = MyUARTCallback;

    UART_Init(&huart2);

    UART_RingBufferInit(&huart2);

    UART_EnableInterrupt(&huart2);

    UART_SendString(&huart2,
                    "Professional UART Driver\r\n");

    UART_SendString(&huart2,
                    "Project-5 Demo\r\n");

    UART_SendNumber(&huart2,123456);

    UART_SendString(&huart2,"\r\n");

    UART_SendHex(&huart2,0xDEADBEEF);

    UART_SendString(&huart2,"\r\n");

    UART_SendFloat(&huart2,24.75f);

    UART_SendString(&huart2,"\r\n");

    while(1)
    {
        uint8_t ch;

        if(UART_RingBufferRead(&huart2,&ch)==UART_OK)
        {
            UART_SendByte(&huart2,ch);
        }
    }
}