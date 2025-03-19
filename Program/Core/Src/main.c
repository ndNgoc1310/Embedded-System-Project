/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "stdbool.h"

/* Private defines -----------------------------------------------------------*/
// Slave adress of DS3231 RTC module
#define DS3231_ADDRESS 0xD0

// Slave address of AT24C64D EEPROM module
#define EEPROM_ADDR 0xA0 // Demo of AT24C256

/* Private dtypedefs ---------------------------------------------------------*/
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

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

// Time variable
TIME time_check;

// Alarm variable
ALARM alarm_check;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);


/* Private function declarations ---------------------------------------------*/
// Convert normal decimal numbers to binary coded decimal
uint8_t decToBcd(int val);

// Convert binary coded decimal to normal decimal numbers
int bcdToDec(uint8_t val);

// Function to initialize RTC module
void Clock_Init (void);

// Function to initially set time to the RTC module through I2C interface (Run only once after reset the RTC)
void Set_Time (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year);

// Function to get time from the RTC module through I2C interface
void Get_Time (void);

// Function to control settings of the RTC module (Alarm 1)
void Ctrl_Time (uint8_t mode, uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, bool dy_dt);

// Function to write a single alarm to the EEPROM module
void Alarm_Save (uint8_t adress, ALARM alarm, bool on_off);

// Function to read a single alarm from the EEPROM module
void Alarm_Check (uint8_t adress);

/* Main program --------------------------------------------------------------*/
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();

  // Initialize RTC module (Run only once after reset the RTC module)
  //Clock_Init();

  // Store a single alarm to the EEPROM module

  /* Infinite loop */
  while (1)
  {

  }
}

/* Private functions --------------------------------------------------------*/
// Write a single alarm to the EEPROM module
void Alarm_Save (uint8_t adress, ALARM alarm, bool on_off)
{
  // A mask bit for On/ Off state of the alarm
  uint8_t onOff = 128;

  if (on_off)
  {
    alarm.second += onOff;
  }

  // A blank array (4 slots) to contain the alarm values
  uint8_t set_alarm[4];
  
  // Store the alarm values into the blank array
  set_alarm[0] = alarm.second;
  set_alarm[1] = alarm.minute;
  set_alarm[2] = alarm.hour;
  set_alarm[3] = alarm.dow_dom;

  // HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
  HAL_I2C_Mem_Write(&hi2c2, EEPROM_ADDR, adress, 4, set_alarm, sizeof(set_alarm), 1000);
}

// Read a single alarm from the EEPROM module
void Alarm_Check (uint8_t adress)
{
  // A blank array (4 slots) to contain the alarm values received from the EEPROM module
  uint8_t get_alarm[4];

  // HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
  HAL_I2C_Mem_Read(&hi2c2, EEPROM_ADDR, adress, 4, get_alarm, sizeof(get_alarm), 1000);

  // Store the alarm values into the alarm variable
  alarm_check.second    = get_alarm[0];
  alarm_check.minute    = get_alarm[1];
  alarm_check.hour      = get_alarm[2];
  alarm_check.dow_dom   = get_alarm[3];
}

