#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>
#include <stdbool.h>

/*======================================================================
 * STM32F407xx System Definitions
 * Project : STM32 Driver Development Framework
 * Author  : Vedula China Venkata Prasanth
 *======================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif

/*======================================================================
 * System Clock
 *======================================================================*/
#define SYSTEM_CORE_CLOCK      16000000UL

/*======================================================================
 * Bus Base Addresses
 *======================================================================*/
#define APB1PERIPH_BASE        0x40000000UL
#define APB2PERIPH_BASE        0x40010000UL
#define AHB1PERIPH_BASE        0x40020000UL

/*======================================================================
 * GPIO Base Addresses
 *======================================================================*/
#define GPIOA_BASE             (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE             (AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE             (AHB1PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE             (AHB1PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE             (AHB1PERIPH_BASE + 0x1000UL)

/*======================================================================
 * RCC Base Address
 *======================================================================*/
#define RCC_BASE               (AHB1PERIPH_BASE + 0x3800UL)

/*======================================================================
 * USART Base Addresses
 *======================================================================*/
#define USART1_BASE            0x40011000UL
#define USART2_BASE            0x40004400UL
#define USART6_BASE            0x40011400UL

/*======================================================================
 * SPI Base Addresses
 *======================================================================*/
#define SPI1_BASE              0x40013000UL
#define SPI2_BASE              0x40003800UL
#define SPI3_BASE              0x40003C00UL

/*======================================================================
 * I2C Base Addresses
 *======================================================================*/
#define I2C1_BASE              0x40005400UL
#define I2C2_BASE              0x40005800UL
#define I2C3_BASE              0x40005C00UL

/*======================================================================
 * Timer Base Addresses
 *======================================================================*/
#define TIM2_BASE              0x40000000UL
#define TIM3_BASE              0x40000400UL
#define TIM4_BASE              0x40000800UL
#define TIM5_BASE              0x40000C00UL

/*======================================================================
 * ADC Base Addresses
 *======================================================================*/
#define ADC1_BASE              0x40012000UL
#define ADC2_BASE              0x40012100UL
#define ADC3_BASE              0x40012200UL

/*======================================================================
 * GPIO Registers
 *======================================================================*/
typedef struct
{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];

} GPIO_TypeDef;

/*======================================================================
 * RCC Registers
 *======================================================================*/
typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;

    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t RESERVED0;

    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1[2];

    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    uint32_t RESERVED2;

    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;

} RCC_TypeDef;

/*======================================================================
 * USART Registers
 *======================================================================*/
typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;

} USART_TypeDef;

/*======================================================================
 * SPI Registers
 *======================================================================*/
typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t CRCPR;
    volatile uint32_t RXCRCR;
    volatile uint32_t TXCRCR;
    volatile uint32_t I2SCFGR;
    volatile uint32_t I2SPR;

} SPI_TypeDef;

/*======================================================================
 * I2C Registers
 *======================================================================*/
typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t DR;
    volatile uint32_t SR1;
    volatile uint32_t SR2;
    volatile uint32_t CCR;
    volatile uint32_t TRISE;
    volatile uint32_t FLTR;

} I2C_TypeDef;

/*======================================================================
 * Timer Registers
 *======================================================================*/
typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t BDTR;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;

} TIM_TypeDef;

/*======================================================================
 * ADC Registers
 *======================================================================*/
typedef struct
{
    volatile uint32_t SR;    /* ADC status register */
    volatile uint32_t CR1;   /* ADC control register 1 */
    volatile uint32_t CR2;   /* ADC control register 2 */
    volatile uint32_t SMPR1; /* ADC sample time register 1 */
    volatile uint32_t SMPR2; /* ADC sample time register 2 */
    volatile uint32_t JOFR1; /* ADC injected channel data offset register 1 */
    volatile uint32_t JOFR2; /* ADC injected channel data offset register 2 */
    volatile uint32_t JOFR3; /* ADC injected channel data offset register 3 */
    volatile uint32_t JOFR4; /* ADC injected channel data offset register 4 */
    volatile uint32_t HTR;   /* ADC watchdog high threshold register */
    volatile uint32_t LTR;   /* ADC watchdog low threshold register */
    volatile uint32_t SQR1;  /* ADC regular sequence register 1 */
    volatile uint32_t SQR2;  /* ADC regular sequence register 2 */
    volatile uint32_t SQR3;  /* ADC regular sequence register 3 */
    volatile uint32_t JSQR;  /* ADC injected sequence register */
    volatile uint32_t JDR1;  /* ADC injected data register 1 */
    volatile uint32_t JDR2;  /* ADC injected data register 2 */
    volatile uint32_t JDR3;  /* ADC injected data register 3 */
    volatile uint32_t JDR4;  /* ADC injected data register 4 */
    volatile uint32_t DR;    /* ADC regular data register */
} ADC_TypeDef;

/*======================================================================
 * Peripheral Instances
 *======================================================================*/

/* GPIO */
#define GPIOA      ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB      ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC      ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD      ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE      ((GPIO_TypeDef *)GPIOE_BASE)

/* RCC */
#define RCC        ((RCC_TypeDef *)RCC_BASE)

/* USART */
#define USART1     ((USART_TypeDef *)USART1_BASE)
#define USART2     ((USART_TypeDef *)USART2_BASE)
#define USART6     ((USART_TypeDef *)USART6_BASE)

/* SPI */
#define SPI1       ((SPI_TypeDef *)SPI1_BASE)
#define SPI2       ((SPI_TypeDef *)SPI2_BASE)
#define SPI3       ((SPI_TypeDef *)SPI3_BASE)

/* I2C */
#define I2C1       ((I2C_TypeDef *)I2C1_BASE)
#define I2C2       ((I2C_TypeDef *)I2C2_BASE)
#define I2C3       ((I2C_TypeDef *)I2C3_BASE)

/* TIM */
#define TIM2       ((TIM_TypeDef *)TIM2_BASE)
#define TIM3       ((TIM_TypeDef *)TIM3_BASE)
#define TIM4       ((TIM_TypeDef *)TIM4_BASE)
#define TIM5       ((TIM_TypeDef *)TIM5_BASE)

/* ADC */
#define ADC1       ((ADC_TypeDef *)ADC1_BASE)
#define ADC2       ((ADC_TypeDef *)ADC2_BASE)
#define ADC3       ((ADC_TypeDef *)ADC3_BASE)

/*======================================================================
 * Generic Driver Status
 *======================================================================*/
typedef enum
{
    STATUS_OK = 0,
    STATUS_ERROR,
    STATUS_TIMEOUT,
    STATUS_BUSY

} StatusTypeDef;

#ifdef __cplusplus
}
/*==========================================================
 System APIs
==========================================================*/

void System_Init(void);

void SystemClock_Config(void);

void System_Delay(volatile uint32_t count);
#endif

#endif /* SYSTEM_H */