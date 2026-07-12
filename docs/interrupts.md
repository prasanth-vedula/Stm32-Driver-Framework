# Interrupt System

## Overview

The driver framework supports interrupt-driven peripheral operation for UART, SPI, I2C, TIMER, PWM and ADC.

Each peripheral provides callback registration to separate application logic from hardware handling.

---

# Supported Interrupt Sources

- UART RX
- UART TX
- SPI Transfer Complete
- I2C Transfer Complete
- Timer Update Event
- PWM Pulse Event
- ADC Conversion Complete

---

# Interrupt Processing

```
Peripheral Event

↓

Interrupt Generated

↓

ISR

↓

Driver IRQ Handler

↓

User Callback
```

---

# Driver APIs

```c
UART_IRQHandler()

SPI_IRQHandler()

I2C_IRQHandler()

TIMER_IRQHandler()

PWM_IRQHandler()

ADC_IRQHandler()
```

---

# Callback Mechanism

Applications register callback functions during initialization.

The driver invokes the callback automatically when the interrupt event occurs.

---

# Benefits

- Event-driven execution
- Improved responsiveness
- Modular software
- Reduced polling