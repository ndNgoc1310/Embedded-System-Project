# ChronoSync: Embedded Digital Clock  

## Overview  
**ChronoSync** is an energy-efficient **digital clock** designed with **real-time clock (RTC) synchronization, Bluetooth connectivity, and an E-Ink display**. It ensures **accurate timekeeping**, low power consumption, and remote configuration via a mobile app or PC.  

## Features  
- **Precise Timekeeping** – Uses **DS3231 RTC** with minimal drift.  
- **E-Ink Display** – Power-efficient and highly readable.  
- **Bluetooth Connectivity** – Remote time sync and alarm control.  
- **Multiple Alarms** – Stores alarms with EEPROM retention.  
- **Power Optimization** – Efficient voltage regulation and sleep modes.  

## System Architecture  
ChronoSync is powered by an **STM32 microcontroller**, interfacing with key peripherals:  
- **Display** – E-Ink LCD via **SPI**.  
- **Wireless** – Bluetooth (HC-05) via **UART**.  
- **Real-Time Clock** – DS3231 via **I2C**.  
- **User Interaction** – Buttons (GPIO) & Buzzer (GPIO).  
- **Memory** – EEPROM for non-volatile data storage.  

### **Block Diagram**  
![Block-diagram](https://github.com/user-attachments/assets/aa5ef7aa-f239-42a0-bf8f-e37bf8ad20eb)

## Setup & Compilation  
### **Requirements**  
- **STM32CubeIDE** or **Keil uVision** (for firmware development)  
- **ST-Link** or **USB-UART** (for flashing/debugging)  
- **Embedded C libraries** (HAL, CMSIS)  

### **Building & Flashing**  
1. Clone the repository:  
   ```sh
   git clone https://github.com/ndNgoc1310/Embedded-System-Project.git
   cd Embedded-System-Project
