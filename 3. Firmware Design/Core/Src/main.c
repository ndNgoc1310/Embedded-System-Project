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
  * - Students at Vietnam National University, Ho Chi Minh City - University of Technology.
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
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "EPD_Test.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// Enum for alarm day of week or date of month mode
// typedef enum
// {
//   DAY_OF_WEEK_MODE,
//   DATE_OF_MONTH_MODE,
//   NOT_USED_MODE
// } ALARM_DY_DT_MODE;

// Enum for button debounce states
typedef enum 
{
  BUTTON_RELEASED,    // Button is not pressed (HIGH state)
  BUTTON_WAITING,     // Button is pressed (LOW state) and waiting for debounce delay
  BUTTON_PRESSED,     // Button is pressed (LOW state)
} BUTTON_STATE;

// Enum for system modes
//typedef enum {
// DEFAULT_MODE,
// TIME_SETUP_MODE,
// ALARM_SETUP_MODE,
// ALARM_VIEW_MODE,
// ALARM_ACTIVE_MODE,
// SYSTEM_OPTIONS_MODE
//} SYSTEM_MODE;

// Enum for system parameters to be selected for modification
// typedef enum
// {
//   TIME_MINUTE,   // Set minutes value
//   TIME_HOUR,     // Set hours value
//   TIME_DOW,      // Set day of the week value
//   TIME_DOM,      // Set date of the month value
//   TIME_MONTH,    // Set month value
//   TIME_YEAR,     // Set year value
// } TIME_SETUP_CURSOR;

// Enum for parameter selection in Alarm Setup Mode
// typedef enum
// {
//   ALARM_MINUTE,   // Set minutes value
//   ALARM_HOUR,     // Set hours value
//   ALARM_DY_DT,    // Set day of week or date of month (1 = day of week, 0 = date of month, 2 = not used)
//   ALARM_DOW_DOM,  // Set day of the week or date of the month value
//   ALARM_ON_OFF,   // Set ON/OFF state
// } ALARM_SETUP_CURSOR;

// Structure of 7 one-byte unsigned characters to store time values
//typedef struct
//{
//	uint8_t second;       // Seconds: 0-59
//	uint8_t minute;       // Minutes: 0-59
//	uint8_t hour;         // Hours: 0-23
//	uint8_t dayofweek;    // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday)
//	uint8_t dateofmonth;  // Date of the month: 1-31
//	uint8_t month;        // Month: 1-12
//	uint8_t year;         // Year: 0-99 (0 = 2000, 1 = 2001, ..., 99 = 2099)
//} TIME_DATA;

// // Structure of 4 one-byte unsigned characters to store alarm values
typedef struct
{
  uint8_t           second;   // Seconds: 0-59 (MSB = 1 for ON, 0 for OFF)
  uint8_t           minute;   // Minutes: 0-59
  uint8_t           hour;     // Hours: 0-23
  ALARM_DY_DT_MODE  dy_dt;    // Select: DAY_OF_WEEK_MODE, DATE_OF_MONTH_MODE, NOT_USED_MODE
  uint8_t           dow_dom;  // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday), or Date of the month: 1-31
  bool              on_off;   // true = ON, false = OFF
} ALARM_DATA;
// typedef enum
// {
//   CLEAR_ALL_ALARM,
//   CONTRIBUTOR_INFO,
// } SYSTEM_OPT_CURSOR;
// Struct representing each button with debouncing state
typedef struct 
{
  GPIO_TypeDef    *gpio_port; // GPIO port of the button      
  uint16_t        gpio_pin;   // GPIO pin of the button
  uint8_t         index;      // Index of the button (0-4)
  BUTTON_STATE    state;      // Current state of the button
  uint32_t        start_tick; // Start time of the debounce timer
  volatile bool   int_flag;   // Set when button interrupt is triggered
  volatile bool   press_flag; // Set when button is pressed
  volatile bool   hold_flag;  // Set when button is held down
  volatile bool   latch;  // Set when button is pressed or held (once only)
} BUTTON_DATA;

// Struct for system state
// typedef struct
// {
//   SYSTEM_MODE         mode;               // Current system mode
//   SYSTEM_MODE         past_mode;          // Previous system mode
//   TIME_SETUP_CURSOR   time_setup_cursor;  // Cursor of selection for Time Setup Mode
//   ALARM_SETUP_CURSOR  alarm_setup_cursor; // Cursor of selection for Alarm Setup Mode
//   uint8_t             alarm_view_cursor;  // Cursor of selection for Alarm View Mode
//   SYSTEM_OPT_CURSOR   system_opt_cursor;  // Cursor of selection for System Options Mode
//   uint8_t             battery_display;    // Battery percentage to be displayed: 100, 75, 50, 25, 0
// } SYSTEM_STATE;

// Struct for time parameters to be modified
// typedef struct
// {
//   uint8_t           minute;   // Minutes: 0-59
//   uint8_t           hour;     // Hours: 0-23
//   uint8_t           dow;      // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday), or Date of the month: 1-31
//   uint8_t           dom;      // Date of the month: 1-31, or Day of the week: 1-7, or not used: 0
//   uint8_t           month;    // Month: 1-12
//   uint8_t           year;     // Year: 0-99 (0 = 2000, 1 = 2001, ..., 99 = 2099)
// } TIME_SETUP_DATA;
// Struct for alarm parameters to be modified
// typedef struct
// {
//   uint8_t           minute;   // Minutes: 0-59
//   uint8_t           hour;     // Hours: 0-23
//   ALARM_DY_DT_MODE  dy_dt;    // Select: DAY_OF_WEEK_MODE, DATE_OF_MONTH_MODE, NOT_USED_MODE
//   uint8_t           dow_dom;  // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday), or Date of the month: 1-31
//   bool              on_off;   // true = ON, false = OFF
// } ALARM_SETUP_DATA;;

FLAG_SYSTEM flag_sSystem_mode = {1, 1, 1, 1, 1};
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

// Number of alarms in the EEPROM module
#define ALARM_SLOT_NUM 10 

// Address of alarm slot pointer in EEPROM module (right next to the final alarm slot)
#define ALARM_SLOT_PTR_ADDR (ALARM_SLOT_NUM * 4)

// Active state of buttons
#define BUTTON_ACTIVE GPIO_PIN_RESET

// Debounce threshold in milliseconds
#define BUTTON_DEBOUNCE_DELAY 30

// Button hold threshold in milliseconds
#define BUTTON_HOLD_TH 600

// Button hold cycle in milliseconds
#define BUTTON_HOLD_CYCLE 250

// Number of system modes
#define SYSTEM_MODE_NUM 6

// 
#define ALARM_VIEW_CURSOR_MAX 9
#define SYSTEM_OPT_CURSOR_MAX 1

#define SYSTEM_CURSOR_MAX 9

#define DISPLAY_DELAY 1
// Active state of the buzzer
#define BUZZER_ACTIVE GPIO_PIN_SET
#define BUZZER_INACTIVE GPIO_PIN_RESET

// Buzzer cycle number for alarm sound
#define BUZZER_CYCLE_NUM 10

// Buzzer delay in milliseconds
#define BUZZER_SHORT_DELAY 100
#define BUZZER_MEDIUM_DELAY 300
#define BUZZER_LONG_DELAY 500
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* SYSTEM ==========================================*/
// Global variable to store the current system mode and selected parameter
SYSTEM_STATE system_state = {0};

// Global variable to store the system parameters to be modified
TIME_SETUP_DATA time_setup_data = {0};
ALARM_SETUP_DATA alarm_setup_data = {0};

/* RTC & EEPROM ====================================*/
// Variable to store the time values received from the RTC module every second
TIME_DATA time_get_data = {0};

// Array to store the alarm values received from the EEPROM module
volatile ALARM_DATA alarm_get_data [ALARM_SLOT_NUM] = {0}; 

// Pointer to the next free alarm in the EEPROM module
uint8_t alarm_slot_ptr;

// Flag for RTC trigger external interrupt (RTC Interrupt Flag) on PB4 (Activated every second)
volatile bool rtc_int_flag = false;

// Debugging: Track the number of alarm activations
uint8_t debug_alarm_activate_ctr = 0;
bool alarm_active_flag = false; 
volatile bool debug_rtc_int = false;

/* BUTTON ========================================*/
// Debugging: Track if the button is pressed or not
volatile uint8_t debug_button_counter[5] = {0};

// Global instances of DEBOUNCE_BUTTON for each button
BUTTON_DATA button0 = {GPIOB, BUTTON0_IN12_Pin, 0, BUTTON_RELEASED, 0, false, false, false, false};
BUTTON_DATA button1 = {GPIOB, BUTTON1_IN13_Pin, 1, BUTTON_RELEASED, 0, false, false, false, false};
BUTTON_DATA button2 = {GPIOB, BUTTON2_IN14_Pin, 2, BUTTON_RELEASED, 0, false, false, false, false};
BUTTON_DATA button3 = {GPIOB, BUTTON3_IN15_Pin, 3, BUTTON_RELEASED, 0, false, false, false, false};
BUTTON_DATA button4 = {GPIOA, BUTTON4_IN8_Pin,  4, BUTTON_RELEASED, 0, false, false, false, false};

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
volatile bool uart_tx_flag = false;
// Variable for UART receive data
uint8_t uart_rx_data[7];

// Variable for UART transmit data
uint8_t uart_tx_data[7];

// Variables to store decoded values received from the UART module
uint8_t uart_hour;      //uart_rx_data[0]; this can also be used to set the mode of the UART module
uint8_t uart_minute;    //uart_rx_data[1]
uint8_t uart_dow;       //uart_rx_data[5]
uint8_t uart_day;       //uart_rx_data[2]; this can also be used to define the day of the week in mode 3
uint8_t uart_month;     //uart_rx_data[3]
uint8_t uart_year;      //uart_rx_data[4]
uint8_t uart_alarmNum;	//uart_rx_data[5] number of the alarm (alarm number1/ alarm number2/...)
uint8_t uart_alarmSta;	//uart_rx_data[6] status of the alarm (on/off)

// Variable to store mask values for the modes of the UART module
//    0. Quit Mode:     0b11000000 
//    1. Time Setup:    0b11110000 (ON)/ 0b11000000 (OFF)
//    2. Alarm Setup    0b01110000 (ON)/ 0b11000000 (OFF)
//    3. Update Alarms: 0b11111111
uint8_t uart_mode;		  
                        
// Variable to indicate the mode of the UART module (1, 2, or 3)
uint8_t uart_appMode = 0;

// Variable to store the number of alarms sent to the App (0-10)
uint8_t sentCount = 0; 

//Variables for alarms:
uint8_t alarm_Info[10][7]; // 10 alarms, each alarm has 7 values (hour, minute, day, month, year, number, status)

/* BUZZER ======================================*/
// Buzzer cycle number
uint8_t buzzer_cycle = 0; 

// Buzzer phase number
uint8_t buzzer_phase = 0; 

// Start time of the buzzer cycle
uint32_t buzzer_tick = 0; 


