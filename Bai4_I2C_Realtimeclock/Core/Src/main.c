/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "software_timer.h"
#include "led_7seg.h"
#include "button.h"
#include "lcd.h"
#include "picture.h"
#include "ds3231.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t count_led_debug = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void system_init();
void test_LedDebug();
void displayTime(uint16_t modeStatus);
void updateTime(uint16_t second, uint16_t minute, uint16_t hours, uint16_t day, uint16_t date, uint16_t month, uint16_t year);
void offSecond();
void offMinute();
void offHour();
void offArticle();
void offDay();
void offMonth();
void offYear();
void checkTime();
void checkAlart();
void alartNotify();
void checkTimeAlart();
void displayAlartTime(uint16_t modeStatus);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t secondAlart = 0;
uint16_t minuteAlart = 0;
uint16_t hourAlart = 0;
uint16_t dayAlart = 0;
uint16_t dateAlart = 0;
uint16_t monthAlart = 0;
uint16_t yearAlart = 0;

void setAlartInit()
{
	secondAlart = ds3231_sec-1;
	minuteAlart = ds3231_min;
	hourAlart = ds3231_hours;
	dayAlart = ds3231_day;
	dateAlart = ds3231_date;
	monthAlart = ds3231_month;
	yearAlart = ds3231_year;
}

uint16_t modifyStatus = 0;
uint16_t modeStatus = 1;
uint16_t toggle = 1;
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
  MX_TIM2_Init();
  MX_SPI1_Init();
  MX_FSMC_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  system_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
void toggleDisplayTime(uint8_t index, uint8_t toggle);
#define WATCH 	1		// chế độ xem giờ
#define SETTIME 2 		// chế độ chỉnh giờ
#define ALARM	3 		// chế độ hẹn giờ


 lcd_Clear(BLACK);

 setTimer2(200);
 ds3231_ReadTime();
 setAlartInit();
 updateTime(ds3231_sec, ds3231_min, ds3231_hours, ds3231_day, ds3231_date, ds3231_month, ds3231_year);
  while (1)
  {
	  if(flag_timer2 == 1){
		  button_Scan();
		  flag_timer2 = 0;
	  }

	  if(modeStatus == 1){
		  if(button_count[11] == 1){
			  button_count[11] = 0;
			  modeStatus = 2;
			  displayTime(modeStatus);
			  setFlagModifyTimer(500);
//			  ds3231_Write(ADDRESS_MONTH, 3);
		  }
		  ds3231_ReadTime();
		  displayTime(modeStatus);
		  checkAlart();
	  }else if(modeStatus == 2){
		  if(button_count[11] == 1){
			  button_count[11] = 0;
			  modeStatus = 3;
			  displayTime(modeStatus);
			  modifyStatus = 0;
		  }
		  if(button_count[12] == 1){
			  button_count[12] = 0;
			  modifyStatus+=1;
			  if(modifyStatus > 6){
				  modifyStatus = 0;
			  }
		  }
		  if(modifyStatus == 0){
			  if(isFlagModify() == 1){
				  toggle = 1 - toggle;
				  if(toggle){
					  displayTime(modeStatus);
				  }else{
					  offSecond();
				  }
			  }
			  if(button_count[3] == 1)
			  {
				  button_count[3] = 0;
				  ds3231_sec++;
				  checkTime();
			  }
		  }else if(modifyStatus == 1){
			  if(isFlagModify() == 1){
				  toggle = 1 - toggle;
				  if(toggle){
					  displayTime(modeStatus);
				  }else{
					  offMinute();
				  }
			  }
			  if(button_count[3] == 1)
			  {
				  button_count[3] = 0;
				  ds3231_min++;
				  checkTime();
			  }
		  }else if(modifyStatus == 2){
			  if(isFlagModify() == 1){
				  toggle = 1 - toggle;
				  if(toggle){
					  displayTime(modeStatus);
				  }else{
					  offHour();
				  }
			  }
			  if(button_count[3] == 1)
			  {
				  button_count[3] = 0;
				  ds3231_hours++;
				  checkTime();
			  }
		  }else if(modifyStatus == 3){
			  if(isFlagModify() == 1){
				  toggle = 1 - toggle;
				  if(toggle){
					  displayTime(modeStatus);
				  }else{
					  offArticle();
				  }
			  }
			  if(button_count[3] == 1)
			  {
				  button_count[3] = 0;
				  ds3231_day++;
				  checkTime();
			  }
		  }else if(modifyStatus == 4){
			  if(isFlagModify() == 1){
				  toggle = 1 - toggle;
				  if(toggle){
					  displayTime(modeStatus);
				  }else{
					  offDay();
				  }
			  }
			  if(button_count[3] == 1)
			  {
				  button_count[3] = 0;
				  ds3231_date++;
				  checkTime();
			  }
		  }else if(modifyStatus == 5){
			  if(isFlagModify() == 1){
				  toggle = 1 - toggle;
				  if(toggle){
					  displayTime(modeStatus);
				  }else{
					  offMonth();
				  }
			  }
			  if(button_count[3] == 1)
			  {
				  button_count[3] = 0;
				  ds3231_month++;
				  checkTime();
			  }
		  }else if(modifyStatus == 6){
			  if(isFlagModify() == 1){
				  toggle = 1 - toggle;
				  if(toggle){
					  displayTime(modeStatus);
				  }else{
					  offYear();
				  }
			  }
			  if(button_count[3] == 1)
			  {
				  button_count[3] = 0;
				  ds3231_year++;
				  checkTime();
			  }
		  }
	// MODE 3 -----------------------------------------
	  }else if(modeStatus == 3){

		  if(button_count[11] == 1){
			  button_count[11] = 0;
			  modeStatus = 1;
			  lcd_Clear(BLACK);
			  displayAlartTime(modeStatus);
		  }
		  setupAlart();

	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void system_init(){
	  HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
	  HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 0);
	  HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 0);
	  timer_init();
	  led7_init();
	  button_init();
	  lcd_init();
	  ds3231_init();
	  setTimer2(50);
}