// Convert normal decimal numbers to binary coded decimal
uint8_t decToBcd(int val)
{
  return (uint8_t)( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
int bcdToDec(uint8_t val)
{
  return (int)( (val/16*10) + (val%16) );
}

// RTC module initialization
void Clock_Init (void)
{
  // Run only once after reset the RTC module to initially set the time
  //    Set_Time (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
  Set_Time (50, 04, 15, 3, 19, 3, 25);

  // Run only once after reset the RTC module to initially set the alarm
  //    Ctrl_Time (uint8_t mode, uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, bool dy_dt)
  //      Mode   : Alarm rate
  //        0    : Alarm once per second
  //        1    : Alarm when seconds match
  //        2    : Alarm when minutes and seconds match
  //        3    : Alarm when hours, minutes, and seconds match
  //        4    : Alarm when date, hours, minutes, and seconds match
  //        5    : Alarm when day, hours, minutes, and seconds match
  Ctrl_Time (0, 0, 0, 0, 0, 0);   
}

// Function to initially set time to the RTC module through I2C interface (Run only once after reset the RTC)
void Set_Time (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
{
	// A blank array (7 slots) to contain the time values
  uint8_t set_time[7];

  // Store the time values (converted in to BCD code) into the blank array
	set_time[0] = decToBcd(sec);
	set_time[1] = decToBcd(min);
	set_time[2] = decToBcd(hour);
	set_time[3] = decToBcd(dow);
	set_time[4] = decToBcd(dom);
	set_time[5] = decToBcd(month);
	set_time[6] = decToBcd(year);

  // Send the array containing the time values to the RTC module through I2C interface at address 00h - 06h (size of value: 7 bytes)
  // HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);	
  HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x00, 1, set_time, sizeof(set_time), 1000);
}

// Function to get time from the RTC module through I2C interface
void Get_Time (void)
{
	// A blank array (7 slots) to contain the time values received from the RTC module
  uint8_t get_time[7];

  // Receive the time values from the RTC module through I2C interface, then store them into the blank array (size of value: 7 bytes)
  // HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);  
  HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x00, 1, get_time, sizeof(get_time), 1000);

  // Store the time values (converted from BCD code to decimal) into the time variable
	time_check.second 	    = bcdToDec(get_time[0]);
	time_check.minute 	    = bcdToDec(get_time[1]);
	time_check.hour 		    = bcdToDec(get_time[2]);
	time_check.dayofweek 	  = bcdToDec(get_time[3]);
	time_check.dayofmonth   = bcdToDec(get_time[4]);
	time_check.month 		    = bcdToDec(get_time[5]);
	time_check.year 		    = bcdToDec(get_time[6]);
}

// Function to control settings of the RTC module (Alarm 1)
void Ctrl_Time (uint8_t mode, uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, bool dy_dt)
{
  // A blank array (4 slots) to contain the RTC alarm settings
  uint8_t Ctrl_Time[4];

  // A mask bit to set the Control register in the RTC module
  //    Address 0Eh: A1IE = 1
  uint8_t ctrl_alarm = 1; 

  // A mask bit to set the alarm modes (Bit 7) in the RTC module
  uint8_t maskBit = 128;

  // A mask bit for selection of [day of week] or [date of month] (Bit 6) in the RTC module
  //    If the input [day of week]/ [date of month] selection is 1, the mask bit will be 64.
  uint8_t dyDt = (dy_dt == 1) ? 64 : 0;

  // Store the RTC alarm time settings (converted into BCD code) into the blank array
  Ctrl_Time[0] = decToBcd(sec);
	Ctrl_Time[1] = decToBcd(min);
	Ctrl_Time[2] = decToBcd(hour);
	Ctrl_Time[3] = decToBcd(dow_dom);

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
      Ctrl_Time[0] += maskBit;
      Ctrl_Time[1] += maskBit;
      Ctrl_Time[2] += maskBit;
      Ctrl_Time[3] += maskBit;
      break;
    case 1:
      // Alarm mask bit: A1M4:A1M1 = b1110, DY/DT = X
      Ctrl_Time[0] += 0;
      Ctrl_Time[1] += maskBit;
      Ctrl_Time[2] += maskBit;
      Ctrl_Time[3] += maskBit;
      break;
    case 2:
      // Alarm mask bit: A1M4:A1M1 = b1100, DY/DT = X
      Ctrl_Time[0] += 0;
      Ctrl_Time[1] += 0;
      Ctrl_Time[2] += maskBit;
      Ctrl_Time[3] += maskBit;
      break;
    case 3:
      // Alarm mask bit: A1M4:A1M1 = b1000, DY/DT = X
      Ctrl_Time[0] += 0;
      Ctrl_Time[1] += 0;
      Ctrl_Time[2] += 0;
      Ctrl_Time[3] += maskBit;
      break;
    case 4:
      // Alarm mask bit: A1M4:A1M1 = b0000, DY/DT = 0
      Ctrl_Time[0] += 0;
      Ctrl_Time[1] += 0;
      Ctrl_Time[2] += 0;
      Ctrl_Time[3] += 0;
      break;
    case 5:
      // Alarm mask bit: A1M4:A1M1 = b0000, DY/DT = 1
      Ctrl_Time[0] += 0;
      Ctrl_Time[1] += 0;
      Ctrl_Time[2] += 0;
      Ctrl_Time[3] += dyDt;
      break;
    default:
      // Alarm mask bit: A1M4:A1M1 = b0000, DY/DT = 0
      Ctrl_Time[0] += 0;
      Ctrl_Time[1] += 0;
      Ctrl_Time[2] += 0;
      Ctrl_Time[3] += 0;
      break;
  }

  // Send the array containing the RTC alarm mode setting to the RTC module through I2C interface at address 07h - 0Ah (size of value: 4 bytes)
  // HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);	  
  HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x07, 1, Ctrl_Time, sizeof(Ctrl_Time), 1000);

  // Send the alarm control mask bits to the RTC module through I2C interface at address 0Eh (size of value: 1 byte)
  // HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
  //    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);	  
  HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0E, 1, &ctrl_alarm, sizeof(ctrl_alarm), 1000);
}

/*
// For test only
//    Temp array to read values of Alarm 1 registers from RTC module
uint8_t alarm_check[4];

//    Temp variable to read values of Control register from RTC module
uint8_t ctrl_check;

//    Temp variable to read values of Status register from RTC module
uint8_t status_check;

// Interrupt Handler for PB4 (Interrupt occurs every second)
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_4)
  {
    // Get time from the RTC module through I2C interface
    Get_Time();
    /*
    // For test only
    //    Read values of Alarm 1 registers from RTC module
	  HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x07, 1, alarm_check, 4, 1000);

    //    Read values of Control register from RTC module
	  HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x0E, 1, &ctrl_check, 1, 1000);

    //    Read values of Status register from RTC module
    HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x0F, 1, &status_check, 1, 1000);
    */
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

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
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
