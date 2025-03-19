/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdbool.h"

// /* Private variables ---------------------------------------------------------*/
// I2C_HandleTypeDef hi2c1;

/* Private user code ---------------------------------------------------------*/

// // Time variable
// TIME time;

/* Function declaration ------------------------------------------------------*/
// Convert normal decimal numbers to binary coded decimal
uint8_t decToBcd(int val);

// Convert binary coded decimal to normal decimal numbers
int bcdToDec(uint8_t val);

// Function to initialize RTC module
void RTC_Init (void);

// Function to initially set time to the RTC module through I2C interface (Run only once after reset the RTC)
void Set_Time (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year);

// Function to get time from the RTC module through I2C interface
void Get_Time (void);

// Function to set the RTC alarm settings (Alarm 1)
void Set_Alarm (uint8_t mode, uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow_dom, bool dy_dt);

// Function to initialize all configured peripherals 
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