void test_LedDebug(){
	count_led_debug = (count_led_debug + 1)%20;
	if(count_led_debug == 0){
		HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
	}
}

void test_7seg(){
	led7_SetDigit(0, 0, 0);
	led7_SetDigit(5, 1, 0);
	led7_SetDigit(4, 2, 0);
	led7_SetDigit(7, 3, 0);
}
void test_button(){
	for(int i = 0; i < 16; i++){
		if(button_count[i] == 1){
			led7_SetDigit(i/10, 2, 0);
			led7_SetDigit(i%10, 3, 0);
		}
	}
}

void updateTime(uint16_t second, uint16_t minute, uint16_t hours, uint16_t day, uint16_t date, uint16_t month, uint16_t year){
	ds3231_Write(ADDRESS_YEAR, year);
	ds3231_Write(ADDRESS_MONTH, month);
	ds3231_Write(ADDRESS_DATE, date);
	ds3231_Write(ADDRESS_DAY, day);
	ds3231_Write(ADDRESS_HOUR, hours);
	ds3231_Write(ADDRESS_MIN, minute);
	ds3231_Write(ADDRESS_SEC, second);
}

uint8_t isButtonUp()
{
    if (button_count[3] == 1)
        return 1;
    else
        return 0;
}
uint8_t isButtonDown()
{
    if (button_count[7] == 1)
        return 1;
    else
        return 0;
}
void displayTime(uint16_t modeStatus){
	lcd_ShowStr(10, 70, "Thu", YELLOW, BLACK, 24, 1);
	lcd_ShowStr(55, 70, "Ngay", YELLOW, BLACK, 24, 1);
	lcd_ShowStr(110, 70, "Thang", YELLOW, BLACK, 24, 1);
	lcd_ShowStr(180, 70, "Nam", YELLOW, BLACK, 24, 1);

	lcd_ShowIntNum(10, 100, ds3231_day, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(60, 100, ds3231_date, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(120, 100, ds3231_month, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(180, 100, ds3231_year, 2, YELLOW, BLACK, 24);

	lcd_ShowStr(40, 160, "Gio", YELLOW, BLACK, 24, 1);
	lcd_ShowStr(90, 160, "Phut", YELLOW, BLACK, 24, 1);
	lcd_ShowStr(150, 160, "Giay", YELLOW, BLACK, 24, 1);

	lcd_ShowIntNum(50, 190, ds3231_hours, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(100, 190, ds3231_min, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(150, 190, ds3231_sec, 2, GREEN, BLACK, 24);

	lcd_ShowStr(40, 250, "Mode: ", BLUE, BLACK, 24, 1);
	lcd_ShowIntNum(100, 250, modeStatus, 2, GREEN, BLACK, 24);
}

void displayAlartTime(uint16_t modeStatus){
	lcd_ShowStr(10, 70, "Thu", YELLOW, BLACK, 24, 1);
	lcd_ShowStr(55, 70, "Ngay", YELLOW, BLACK, 24, 1);
	lcd_ShowStr(110, 70, "Thang", YELLOW, BLACK, 24, 1);
	lcd_ShowStr(180, 70, "Nam", YELLOW, BLACK, 24, 1);

	lcd_ShowIntNum(10, 100, dayAlart, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(60, 100, dateAlart, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(120, 100, monthAlart, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(180, 100, yearAlart, 2, YELLOW, BLACK, 24);

	lcd_ShowStr(40, 160, "Gio", YELLOW, BLACK, 24, 1);
	lcd_ShowStr(90, 160, "Phut", YELLOW, BLACK, 24, 1);
	lcd_ShowStr(150, 160, "Giay", YELLOW, BLACK, 24, 1);

	lcd_ShowIntNum(50, 190, hourAlart, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(100, 190, minuteAlart, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(150, 190, secondAlart, 2, GREEN, BLACK, 24);

	lcd_ShowStr(40, 250, "Mode: ", BLUE, BLACK, 24, 1);
	lcd_ShowIntNum(100, 250, modeStatus, 2, GREEN, BLACK, 24);
}

void toggleDisplayTime(uint8_t index, uint8_t toggle)
{
	lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
}

void offSecond()
{
	lcd_ShowIntNum(150, 190, ds3231_sec, 2, BLACK, BLACK, 24);
}
void offMinute()
{
	lcd_ShowIntNum(100, 190, ds3231_min, 2, BLACK, BLACK, 24);
}
void offHour()
{
	lcd_ShowIntNum(50, 190, ds3231_hours, 2, BLACK, BLACK, 24);
}
void offArticle()
{
	lcd_ShowIntNum(10, 100, ds3231_day, 2, BLACK, BLACK, 24);
}
void offDay()
{
	lcd_ShowIntNum(60, 100, ds3231_date, 2, BLACK, BLACK, 24);
}
void offMonth()
{
	lcd_ShowIntNum(120, 100, ds3231_month, 2, BLACK, BLACK, 24);
}
void offYear()
{
	lcd_ShowIntNum(180, 100, ds3231_year, 2, BLACK, BLACK, 24);
}
void checkTime()
{
	if(ds3231_sec>59){
	  ds3231_sec = 0;
	  ds3231_min++;
	}
	if(ds3231_min>59){
		ds3231_min = 0;
	  ds3231_hours++;
	}
	if(ds3231_hours>23){
		ds3231_hours = 0;
		ds3231_day++;
	}
	if(ds3231_day>8){
		ds3231_day = 2;
		ds3231_date++;
	}
	if(ds3231_date>30){
		ds3231_date = 0;
		ds3231_month++;
	}
	if(ds3231_month>30){
		ds3231_month = 0;
		ds3231_year++;
	}
	if(ds3231_year>99){
		ds3231_year = 1;
	}
	updateTime(ds3231_sec, ds3231_min, ds3231_hours, ds3231_day, ds3231_date, ds3231_month, ds3231_year);
}

void checkTimeAlart()
{
	if(secondAlart>59){
	  secondAlart = 0;
	  minuteAlart++;
	}
	if(minuteAlart>59){
		minuteAlart = 0;
	  hourAlart++;
	}
	if(hourAlart>23){
		hourAlart = 0;
		dayAlart++;
	}
	if(dayAlart>8){
		dayAlart = 2;
		dateAlart++;
	}
	if(dateAlart>30){
		dateAlart = 0;
		monthAlart++;
	}
	if(monthAlart>30){
		monthAlart = 0;
		yearAlart++;
	}
	if(yearAlart>99){
		yearAlart = 1;
	}
}

void setupAlart()
{
	if(button_count[12] == 1){
		  button_count[12] = 0;
		  modifyStatus+=1;
		  if(modifyStatus > 6){
			  modifyStatus = 0;
		  }
	  }
	  if(modifyStatus == 0){
		  if(isFlagModify() == 1){
			  toggle = 1 - toggle;
			  if(toggle){
				  displayAlartTime(modeStatus);
			  }else{
				  offSecond();
			  }
		  }
		  if(button_count[3] == 1)
		  {
			  button_count[3] = 0;
			  secondAlart++;
			  checkTimeAlart();
		  }
	  }else if(modifyStatus == 1){
		  if(isFlagModify() == 1){
			  toggle = 1 - toggle;
			  if(toggle){
				  displayAlartTime(modeStatus);
			  }else{
				  offMinute();
			  }
		  }
		  if(button_count[3] == 1)
		  {
			  button_count[3] = 0;
			  minuteAlart++;
			  checkTimeAlart();
		  }
	  }else if(modifyStatus == 2){
		  if(isFlagModify() == 1){
			  toggle = 1 - toggle;
			  if(toggle){
				  displayAlartTime(modeStatus);
			  }else{
				  offHour();
			  }
		  }
		  if(button_count[3] == 1)
		  {
			  button_count[3] = 0;
			  hourAlart++;
			  checkTimeAlart();
		  }
	  }else if(modifyStatus == 3){
		  if(isFlagModify() == 1){
			  toggle = 1 - toggle;
			  if(toggle){
				  displayAlartTime(modeStatus);
			  }else{
				  offArticle();
			  }
		  }
		  if(button_count[3] == 1)
		  {
			  button_count[3] = 0;
			  dayAlart++;
			  checkTimeAlart();
		  }
	  }else if(modifyStatus == 4){
		  if(isFlagModify() == 1){
			  toggle = 1 - toggle;
			  if(toggle){
				  displayAlartTime(modeStatus);
			  }else{
				  offDay();
			  }
		  }
		  if(button_count[3] == 1)
		  {
			  button_count[3] = 0;
			  dateAlart++;
			  checkTimeAlart();
		  }
	  }else if(modifyStatus == 5){
		  if(isFlagModify() == 1){
			  toggle = 1 - toggle;
			  if(toggle){
				  displayAlartTime(modeStatus);
			  }else{
				  offMonth();
			  }
		  }
		  if(button_count[3] == 1)
		  {
			  button_count[3] = 0;
			  monthAlart++;
			  checkTimeAlart();
		  }
	  }else if(modifyStatus == 6){
		  if(isFlagModify() == 1){
			  toggle = 1 - toggle;
			  if(toggle){
				  displayAlartTime(modeStatus);
			  }else{
				  offYear();
			  }
		  }
		  if(button_count[3] == 1)
		  {
			  button_count[3] = 0;
			  yearAlart++;
			  checkTimeAlart();
		  }
	  }
}

void checkAlart()
{
	if(ds3231_sec == secondAlart)
	{
		if(ds3231_min == minuteAlart)
		{
			if(ds3231_hours == hourAlart)
			{
				if(ds3231_date == dateAlart)
				{
					if(ds3231_day == dayAlart)
					{
						if(ds3231_month == monthAlart)
						{
							if(ds3231_year == yearAlart)
							{
								alartNotify();
							}
						}
					}
				}
			}
		}
	}
}

void alartNotify()
{
	lcd_DrawCircle(120, 160, RED, 50, 1);
	lcd_ShowStr(100, 160, "ALART", BLACK, RED, 24, 1);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
