/**
 * @file    rcc.c
 * @brief   Reset and Clock Control (RCC) Driver Implementation
 * @details Production-ready, MISRA-friendly driver for STM32F407VG.
 * Implements register-level clock control for GPIO, USART, SPI, I2C, TIM, and ADC.
 */

#include "rcc.h"
#include "bit_utils.h"
#include <stddef.h>

/*=============================================================================
 * MACROS & DEFINES
 *===========================================================================*/

/** @brief Bit position definitions to eliminate magic numbers */
#define RCC_AHB1_GPIOA_POS  (0U)
#define RCC_AHB1_GPIOB_POS  (1U)
#define RCC_AHB1_GPIOC_POS  (2U)
#define RCC_AHB1_GPIOD_POS  (3U)
#define RCC_AHB1_GPIOE_POS  (4U)

#define RCC_APB2_USART1_POS (4U)
#define RCC_APB1_USART2_POS (17U)
#define RCC_APB2_USART6_POS (5U)

#define RCC_APB2_SPI1_POS   (12U)
#define RCC_APB1_SPI2_POS   (14U)
#define RCC_APB1_SPI3_POS   (15U)

#define RCC_APB1_I2C1_POS   (21U)
#define RCC_APB1_I2C2_POS   (22U)
#define RCC_APB1_I2C3_POS   (23U)

#define RCC_APB1_TIM2_POS   (0U)
#define RCC_APB1_TIM3_POS   (1U)
#define RCC_APB1_TIM4_POS   (2U)
#define RCC_APB1_TIM5_POS   (3U)

#define RCC_APB2_ADC_POS    (8U)

/** @brief Helper macro to compute bitmask from position */
#define RCC_BIT(bit)        (1UL << (bit))

/*=============================================================================
 * PRIVATE TYPES
 *===========================================================================*/

/**
 * @brief Configuration structure linking a peripheral to its RCC registers
 */
typedef struct
{
    volatile uint32_t *enReg;  /**< Pointer to the Clock Enable Register */
    volatile uint32_t *rstReg; /**< Pointer to the Peripheral Reset Register */
    uint32_t           mask;   /**< Bitmask for the specific peripheral */
} RccConfig_t;

/*=============================================================================
 * PRIVATE FUNCTION PROTOTYPES
 *===========================================================================*/

static uint8_t RCC_GetGPIOConfig(const GPIO_TypeDef *const GPIOx, RccConfig_t *const pConfig);
static uint8_t RCC_GetUSARTConfig(const USART_TypeDef *const USARTx, RccConfig_t *const pConfig);
static uint8_t RCC_GetSPIConfig(const SPI_TypeDef *const SPIx, RccConfig_t *const pConfig);
static uint8_t RCC_GetI2CConfig(const I2C_TypeDef *const I2Cx, RccConfig_t *const pConfig);
static uint8_t RCC_GetTIMConfig(const TIM_TypeDef *const TIMx, RccConfig_t *const pConfig);

static void RCC_EnableClockInternal(const RccConfig_t *const pConfig);
static void RCC_DisableClockInternal(const RccConfig_t *const pConfig);
static void RCC_ResetPeripheralInternal(const RccConfig_t *const pConfig);

/*=============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 *===========================================================================*/

static uint8_t RCC_GetGPIOConfig(const GPIO_TypeDef *const GPIOx, RccConfig_t *const pConfig)
{
    uint8_t isValid = 1U;
    pConfig->enReg  = &(RCC->AHB1ENR);
    pConfig->rstReg = &(RCC->AHB1RSTR);
    if      (GPIOx == GPIOA) pConfig->mask = RCC_BIT(RCC_AHB1_GPIOA_POS);
    else if (GPIOx == GPIOB) pConfig->mask = RCC_BIT(RCC_AHB1_GPIOB_POS);
    else if (GPIOx == GPIOC) pConfig->mask = RCC_BIT(RCC_AHB1_GPIOC_POS);
    else if (GPIOx == GPIOD) pConfig->mask = RCC_BIT(RCC_AHB1_GPIOD_POS);
    else if (GPIOx == GPIOE) pConfig->mask = RCC_BIT(RCC_AHB1_GPIOE_POS);
    else isValid = 0U;
    return isValid;
}

