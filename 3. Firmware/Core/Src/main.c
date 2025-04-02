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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// A structure of 7 one-byte unsigned characters to store 7 time values
typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t dayofweek;
	uint8_t dayofmonth;
	uint8_t month;
	uint8_t year;
} TIME;

// A structure of 4 one-byte unsigned characters to store 4 alarm values
typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t dow_dom;  // value of [Day of the week] or [Date of the month]
} ALARM;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// Slave address of DS3231 RTC module
#define DS3231_ADDRESS 0xD0

// Slave address of AT24C64D EEPROM module
#define EEPROM_ADDR 0xA0

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// Time variable
    // A variable to store the time values received from the RTC module every second
volatile TIME time_get;

// Alarm variable
    // A variable to store the alarm values received from the EEPROM module every second
volatile ALARM alarm_get;

// Flag variables
    // Flag for alarm check external interrupt (Alarm Check Flag) on PB4 (Activated every second)
volatile bool alarm_check_flag = 0;

// Pointer variables
    // Pointer variable to store the next available address (alarm) on the EEPROM module
uint8_t alarm_pointer = 0;

bool alarm_activated = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

// Function to convert a normal decimal number to a binary coded decimal (BCD) value
uint8_t Dec_To_BCD(int val);

// Function to convert a binary coded decimal (BCD) value to a normal decimal number
int BCD_To_Dec(uint8_t val);

// Function to initialize RTC module
void Time_Init (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year);

// Function to initially set time to the RTC module through I2C interface (Run only once after reset the RTC)
void Time_Set (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year);

// Function to get time from the RTC module through I2C interface
void Time_Get (void);

// Function to control settings of the RTC module (Alarm 1)
void Time_Ctrl (uint8_t mode, uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, bool dy_dt);

// Function to write a single alarm to the EEPROM module
void Alarm_Set (uint8_t adress, uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, bool on_off);

// Function to read a single alarm from the EEPROM module
void Alarm_Get (uint8_t adress);

// Function to check the alarms
void Alarm_Check (void);

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

  // Initialize RTC module (Run only once after reset the RTC module)
  //Time_Init(00, 53, 15, 3, 26, 3, 25);

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

  // Store values of a single alarm to the next available address on the EEPROM module
  //    void Alarm_Set (uint8_t adress, uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, bool on_off)
  Alarm_Set(0, 30, 59, 15, 0, 1);

  // Read values of a single alarm from a specific address on the EEPROM module
  //    void Alarm_Get (uint8_t adress)
  Alarm_Get(0);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */

    if (alarm_check_flag == 1)
    {
      // Retrieve the current time from the RTC module
      //    void Time_Get()
      Time_Get();

      // Check the alarms
      //    void Alarm_Check()
      Alarm_Check();

      // Reset the Alarm Check Flag
      alarm_check_flag = 0;
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
// Convert normal decimal numbers to binary coded decimal
uint8_t Dec_To_BCD(int val)
{
  return (uint8_t)( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
int BCD_To_Dec(uint8_t val)
{
  return (int)( (val/16*10) + (val%16) );
}

// Function to initially set time to the RTC module through I2C interface (Run only once after reset the RTC)
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
  HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x00, 1, setTime, sizeof(setTime), 1000);
}

// Function to get time from the RTC module through I2C interface
void Time_Get (void)
{
	// A blank array (7 slots) to contain the time values received from the RTC module
  uint8_t getTime[7];

  // Receive the time values from the RTC module through I2C interface, then store them into the blank array (size of value: 7 bytes)
  // HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);  
  HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x00, 1, getTime, sizeof(getTime), 1000);

  // Store the time values (converted from BCD code to decimal) into the time variable
	time_get.second     = BCD_To_Dec(getTime[0]);
	time_get.minute     = BCD_To_Dec(getTime[1]);
	time_get.hour       = BCD_To_Dec(getTime[2]);
	time_get.dayofweek  = BCD_To_Dec(getTime[3]);
	time_get.dayofmonth = BCD_To_Dec(getTime[4]);
	time_get.month      = BCD_To_Dec(getTime[5]);
	time_get.year       = BCD_To_Dec(getTime[6]);
}

// Function to control settings of the RTC module (Alarm 1)
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
  uint8_t dyDt = (dy_dt == 1) ? 64 : 0;

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
  HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x07, 1, ctrlTime, sizeof(ctrlTime), 1000);

  // Send the alarm control mask bits to the RTC module through I2C interface at address 0Eh (size of value: 1 byte)
  // HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);	  
  HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0E, 1, &ctrlAlarm, sizeof(ctrlAlarm), 1000);
}

