/**
 * @file    i2c.c
 * @brief   Inter-Integrated Circuit (I2C) Driver Implementation
 * @details Production-ready, MISRA-friendly driver for STM32F407VG.
 * Implements register-level I2C control with correct ACK/STOP handling.
 */

#include "i2c.h"
#include "rcc.h"
#include <stddef.h>

/*=============================================================================
 * MACROS & DEFINES
 *===========================================================================*/

/** @brief Helper macro to compute bitmask from position */
#define I2C_BIT(bit)                    (1UL << (bit))

/** @brief Timeout value for polling operations */
#define I2C_TIMEOUT_VALUE               (1000000U)

/** @brief Bit position definitions to eliminate magic numbers */
#define I2C_CR1_PE_POS                  (0U)
#define I2C_CR1_START_POS               (8U)
#define I2C_CR1_STOP_POS                (9U)
#define I2C_CR1_ACK_POS                 (10U)

#define I2C_CR2_ITERREN_POS             (8U)
#define I2C_CR2_ITEVTEN_POS             (9U)
#define I2C_CR2_ITBUFEN_POS             (10U)

#define I2C_OAR1_BIT14_POS              (14U)

#define I2C_SR1_SB_POS                  (0U)
#define I2C_SR1_ADDR_POS                (1U)
#define I2C_SR1_BTF_POS                 (2U)
#define I2C_SR1_RXNE_POS                (6U)
#define I2C_SR1_TXE_POS                 (7U)

/** @brief Clock and Timing Configuration Defaults (16MHz APB1) */
#define I2C_CLOCK_FREQ_16MHZ            (16U)
#define I2C_CCR_STANDARD_MODE_16MHZ     (80U)
#define I2C_TRISE_STANDARD_MODE_16MHZ   (17U)
#define I2C_CCR_FAST_MODE_16MHZ         (27U)
#define I2C_TRISE_FAST_MODE_16MHZ       (9U)

/*=============================================================================
 * PRIVATE FUNCTION PROTOTYPES
 *===========================================================================*/

static I2C_Status_t I2C_WaitFlagInternal(volatile uint32_t *reg, uint32_t mask);
static void I2C_ClearAddrFlagInternal(const I2C_HandleTypeDef *const hi2c);
static I2C_Status_t I2C_HandleTimeout(I2C_HandleTypeDef *hi2c);

/*=============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 *===========================================================================*/

/**
 * @brief Internal helper to wait for a specific register flag
 */
static I2C_Status_t I2C_WaitFlagInternal(volatile uint32_t *reg, uint32_t mask)
{
    uint32_t timeout = I2C_TIMEOUT_VALUE;
    I2C_Status_t status = I2C_OK;

    while (((*reg) & mask) == 0U)
    {
        if (timeout == 0U)
        {
            status = I2C_TIMEOUT;
            break;
        }
        timeout--;
    }

    return status;
}

/**
 * @brief Internal helper to clear the ADDR flag by reading SR1 then SR2
 */
static void I2C_ClearAddrFlagInternal(const I2C_HandleTypeDef *const hi2c)
{
    volatile uint32_t tempReg;
    
    tempReg = hi2c->Instance->SR1;
    tempReg = hi2c->Instance->SR2;
    (void)tempReg;
}

/**
 * @brief Internal helper to centralize timeout cleanup
 */
static I2C_Status_t I2C_HandleTimeout(I2C_HandleTypeDef *hi2c)
{
    if (hi2c != NULL)
    {
        hi2c->ErrorCode = I2C_ERROR_TIMEOUT;
        hi2c->Busy = 0U;
    }
    return I2C_TIMEOUT;
}

/*=============================================================================
 * PUBLIC API IMPLEMENTATIONS
 *===========================================================================*/

/**
 * @brief Initializes the I2C peripheral
 *
 * @param hi2c Pointer to I2C peripheral configuration handle
 */
