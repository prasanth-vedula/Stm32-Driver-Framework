/**
 * @file    spi.c
 * @brief   Serial Peripheral Interface (SPI) Driver Implementation
 * @details Production-ready, MISRA-friendly driver for STM32F407VG.
 * Implements register-level SPI configurations, polling, and interrupts.
 */

#include "spi.h"
#include "rcc.h"
#include <stddef.h>

/*=============================================================================
 * MACROS & DEFINES
 *===========================================================================*/

/** @brief Helper macro to compute bitmask from position */
#define SPI_BIT(bit)                    (1UL << (bit))

/** @brief Timeout value for polling operations */
#define SPI_TIMEOUT_VALUE               (1000000U)

/** @brief Bit position definitions to eliminate magic numbers */
#define SPI_CR1_CPHA_POS                (0U)
#define SPI_CR1_CPOL_POS                (1U)
#define SPI_CR1_MSTR_POS                (2U)
#define SPI_CR1_SPE_POS                 (6U)
#define SPI_CR1_LSBFIRST_POS            (7U)
#define SPI_CR1_SSI_POS                 (8U)
#define SPI_CR1_SSM_POS                 (9U)
#define SPI_CR1_DFF_POS                 (11U)

#define SPI_CR2_RXNEIE_POS              (6U)
#define SPI_CR2_TXEIE_POS               (7U)

#define SPI_SR_RXNE_POS                 (0U)
#define SPI_SR_TXE_POS                  (1U)
#define SPI_SR_OVR_POS                  (6U)
#define SPI_SR_BSY_POS                  (7U)

/*=============================================================================
 * PRIVATE FUNCTION PROTOTYPES
 *===========================================================================*/

static SPI_Status_t SPI_WaitFlagInternal(volatile uint32_t *reg, uint32_t mask);
static SPI_Status_t SPI_WaitFlagClearInternal(volatile uint32_t *reg, uint32_t mask);
static SPI_Status_t SPI_HandleTimeout(SPI_HandleTypeDef *hspi);

/*=============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 *===========================================================================*/

/**
 * @brief Internal helper to wait for a specific register flag to be SET
 */
static SPI_Status_t SPI_WaitFlagInternal(volatile uint32_t *reg, uint32_t mask)
{
    uint32_t timeout = SPI_TIMEOUT_VALUE;
    SPI_Status_t status = SPI_OK;

    while (((*reg) & mask) == 0U)
    {
        if (timeout == 0U)
        {
            status = SPI_TIMEOUT;
            break;
        }
        timeout--;
    }

    return status;
}

/**
 * @brief Internal helper to wait for a specific register flag to be CLEARED
 */
static SPI_Status_t SPI_WaitFlagClearInternal(volatile uint32_t *reg, uint32_t mask)
{
    uint32_t timeout = SPI_TIMEOUT_VALUE;
    SPI_Status_t status = SPI_OK;

    while (((*reg) & mask) != 0U)
    {
        if (timeout == 0U)
        {
            status = SPI_TIMEOUT;
            break;
        }
        timeout--;
    }

    return status;
}

/**
 * @brief Internal helper to centralize timeout cleanup
 */
static SPI_Status_t SPI_HandleTimeout(SPI_HandleTypeDef *hspi)
{
    if (hspi != NULL)
    {
        hspi->ErrorCode = SPI_ERROR_TIMEOUT;
        hspi->Busy = 0U;
    }
    return SPI_TIMEOUT;
}

/*=============================================================================
 * PUBLIC API IMPLEMENTATIONS
 *===========================================================================*/

/**
 * @brief Initializes the SPI peripheral
 *
 * @param hspi Pointer to SPI peripheral configuration handle
 */
