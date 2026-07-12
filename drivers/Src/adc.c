/**
 * @file    adc.h
 * @brief   Analog-to-Digital Converter (ADC) Driver Header
 */

#ifndef ADC_H
#define ADC_H

#include "system.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { ADC_OK = 0x00U, ADC_ERROR = 0x01U, ADC_TIMEOUT = 0x02U, ADC_NULL_POINTER = 0x03U } ADC_Status_t;
typedef enum { ADC_ERROR_NONE = 0x00U, ADC_ERROR_OVERRUN = (1U << 0), ADC_ERROR_TIMEOUT = (1U << 1) } ADC_Error_t;
typedef enum { ADC_MODE_SINGLE = 0x00U, ADC_MODE_CONTINUOUS = 0x01U } ADC_Mode_t;
typedef enum { ADC_RES_12BIT = 0x00U, ADC_RES_10BIT = 0x01U, ADC_RES_8BIT = 0x02U, ADC_RES_6BIT = 0x03U } ADC_Resolution_t;
typedef enum { ADC_SAMPLETIME_3 = 0x00U, ADC_SAMPLETIME_15 = 0x01U, ADC_SAMPLETIME_28 = 0x02U, ADC_SAMPLETIME_56 = 0x03U, 
               ADC_SAMPLETIME_84 = 0x04U, ADC_SAMPLETIME_112 = 0x05U, ADC_SAMPLETIME_144 = 0x06U, ADC_SAMPLETIME_480 = 0x07U } ADC_SampleTime_t;

typedef struct {
    ADC_Mode_t       mode;
    ADC_Resolution_t resolution;
    ADC_SampleTime_t sampleTime;
    uint8_t          channel;
} ADC_Config_t;

typedef void (*ADC_Callback_t)(void);

typedef struct {
    ADC_TypeDef     *Instance;
    ADC_Config_t     Init;
    volatile bool    Busy;
    ADC_Error_t      ErrorCode;
    ADC_Callback_t   ConversionCompleteCallback;
} ADC_HandleTypeDef;

ADC_Status_t ADC_Init(ADC_HandleTypeDef *hadc);
ADC_Status_t ADC_DeInit(ADC_HandleTypeDef *hadc);
ADC_Status_t ADC_Start(ADC_HandleTypeDef *hadc);
ADC_Status_t ADC_Stop(ADC_HandleTypeDef *hadc);
ADC_Status_t ADC_Read(ADC_HandleTypeDef *hadc, uint16_t *value);
ADC_Status_t ADC_EnableInterrupt(ADC_HandleTypeDef *hadc);
ADC_Status_t ADC_DisableInterrupt(ADC_HandleTypeDef *hadc);
void ADC_IRQHandler(ADC_HandleTypeDef *hadc);
void ADC_RegisterCallback(ADC_HandleTypeDef *hadc,
                          ADC_Callback_t callback)
{
    if(hadc != NULL)
    {
        hadc->ConversionCompleteCallback = callback;
    }
}
uint8_t ADC_GetState(ADC_HandleTypeDef *hadc);
ADC_Error_t ADC_GetError(ADC_HandleTypeDef *hadc);

#ifdef __cplusplus
}
#endif
#endif /* ADC_H */