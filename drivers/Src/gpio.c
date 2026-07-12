/**
 * @file    gpio.c
 * @brief   General Purpose Input/Output (GPIO) Driver Implementation
 * @details Production-ready, MISRA-friendly driver for STM32F407VG.
 * Implements register-level configuration, read/write, and toggle operations.
 */

#include "gpio.h"
#include "rcc.h"
#include "bit_utils.h"

/*=============================================================================
 * PRIVATE MACROS
 *===========================================================================*/

#define GPIO_MODER_MASK         (0x03U)
#define GPIO_OSPEEDR_MASK       (0x03U)
#define GPIO_PUPDR_MASK         (0x03U)
#define GPIO_AFR_MASK           (0x0FU)

/*=============================================================================
 * PRIVATE HELPER FUNCTIONS
 *===========================================================================*/

/**
 * @brief Internal helper to validate if the pin is within range
 */
static bool GPIO_IsPinValid(GPIO_Pin_t pin)
{
    return (pin <= GPIO_PIN_15);
}

/*=============================================================================
 * PUBLIC API IMPLEMENTATIONS
 *===========================================================================*/

/**
 * @brief Initializes the GPIO peripheral according to the specified parameters
 */
GPIO_Status_t GPIO_Init(GPIO_TypeDef *GPIOx, const GPIO_Config_t *config)
{
    /* NULL Pointer Validation */
    if ((GPIOx == NULL) || (config == NULL))
    {
        return GPIO_ERROR_NULL_POINTER;
    }

    /* Validate GPIO Port (STM32F407 supports A-I) */
    if ((GPIOx != GPIOA) && (GPIOx != GPIOB) && (GPIOx != GPIOC) &&
        (GPIOx != GPIOD) && (GPIOx != GPIOE) && (GPIOx != GPIOF) &&
        (GPIOx != GPIOG) && (GPIOx != GPIOH) && (GPIOx != GPIOI))
    {
        return GPIO_ERROR_INVALID_PORT;
    }

    /* Validate Pin */
    if (!GPIO_IsPinValid(config->pin))
    {
        return GPIO_ERROR_INVALID_PIN;
    }

    /* Enable GPIO Clock */
    RCC_EnableGPIOClock(GPIOx);

    uint32_t pinPos = (uint32_t)config->pin;

    /* Configure Mode */
    MODIFY_REG(GPIOx->MODER, (GPIO_MODER_MASK << (pinPos * 2U)), 
               ((uint32_t)config->mode << (pinPos * 2U)));

    /* Configure Output Type */
    MODIFY_REG(GPIOx->OTYPER, (1U << pinPos), 
               ((uint32_t)config->outputType << pinPos));

    /* Configure Speed */
    MODIFY_REG(GPIOx->OSPEEDR, (GPIO_OSPEEDR_MASK << (pinPos * 2U)), 
               ((uint32_t)config->speed << (pinPos * 2U)));

    /* Configure Pull-Up/Pull-Down */
    MODIFY_REG(GPIOx->PUPDR, (GPIO_PUPDR_MASK << (pinPos * 2U)), 
               ((uint32_t)config->pull << (pinPos * 2U)));

    /* Configure Alternate Function */
    if (config->mode == GPIO_MODE_ALTERNATE)
    {
        uint32_t afReg = (pinPos / 8U);
        uint32_t afPos = (pinPos % 8U) * 4U;
        MODIFY_REG(GPIOx->AFR[afReg], (GPIO_AFR_MASK << afPos), 
                   ((uint32_t)config->alternate << afPos));
    }

    return GPIO_OK;
}

/**
 * @brief De-initializes the GPIO pin to reset state
 */
GPIO_Status_t GPIO_DeInit(GPIO_TypeDef *GPIOx, GPIO_Pin_t pin)
{
    if ((GPIOx == NULL) || !GPIO_IsPinValid(pin))
    {
        return GPIO_ERROR_INVALID_PIN;
    }

    uint32_t pinPos = (uint32_t)pin;

    /* Reset to Input mode (00), No Pull (00), Default Speed */
    CLEAR_BIT(GPIOx->MODER, (GPIO_MODER_MASK << (pinPos * 2U)));
    CLEAR_BIT(GPIOx->PUPDR, (GPIO_PUPDR_MASK << (pinPos * 2U)));
    CLEAR_BIT(GPIOx->OSPEEDR, (GPIO_OSPEEDR_MASK << (pinPos * 2U)));
    CLEAR_BIT(GPIOx->OTYPER, (1U << pinPos));

    /* Clear Alternate Function mapping */
    uint32_t afReg = (pinPos / 8U);
    uint32_t afPos = (pinPos % 8U) * 4U;
    MODIFY_REG(GPIOx->AFR[afReg], (GPIO_AFR_MASK << afPos), 0U);

    return GPIO_OK;
}

/**
 * @brief Writes a state to a GPIO pin
 */
void GPIO_WritePin(GPIO_TypeDef *GPIOx, GPIO_Pin_t pin, GPIO_PinState_t state)
{
    if ((GPIOx != NULL) && GPIO_IsPinValid(pin))
    {
        if (state == GPIO_PIN_SET)
        {
            GPIOx->BSRR = (1U << (uint32_t)pin);
        }
        else
        {
            GPIOx->BSRR = (1U << ((uint32_t)pin + 16U));
        }
    }
}

/**
 * @brief Reads the state of a GPIO pin
 */
GPIO_PinState_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, GPIO_Pin_t pin)
{
    GPIO_PinState_t state = GPIO_PIN_RESET;

    if ((GPIOx != NULL) && GPIO_IsPinValid(pin))
    {
        state = ((GPIOx->IDR & (1U << (uint32_t)pin)) != 0U) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    }

    return state;
}

/**
 * @brief Toggles a GPIO pin
 */
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, GPIO_Pin_t pin)
{
    if ((GPIOx != NULL) && GPIO_IsPinValid(pin))
    {
        TOGGLE_BIT(GPIOx->ODR, (1U << (uint32_t)pin));
    }
}