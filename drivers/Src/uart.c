/**
 * @file    uart.c
 * @brief   Universal Asynchronous Receiver-Transmitter (UART) Driver Implementation
 * @details Production-ready, MISRA-friendly driver for STM32F407VG.
 * Implements register-level UART configurations, blocking I/O, ring buffers, and interrupts.
 */

#include "uart.h"
#include "rcc.h"
#include <stddef.h>

/*=============================================================================
 * MACROS & DEFINES
 *===========================================================================*/

/** @brief Helper macro to compute bitmask from position */
#define UART_BIT(bit)                   (1UL << (bit))

/** @brief Timeout value for polling operations */
#define UART_TIMEOUT_VALUE              (1000000U)

/** @brief Default Core Clock Frequency if not provided by system.h */
#ifndef SYSTEM_CORE_CLOCK
#define SYSTEM_CORE_CLOCK               (16000000U)
#endif

/** @brief Bit position definitions to eliminate magic numbers */
#define UART_CR1_UE_POS                 (13U)
#define UART_CR1_M_POS                  (12U)
#define UART_CR1_PCE_POS                (10U)
#define UART_CR1_PS_POS                 (9U)
#define UART_CR1_PEIE_POS               (8U)
#define UART_CR1_TXEIE_POS              (7U)
#define UART_CR1_RXNEIE_POS             (6U)
#define UART_CR1_TE_POS                 (3U)
#define UART_CR1_RE_POS                 (2U)

#define UART_CR2_STOP_POS               (12U)

#define UART_SR_TXE_POS                 (7U)
#define UART_SR_TC_POS                  (6U)
#define UART_SR_RXNE_POS                (5U)
#define UART_SR_IDLE_POS                (4U)
#define UART_SR_ORE_POS                 (3U)

/*=============================================================================
 * PRIVATE FUNCTION PROTOTYPES
 *===========================================================================*/

static UART_Status_t UART_WaitFlagInternal(volatile uint32_t *reg, uint32_t mask);
static UART_Status_t UART_HandleTimeout(UART_HandleTypeDef *huart);
static void UART_ReverseString(char *str, uint16_t length);

/*=============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 *===========================================================================*/

/**
 * @brief Internal helper to wait for a specific register flag to be SET
 */
static UART_Status_t UART_WaitFlagInternal(volatile uint32_t *reg, uint32_t mask)
{
    uint32_t timeout = UART_TIMEOUT_VALUE;
    UART_Status_t status = UART_OK;

    while (((*reg) & mask) == 0U)
    {
        if (timeout == 0U)
        {
            status = UART_TIMEOUT;
            break;
        }
        timeout--;
    }

    return status;
}

/**
 * @brief Internal helper to centralize timeout cleanup
 */
static UART_Status_t UART_HandleTimeout(UART_HandleTypeDef *huart)
{
    if (huart != NULL)
    {
        huart->ErrorCode = UART_TIMEOUT;
        huart->Busy = false;
    }
    return UART_TIMEOUT;
}

/**
 * @brief Reverses a string in-place (used for number formatting)
 */
