/**
 * @file    uart.h
 * @brief   Universal Asynchronous Receiver-Transmitter (UART) Driver Header
 * @details Production-ready, MISRA-friendly header for STM32F407VG UART control.
 * Implements register-level configurations, polling, interrupts, and ring buffers.
 */

#ifndef UART_H
#define UART_H

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
 * MACROS & DEFINES
 *===========================================================================*/

/** @brief UART Ring Buffer Sizes */
#define UART_RX_BUFFER_SIZE             (128U)
#define UART_TX_BUFFER_SIZE             (128U)

/*=============================================================================
 * EXPORTED TYPES & ENUMS
 *===========================================================================*/

/**
 * @brief UART Word Length configuration
 */
typedef enum
{
    UART_WORDLENGTH_8B = 0x00U,
    UART_WORDLENGTH_9B = 0x01U
} UART_WordLength_t;

/**
 * @brief UART Stop Bits configuration
 */
typedef enum
{
    UART_STOPBITS_1 = 0x00U,
    UART_STOPBITS_2 = 0x01U
} UART_StopBits_t;

/**
 * @brief UART Parity configuration
 */
typedef enum
{
    UART_PARITY_NONE = 0x00U,
    UART_PARITY_EVEN = 0x01U,
    UART_PARITY_ODD  = 0x02U
} UART_Parity_t;

/**
 * @brief UART Operating Mode configuration
 */
typedef enum
{
    UART_MODE_RX    = 0x01U,
    UART_MODE_TX    = 0x02U,
    UART_MODE_TX_RX = 0x03U
} UART_Mode_t;

/**
 * @brief UART Standard Baud Rates
 */
typedef enum
{
    UART_BAUD_9600   = 9600U,
    UART_BAUD_19200  = 19200U,
    UART_BAUD_38400  = 38400U,
    UART_BAUD_57600  = 57600U,
    UART_BAUD_115200 = 115200U
} UART_BaudRate_t;

/**
 * @brief UART operation status codes
 */
typedef enum
{
    UART_OK           = 0x00U,
    UART_ERROR        = 0x01U,
    UART_TIMEOUT      = 0x02U,
    UART_NULL_POINTER = 0x03U
} UART_Status_t;

/**
 * @brief UART Event types for callbacks
 */
typedef enum
{
    UART_EVENT_TX_COMPLETE = 0x00U,
    UART_EVENT_RX_COMPLETE = 0x01U,
    UART_EVENT_ERROR       = 0x02U
} UART_Event_t;

/**
 * @brief UART callback function pointer type definition
 */
typedef void (*UART_Callback_t)(UART_Event_t event);

/**
 * @brief UART Configuration structure
 */
typedef struct
{
    UART_BaudRate_t   baudRate;
    UART_WordLength_t wordLength;
    UART_StopBits_t   stopBits;
    UART_Parity_t     parity;
    UART_Mode_t       mode;
} UART_Config_t;

/**
 * @brief UART Ring Buffer structure
 */