// time save
TIME_DATA time_setup = {0};
// alarm data
ALARM_DATA_SETUP_MODE sParam_data_alarm_setup_mode_1  = {0};
ALARM_DATA_SETUP_MODE sParam_data_alarm_setup_mode_2  = {0};
ALARM_DATA_SETUP_MODE sParam_data_alarm_setup_mode_3  = {0};
ALARM_DATA_SETUP_MODE sParam_data_alarm_setup_mode_4  = {0};
ALARM_DATA_SETUP_MODE sParam_data_alarm_setup_mode_5  = {0};
ALARM_DATA_SETUP_MODE sParam_data_alarm_setup_mode_6  = {0};
ALARM_DATA_SETUP_MODE sParam_data_alarm_setup_mode_7  = {0};
ALARM_DATA_SETUP_MODE sParam_data_alarm_setup_mode_8  = {0};
ALARM_DATA_SETUP_MODE sParam_data_alarm_setup_mode_9  = {0};
ALARM_DATA_SETUP_MODE sParam_data_alarm_setup_mode_10 = {0};

SYSTEM_PARAM_DATA_ALARM_VIEW_MODE sParam_data_alarm_view_mode = {
  &sParam_data_alarm_setup_mode_1,
  &sParam_data_alarm_setup_mode_2,
  &sParam_data_alarm_setup_mode_3,
  &sParam_data_alarm_setup_mode_4,
  &sParam_data_alarm_setup_mode_5,
  &sParam_data_alarm_setup_mode_6,
  &sParam_data_alarm_setup_mode_7,
  &sParam_data_alarm_setup_mode_8,
  &sParam_data_alarm_setup_mode_9,
  &sParam_data_alarm_setup_mode_10
};
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
void Time_Get (volatile TIME_DATA *time_get_data);

// Function to control settings of the RTC module (Alarm 1)
void Time_Ctrl (uint8_t mode, uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, bool dy_dt);

// Function to write a single alarm to the EEPROM module
void Alarm_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, ALARM_DY_DT_MODE dy_dt, bool on_off, uint8_t slot);

// Function to read a single alarm from the EEPROM module
void Alarm_Get (uint8_t slot, volatile ALARM_DATA *alarm_get_data);

// Function to clear a single alarm from the EEPROM module
void Alarm_Clear (uint8_t slot);

// // Function to check the alarms
void Alarm_Check (volatile TIME_DATA *time_get_data);
void Alarm_Ring (void);
// Functions to handle alarm slot pointer
void Alarm_Slot_Pointer_Set (void);
void Alarm_Slot_Pointer_Get (void);

// Debounce handler function to be called in main loop
void Button_Debounce(BUTTON_DATA *button);

// Button handler function to be called in main loop
void Button_Handle(void);

// System mode handlers
void System_Default_Mode_Handle (BUTTON_DATA *button);
void System_Time_Setup_Mode_Handle (BUTTON_DATA *button);
void System_Alarm_Setup_Mode_Handle (BUTTON_DATA *button);
void System_Alarm_View_Mode_Handle (BUTTON_DATA *button);
void System_Alarm_Active_Mode_Handle (BUTTON_DATA *button);
void System_Options_Mode_Handle (BUTTON_DATA *button);
// Update value to display E-ink code
void Update_alarm_data(void);

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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  bool uartDoneFlag = false;

    // Initially reset the buzzer
  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, BUZZER_INACTIVE); 
