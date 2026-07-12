/**
 * @file    gpio.h
 * @brief   General Purpose Input/Output (GPIO) Driver Header
 * @details Production-ready, MISRA-friendly header for STM32F407VG GPIO control.
 * Implements register-level configurations, state management, and type-safe APIs.
 */

#ifndef GPIO_H
#define GPIO_H

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
 * EXPORTED TYPES & ENUMS
 *===========================================================================*/

/**
 * @brief GPIO Pin definitions
 */
typedef enum
{
    GPIO_PIN_0  = 0x00U,
    GPIO_PIN_1  = 0x01U,
    GPIO_PIN_2  = 0x02U,
    GPIO_PIN_3  = 0x03U,
    GPIO_PIN_4  = 0x04U,
    GPIO_PIN_5  = 0x05U,
    GPIO_PIN_6  = 0x06U,
    GPIO_PIN_7  = 0x07U,
    GPIO_PIN_8  = 0x08U,
    GPIO_PIN_9  = 0x09U,
    GPIO_PIN_10 = 0x0AU,
    GPIO_PIN_11 = 0x0BU,
    GPIO_PIN_12 = 0x0CU,
    GPIO_PIN_13 = 0x0DU,
    GPIO_PIN_14 = 0x0EU,
    GPIO_PIN_15 = 0x0FU
} GPIO_Pin_t;

/**
 * @brief GPIO Mode configuration
 */
typedef enum
{
    GPIO_MODE_INPUT     = 0x00U,
    GPIO_MODE_OUTPUT    = 0x01U,
    GPIO_MODE_ALTERNATE = 0x02U,
    GPIO_MODE_ANALOG    = 0x03U
} GPIO_Mode_t;

/**
 * @brief GPIO Pull-up/Pull-down configuration
 */
typedef enum
{
    GPIO_NOPULL   = 0x00U,
    GPIO_PULLUP   = 0x01U,
    GPIO_PULLDOWN = 0x02U
} GPIO_Pull_t;

/**
 * @brief GPIO Output Speed configuration
 */
typedef enum
{
    GPIO_SPEED_LOW    = 0x00U,
    GPIO_SPEED_MEDIUM = 0x01U,
    GPIO_SPEED_FAST   = 0x02U,
    GPIO_SPEED_HIGH   = 0x03U
} GPIO_Speed_t;

/**
 * @brief GPIO Output Type configuration
 */
typedef enum
{
    GPIO_PUSH_PULL  = 0x00U,
    GPIO_OPEN_DRAIN = 0x01U
} GPIO_OutputType_t;

/**
 * @brief GPIO Alternate Function configuration
 */
typedef enum
{
    GPIO_AF0  = 0x00U,
    GPIO_AF1  = 0x01U,
    GPIO_AF2  = 0x02U,
    GPIO_AF3  = 0x03U,
    GPIO_AF4  = 0x04U,
    GPIO_AF5  = 0x05U,
    GPIO_AF6  = 0x06U,
    GPIO_AF7  = 0x07U,
    GPIO_AF8  = 0x08U,
    GPIO_AF9  = 0x09U,
    GPIO_AF10 = 0x0AU,
    GPIO_AF11 = 0x0BU,
    GPIO_AF12 = 0x0CU,
    GPIO_AF13 = 0x0DU,
    GPIO_AF14 = 0x0EU,
    GPIO_AF15 = 0x0FU
} GPIO_Alternate_t;

/**
 * @brief GPIO Pin State definition
 */
typedef enum
{
    GPIO_PIN_RESET = 0x00U,
    GPIO_PIN_SET   = 0x01U
} GPIO_PinState_t;

/**
 * @brief GPIO operation status codes
 */
typedef enum
{
    GPIO_OK                 = 0x00U,
    GPIO_ERROR_INVALID_PORT = 0x01U,
    GPIO_ERROR_INVALID_PIN  = 0x02U,
    GPIO_ERROR_INVALID_MODE = 0x03U,
    GPIO_ERROR_NULL_POINTER = 0x04U
} GPIO_Status_t;