typedef struct
{
    uint8_t           buffer[UART_RX_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} UART_RingBuffer_t;

/**
 * @brief UART handle structure definition
 */
typedef struct
{
    USART_TypeDef      *Instance;       /**< Pointer to UART peripheral base address */
    UART_Config_t       Init;           /**< UART configuration parameters */
    UART_Callback_t     Callback;       /**< Pointer to the event callback function */
    UART_RingBuffer_t   RxRingBuffer;   /**< Receive Ring Buffer instance */
    UART_RingBuffer_t   TxRingBuffer;   /**< Transmit Ring Buffer instance */
    volatile bool       Busy;           /**< UART busy state flag */
    UART_Status_t       ErrorCode;      /**< UART current error code */
} UART_HandleTypeDef;

/*=============================================================================
 * PUBLIC API FUNCTION PROTOTYPES
 *===========================================================================*/

/*------------------------- Initialization ----------------------------------*/

/**
 * @brief Initializes the UART peripheral
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @return UART_Status_t Status of the initialization
 */
UART_Status_t UART_Init(UART_HandleTypeDef *huart);

/**
 * @brief De-initializes the UART peripheral
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @return UART_Status_t Status of the de-initialization
 */
UART_Status_t UART_DeInit(UART_HandleTypeDef *huart);

/*------------------------- Polling Transmit/Receive ------------------------*/

/**
 * @brief Sends a single byte via UART in blocking mode
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @param data Byte to be transmitted
 * @return UART_Status_t Status of the transmission
 */
UART_Status_t UART_SendByte(UART_HandleTypeDef *huart, uint8_t data);

/**
 * @brief Reads a single byte from UART in blocking mode
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @param data Pointer to store the received byte
 * @return UART_Status_t Status of the reception
 */
UART_Status_t UART_ReadByte(UART_HandleTypeDef *huart, uint8_t *data);

/**
 * @brief Sends a null-terminated string via UART
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @param str Pointer to the null-terminated string
 * @return UART_Status_t Status of the transmission
 */
UART_Status_t UART_SendString(UART_HandleTypeDef *huart, const char *str);

/**
 * @brief Reads a string from UART until a newline or maximum length is reached
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @param buffer Pointer to the data buffer
 * @param maxLength Maximum number of characters to read
 * @return UART_Status_t Status of the reception
 */
UART_Status_t UART_ReadString(UART_HandleTypeDef *huart, char *buffer, uint16_t maxLength);

/**
 * @brief Sends a signed 32-bit integer formatted as a string
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @param number Integer value to be sent
 * @return UART_Status_t Status of the transmission
 */
UART_Status_t UART_SendNumber(UART_HandleTypeDef *huart, int32_t number);

/**
 * @brief Sends an unsigned 32-bit integer formatted as a hexadecimal string
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @param value Hexadecimal value to be sent
 * @return UART_Status_t Status of the transmission
 */
UART_Status_t UART_SendHex(UART_HandleTypeDef *huart, uint32_t value);

/**
 * @brief Sends a floating-point number formatted as a string
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @param value Floating-point value to be sent
 * @return UART_Status_t Status of the transmission
 */
UART_Status_t UART_SendFloat(UART_HandleTypeDef *huart, float value);

/*------------------------- Ring Buffer APIs --------------------------------*/

/**
 * @brief Initializes the UART ring buffers
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 */
void UART_RingBufferInit(UART_HandleTypeDef *huart);

/**
 * @brief Writes a single byte into the UART ring buffer
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @param data Byte to be written
 * @return UART_Status_t Status of the write operation
 */
UART_Status_t UART_RingBufferWrite(UART_HandleTypeDef *huart, uint8_t data);

/**
 * @brief Reads a single byte from the UART ring buffer
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @param data Pointer to store the read byte
 * @return UART_Status_t Status of the read operation
 */
UART_Status_t UART_RingBufferRead(UART_HandleTypeDef *huart, uint8_t *data);

/**
 * @brief Returns the number of unread bytes available in the ring buffer
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @return uint16_t Number of bytes available
 */
uint16_t UART_RingBufferAvailable(UART_HandleTypeDef *huart);

/*------------------------- Interrupts & Callbacks --------------------------*/

/**
 * @brief Enables UART interrupts
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @return UART_Status_t Status of the interrupt enable operation
 */
UART_Status_t UART_EnableInterrupt(UART_HandleTypeDef *huart);

/**
 * @brief Disables UART interrupts
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @return UART_Status_t Status of the interrupt disable operation
 */
UART_Status_t UART_DisableInterrupt(UART_HandleTypeDef *huart);

/**
 * @brief UART Interrupt Request Handler
 * @details Should be called from the respective USARTx_IRQHandler
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 */
void UART_IRQHandler(UART_HandleTypeDef *huart);

/*------------------------- Status & Control (Future APIs) ------------------*/

/**
 * @brief Flushes the UART transmit and receive ring buffers
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 */
void UART_Flush(UART_HandleTypeDef *huart);

/**
 * @brief Aborts an ongoing transmit operation
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @return UART_Status_t Status of the abort operation
 */
UART_Status_t UART_AbortTransmit(UART_HandleTypeDef *huart);

/**
 * @brief Aborts an ongoing receive operation
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @return UART_Status_t Status of the abort operation
 */
UART_Status_t UART_AbortReceive(UART_HandleTypeDef *huart);

/**
 * @brief Retrieves the current status and error state of the UART
 *
 * @param huart Pointer to a UART_HandleTypeDef structure
 * @return UART_Status_t Current operational status
 */
UART_Status_t UART_GetStatus(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif /* UART_H */