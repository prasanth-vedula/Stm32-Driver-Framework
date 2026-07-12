/**
 * @file    timer.c
 * @brief   Hardware Timer Driver Implementation
 * @details Production-ready, MISRA-friendly implementation for STM32F407VG.
 * Handles register-level Timer configurations, interrupts, and operations.
 */

#include "timer.h"

/*=============================================================================
 * MACROS & DEFINES
 *===========================================================================*/

/** @brief Helper macro to compute bitmask from position */
#define TIMER_BIT(bit)                  (1UL << (bit))

/** @brief Bit position definitions to eliminate magic numbers */
#define TIMER_CR1_CEN_POS               (0U)
#define TIMER_CR1_DIR_POS               (4U)
#define TIMER_CR1_CKD_POS               (8U)

#define TIMER_DIER_UIE_POS              (0U)

#define TIMER_SR_UIF_POS                (0U)

#define TIMER_EGR_UG_POS                (0U)

/*=============================================================================
 * PUBLIC API IMPLEMENTATIONS
 *===========================================================================*/

/*------------------------- Initialization ----------------------------------*/

/**
 * @brief Initializes the Timer peripheral according to the specified parameters in htim
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return TIMER_Status_t Status of the initialization
 */
TIMER_Status_t TIMER_Init(TIMER_HandleTypeDef *htim)
{
    if (htim == NULL)
    {
        return TIMER_NULL_POINTER;
    }

    /* Reset Control Register 1 */
    htim->Instance->CR1 = 0U;

    /* Set Prescaler and Auto-Reload values */
    htim->Instance->PSC = htim->Init.Prescaler;
    htim->Instance->ARR = htim->Init.Period;

    /* Configure Counter Mode */
    if (htim->Init.CounterMode == TIMER_COUNTER_DOWN)
    {
        htim->Instance->CR1 |= TIMER_BIT(TIMER_CR1_DIR_POS);
    }

    /* Configure Clock Division */
    htim->Instance->CR1 |= ((uint32_t)htim->Init.ClockDivision << TIMER_CR1_CKD_POS);

    /* Generate an update event to reload the Prescaler and the Repetition counter immediately */
    htim->Instance->EGR |= TIMER_BIT(TIMER_EGR_UG_POS);

    /* Initialize state flags */
    htim->Running = false;
    htim->ErrorCode = TIMER_OK;
    htim->PeriodElapsedCallback = NULL;

    return TIMER_OK;
}

/**
 * @brief De-initializes the Timer peripheral
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return TIMER_Status_t Status of the de-initialization
 */
TIMER_Status_t TIMER_DeInit(TIMER_HandleTypeDef *htim)
{
    if (htim == NULL)
    {
        return TIMER_NULL_POINTER;
    }

    /* Reset all relevant Timer registers */
    htim->Instance->CR1  = 0U;
    htim->Instance->CR2  = 0U;
    htim->Instance->SMCR = 0U;
    htim->Instance->DIER = 0U;
    htim->Instance->SR   = 0U;
    htim->Instance->PSC  = 0U;
    htim->Instance->ARR  = 0U;
    htim->Instance->CNT  = 0U;

    /* Reset state flags */
    htim->Running = false;
    htim->ErrorCode = TIMER_OK;

    return TIMER_OK;
}

/*------------------------- Counter Control ---------------------------------*/

/**
 * @brief Starts the Timer counter
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return TIMER_Status_t Status of the start operation
 */
TIMER_Status_t TIMER_Start(TIMER_HandleTypeDef *htim)
{
    if (htim == NULL)
    {
        return TIMER_NULL_POINTER;
    }

    /* Enable Counter */
    htim->Instance->CR1 |= TIMER_BIT(TIMER_CR1_CEN_POS);
    htim->Running = true;

    return TIMER_OK;
}

/**
 * @brief Stops the Timer counter
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return TIMER_Status_t Status of the stop operation
 */
TIMER_Status_t TIMER_Stop(TIMER_HandleTypeDef *htim)
{
    if (htim == NULL)
    {
        return TIMER_NULL_POINTER;
    }

    /* Disable Counter */
    htim->Instance->CR1 &= ~TIMER_BIT(TIMER_CR1_CEN_POS);
    htim->Running = false;

    return TIMER_OK;
}

/**
 * @brief Resets the Timer peripheral to default state
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return TIMER_Status_t Status of the reset operation
 */
TIMER_Status_t TIMER_Reset(TIMER_HandleTypeDef *htim)
{
    if (htim == NULL)
    {
        return TIMER_NULL_POINTER;
    }

    /* Generate an update event to reset the counter and reload prescaler */
    htim->Instance->EGR |= TIMER_BIT(TIMER_EGR_UG_POS);
    
    return TIMER_OK;
}

/*------------------------- Interrupts & Callbacks --------------------------*/

