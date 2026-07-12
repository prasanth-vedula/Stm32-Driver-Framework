# I2C Driver

## Overview

The I2C driver provides register-level Inter-Integrated Circuit (I2C) communication for the STM32F407VG microcontroller.

The driver supports master mode communication using direct register programming with polling, interrupt, callback and error handling mechanisms.

---

## Features

- I2C Initialization
- I2C DeInitialization
- Master Transmit
- Master Receive
- Interrupt Support
- Callback Registration
- Error Handling
- Timeout Protection
- Register-Level Programming
- MISRA-Friendly Design

---

## Supported Peripherals

- I2C1
- I2C2
- I2C3

---

## Public APIs

```c
I2C_Init()

I2C_DeInit()

I2C_MasterTransmit()

I2C_MasterReceive()

I2C_EnableInterrupt()

I2C_DisableInterrupt()

I2C_IRQHandler()

I2C_RegisterCallback()

I2C_GetError()

I2C_ClearError()
```

---

## Driver Architecture

```
Application

↓

I2C Driver

↓

I2C Registers

↓

STM32 Hardware
```

---

## Supported Modes

- Master Mode
- 7-bit Addressing
- 10-bit Addressing
- Standard Mode (100 kHz)
- Fast Mode (400 kHz)

---

## Design Goals

- Modular
- Portable
- Register-Level
- Production Ready
- Static Memory