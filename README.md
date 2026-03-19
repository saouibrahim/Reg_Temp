# Reg_Temp — Thermal Supervision System

A C project for monitoring and regulating indoor temperature via USB, with both a hardware (STM32/FTDI) mode and a software simulator mode.

---

## Overview

Reg_Temp reads interior and exterior temperatures, applies a heating regulation law (on/off or PID), sends commands to a heating system, and writes state data for a companion visualisation GUI (`gestiontemp.exe`). The setpoint is read live from a file, allowing external adjustment without restarting the program.

---

## Project Structure

```
Reg_Temp/
├── main_sim.c          # Entry point — simulator mode
├── main_usb.c          # Entry point — USB/hardware mode
├── simulateur.c        # Thermal simulator (replaces STM32 hardware)
├── regulation.c        # On/off and PID regulation laws
├── consigne.c          # Reads setpoint from consigne.txt
├── visualisationT.c    # Writes temperatures to data.txt
├── visualisationC.c    # Writes heater state to data.txt
├── releve.c            # Reads temperatures from STM32 via USB (FTDI)
├── commande.c          # Sends heating command to STM32 via USB (FTDI)
├── include/
│   ├── define.h        # Shared types (temp_t) and PID constants
│   ├── simulateur.h
│   ├── regulation.h
│   ├── consigne.h
│   ├── visualisationT.h
│   ├── visualisationC.h
│   ├── releve.h
│   ├── commande.h
│   └── ftd2xx.h        # FTDI D2XX driver API
├── consigne.txt        # Setpoint file (edit while running)
├── data.txt            # Output state file (read by GUI)
├── trace.txt           # Simulation trace log
└── Makefile
```

---

## Building

Requires `gcc` and the FTDI D2XX library (`ftd2xx.lib`) on Windows.

```bash
# Build both executables
make

# Build simulator only
make sim.exe

# Build USB/hardware executable only
make usb.exe

# Clean generated files
make clean
```

The Makefile compiles all `.c` files to `.o` objects under `./include` as the header search path, then links them into `sim.exe` or `usb.exe`.

---

## Running

### Simulator Mode

Runs a thermal model instead of real hardware. Useful for testing regulation without a physical device.

```bash
./sim.exe
```

Initial conditions: interior temperature 15 °C, exterior 10 °C. The setpoint defaults to 19 °C and is re-read from `consigne.txt` each iteration.

### USB / Hardware Mode

Connects to an STM32 microcontroller via FTDI USB at 115200 baud, 8N1, no flow control.

```bash
./usb.exe
```

The program opens the first available FTDI device, reads temperatures from the STM32 each second, computes the heating command, and sends it back.

---

## Configuration

### Setpoint — `consigne.txt`

Edit this file at any time while the program is running to change the target temperature:

```
30.0
```

Setting the value to **≤ 5.0 °C** commands 0% power and stops the program cleanly.

### Regulation Mode

The regulation mode is set by the `regul` variable in `main_sim.c` or `main_usb.c`:

| Value | Mode |
|-------|------|
| `1`   | On/Off (bang-bang) |
| `2`   | PID (default) |

### PID Constants — `include/define.h`

```c
#define KP  1.1
#define KI  0.2
#define KD  0.15
#define dt  10.0   // seconds per simulation step
```

---

## Data Flow

```
consigne.txt
     │ (setpoint)
     ▼
consigne() ──► regulation_1ou0() or regulation_PID()
                          │ (command 0–100%)
          ┌───────────────┤
          │               ▼
     sim mode:       usb mode:
   simCalc()        commande() ──► STM32 via FT_Write
          │
          ▼
   temp_t (Tint, Text)
          │
          ├──► visualisationT()  ┐
          └──► visualisationC()  ├──► data.txt  ──► gestiontemp.exe (GUI)
                                 ┘
```

---

## Output Files

### `data.txt`

Written each cycle; read by the external GUI:

```
true        ← heater state (true/false)
22.56       ← interior temperature (°C)
3.96        ← exterior temperature (°C)
```

### `trace.txt`

Simulation log written by the simulator. Columns: `time(s)`, `power(%)`, `Text(°C)`, `Tint(°C)`.

---

## USB Protocol — STM32 Communication

### Temperature Reception (`releve.c`)

The STM32 sends 6 bytes per cycle — 3 for exterior temperature, 3 for interior. Each byte encodes a sensor ID in the top nibble and a 4-bit data chunk in the bottom nibble:

| Top nibble | Meaning |
|------------|---------|
| `0x0` | Exterior SOT bits 11–8 |
| `0x1` | Exterior SOT bits 7–4 |
| `0x2` | Exterior SOT bits 3–0 |
| `0x8` | Interior SOT bits 11–8 |
| `0x9` | Interior SOT bits 7–4 |
| `0xA` | Interior SOT bits 3–0 |

Temperature conversion (SHT sensor): `T = −39.64 + 0.04 × SOT`

### Command Transmission (`commande.c`)

A single byte is sent: `(power% × 127) / 100`, mapping 0–100% to 0–127.

---

## Dependencies

- **gcc** — C compiler
- **FTDI D2XX library** (`ftd2xx.lib` / `ftd2xx.h`) — USB communication in hardware mode
- **Windows** — required for USB mode (FTDI D2XX is Windows-only in this configuration)
- Simulator mode compiles and runs on any platform with `gcc` and POSIX `usleep`