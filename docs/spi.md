# SPI Driver

## Overview

The SPI driver provides register-level Serial Peripheral Interface communication for STM32F407VG.

The driver supports master mode communication with blocking transfer APIs and interrupt support.

---

## Features

- SPI Initialization
- SPI DeInitialization
- Master Mode
- Full Duplex Communication
- Blocking Transfer
- Interrupt Support
- Callback Registration
- Error Handling
- Timeout Protection

---

## Supported SPI Peripherals

- SPI1
- SPI2
- SPI3

---

## Public APIs

```c
SPI_Init()

SPI_DeInit()

SPI_Transmit()

SPI_Receive()

SPI_TransmitReceive()

SPI_EnableInterrupt()

SPI_DisableInterrupt()

SPI_IRQHandler()

SPI_RegisterCallback()
```

---

## Driver Architecture

```
Application

↓

SPI Driver

↓

SPI Registers

↓

STM32 Hardware
```

---

## Supported Features

- Master Mode
- 8-bit Data
- 16-bit Data
- MSB First
- LSB First
- CPOL Configuration
- CPHA Configuration

---

## Design Goals

- Register-Level Driver
- Reusable
- MISRA Friendly
- Static Memory
- Production Ready