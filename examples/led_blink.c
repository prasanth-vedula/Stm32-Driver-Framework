/******************************************************************************
 * File        : led_blink.c
 * Project     : STM32 Driver Development Framework
 ******************************************************************************/

#include "drivers.h"

/*==========================================================
                    Global Objects
==========================================================*/

static GPIO_Config_t led;

/*==========================================================
                    Main
==========================================================*/

int main(void)
{
    RCC_EnableGPIOClock(GPIOA);

    led.pin = GPIO_PIN_5;
    led.mode = GPIO_MODE_OUTPUT;
    led.outputType = GPIO_PUSH_PULL;
    led.speed = GPIO_SPEED_HIGH;
    led.pull = GPIO_NOPULL;

    GPIO_Init(GPIOA, &led);

    while (1)
    {
        GPIO_TogglePin(GPIOA, GPIO_PIN_5);

        System_Delay(500000);
    }
}