/**
 * @file    spi.h
 * @brief   Serial Peripheral Interface (SPI) Driver Header
 * @details Production-ready, MISRA-friendly header for STM32F407VG SPI control.
 * Implements register-level configurations and state management.
 */

#ifndef SPI_H
#define SPI_H

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
 * EXPORTED TYPES & ENUMS
 *===========================================================================*/

/**
 * @brief SPI operation status codes
 */
typedef enum
{
    SPI_OK             = 0x00U,  /**< Operation successful */
    SPI_ERROR          = 0x01U,  /**< General error */
    SPI_TIMEOUT        = 0x02U,  /**< Operation timed out */
    SPI_NULL_POINTER   = 0x03U,  /**< Invalid null pointer provided */
    SPI_NOT_SUPPORTED  = 0x04U   /**< Feature not supported (e.g., DMA stubs) */
} SPI_Status_t;

/**
 * @brief SPI Error codes (Bitmask values mapped to hardware flags)
 */
typedef enum
{
    SPI_ERROR_NONE       = 0x00U,       /**< No error */
    SPI_ERROR_OVERRUN    = (1U << 0),   /**< Overrun error (OVR) */
    SPI_ERROR_MODE_FAULT = (1U << 1),   /**< Mode fault (MODF) */
    SPI_ERROR_CRC        = (1U << 2),   /**< CRC error (CRCERR) */
    SPI_ERROR_FRAME      = (1U << 3),   /**< Frame format error (FRE) */
    SPI_ERROR_TIMEOUT    = (1U << 4)    /**< Timeout error */
} SPI_Error_t;

/**
 * @brief SPI Operating Mode (Master/Slave)
 */
typedef enum
{
    SPI_MODE_SLAVE  = 0x00U,
    SPI_MODE_MASTER = 0x01U
} SPI_Mode_t;

/**
 * @brief SPI Clock Polarity (CPOL)
 */
typedef enum
{
    SPI_CPOL_LOW  = 0x00U,
    SPI_CPOL_HIGH = 0x01U
} SPI_CPOL_t;

/**
 * @brief SPI Clock Phase (CPHA)
 */
typedef enum
{
    SPI_CPHA_FIRST_EDGE  = 0x00U,
    SPI_CPHA_SECOND_EDGE = 0x01U
} SPI_CPHA_t;

/**
 * @brief SPI Data Size (8-bit or 16-bit)
 */
typedef enum
{
    SPI_DATASIZE_8BIT  = 0x00U,
    SPI_DATASIZE_16BIT = 0x01U
} SPI_DataSize_t;

/**
 * @brief SPI Frame Format (MSB or LSB First)
 */
typedef enum
{
    SPI_MSB_FIRST = 0x00U,
    SPI_LSB_FIRST = 0x01U
} SPI_FirstBit_t;

/**
 * @brief SPI Configuration structure
 */
typedef struct
{
    SPI_Mode_t     mode;        /**< Specifies the SPI operating mode */
    SPI_CPOL_t     cpol;        /**< Specifies the serial clock steady state */
    SPI_CPHA_t     cpha;        /**< Specifies the clock active edge for the bit capture */
    SPI_DataSize_t dataSize;    /**< Specifies the SPI data size */
    SPI_FirstBit_t firstBit;    /**< Specifies whether data transfers start from MSB or LSB bit */
} SPI_Config_t;

/**
 * @brief SPI callback function pointer type definition
 */
typedef void (*SPI_Callback_t)(void);

/**
 * @brief SPI handle structure definition
 */
typedef struct
{
    SPI_TypeDef      *Instance;                 /**< Pointer to SPI peripheral base address */
    SPI_Config_t      Init;                     /**< SPI configuration parameters */
    volatile bool     Busy;                     /**< SPI running state flag (true = busy, false = ready) */
    SPI_Error_t       ErrorCode;                /**< SPI current error code */
    SPI_Callback_t    TransferCompleteCallback; /**< Callback function pointer for Transfer Complete */
} SPI_HandleTypeDef;

/*=============================================================================
 * PUBLIC API FUNCTION PROTOTYPES
 *===========================================================================*/

/*------------------------- Initialization ----------------------------------*/

/**
 * @brief Initializes the SPI peripheral
 *
 * @param hspi Pointer to an SPI_HandleTypeDef structure
 * @return SPI_Status_t Status of the initialization
 */
SPI_Status_t SPI_Init(SPI_HandleTypeDef *hspi);

/**
 * @brief De-initializes the SPI peripheral
 *
 * @param hspi Pointer to an SPI_HandleTypeDef structure
 * @return SPI_Status_t Status of the de-initialization
 */
SPI_Status_t SPI_DeInit(SPI_HandleTypeDef *hspi);

