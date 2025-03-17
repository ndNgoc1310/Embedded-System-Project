/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "stdbool.h"

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

/* Private user code ---------------------------------------------------------*/
#define DS3231_ADDRESS 0xD0

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

// A structure of 7 one-byte unsigned characters to store 7 time values
typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hour;
	uint8_t dayofweek;
	uint8_t dayofmonth;
	uint8_t month;
	uint8_t year;
} TIME;

// Time variable
TIME time;

// Function to set time (Run only once after reset the RTC)
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
    // TypeDef: HAL_I2C_Mem_Write(*hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
	HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x00, 1, set_time, 7, 1000);
}

// Function to get time
void Get_Time (void)
{
	// A blank array (7 slots) to contain the time values received from the RTC module
  uint8_t get_time[7];

  // Receive the time values from the RTC module through I2C interface, then store them into the blank array (size of value: 7 bytes)
	  // TypeDef: HAL_I2C_Mem_Read(*hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
  HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x00, 1, get_time, 7, 1000);

  // Store the time values (converted from BCD code to decimal) into the time variable
	time.seconds 	  = bcdToDec(get_time[0]);
	time.minutes 	  = bcdToDec(get_time[1]);
	time.hour 		  = bcdToDec(get_time[2]);
	time.dayofweek 	= bcdToDec(get_time[3]);
	time.dayofmonth = bcdToDec(get_time[4]);
	time.month 		  = bcdToDec(get_time[5]);
	time.year 		  = bcdToDec(get_time[6]);
}

// Function to set the RTC alarm settings (Alarm 1)
void Set_Alarm (uint8_t mode, uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, bool dy_dt)
{
  // A blank array (4 slots) to contain the RTC alarm settings
  uint8_t set_alarm[4];

  // Mask bits to set the Control register in the RTC module
    // Address 0Eh: A1IE = 1, INTCN = 1
  //uint8_t ctrl_alarm = 5;
  uint8_t ctrl_alarm = 1; // Test the effect of the EOSC bit of Control register of the RTC module

  // A mask bit to set the alarm modes (Bit 7) in the RTC module
  uint8_t maskBit = 128;

  // A mask bit for selection of [day of week] or [date of month] (Bit 6) in the RTC module
    // If the input [day of week]/ [date of month] selection is 1, the mask bit will be 64.
  uint8_t dyDt = (dy_dt == 1) ? 64 : 0;

  // Store the RTC alarm time settings (converted into BCD code) into the blank array
  set_alarm[0] = decToBcd(sec);
	set_alarm[1] = decToBcd(min);
	set_alarm[2] = decToBcd(hour);
	set_alarm[3] = decToBcd(dow_dom);

  // Adjust the RTC alarm mode settings into the array basing on the input mode
    // Mode  : Alarm rate
    //  0    : Alarm once per second
    //  1    : Alarm when seconds match
    //  2    : Alarm when minutes and seconds match
    //  3    : Alarm when hours, minutes, and seconds match
    //  4    : Alarm when date, hours, minutes, and seconds match
    //  5    : Alarm when day, hours, minutes, and seconds match
  switch (mode)
  {
    case 0:
      // Alarm mask bit: A1M4:A1M1 = b1111, DY/DT = X
      set_alarm[0] += maskBit;
      set_alarm[1] += maskBit;
      set_alarm[2] += maskBit;
      set_alarm[3] += maskBit;
      break;
    case 1:
      // Alarm mask bit: A1M4:A1M1 = b1110, DY/DT = X
      set_alarm[0] += 0;
      set_alarm[1] += maskBit;
      set_alarm[2] += maskBit;
      set_alarm[3] += maskBit;
      break;
    case 2:
      // Alarm mask bit: A1M4:A1M1 = b1100, DY/DT = X
      set_alarm[0] += 0;
      set_alarm[1] += 0;
      set_alarm[2] += maskBit;
      set_alarm[3] += maskBit;
      break;
    case 3:
      // Alarm mask bit: A1M4:A1M1 = b1000, DY/DT = X
      set_alarm[0] += 0;
      set_alarm[1] += 0;
      set_alarm[2] += 0;
      set_alarm[3] += maskBit;
      break;
    case 4:
      // Alarm mask bit: A1M4:A1M1 = b0000, DY/DT = 0
      set_alarm[0] += 0;
      set_alarm[1] += 0;
      set_alarm[2] += 0;
      set_alarm[3] += 0;
      break;
    case 5:
      // Alarm mask bit: A1M4:A1M1 = b0000, DY/DT = 1
      set_alarm[0] += 0;
      set_alarm[1] += 0;
      set_alarm[2] += 0;
      set_alarm[3] += dyDt;
      break;
    default:
      // Alarm mask bit: A1M4:A1M1 = b0000, DY/DT = 0
      set_alarm[0] += 0;
      set_alarm[1] += 0;
      set_alarm[2] += 0;
      set_alarm[3] += 0;
      break;
  }

  // Send the array containing the RTC alarm mode setting to the RTC module through I2C interface at address 07h - 0Ah (size of value: 4 bytes)
    // TypeDef: HAL_I2C_Mem_Write(*hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
  HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x07, 1, set_alarm, 4, 1000);

  // Send the alarm control mask bits to the RTC module through I2C interface at address 0Eh (size of value: 1 byte)
      // TypeDef: HAL_I2C_Mem_Write(*hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
  HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0E, 1, &ctrl_alarm, 1, 1000);
}

// Temp variable to read value from Port B (Pin 4) and check the interrupt from INT/SQW pin of RTC module 
uint8_t PB4;

// Temp array to read values of Alarm 1 registers from RTC module
uint8_t alarm_check[4];

// Temp variable to read values of Control register from RTC module
uint8_t ctrl_check;

// Temp variable to read values of Status register from RTC module
uint8_t status_check;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
 
  // Run only once after reset the RTC module to initially set the time
  //Set_Time (00, 55, 12, 5, 13, 3, 25);

  // Run only to initially set the alarm  for the RTC module
  Set_Alarm (0, 0, 0, 0, 0, 0);

  /* Infinite loop */
  while (1)
  {
	  Get_Time();
	  if(HAL_GPIO_ReadPin(Test_GPIO_Port, Test_Pin) == 1)
    {
      PB4 += 1;
    }
	  HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x07, 1, alarm_check, 4, 1000);
	  HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x0E, 1, &ctrl_check, 1, 1000);
    HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x0F, 1, &status_check, 1, 1000);
	  HAL_Delay(500);
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : Test_Pin */
  GPIO_InitStruct.Pin = Test_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Test_GPIO_Port, &GPIO_InitStruct);

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