//  // Set time to the RTC module through I2C interface (Run only once after reset the RTC).
//  //    void Time_Set(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
//  Time_Set
//  (
//     0, // Seconds: 0-59
//    26, // Minutes: 0-59
//    11, // Hours: 0-23
//     4, // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday)
//    18, // Date of the month: 1-31
//     4, // Month: 1-12
//    25  // Year: 0-99 (0 = 2000, 1 = 2001, ..., 99 = 2099)
//  );

  // Initialize RTC module (Run only once after reset the RTC).
  Time_Ctrl 
  (
     0, // Alarm rate: 0-5
     0, // Seconds: 0-59
     0, // Minutes: 0-59
     0, // Hours: 0-23
     0, // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday), or Date of the month: 1-31
     0  // Select: 1 = day of week, 0 = date of month
  );   

  // Initially retrieve the time values from the RTC module
  Time_Get(&time_get_data);

   // Clear all alarms in the EEPROM module
  //  for (int i = 0; i < ALARM_SLOT_NUM; i++)
  //  {
  //    Alarm_Clear(i);
  //    alarm_slot_ptr = 0;
  //    Alarm_Slot_Pointer_Set();
  //  }

   // Store values of a single alarm to the next available address on the EEPROM module
   //    void Alarm_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, ALARM_DY_DT_MODE dy_dt, bool on_off, uint8_t slot)
  //  Alarm_Set
  //  (
  //    00,    // Seconds: 0-59
  //    10,    // Minutes: 0-59
  //    23,    // Hours: 0-23
  //     9,    // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday), or Date of the month: 1-31, or not used: 0
  //     2,    // Select: 0 = date of month, 1 = day of week, 2 = not used
  //    true,  // true = ON, false = OFF
  //     1     // Slot number of the alarm in the EEPROM module (0-9)
  //  );
  //  Alarm_Get(1, &alarm_get_data[1]);
  //  alarm_slot_ptr = 1;


  // Initially retrieve the alarm slot pointer data from the EEPROM module
  Alarm_Slot_Pointer_Get();

  // Initially retrieve the alarm values from the EEPROM module and send to system memory
  for (int i = 0; i < alarm_slot_ptr; i++)
  {
    Alarm_Get(i, &alarm_get_data[i]);
  }

  // Set the initial system parameters to current time values and default settings for convenience
 

  // Initialize the UART module to receive data
  //    HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
  HAL_UART_Receive_IT(&huart1, uart_rx_data, 7);

  // Initialize the ADC module to monitor battery voltage
  //    HAL_ADC_Start_IT(ADC_HandleTypeDef *hadc);
  HAL_ADC_Start_IT(&hadc1);
  void toggle_alarm_buzzer()
  {
	  HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // while ((button0.int_flag || button1.int_flag || button2.int_flag || button3.int_flag || button4.int_flag)
    // ||     (button0.state == BUTTON_PRESSED || button1.state == BUTTON_PRESSED || button2.state == BUTTON_PRESSED || button3.state == BUTTON_PRESSED || button4.state == BUTTON_PRESSED)
    // ||     (button0.state == BUTTON_WAITING || button1.state == BUTTON_WAITING || button2.state == BUTTON_WAITING || button3.state == BUTTON_WAITING || button4.state == BUTTON_WAITING)
    // ||     ((button0.state == BUTTON_RELEASED || button1.state == BUTTON_RELEASED || button2.state == BUTTON_RELEASED || button3.state == BUTTON_RELEASED || button4.state == BUTTON_RELEASED)
    // &&     (button0.int_flag || button1.int_flag || button2.int_flag || button3.int_flag || button4.int_flag)))
    while ((button0.int_flag || button1.int_flag || button2.int_flag || button3.int_flag || button4.int_flag))
    {
      Button_Handle();
    }
    // TIME_DATA time_get;
    

    // Check if the RTC Interrupt Flag is set (RTC Interrupt Flag) on PB4 (Activated every second)
    if (rtc_int_flag)
    {

      // Retrieve the time values from the RTC module
      Time_Get(&time_get_data);
      Update_alarm_data();

      // Check the alarms
      //   void Alarm_Check (volatile TIME_DATA *time_get_data)
      Alarm_Check(&time_get_data);

      // Reset the RTC Interrupt Flag
      rtc_int_flag = false;

      // Toggle the debug RTC interrupt flag for debugging purposes
      debug_rtc_int = !debug_rtc_int;

      if(system_state.mode == DEFAULT_MODE)
      {   
        default_mode(&flag_sSystem_mode, &time_get_data, battery_percentage);
      }

      if ((button0.press_flag || button1.press_flag|| button2.press_flag || button3.press_flag || button4.press_flag))
      {
        // time_get = (TIME_DATA) time_get_data;
        // Default mode
        if(system_state.mode == DEFAULT_MODE)
        {   
          default_mode(&flag_sSystem_mode, &time_get_data, battery_percentage);
          button0.press_flag = 0;
          button1.press_flag = 0;
          button2.press_flag = 0;
          button3.press_flag = 0;
          button4.press_flag = 0;
        }
        // System setup mode
        else if(system_state.mode == SYSTEM_OPTIONS_MODE)
        {   
          // system_setup_mode (FLAG_SYSTEM *flag_set_up,  CONTROL_SCREEN *Control_screen);
          system_setup_mode (&flag_sSystem_mode,  &system_state, battery_percentage);
          button0.press_flag = 0;
          button1.press_flag = 0;
          button2.press_flag = 0;
          button3.press_flag = 0;
          button4.press_flag = 0;
        }
        // Alarm_set_up mode
        else if(system_state.mode == ALARM_SETUP_MODE)
        {   
          //alarm_setup_mode(FLAG_SYSTEM *flag_alarm_set_up_mode, CONTROL_SCREEN *Control_screen, SYSTEM_PARAM_DATA_ALARM_VIEW_MODE *alarm_view_mode);
          alarm_setup_mode(&flag_sSystem_mode, &system_state, &alarm_setup_data, battery_percentage);
          button0.press_flag = 0;
          button1.press_flag = 0;
          button2.press_flag = 0;
          button3.press_flag = 0;
          button4.press_flag = 0;
        }
        // alarm view mode
        else if(system_state.mode == ALARM_VIEW_MODE)
        {   
          //alarm_view_mode (FLAG_SYSTEM *flag_alarm_view_mode, CONTROL_SCREEN *Control_screen, uint8_t *count_view, uint8_t *count_select, SYSTEM_PARAM_DATA_ALARM_VIEW_MODE *alarm_view_mode);
          alarm_view_mode (&flag_sSystem_mode, &system_state, &sParam_data_alarm_view_mode, battery_percentage);
          button0.press_flag = 0;
          button1.press_flag = 0;
          button2.press_flag = 0;
          button3.press_flag = 0;
          button4.press_flag = 0;
        }
        //alarm time set up mode
        else if(system_state.mode == TIME_SETUP_MODE)
        {   
          time_setup_mode(&flag_sSystem_mode, &system_state, &time_setup_data, battery_percentage);
          button0.press_flag = 0;
          button1.press_flag = 0;
          button2.press_flag = 0;
          button3.press_flag = 0;
          button4.press_flag = 0;
        }
      }
      toggle_alarm_buzzer();

       // Check if the alarm is active
      if (alarm_active_flag)
      {
      // Call the alarm ringing function
        Alarm_Ring();

      // Call the system alarm active mode handler to track if any button is pressed
        System_Alarm_Active_Mode_Handle(&button0);
      }
    }

    // Check if the ADC interrupt flag is set (ADC Valid Flag)
      if (adc_valid_flag)
	  {
      // Re-enable the ADC interrupt to continue monitoring ADC values
      HAL_ADC_Start_IT(&hadc1);

      // Delay for 100ms to allow the ADC to stabilize
      HAL_Delay(100);
      if ((battery_percentage % 25) == 0)
      {
        // Update the battery percentage value to be displayed
        system_state.battery_display = battery_percentage;
      }

      // Reset the ADC interrupt flag
      adc_valid_flag = false;
	  }

// Check for UART mode change
    //    0. Quit Mode:     0b11000000
    //    1. Time Setup:    0b11110000 (ON)/ 0b11000000 (OFF)
    //    2. Alarm Setup:   0b01110000 (ON)/ 0b11000000 (OFF)
    //    3. Update Alarms: 0b11111111
    if      (uart_mode == 0b11110000 || uart_rx_data[1] == 0b11110000)    
    {
      // Set the UART mode to 1 (Time Setup Mode)
      uart_appMode = 1;
      uartDoneFlag = false;
    }
    else if (uart_mode == 0b01110000 || uart_rx_data[1] == 0b01110000)     
    {
      // Set the UART mode to 2 (Alarm Setup Mode)
      uart_appMode = 2;
      uartDoneFlag = false;
    }
    else if (uart_mode == 0b11111111 || uart_rx_data[1] == 0b11111111)      
    {
      // Set the UART mode to 3 (Update Alarms Mode)
      uart_appMode = 3;
      uartDoneFlag = false;
    }
    else if (uart_mode == 0b11000000 || uart_rx_data[1] == 0b11000000)      
    {
      // Set the UART mode to 0 (Quit Mode)
      uart_appMode = 0;
      uartDoneFlag = false;
    }    

    // Check if the UART interrupt flag is set (UART Receive Flag)
    if (uart_rx_flag)
	  { 
      uart_mode = uart_rx_data[0];
      // Re-enable the UART interrupt to continue receiving data
      HAL_Delay(100);

      if (!uartDoneFlag)
      {
        switch (uart_appMode)
        {
          // Time Setup Mode
          case 1:
            // Decode the received data and set the time values
            uart_hour   = uart_rx_data[0]; //uart_rx_data[0] = hour
            uart_minute = uart_rx_data[1]; //uart_rx_data[1] = minute
            uart_dow    = uart_rx_data[5] + 1; //uart_rx_data[5] = day of the week (1-7)
            uart_day    = uart_rx_data[2]; //uart_rx_data[2] = date of the month (1-31)
            uart_month  = uart_rx_data[3]; //uart_rx_data[3] = month (1-12)
            uart_year   = uart_rx_data[4]; //uart_rx_data[4] = year (0-99)

            // Set the time to the RTC module through I2C interface
            Time_Init
            (
              0,           // Seconds: 0-59
              uart_minute, // Minutes: 0-59
              uart_hour,   // Hours: 0-23
              uart_dow,    // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday)
              uart_day,    // Date of the month: 1-31
              uart_month,  // Month: 1-12
              uart_year    // Year: 0-99 (0 = 2000, 1 = 2001, ..., 99 = 2099)
            );

            uartDoneFlag = true;
            break;

          // Alarm Setup Mode
          case 2:
            // Decode the received data and set the alarm values
            uart_hour     = uart_rx_data[0]; //uart_rx_data[0] = hour
            uart_minute   = uart_rx_data[1]; //uart_rx_data[1] = minute
            uart_alarmSta = uart_rx_data[6]; //uart_rx_data[6] status of the alarm (on/off)

            // Set the alarm to the EEPROM module through I2C interface
            Alarm_Set
            (
              0,              // Seconds: 0-59
              uart_minute,    // Minutes: 0-59
              uart_hour,      // Hours: 0-23
              0,              // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday), or Date of the month: 1-31, or not used: 0
              2,              // Select: 0 = date of month, 1 = day of week, 2 = not used
              uart_alarmSta,  // true = ON, false = OFF
              alarm_slot_ptr  // Slot number of the alarm in the EEPROM module (0-9)
            );

            // Update the newly set alarm data
            Alarm_Get(alarm_slot_ptr, &alarm_get_data[alarm_slot_ptr]);
            
            // Increment the pointer to the next available slot in the EEPROM module
            alarm_slot_ptr = (alarm_slot_ptr < ALARM_SLOT_NUM) ? (alarm_slot_ptr + 1) : 0;

            // Save the alarm pointer data to the EEPROM module
            Alarm_Slot_Pointer_Set();

            uartDoneFlag = true;
            break;
          
          // Update Alarms Mode
          case 3:
            uart_tx_flag = 1; //Set the flag to transmit data to App
            HAL_Delay(50);                     // Delay for 50ms to allow the UART to stabilize
            
            break;

          default:
            // Do nothing if the UART mode is not recognized
            break;
        }
      }
    
      // if (uart_rx_data[0] == 0b11111111) //Transmit signal from App to STM32
      // {
      //   uart_tx_flag = 1; //Set the flag to transmit data to App
      //   HAL_Delay(50);                     // Delay for 50ms to allow the UART to stabilize
      //   // uart_tx_flag  = 1;
      //   //uart_rx_data[0] = 0b00000000; // Reset the uart_rx_data array to avoid sending the same data again
      // }

      // Re-enable the UART interrupt to continue receiving data
      HAL_UART_Receive_IT(&huart1, uart_rx_data, 7);

      // Delay for 100ms to allow the UART to stabilize
      HAL_Delay(100);
	  }

    // Check if the UART transmit flag is set (Transmit Flag)
    if (uart_tx_flag)
    {
      // Update Alarms Mode
      if (uart_appMode == 3)
      {
        // Encode alarm data into packages to be sent to the App
        for (int i = 0; i < alarm_slot_ptr; i++)
        {
          alarm_Info[i][0] = alarm_get_data[i].hour;   // Store the hour value of the alarm
          alarm_Info[i][1] = alarm_get_data[i].minute; // Store the minute value of the alarm
          alarm_Info[i][2] = alarm_get_data[i].dow_dom;    // Store the day value of the alarm
          alarm_Info[i][5] = i;                        // Store the alarm number (slot number)
          alarm_Info[i][6] = alarm_get_data[i].on_off; // Store the status of the alarm (on/off)
        }
        
        // Transmit the alarm information packages to the App through UART
        for (int i = 0; i < 10; i++)            // Alarm 0 - 9
        {
          for (int j = 0; j < 7; j++)           // Alarm details 0 - 6 (hour, minute, day, month, year, number, status)
          {
            uart_tx_data[j] = alarm_Info[i][j]; // Store the alarm information into the uart_tx_data array
          }
          
          // Delay for 50ms to allow the UART to stabilize
          HAL_Delay(50);      
          
          // Transmit the alarm information to the App through UART
          HAL_UART_Transmit_IT(&huart1, uart_tx_data, 7);

          // Delay for 50ms to allow the UART to stabilize
          HAL_Delay(50);                        
        }
      }
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

  // Delay for 1ms to allow the RTC module to process the data
  HAL_Delay(1);
}

/**
  * @brief  Get time from the RTC module through I2C interface.
  * @param  time_get_data: Pointer to a TIME structure to store the retrieved time values.
  * @retval None
*/
void Time_Get (volatile TIME_DATA *time_get_data)
{
	// A blank array (7 slots) to contain the time values received from the RTC module
  uint8_t getTime[7];

  // Receive the time values from the RTC module through I2C interface, then store them into the blank array (size of value: 7 bytes)
  // HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);  
  HAL_I2C_Mem_Read(DS3231_I2C, DS3231_ADDR, 0x00, 1, getTime, sizeof(getTime), 1000);

  // Delay for 1ms to allow the RTC module to process the data
  HAL_Delay(1);

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

  // Delay for 1ms to allow the RTC module to process the data
  HAL_Delay(1);

  // Send the alarm control mask bits to the RTC module through I2C interface at address 0Eh (size of value: 1 byte)
  // HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);	  
  HAL_I2C_Mem_Write(DS3231_I2C, DS3231_ADDR, 0x0E, 1, &ctrlAlarm, sizeof(ctrlAlarm), 1000);

  // Delay for 1ms to allow the RTC module to process the data
  HAL_Delay(1);
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
void Alarm_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, ALARM_DY_DT_MODE dy_dt, bool on_off, uint8_t slot)
{
 
  // Add an ON/OFF (1 bit) signal into the alarm package by using the MSB of the second register
  if (on_off)
  {
    sec += (1 << 7);
  }

  // Add a [day of week] or [date of month] (1 bit) signal into the alarm package by using bit 6 of the dow_dom register
  switch (dy_dt)
  {
    // Date of the month
    case DATE_OF_MONTH_MODE:
      // Decoding: [Bit 7] = 1, [Bit 6] = 0
      dow_dom += (1 << 7);
      break;

    // Day of the week
    case DAY_OF_WEEK_MODE:
      // Decoding: [Bit 7] = 1, [Bit 6] = 1
      dow_dom += ((1 << 7) | (1 << 6));
      break;

    // Not used
    case NOT_USED_MODE:
      // Decoding: [Bit 7] = 0, [Bit 6] = 0
      dow_dom += 0;
      break;

    default:
      dow_dom += 0;
      break;
  }


  // Store the alarm values into the blank array
  uint8_t setAlarm[4] = {sec, min, hour, dow_dom};

  // HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
  uint16_t address = slot * 4;

  HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, address, 2, setAlarm, sizeof(setAlarm), 1000);

  // Delay to allow the EEPROM module to complete the Page Write operation
  HAL_Delay(5);
}

/**
  * @brief  Read a single alarm from the EEPROM module.
  * @param  slot: Slot number of the alarm in the EEPROM module (0-9)
  * @param  alarm_get_data: Pointer to an ALARM_DATA structure to store the retrieved alarm values
  * @retval None
*/
void Alarm_Get (uint8_t slot, volatile ALARM_DATA *alarm_get_data)
{
  // Address of the alarm in the EEPROM module (0-8192, or 13 bits)
  uint16_t address = slot * 4;

  // A blank array (4 slots) to contain the alarm values received from the EEPROM module
  uint8_t getAlarm[4];

  // HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
  HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, address, 2, getAlarm, sizeof(getAlarm), 1000);

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
  alarm_get_data->second = 0;
  alarm_get_data->minute = getAlarm[1];
  alarm_get_data->hour = getAlarm[2];
  
  if (getAlarm[3] >= (1 << 7))
  {
    if ((getAlarm[3] & 0x7F) >= (1 << 6))
    {
      alarm_get_data->dy_dt = DAY_OF_WEEK_MODE;
      alarm_get_data->dow_dom = getAlarm[3] & 0x3F;
    }
    else 
    {
      alarm_get_data->dy_dt = DATE_OF_MONTH_MODE;
      alarm_get_data->dow_dom = getAlarm[3] & 0x7F;
    }
  }
  else alarm_get_data->dy_dt = NOT_USED_MODE;

  if (getAlarm[0] >= (1 << 7)) alarm_get_data->on_off = true;
  else alarm_get_data->on_off = false;
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
  HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, address, 2, clearAlarm, sizeof(clearAlarm), 1000);

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
  HAL_Delay(5);
}

/**
 * @brief Save the data of pointer for the alarm slot to EEPROM module.
  * @retval None
 */
void Alarm_Slot_Pointer_Set (void)
{
  // HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
  HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, ALARM_SLOT_PTR_ADDR, 2, &alarm_slot_ptr, sizeof(alarm_slot_ptr), 1000);

  // Delay to allow the EEPROM module to complete the Page Write operation
  //    Neccesary delay cycle calculation:
  //        1 [Start Condition by Host] +
  //     +  8 [Device Address Byte]     + 1 [ACK from Client]  +
  //     +  8 [1st Word Address Byte]   + 1 [ACK from Client]  +
  //     +  8 [2nd Word Address Byte]   + 1 [ACK from Client]  +
  //     +  8 [1st Data Word]           + 1 [ACK from Client]} + 
  //     +  1 [Stop Condition by Host] 
  //     =  38 cycles  
  //    Neccesary delay time = 65 cycles / 400 kHz = 95 us = ~ 0.01 ms
  HAL_Delay(5);
}

