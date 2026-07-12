# GPIO Driver

## Overview

The GPIO driver provides register-level configuration and control of General Purpose Input/Output peripherals for the STM32F407VG microcontroller.

The driver is implemented without STM32 HAL or LL libraries and follows a modular, reusable architecture.

---

## Features

- GPIO Initialization
- GPIO DeInitialization
- Input Mode
- Output Mode
- Alternate Function Mode
- Analog Mode
- Pull-up / Pull-down Configuration
- Output Type Configuration
- Speed Configuration
- Pin Read
- Pin Write
- Pin Toggle
- Register-Level Access
- MISRA-Friendly Design

---

## Supported GPIO Ports

- GPIOA
- GPIOB
- GPIOC
- GPIOD
- GPIOE

---

## Public APIs

```c
GPIO_Init()

GPIO_DeInit()

GPIO_WritePin()

GPIO_ReadPin()

GPIO_TogglePin()
```

---

## Driver Architecture

```
Application

↓

GPIO Driver

↓

GPIO Registers

↓

STM32 Hardware
```

---

## Error Handling

- NULL Pointer Checking
- Invalid Pin Validation
- Invalid Port Validation
- Invalid Mode Validation

---

## Design Goals

- Portable
- Reusable
- Register-Level
- Static Memory
- Production Ready