/**
 * @brief GPIO Configuration structure
 */
typedef struct
{
    GPIO_Pin_t        pin;        /**< Specifies the GPIO pin to be configured */
    GPIO_Mode_t       mode;       /**< Specifies the operating mode for the selected pin */
    GPIO_OutputType_t outputType; /**< Specifies the output type for the selected pin */
    GPIO_Speed_t      speed;      /**< Specifies the speed for the selected pin */
    GPIO_Pull_t       pull;       /**< Specifies the Pull-up or Pull-Down activation for the selected pin */
    GPIO_Alternate_t  alternate;  /**< Specifies the Alternate Function for the selected pin */
} GPIO_Config_t;

/*=============================================================================
 * PUBLIC API FUNCTION PROTOTYPES
 *===========================================================================*/

/**
 * @brief Initializes the GPIO peripheral according to the specified parameters
 *
 * @param GPIOx Pointer to GPIO peripheral base address
 * @param config Pointer to a GPIO_Config_t structure that contains the configuration
 * @return GPIO_Status_t Status of the initialization
 */
GPIO_Status_t GPIO_Init(GPIO_TypeDef *GPIOx, const GPIO_Config_t *config);

/**
 * @brief De-initializes the GPIO peripheral, reverting to default reset state
 *
 * @param GPIOx Pointer to GPIO peripheral base address
 * @param pin The GPIO pin to be de-initialized
 * @return GPIO_Status_t Status of the de-initialization
 */
GPIO_Status_t GPIO_DeInit(GPIO_TypeDef *GPIOx, GPIO_Pin_t pin);

/**
 * @brief Sets or clears the selected data port bit
 *
 * @param GPIOx Pointer to GPIO peripheral base address
 * @param pin Specifies the port bit to be written
 * @param state Specifies the value to be written to the selected bit
 */
void GPIO_WritePin(GPIO_TypeDef *GPIOx, GPIO_Pin_t pin, GPIO_PinState_t state);

/**
 * @brief Reads the specified input port pin
 *
 * @param GPIOx Pointer to GPIO peripheral base address
 * @param pin Specifies the port bit to read
 * @return GPIO_PinState_t The input port pin value (GPIO_PIN_SET or GPIO_PIN_RESET)
 */
GPIO_PinState_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, GPIO_Pin_t pin);

/**
 * @brief Toggles the specified GPIO pin
 *
 * @param GPIOx Pointer to GPIO peripheral base address
 * @param pin Specifies the port bit to toggle
 */
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, GPIO_Pin_t pin);

/*------------------------- Future APIs -------------------------------------*/

/**
 * @brief Locks GPIO Pins configuration registers
 *
 * @param GPIOx Pointer to GPIO peripheral base address
 * @param pin Specifies the port bit to be locked
 * @return GPIO_Status_t Status of the lock operation
 */
GPIO_Status_t GPIO_LockPin(GPIO_TypeDef *GPIOx, GPIO_Pin_t pin);

/**
 * @brief Configures Alternate Function for a specified pin dynamically
 *
 * @param GPIOx Pointer to GPIO peripheral base address
 * @param pin Specifies the port bit
 * @param alternate Alternate function to be configured
 * @return GPIO_Status_t Status of the configuration
 */
GPIO_Status_t GPIO_SetAlternateFunction(GPIO_TypeDef *GPIOx, GPIO_Pin_t pin, GPIO_Alternate_t alternate);

/**
 * @brief Reads the entire GPIO port value
 *
 * @param GPIOx Pointer to GPIO peripheral base address
 * @param portValue Pointer to store the port value
 * @return GPIO_Status_t Status of the read operation
 */
GPIO_Status_t GPIO_ReadPort(GPIO_TypeDef *GPIOx, uint16_t *portValue);

/**
 * @brief Writes to the entire GPIO port
 *
 * @param GPIOx Pointer to GPIO peripheral base address
 * @param portValue The value to be written to the port
 * @return GPIO_Status_t Status of the write operation
 */
GPIO_Status_t GPIO_WritePort(GPIO_TypeDef *GPIOx, uint16_t portValue);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_H */