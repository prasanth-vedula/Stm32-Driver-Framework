# Analog-to-Digital Converter (ADC) Driver

## Overview

The ADC driver provides a production-quality register-level implementation for the STM32F407VG Analog-to-Digital Converter peripheral without using STM32 HAL or LL libraries.

The driver supports single-channel analog input conversion, interrupt-driven conversion, configurable resolution, configurable sampling time, and continuous conversion mode.

---

# Features

- Register-Level Programming
- Single Conversion Mode
- Continuous Conversion Mode
- 12-bit Resolution
- 10-bit Resolution
- 8-bit Resolution
- 6-bit Resolution
- Configurable Sampling Time
- Interrupt Support
- Callback Registration
- Error Handling
- Timeout Protection
- Static Memory Allocation
- MISRA-Friendly Design

---

# Driver Architecture

```text
Application

↓

ADC Driver APIs

↓

Register Layer

↓

STM32 ADC Peripheral
```

---

# Configuration Structure

```c
typedef struct
{
    ADC_Mode_t mode;
    ADC_Resolution_t resolution;
    ADC_SampleTime_t sampleTime;
    uint8_t channel;

} ADC_Config_t;
```

---

# Supported Modes

## Single Conversion

Converts one analog sample each time the ADC is started.

Suitable for:

- Battery Voltage
- Potentiometer Reading
- Temperature Sensor

---

## Continuous Conversion

ADC continuously converts analog values until stopped.

Suitable for:

- Sensor Monitoring
- Audio Sampling
- Data Acquisition

---

# Supported Resolutions

| Resolution | Maximum Value |
|------------|--------------:|
| 12-bit | 4095 |
| 10-bit | 1023 |
| 8-bit | 255 |
| 6-bit | 63 |

---

# Sampling Times

- 3 Cycles
- 15 Cycles
- 28 Cycles
- 56 Cycles
- 84 Cycles
- 112 Cycles
- 144 Cycles
- 480 Cycles

---

# Public APIs

## Initialization

```c
ADC_Init()

ADC_DeInit()
```

---

## Conversion Control

```c
ADC_Start()

ADC_Stop()

ADC_Read()
```

---

## Interrupt APIs

```c
ADC_EnableInterrupt()

ADC_DisableInterrupt()

ADC_IRQHandler()
```

---

## Callback API

```c
ADC_RegisterCallback()
```

---

## Status APIs

```c
ADC_GetState()

ADC_GetError()
```

---

# Typical Workflow

```text
Initialize ADC

↓

Configure Channel

↓

Start Conversion

↓

Wait for End Of Conversion

↓

Read ADC Value

↓

Process Data
```

---

# Interrupt Flow

```text
ADC Conversion Complete

↓

EOC Flag Set

↓

ADC Interrupt

↓

ADC_IRQHandler()

↓

User Callback
```

---

# Example Usage

```c
ADC_HandleTypeDef hadc;

ADC_Init(&hadc);

ADC_Start(&hadc);

ADC_Read(&hadc, &adcValue);

ADC_Stop(&hadc);
```

---

# Error Handling

The driver detects the following conditions:

- Invalid Pointer
- ADC Timeout
- ADC Overrun
- Invalid Configuration

---

# Applications

- Temperature Sensors
- Light Sensors
- Pressure Sensors
- Potentiometers
- Battery Monitoring
- Analog Joystick
- Industrial Data Acquisition
- Embedded Control Systems

---

# Advantages

- Direct Register Programming
- No HAL Dependency
- Lightweight Implementation
- Portable Design
- Fast Execution
- Low Memory Footprint
- Interrupt Support
- Easy Integration

---

# Complexity

| Operation | Complexity |
|-----------|-----------:|
| Initialization | O(1) |
| Start Conversion | O(1) |
| Read Conversion | O(1) |
| Stop Conversion | O(1) |

---

# File Structure

```text
Drivers
│
├── Inc
│   └── adc.h
│
└── Src
    └── adc.c
```

---

# Future Improvements

- Multi-Channel Scan Mode
- DMA Support
- Injected Channels
- Analog Watchdog
- Dual ADC Mode
- Triple ADC Mode
- Calibration Support
- Low Power ADC

---

# Status

- ✅ Production Ready
- ✅ Register-Level Implementation
- ✅ STM32F407VG Compatible
- ✅ MISRA-Friendly
- ✅ Interrupt Support
- ✅ Callback Support
- ✅ Static Memory Only
- ✅ Professional Driver Architecture
- ✅ Interview Ready
