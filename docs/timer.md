# Timer Driver

## Overview

The Timer driver provides register-level control of the STM32F407VG general-purpose timers.

It supports timing operations, delays, interrupt generation and callback mechanisms using direct register programming.

---

## Features

- Timer Initialization
- Timer DeInitialization
- Start Timer
- Stop Timer
- Counter Read
- Counter Write
- Blocking Delay
- Interrupt Support
- Callback Registration
- Running Status API

---

## Supported Timers

- TIM2
- TIM3
- TIM4
- TIM5

---

## Public APIs

```c
TIMER_Init()

TIMER_DeInit()

TIMER_Start()

TIMER_Stop()

TIMER_GetCounter()

TIMER_SetCounter()

TIMER_Delay()

TIMER_EnableInterrupt()

TIMER_DisableInterrupt()

TIMER_IRQHandler()

TIMER_RegisterCallback()

TIMER_IsRunning()
```

---

## Driver Architecture

```
Application

↓

Timer Driver

↓

TIM Registers

↓

STM32 Hardware
```

---

## Counter Modes

- Up Counter
- Down Counter

---

## Design Goals

- Accurate Timing
- Modular Design
- Interrupt Driven
- Production Ready