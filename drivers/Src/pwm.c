/**
 * @file    pwm.c
 * @brief   Pulse Width Modulation (PWM) Driver Implementation
 * @details Production-ready, MISRA-friendly driver for STM32F407VG.
 * Implements register-level PWM configurations, channel control, and interrupts.
 */

#include "pwm.h"
#include "rcc.h"
#include "bit_utils.h"
#include <stddef.h>

/*=============================================================================
 * MACROS & DEFINES
 *===========================================================================*/
#define PWM_BIT(bit)            (1UL << (bit))
#define PWM_CCER_CC1E_POS       (0U)
#define PWM_CCMR1_OC1M_PWM1     (0x06U << 4U)
#define PWM_DIER_CC1IE_POS      (1U)

/*=============================================================================
 * PUBLIC API IMPLEMENTATIONS
 *===========================================================================*/

/**
 * @brief Initializes the PWM peripheral on the specified Timer instance
 */
PWM_Status_t PWM_Init(PWM_HandleTypeDef *hpwm)
{
    if (hpwm == NULL) return PWM_NULL_POINTER;

    RCC_EnableTIMClock(hpwm->Instance);

    /* PWM Mode 1 configuration for selected channel */
    uint32_t channelOffset = (uint32_t)hpwm->Init.Channel * 8U;
    if (hpwm->Init.Channel < 2U)
    {
        MODIFY_REG(hpwm->Instance->CCMR1, (0x7U << (channelOffset + 4U)), PWM_CCMR1_OC1M_PWM1);
        SET_BIT(hpwm->Instance->CCMR1, (1U << (channelOffset + 3U))); /* OCxPE enable */
    }
    else
    {
        MODIFY_REG(hpwm->Instance->CCMR2, (0x7U << (channelOffset - 16U + 4U)), PWM_CCMR1_OC1M_PWM1);
    }

    hpwm->Busy = false;
    hpwm->ErrorCode = PWM_ERROR_NONE;
    
    /* Set Frequency and Duty Cycle to validate and store into Init */
    if (PWM_SetFrequency(hpwm, hpwm->Init.Frequency) != PWM_OK) return PWM_ERROR;
    if (PWM_SetDutyCycle(hpwm, hpwm->Init.DutyCycle) != PWM_OK) return PWM_ERROR;

    return PWM_OK;
}

/**
 * @brief Updates the PWM Duty Cycle (0.0f to 100.0f)
 */
PWM_Status_t PWM_SetDutyCycle(PWM_HandleTypeDef *hpwm, float dutyCycle)
{
    if (hpwm == NULL) return PWM_NULL_POINTER;
    if ((dutyCycle < 0.0f) || (dutyCycle > 100.0f)) return PWM_ERROR;

    uint32_t arr = hpwm->Instance->ARR;
    uint32_t ccr = (uint32_t)((float)(arr + 1U) * (dutyCycle / 100.0f));
    
    switch (hpwm->Init.Channel)
    {
        case PWM_CHANNEL_1: hpwm->Instance->CCR1 = ccr; break;
        case PWM_CHANNEL_2: hpwm->Instance->CCR2 = ccr; break;
        case PWM_CHANNEL_3: hpwm->Instance->CCR3 = ccr; break;
        case PWM_CHANNEL_4: hpwm->Instance->CCR4 = ccr; break;
        default: return PWM_ERROR;
    }

    hpwm->Init.DutyCycle = dutyCycle;
    return PWM_OK;
}

/**
 * @brief Updates the PWM Frequency (> 0Hz)
 */
PWM_Status_t PWM_SetFrequency(PWM_HandleTypeDef *hpwm, uint32_t frequency)
{
    if (hpwm == NULL) return PWM_NULL_POINTER;
    if (frequency == 0U) return PWM_ERROR;
    
    uint32_t psc = (SYSTEM_CORE_CLOCK / 1000000U) - 1U;
    uint32_t arr = (1000000U / frequency) - 1U;
    
    hpwm->Instance->PSC = psc;
    hpwm->Instance->ARR = arr;
    
    hpwm->Init.Frequency = frequency;
    return PWM_OK;
}

float PWM_GetDutyCycle(const PWM_HandleTypeDef *hpwm)
{
    return (hpwm != NULL) ? hpwm->Init.DutyCycle : 0.0f;
}

uint32_t PWM_GetFrequency(const PWM_HandleTypeDef *hpwm)
{
    return (hpwm != NULL) ? hpwm->Init.Frequency : 0U;
}

PWM_Status_t PWM_Start(PWM_HandleTypeDef *hpwm)
{
    if (hpwm == NULL) return PWM_NULL_POINTER;
    if (hpwm->Busy) return PWM_ERROR;
    
    hpwm->Busy = true;
    SET_BIT(hpwm->Instance->CR1, PWM_BIT(0)); /* CEN */
    return PWM_OK;
}

PWM_Status_t PWM_EnableOutput(PWM_HandleTypeDef *hpwm)
{
    if (hpwm == NULL) return PWM_NULL_POINTER;
    SET_BIT(hpwm->Instance->CCER, PWM_BIT((uint32_t)hpwm->Init.Channel * 4U));
    return PWM_OK;
}

PWM_Status_t PWM_EnableInterrupt(PWM_HandleTypeDef *hpwm)
{
    if (hpwm == NULL) return PWM_NULL_POINTER;
    SET_BIT(hpwm->Instance->DIER, PWM_BIT((uint32_t)hpwm->Init.Channel + PWM_DIER_CC1IE_POS));
    return PWM_OK;
}

void PWM_IRQHandler(PWM_HandleTypeDef *hpwm)
{
    if (hpwm != NULL && (hpwm->Instance->SR & PWM_BIT((uint32_t)hpwm->Init.Channel + 1U)))
    {
        CLEAR_BIT(hpwm->Instance->SR, PWM_BIT((uint32_t)hpwm->Init.Channel + 1U));
        if (hpwm->PulseFinishedCallback != NULL) hpwm->PulseFinishedCallback();
    }
}

PWM_Status_t PWM_Stop(PWM_HandleTypeDef *hpwm)
{
    if (hpwm == NULL) return PWM_NULL_POINTER;
    CLEAR_BIT(hpwm->Instance->CR1, PWM_BIT(0));
    hpwm->Busy = false;
    return PWM_OK;
}

PWM_Status_t PWM_DeInit(PWM_HandleTypeDef *hpwm)
{
    if (hpwm == NULL) return PWM_NULL_POINTER;
    RCC_ResetTIM(hpwm->Instance);
    return PWM_OK;
}