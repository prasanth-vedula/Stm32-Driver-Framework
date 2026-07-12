# Interrupt Flow

## Overview

This document illustrates the interrupt execution flow implemented by the driver framework.

---

# Flow Diagram

```
Peripheral

↓

Interrupt Flag Set

↓

NVIC

↓

Interrupt Service Routine

↓

Driver IRQ Handler

↓

Clear Interrupt Flag

↓

Execute Callback

↓

Return to Application
```

---

# Execution Sequence

1. Peripheral generates an interrupt.

2. NVIC forwards execution to the ISR.

3. ISR calls the corresponding Driver IRQ Handler.

4. Driver validates the interrupt source.

5. Driver clears the interrupt flag.

6. Registered callback is executed.

7. Program returns to the interrupted application.

---

# Design Goals

- Low latency
- Clear separation of layers
- Reusable callbacks
- Simple ISR implementation