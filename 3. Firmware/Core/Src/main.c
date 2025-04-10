/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  * 
  * This project is developed by Nguyen Hung Minh, Bui Dinh Trung Nam, and Nguyen Duy Ngoc
  * (Students at Vietnam National University, Ho Chi Minh City - University of Technology).
  * 
  * Please read and follow the naming conventions in naming_conventions.md before editing this file.
  * 
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stdbool.h"
#include "stdio.h"
#include "string.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// Structure of 7 one-byte unsigned characters to store time values
typedef struct 
{
	uint8_t second;       // Seconds: 0-59
	uint8_t minute;       // Minutes: 0-59
	uint8_t hour;         // Hours: 0-23    
	uint8_t dayofweek;    // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday)
	uint8_t dateofmonth;  // Date of the month: 1-31
	uint8_t month;        // Month: 1-12
	uint8_t year;         // Year: 0-99 (0 = 2000, 1 = 2001, ..., 99 = 2099)
} TIME;

// Structure of 4 one-byte unsigned characters to store alarm values
typedef struct 
{
	uint8_t second;   // Seconds: 0-59 (MSB = 1 for ON, 0 for OFF)
	uint8_t minute;   // Minutes: 0-59
	uint8_t hour;     // Hours: 0-23
	uint8_t dow_dom;  // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday), or Date of the month: 1-31
                    //    [Bit 7] = 1, [Bit 6] = 0: Date of the month
                    //    [Bit 7] = 1, [Bit 6] = 1: Day of the week
                    //    [Bit 7] = 0, [Bit 6] = X: Not used
} ALARM;

// Debugging: Structure to track the values being selected for modification
typedef struct
{
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t dow_dom;
  uint8_t dy_dt;
  bool    on_off;
  uint8_t slot;
} DEBUG_ALARM_SELECT;

// Enum for button debounce states
typedef enum {
  BUTTON_STATE_HIGH,    // Button is not pressed (HIGH state)
  BUTTON_STATE_WAITING, // Button is pressed (LOW state) and waiting for debounce delay
  BUTTON_STATE_LOW      // Button is pressed (LOW state)
} BUTTON_STATE;

// Struct representing each button with debouncing state
typedef struct {
  GPIO_TypeDef    *gpio_port; // GPIO port of the button      
  uint16_t        gpio_pin;   // GPIO pin of the button
  BUTTON_STATE    state;      // Current state of the button
  uint32_t        start_tick; // Start time of the debounce timer
  volatile bool   int_flag;   // Set when button interrupt is triggered
  volatile bool   press_flag; // Set when button is pressed
} BUTTON;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// Slave address of DS3231 RTC module
#define DS3231_ADDR 0xD0

// I2C handle for DS3231 RTC module
#define DS3231_I2C &hi2c1

// Slave address of AT24C64D EEPROM module
#define EEPROM_ADDR 0xA0

// I2C handle for AT24C64D EEPROM module
#define EEPROM_I2C &hi2c1

// EEPROM page size (in bytes)
#define EEPROM_PAGE_SIZE 32

// EEPROM page number (for 64K EEPROM)
#define EEPROM_PAGE_NUM  256

// Number of alarms in the EEPROM module
#define ALARM_SLOT_NUM 10 

// Debounce threshold in milliseconds
#define DEBOUNCE_DELAY_MS 15 

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* RTC & EEPROM ====================================*/
// Variable to store the time values received from the RTC module every second
volatile TIME time_get_data = {0};

// Array to store the alarm values received from the EEPROM module
volatile ALARM alarm_get_data[10] = {0}; 

// Variable to store the alarm values received from the EEPROM module
volatile ALARM alarm_check_data = {0};

// Flag for RTC trigger external interrupt (RTC Interrupt Flag) on PB4 (Activated every second)
volatile bool rtc_int_flag = false;

// Debugging: Track the number of alarm activations
uint8_t debug_alarm_activate_ctr = 0;

/* BUTTON ========================================*/
// Debugging: Track if the button is pressed or not
volatile uint8_t debug_button_counter[5] = {0};

// Global instances of DEBOUNCE_BUTTON for each button
BUTTON button1 = {GPIOB, BUTTON1_IN12_Pin, BUTTON_STATE_HIGH, 0, false, false};
BUTTON button2 = {GPIOB, BUTTON2_IN13_Pin, BUTTON_STATE_HIGH, 0, false, false};
BUTTON button3 = {GPIOB, BUTTON3_IN14_Pin, BUTTON_STATE_HIGH, 0, false, false};
BUTTON button4 = {GPIOB, BUTTON4_IN15_Pin, BUTTON_STATE_HIGH, 0, false, false};
BUTTON button5 = {GPIOA, BUTTON5_IN8_Pin, BUTTON_STATE_HIGH, 0, false, false};

// Debugging: Track the values of alarm being selected for modification
DEBUG_ALARM_SELECT debug_alarm_select = {0, 0, 0, 0, 0, false, 0};

