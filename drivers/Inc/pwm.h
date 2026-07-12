/**
 * @file    pwm.h
 * @brief   Pulse Width Modulation (PWM) Driver Header
 * @details Production-ready, MISRA-friendly header for STM32F407VG PWM control.
 * Implements register-level PWM configurations and state management based on Timers.
 */

#ifndef PWM_H
#define PWM_H

#include "system.h"
#include "timer.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
 * EXPORTED TYPES & ENUMS
 *===========================================================================*/

typedef enum
{
    PWM_OK             = 0x00U,
    PWM_ERROR          = 0x01U,
    PWM_TIMEOUT        = 0x02U,
    PWM_NULL_POINTER   = 0x03U
} PWM_Status_t;

typedef enum
{
    PWM_ERROR_NONE    = 0x00U,
    PWM_ERROR_INVALID = (1U << 0)
} PWM_Error_t;

typedef enum
{
    PWM_CHANNEL_1 = 0x00U,
    PWM_CHANNEL_2 = 0x01U,
    PWM_CHANNEL_3 = 0x02U,
    PWM_CHANNEL_4 = 0x03U
} PWM_Channel_t;

typedef enum
{
    PWM_POLARITY_HIGH = 0x00U,
    PWM_POLARITY_LOW  = 0x01U
} PWM_Polarity_t;

typedef struct
{
    uint32_t       Frequency;
    float          DutyCycle;
    PWM_Channel_t  Channel;
    PWM_Polarity_t Polarity;
} PWM_Config_t;

typedef void (*PWM_Callback_t)(void);

typedef struct
{
    TIM_TypeDef      *Instance;
    PWM_Config_t      Init;
    volatile bool     Busy;
    PWM_Error_t       ErrorCode;
    PWM_Callback_t    PulseFinishedCallback;
} PWM_HandleTypeDef;

/*=============================================================================
 * PUBLIC API FUNCTION PROTOTYPES
 *===========================================================================*/

PWM_Status_t PWM_Init(PWM_HandleTypeDef *hpwm);
PWM_Status_t PWM_DeInit(PWM_HandleTypeDef *hpwm);
PWM_Status_t PWM_Start(PWM_HandleTypeDef *hpwm);
PWM_Status_t PWM_Stop(PWM_HandleTypeDef *hpwm);
PWM_Status_t PWM_SetDutyCycle(PWM_HandleTypeDef *hpwm, float dutyCycle);
PWM_Status_t PWM_SetFrequency(PWM_HandleTypeDef *hpwm, uint32_t frequency);
float        PWM_GetDutyCycle(const PWM_HandleTypeDef *hpwm);
uint32_t     PWM_GetFrequency(const PWM_HandleTypeDef *hpwm);
PWM_Status_t PWM_EnableOutput(PWM_HandleTypeDef *hpwm);
PWM_Status_t PWM_DisableOutput(PWM_HandleTypeDef *hpwm);
PWM_Status_t PWM_EnableInterrupt(PWM_HandleTypeDef *hpwm);
PWM_Status_t PWM_DisableInterrupt(PWM_HandleTypeDef *hpwm);
void         PWM_IRQHandler(PWM_HandleTypeDef *hpwm);
void         PWM_RegisterCallback(PWM_HandleTypeDef *hpwm, PWM_Callback_t callback);

#ifdef __cplusplus
}
#endif

#endif /* PWM_H */