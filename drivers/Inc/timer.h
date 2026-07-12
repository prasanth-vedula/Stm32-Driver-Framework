/**
 * @file    timer.h
 * @brief   Hardware Timer Driver Header
 * @details Production-ready, MISRA-friendly header for STM32F407VG Timer control.
 * Implements register-level configurations and state management.
 */

#ifndef TIMER_H
#define TIMER_H

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
 * EXPORTED TYPES & ENUMS
 *===========================================================================*/

/**
 * @brief Timer operation status codes
 */
typedef enum
{
    TIMER_OK             = 0x00U,  /**< Operation successful */
    TIMER_ERROR          = 0x01U,  /**< General error */
    TIMER_TIMEOUT        = 0x02U,  /**< Operation timed out */
    TIMER_NULL_POINTER   = 0x03U   /**< Invalid null pointer provided */
} TIMER_Status_t;

/**
 * @brief Timer counter modes (Direction)
 */
typedef enum
{
    TIMER_COUNTER_UP     = 0x00U,  /**< Counter counts upwards */
    TIMER_COUNTER_DOWN   = 0x01U   /**< Counter counts downwards */
} TIMER_CounterMode_t;

/**
 * @brief Timer clock division (CKD) settings
 */
typedef enum
{
    TIMER_CLOCK_DIV1     = 0x00U,  /**< Clock division: tDTS = tCK_INT */
    TIMER_CLOCK_DIV2     = 0x01U,  /**< Clock division: tDTS = 2 * tCK_INT */
    TIMER_CLOCK_DIV4     = 0x02U   /**< Clock division: tDTS = 4 * tCK_INT */
} TIMER_ClockDivision_t;

/**
 * @brief Timer configuration structure
 */
typedef struct
{
    uint32_t              Prescaler;     /**< Specifies the prescaler value used to divide the TIM clock */
    uint32_t              Period;        /**< Specifies the auto-reload value (ARR) */
    TIMER_CounterMode_t   CounterMode;   /**< Specifies the counter mode (Up/Down) */
    TIMER_ClockDivision_t ClockDivision; /**< Specifies the clock division (CKD) */
} TIMER_Config_t;

/**
 * @brief Timer callback function pointer type definition
 */
typedef void (*TIMER_Callback_t)(void);

/**
 * @brief Timer handle structure definition
 */
typedef struct
{
    TIM_TypeDef         *Instance;              /**< Pointer to Timer peripheral base address */
    TIMER_Config_t       Init;                  /**< Timer configuration parameters */
    volatile bool        Running;               /**< Timer running state flag (true = running, false = stopped) */
    TIMER_Status_t       ErrorCode;             /**< Timer current error code */
    TIMER_Callback_t     PeriodElapsedCallback; /**< Callback function pointer for Period Elapsed interrupt */
} TIMER_HandleTypeDef;

/*=============================================================================
 * PUBLIC API FUNCTION PROTOTYPES
 *===========================================================================*/

/*------------------------- Initialization ----------------------------------*/

/**
 * @brief Initializes the Timer peripheral according to the specified parameters in htim
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return TIMER_Status_t Status of the initialization
 */
TIMER_Status_t TIMER_Init(TIMER_HandleTypeDef *htim);

/**
 * @brief De-initializes the Timer peripheral
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return TIMER_Status_t Status of the de-initialization
 */
TIMER_Status_t TIMER_DeInit(TIMER_HandleTypeDef *htim);

/*------------------------- Counter Control ---------------------------------*/

/**
 * @brief Starts the Timer counter
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return TIMER_Status_t Status of the start operation
 */
TIMER_Status_t TIMER_Start(TIMER_HandleTypeDef *htim);

/**
 * @brief Stops the Timer counter
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return TIMER_Status_t Status of the stop operation
 */
TIMER_Status_t TIMER_Stop(TIMER_HandleTypeDef *htim);

/**
 * @brief Resets the Timer peripheral to default state
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return TIMER_Status_t Status of the reset operation
 */
TIMER_Status_t TIMER_Reset(TIMER_HandleTypeDef *htim);

/*------------------------- Interrupts & Callbacks --------------------------*/

/**
 * @brief Enables the Timer Update (Period Elapsed) Interrupt
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return TIMER_Status_t Status of the interrupt enable operation
 */
TIMER_Status_t TIMER_EnableInterrupt(TIMER_HandleTypeDef *htim);

/**
 * @brief Disables the Timer Update (Period Elapsed) Interrupt
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return TIMER_Status_t Status of the interrupt disable operation
 */
TIMER_Status_t TIMER_DisableInterrupt(TIMER_HandleTypeDef *htim);

/**
 * @brief Timer Interrupt Request Handler
 * @details Should be called from the respective TIMx_IRQHandler in the application layer
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 */
void TIMER_IRQHandler(TIMER_HandleTypeDef *htim);

/**
 * @brief Registers a user callback for the Period Elapsed event
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @param callback Function pointer to the user callback
 */
void TIMER_RegisterCallback(TIMER_HandleTypeDef *htim, TIMER_Callback_t callback);

/*------------------------- Configuration APIs ------------------------------*/

/**
 * @brief Retrieves the current Timer counter value
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return uint32_t Current counter value (CNT register)
 */
uint32_t TIMER_GetCounter(TIMER_HandleTypeDef *htim);

/**
 * @brief Sets the Timer counter to a specific value
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @param value The value to set the counter to
 */
void TIMER_SetCounter(TIMER_HandleTypeDef *htim, uint32_t value);

/**
 * @brief Dynamically updates the Timer prescaler value
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @param prescaler The new prescaler value
 */
void TIMER_SetPrescaler(TIMER_HandleTypeDef *htim, uint32_t prescaler);

/**
 * @brief Dynamically updates the Timer auto-reload period value
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @param period The new period value
 */
void TIMER_SetPeriod(TIMER_HandleTypeDef *htim, uint32_t period);

/*------------------------- Delay & Status ----------------------------------*/

/**
 * @brief Implements a blocking delay based on the Timer counter
 * @note The Timer must be configured and running for this function to operate
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @param ticks Number of timer ticks to wait
 * @return TIMER_Status_t Status of the delay operation
 */
TIMER_Status_t TIMER_Delay(TIMER_HandleTypeDef *htim, uint32_t ticks);

/**
 * @brief Checks if the Timer is currently running
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return bool true if running, false if stopped
 */
bool TIMER_IsRunning(TIMER_HandleTypeDef *htim);

/**
 * @brief Retrieves the current operating status and error state of the Timer
 *
 * @param htim Pointer to a TIMER_HandleTypeDef structure
 * @return TIMER_Status_t Current operational status
 */
TIMER_Status_t TIMER_GetStatus(TIMER_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif

#endif /* TIMER_H */