/* ADC ==========================================*/
// Flag for ADC interrupt (ADC Valid Flag)
volatile bool adc_valid_flag = false;   

// Variable for ADC raw data
uint16_t adc_data;           

// Variable for battery percentage (raw data * 100 / 4095 (2^12))
uint16_t battery_percentage; 

/* UART ========================================*/
// Flag for UART interrupt (UART Receive Flag)
volatile bool uart_rx_flag = false;

// Variable for UART receive data: hour and minute
uint8_t uart_rx_data[2];

// Variable to store hour and minute values received from the UART module
uint8_t uart_rx_hour;
uint8_t uart_rx_minute;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

// Function to convert a normal decimal number to binary coded decimal (BCD)
uint8_t Dec_To_BCD(int val);

// Function to convert a binary coded decimal (BCD) value to a normal decimal number
int BCD_To_Dec(uint8_t val);

// Function to initialize RTC module
void Time_Init (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year);

// Function to initially set time to the RTC module through I2C interface (Run only once after reset the RTC)
void Time_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year);

// Function to get time from the RTC module through I2C interface
void Time_Get (volatile TIME *time_get_data);

// Function to control settings of the RTC module (Alarm 1)
void Time_Ctrl (uint8_t mode, uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, bool dy_dt);

// Function to write a single alarm to the EEPROM module
void Alarm_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, uint8_t dy_dt, bool on_off, uint8_t slot);

// Function to read a single alarm from the EEPROM module
void Alarm_Get (uint8_t slot, volatile ALARM *alarm_get_data);

// Function to clear a single alarm from the EEPROM module
void Alarm_Clear (uint8_t slot);

// // Function to check the alarms
void Alarm_Check (volatile ALARM *alarm_check_data, volatile TIME *time_get_data);