/**
 * @brief Retrieve the data of pointer for the alarm slot from EEPROM module.
  * @retval None
 */
void Alarm_Slot_Pointer_Get (void)
{
  // HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
  HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, ALARM_SLOT_PTR_ADDR, 2, &alarm_slot_ptr, sizeof(alarm_slot_ptr), 1000);

  // Delay to allow the EEPROM module to complete the Random Read operation
  //    Neccesary delay cycle calculation:
  //     {Dummy Write}
  //        1 [Start Condition by Host] +
  //     +  8 [Device Address Byte]     + 1 [ACK from Client]  +
  //     +  8 [1st Word Address Byte]   + 1 [ACK from Client]  +
  //     +  8 [2nd Word Address Byte]   + 1 [ACK from Client]  +
  //     {Random Read}
  //     +  1 [Start Condition by Host] +
  //     +  8 [Device Address Byte]     + 1 [ACK from Client]  +
  //     +  8 [1st Data Word]           + 1 [ACK from Client]} + 
  //     +  1 [Stop Condition by Host] 
  //     =  48 cycles
  //    Neccesary delay time = 75 cycles / 400 kHz = 0.12 ms
  HAL_Delay(5);
}

/**
  * @brief  Check the alarms stored in the EEPROM module against the current time.
  * @param  time_get_data: Pointer to a TIME structure to store the current time values
  * @retval None
  * @note   This function compares the current time with all alarms stored in the EEPROM module.
*/
void Alarm_Check (volatile TIME_DATA *time_get_data)
{
  volatile ALARM_DATA alarmCheckData = {0};

  // Compare the current time with all available alarms in the EEPROM module
  for (int i = 0; i < alarm_slot_ptr; i++)
  {
    // Retrieve the alarm values from the EEPROM module
    //    void Alarm_Get (uint8_t slot, ALARM_DATA *alarm_get_data)
    Alarm_Get(i, &alarmCheckData);

    // Check if the alarm is at ON or OFF state by checking the MSB of the second register
    if (!alarmCheckData.on_off)
    {
      // Skip if the alarm is OFF
      continue;
    }

    // Check if the current time matches the alarm time (day of week)
    if ((alarmCheckData.dy_dt == DAY_OF_WEEK_MODE)
    &&  (alarmCheckData.dow_dom != time_get_data->dayofweek))
    {
      // Skip if the current time does NOT match the alarm time (day of week)
      continue;
    }

    // Check if the current time matches the alarm time (date of month)
    if ((alarmCheckData.dy_dt == DATE_OF_MONTH_MODE)
    &&  (alarmCheckData.dow_dom != time_get_data->dateofmonth))
    {
      // Skip if the current time does NOT match the alarm time (date of month)
      continue;
    }

    // Check if the current time matches the alarm time (hour)
    if (alarmCheckData.hour != time_get_data->hour)
    {
      // Skip if the current time does NOT match the alarm time (hour)
      continue;
    }

    // Check if the current time matches the alarm time (minute)
    if (alarmCheckData.minute != time_get_data->minute)
    {
      // Skip if the current time does NOT match the alarm time (minute)
      continue;
    }

    // Check if the current time matches the alarm time (second)
    if (alarmCheckData.second != time_get_data->second)
    {
      // Skip if the current time does NOT match the alarm time (second)
      continue;
    }

    // If all the above checks pass, the alarm is activated
    debug_alarm_activate_ctr++;

    // Stop checking time matching
    // to make sure that only one alarm can be activated at a time
    break;
  }
}

/**
  * @brief  Debounce the button state and handle button press events.
  * @param  button: Pointer to a BUTTON structure representing the button to be debounced.
  * * @param  button->state: Current state of the button (BUTTON_RELEASED, BUTTON_WAITING, BUTTON_PRESSED).
  * * @param  button->int_flag: Interrupt flag indicating if the button is pressed (LOW).
  * * @param  button->press_flag: Flag indicating if the button is pressed (LOW).
  * * @param  button->hold_flag: Flag indicating if the button is held down (LOW).
  * * @param  button->hold_latch: 
  * * @param  button->start_tick: Start time of the button press event (in milliseconds).
  * @retval None
*/
// void Button_Debounce(BUTTON_DATA *button)
// {
//   // Handle button state transitions and debounce logic
//   switch (button->state)
//   {
//     // Initial state: Button is released (HIGH)
//     case BUTTON_RELEASED:
//       // Check if interrupt flag is set
//       if (button->int_flag) 
//       {
//         // Record the start time for debounce
//         button->start_tick = HAL_GetTick();

//         // Transition to waiting state
//         button->state = BUTTON_WAITING;
//       }
//       break;

//     // Waiting state: Button is pressed (LOW) but not yet confirmed
//     case BUTTON_WAITING:
//       // Check if debounce delay has passed
//       if (HAL_GetTick() - button->start_tick >= (BUTTON_DEBOUNCE_DELAY + DISPLAY_DELAY)) 
//       {
//         // Check if button is still pressed (LOW state) after debounce delay
//         if (HAL_GPIO_ReadPin(button->gpio_port, button->gpio_pin) == BUTTON_ACTIVE) 
//         {
//           // Transition to pressed state
//           button->state = BUTTON_PRESSED;
//           button->press_flag = true;
//           button->latch = true;
//         } 
//         else 
//         {
//           // Reset interrupt flag and return to released state
//           button->int_flag = false;  
//           button->state = BUTTON_RELEASED;
//         }
//       }
//       break;
    
//     // Pressed state: Button is pressed (LOW)
//     case BUTTON_PRESSED:
//       // Check if button is released (HIGH state)
//       if (HAL_GPIO_ReadPin(button->gpio_port, button->gpio_pin) != BUTTON_ACTIVE) 
//       {
//         // Check if press duration is less than hold threshold
//         // if (HAL_GetTick() - button->start_tick < (BUTTON_HOLD_TH + DISPLAY_DELAY))
//         // {
//         //   // Set press flag for short press
//         //   button->press_flag = true;
//         // }
      
//         // Reset interrupt flag and return to released state
//         button->int_flag = false;
//         button->press_flag = false;
//         button->hold_flag = false;
//         button->latch = false;
//         button->state = BUTTON_RELEASED;
//       }
//       // Check if button is held down for long press
//       // else if ((HAL_GetTick() - button->start_tick >= (BUTTON_HOLD_TH + DISPLAY_DELAY)) && !button->hold_flag)
//       // {
//       //   // Set hold flag for long press
//       //   button->hold_flag = true;
//       // }
//       if(button->latch == true)
//       {
//         button->press_flag = false;
//         button->latch = false;
//       }
//       break;

//     default:
//       // Reset all flags and return to released state
//       button->state = BUTTON_RELEASED;
//       button->int_flag = false;
//       button->press_flag = false;
//       button->hold_flag = false;
//       button->latch = false;
//       break;
//   }
// }
// Button debounce using delay
void Button_Debounce(BUTTON_DATA *button)
{
  // Handle button state transitions and debounce logic
  switch (button->state)
  {
    // Initial state: Button is released (HIGH)
    case BUTTON_RELEASED:

      button->press_flag = false;
      button->hold_flag = false;
      button->latch = false;
    
      // Check if interrupt flag is set
      if (button->int_flag) 
      {
        // Record the start time for debounce
        button->start_tick = HAL_GetTick();

        // Transition to waiting state
        button->state = BUTTON_WAITING;
      }
      break;

    // Waiting state: Button is pressed (LOW) but not yet confirmed
    case BUTTON_WAITING:
      // Check if debounce delay has passed
      if (HAL_GetTick() - button->start_tick >= (BUTTON_DEBOUNCE_DELAY + DISPLAY_DELAY)) 
      {
        // Check if button is still pressed (LOW state) after debounce delay
        if (HAL_GPIO_ReadPin(button->gpio_port, button->gpio_pin) == BUTTON_ACTIVE) 
        {
          // Transition to pressed state
          button->state = BUTTON_PRESSED;
        } 
        else 
        {
          // Reset interrupt flag and return to released state
          button->int_flag = false;  
          button->state = BUTTON_RELEASED;
        }
      }
      break;
    
    // Pressed state: Button is pressed (LOW)
    case BUTTON_PRESSED:
      // Check if button is released (HIGH state)
      if (HAL_GPIO_ReadPin(button->gpio_port, button->gpio_pin) != BUTTON_ACTIVE) 
      {
        // Check if press duration is less than hold threshold
        if (HAL_GetTick() - button->start_tick < (BUTTON_HOLD_TH + DISPLAY_DELAY))
        {
          // Set press flag for short press
          button->press_flag = true;
        }
      
        // Reset interrupt flag and return to released state
        button->int_flag = false;

        //button->latch = false;
        button->state = BUTTON_RELEASED;
      }
      // Check if button is held down for long press
      else if ((HAL_GetTick() - button->start_tick >= (BUTTON_HOLD_TH + DISPLAY_DELAY)) && !button->hold_flag)
      {
        // Set hold flag for long press
        button->hold_flag = true;
      }
      break;

    default:
      // Reset all flags and return to released state
      button->state = BUTTON_RELEASED;
      button->int_flag = false;
      button->press_flag = false;
      button->hold_flag = false;
      button->latch = false;
      break;
  }
}

/**
 * @brief  Button interrupt handler function to be called in main loop
 * @retval None
 */
void Button_Handle (void) 
{
  // An empty button variable containing reset values for button pointer initiallization
  BUTTON_DATA buttonReset = {GPIOB, BUTTON0_IN12_Pin, 0, BUTTON_RELEASED, 0, false, false, false, false};

  // Initially point to an empty button variable
  BUTTON_DATA *button = &buttonReset;

  // Perform button debouncing to filter out noise
  Button_Debounce(&button0);
  Button_Debounce(&button1);
  Button_Debounce(&button2);
  Button_Debounce(&button3);
  Button_Debounce(&button4);

  
  if      (button0.press_flag || button0.hold_flag) button = &button0;
  else if (button1.press_flag || button1.hold_flag) button = &button1;
  else if (button2.press_flag || button2.hold_flag) button = &button2;
  else if (button3.press_flag || button3.hold_flag) button = &button3;
  else if (button4.press_flag || button4.hold_flag) button = &button4;
  

  // Debugging: Initialize the start tick for button hold detection
  uint32_t startTick = 0;
  
  // Debugging: Check if the button is pressed or held by increment its counter in activation
  if (button->press_flag)
  {
    // Increment the debug counter for the button
    debug_button_counter[button->index]++;
  }
  else if (button->hold_flag)
  {
    // Check if the button is held down for a certain duration
    if (HAL_GetTick() - startTick >= BUTTON_HOLD_CYCLE) 
    {
      // Increment the debug counter for the button
      debug_button_counter[button->index]++;

      // Reset the start tick for the next hold cycle
      startTick = HAL_GetTick();
    }
  }

  // Check if the button is pressed or held
  if (button->press_flag || button->hold_flag) 
  {
    // Handle button actions based on the current system mode
    switch (system_state.mode) 
    {
      case DEFAULT_MODE: 
        System_Default_Mode_Handle (button);
        break;

      case TIME_SETUP_MODE: 
        System_Time_Setup_Mode_Handle (button);
        break;
      
      case ALARM_SETUP_MODE: 
        System_Alarm_Setup_Mode_Handle (button);
        break;

      case ALARM_VIEW_MODE: 
        System_Alarm_View_Mode_Handle (button);
        break;

      case ALARM_ACTIVE_MODE: 
        System_Alarm_Active_Mode_Handle (button);
        break;

      case SYSTEM_OPTIONS_MODE:
        System_Options_Mode_Handle (button);
        break;

      default:
        break;
    }
  }
}

