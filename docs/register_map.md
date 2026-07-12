# STM32F407VG Register Map

## Overview

This document summarizes the peripheral register map used by the STM32 Driver Framework.

The framework accesses peripherals directly through memory-mapped registers without using STM32 HAL or LL libraries.

---

# Peripheral Base Addresses

| Peripheral | Base Address |
|------------|-------------:|
| GPIOA | 0x40020000 |
| GPIOB | 0x40020400 |
| GPIOC | 0x40020800 |
| GPIOD | 0x40020C00 |
| GPIOE | 0x40021000 |
| RCC | 0x40023800 |
| USART1 | 0x40011000 |
| USART2 | 0x40004400 |
| USART6 | 0x40011400 |
| SPI1 | 0x40013000 |
| SPI2 | 0x40003800 |
| SPI3 | 0x40003C00 |
| I2C1 | 0x40005400 |
| I2C2 | 0x40005800 |
| I2C3 | 0x40005C00 |
| TIM2 | 0x40000000 |
| TIM3 | 0x40000400 |
| TIM4 | 0x40000800 |
| TIM5 | 0x40000C00 |

---

# Driver Access Layer

```
Application

↓

Driver API

↓

Register Structures

↓

Memory Mapped Registers

↓

Peripheral Hardware
```

---

# Register Access

The framework accesses all peripherals using C structures mapped directly to hardware registers.

No HAL functions are used.

---

# Advantages

- Faster execution
- Lower memory usage
- Full hardware control
- Portable architecture
- Educational clarity