// Debounce handler function to be called in main loop
void Button_Debounce(BUTTON *button);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  // // Initialize RTC module (Run only once after reset the RTC module)
  // //    void Time_Init(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
  // Time_Init
  // (
  //    0, // Seconds: 0-59
  //   01, // Minutes: 0-59
  //   15, // Hours: 0-23
  //    5, // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday)
  //   10, // Date of the month: 1-31
  //    4, // Month: 1-12
  //   25  // Year: 0-99 (0 = 2000, 1 = 2001, ..., 99 = 2099)
  // );

  // // Clear all alarms in the EEPROM module
  // for (int i = 0; i < 10; i++)
  // {
  //   Alarm_Clear(i);
  // }

  // Store values of a single alarm to the next available address on the EEPROM module
  //    void Alarm_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, bool dy_dt, bool on_off, uint8_t slot)
  Alarm_Set
  (
    10,    // Seconds: 0-59
    20,    // Minutes: 0-59
    22,    // Hours: 0-23
     9,    // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday), or Date of the month: 1-31, or not used: 0
     0,    // Select: 0 = date of month, 1 = day of week, 2 = not used
    false, // true = ON, false = OFF 
     1     // Slot number of the alarm in the EEPROM module (0-9)
  );  

  // Initially retrieve the alarm values from the EEPROM module
  for (int i = 0; i <= 10; i++)
  {
    Alarm_Get(i, &alarm_get_data[i]);
  }

  // Initialize the UART module to receive data
  //    HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
  HAL_UART_Receive_IT(&huart1, uart_rx_data, 2);

  // Initialize the ADC module to monitor battery voltage
  //    HAL_ADC_Start_IT(ADC_HandleTypeDef *hadc);
  HAL_ADC_Start_IT(&hadc1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    // Check if the Button 1 Interrupt Flag is set (Button 1 Interrupt Flag) on PB12
    if (button1.int_flag)
    {
      // Handle button 1 press event
      //    void Button_Debounce(BUTTON *button)
      Button_Debounce(&button1);

      // Check if the button is pressed (LOW)
      if (button1.press_flag)
      {
        // Debugging: Track the number of button presses
        //    debug_button_counter[0]: parameter selection for an alarm
        //      0: seconds
        //      1: minutes
        //      2: hours
        //      3: day of the week, or date of the month
        //      4: selection: date of month, day of week, not used
        //      5: ON/OFF state of the alarm
        //      6: slot number of the alarm in the EEPROM module (0-9)
        
        if (debug_button_counter[0] < 6)
        {
          debug_button_counter[0] += 1;
        }
        else
        {
          debug_button_counter[0] = 0;
        }
      }
    
      // Reset the Button 1 Press Flag
      button1.press_flag = false;
    }

    // Check if the Button 2 Interrupt Flag is set (Button 2 Interrupt Flag) on PB13
    if (button2.int_flag)
    {
      // Handle button 2 press event
      //    void Button_Debounce(BUTTON *button)
      Button_Debounce(&button2);

      // Check if the button is pressed (LOW)
      if (button2.press_flag)
      {
        // Debugging: Track the number of button presses
        debug_button_counter[1] += 1;

        // Debugging: Increment the alarm value based on the button press count
        switch (debug_button_counter[0])
        {
          case 0:
            // Debugging: Increment the seconds value of the alarm
            if (debug_alarm_select.second < 59)
            {
              debug_alarm_select.second += 1;
            }
            else
            {
              debug_alarm_select.second = 0;
            }
          break;
          
          case 1:
            // Debugging: Increment the minutes value of the alarm
            if (debug_alarm_select.minute < 59)
            {
              debug_alarm_select.minute += 1;
            }
            else
            {
              debug_alarm_select.minute = 0;
            }
          break;

          case 2:
            // Debugging: Increment the hours value of the alarm
            if (debug_alarm_select.hour < 23)
            {
              debug_alarm_select.hour += 1;
            }
            else
            {
              debug_alarm_select.hour = 0;
            }
          break;

          case 3:
            // Debugging: Increment the day of the week or date of the month value of the alarm
            if (debug_alarm_select.dow_dom < 31)
            {
              debug_alarm_select.dow_dom += 1;
            }
            else
            {
              debug_alarm_select.dow_dom = 0;
            }
          break;

          case 4:
            // Debugging: Increment the selection value of the alarm (date of month, day of week, not used)
            if (debug_alarm_select.dy_dt < 2)
            {
              debug_alarm_select.dy_dt += 1;
            }
            else
            {
              debug_alarm_select.dy_dt = 0;
            }
          break;

          case 5:
            // Debugging: Toggle the ON/OFF state of the alarm
            debug_alarm_select.on_off = !debug_alarm_select.on_off;
          break;

          case 6:
            // Debugging: Increment the slot number of the alarm in the EEPROM module
           if (debug_alarm_select.slot < 9)
            {
              debug_alarm_select.slot += 1;
            }
            else
            {
              debug_alarm_select.slot = 0;
            }
          break;

          default:
            // No operation needed for default case
          break;
        }
      }
    
      // Reset the Button 2 Press Flag
      button2.press_flag = false;
    }

    // Check if the Button 3 Interrupt Flag is set (Button 3 Interrupt Flag) on PB14
    if (button3.int_flag)
    {
      // Handle button 3 press event
      //    void Button_Debounce(BUTTON *button)
      Button_Debounce(&button3);

      // Check if the button is pressed (LOW)
      if (button3.press_flag)
      {
        // Debugging: Track the number of button presses
        debug_button_counter[2] += 1;

        switch(debug_button_counter[0])
        {
          case 0:
            // Debugging: Decrement the seconds value of the alarm
            if (debug_alarm_select.second > 0)
            {
              debug_alarm_select.second -= 1;
            }
            else
            {
              debug_alarm_select.second = 59;
            }
          break;

          case 1:
            // Debugging: Decrement the minutes value of the alarm
            if (debug_alarm_select.minute > 0)
            {
              debug_alarm_select.minute -= 1;
            }
            else
            {
              debug_alarm_select.minute = 59;
            }
          break;

          case 2:
            // Debugging: Decrement the hours value of the alarm
            if (debug_alarm_select.hour > 0)
            {
              debug_alarm_select.hour -= 1;
            }
            else
            {
              debug_alarm_select.hour = 23;
            }
          break;

          case 3:
            // Debugging: Decrement the day of the week or date of the month value of the alarm
            if (debug_alarm_select.dow_dom > 0)
            {
              debug_alarm_select.dow_dom -= 1;
            }
            else
            {
              debug_alarm_select.dow_dom = 31;
            }
          break;

          case 4:
            // Debugging: Decrement the selection value of the alarm (date of month, day of week, not used)
            if (debug_alarm_select.dy_dt > 0)
            {
              debug_alarm_select.dy_dt -= 1;
            }
            else
            {
              debug_alarm_select.dy_dt = 2;
            }
          break;

          case 5:
            // Debugging: Toggle the ON/OFF state of the alarm
            debug_alarm_select.on_off = !debug_alarm_select.on_off;
          break;

          case 6:
            // Debugging: Decrement the slot number of the alarm in the EEPROM module
            if (debug_alarm_select.slot > 0)
            {
              debug_alarm_select.slot -= 1;
            }
            else
            {
              debug_alarm_select.slot = 9;
            }
          break;

          default:
            // No operation needed for default case
          break;
        }
      }
    
      // Reset the Button 3 Press Flag
      button3.press_flag = false;
    }

    // Check if the Button 4 Interrupt Flag is set (Button 4 Interrupt Flag) on PB15
    if (button4.int_flag)
    {
      // Handle button 4 press event
      //    void Button_Debounce(BUTTON *button)
      Button_Debounce(&button4);

      // Check if the button is pressed (LOW)
      if (button4.press_flag)
      {
        // Debugging: Track the number of button presses
        debug_button_counter[3] += 1;

        // Debugging: Store the alarm values to the EEPROM module
        //   void Alarm_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, uint8_t dy_dt, bool on_off, uint8_t slot)
        Alarm_Set
        (
          debug_alarm_select.second, // Seconds: 0-59
          debug_alarm_select.minute, // Minutes: 0-59
          debug_alarm_select.hour,   // Hours: 0-23
          debug_alarm_select.dow_dom,// Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday), or Date of the month: 1-31, or not used: 0
          debug_alarm_select.dy_dt,  // Select: 0 = date of month, 1 = day of week, 2 = not used
          debug_alarm_select.on_off, // true = ON, false = OFF 
          debug_alarm_select.slot    // Slot number of the alarm in the EEPROM module (0-9)
        );

        Alarm_Get(debug_alarm_select.slot, &alarm_get_data[debug_alarm_select.slot]);
      }
    
      // Reset the Button 4 Press Flag
      button4.press_flag = false;
    }

    // Check if the Button 5 Interrupt Flag is set (Button 5 Interrupt Flag) on PA8
    if (button5.int_flag)
    {
      // Handle button 5 press event
      //    void Button_Debounce(BUTTON *button)
      Button_Debounce(&button5);

      // Check if the button is pressed (LOW)
      if (button5.press_flag)
      {
        // Debugging: Track the number of button presses
        debug_button_counter[4] += 1;

        // Debugging: Clear all alarms in the EEPROM module
        for (int i = 0; i < 10; i++)
        {
          Alarm_Clear(i);
        }

        for (int i = 0; i <= 10; i++)
        {
          Alarm_Get(i, &alarm_get_data[i]);
        }
      }
    
      // Reset the Button 5 Press Flag
      button5.press_flag = false;
    }

    // Check if the RTC Interrupt Flag is set (RTC Interrupt Flag) on PB4 (Activated every second)
    if (rtc_int_flag)
    {
      // Retrieve the time values from the RTC module
      Time_Get(&time_get_data);
      
      // Check the alarms
      //   void Alarm_Check (ALARM *alarm_check_data, TIME *time_get_data)
      Alarm_Check(&alarm_check_data, &time_get_data);

      // Reset the RTC Interrupt Flag
      rtc_int_flag = false;
    }

    // Check if the ADC interrupt flag is set (ADC Valid Flag)
    if (adc_valid_flag)
	  {
      // Re-enable the ADC interrupt to continue monitoring ADC values
      HAL_ADC_Start_IT(&hadc1);

      // Delay for 100ms to allow the ADC to stabilize
      HAL_Delay(100);
	  }

    // Check if the UART interrupt flag is set (UART Receive Flag)
    if (uart_rx_flag)
	  {
      // Re-enable the UART interrupt to continue receiving data
      HAL_UART_Receive_IT(&huart1,uart_rx_data,2); 
      
      // Delay for 100ms to allow the UART to stabilize
      HAL_Delay(100);
	  }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* User-defined functions---------------------------------------------------*/

/**
  * @brief  Convert a normal decimal number to Binary Coded Decimal (BCD).
  * @param  val: Decimal number to be converted.
  * @retval uint8_t: BCD representation of the input decimal number.
  */
uint8_t Dec_To_BCD(int val)
{
  return (uint8_t)( (val/10*16) + (val%10) );
}

/**
  * @brief  Convert a Binary Coded Decimal (BCD) value to a normal decimal number.
  * @param  val: BCD value to be converted.
  * @retval int: Decimal representation of the input BCD value.
  */
int BCD_To_Dec(uint8_t val)
{
  return (int)( (val/16*10) + (val%16) );
}

/**
  * @brief  Set time to the RTC module through I2C interface (Run only once after reset the RTC).
  * @param  sec: Seconds: 0-59
  * @param  min: Minutes: 0-59
  * @param  hour: Hours: 0-23
  * @param  dow: Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday)
  * @param  dom: Date of the month: 1-31
  * @param  month: Month: 1-12
  * @param  year: Year: 0-99 (0 = 2000, 1 = 2001, ..., 99 = 2099)
  * @retval None
*/
void Time_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
{
	// A blank array (7 slots) to contain the time values
  uint8_t setTime[7];

  // Store the time values (converted in to BCD code) into the blank array
	setTime[0] = Dec_To_BCD(sec);
	setTime[1] = Dec_To_BCD(min);
	setTime[2] = Dec_To_BCD(hour);
	setTime[3] = Dec_To_BCD(dow);
	setTime[4] = Dec_To_BCD(dom);
	setTime[5] = Dec_To_BCD(month);
	setTime[6] = Dec_To_BCD(year);

  // Send the array containing the time values to the RTC module through I2C interface at address 00h - 06h (size of value: 7 bytes)
  // HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);	
  HAL_I2C_Mem_Write(DS3231_I2C, DS3231_ADDR, 0x00, 1, setTime, sizeof(setTime), 1000);
}

/**
  * @brief  Get time from the RTC module through I2C interface.
  * @param  time_get_data: Pointer to a TIME structure to store the retrieved time values.
  * @retval None
*/
void Time_Get (volatile TIME *time_get_data)
{
	// A blank array (7 slots) to contain the time values received from the RTC module
  uint8_t getTime[7];

  // Receive the time values from the RTC module through I2C interface, then store them into the blank array (size of value: 7 bytes)
  // HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);  
  HAL_I2C_Mem_Read(DS3231_I2C, DS3231_ADDR, 0x00, 1, getTime, sizeof(getTime), 1000);

  // Store the time values (converted from BCD code to decimal) into the time variable
	time_get_data->second       = BCD_To_Dec(getTime[0]);
	time_get_data->minute       = BCD_To_Dec(getTime[1]);
	time_get_data->hour         = BCD_To_Dec(getTime[2]);
	time_get_data->dayofweek    = BCD_To_Dec(getTime[3]);
	time_get_data->dateofmonth  = BCD_To_Dec(getTime[4]);
	time_get_data->month        = BCD_To_Dec(getTime[5]);
	time_get_data->year         = BCD_To_Dec(getTime[6]);
}

/**
  * @brief  Control settings of the RTC module (Alarm 1).
  * @param  mode: Alarm rate (0-5).
  * @param  sec: Seconds: 0-59.
  * @param  min: Minutes: 0-59.
  * @param  hour: Hours: 0-23.
  * @param  dow_dom: Day of the week or date of the month (1-7 or 1-31).
  * @param  dy_dt: Day of week or date of month (1 = day of week, 0 = date of month, 0 = not used).
  * @retval None
*/
void Time_Ctrl (uint8_t mode, uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, bool dy_dt)
{
  // A blank array (4 slots) to contain the RTC alarm settings
  uint8_t ctrlTime[4];

  // A mask bit to set the Control register in the RTC module
  //    Address 0Eh: A1IE = 1
  uint8_t ctrlAlarm = 1; 

  // A mask bit to set the alarm modes (Bit 7) in the RTC module
  uint8_t maskBit = 128;

  // A mask bit for selection of [day of week] or [date of month] (Bit 6) in the RTC module
  //    If the input [day of week]/ [date of month] selection is 1, the mask bit will be 64.
  uint8_t dyDt = (dy_dt == true) ? 64 : 0;

  // Store the RTC alarm time settings (converted into BCD code) into the blank array
  ctrlTime[0] = Dec_To_BCD(sec);
	ctrlTime[1] = Dec_To_BCD(min);
	ctrlTime[2] = Dec_To_BCD(hour);
	ctrlTime[3] = Dec_To_BCD(dow_dom);

  // Adjust the RTC alarm mode settings into the array basing on the input mode
  //    Mode  : Alarm rate
  //      0    : Alarm once per second
  //      1    : Alarm when seconds match
  //      2    : Alarm when minutes and seconds match
  //      3    : Alarm when hours, minutes, and seconds match
  //      4    : Alarm when date, hours, minutes, and seconds match
  //      5    : Alarm when day, hours, minutes, and seconds match
  switch (mode)
  {
    case 0:
      // Alarm mask bit: A1M4:A1M1 = b1111, DY/DT = X
      ctrlTime[0] += maskBit;
      ctrlTime[1] += maskBit;
      ctrlTime[2] += maskBit;
      ctrlTime[3] += maskBit;
      break;
    case 1:
      // Alarm mask bit: A1M4:A1M1 = b1110, DY/DT = X
      ctrlTime[0] += 0;
      ctrlTime[1] += maskBit;
      ctrlTime[2] += maskBit;
      ctrlTime[3] += maskBit;
      break;
    case 2:
      // Alarm mask bit: A1M4:A1M1 = b1100, DY/DT = X
      ctrlTime[0] += 0;
      ctrlTime[1] += 0;
      ctrlTime[2] += maskBit;
      ctrlTime[3] += maskBit;
      break;
    case 3:
      // Alarm mask bit: A1M4:A1M1 = b1000, DY/DT = X
      ctrlTime[0] += 0;
      ctrlTime[1] += 0;
      ctrlTime[2] += 0;
      ctrlTime[3] += maskBit;
      break;
    case 4:
      // Alarm mask bit: A1M4:A1M1 = b0000, DY/DT = 0
      ctrlTime[0] += 0;
      ctrlTime[1] += 0;
      ctrlTime[2] += 0;
      ctrlTime[3] += 0;
      break;
    case 5:
      // Alarm mask bit: A1M4:A1M1 = b0000, DY/DT = 1
      ctrlTime[0] += 0;
      ctrlTime[1] += 0;
      ctrlTime[2] += 0;
      ctrlTime[3] += dyDt;
      break;
    default:
      // Alarm mask bit: A1M4:A1M1 = b0000, DY/DT = 0
      ctrlTime[0] += 0;
      ctrlTime[1] += 0;
      ctrlTime[2] += 0;
      ctrlTime[3] += 0;
      break;
  }

  // Send the array containing the RTC alarm mode setting to the RTC module through I2C interface at address 07h - 0Ah (size of value: 4 bytes)
  // HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);	  
  HAL_I2C_Mem_Write(DS3231_I2C, DS3231_ADDR, 0x07, 1, ctrlTime, sizeof(ctrlTime), 1000);

  // Send the alarm control mask bits to the RTC module through I2C interface at address 0Eh (size of value: 1 byte)
  // HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);	  
  HAL_I2C_Mem_Write(DS3231_I2C, DS3231_ADDR, 0x0E, 1, &ctrlAlarm, sizeof(ctrlAlarm), 1000);
}

/**
  * @brief  Initialize RTC module (Run only once after reset the RTC).
  * @param  sec: Seconds: 0-59
  * @param  min: Minutes: 0-59
  * @param  hour: Hours: 0-23
  * @param  dow: Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday)
  * @param  dom: Date of the month: 1-31
  * @param  month: Month: 1-12
  * @param  year: Year: 0-99 (0 = 2000, 1 = 2001, ..., 99 = 2099)
  * @retval None
*/
void Time_Init (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
{
  // This function is intended for initial setup of the RTC module after a reset.
  // It should not be called repeatedly unless reinitialization is required.
  //    Time_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
  Time_Set (sec, min, hour, dow, dom, month, year);

  // Run only once after reset the RTC module to initially set the alarm
  //    Time_Ctrl (uint8_t mode, uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, bool dy_dt)
  //      Mode   : Alarm rate
  //        0    : Alarm once per second
  //        1    : Alarm when seconds match
  //        2    : Alarm when minutes and seconds match
  //        3    : Alarm when hours, minutes, and seconds match
  //        4    : Alarm when date, hours, minutes, and seconds match
  //        5    : Alarm when day, hours, minutes, and seconds match
  //      For mode 0, the remaining input are don't-care values
  Time_Ctrl 
  (
     0, // Alarm rate: 0-5
     0, // Seconds: 0-59
     0, // Minutes: 0-59
     0, // Hours: 0-23
     0, // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday), or Date of the month: 1-31
     0  // Select: 1 = day of week, 0 = date of month
  );   
}

/**
  * @brief  Write a single alarm to the EEPROM module.
  * @param  sec: Seconds: 0-59
  * @param  min: Minutes: 0-59
  * @param  hour: Hours: 0-23
  * @param  dow_dom: Day of the week or date of the month (day of week: 1-7, date of month: 1-31, not used: X)
  * @param  dy_dt: Day of week or date of month (0 = date of month, 1 = day of week, 2 = not used)
  * @param  on_off: On/ Off state of the alarm (1 = ON, 0 = OFF)
  * @param  slot: Slot number of the alarm in the EEPROM module (0-9)
  * @retval None
*/
void Alarm_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, uint8_t dy_dt, bool on_off, uint8_t slot)
{
  // Internal address of the alarm in the EEPROM module (0-8192, or 13 bits)
  uint16_t address = slot * 4;

  // Add an ON/OFF (1 bit) signal into the alarm package by using the MSB of the second register
  if (on_off)
  {
    sec += (1 << 7);
  }

  // Add a [day of week] or [date of month] (1 bit) signal into the alarm package by using bit 6 of the dow_dom register
  switch (dy_dt)
  {
    // Date of the month
    case 0:
      // Decoding: [Bit 7] = 1, [Bit 6] = 0
      dow_dom += (1 << 7);
    break;

    // Day of the week
    case 1:
      // Decoding: [Bit 7] = 1, [Bit 6] = 1
      dow_dom += ((1 << 7) | (1 << 6));
    break;

    // Not used
    case 2:
      // Decoding: [Bit 7] = 0, [Bit 6] = 0
      dow_dom += 0;
    break;

    default:
      dow_dom += 0;
    break;
  }

  // A blank array (4 slots) to contain the alarm values
  uint8_t setAlarm[4];
  
  // Store the alarm values into the blank array
  setAlarm[0] = sec;
  setAlarm[1] = min;
  setAlarm[2] = hour;
  setAlarm[3] = dow_dom;

  // HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
  HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, address, 1, setAlarm, sizeof(setAlarm), 1000);

  // Delay to allow the EEPROM module to complete the Page Write operation
  //    Neccesary delay cycle calculation:
  //        1 [Start Condition by Host] +
  //     +  8 [Device Address Byte]     + 1 [ACK from Client]      +
  //     +  8 [1st Word Address Byte]   + 1 [ACK from Client]      +
  //     +  8 [2nd Word Address Byte]   + 1 [ACK from Client]      +
  //     + {8 [1st Data Word]           + 1 [ACK from Client]} * 4 + 
  //     +  1 [Stop Condition by Host] 
  //     =  65 cycles  
  //    Neccesary delay time = 65 cycles / 400 kHz = 162.5 us = ~ 0.17 ms
  HAL_Delay(1);
}