/*------------------------- Polling Transmit/Receive ------------------------*/

/**
 * @brief Transmits data via SPI in blocking mode
 *
 * @param hspi Pointer to an SPI_HandleTypeDef structure
 * @param txData Pointer to the data buffer to be transmitted
 * @param size Amount of data to be sent
 * @return SPI_Status_t Status of the transmission
 */
SPI_Status_t SPI_Transmit(SPI_HandleTypeDef *hspi, const uint8_t *txData, uint16_t size);

/**
 * @brief Receives data via SPI in blocking mode
 *
 * @param hspi Pointer to an SPI_HandleTypeDef structure
 * @param rxData Pointer to the buffer where received data will be stored
 * @param size Amount of data to be received
 * @return SPI_Status_t Status of the reception
 */
SPI_Status_t SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *rxData, uint16_t size);

/**
 * @brief Transmits and receives data via SPI simultaneously in blocking mode
 *
 * @param hspi Pointer to an SPI_HandleTypeDef structure
 * @param txData Pointer to the data buffer to be transmitted
 * @param rxData Pointer to the buffer where received data will be stored
 * @param size Amount of data to be processed
 * @return SPI_Status_t Status of the operation
 */
SPI_Status_t SPI_TransmitReceive(SPI_HandleTypeDef *hspi, const uint8_t *txData, uint8_t *rxData, uint16_t size);

/*------------------------- Interrupts & Callbacks --------------------------*/

/**
 * @brief Enables SPI interrupts
 *
 * @param hspi Pointer to an SPI_HandleTypeDef structure
 * @return SPI_Status_t Status of the interrupt enable operation
 */
SPI_Status_t SPI_EnableInterrupt(SPI_HandleTypeDef *hspi);

/**
 * @brief Disables SPI interrupts
 *
 * @param hspi Pointer to an SPI_HandleTypeDef structure
 * @return SPI_Status_t Status of the interrupt disable operation
 */
SPI_Status_t SPI_DisableInterrupt(SPI_HandleTypeDef *hspi);

/**
 * @brief SPI Interrupt Request Handler
 * @details Should be called from the respective SPIx_IRQHandler
 *
 * @param hspi Pointer to an SPI_HandleTypeDef structure
 */
void SPI_IRQHandler(SPI_HandleTypeDef *hspi);

/**
 * @brief Registers a user callback for the Transfer Complete event
 *
 * @param hspi Pointer to an SPI_HandleTypeDef structure
 * @param callback Function pointer to the user callback
 */
void SPI_RegisterCallback(SPI_HandleTypeDef *hspi, SPI_Callback_t callback);

/*------------------------- Error & Status Handling -------------------------*/

/**
 * @brief Retrieves the current operating status and error state of the SPI
 *
 * @param hspi Pointer to an SPI_HandleTypeDef structure
 * @return SPI_Status_t Current operational status
 */
SPI_Status_t SPI_GetStatus(SPI_HandleTypeDef *hspi);

/**
 * @brief Retrieves the current SPI error code
 *
 * @param hspi Pointer to an SPI_HandleTypeDef structure
 * @return SPI_Error_t Current error state
 */
SPI_Error_t SPI_GetError(SPI_HandleTypeDef *hspi);

/**
 * @brief Clears the current SPI error code
 *
 * @param hspi Pointer to an SPI_HandleTypeDef structure
 */
void SPI_ClearError(SPI_HandleTypeDef *hspi);

/*------------------------- DMA Transmit/Receive (Stubs) --------------------*/

/**
 * @brief Transmits data via DMA (Stub)
 *
 * @param hspi Pointer to an SPI_HandleTypeDef structure
 * @param txData Pointer to the data buffer to be transmitted
 * @param size Amount of data to be sent
 * @return SPI_Status_t Status of the transmission
 */
SPI_Status_t SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, const uint8_t *txData, uint16_t size);

/**
 * @brief Receives data via DMA (Stub)
 *
 * @param hspi Pointer to an SPI_HandleTypeDef structure
 * @param rxData Pointer to the buffer where received data will be stored
 * @param size Amount of data to be received
 * @return SPI_Status_t Status of the reception
 */
SPI_Status_t SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *rxData, uint16_t size);

/**
 * @brief Transmits and receives data via DMA (Stub)
 *
 * @param hspi Pointer to an SPI_HandleTypeDef structure
 * @param txData Pointer to the data buffer to be transmitted
 * @param rxData Pointer to the buffer where received data will be stored
 * @param size Amount of data to be processed
 * @return SPI_Status_t Status of the operation
 */
SPI_Status_t SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi, const uint8_t *txData, uint8_t *rxData, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif /* SPI_H */