static uint8_t RCC_GetUSARTConfig(const USART_TypeDef *const USARTx, RccConfig_t *const pConfig)
{
    uint8_t isValid = 1U;
    if      (USARTx == USART1) { pConfig->enReg = &(RCC->APB2ENR); pConfig->rstReg = &(RCC->APB2RSTR); pConfig->mask = RCC_BIT(RCC_APB2_USART1_POS); }
    else if (USARTx == USART2) { pConfig->enReg = &(RCC->APB1ENR); pConfig->rstReg = &(RCC->APB1RSTR); pConfig->mask = RCC_BIT(RCC_APB1_USART2_POS); }
    else if (USARTx == USART6) { pConfig->enReg = &(RCC->APB2ENR); pConfig->rstReg = &(RCC->APB2RSTR); pConfig->mask = RCC_BIT(RCC_APB2_USART6_POS); }
    else isValid = 0U;
    return isValid;
}

static uint8_t RCC_GetSPIConfig(const SPI_TypeDef *const SPIx, RccConfig_t *const pConfig)
{
    uint8_t isValid = 1U;
    if      (SPIx == SPI1) { pConfig->enReg = &(RCC->APB2ENR); pConfig->rstReg = &(RCC->APB2RSTR); pConfig->mask = RCC_BIT(RCC_APB2_SPI1_POS); }
    else if (SPIx == SPI2) { pConfig->enReg = &(RCC->APB1ENR); pConfig->rstReg = &(RCC->APB1RSTR); pConfig->mask = RCC_BIT(RCC_APB1_SPI2_POS); }
    else if (SPIx == SPI3) { pConfig->enReg = &(RCC->APB1ENR); pConfig->rstReg = &(RCC->APB1RSTR); pConfig->mask = RCC_BIT(RCC_APB1_SPI3_POS); }
    else isValid = 0U;
    return isValid;
}

static uint8_t RCC_GetI2CConfig(const I2C_TypeDef *const I2Cx, RccConfig_t *const pConfig)
{
    uint8_t isValid = 1U;
    pConfig->enReg  = &(RCC->APB1ENR);
    pConfig->rstReg = &(RCC->APB1RSTR);
    if      (I2Cx == I2C1) pConfig->mask = RCC_BIT(RCC_APB1_I2C1_POS);
    else if (I2Cx == I2C2) pConfig->mask = RCC_BIT(RCC_APB1_I2C2_POS);
    else if (I2Cx == I2C3) pConfig->mask = RCC_BIT(RCC_APB1_I2C3_POS);
    else isValid = 0U;
    return isValid;
}

static uint8_t RCC_GetTIMConfig(const TIM_TypeDef *const TIMx, RccConfig_t *const pConfig)
{
    uint8_t isValid = 1U;
    pConfig->enReg  = &(RCC->APB1ENR);
    pConfig->rstReg = &(RCC->APB1RSTR);
    if      (TIMx == TIM2) pConfig->mask = RCC_BIT(RCC_APB1_TIM2_POS);
    else if (TIMx == TIM3) pConfig->mask = RCC_BIT(RCC_APB1_TIM3_POS);
    else if (TIMx == TIM4) pConfig->mask = RCC_BIT(RCC_APB1_TIM4_POS);
    else if (TIMx == TIM5) pConfig->mask = RCC_BIT(RCC_APB1_TIM5_POS);
    else isValid = 0U;
    return isValid;
}

static void RCC_EnableClockInternal(const RccConfig_t *const pConfig)
{
    if (pConfig->enReg != NULL) SET_BIT(*(pConfig->enReg), pConfig->mask);
}

static void RCC_DisableClockInternal(const RccConfig_t *const pConfig)
{
    if (pConfig->enReg != NULL) CLEAR_BIT(*(pConfig->enReg), pConfig->mask);
}

static void RCC_ResetPeripheralInternal(const RccConfig_t *const pConfig)
{
    if (pConfig->rstReg != NULL)
    {
        SET_BIT(*(pConfig->rstReg), pConfig->mask);
        CLEAR_BIT(*(pConfig->rstReg), pConfig->mask);
    }
}

/*=============================================================================
 * PUBLIC API IMPLEMENTATIONS
 *===========================================================================*/

void RCC_EnableGPIOClock(GPIO_TypeDef *GPIOx)
{
    RccConfig_t config = { NULL, NULL, 0U };
    if ((GPIOx != NULL) && (RCC_GetGPIOConfig(GPIOx, &config) == 1U)) RCC_EnableClockInternal(&config);
}

void RCC_DisableGPIOClock(GPIO_TypeDef *GPIOx)
{
    RccConfig_t config = { NULL, NULL, 0U };
    if ((GPIOx != NULL) && (RCC_GetGPIOConfig(GPIOx, &config) == 1U)) RCC_DisableClockInternal(&config);
}