/**
  * @brief  Read a single alarm from the EEPROM module.
  * @param  slot: Slot number of the alarm in the EEPROM module (0-9)
  * @param  alarm_get_data: Pointer to an ALARM structure to store the retrieved alarm values
  * @retval None
*/
void Alarm_Get (uint8_t slot, volatile ALARM *alarm_get_data)
{
  // Address of the alarm in the EEPROM module (0-8192, or 13 bits)
  uint16_t address = slot * 4;

  // A blank array (4 slots) to contain the alarm values received from the EEPROM module
  uint8_t getAlarm[4];

  // HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
  HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, address, 1, getAlarm, sizeof(getAlarm), 1000);

  // Delay to allow the EEPROM module to complete the Sequential Read operation
  //    Neccesary delay cycle calculation:
  //     {Dummy Write}
  //        1 [Start Condition by Host] +
  //     +  8 [Device Address Byte]     + 1 [ACK from Client]      +
  //     +  8 [1st Word Address Byte]   + 1 [ACK from Client]      +
  //     +  8 [2nd Word Address Byte]   + 1 [ACK from Client]      +
  //     {Sequential Read}
  //     +  1 [Start Condition by Host] +
  //     +  8 [Device Address Byte]     + 1 [ACK from Client]      +
  //     + {8 [1st Data Word]           + 1 [ACK from Client]} * 4 + 
  //     +  1 [Stop Condition by Host] 
  //     =  75 cycles
  //    Neccesary delay time = 75 cycles / 400 kHz = 187.5 us = ~ 0.19 ms
  HAL_Delay(1);

  // Store the alarm values into the alarm variable
  alarm_get_data->second  = getAlarm[0];
  alarm_get_data->minute  = getAlarm[1];
  alarm_get_data->hour    = getAlarm[2];
  alarm_get_data->dow_dom = getAlarm[3];
}