/**
 * @brief  Handles the default system mode based on button actions.
 * @param  button: Pointer to the BUTTON_DATA structure containing button state and index.
 * @retval None
 */
void System_Default_Mode_Handle (BUTTON_DATA *button)
{
  switch (button->index) 
  {
    // Button 0: Reserved for future use
    case 0:
      if (button->press_flag)
      {
        // Reserved
      }
      else if (button->hold_flag)
      {
        // Reserved
      }
      break;
    
    // Button 1: Reserved for future use
    case 1: 
      if (button->press_flag)
      {
        // Reserved
      }
      else if (button->hold_flag)
      {
        // Reserved
      }
      break;

    // Button 2: If pressed, do nothing (reserved for future use); if held, do nothing (reserved for future use)
    case 2: 
      if (button->press_flag) 
      {
        // Reserved
      }
      else if (button->hold_flag)
      {
        // Reserved
      }
      break;
    
    // Button 3: If pressed, do nothing (reserved for future use); if held, do nothing (reserved for future use)
    case 3:
      if (button->press_flag)
      {
        // Reserved
      }
      else if (button->hold_flag)
      {
        // Reserved
      }
      break;

    // Button 4: If pressed, cycle through the modes; if held, do nothing (reserved for future use)
    case 4:
      if (button->press_flag || (button->hold_flag && !button->latch))
      {
        // system_state.mode = (system_state.mode < (SYSTEM_MODE_NUM - 1)) ? (system_state.mode + 1) : 0;

        // Cycle to the next mode in the system state
        system_state.mode = TIME_SETUP_MODE;

        // Set the system past mode to default mode
        system_state.past_mode = DEFAULT_MODE;

        // Set the Time Setup data to the current time values for convinience
        time_setup_data =
        (TIME_SETUP_DATA)
        {
          time_get_data.minute,       // Minutes: 0-59
          time_get_data.hour,         // Hours: 0-23
          time_get_data.dayofweek,    // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday)
          time_get_data.dateofmonth,  // Date of the month: 1-31
          time_get_data.month,        // Month: 1-12
          time_get_data.year          // Year: 0-99 (0 = 2000, 1 = 2001, ..., 99 = 2099)
        };

        // Reset the cursor for the Time Setup mode to the first parameter (minute)
        system_state.time_setup_cursor = TIME_MINUTE;

        button->latch = true;
      }
      // else if (button->hold_flag)
      // {
      //   // Reserved
      // }
      break;

    default: 
      break;
  }
}

/**
 * @brief  Handles the time setup mode based on button actions.
 * @param  button: Pointer to the BUTTON structure containing button state and index.
 * @retval None
 */
void System_Time_Setup_Mode_Handle (BUTTON_DATA *button)
{
  switch (button->index) 
  {
    // Button 0: If pressed, increment the selected parameter; if held, do nothing (reserved for future use)
    case 0: 
      // Pointer to the selected parameter, its maximum value, and minimum value
      uint8_t *paramPtr1 = NULL;
      uint8_t maxValue1 = 0;    
      uint8_t minValue1 = 0;  

      // Determine the parameter to increment based on the current selection
      switch (system_state.time_setup_cursor)
      {
        case TIME_MINUTE:  paramPtr1 = &time_setup_data.minute;  maxValue1 = 59; minValue1 = 0; break;
        case TIME_HOUR:    paramPtr1 = &time_setup_data.hour;    maxValue1 = 23; minValue1 = 0; break;
        case TIME_DOW:     paramPtr1 = &time_setup_data.dow;     maxValue1 = 7;  minValue1 = 1; break;
        case TIME_DOM:     paramPtr1 = &time_setup_data.dom;     maxValue1 = 31; minValue1 = 1; break;
        case TIME_MONTH:   paramPtr1 = &time_setup_data.month;   maxValue1 = 12; minValue1 = 1; break;
        case TIME_YEAR:    paramPtr1 = &time_setup_data.year;    maxValue1 = 99; minValue1 = 0; break;
        default: break;
      }

      // Check if the button is pressed or held
      if (button->press_flag) 
      {
        // Increment the selected parameter value once for a button press
        if (paramPtr1) 
        {
          // Increment the parameter value, wrapping around if necessary
          *paramPtr1 = (*paramPtr1 < maxValue1) ? (*paramPtr1 + 1) : minValue1;
        }
      }
      // else if (button->hold_flag) 
      // {
      // // Initialize the start tick for button 1 hold detection
      // uint32_t startTick1 = 0;

      //   // If the button is held down, increment the parameter value continuously
      //   if (HAL_GetTick() - startTick1 >= BUTTON_HOLD_CYCLE) 
      //   {
      //     if (paramPtr1) 
      //     {
      //       // Increment the parameter value, wrapping around if necessary
      //       *paramPtr1 = (*paramPtr1 < maxValue1) ? (*paramPtr1 + 1) : minValue1;
      //     }

      //     // Update the start tick for the next hold cycle
      //     startTick1 = HAL_GetTick();
      //   }
      // } 
      break;
    
      // Button 1: If pressed, decrement the selected parameter; if held, do nothing (reserved for future use)
      case 1: 
        // Pointer to the selected parameter, its maximum value, and minimum value
        uint8_t *paramPtr2 = NULL;
        uint8_t maxValue2 = 0;    
        uint8_t minValue2 = 0;  

        // Determine the parameter to decrement based on the current selection
        switch (system_state.time_setup_cursor)
        {
          case TIME_MINUTE:  paramPtr2 = &time_setup_data.minute;  maxValue2 = 59; minValue2 = 0; break;
          case TIME_HOUR:    paramPtr2 = &time_setup_data.hour;    maxValue2 = 23; minValue2 = 0; break;
          case TIME_DOW:     paramPtr2 = &time_setup_data.dow;     maxValue2 = 7;  minValue2 = 1; break;
          case TIME_DOM:     paramPtr2 = &time_setup_data.dom;     maxValue2 = 31; minValue2 = 1; break;
          case TIME_MONTH:   paramPtr2 = &time_setup_data.month;   maxValue2 = 12; minValue2 = 1; break;
          case TIME_YEAR:    paramPtr2 = &time_setup_data.year;    maxValue2 = 99; minValue2 = 0; break;
          default: break;
        }

        // Check if the button is pressed or held
        if (button->press_flag) 
        {
          // Decrement the selected parameter value once for a button press
          if (paramPtr2) 
          {
            // Decrement the parameter value, wrapping around if necessary
            *paramPtr2 = (*paramPtr2 > minValue2) ? (*paramPtr2 - 1) : maxValue2;
          }
        }
        // else if (button->hold_flag) 
        // {
        //   // Initialize the start tick for button 2 hold detection
        //   uint32_t startTick2 = 0;

        //   // If the button is held down, decrement the parameter value continuously
        //   if (HAL_GetTick() - startTick2 >= BUTTON_HOLD_CYCLE) 
        //   {
        //     if (paramPtr2) 
        //     {
        //       // Decrement the parameter value, wrapping around if necessary
        //       *paramPtr2 = (*paramPtr2 > minValue2) ? (*paramPtr2 - 1) : maxValue2;
        //     }

        //     // Update the start tick for the next hold cycle
        //     startTick2 = HAL_GetTick();
        //   }
        // } 
        break;

    // Button 2: If pressed, set the time using the current parameter values; if held, do nothing (reserved for future use)
    case 2: 
      // If the button is pressed, move to the previous parameter field, wrapping around if necessary
      if (button->press_flag)
      {
        // Set the time using the current parameter values
        Time_Init
        (
          0, 
          time_setup_data.minute, 
          time_setup_data.hour, 
          time_setup_data.dow, 
          time_setup_data.dom, 
          time_setup_data.month, 
          time_setup_data.year
        );
      }
      
      // If the button is held down, do nothing (reserved for future use)
      else if (button->hold_flag) 
      {
        // Reserved
      }
      break;

    // Button 3: If pressed, move to the next field; if held, do nothing (reserved for future use)
    case 3: 
      // If the button is pressed, move to the next parameter field, wrapping around if necessary
      if(button->press_flag)
      {
        system_state.time_setup_cursor = (system_state.time_setup_cursor == TIME_YEAR) ? TIME_MINUTE : (system_state.time_setup_cursor + 1);
      }

      // // If the button is held down, set the time using the current parameter values
      // else if (button->hold_flag && !button->latch) 
      // {
      //   // Set the time using the current parameter values
      //   //    void Time_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
      //   Time_Init
      //   (
      //     0, 
      //     system_param_data.minute, 
      //     system_param_data.hour, 
      //     system_param_data.dow, 
      //     system_param_data.dom, 
      //     system_param_data.month, 
      //     system_param_data.year
      //   );

      //   button->latch = true
      break;

    // Button 4: If pressed, cycle through the system modes; if held, do nothing (reserved for future use)
    case 4:
      // If pressed, cycle through the time system modes, but the temporary setting data is still retained (avoid accidental press)
      if (button->press_flag)
      {
        // system_state.mode = (system_state.mode < (SYSTEM_MODE_NUM - 1)) ? (system_state.mode + 1) : 0;

        // Cycle through the next system mode
        system_state.mode = ALARM_SETUP_MODE;

        // Set the system past mode to time setup mode
        system_state.past_mode = TIME_SETUP_MODE;

        // Set the Alarm Setup data to the current time values for convenience
        alarm_setup_data =
        (ALARM_SETUP_DATA)
        {
          time_get_data.minute,       // Minutes: 0-59
          time_get_data.hour,         // Hours: 0-23
          NOT_USED_MODE,              // Select: DAY_OF_WEEK_MODE, DATE_OF_MONTH_MODE, NOT_USED_MODE         
          time_get_data.dayofweek,    // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday)
          true                       	// Alarm ON/OFF state: 1 = ON, 0 = OFF
        };
        
        // Reset the cursor for the Alarm Setup mode to the first parameter (minute)
        system_state.alarm_setup_cursor = ALARM_MINUTE;
      }

      // // If held, set the mode to default, 
      // // and refresh the system parameters to current time values and default settings for convenience
      // else if (button->hold_flag && !button->latch)
      // {
      //   system_state.mode = DEFAULT_MODE; 
      //   button->latch = true;

      //   // Set the initial system parameters to current time values and default settings for convenience
      //   system_param_data =
      //   (SYSTEM_PARAM_DATA)
      //   {
      //     time_get_data.minute,       // Minutes: 0-59
      //     time_get_data.hour,         // Hours: 0-23
      //     time_get_data.dayofweek,    // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday)
      //     time_get_data.dateofmonth,  // Date of the month: 1-31
      //     time_get_data.month,        // Month: 1-12
      //     time_get_data.year,         // Year: 0-99 (0 = 2000, 1 = 2001, ..., 99 = 2099)
      //     NOT_USED_MODE,              // Select: DAY_OF_WEEK_MODE, DATE_OF_MONTH_MODE, NOT_USED_MODE
      //     time_get_data.dayofweek,    // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday), or Date of the month: 1-31
      //     true                        // true = ON, false = OFF
      //   };
      // }
      break;

    default: 
      break;
  }
}