I2C_Status_t I2C_Init(I2C_HandleTypeDef *hi2c)
{
    if (hi2c == NULL)
    {
        return I2C_NULL_POINTER;
    }

    RCC_EnableI2CClock(hi2c->Instance);

    hi2c->Busy = 0U;
    hi2c->ErrorCode = I2C_ERROR_NONE;
    hi2c->TransferCompleteCallback = NULL;

    /* Disable peripheral */
    hi2c->Instance->CR1 &= ~I2C_BIT(I2C_CR1_PE_POS);

    /* Peripheral clock configuration */
    hi2c->Instance->CR2 = I2C_CLOCK_FREQ_16MHZ;

    /* Set own address (Bit 14 must be kept at 1 per STM32 RM) */
    hi2c->Instance->OAR1 = I2C_BIT(I2C_OAR1_BIT14_POS) | (uint32_t)(hi2c->Init.ownAddress << 1U);

    /* Clock speed configuration */
    if (hi2c->Init.clockSpeed == I2C_SPEED_STANDARD)
    {
        hi2c->Instance->CCR = I2C_CCR_STANDARD_MODE_16MHZ;
        hi2c->Instance->TRISE = I2C_TRISE_STANDARD_MODE_16MHZ;
    }
    else
    {
        hi2c->Instance->CCR = I2C_CCR_FAST_MODE_16MHZ;
        hi2c->Instance->TRISE = I2C_TRISE_FAST_MODE_16MHZ;
    }

    /* Enable peripheral */
    hi2c->Instance->CR1 |= I2C_BIT(I2C_CR1_PE_POS);

    return I2C_OK;
}

/**
 * @brief De-initializes the I2C peripheral
 *
 * @param hi2c Pointer to I2C peripheral configuration handle
 */
I2C_Status_t I2C_DeInit(I2C_HandleTypeDef *hi2c)
{
    if (hi2c == NULL)
    {
        return I2C_NULL_POINTER;
    }

    RCC_ResetI2C(hi2c->Instance);

    return I2C_OK;
}

/**
 * @brief Transmits data as Master in blocking mode
 *
 * @param hi2c Pointer to I2C peripheral configuration handle
 * @param address Target device address
 * @param data Pointer to data buffer
 * @param size Amount of data to be sent
 */
I2C_Status_t I2C_MasterTransmit(I2C_HandleTypeDef *hi2c, uint16_t address, uint8_t *data, uint16_t size)
{
    if ((hi2c == NULL) || (data == NULL))
    {
        return I2C_NULL_POINTER;
    }

    if (hi2c->Busy != 0U)
    {
        return I2C_ERROR;
    }

    hi2c->Busy = 1U;
    hi2c->ErrorCode = I2C_ERROR_NONE;

    /* Generate START condition */
    hi2c->Instance->CR1 |= I2C_BIT(I2C_CR1_START_POS);

    if (I2C_WaitFlagInternal(&(hi2c->Instance->SR1), I2C_BIT(I2C_SR1_SB_POS)) != I2C_OK)
    {
        return I2C_HandleTimeout(hi2c);
    }

    /* Send Address with Write intention (LSB = 0) */
    hi2c->Instance->DR = (uint32_t)(address << 1U);

    if (I2C_WaitFlagInternal(&(hi2c->Instance->SR1), I2C_BIT(I2C_SR1_ADDR_POS)) != I2C_OK)
    {
        return I2C_HandleTimeout(hi2c);
    }

    /* Clear ADDR flag */
    I2C_ClearAddrFlagInternal(hi2c);

    /* Transmit Data */
    while (size > 0U)
    {
        if (I2C_WaitFlagInternal(&(hi2c->Instance->SR1), I2C_BIT(I2C_SR1_TXE_POS)) != I2C_OK)
        {
            return I2C_HandleTimeout(hi2c);
        }

        hi2c->Instance->DR = (uint32_t)(*data);
        data++;
        size--;
    }

    /* Wait for Byte Transfer Finished */
    if (I2C_WaitFlagInternal(&(hi2c->Instance->SR1), I2C_BIT(I2C_SR1_BTF_POS)) != I2C_OK)
    {
        return I2C_HandleTimeout(hi2c);
    }

    /* Generate STOP condition */
    hi2c->Instance->CR1 |= I2C_BIT(I2C_CR1_STOP_POS);

    hi2c->Busy = 0U;

    return I2C_OK;
}