/**
  * @brief  Clear a single alarm from the EEPROM module.
  * @param  slot: Slot number of the alarm in the EEPROM module (0-9)
  * @retval None
*/
void Alarm_Clear (uint8_t slot)
{
  // Address of the alarm in the EEPROM module (0-8192, or 13 bits)
  uint16_t address = slot * 4;

  // A blank array (4 slots) to contain the alarm values to be cleared
  uint8_t clearAlarm[4] = {0, 0, 0, 0};

  // HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
  HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, address, 1, clearAlarm, sizeof(clearAlarm), 1000);

  // Delay to allow the EEPROM module to complete the Page Write operation
  //    Neccesary delay cycle calculation:
  //        1 [Start Condition by Host] +
  //     +  8 [Device Address Byte]     + 1 [ACK from Client]      +
  //     +  8 [1st Word Address Byte]   + 1 [ACK from Client]      +
  //     +  8 [2nd Word Address Byte]   + 1 [ACK from Client]      +
  //     + {8 [1st Data Word]           + 1 [ACK from Client]} * 4 + 
  //     +  1 [Stop Condition by Host] 
  //     =  65 cycles  
  //    Neccesary delay time = 65 cycles / 400 kHz = 162.5 us = ~ 0.17 ms
  HAL_Delay(1);
}