/**
 * @brief  Handles the alarm setup mode based on button actions.
 * @param  button: Pointer to the BUTTON structure containing button state and index.
 * @retval None
 */
void System_Alarm_Setup_Mode_Handle (BUTTON_DATA *button)
{ 
  switch (button->index) 
  {
    // Button 0: If pressed, increment the selected parameter; if held, do nothing (reserved for future use)
    case 0: 
      // Pointer to the selected parameter, its maximum value, and minimum value
      uint8_t *paramPtr1 = NULL;
      uint8_t maxValue1 = 0;    
      uint8_t minValue1 = 0;  

      // Determine the parameter to increment based on the current selection
      switch (system_state.alarm_setup_cursor)
      {
        case ALARM_MINUTE:  paramPtr1 = &alarm_setup_data.minute;  maxValue1 = 59; minValue1 = 0; break;
        case ALARM_HOUR:    paramPtr1 = &alarm_setup_data.hour;    maxValue1 = 23; minValue1 = 0; break;
        case ALARM_DY_DT:   paramPtr1 = (uint8_t *)&alarm_setup_data.dy_dt; maxValue1 = 2; minValue1 = 0; break;
        case ALARM_DOW_DOM: paramPtr1 = &alarm_setup_data.dow_dom; maxValue1 = 31; minValue1 = 1; break;
        case ALARM_ON_OFF:  alarm_setup_data.on_off = !alarm_setup_data.on_off;                   break;
        default: break;
      }

      if (system_state.alarm_setup_cursor == ALARM_ON_OFF)
      {
        break;
      }

      // Check if the button is pressed or held
      if (button->press_flag) 
      {
        // Increment the selected parameter value once for a button press
        if (paramPtr1) 
        {
          // Increment the parameter value, wrapping around if necessary
          *paramPtr1 = (*paramPtr1 < maxValue1) ? (*paramPtr1 + 1) : minValue1;
        }
      }
      // else if (button->hold_flag) 
      // {
      //   // Initialize the start tick for button 1 hold detection
      //   uint32_t startTick1 = 0;   
        
      //   // If the button is held down, increment the parameter value continuously
      //   if (HAL_GetTick() - startTick1 >= BUTTON_HOLD_CYCLE) 
      //   {
      //     // Increment the selected parameter value continuously
      //     if (paramPtr1) 
      //     {
      //       // Increment the parameter value, wrapping around if necessary
      //       *paramPtr1 = (*paramPtr1 < maxValue1) ? (*paramPtr1 + 1) : minValue1;
      //     }
      //   }

      //   // Update the start tick for the next hold cycle
      //   startTick1 = HAL_GetTick();
      // } 
      break;

      // Button 1: If pressed, decrement the selected parameter; if held, do nothing (reserved for future use)
      case 1: 
        // Pointer to the selected parameter, its maximum value, and minimum value
        uint8_t *paramPtr2 = NULL;
        uint8_t maxValue2 = 0;    
        uint8_t minValue2 = 0;  

        // Determine the parameter to decrement based on the current selection
        switch (system_state.alarm_setup_cursor)
        {
          case ALARM_MINUTE:  paramPtr2 = &alarm_setup_data.minute;  maxValue2 = 59; minValue2 = 0; break;
          case ALARM_HOUR:    paramPtr2 = &alarm_setup_data.hour;    maxValue2 = 23; minValue2 = 0; break;
          case ALARM_DY_DT:   paramPtr2 = (uint8_t *)&alarm_setup_data.dy_dt; maxValue2 = 2; minValue2 = 0; break;
          case ALARM_DOW_DOM: paramPtr2 = &alarm_setup_data.dow_dom; maxValue2 = 31; minValue2 = 1; break;
          case ALARM_ON_OFF:  alarm_setup_data.on_off = !alarm_setup_data.on_off;                  break;
          default: break;
        }

        // Check if the selected parameter is ON/OFF
        if (system_state.alarm_setup_cursor == ALARM_ON_OFF)
        {
          break;
        }

        // Check if the button is pressed or held
        if (button->press_flag) 
        {
          // Check if the selected parameter is ON/OFF
          if (system_state.alarm_setup_cursor == ALARM_ON_OFF)
          {
            // Toggle the on/off state of the alarm
            alarm_setup_data.on_off = !alarm_setup_data.on_off;
          } 
          // Else, decrement the selected parameter value once for a button press
          else 
          {
            // Decrement the selected parameter value once for a button press
            if (paramPtr2) 
            {
              // Decrement the parameter value, wrapping around if necessary
              *paramPtr2 = (*paramPtr2 > minValue2) ? (*paramPtr2 - 1) : maxValue2;
            }
          }
        }
        // else if (button->hold_flag) 
        // {
        //   // Initialize the start tick for button 2 hold detection
        //   uint32_t startTick2 = 0;

        //   // If the button is held down, decrement the parameter value continuously
        //   if (HAL_GetTick() - startTick2 >= BUTTON_HOLD_CYCLE) 
        //   {
        //     // Check if the selected parameter is ON/OFF
        //     if (system_state.time_setup_cursor == SET_ON_OFF) 
        //     {
        //       // Toggle the on/off state of the alarm
        //       system_param_data.on_off = !system_param_data.on_off;
        //     } 
        //     // Else, decrement the selected parameter value continuously
        //     else 
        //     {
        //       // Decrement the selected parameter value continuously
        //       if (paramPtr2) 
        //       {
        //         // Decrement the parameter value, wrapping around if necessary
        //         *paramPtr2 = (*paramPtr2 > minValue2) ? (*paramPtr2 - 1) : maxValue2;
        //       }
        //     }

        //     // Update the start tick for the next hold cycle
        //     startTick2 = HAL_GetTick();
        //   }
        // } 
        break;
    
    // Button 2: If pressed, save the alarm and return to default mode; if held, do nothing (reserved for future use)
    case 2:
      // If the button is pressed, save the alarm and return to default mode 
      if (button->press_flag)
      {
        // If the previous mode is Alarm View Mode
        // Use the system cursor instead to keep the alarm slot pointer unchanged
        if (system_state.past_mode == ALARM_VIEW_MODE)
        {
          // Save the alarm to EEPROM and return to Default Mode
          //    void Alarm_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, ALARM_DY_DT_MODE dy_dt, uint8_t on_off, uint8_t slot)
          Alarm_Set
          (
            0,  
            alarm_setup_data.minute,
            alarm_setup_data.hour,
            alarm_setup_data.dow_dom,
            alarm_setup_data.dy_dt,
            alarm_setup_data.on_off,
            system_state.alarm_view_cursor         // Save the alarm at the address where the cursor points at in Alarm View Mode
          );

          // Update the newly set alarm data
          Alarm_Get(system_state.alarm_view_cursor, &alarm_get_data[system_state.alarm_view_cursor]);
        }
        else
        {
          // Save the alarm to EEPROM and return to Default Mode
          //    void Alarm_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, ALARM_DY_DT_MODE dy_dt, uint8_t on_off, uint8_t slot)
          Alarm_Set
          (
            0,  
            alarm_setup_data.minute,
            alarm_setup_data.hour,
            alarm_setup_data.dow_dom,
            alarm_setup_data.dy_dt,
            alarm_setup_data.on_off,
            alarm_slot_ptr              // Pointer to the next available slot in the EEPROM module
          );

          // Update the newly set alarm data
          Alarm_Get(alarm_slot_ptr, &alarm_get_data[alarm_slot_ptr]);
          
          // Increment the pointer to the next available slot in the EEPROM module
          alarm_slot_ptr = (alarm_slot_ptr < ALARM_SLOT_NUM) ? (alarm_slot_ptr + 1) : 0;

          // Save the alarm pointer data to the EEPROM module
          Alarm_Slot_Pointer_Set();
        }
      }

      // If the button is held down, do nothing (reserved for future use)
      else if (button->hold_flag) 
      {
        // Reserved
      }
      break;

    // Button 3: If pressed, move to the next field; if held, do nothing (reserved for future use)
    case 3:
      // If the button is pressed, move to the next parameter field, wrapping around if necessary  
      if (button->press_flag)
      {
        system_state.alarm_setup_cursor = (system_state.alarm_setup_cursor == ALARM_ON_OFF) ? ALARM_MINUTE : (system_state.alarm_setup_cursor + 1);
      }  
      // // If the button is held down, save the alarm and return to default mode
      // else if (button->hold_flag && !button->latch) 
      // {
      //   // If the previous mode is Alarm View Mode
      //   // Use the system cursor instead to keep the alarm slot pointer unchanged
      //   if (system_state.past_mode == ALARM_VIEW_MODE)
      //   {
      //     // Save the alarm to EEPROM and return to Default Mode
      //     //    void Alarm_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, ALARM_DY_DT_MODE dy_dt, uint8_t on_off, uint8_t slot)
      //     Alarm_Set
      //     (
      //       0,  
      //       system_param_data.minute,
      //       system_param_data.hour,
      //       system_param_data.dow_dom,
      //       system_param_data.dy_dt,
      //       system_param_data.on_off,
      //       system_state.cursor         // Save the alarm at the address where the cursor points at in Alarm View Mode
      //     );

      //     // Update the newly set alarm data
      //     Alarm_Get(system_state.cursor, &alarm_get_data[system_state.cursor]);
      //   }
      //   else
      //   {
      //     // Save the alarm to EEPROM and return to Default Mode
      //     //    void Alarm_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, ALARM_DY_DT_MODE dy_dt, uint8_t on_off, uint8_t slot)
      //     Alarm_Set
      //     (
      //       0,  
      //       system_param_data.minute,
      //       system_param_data.hour,
      //       system_param_data.dow_dom,
      //       system_param_data.dy_dt,
      //       system_param_data.on_off,
      //       alarm_slot_ptr              // Pointer to the next available slot in the EEPROM module
      //     );

      //     // Update the newly set alarm data
      //     Alarm_Get(alarm_slot_ptr, &alarm_get_data[alarm_slot_ptr]);
          
      //     // Increment the pointer to the next available slot in the EEPROM module
      //     alarm_slot_ptr = (alarm_slot_ptr < ALARM_SLOT_NUM) ? (alarm_slot_ptr + 1) : 0;

      //     // Save the alarm pointer data to the EEPROM module
      //     Alarm_Slot_Pointer_Set();
      //   }

      //   // Reset the button latch to avoid function replication
      //   button->latch = true;
      // }
      break;
    
    // Button 4: If pressed, cycle through the system modes; if held, do nothing (reserved for future use)
    case 4:
      // If pressed, cycle through the time system modes, but the temporary setting data is still retained (avoid accidental press)
      if (button->press_flag)
      {
        // system_state.mode = (system_state.mode < (SYSTEM_MODE_NUM - 1)) ? (system_state.mode + 1) : 0;
        
        // Cycle through the next system mode
        system_state.mode = ALARM_VIEW_MODE;

        // Set the system past mode to alarm setup mode
        system_state.past_mode = ALARM_SETUP_MODE;

        // Reset the cursor for the Alarm View mode
        system_state.alarm_view_cursor = 0; 
      }
      
      // // If held, set the mode to default, 
      // // and refresh the system parameters to current time values and default settings for convenience
      // else if (button->hold_flag && !button->latch)
      // {
      //   system_state.mode = DEFAULT_MODE; 
      //   button->latch = true;

      //   // Set the initial system parameters to current time values and default settings for convenience
      //   system_param_data =
      //   (SYSTEM_PARAM_DATA)
      //   {
      //     time_get_data.minute,       // Minutes: 0-59
      //     time_get_data.hour,         // Hours: 0-23
      //     time_get_data.dayofweek,    // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday)
      //     time_get_data.dateofmonth,  // Date of the month: 1-31
      //     time_get_data.month,        // Month: 1-12
      //     time_get_data.year,         // Year: 0-99 (0 = 2000, 1 = 2001, ..., 99 = 2099)
      //     NOT_USED_MODE,              // Select: DAY_OF_WEEK_MODE, DATE_OF_MONTH_MODE, NOT_USED_MODE
      //     time_get_data.dayofweek,    // Day of the week: 1-7 (1 = Sunday, 2 = Monday, ..., 7 = Saturday), or Date of the month: 1-31
      //     true                        // true = ON, false = OFF
      //   };
      // }
      break;

    default:
      // Reserved for future use
      break;
  }
}

