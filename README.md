# HX11 Power Systems Firmware

**UC Irvine HyperXite | 2025-2026 Season**

---

## About UC Irvine HyperXite

[UC Irvine HyperXite](https://hyperxite.com) is the University of California, Irvine's student-led hyperloop engineering team. Each year, approximately 40 undergraduate members design, build, and test a small-scale hyperloop pod featuring linear induction motor propulsion and autonomous control, while conducting parallel R&D on electromagnetic levitation for future integration.

## What This Repository Contains

This is the STM32H753ZI firmware for the high-voltage power subsystem on the HX11 pod. It handles communication with the three devices that make up the pod's high-voltage bus:

- **Kelly KHA32020I Variable Frequency Drive (VFD)**, which drives the double-sided linear induction motor (DSLIM) via field-oriented control (FOC)
- **Orion BMS 2**, which reports cell voltages and pack state of charge
- **Bender iso175 Insulation Monitoring Device (IMD)**, which continuously measures insulation resistance between the HV bus and chassis ground

The firmware collects its data into a set of global telemetry variables that the rest of the control system processes.

This module is one piece of a larger codebase. It will merge into the HX11 master firmware, which features FreeRTOS task scheduling, a finite state machine with safety interlocks, INA260 current/voltage sensing, thermistor readout, SD card logging, and an ESP32-based telemetry GUI. The controls PCB that ties all of this together is a custom 4-layer board fabricated by [PCBWay](https://www.pcbway.com/), who is sponsoring HyperXite's hardware for this year's pod iteration.

## CAN Protocol

Each device on the HV bus broadcasts classic 8-byte frames. The firmware parses them by ID in a switch/case dispatcher.

### Kelly VFD

**0x10F8109A: Drive Status**

| Byte | Field | Unit | Notes |
|:-----|:------|:-----|:------|
| 1 | Speed LSB | 1 RPM/bit | Combined with byte 2 for 16-bit speed |
| 2 | Speed MSB | 1 RPM/bit | |
| 3 | Error Code | - | See Kelly VFD datasheet, Table 1 |

**0x10F8108D: Electrical and Thermal**

| Byte | Field | Unit |
|:-----|:------|:-----|
| 0-1 | Battery Voltage | 0.1 V/bit |
| 2-3 | Motor Current | 0.1 A/bit |
| 4-5 | Motor Temperature | 0.1 C/bit |
| 6-7 | Controller Temperature | 0.1 C/bit |

### Orion BMS 2

**0x10DEADBE: Pack Status**

| Byte | Field | Unit |
|:-----|:------|:-----|
| 0-1 | Lowest Cell Voltage | mV |
| 2-3 | Average Cell Voltage | mV |
| 4-5 | Highest Cell Voltage | mV |
| 6 | Pack State of Charge | % |
| 7 | Test Counter | Incrementing heartbeat |

### Bender iso175 IMD

**0x18FF01F4: Insulation Status**

| Byte | Field | Notes |
|:-----|:------|:------|
| 0-1 | Insulation Resistance | kOhm between HV bus and chassis |
| 2 | ISO Status | 0x00-0xFE reliability scale (0xFE = most reliable) |
| 3 | IMD Counter | Should increment continuously - stall indicates fault |
| 4-5 | Warnings | Bitfield of active warning conditions |
| 6 | Device Activity | Operational status byte |


### Throttle Control

The DAC outputs a 0-3.3V analog signal on PA5. An MCP4725 breakout on the PCBWay-fabricated controls board scales this to the 0-5V range the Kelly VFD expects on its analog throttle input. Three functions handle throttle behavior:

- `throttleTest()` ramps through the full 12-bit range (4096 steps) in both directions for bench verification.
- `accelerate()` ramps monotonically from 0V to full scale.
- `killThrottle()` writes 0 to the DAC immediately for emergency shutdown.

## Acknowledgments

[PCBWay](https://www.pcbway.com/) is sponsoring the fabrication of the HX11 controls carrier board. The board is a 4-layer design that connects the Nucleo-H753ZI to every sensor and actuator on the pod: dual CAN buses for high-voltage device communication, five I2C sensors for current/voltage/motion data, eight thermistor channels for thermal monitoring, an I2C DAC for throttle output, SDMMC for SD card logging, and UART for ESP32 wireless telemetry. PCBWay's fabrication quality and turnaround time let a student team iterate on this board within a few months of our Demo Day deadline. We are incredibly grateful for their support of student engineering!

## License

This project is developed as part of UC Irvine HyperXite's academic and competition activities. Please contact the team for licensing inquiries.
