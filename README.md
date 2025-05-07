# ChronoSync – Embedded Digital Clock

**Course**: EE3427 Embedded Systems  
**Semester**: 242  
**University**: Ho Chi Minh City University of Technology  
**Group**: TT01 – Group 01  
**Members**: Nguyễn Duy Ngọc, Bùi Đình Trung Nam, Nguyễn Hùng Minh

---

## 📌 Overview

ChronoSync is a compact, low-power embedded digital clock system built on the STM32F103C8T6 microcontroller. It features:

- Real-time timekeeping with ±2 ppm accuracy (DS3231)
- E-Ink display for ultra-low power static UI
- Alarm scheduling (up to 10 configurable alarms)
- Button-based and Bluetooth-based configuration
- Mobile app interface built with MIT App Inventor
- Persistent EEPROM-based alarm storage

---

## 🧩 Project Objectives

- Integrate real-time hardware modules using I²C, SPI, UART
- Optimize for low-power and battery-operated design
- Provide physical and mobile-based time/alarm configuration
- Ensure modular firmware and maintainability
- Design a compact PCB with future 3D enclosure support

---

## ⚙️ System Features

### ✅ Implemented

- Real-time clock display and update every second
- Time and alarm configuration via 5 physical buttons
- View and edit alarm slots with ON/OFF toggle
- Bluetooth mobile app for time and alarm setting
- Alarm match detection and buzzer melody
- Power-efficient E-Ink display
- EEPROM alarm persistence across resets
- System option to clear all alarms

### 🚧 Pending/Partially Implemented

- Button long-press for continuous input adjustment
- Factory reset, display test mode
- Battery holder integration and battery percentage
- Real-time update and editing from mobile app
- Final enclosure (currently carton prototype)
- Display UI refinement

---

## 📐 Hardware Overview

| Component        | Description                                       |
|------------------|---------------------------------------------------|
| **MCU**          | STM32F103C8T6 (ARM Cortex-M3, 72 MHz)             |
| **RTC**          | DS3231SN – I²C, high-precision timekeeping        |
| **Display**      | 2.9” Waveshare E-Ink display – SPI                |
| **EEPROM**       | AT24C64D – 64Kbit, I²C                            |
| **Bluetooth**    | HC-05 module – UART                               |
| **Buttons**      | 5 tactile buttons – short & long press detection  |
| **Buzzer**       | 9x4.2mm, 80 dB with transistor driver             |
| **Power**        | Buck converter TPS54231 + AMS1117-3.3V regulator  |

---

## 📱 Mobile App

- Developed using **MIT App Inventor**
- Connects via **Bluetooth HC-05**
- Supports:
  - Time sync & setup
  - Alarm configuration (DoW or DoM modes)
  - Real-time communication with STM32

---

## 🛠️ Development Tools

- **Hardware**: Altium Designer, Oscilloscope, Multimeter
- **Firmware**: STM32CubeIDE, STM32CubeMX, ST-Link Utility
- **App**: MIT App Inventor
- **Version Control**: GitHub

---

## 📈 Performance Highlights

| Constraint           | Status  | Description                          |
|----------------------|---------|--------------------------------------|
| C-1.1 ±3 ppm Accuracy| ✅ Met   | DS3231 TCXO verified over 24 hrs     |
| C-2.1 Data Retention | ✅ Met   | EEPROM retains data post power-loss  |
| C-3.1 Input <30 ms   | ✅ Met   | Short press detected ~20–30 ms       |
| C-5.1 Size ≤ 15x15x8cm| ✅ Met  | PCB and layout within limits         |

*See report for unverified constraints.*

---

## 📁 Folder Structure

```bash
.
├── 1. Documents/                 # Figures, planning documents, technical references
├── 2. Hardware Design/           # Schematic and PCB layout files (Altium Designer project)
├── 3. Firmware Design/           # STM32CubeIDE project for embedded firmware
├── 4. Mobile Application Design/ # MIT App Inventor source files and exported .aia/.apk packages
├── 5. Report                     # # Final formatted report for submission, figures, and exported PDF
├── .vscode/                      # VSCode workspace settings
├── README.md                     # Project overview and documentation