SPI_Status_t SPI_Init(SPI_HandleTypeDef *hspi)
{
    if (hspi == NULL)
    {
        return SPI_NULL_POINTER;
    }

    RCC_EnableSPIClock(hspi->Instance);

    /* Disable SPI before configuration */
    hspi->Instance->CR1 &= ~SPI_BIT(SPI_CR1_SPE_POS);

    /* Configure Master / Slave mode */
    if (hspi->Init.mode == SPI_MODE_MASTER)
    {
        hspi->Instance->CR1 |= SPI_BIT(SPI_CR1_MSTR_POS);
    }
    else
    {
        hspi->Instance->CR1 &= ~SPI_BIT(SPI_CR1_MSTR_POS);
    }

    /* Configure Clock Polarity */
    if (hspi->Init.cpol == SPI_CPOL_HIGH)
    {
        hspi->Instance->CR1 |= SPI_BIT(SPI_CR1_CPOL_POS);
    }
    else
    {
        hspi->Instance->CR1 &= ~SPI_BIT(SPI_CR1_CPOL_POS);
    }

    /* Configure Clock Phase */
    if (hspi->Init.cpha == SPI_CPHA_SECOND_EDGE)
    {
        hspi->Instance->CR1 |= SPI_BIT(SPI_CR1_CPHA_POS);
    }
    else
    {
        hspi->Instance->CR1 &= ~SPI_BIT(SPI_CR1_CPHA_POS);
    }

    /* Configure Data Size */
    if (hspi->Init.dataSize == SPI_DATASIZE_16BIT)
    {
        hspi->Instance->CR1 |= SPI_BIT(SPI_CR1_DFF_POS);
    }
    else
    {
        hspi->Instance->CR1 &= ~SPI_BIT(SPI_CR1_DFF_POS);
    }

    /* Configure Frame Format (LSB/MSB First) */
    if (hspi->Init.firstBit == SPI_LSB_FIRST)
    {
        hspi->Instance->CR1 |= SPI_BIT(SPI_CR1_LSBFIRST_POS);
    }
    else
    {
        hspi->Instance->CR1 &= ~SPI_BIT(SPI_CR1_LSBFIRST_POS);
    }

    /* Enable Software Slave Management */
    hspi->Instance->CR1 |= SPI_BIT(SPI_CR1_SSM_POS);
    hspi->Instance->CR1 |= SPI_BIT(SPI_CR1_SSI_POS);

    /* Initialize State structure variables */
    hspi->Busy = 0U;
    hspi->ErrorCode = SPI_ERROR_NONE;
    hspi->TxRxCompleteCallback = NULL;

    /* Enable SPI */
    hspi->Instance->CR1 |= SPI_BIT(SPI_CR1_SPE_POS);

    return SPI_OK;
}

/**
 * @brief De-initializes the SPI peripheral
 *
 * @param hspi Pointer to SPI peripheral configuration handle
 */
SPI_Status_t SPI_DeInit(SPI_HandleTypeDef *hspi)
{
    if (hspi == NULL)
    {
        return SPI_NULL_POINTER;
    }

    RCC_ResetSPI(hspi->Instance);

    /* Clear state variables */
    hspi->Busy = 0U;
    hspi->ErrorCode = SPI_ERROR_NONE;
    hspi->TxRxCompleteCallback = NULL;

    return SPI_OK;
}

/**
 * @brief Transmits data via SPI in blocking mode
 *
 * @param hspi Pointer to SPI peripheral configuration handle
 * @param txData Pointer to data buffer to be transmitted
 * @param size Amount of data to be sent
 */
SPI_Status_t SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *txData, uint16_t size)
{
    if ((hspi == NULL) || (txData == NULL))
    {
        return SPI_NULL_POINTER;
    }

    if (hspi->Busy != 0U)
    {
        return SPI_ERROR;
    }

    hspi->Busy = 1U;
    hspi->ErrorCode = SPI_ERROR_NONE;

    while (size > 0U)
    {
        /* Wait for TXE (Transmit Buffer Empty) */
        if (SPI_WaitFlagInternal(&(hspi->Instance->SR), SPI_BIT(SPI_SR_TXE_POS)) != SPI_OK)
        {
            return SPI_HandleTimeout(hspi);
        }

        /* Write data to Data Register */
        *((volatile uint8_t*)&(hspi->Instance->DR)) = *txData;
        
        txData++;
        size--;
    }

    /* Wait for Busy flag to clear (SPI completely idle) */
    if (SPI_WaitFlagClearInternal(&(hspi->Instance->SR), SPI_BIT(SPI_SR_BSY_POS)) != SPI_OK)
    {
        return SPI_HandleTimeout(hspi);
    }

    hspi->Busy = 0U;

    return SPI_OK;
}

/**
 * @brief Receives data via SPI in blocking mode
 *
 * @param hspi Pointer to SPI peripheral configuration handle
 * @param rxData Pointer to buffer to store received data
 * @param size Amount of data to be received
 */
