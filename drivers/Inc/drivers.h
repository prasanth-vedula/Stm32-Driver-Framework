#ifndef DRIVERS_H
#define DRIVERS_H

#ifdef __cplusplus
extern "C"
{
#endif

/*==========================================================
                STM32 Driver Framework
==========================================================*/

/* Core */

#include "system.h"

#include "common.h"

#include "bit_utils.h"

/* Peripheral Drivers */

#include "rcc.h"

#include "gpio.h"

#include "uart.h"

#include "spi.h"

#include "i2c.h"

#include "timer.h"

/*
 * Uncomment when available
 *
 * #include "adc.h"
 *
 * #include "pwm.h"
 *
 * #include "dma.h"
 *
 * #include "flash.h"
 *
 * #include "rtc.h"
 *
 */

#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_H */