/**
  * @brief  Check the alarms stored in the EEPROM module against the current time.
  * @param  alarm_check_data: Pointer to an ALARM structure to store the retrieved alarm values
  * @param  time_get_data: Pointer to a TIME structure to store the current time values
  * @retval None
  * @note   This function compares the current time with all alarms stored in the EEPROM module.
*/
void Alarm_Check (volatile ALARM *alarm_check_data, volatile TIME *time_get_data)
{
  // Compare the current time with all alarms stored in the EEPROM module
  for (int i = 0; i <= ALARM_SLOT_NUM; i++)
  {
    // Retrieve the alarm values from the EEPROM module
    //    void Alarm_Get (uint8_t address, ALARM *alarm_get_data)
    Alarm_Get(i, alarm_check_data);

    // Check if the alarm is at ON or OFF state by checking the MSB of the second register
    if (!(alarm_check_data->second >= (1 << 7)))
    {
      // Skip if the alarm is OFF
      break;
    }

    // Check if the current time matches the alarm time (day of week or date of month)
    //    by first check if the alarm is at [day of week] or [date of month] mode by examining the MSB of the dow_dom register
    if (alarm_check_data->dow_dom >= (1 << 7))
    {
      // Check if the alarm is at the [day of week] mode by checking the mask bit (bit 6) of the dow_dom register
      if ((alarm_check_data->dow_dom & 0x7F) >= (1 << 6))
      {
        // Check if the [day of week] matches the current time
        //    by unmasking the MSB and bit 6 of the dow_dom register to get the original value of the dow_dom register
        if ((alarm_check_data->dow_dom & 0x3F) != time_get_data->dayofweek)
        {
          // Skip if the current time does NOT match the alarm time (day of week)
          break;
        }
      }
      else
      {
        // Check if the [date of month] matches the current time
        //    by unmasking the MSB of the dow_dom register to get the original value of the dow_dom register
        if ((alarm_check_data->dow_dom & 0x7F) != time_get_data->dateofmonth)
        {
          // Skip if the current time does NOT match the alarm time (date of month)
          break;
        }
      }
    }

    // Check if the current time matches the alarm time (hour)
    if (alarm_check_data->hour != time_get_data->hour)
    {
      // Skip if the current time does NOT match the alarm time (hour)
      break;
    }

    // Check if the current time matches the alarm time (minute)
    if (alarm_check_data->minute != time_get_data->minute)
    {
      // Skip if the current time does NOT match the alarm time (minute)
      break;
    }

    // Check if the current time matches the alarm time (second)
    //    by unmasking the MSB of the second register to get the original value of the second register
    if ((alarm_check_data->second & 0x7F) != time_get_data->second)
    {
      // Skip if the current time does NOT match the alarm time (second)
      break;
    }

    // If all the above checks pass, the alarm is activated
    debug_alarm_activate_ctr += 1;
    break;
  }

}