SPI_Status_t SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *rxData, uint16_t size)
{
    if ((hspi == NULL) || (rxData == NULL))
    {
        return SPI_NULL_POINTER;
    }

    if (hspi->Busy != 0U)
    {
        return SPI_ERROR;
    }

    hspi->Busy = 1U;
    hspi->ErrorCode = SPI_ERROR_NONE;

    while (size > 0U)
    {
        /* Generate clock by transmitting dummy byte (Wait for TXE first) */
        if (SPI_WaitFlagInternal(&(hspi->Instance->SR), SPI_BIT(SPI_SR_TXE_POS)) != SPI_OK)
        {
            return SPI_HandleTimeout(hspi);
        }
        
        *((volatile uint8_t*)&(hspi->Instance->DR)) = 0xFFU; /* Dummy byte */

        /* Wait for RXNE (Receive Buffer Not Empty) */
        if (SPI_WaitFlagInternal(&(hspi->Instance->SR), SPI_BIT(SPI_SR_RXNE_POS)) != SPI_OK)
        {
            return SPI_HandleTimeout(hspi);
        }

        /* Read data from Data Register */
        *rxData = *((volatile uint8_t*)&(hspi->Instance->DR));
        
        rxData++;
        size--;
    }

    /* Wait for Busy flag to clear */
    if (SPI_WaitFlagClearInternal(&(hspi->Instance->SR), SPI_BIT(SPI_SR_BSY_POS)) != SPI_OK)
    {
        return SPI_HandleTimeout(hspi);
    }

    hspi->Busy = 0U;

    return SPI_OK;
}

/**
 * @brief Transmits and receives data via SPI simultaneously in blocking mode
 *
 * @param hspi Pointer to SPI peripheral configuration handle
 * @param txData Pointer to data buffer to be transmitted
 * @param rxData Pointer to buffer to store received data
 * @param size Amount of data to be processed
 */
SPI_Status_t SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *txData, uint8_t *rxData, uint16_t size)
{
    if ((hspi == NULL) || (txData == NULL) || (rxData == NULL))
    {
        return SPI_NULL_POINTER;
    }

    if (hspi->Busy != 0U)
    {
        return SPI_ERROR;
    }

    hspi->Busy = 1U;
    hspi->ErrorCode = SPI_ERROR_NONE;

    while (size > 0U)
    {
        /* Wait for TXE (Transmit Buffer Empty) */
        if (SPI_WaitFlagInternal(&(hspi->Instance->SR), SPI_BIT(SPI_SR_TXE_POS)) != SPI_OK)
        {
            return SPI_HandleTimeout(hspi);
        }

        /* Write data to Data Register */
        *((volatile uint8_t*)&(hspi->Instance->DR)) = *txData;

        /* Wait for RXNE (Receive Buffer Not Empty) */
        if (SPI_WaitFlagInternal(&(hspi->Instance->SR), SPI_BIT(SPI_SR_RXNE_POS)) != SPI_OK)
        {
            return SPI_HandleTimeout(hspi);
        }

        /* Read data from Data Register */
        *rxData = *((volatile uint8_t*)&(hspi->Instance->DR));

        txData++;
        rxData++;
        size--;
    }

    /* Wait for Busy flag to clear */
    if (SPI_WaitFlagClearInternal(&(hspi->Instance->SR), SPI_BIT(SPI_SR_BSY_POS)) != SPI_OK)
    {
        return SPI_HandleTimeout(hspi);
    }

    hspi->Busy = 0U;

    return SPI_OK;
}

/**
 * @brief Enables SPI Interrupts (RXNE and TXE)
 *
 * @param hspi Pointer to SPI peripheral configuration handle
 */
SPI_Status_t SPI_EnableInterrupt(SPI_HandleTypeDef *hspi)
{
    if (hspi == NULL)
    {
        return SPI_NULL_POINTER;
    }

    /* Enable both TXE and RXNE interrupts */
    hspi->Instance->CR2 |= (SPI_BIT(SPI_CR2_TXEIE_POS) | SPI_BIT(SPI_CR2_RXNEIE_POS));

    return SPI_OK;
}

/**
 * @brief Disables SPI Interrupts
 *
 * @param hspi Pointer to SPI peripheral configuration handle
 */
