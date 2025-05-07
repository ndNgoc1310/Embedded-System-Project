# **Naming Conventions for Embedded System Project**

This document outlines the naming conventions to be followed throughout the project to ensure consistency, readability, and maintainability of the code.

---

## **1. General Guidelines**
- Use **descriptive names** that clearly indicate the purpose of the variable, function, or constant.
- Use **lowercase with underscores (`_`)** for global variables.
- Use **camelCase** for local variables.
- Use **Upper_Camel_Snake_Case** (uppercase with the first letter of each word separated by underscores) for function names.
- Use **ALL_CAPS_WITH_UNDERSCORES** for macros and constants.
- Use **ALL_CAPS** for struct names.
- Avoid abbreviations unless they are widely understood (e.g., `ADC`, `UART`, `RTC`).

---

## **2. Variables**
### **2.1 Global Variables**
- Use **lowercase with underscores (`_`)** for global variables.
  - Example: `rtc_trigger_flag`, `adc_data`, `uart_rx_flag`.
- Prefix global variables with the module or purpose if necessary to avoid naming conflicts.
- Use `volatile` for variables shared between interrupts and the main program.

### **2.2 Local Variables**
- Use **camelCase** for local variables.
  - Example: `setTime`, `getTime`, `ctrlTime`.

### **2.3 Flags**
- Use the suffix `_flag` for boolean variables that act as flags.
  - Example: `rtc_trigger_flag`, `adc_valid_flag`, `uart_rx_flag`.

### **2.4 Structs**
- Use **ALL_CAPS** for struct names.
  - Example: `TIME`, `ALARM`.
- Use **lowercase with underscores (`_`)** for struct members.
  - Example: `second`, `minute`, `hour`.

---

## **3. Functions**
- Use **Upper_Camel_Snake_Case** for function names.
- Use a verb or action word as the prefix to indicate the function's purpose.
  - Example: `Time_Init`, `Time_Set`, `Alarm_Check`.
- For module-specific functions, prefix the function name with the module name.
  - Example: `Time_Get`, `Alarm_Set`.

---

## **4. Constants and Macros**
- Use **ALL_CAPS_WITH_UNDERSCORES** for constants and macros.
- Prefix constants with the module or purpose.
  - Example: `DS3231_ADDRESS`, `EEPROM_ADDR`.
- Use meaningful names that describe the constant's purpose.

---

## **5. File-Specific Conventions**
- Use `USER CODE BEGIN` and `USER CODE END` comments to mark user-defined sections.
- Group related variables and functions together for better readability.

---

## **6. Additional Notes**
- Follow the conventions consistently across all files in the project.
- Use comments to explain the purpose of variables, functions, and constants where necessary.
- Avoid using magic numbers; define them as constants with meaningful names.

By adhering to these conventions, the codebase will remain clean, consistent, and easy to understand for all contributors.