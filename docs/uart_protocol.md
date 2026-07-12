# UART Protocol

## Overview

The UART driver implements asynchronous serial communication using the USART peripherals of the STM32F407VG.

---

# Frame Format

```
Start Bit

↓

Data Bits (8 or 9)

↓

Optional Parity

↓

Stop Bit(s)
```

---

# Supported Configuration

- 8-bit Word Length
- 9-bit Word Length
- No Parity
- Even Parity
- Odd Parity
- 1 Stop Bit
- 2 Stop Bits

---

# Supported Baud Rates

- 9600
- 19200
- 38400
- 57600
- 115200

---

# Communication Flow

```
Application

↓

UART Driver

↓

USART Registers

↓

TX Pin

↓

Serial Line

↓

RX Pin

↓

USART Registers

↓

Application
```

---

# Driver Features

- Blocking Communication
- Interrupt Communication
- Ring Buffer Reception
- Callback Support
- Error Detection
- Timeout Handling

---

# Applications

- Debug Console
- PC Communication
- GPS Modules
- GSM Modules
- Bluetooth Modules
- Sensor Interfaces