SPI_Status_t SPI_DisableInterrupt(SPI_HandleTypeDef *hspi)
{
    if (hspi == NULL)
    {
        return SPI_NULL_POINTER;
    }

    /* Disable both TXE and RXNE interrupts */
    hspi->Instance->CR2 &= ~(SPI_BIT(SPI_CR2_TXEIE_POS) | SPI_BIT(SPI_CR2_RXNEIE_POS));

    return SPI_OK;
}

/**
 * @brief Registers a user callback for SPI interrupt events
 *
 * @param hspi Pointer to SPI peripheral configuration handle
 * @param callback Function pointer to the user callback
 */
void SPI_RegisterCallback(SPI_HandleTypeDef *hspi, void (*callback)(void))
{
    if (hspi != NULL)
    {
        hspi->TxRxCompleteCallback = callback;
    }
}

/**
 * @brief SPI Interrupt Request Handler
 * @details Should be called from the respective SPIx_IRQHandler
 *
 * @param hspi Pointer to SPI peripheral configuration handle
 */
void SPI_IRQHandler(SPI_HandleTypeDef *hspi)
{
    if (hspi == NULL)
    {
        return;
    }

    /* Handle Receive Buffer Not Empty Interrupt */
    if ((hspi->Instance->SR & SPI_BIT(SPI_SR_RXNE_POS)) != 0U)
    {
        volatile uint8_t dummy;

        /* Reading DR clears the RXNE flag */
        dummy = *((volatile uint8_t*)&(hspi->Instance->DR));
        (void)dummy;

        if (hspi->TxRxCompleteCallback != NULL)
        {
            hspi->TxRxCompleteCallback();
        }
    }

    /* Handle Transmit Buffer Empty Interrupt (Future placeholder) */
    if ((hspi->Instance->SR & SPI_BIT(SPI_SR_TXE_POS)) != 0U)
    {
        /* Note: TXE interrupt handling logic can be expanded here */
    }

    /* Handle Overrun Error Interrupt (Future placeholder) */
    if ((hspi->Instance->SR & SPI_BIT(SPI_SR_OVR_POS)) != 0U)
    {
        volatile uint32_t dummy;
        
        /* Clear OVR flag by reading DR then SR */
        dummy = hspi->Instance->DR;
        dummy = hspi->Instance->SR;
        (void)dummy;
    }
}

/**
 * @brief Retrieves the current SPI error code
 *
 * @param hspi Pointer to SPI peripheral configuration handle
 */
SPI_Error_t SPI_GetError(SPI_HandleTypeDef *hspi)
{
    SPI_Error_t errorState = SPI_ERROR_NONE;

    if (hspi != NULL)
    {
        errorState = (SPI_Error_t)hspi->ErrorCode;
    }

    return errorState;
}

/**
 * @brief Clears the current SPI error code
 *
 * @param hspi Pointer to SPI peripheral configuration handle
 */
void SPI_ClearError(SPI_HandleTypeDef *hspi)
{
    if (hspi != NULL)
    {
        hspi->ErrorCode = SPI_ERROR_NONE;
    }
}

/**
 * @brief Transmits data as Master via DMA (Stub)
 *
 * @param hspi Pointer to SPI peripheral configuration handle
 * @param txData Pointer to data buffer
 * @param size Amount of data to be sent
 */
SPI_Status_t SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, uint8_t *txData, uint16_t size)
{
    (void)hspi;
    (void)txData;
    (void)size;

    return SPI_ERROR;
}

/**
 * @brief Receives data as Master via DMA (Stub)
 *
 * @param hspi Pointer to SPI peripheral configuration handle
 * @param rxData Pointer to data buffer
 * @param size Amount of data to be received
 */
SPI_Status_t SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *rxData, uint16_t size)
{
    (void)hspi;
    (void)rxData;
    (void)size;

    return SPI_ERROR;
}

/**
 * @brief Transmits and receives data as Master via DMA (Stub)
 *
 * @param hspi Pointer to SPI peripheral configuration handle
 * @param txData Pointer to data buffer to be transmitted
 * @param rxData Pointer to buffer to store received data
 * @param size Amount of data to be processed
 */
SPI_Status_t SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi, uint8_t *txData, uint8_t *rxData, uint16_t size)
{
    (void)hspi;
    (void)txData;
    (void)rxData;
    (void)size;

    return SPI_ERROR;
}