// RTC module initialization
void Time_Init (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
{
  // Run only once after reset the RTC module to initially set the time
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
  Time_Ctrl (0, 0, 0, 0, 0, 0);   
}

// Write a single alarm to the EEPROM module
void Alarm_Set (uint8_t adress, uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, bool on_off)
{
  // A mask bit for On/ Off state of the alarm
  uint8_t onOff = 128;

  // For test: Try to add an On/ Off (1 bit) signal into the alarm pakage by using the empty MSB of the second register
  if (on_off == 1)
  {
    sec += onOff;
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
  HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, adress, 4, setAlarm, sizeof(setAlarm), 1000);
}

// Read a single alarm from the EEPROM module
void Alarm_Get (uint8_t adress)
{
  // A blank array (4 slots) to contain the alarm values received from the EEPROM module
  uint8_t getAlarm[4];

  // HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
  HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, adress, 4, getAlarm, sizeof(getAlarm), 1000);

  // Store the alarm values into the alarm variable
  alarm_get.second  = getAlarm[0];
  alarm_get.minute  = getAlarm[1];
  alarm_get.hour    = getAlarm[2];
  alarm_get.dow_dom = getAlarm[3];
}

uint8_t alarm_check_counter = 0;
uint8_t alarm_check_onoff = 0;
uint8_t alarm_check_match1 = 0;
uint8_t alarm_check_match2 = 0;
uint8_t alarm_check_match3 = 0;
uint8_t alarm_check_dowdom = 0;
uint8_t alarm_check_dow = 0;
uint8_t alarm_check_dom = 0;

// Function to check the alarms
void Alarm_Check (void)
{
  // Compare the current time with all alarms stored in the EEPROM module
  for (int i = 0; i <= alarm_pointer; i++)
  {
    // Retrieve the alarm values from the EEPROM module
    //    void Alarm_Get (uint8_t adress)
    Alarm_Get(i);

    // Check if the alarm is at ON or OFF state by checking the MSB of the second register
    if (alarm_get.second < 128)
    {
      // Stop checking other conditions if the alarm is at OFF state
      break;
    }

    // For test only
    alarm_check_onoff = 1;

    // Check if the current time matches the alarm time
        // Unmask the MSB of the second register to get the original value of the second register
    if ((alarm_get.second - 128 == time_get.second)  
      && (alarm_get.minute      == time_get.minute)
      && (alarm_get.hour        == time_get.hour))
    {
      // For test only
      alarm_check_match1 = 1;

      // Check if the alarm is at the [day of week]/ [date of month] mode by checking the MSB of the dow_dom register
      if (alarm_get.dow_dom >= 128)
      {
        // For test only
        alarm_check_dowdom = 1;
        
        // Check if the alarm is at the [day of week] mode by checking the mask bit (bit 6) of the dow_dom register
            // Unmask the MSB of the dow_dom register to get the original value of the dow_dom register
        if (alarm_get.dow_dom - 128 >= 64)
        {
          // For test only
          alarm_check_dow = 1;
          
          // Check if the [day of the week] matches the current time
              // Unmask MSB and bit 6 of the dow_dom register to get the original value of the dow_dom register
          if (alarm_get.dow_dom - 128 - 64 == time_get.dayofweek)
          {
            // For test only
            alarm_check_match2 = 1;

            // Alarm is triggered
            alarm_activated = 1;

            break;
          }
        }
        
        // If the alarm is at the [date of month] mode, check if the [date of month] matches the current time
        else if (alarm_get.dow_dom - 128 == time_get.dayofmonth)
        {
          // For test only
          alarm_check_dom = 1;
          
          // For test only
          alarm_check_match3 = 1;
          
          // Alarm is triggered
          alarm_activated = 1;

          break;
        }
      }
    }
    else
    {
      break;
    }

    alarm_check_counter += 1;
  }

  alarm_check_onoff = 0;
  alarm_check_match1 = 0;
  alarm_check_match2 = 0;
  alarm_check_match3 = 0;
  alarm_check_dowdom = 0;
  alarm_check_dow = 0;
  alarm_check_dom = 0;
}

// Callback function to handle external GPIO interrupts
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  // Check the external interrupt on PB4
  if(GPIO_Pin == RTC_TRIGGER_Pin)
  {
    // Set the Alarm Check Flag
    alarm_check_flag = 1;
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
