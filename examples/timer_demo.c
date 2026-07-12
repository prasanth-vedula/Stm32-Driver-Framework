#include "timer.h"
#include "rcc.h"

static TIMER_HandleTypeDef htim2;

/*==========================================================
 Timer Callback
==========================================================*/
void TimerCallback(void)
{
    /* User application code */

}

/*==========================================================
 Main
==========================================================*/
int main(void)
{
    /* Enable Peripheral Clock */
    RCC_EnableTIMClock(TIM2);

    /* Configure Timer */
    htim2.Instance = TIM2;

    htim2.Init.Prescaler = 16000U - 1U;
    htim2.Init.Period = 1000U - 1U;
    htim2.Init.CounterMode = TIMER_COUNTER_UP;
    htim2.Init.ClockDivision = TIMER_CLOCK_DIV1;

    /* Initialize Driver */
    TIMER_Init(&htim2);

    /* Register Interrupt Callback */
    TIMER_RegisterCallback(&htim2, TimerCallback);

    /* Enable Interrupt */
    TIMER_EnableInterrupt(&htim2);

    /* Start Timer */
    TIMER_Start(&htim2);

    while(1)
    {
        TIMER_Delay(&htim2, 500U);

        /* User Application */

    }
}