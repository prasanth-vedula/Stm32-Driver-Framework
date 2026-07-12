# PWM Driver

## Overview

The PWM driver provides register-level Pulse Width Modulation generation using the STM32F407VG timer peripherals.

The driver supports frequency control, duty cycle adjustment, interrupt handling and callback registration.

---

## Features

- PWM Initialization
- PWM DeInitialization
- PWM Start
- PWM Stop
- Duty Cycle Control
- Frequency Control
- Output Enable
- Output Disable
- Interrupt Support
- Callback Registration
- Register-Level Programming

---

## Supported Timers

- TIM2
- TIM3
- TIM4
- TIM5

---

## Supported Channels

- Channel 1
- Channel 2
- Channel 3
- Channel 4

---

## Public APIs

```c
PWM_Init()

PWM_DeInit()

PWM_Start()

PWM_Stop()

PWM_SetDutyCycle()

PWM_SetFrequency()

PWM_GetDutyCycle()

PWM_GetFrequency()

PWM_EnableOutput()

PWM_DisableOutput()

PWM_EnableInterrupt()

PWM_DisableInterrupt()

PWM_IRQHandler()

PWM_RegisterCallback()
```

---

## Driver Architecture

```
Application

↓

PWM Driver

↓

Timer Registers

↓

PWM Output

↓

STM32 Hardware
```

---

## Supported Features

- Variable Frequency
- Variable Duty Cycle
- High Polarity
- Low Polarity
- Interrupt Driven Operation

---

## Design Goals

- Production Ready
- Modular Design
- Register-Level Implementation
- Reusable Driver Framework