/**
 * @brief  Handles the alarm view mode based on button actions.
 * @param  button: Pointer to the BUTTON structure containing button state and index.
 * @retval None
 */
void System_Alarm_View_Mode_Handle (BUTTON_DATA *button)
{ 
  // Handle button actions in alarm view mode
  switch (button->index) 
  {
    // Button 0: If pressed, increment the selection cursor; if held, do nothing (reserved for future use)
    case 0:
      if      (button->press_flag)
      {
        system_state.alarm_view_cursor = (system_state.alarm_view_cursor == ALARM_VIEW_CURSOR_MAX) ? 0 : (system_state.alarm_view_cursor + 1);
      }
      else if (button->hold_flag)
      {
        // Reserved
      }
      break;

    // Button 1: If pressed, decrement the selection cursor; if held, do nothing (reserved for future use)
    case 1:
      if      (button->press_flag)
      {
        system_state.alarm_view_cursor = (system_state.alarm_view_cursor == 0) ? ALARM_VIEW_CURSOR_MAX : (system_state.alarm_view_cursor - 1);
      }
      else if (button->hold_flag)
      {
        // Reserved
      }
      break;
    
    // Button 2: If pressed, toggle ON/OFF; if held, do nothing (reserved for future use)
    case 2:
      if      (button->press_flag)
      {
        // Retrieve the selected alarm data to system parameter data
        alarm_setup_data.minute  = alarm_get_data[system_state.alarm_view_cursor].minute;
        alarm_setup_data.hour    = alarm_get_data[system_state.alarm_view_cursor].hour;
        alarm_setup_data.dy_dt   = alarm_get_data[system_state.alarm_view_cursor].dy_dt;
        alarm_setup_data.dow_dom = alarm_get_data[system_state.alarm_view_cursor].dow_dom;
        alarm_setup_data.on_off  = alarm_get_data[system_state.alarm_view_cursor].on_off;

        // Toggle ON/OFF data
        alarm_setup_data.on_off = !alarm_setup_data.on_off;

        // Save the modified alarm to the EEPROM module
        //    void Alarm_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, ALARM_DY_DT_MODE dy_dt, uint8_t on_off, uint8_t slot)
        Alarm_Set
        (
          0,  
          alarm_setup_data.minute,
          alarm_setup_data.hour,
          alarm_setup_data.dow_dom,
          alarm_setup_data.dy_dt,
          alarm_setup_data.on_off,
          system_state.alarm_view_cursor   
        );

        // Update the newly set alarm data
        Alarm_Get(system_state.alarm_view_cursor, &alarm_get_data[system_state.alarm_view_cursor]);
      }
      // else if (button->hold_flag)
      // {
      //   // Retrieve the selected alarm data to system parameter data
      //   system_param_data.minute  = alarm_get_data[system_state.cursor].minute;
      //   system_param_data.hour    = alarm_get_data[system_state.cursor].hour;
      //   system_param_data.dy_dt   = alarm_get_data[system_state.cursor].dy_dt;
      //   system_param_data.dow_dom = alarm_get_data[system_state.cursor].dow_dom;
      //   system_param_data.on_off  = alarm_get_data[system_state.cursor].on_off;

      //   // System switches to Alarm Setup Mode
      //   system_state.mode = ALARM_SETUP_MODE;
      // }
      break;

    // Button 3: If pressed, enter Alarm Setup Mode with selected alarm data preloaded (for editing); if held, do nothing (reserved for future use)
    case 3: 
      if      (button->press_flag)
      {
        // Retrieve the selected alarm data to system parameter data
        alarm_setup_data.minute  = alarm_get_data[system_state.alarm_view_cursor].minute;
        alarm_setup_data.hour    = alarm_get_data[system_state.alarm_view_cursor].hour;
        alarm_setup_data.dy_dt   = alarm_get_data[system_state.alarm_view_cursor].dy_dt;
        alarm_setup_data.dow_dom = alarm_get_data[system_state.alarm_view_cursor].dow_dom;
        alarm_setup_data.on_off  = alarm_get_data[system_state.alarm_view_cursor].on_off;

        // System switches to Alarm Setup Mode
        system_state.mode = ALARM_SETUP_MODE;

        // Set the system past mode to alarm view mode
        system_state.past_mode = ALARM_VIEW_MODE;
      }
      // else if (button->hold_flag)
      // {
      //   // Clear current alarm
      //   Alarm_Clear(system_state.cursor);

      //   // Update alarm data
      //   Alarm_Get(system_state.cursor, &alarm_get_data[system_state.cursor]);
      // }
      break;

    // Button 4: If pressed, cycle through the time setup fields; if held, do nothing (reserved for future use)
    case 4:
      // If pressed, cycle to the next system mode
      if (button->press_flag)
      {
        // Cycle through the next system mode
        system_state.mode = SYSTEM_OPTIONS_MODE;

        // Set the system past mode to alarm view mode
        system_state.past_mode = ALARM_VIEW_MODE;
      }
      // // If held, set the mode to default
      // else if (button->hold_flag && !button->latch)
      // {
      //   system_state.mode = DEFAULT_MODE; 
      //   button->latch = true;
      // }
      break;

    default: 
      break;
  }
}
/**
 * @brief  Handles the system options mode based on button actions.
 * @param  button: Pointer to the BUTTON structure containing button state and index.
 * @retval None
 */
void System_Options_Mode_Handle (BUTTON_DATA *button)
{
  switch (button->index) 
  {
    // Button 0: If pressed, increment the selection cursor; if held, do nothing (reserved for future use)
    case 0:
      if      (button->press_flag)
      {
        system_state.system_opt_cursor = (system_state.system_opt_cursor == SYSTEM_OPT_CURSOR_MAX) ? 0 : (system_state.system_opt_cursor + 1);
      }
      else if (button->hold_flag)
      {
        // Reserved
      }
      break;
    
    // Button 1: If pressed, decrement the selection cursor; if held, do nothing (reserved for future use)
    case 1:
      if      (button->press_flag)
      {
        system_state.system_opt_cursor = (system_state.system_opt_cursor == 0) ? SYSTEM_OPT_CURSOR_MAX : (system_state.system_opt_cursor - 1);
      }
      else if (button->hold_flag)
      {
        // Reserved
      }
      break;
    
    // Button 2: If pressed, operate the selected option; if held, do nothing (reserved for future use)
    case 2:
      if      (button->press_flag)
      {
        switch (system_state.system_opt_cursor)
        {
          case CLEAR_ALL_ALARM:
            for (int i = 0; i < ALARM_SLOT_NUM; i++)
            {
              // Clear all alarms in the EEPROM module
              Alarm_Clear(i);

              // Update the alarm data
              Alarm_Get(i, &alarm_get_data[i]);
            }
            // Reset the alarm slot pointer to 0
            alarm_slot_ptr = 0;

            // Save the alarm slot pointer data to the EEPROM module
            Alarm_Slot_Pointer_Set();
            break;

          case CONTRIBUTOR_INFO:
            // Display contributor informatio
            E_ink_display_name();
            
            break;

          default:
            break;
        }
      }
      else if (button->hold_flag)
      {
        // Reserved
      }
      break;

    case 3: 
      if      (button->press_flag)
      {

      }
      else if (button->hold_flag)
      {
        // Reserved
      }
      break;
    
    // Button 4: If pressed, cycle through the time setup fields; if held, do nothing (reserved for future use)
    case 4:
      // If pressed, cycle through the next system mode
      if      (button->press_flag)
      {
        // system_state.mode = (system_state.mode < (SYSTEM_MODE_NUM - 1)) ? (system_state.mode + 1) : 0;

        // Cycle through the next system mode
        system_state.mode = DEFAULT_MODE;

        // Set the system past mode to system options mode
        system_state.past_mode = SYSTEM_OPTIONS_MODE;
      }
      // // If held, set the mode to default
      // else if (button->hold_flag)
      // {
      //   system_state.mode = DEFAULT_MODE;
      // }
      break;

    default: 
      break;
  }
}

/**
 * @brief  Handles the alarm active mode based on button actions.
 * @param  button: Pointer to the BUTTON structure containing button state and index.
 * @retval None
 */
