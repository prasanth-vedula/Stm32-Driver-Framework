/**
 * @file    i2c.h
 * @brief   Inter-Integrated Circuit (I2C) Driver Header
 * @details Production-ready, MISRA-friendly header for STM32F407VG I2C control.
 * Implements register-level configurations and state management.
 */

#ifndef I2C_H
#define I2C_H

#include "system.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
 * EXPORTED TYPES & ENUMS
 *===========================================================================*/

/**
 * @brief I2C operation status codes
 */
typedef enum
{
    I2C_OK             = 0x00U,  /**< Operation successful */
    I2C_ERROR          = 0x01U,  /**< General error */
    I2C_TIMEOUT        = 0x02U,  /**< Operation timed out */
    I2C_NULL_POINTER   = 0x03U,  /**< Invalid null pointer provided */
    I2C_NOT_SUPPORTED  = 0x04U   /**< Feature not supported (e.g., DMA stubs) */
} I2C_Status_t;

/**
 * @brief I2C Error codes
 */
typedef enum
{
    I2C_ERROR_NONE    = 0x00U,       /**< No error */
    I2C_ERROR_BERR    = (1U << 0),   /**< Bus error */
    I2C_ERROR_ARLO    = (1U << 1),   /**< Arbitration lost */
    I2C_ERROR_AF      = (1U << 2),   /**< Acknowledge failure */
    I2C_ERROR_OVR     = (1U << 3),   /**< Overrun/Underrun */
    I2C_ERROR_TIMEOUT = (1U << 4)    /**< Timeout error */
} I2C_Error_t;

/**
 * @brief I2C Mode configuration
 */
typedef enum
{
    I2C_MODE_MASTER = 0x00U,
    I2C_MODE_SLAVE  = 0x01U
} I2C_Mode_t;

/**
 * @brief I2C Address Mode (7-bit or 10-bit)
 */
typedef enum
{
    I2C_ADDRESS_7BIT  = 0x00U,
    I2C_ADDRESS_10BIT = 0x01U
} I2C_AddressMode_t;

/**
 * @brief I2C Fast Mode Duty Cycle
 */
typedef enum
{
    I2C_DUTY_2    = 0x00U,
    I2C_DUTY_16_9 = 0x01U
} I2C_DutyCycle_t;

/**
 * @brief I2C Clock Speed definitions
 */
typedef enum
{
    I2C_SPEED_STANDARD = 100000U, /**< Standard mode: 100 kHz */
    I2C_SPEED_FAST     = 400000U  /**< Fast mode: 400 kHz */
} I2C_ClockSpeed_t;

/**
 * @brief I2C Configuration structure
 */
typedef struct
{
    I2C_ClockSpeed_t  clockSpeed;   /**< I2C operating speed */
    I2C_DutyCycle_t   dutyCycle;    /**< I2C fast mode duty cycle */
    uint16_t          ownAddress;   /**< I2C equipment own address */
    I2C_AddressMode_t addressMode;  /**< I2C addressing mode */
    I2C_Mode_t        mode;         /**< I2C master/slave mode */
} I2C_Config_t;

/**
 * @brief I2C callback function pointer type definition
 */
typedef void (*I2C_Callback_t)(void);

/**
 * @brief I2C handle structure definition
 */
typedef struct
{
    I2C_TypeDef      *Instance;                 /**< Pointer to I2C peripheral base address */
    I2C_Config_t      Init;                     /**< I2C configuration parameters */
    volatile bool     Busy;                     /**< I2C running state flag (true = busy, false = ready) */
    I2C_Error_t       ErrorCode;                /**< I2C current error code */
    I2C_Callback_t    TransferCompleteCallback; /**< Callback function pointer for Transfer Complete */
} I2C_HandleTypeDef;

/*=============================================================================
 * PUBLIC API FUNCTION PROTOTYPES
 *===========================================================================*/

/*------------------------- Initialization ----------------------------------*/

/**
 * @brief Initializes the I2C peripheral
 *
 * @param hi2c Pointer to an I2C_HandleTypeDef structure
 * @return I2C_Status_t Status of the initialization
 */