/**
 * @brief Receives data as Master in blocking mode
 *
 * @param hi2c Pointer to I2C peripheral configuration handle
 * @param address Target device address
 * @param data Pointer to data buffer
 * @param size Amount of data to be received
 */
I2C_Status_t I2C_MasterReceive(I2C_HandleTypeDef *hi2c, uint16_t address, uint8_t *data, uint16_t size)
{
    if ((hi2c == NULL) || (data == NULL) || (size == 0U))
    {
        return I2C_NULL_POINTER;
    }

    if (hi2c->Busy != 0U)
    {
        return I2C_ERROR;
    }

    hi2c->Busy = 1U;
    hi2c->ErrorCode = I2C_ERROR_NONE;

    /* Enable Acknowledgment */
    hi2c->Instance->CR1 |= I2C_BIT(I2C_CR1_ACK_POS);

    /* Generate START condition */
    hi2c->Instance->CR1 |= I2C_BIT(I2C_CR1_START_POS);

    if (I2C_WaitFlagInternal(&(hi2c->Instance->SR1), I2C_BIT(I2C_SR1_SB_POS)) != I2C_OK)
    {
        return I2C_HandleTimeout(hi2c);
    }

    /* Send Address with Read intention (LSB = 1) */
    hi2c->Instance->DR = (uint32_t)((address << 1U) | 1U);

    if (I2C_WaitFlagInternal(&(hi2c->Instance->SR1), I2C_BIT(I2C_SR1_ADDR_POS)) != I2C_OK)
    {
        return I2C_HandleTimeout(hi2c);
    }

    /* Handle specific STM32 receive sequence based on size */
    if (size == 1U)
    {
        /* Disable ACK before clearing ADDR */
        hi2c->Instance->CR1 &= ~I2C_BIT(I2C_CR1_ACK_POS);
        I2C_ClearAddrFlagInternal(hi2c);
        
        /* Generate STOP */
        hi2c->Instance->CR1 |= I2C_BIT(I2C_CR1_STOP_POS);

        /* Wait for RXNE */
        if (I2C_WaitFlagInternal(&(hi2c->Instance->SR1), I2C_BIT(I2C_SR1_RXNE_POS)) != I2C_OK)
        {
            return I2C_HandleTimeout(hi2c);
        }
        
        *data = (uint8_t)(hi2c->Instance->DR);
    }
    else
    {
        I2C_ClearAddrFlagInternal(hi2c);

        while (size > 2U)
        {
            /* Wait for RXNE */
            if (I2C_WaitFlagInternal(&(hi2c->Instance->SR1), I2C_BIT(I2C_SR1_RXNE_POS)) != I2C_OK)
            {
                return I2C_HandleTimeout(hi2c);
            }

            *data = (uint8_t)(hi2c->Instance->DR);
            data++;
            size--;
        }

        /* Wait for BTF indicating both N-1 and N bytes are present */
        if (I2C_WaitFlagInternal(&(hi2c->Instance->SR1), I2C_BIT(I2C_SR1_BTF_POS)) != I2C_OK)
        {
            return I2C_HandleTimeout(hi2c);
        }

        /* Disable ACK and Generate STOP */
        hi2c->Instance->CR1 &= ~I2C_BIT(I2C_CR1_ACK_POS);
        hi2c->Instance->CR1 |= I2C_BIT(I2C_CR1_STOP_POS);

        /* Read N-1 Byte */
        *data = (uint8_t)(hi2c->Instance->DR);
        data++;
        
        /* Read N Byte */
        *data = (uint8_t)(hi2c->Instance->DR);
    }

    hi2c->Busy = 0U;

    return I2C_OK;
}