void System_Alarm_Active_Mode_Handle (BUTTON_DATA *button)
{
  // Handle button actions in alarm active mode
  switch (button->index) 
  {
    // Button 0: If pressed, stop ringing the alarm; if held, do nothing (reserved for future use)
    case 0:
      // If pressed, cycle through the time setup fields
      if (button->press_flag)
      {
        // Stop ringing the alarm
        alarm_active_flag = false; 

        // Return to the previous mode
        system_state.mode = system_state.past_mode; 

        // Set the system past mode to alarm active mode
        system_state.past_mode = ALARM_ACTIVE_MODE; 

        // Reset the buzzer
        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, BUZZER_INACTIVE); 
      }
      // If held, do nothing (reserved for future use)
      else if (button->hold_flag && !button->latch)
      {
        // Reserved
      }
      break;

    // Button 1: If pressed, stop ringing the alarm; if held, do nothing (reserved for future use)
    case 1:
      if      (button->press_flag)
      {
        // Stop ringing the alarm
        alarm_active_flag = false;  

        // Return to the previous mode
        system_state.mode = system_state.past_mode; 

        // Set the system past mode to alarm active mode
        system_state.past_mode = ALARM_ACTIVE_MODE; 

        // Reset the buzzer
        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, BUZZER_INACTIVE); 
      }
      else if (button->hold_flag)
      {
        // Reserved 
      }
      break;

    // Button 2: If pressed, stop ringing the alarm; if held, do nothing (reserved for future use)
    case 2:
      if      (button->press_flag)
      {
        // Stop ringing the alarm
        alarm_active_flag = false;  

        // Return to the previous mode
        system_state.mode = system_state.past_mode; 

        // Set the system past mode to alarm active mode
        system_state.past_mode = ALARM_ACTIVE_MODE; 

        // Reset the buzzer
        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, BUZZER_INACTIVE); 
      }
      else if (button->hold_flag)
      {
        // Reserved
      }
      break;
    
    // Button 3: If pressed, stop ringing the alarm; if held, do nothing (reserved for future use)
    case 3:
      if      (button->press_flag)
      {
        // Stop ringing the alarm
        alarm_active_flag = false;  

        // Return to the previous mode
        system_state.mode = system_state.past_mode; 

        // Set the system past mode to alarm active mode
        system_state.past_mode = ALARM_ACTIVE_MODE; 

        // Reset the buzzer
        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, BUZZER_INACTIVE); 
      }
      else if (button->hold_flag)
      {
        // Reserved
      }
      break;

    // Button 4: If pressed, stop ringing the alarm; if held, do nothing (reserved for future use)
    case 4: 
      if      (button->press_flag)
      {
        // Stop ringing the alarm
        alarm_active_flag = false;  

        // Return to the previous mode
        system_state.mode = system_state.past_mode; 

        // Set the system past mode to alarm active mode
        system_state.past_mode = ALARM_ACTIVE_MODE; 

        // Reset the buzzer
        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, BUZZER_INACTIVE); 
      }
      else if (button->hold_flag)
      {
        // Reserved
      }
      break;

    default: 
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
    
    case BUTTON0_IN12_Pin:
      // Set the Button 0 Interrupt Flag
      button0.int_flag = true;
      break;

    case BUTTON1_IN13_Pin:
      // Set the Button 1 Interrupt Flag
      button1.int_flag = true;
      break;

    case BUTTON2_IN14_Pin:
      // Set the Button 2 Interrupt Flag
      button2.int_flag = true;
      break;

    case BUTTON3_IN15_Pin:
      // Set the Button 3 Interrupt Flag
      button3.int_flag = true;
      break;  

    case BUTTON4_IN8_Pin:
      // Set the Button 4 Interrupt Flag
      button4.int_flag = true;
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
    uart_hour 	  = uart_rx_data[0];
    uart_minute   = uart_rx_data[1];
    uart_dow      = uart_rx_data[5];
    uart_day 	    = uart_rx_data[2];
    uart_month	  = uart_rx_data[3];
    uart_year 	  = uart_rx_data[4];
    uart_alarmNum = uart_rx_data[5];
    uart_alarmSta = uart_rx_data[6];

    // For mode 2:
    if (uart_appMode == 2)
    {
      for (int i = 0; i < 7; i++)
      {
        alarm_Info[uart_alarmNum-1][i] = uart_rx_data[i]; // Store the alarm information in the array. Alarm number is uart_alarmNum, information is uart_rx_data[i]
      }
    }

    // Set the UART receive flag
    uart_rx_flag = 1;
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  // Verify the UART instance to ensure the callback is for USART1
  // If the UART instance is USART1, set the UART transmit flag
  if(huart->Instance == USART1)
  {
    // Set the UART transmit flag to indicate that the data has been sent
    sentCount += 1;

    if (sentCount == 10) // If 10 alarms are sent, reset the sentCount variable
    {
      sentCount = 0;
      uart_tx_flag = 0; // Reset the UART transmit flag
      uart_rx_data[0] = 0b01110000;
    }
    else
    {
      uart_tx_flag = 1;
    }
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

void Update_alarm_data(void)
{
  sParam_data_alarm_setup_mode_1.hour     = alarm_get_data[0].hour;
  sParam_data_alarm_setup_mode_1.minute   = alarm_get_data[0].minute;
  sParam_data_alarm_setup_mode_1.dy_dt    = alarm_get_data[0].dy_dt;
  sParam_data_alarm_setup_mode_1.dow_dom  = alarm_get_data[0].on_off;
  sParam_data_alarm_setup_mode_1.on_off   = alarm_get_data[0].on_off;
  sParam_data_alarm_setup_mode_1.second   = 0;

  sParam_data_alarm_setup_mode_2.hour     = alarm_get_data[1].hour;
  sParam_data_alarm_setup_mode_2.minute   = alarm_get_data[1].minute;
  sParam_data_alarm_setup_mode_2.dy_dt    = alarm_get_data[1].dy_dt;
  sParam_data_alarm_setup_mode_2.dow_dom  = alarm_get_data[1].on_off;
  sParam_data_alarm_setup_mode_2.on_off   = alarm_get_data[1].on_off;
  sParam_data_alarm_setup_mode_2.second   = 0;

  sParam_data_alarm_setup_mode_3.hour     = alarm_get_data[2].hour;
  sParam_data_alarm_setup_mode_3.minute   = alarm_get_data[2].minute;
  sParam_data_alarm_setup_mode_3.dy_dt    = alarm_get_data[2].dy_dt;
  sParam_data_alarm_setup_mode_3.dow_dom  = alarm_get_data[2].on_off;
  sParam_data_alarm_setup_mode_3.on_off   = alarm_get_data[2].on_off;
  sParam_data_alarm_setup_mode_3.second   = 0;

  sParam_data_alarm_setup_mode_4.hour     = alarm_get_data[3].hour;
  sParam_data_alarm_setup_mode_4.minute   = alarm_get_data[3].minute;
  sParam_data_alarm_setup_mode_4.dy_dt    = alarm_get_data[3].dy_dt;
  sParam_data_alarm_setup_mode_4.dow_dom  = alarm_get_data[3].on_off;
  sParam_data_alarm_setup_mode_4.on_off   = alarm_get_data[3].on_off;
  sParam_data_alarm_setup_mode_4.second   = 0;

  sParam_data_alarm_setup_mode_5.hour     = alarm_get_data[4].hour;
  sParam_data_alarm_setup_mode_5.minute   = alarm_get_data[4].minute;
  sParam_data_alarm_setup_mode_5.dy_dt    = alarm_get_data[4].dy_dt;
  sParam_data_alarm_setup_mode_5.dow_dom  = alarm_get_data[4].on_off;
  sParam_data_alarm_setup_mode_5.on_off   = alarm_get_data[4].on_off;
  sParam_data_alarm_setup_mode_5.second   = 0;

  sParam_data_alarm_setup_mode_6.hour     = alarm_get_data[5].hour;
  sParam_data_alarm_setup_mode_6.minute   = alarm_get_data[5].minute;
  sParam_data_alarm_setup_mode_6.dy_dt    = alarm_get_data[6].dy_dt;
  sParam_data_alarm_setup_mode_6.dow_dom  = alarm_get_data[5].on_off;
  sParam_data_alarm_setup_mode_6.on_off   = alarm_get_data[5].on_off;
  sParam_data_alarm_setup_mode_6.second   = 0;

  sParam_data_alarm_setup_mode_7.hour     = alarm_get_data[6].hour;
  sParam_data_alarm_setup_mode_7.minute   = alarm_get_data[6].minute;
  sParam_data_alarm_setup_mode_7.dy_dt    = alarm_get_data[6].dy_dt; 
  sParam_data_alarm_setup_mode_7.dow_dom  = alarm_get_data[6].on_off;
  sParam_data_alarm_setup_mode_7.on_off   = alarm_get_data[6].on_off;
  sParam_data_alarm_setup_mode_7.second   = 0;

  sParam_data_alarm_setup_mode_8.hour     = alarm_get_data[7].hour;
  sParam_data_alarm_setup_mode_8.minute   = alarm_get_data[7].minute;
  sParam_data_alarm_setup_mode_8.dy_dt    = alarm_get_data[7].dy_dt;
  sParam_data_alarm_setup_mode_8.dow_dom  = alarm_get_data[7].on_off;
  sParam_data_alarm_setup_mode_8.on_off   = alarm_get_data[7].on_off;
  sParam_data_alarm_setup_mode_8.second   = 0;

  sParam_data_alarm_setup_mode_9.hour     = alarm_get_data[8].hour;
  sParam_data_alarm_setup_mode_9.minute   = alarm_get_data[8].minute;
  sParam_data_alarm_setup_mode_9.dy_dt    = alarm_get_data[8].dy_dt;
  sParam_data_alarm_setup_mode_9.dow_dom  = alarm_get_data[8].on_off;
  sParam_data_alarm_setup_mode_9.on_off   = alarm_get_data[8].on_off;
  sParam_data_alarm_setup_mode_9.second   = 0;

  sParam_data_alarm_setup_mode_10.hour     = alarm_get_data[9].hour;
  sParam_data_alarm_setup_mode_10.minute   = alarm_get_data[9].minute;
  sParam_data_alarm_setup_mode_10.dy_dt    = alarm_get_data[9].dy_dt;
  sParam_data_alarm_setup_mode_10.dow_dom  = alarm_get_data[9].on_off;
  sParam_data_alarm_setup_mode_10.on_off   = alarm_get_data[9].on_off;
  sParam_data_alarm_setup_mode_10.second   = 0;

}

void Alarm_Ring (void)
{
  // Check if the buzzer cycle number is less than or equal to the maximum cycle number
  if (buzzer_cycle <= BUZZER_CYCLE_NUM)
  {
    // Check if the buzzer phase number is less than or equal to the maximum phase number
    if (buzzer_phase <= 4)
    {
      // Check if the buzzer tick is within the specified delay time
      if      ((buzzer_phase == 0) && (HAL_GetTick() - buzzer_tick <= BUZZER_SHORT_DELAY))
      {
        // Turn on the buzzer for a short duration
        HAL_GPIO_WritePin(GPIOB, BUZZER_Pin, BUZZER_ACTIVE);
      }
      else if ((buzzer_phase == 1) && (HAL_GetTick() - buzzer_tick <= BUZZER_SHORT_DELAY))
      {
        // Turn off the buzzer for a short duration
        HAL_GPIO_WritePin(GPIOB, BUZZER_Pin, BUZZER_INACTIVE);
      }
      else if ((buzzer_phase == 3) && (HAL_GetTick() - buzzer_tick <= BUZZER_MEDIUM_DELAY))
      {
        // Turn on the buzzer for a medium duration
        HAL_GPIO_WritePin(GPIOB, BUZZER_Pin, BUZZER_ACTIVE);
      }
      else if ((buzzer_phase == 4) && (HAL_GetTick() - buzzer_tick <= BUZZER_LONG_DELAY))
      {
        // Turn off the buzzer for a long duration
        HAL_GPIO_WritePin(GPIOB, BUZZER_Pin, BUZZER_INACTIVE);
      }
      else
      {
        // Reset the tick for the next phase
        buzzer_tick = HAL_GetTick(); 

        // Move to the next phase
        buzzer_phase++; 
      }
    }
    else
    {
      // Reset the tick for the next cycle
      buzzer_tick = HAL_GetTick(); 

      // Reset the phase counter
      buzzer_phase = 0; 

      // Move to the next cycle
      buzzer_cycle++; 
    }
  }
  else
  {
    // Stop ringing the alarm
    alarm_active_flag = false; 

    // Return to the previous mode
    system_state.mode = system_state.past_mode; 

    // Set the system past mode to alarm active mode
    system_state.past_mode = ALARM_ACTIVE_MODE; 

    // Reset the buzzer
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, BUZZER_INACTIVE); 
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