/**
  * @brief  Debounce the button state and handle button press events.
  * @param  button: Pointer to a BUTTON structure representing the button to be debounced.
  * @retval None
*/
void Button_Debounce(BUTTON *button)
{
  // Check button state and handle debouncing
  switch (button->state)
  {
    // State when the button is not pressed (HIGH)
    case BUTTON_STATE_HIGH:

      // Check if the button is pressed (LOW)
      if (button->int_flag) 
      {
        // Start the debounce timer
        button->start_tick = HAL_GetTick();

        // Set the button state to waiting
        button->state = BUTTON_STATE_WAITING;
      }

    break;

    // State when the button is in transition (waiting for debounce)
    case BUTTON_STATE_WAITING:

      // Check if the button is still pressed (LOW) after the debounce delay
      if (HAL_GetTick() - button->start_tick >= DEBOUNCE_DELAY_MS) 
      {
        // Check if the button is still pressed (LOW)
        if (HAL_GPIO_ReadPin(button->gpio_port, button->gpio_pin) == GPIO_PIN_RESET) 
        {
          // Set the button press flag to true (valid press detected)
          button->press_flag = true;

          // Set the button state to stable low (pressed)
          button->state = BUTTON_STATE_LOW;
        } 
        else 
        {
          // Set the button state to stable high (not pressed)
          button->state = BUTTON_STATE_HIGH; // False alarm
        }
      }

    break;

    // State when the button is pressed (LOW)
    case BUTTON_STATE_LOW:

      // Wait until button is released
      if (HAL_GPIO_ReadPin(button->gpio_port, button->gpio_pin) == GPIO_PIN_SET) 
      {
        // Set the button state to stable high (not pressed)
        button->state = BUTTON_STATE_HIGH;

        // Reset the button interrupt flag
        button->int_flag = false;
      }

    break;
  }
}

