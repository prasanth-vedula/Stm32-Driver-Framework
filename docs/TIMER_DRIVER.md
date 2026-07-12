# STM32 Timer Driver

## Overview

The Timer Driver provides a lightweight and reusable Hardware Abstraction Layer (HAL) for STM32F407 General Purpose Timers using direct register-level programming.

This implementation avoids STM32 HAL libraries and is designed as a professional embedded driver suitable for firmware development, industrial projects, and interview demonstrations.

---

# Features

- Timer Initialization
- Timer Deinitialization
- Timer Start
- Timer Stop
- Interrupt Enable
- Interrupt Disable
- Callback Registration
- Counter Read
- Counter Write
- Blocking Delay
- Running Status
- Static Memory Design
- Register Level Programming
- MISRA Friendly

---

# Supported Timers

| Timer | Supported |
|-------|-----------|
| TIM2 | ✔ |
| TIM3 | ✔ |
| TIM4 | ✔ |
| TIM5 | ✔ |

---

# Driver Architecture

```
Application Layer
        │
        ▼
Timer Driver
(timer.c)
        │
        ▼
STM32 Timer Registers
        │
        ▼
Hardware Timer Peripheral
```

---

# Public APIs

### Initialization

```c
TIMER_Init()
TIMER_DeInit()
```

### Control

```c
TIMER_Start()
TIMER_Stop()
```

### Interrupts

```c
TIMER_EnableInterrupt()
TIMER_DisableInterrupt()
TIMER_IRQHandler()
```

### Callback

```c
TIMER_RegisterCallback()
```

### Counter

```c
TIMER_GetCounter()
TIMER_SetCounter()
```

### Delay

```c
TIMER_Delay()
```

### Status

```c
TIMER_IsRunning()
```

---

# Design Goals

- Modular Driver Design
- Layered Architecture
- Hardware Abstraction
- Register-Level Programming
- Production Quality
- Static Memory Allocation
- No Dynamic Memory Usage
- Low Runtime Overhead
- Easy Portability
- Future RTOS Integration

---

# Coding Standard

- Embedded C
- ANSI C Compatible
- C17 Compatible
- MISRA-C Friendly
- Professional Formatting
- GitHub Portfolio Ready

---

# Future Enhancements

- PWM Support
- Input Capture
- Output Compare
- DMA Transfers
- Encoder Mode
- Advanced Timer Support
- One Pulse Mode

---

## Project Status

**Module:** Timer Driver

**Status:** Completed

**Framework Progress:** 100%