/**
 * @brief Enables I2C interrupts (Event, Error, Buffer)
 *
 * @param hi2c Pointer to I2C peripheral configuration handle
 */
I2C_Status_t I2C_EnableInterrupt(I2C_HandleTypeDef *hi2c)
{
    if (hi2c == NULL)
    {
        return I2C_NULL_POINTER;
    }

    hi2c->Instance->CR2 |= (I2C_BIT(I2C_CR2_ITEVTEN_POS) |
                            I2C_BIT(I2C_CR2_ITERREN_POS) |
                            I2C_BIT(I2C_CR2_ITBUFEN_POS));

    return I2C_OK;
}

/**
 * @brief Disables I2C interrupts
 *
 * @param hi2c Pointer to I2C peripheral configuration handle
 */
I2C_Status_t I2C_DisableInterrupt(I2C_HandleTypeDef *hi2c)
{
    if (hi2c == NULL)
    {
        return I2C_NULL_POINTER;
    }

    hi2c->Instance->CR2 &= ~(I2C_BIT(I2C_CR2_ITEVTEN_POS) |
                             I2C_BIT(I2C_CR2_ITERREN_POS) |
                             I2C_BIT(I2C_CR2_ITBUFEN_POS));

    return I2C_OK;
}

/**
 * @brief Registers a callback function for transfer completion
 *
 * @param hi2c Pointer to I2C peripheral configuration handle
 * @param callback Function pointer to the callback
 */
void I2C_RegisterCallback(I2C_HandleTypeDef *hi2c, void (*callback)(void))
{
    if (hi2c != NULL)
    {
        hi2c->TransferCompleteCallback = callback;
    }
}

/**
 * @brief I2C Interrupt Handler
 *
 * @param hi2c Pointer to I2C peripheral configuration handle
 */
void I2C_IRQHandler(I2C_HandleTypeDef *hi2c)
{
    if ((hi2c != NULL) && (hi2c->TransferCompleteCallback != NULL))
    {
        hi2c->TransferCompleteCallback();
    }
}

/**
 * @brief Gets the current I2C error code
 *
 * @param hi2c Pointer to I2C peripheral configuration handle
 */
I2C_Error_t I2C_GetError(I2C_HandleTypeDef *hi2c)
{
    I2C_Error_t errorState = I2C_ERROR_NONE;

    if (hi2c != NULL)
    {
        errorState = (I2C_Error_t)hi2c->ErrorCode;
    }

    return errorState;
}

/**
 * @brief Clears the current I2C error code
 *
 * @param hi2c Pointer to I2C peripheral configuration handle
 */
void I2C_ClearError(I2C_HandleTypeDef *hi2c)
{
    if (hi2c != NULL)
    {
        hi2c->ErrorCode = I2C_ERROR_NONE;
    }
}

/**
 * @brief Transmits data as Master via DMA (Stub)
 *
 * @param hi2c Pointer to I2C peripheral configuration handle
 * @param address Target device address
 * @param data Pointer to data buffer
 * @param size Amount of data to be sent
 */
I2C_Status_t I2C_MasterTransmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t address, uint8_t *data, uint16_t size)
{
    (void)hi2c;
    (void)address;
    (void)data;
    (void)size;

    return I2C_NOT_SUPPORTED;
}

/**
 * @brief Receives data as Master via DMA (Stub)
 *
 * @param hi2c Pointer to I2C peripheral configuration handle
 * @param address Target device address
 * @param data Pointer to data buffer
 * @param size Amount of data to be received
 */
I2C_Status_t I2C_MasterReceive_DMA(I2C_HandleTypeDef *hi2c, uint16_t address, uint8_t *data, uint16_t size)
{
    (void)hi2c;
    (void)address;
    (void)data;
    (void)size;

    return I2C_NOT_SUPPORTED;
}