static void UART_ReverseString(char *str, uint16_t length)
{
    uint16_t start = 0U;
    uint16_t end = length - 1U;
    char temp;

    while (start < end)
    {
        temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

/*=============================================================================
 * PUBLIC API IMPLEMENTATIONS
 *===========================================================================*/

/*------------------------- Initialization ----------------------------------*/

UART_Status_t UART_Init(UART_HandleTypeDef *huart)
{
    uint32_t tempReg = 0U;

    if (huart == NULL)
    {
        return UART_NULL_POINTER;
    }

    RCC_EnableUSARTClock(huart->Instance);

    /* Disable UART before configuration */
    huart->Instance->CR1 &= ~UART_BIT(UART_CR1_UE_POS);

    /* Configure Word Length, Parity, and Transmitter/Receiver Mode */
    tempReg = huart->Instance->CR1;
    tempReg &= ~(UART_BIT(UART_CR1_M_POS) | UART_BIT(UART_CR1_PCE_POS) | 
                 UART_BIT(UART_CR1_PS_POS) | UART_BIT(UART_CR1_TE_POS) | 
                 UART_BIT(UART_CR1_RE_POS));

    if (huart->Init.wordLength == UART_WORDLENGTH_9B)
    {
        tempReg |= UART_BIT(UART_CR1_M_POS);
    }

    if (huart->Init.parity == UART_PARITY_EVEN)
    {
        tempReg |= UART_BIT(UART_CR1_PCE_POS);
    }
    else if (huart->Init.parity == UART_PARITY_ODD)
    {
        tempReg |= (UART_BIT(UART_CR1_PCE_POS) | UART_BIT(UART_CR1_PS_POS));
    }

    if ((huart->Init.mode == UART_MODE_TX) || (huart->Init.mode == UART_MODE_TX_RX))
    {
        tempReg |= UART_BIT(UART_CR1_TE_POS);
    }

    if ((huart->Init.mode == UART_MODE_RX) || (huart->Init.mode == UART_MODE_TX_RX))
    {
        tempReg |= UART_BIT(UART_CR1_RE_POS);
    }

    huart->Instance->CR1 = tempReg;

    /* Configure Stop Bits */
    tempReg = huart->Instance->CR2;
    tempReg &= ~(3U << UART_CR2_STOP_POS);
    if (huart->Init.stopBits == UART_STOPBITS_2)
    {
        tempReg |= (2U << UART_CR2_STOP_POS);
    }
    huart->Instance->CR2 = tempReg;

    /* Configure Baud Rate */
    huart->Instance->BRR = (SYSTEM_CORE_CLOCK + (huart->Init.baudRate / 2U)) / huart->Init.baudRate;

    /* Initialize state flags and ring buffers */
    huart->Busy = false;
    huart->ErrorCode = UART_OK;
    UART_RingBufferInit(huart);

    /* Enable UART */
    huart->Instance->CR1 |= UART_BIT(UART_CR1_UE_POS);

    return UART_OK;
}

UART_Status_t UART_DeInit(UART_HandleTypeDef *huart)
{
    if (huart == NULL)
    {
        return UART_NULL_POINTER;
    }

    RCC_ResetUSART(huart->Instance);

    huart->Busy = false;
    huart->ErrorCode = UART_OK;
    huart->Callback = NULL;

    return UART_OK;
}

/*------------------------- Polling Transmit/Receive ------------------------*/

UART_Status_t UART_SendByte(UART_HandleTypeDef *huart, uint8_t data)
{
    if (huart == NULL)
    {
        return UART_NULL_POINTER;
    }

    if (huart->Busy)
    {
        return UART_ERROR;
    }

    huart->Busy = true;
    huart->ErrorCode = UART_OK;

    if (UART_WaitFlagInternal(&(huart->Instance->SR), UART_BIT(UART_SR_TXE_POS)) != UART_OK)
    {
        return UART_HandleTimeout(huart);
    }

    huart->Instance->DR = (uint32_t)data;

    huart->Busy = false;
    return UART_OK;
}

UART_Status_t UART_ReadByte(UART_HandleTypeDef *huart, uint8_t *data)
{
    if ((huart == NULL) || (data == NULL))
    {
        return UART_NULL_POINTER;
    }

    if (huart->Busy)
    {
        return UART_ERROR;
    }

    huart->Busy = true;
    huart->ErrorCode = UART_OK;

    if (UART_WaitFlagInternal(&(huart->Instance->SR), UART_BIT(UART_SR_RXNE_POS)) != UART_OK)
    {
        return UART_HandleTimeout(huart);
    }

    *data = (uint8_t)(huart->Instance->DR & 0xFFU);

    huart->Busy = false;
    return UART_OK;
}

UART_Status_t UART_SendString(UART_HandleTypeDef *huart, const char *str)
{
    UART_Status_t status = UART_OK;

    if ((huart == NULL) || (str == NULL))
    {
        return UART_NULL_POINTER;
    }

    while (*str != '\0')
    {
        status = UART_SendByte(huart, (uint8_t)(*str));
        if (status != UART_OK)
        {
            break;
        }
        str++;
    }

    return status;
}

UART_Status_t UART_ReadString(UART_HandleTypeDef *huart, char *buffer, uint16_t maxLength)
{
    UART_Status_t status = UART_OK;
    uint16_t index = 0U;
    uint8_t receivedByte = 0U;

    if ((huart == NULL) || (buffer == NULL) || (maxLength == 0U))
    {
        return UART_NULL_POINTER;
    }

    while (index < (maxLength - 1U))
    {
        status = UART_ReadByte(huart, &receivedByte);
        if (status != UART_OK)
        {
            break;
        }

        if ((receivedByte == (uint8_t)'\r') || (receivedByte == (uint8_t)'\n'))
        {
            break;
        }

        buffer[index] = (char)receivedByte;
        index++;
    }

    buffer[index] = '\0';
    return status;
}

/*------------------------- Formatting APIs ---------------------------------*/

UART_Status_t UART_SendNumber(UART_HandleTypeDef *huart, int32_t number)
{
    char strBuffer[12];
    uint16_t idx = 0U;
    bool isNegative = false;
    uint32_t unsignedNum;

    if (number == 0)
    {
        return UART_SendByte(huart, (uint8_t)'0');
    }

    if (number < 0)
    {
        isNegative = true;
        unsignedNum = (uint32_t)(-number);
    }
    else
    {
        unsignedNum = (uint32_t)number;
    }

    while (unsignedNum > 0U)
    {
        strBuffer[idx] = (char)((unsignedNum % 10U) + (uint32_t)'0');
        idx++;
        unsignedNum /= 10U;
    }

    if (isNegative)
    {
        strBuffer[idx] = '-';
        idx++;
    }

    UART_ReverseString(strBuffer, idx);
    strBuffer[idx] = '\0';

    return UART_SendString(huart, strBuffer);
}

UART_Status_t UART_SendHex(UART_HandleTypeDef *huart, uint32_t value)
{
    const char hexDigits[] = "0123456789ABCDEF";
    char strBuffer[11];
    int8_t i;

    strBuffer[0] = '0';
    strBuffer[1] = 'x';

    for (i = 7; i >= 0; i--)
    {
        strBuffer[9 - i] = hexDigits[(value >> (i * 4)) & 0x0FU];
    }
    strBuffer[10] = '\0';

    return UART_SendString(huart, strBuffer);
}

UART_Status_t UART_SendFloat(UART_HandleTypeDef *huart, float value)
{
    int32_t intPart = (int32_t)value;
    float fractionalPart = value - (float)intPart;
    int32_t fracInt;
    UART_Status_t status;

    if (value < 0.0f)
    {
        fractionalPart = -fractionalPart;
        
        /* Handle edge case where intPart is 0 but value is negative (e.g. -0.5) */
        if (intPart == 0)
        {
            status = UART_SendByte(huart, (uint8_t)'-');
            if (status != UART_OK)
            {
                return status;
            }
        }
    }

    /* 3 decimal places with rounding */
    fracInt = (int32_t)((fractionalPart * 1000.0f) + 0.5f); 

    status = UART_SendNumber(huart, intPart);
    if (status == UART_OK)
    {
        status = UART_SendByte(huart, (uint8_t)'.');
        if (status == UART_OK)
        {
            /* Add leading zeros for the fractional part */
            if (fracInt < 10)
            {
                status = UART_SendString(huart, "00");
            }
            else if (fracInt < 100)
            {
                status = UART_SendString(huart, "0");
            }

            if (status == UART_OK)
            {
                status = UART_SendNumber(huart, fracInt);
            }
        }
    }

    return status;
}

/*------------------------- Ring Buffer APIs --------------------------------*/

void UART_RingBufferInit(UART_HandleTypeDef *huart)
{
    if (huart != NULL)
    {
        huart->RxRingBuffer.head = 0U;
        huart->RxRingBuffer.tail = 0U;
        
        huart->TxRingBuffer.head = 0U;
        huart->TxRingBuffer.tail = 0U;
    }
}

UART_Status_t UART_RingBufferWrite(UART_HandleTypeDef *huart, uint8_t data)
{
    uint16_t nextHead;

    if (huart == NULL)
    {
        return UART_NULL_POINTER;
    }

    nextHead = (huart->RxRingBuffer.head + 1U) % UART_RX_BUFFER_SIZE;

    if (nextHead == huart->RxRingBuffer.tail)
    {
        return UART_ERROR; /* Buffer Full */
    }

    huart->RxRingBuffer.buffer[huart->RxRingBuffer.head] = data;
    huart->RxRingBuffer.head = nextHead;

    return UART_OK;
}

UART_Status_t UART_RingBufferRead(UART_HandleTypeDef *huart, uint8_t *data)
{
    if ((huart == NULL) || (data == NULL))
    {
        return UART_NULL_POINTER;
    }

    if (huart->RxRingBuffer.head == huart->RxRingBuffer.tail)
    {
        return UART_ERROR; /* Buffer Empty */
    }

    *data = huart->RxRingBuffer.buffer[huart->RxRingBuffer.tail];
    huart->RxRingBuffer.tail = (huart->RxRingBuffer.tail + 1U) % UART_RX_BUFFER_SIZE;

    return UART_OK;
}

uint16_t UART_RingBufferAvailable(UART_HandleTypeDef *huart)
{
    uint16_t available = 0U;

    if (huart != NULL)
    {
        if (huart->RxRingBuffer.head >= huart->RxRingBuffer.tail)
        {
            available = huart->RxRingBuffer.head - huart->RxRingBuffer.tail;
        }
        else
        {
            available = UART_RX_BUFFER_SIZE - huart->RxRingBuffer.tail + huart->RxRingBuffer.head;
        }
    }

    return available;
}

/*------------------------- Interrupts & Callbacks --------------------------*/

UART_Status_t UART_EnableInterrupt(UART_HandleTypeDef *huart)
{
    if (huart == NULL)
    {
        return UART_NULL_POINTER;
    }

    huart->Instance->CR1 |= UART_BIT(UART_CR1_RXNEIE_POS);
    /* Note: TXEIE is typically enabled dynamically when there is data to send */

    return UART_OK;
}

UART_Status_t UART_DisableInterrupt(UART_HandleTypeDef *huart)
{
    if (huart == NULL)
    {
        return UART_NULL_POINTER;
    }

    huart->Instance->CR1 &= ~(UART_BIT(UART_CR1_RXNEIE_POS) | UART_BIT(UART_CR1_TXEIE_POS));

    return UART_OK;
}

void UART_IRQHandler(UART_HandleTypeDef *huart)
{
    uint32_t isrflags;
    uint32_t cr1its;
    uint8_t receivedData;

    if (huart == NULL)
    {
        return;
    }

    isrflags = huart->Instance->SR;
    cr1its   = huart->Instance->CR1;

    /* Handle Receive Data Register Not Empty (RXNE) Interrupt */
    if (((isrflags & UART_BIT(UART_SR_RXNE_POS)) != 0U) && ((cr1its & UART_BIT(UART_CR1_RXNEIE_POS)) != 0U))
    {
        receivedData = (uint8_t)(huart->Instance->DR & 0xFFU);
        (void)UART_RingBufferWrite(huart, receivedData);

        if (huart->Callback != NULL)
        {
            huart->Callback(UART_EVENT_RX_COMPLETE);
        }
    }

    /* Handle Transmit Data Register Empty (TXE) Interrupt */
    if (((isrflags & UART_BIT(UART_SR_TXE_POS)) != 0U) && ((cr1its & UART_BIT(UART_CR1_TXEIE_POS)) != 0U))
    {
        /* Check if there is data in the Tx ring buffer to send */
        if (huart->TxRingBuffer.head != huart->TxRingBuffer.tail)
        {
            huart->Instance->DR = (uint32_t)(huart->TxRingBuffer.buffer[huart->TxRingBuffer.tail]);
            huart->TxRingBuffer.tail = (huart->TxRingBuffer.tail + 1U) % UART_TX_BUFFER_SIZE;
        }
        else
        {
            /* Buffer empty, disable TXE interrupt */
            huart->Instance->CR1 &= ~UART_BIT(UART_CR1_TXEIE_POS);
            if (huart->Callback != NULL)
            {
                huart->Callback(UART_EVENT_TX_COMPLETE);
            }
        }
    }

    /* Handle Overrun Error (ORE) */
    if ((isrflags & UART_BIT(UART_SR_ORE_POS)) != 0U)
    {
        /* Clear ORE by reading SR then DR */
        volatile uint32_t dummy = huart->Instance->SR;
        dummy = huart->Instance->DR;
        (void)dummy;

        if (huart->Callback != NULL)
        {
            huart->Callback(UART_EVENT_ERROR);
        }
    }
}

/*------------------------- Status & Control (Future APIs) ------------------*/

void UART_Flush(UART_HandleTypeDef *huart)
{
    if (huart != NULL)
    {
        UART_RingBufferInit(huart);
    }
}

UART_Status_t UART_AbortTransmit(UART_HandleTypeDef *huart)
{
    if (huart == NULL)
    {
        return UART_NULL_POINTER;
    }

    huart->Instance->CR1 &= ~UART_BIT(UART_CR1_TXEIE_POS);
    huart->TxRingBuffer.head = 0U;
    huart->TxRingBuffer.tail = 0U;
    huart->Busy = false;

    return UART_OK;
}

UART_Status_t UART_AbortReceive(UART_HandleTypeDef *huart)
{
    if (huart == NULL)
    {
        return UART_NULL_POINTER;
    }

    huart->Instance->CR1 &= ~UART_BIT(UART_CR1_RXNEIE_POS);
    huart->RxRingBuffer.head = 0U;
    huart->RxRingBuffer.tail = 0U;
    huart->Busy = false;

    return UART_OK;
}

UART_Status_t UART_GetStatus(UART_HandleTypeDef *huart)
{
    UART_Status_t status = UART_ERROR;

    if (huart != NULL)
    {
        status = huart->ErrorCode;
    }

    return status;
}