# UART Driver

## Overview

The UART driver implements full-duplex serial communication using direct register programming.

The implementation supports polling, interrupts, callbacks and ring-buffer based reception.

---

## Features

- UART Initialization
- UART DeInitialization
- Blocking Transmission
- Blocking Reception
- String Transmission
- Integer Transmission
- Hexadecimal Transmission
- Floating Point Transmission
- Interrupt Support
- Callback Support
- Ring Buffer
- Error Handling
- Timeout Protection

---

## Supported UARTs

- USART1
- USART2
- USART6

---

## Public APIs

```c
UART_Init()

UART_DeInit()

UART_SendByte()

UART_ReadByte()

UART_SendString()

UART_SendNumber()

UART_SendHex()

UART_SendFloat()

UART_EnableInterrupt()

UART_DisableInterrupt()

UART_IRQHandler()

UART_RingBufferInit()

UART_RingBufferRead()

UART_RingBufferWrite()
```

---

## Driver Architecture

```
Application

↓

UART Driver

↓

USART Registers

↓

STM32 Hardware
```

---

## Supported Baud Rates

- 9600
- 19200
- 38400
- 57600
- 115200

---

## Design Goals

- Professional API
- Interrupt Driven
- High Reliability
- Production Ready