/******************************************************************************
 * File        : system.c
 * Project     : STM32 Driver Development Framework
 * MCU         : STM32F407VG
 ******************************************************************************/

#include "system.h"

/*==========================================================
                System Initialization
==========================================================*/
void System_Init(void)
{
    SystemClock_Config();
}

/*==========================================================
                Clock Configuration
==========================================================*/
void SystemClock_Config(void)
{
    /*
     * Default Clock Configuration
     *
     * HSI = 16 MHz
     *
     * PLL Configuration
     * will be added in future versions.
     */
}

/*==========================================================
                Delay
==========================================================*/
void System_Delay(volatile uint32_t count)
{
    while (count--)
    {

    }
}