void RCC_ResetGPIO(GPIO_TypeDef *GPIOx)
{
    RccConfig_t config = { NULL, NULL, 0U };
    if ((GPIOx != NULL) && (RCC_GetGPIOConfig(GPIOx, &config) == 1U)) RCC_ResetPeripheralInternal(&config);
}

void RCC_EnableUSARTClock(USART_TypeDef *USARTx)
{
    RccConfig_t config = { NULL, NULL, 0U };
    if ((USARTx != NULL) && (RCC_GetUSARTConfig(USARTx, &config) == 1U)) RCC_EnableClockInternal(&config);
}

void RCC_DisableUSARTClock(USART_TypeDef *USARTx)
{
    RccConfig_t config = { NULL, NULL, 0U };
    if ((USARTx != NULL) && (RCC_GetUSARTConfig(USARTx, &config) == 1U)) RCC_DisableClockInternal(&config);
}

void RCC_ResetUSART(USART_TypeDef *USARTx)
{
    RccConfig_t config = { NULL, NULL, 0U };
    if ((USARTx != NULL) && (RCC_GetUSARTConfig(USARTx, &config) == 1U)) RCC_ResetPeripheralInternal(&config);
}

void RCC_EnableSPIClock(SPI_TypeDef *SPIx)
{
    RccConfig_t config = { NULL, NULL, 0U };
    if ((SPIx != NULL) && (RCC_GetSPIConfig(SPIx, &config) == 1U)) RCC_EnableClockInternal(&config);
}

void RCC_DisableSPIClock(SPI_TypeDef *SPIx)
{
    RccConfig_t config = { NULL, NULL, 0U };
    if ((SPIx != NULL) && (RCC_GetSPIConfig(SPIx, &config) == 1U)) RCC_DisableClockInternal(&config);
}

void RCC_ResetSPI(SPI_TypeDef *SPIx)
{
    RccConfig_t config = { NULL, NULL, 0U };
    if ((SPIx != NULL) && (RCC_GetSPIConfig(SPIx, &config) == 1U)) RCC_ResetPeripheralInternal(&config);
}

void RCC_EnableI2CClock(I2C_TypeDef *I2Cx)
{
    RccConfig_t config = { NULL, NULL, 0U };
    if ((I2Cx != NULL) && (RCC_GetI2CConfig(I2Cx, &config) == 1U)) RCC_EnableClockInternal(&config);
}

void RCC_DisableI2CClock(I2C_TypeDef *I2Cx)
{
    RccConfig_t config = { NULL, NULL, 0U };
    if ((I2Cx != NULL) && (RCC_GetI2CConfig(I2Cx, &config) == 1U)) RCC_DisableClockInternal(&config);
}

void RCC_ResetI2C(I2C_TypeDef *I2Cx)
{
    RccConfig_t config = { NULL, NULL, 0U };
    if ((I2Cx != NULL) && (RCC_GetI2CConfig(I2Cx, &config) == 1U)) RCC_ResetPeripheralInternal(&config);
}

void RCC_EnableTIMClock(TIM_TypeDef *TIMx)
{
    RccConfig_t config = { NULL, NULL, 0U };
    if ((TIMx != NULL) && (RCC_GetTIMConfig(TIMx, &config) == 1U)) RCC_EnableClockInternal(&config);
}

void RCC_DisableTIMClock(TIM_TypeDef *TIMx)
{
    RccConfig_t config = { NULL, NULL, 0U };
    if ((TIMx != NULL) && (RCC_GetTIMConfig(TIMx, &config) == 1U)) RCC_DisableClockInternal(&config);
}

void RCC_ResetTIM(TIM_TypeDef *TIMx)
{
    RccConfig_t config = { NULL, NULL, 0U };
    if ((TIMx != NULL) && (RCC_GetTIMConfig(TIMx, &config) == 1U)) RCC_ResetPeripheralInternal(&config);
}

void RCC_EnableADCClock(ADC_TypeDef *ADCx)
{
    SET_BIT(RCC->APB2ENR, RCC_BIT(RCC_APB2_ADC_POS));
}

void RCC_ResetADC(ADC_TypeDef *ADCx)
{
    SET_BIT(RCC->APB2RSTR, RCC_BIT(RCC_APB2_ADC_POS));
    CLEAR_BIT(RCC->APB2RSTR, RCC_BIT(RCC_APB2_ADC_POS));
}