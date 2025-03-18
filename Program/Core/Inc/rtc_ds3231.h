/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdbool.h"

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* Private user code ---------------------------------------------------------*/
// Slave adress of DS3231
#define DS3231_ADDRESS 0xD0

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