/**
  * @brief  Callback function to handle GPIO interrupts.
  * @param  GPIO_Pin: The pin number of the GPIO that triggered the interrupt.
  * @retval None
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
    case RTC_IN5_Pin:
      // Set the RTC Interrupt Flag
      rtc_int_flag = true;
    break;
    
    case BUTTON1_IN12_Pin:
      // Set the Button 1 Interrupt Flag
      button1.int_flag = true;
    break;

    case BUTTON2_IN13_Pin:
      // Set the Button 2 Interrupt Flag
      button2.int_flag = true;
    break;

    case BUTTON3_IN14_Pin:
      // Set the Button 3 Interrupt Flag
      button3.int_flag = true;
    break;

    case BUTTON4_IN15_Pin:
      // Set the Button 4 Interrupt Flag
      button4.int_flag = true;
    break;  

    case BUTTON5_IN8_Pin:
      // Set the Button 5 Interrupt Flag
      button5.int_flag = true;
    break;  

    default:
      // Handle other GPIO interrupts if necessary
    break;
  }
}

/**
  * @brief  Callback function to handle UART interrupts.
  * @param  huart: Pointer to the UART handle.
  * @retval None
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  // Verify the UART instance to ensure the callback is for USART1
  // If the UART instance is USART1, store the received data into the uart_rx_data array
  if(huart->Instance == USART1)
  {
    // Store the received data into the uart_rx_data array
    uart_rx_hour = uart_rx_data[0];
    uart_rx_minute = uart_rx_data[1];

    // Set the UART receive flag
    uart_rx_flag = 1;
  }
}

/**
  * @brief  Callback function to handle ADC conversion complete interrupts.
  * @param  hadc: Pointer to the ADC handle.
  * @retval None
*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	// Verify the ADC instance to ensure the callback is for ADC1
  // If the ADC instance is ADC1, get the ADC value and calculate the battery percentage
  if (hadc == &hadc1)
	{
    // Get the ADC value
    adc_data = HAL_ADC_GetValue(&hadc1);

    // Set the ADC valid flag
    adc_valid_flag = 1;

    // Calculate the battery percentage
    battery_percentage = adc_data * 100 / 4095;
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