/**
 * @brief Enables the Timer Update (Period Elapsed) Interrupt
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return TIMER_Status_t Status of the interrupt enable operation
 */
TIMER_Status_t TIMER_EnableInterrupt(TIMER_HandleTypeDef *htim)
{
    if (htim == NULL)
    {
        return TIMER_NULL_POINTER;
    }

    /* Enable Update Interrupt */
    htim->Instance->DIER |= TIMER_BIT(TIMER_DIER_UIE_POS);

    return TIMER_OK;
}

/**
 * @brief Disables the Timer Update (Period Elapsed) Interrupt
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return TIMER_Status_t Status of the interrupt disable operation
 */
TIMER_Status_t TIMER_DisableInterrupt(TIMER_HandleTypeDef *htim)
{
    if (htim == NULL)
    {
        return TIMER_NULL_POINTER;
    }

    /* Disable Update Interrupt */
    htim->Instance->DIER &= ~TIMER_BIT(TIMER_DIER_UIE_POS);

    return TIMER_OK;
}

/**
 * @brief Registers a user callback for the Period Elapsed event
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @param callback Function pointer to the user callback
 */
void TIMER_RegisterCallback(TIMER_HandleTypeDef *htim, TIMER_Callback_t callback)
{
    if (htim != NULL)
    {
        htim->PeriodElapsedCallback = callback;
    }
}

/**
 * @brief Timer Interrupt Request Handler
 * @details Should be called from the respective TIMx_IRQHandler in the application layer
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 */
void TIMER_IRQHandler(TIMER_HandleTypeDef *htim)
{
    if (htim == NULL)
    {
        return;
    }

    /* Check if the Update Interrupt Flag is set */
    if ((htim->Instance->SR & TIMER_BIT(TIMER_SR_UIF_POS)) != 0U)
    {
        /* Clear the Update Interrupt Flag */
        htim->Instance->SR &= ~TIMER_BIT(TIMER_SR_UIF_POS);

        /* Execute the registered callback if available */
        if (htim->PeriodElapsedCallback != NULL)
        {
            htim->PeriodElapsedCallback();
        }
    }
}

/*------------------------- Configuration APIs ------------------------------*/

/**
 * @brief Retrieves the current Timer counter value
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return uint32_t Current counter value (CNT register)
 */
uint32_t TIMER_GetCounter(TIMER_HandleTypeDef *htim)
{
    uint32_t counterValue = 0U;

    if (htim != NULL)
    {
        counterValue = htim->Instance->CNT;
    }

    return counterValue;
}

/**
 * @brief Sets the Timer counter to a specific value
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @param value The value to set the counter to
 */
void TIMER_SetCounter(TIMER_HandleTypeDef *htim, uint32_t value)
{
    if (htim != NULL)
    {
        htim->Instance->CNT = value;
    }
}

/**
 * @brief Dynamically updates the Timer prescaler value
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @param prescaler The new prescaler value
 */
void TIMER_SetPrescaler(TIMER_HandleTypeDef *htim, uint32_t prescaler)
{
    if (htim != NULL)
    {
        htim->Instance->PSC = prescaler;
        htim->Init.Prescaler = prescaler;
    }
}

/**
 * @brief Dynamically updates the Timer auto-reload period value
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @param period The new period value
 */
void TIMER_SetPeriod(TIMER_HandleTypeDef *htim, uint32_t period)
{
    if (htim != NULL)
    {
        htim->Instance->ARR = period;
        htim->Init.Period = period;
    }
}

/*------------------------- Delay & Status ----------------------------------*/

/**
 * @brief Implements a blocking delay based on the Timer counter
 * @note The Timer must be configured and running for this function to operate
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @param ticks Number of timer ticks to wait
 * @return TIMER_Status_t Status of the delay operation
 */
TIMER_Status_t TIMER_Delay(TIMER_HandleTypeDef *htim, uint32_t ticks)
{
    if (htim == NULL)
    {
        return TIMER_NULL_POINTER;
    }

    /* Reset the counter to 0 */
    TIMER_SetCounter(htim, 0U);

    /* Poll the counter until the specified ticks have elapsed */
    while (TIMER_GetCounter(htim) < ticks)
    {
        /* Wait */
    }

    return TIMER_OK;
}

/**
 * @brief Checks if the Timer is currently running
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return bool true if running, false if stopped
 */
bool TIMER_IsRunning(TIMER_HandleTypeDef *htim)
{
    bool isRunning = false;

    if (htim != NULL)
    {
        isRunning = htim->Running;
    }

    return isRunning;
}

/**
 * @brief Retrieves the current operating status and error state of the Timer
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return TIMER_Status_t Current operational status
 */
TIMER_Status_t TIMER_GetStatus(TIMER_HandleTypeDef *htim)
{
    TIMER_Status_t status = TIMER_ERROR;

    if (htim != NULL)
    {
        status = htim->ErrorCode;
    }

    return status;
}