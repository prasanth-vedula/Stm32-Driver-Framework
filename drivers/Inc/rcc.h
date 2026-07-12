#ifndef RCC_H
#define RCC_H

#include "system.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*======================================================================
 * RCC Driver
 * STM32 Driver Development Framework
 *====================================================================*/

/*======================================================================
 * GPIO Clock Control
 *====================================================================*/
void RCC_EnableGPIOClock(GPIO_TypeDef *GPIOx);

void RCC_DisableGPIOClock(GPIO_TypeDef *GPIOx);

void RCC_ResetGPIO(GPIO_TypeDef *GPIOx);

/*======================================================================
 * USART Clock Control
 *====================================================================*/
void RCC_EnableUSARTClock(USART_TypeDef *USARTx);

void RCC_DisableUSARTClock(USART_TypeDef *USARTx);

void RCC_ResetUSART(USART_TypeDef *USARTx);

/*======================================================================
 * SPI Clock Control
 *====================================================================*/
void RCC_EnableSPIClock(SPI_TypeDef *SPIx);

void RCC_DisableSPIClock(SPI_TypeDef *SPIx);

void RCC_ResetSPI(SPI_TypeDef *SPIx);

/*======================================================================
 * I2C Clock Control
 *====================================================================*/
void RCC_EnableI2CClock(I2C_TypeDef *I2Cx);

void RCC_DisableI2CClock(I2C_TypeDef *I2Cx);

void RCC_ResetI2C(I2C_TypeDef *I2Cx);

/*======================================================================
 * Timer Clock Control
 *====================================================================*/
void RCC_EnableTIMClock(TIM_TypeDef *TIMx);

void RCC_DisableTIMClock(TIM_TypeDef *TIMx);

void RCC_ResetTIM(TIM_TypeDef *TIMx);

/*======================================================================
 * ADC Clock Control
 *====================================================================*/
void RCC_EnableADCClock(ADC_TypeDef *ADCx);

void RCC_ResetADC(ADC_TypeDef *ADCx);

#ifdef __cplusplus
}
#endif

#endif /* RCC_H */