I2C_Status_t I2C_Init(I2C_HandleTypeDef *hi2c);

/**
 * @brief De-initializes the I2C peripheral
 *
 * @param hi2c Pointer to an I2C_HandleTypeDef structure
 * @return I2C_Status_t Status of the de-initialization
 */
I2C_Status_t I2C_DeInit(I2C_HandleTypeDef *hi2c);

/*------------------------- Polling Transmit/Receive ------------------------*/

/**
 * @brief Transmits data in Master mode (Blocking)
 *
 * @param hi2c Pointer to an I2C_HandleTypeDef structure
 * @param address Target device address
 * @param data Pointer to the data buffer
 * @param size Amount of data to be sent
 * @return I2C_Status_t Status of the transmission
 */
I2C_Status_t I2C_MasterTransmit(I2C_HandleTypeDef *hi2c, uint16_t address, const uint8_t *data, uint16_t size);

/**
 * @brief Receives data in Master mode (Blocking)
 *
 * @param hi2c Pointer to an I2C_HandleTypeDef structure
 * @param address Target device address
 * @param data Pointer to the buffer where received data will be stored
 * @param size Amount of data to be received
 * @return I2C_Status_t Status of the reception
 */
I2C_Status_t I2C_MasterReceive(I2C_HandleTypeDef *hi2c, uint16_t address, uint8_t *data, uint16_t size);

/*------------------------- Interrupts & Callbacks --------------------------*/

/**
 * @brief Enables I2C interrupts
 *
 * @param hi2c Pointer to an I2C_HandleTypeDef structure
 * @return I2C_Status_t Status of the interrupt enable operation
 */
I2C_Status_t I2C_EnableInterrupt(I2C_HandleTypeDef *hi2c);

/**
 * @brief Disables I2C interrupts
 *
 * @param hi2c Pointer to an I2C_HandleTypeDef structure
 * @return I2C_Status_t Status of the interrupt disable operation
 */
I2C_Status_t I2C_DisableInterrupt(I2C_HandleTypeDef *hi2c);

/**
 * @brief I2C Interrupt Request Handler
 * @details Should be called from the respective I2Cx_EV_IRQHandler / I2Cx_ER_IRQHandler
 *
 * @param hi2c Pointer to an I2C_HandleTypeDef structure
 */
void I2C_IRQHandler(I2C_HandleTypeDef *hi2c);

/**
 * @brief Registers a user callback for the Transfer Complete event
 *
 * @param hi2c Pointer to an I2C_HandleTypeDef structure
 * @param callback Function pointer to the user callback
 */
void I2C_RegisterCallback(I2C_HandleTypeDef *hi2c, I2C_Callback_t callback);

/*------------------------- Error Handling ----------------------------------*/

/**
 * @brief Retrieves the current I2C error code
 *
 * @param hi2c Pointer to an I2C_HandleTypeDef structure
 * @return I2C_Error_t Current error state
 */
I2C_Error_t I2C_GetError(I2C_HandleTypeDef *hi2c);

/**
 * @brief Clears the current I2C error code
 *
 * @param hi2c Pointer to an I2C_HandleTypeDef structure
 */
void I2C_ClearError(I2C_HandleTypeDef *hi2c);

/*------------------------- DMA Transmit/Receive (Stubs) --------------------*/

/**
 * @brief Transmits data in Master mode via DMA
 *
 * @param hi2c Pointer to an I2C_HandleTypeDef structure
 * @param address Target device address
 * @param data Pointer to the data buffer
 * @param size Amount of data to be sent
 * @return I2C_Status_t Status of the transmission
 */
I2C_Status_t I2C_MasterTransmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t address, const uint8_t *data, uint16_t size);

/**
 * @brief Receives data in Master mode via DMA
 *
 * @param hi2c Pointer to an I2C_HandleTypeDef structure
 * @param address Target device address
 * @param data Pointer to the buffer where received data will be stored
 * @param size Amount of data to be received
 * @return I2C_Status_t Status of the reception
 */
I2C_Status_t I2C_MasterReceive_DMA(I2C_HandleTypeDef *hi2c, uint16_t address, uint8_t *data, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif /* I2C_H */