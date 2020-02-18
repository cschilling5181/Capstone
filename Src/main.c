/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c Professor Unit
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f3xx_hal.h"
#include "lcd.h"
#include "keypad.h"
#include "MY_NRF24.h"
#include <string.h>
#include <stdbool.h>
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
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim15;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM15_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void delay_us (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim15,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim15) < us);  // wait for the counter to reach the us input in the parameter
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */
	uint8_t returnKey = 16;
	uint8_t menuState = 1;
	uint8_t dutyCycles[3];

	uint32_t ringDelay = 0;
	bool set = false;

	uint64_t RxPipeAddr = 0x11223344AA;
	uint64_t TxPipeAddr = 0x99887766BB;
	char RxData[34];
	char TxData[32];

	char userStatus[] = "Available";
	char userName[19] = { 0 };
	char profResponses[4][19] = {"First", "Second", "Third", "Fourth"};
	char alphabets[2][27] = { { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v','w', 'x', 'y', 'z', ' ' },
			{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I','J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ' } };
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
	MX_SPI1_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_TIM15_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	NRF24_begin(GPIOB, CSpin_Pin, CEpin_Pin, hspi1);
	nrf24_DebugUART_Init(huart1);
	printRadioSettings();
	NRF24_setChannel(44);
	NRF24_setPayloadSize(32);
	NRF24_openReadingPipe(1, RxPipeAddr);
	NRF24_openWritingPipe(TxPipeAddr);
	//NRF24_setAutoAck(true);
	//NRF24_enableDynamicPayloads();
	//NRF24_enableAckPayload();
	//NRF24_startListening();

	HAL_TIM_Base_Start(&htim15);

	Lcd_PortType ports[] = {D4_GPIO_Port, D5_GPIO_Port, D6_GPIO_Port, D7_GPIO_Port};
	Lcd_PinType pins[] = {D4_Pin, D5_Pin, D6_Pin, D7_Pin};
	Lcd_HandleTypeDef lcd = Lcd_create(ports, pins, RS_GPIO_Port, RS_Pin, EN_GPIO_Port, EN_Pin, LCD_4_BIT_MODE);

	void resetAll()
	{
		ringDelay = 0;
		Lcd_clear(&lcd);
		set = false;
	}

	void txData()
	{
		NRF24_stopListening();

		if(NRF24_write(TxData, 32))
		{
			HAL_UART_Transmit(&huart1, (uint8_t *)"\r\nTransmitted Successfully!\r\n", strlen("\r\nTransmitted Successfully!\r\n"), 10);

			HAL_UART_Transmit(&huart1, (uint8_t *)TxData, strlen(TxData), 10);
		}else
		{
			HAL_UART_Transmit(&huart1, (uint8_t *)"\r\nTransmitted Un-successfully!\r\n", strlen("\r\nTransmitted Un-successfully!\r\n"), 10);
		}
	}

	void responseScreen()
	{
		while(/*HAL_GetTick() < (ringDelay + 10000)*/ returnKey != 0 && returnKey != 1 && returnKey != 2)
		{
			if(!set){
				Lcd_string(&lcd, "1.");
				Lcd_string(&lcd, profResponses[0]);
				Lcd_cursor(&lcd, 1, 0);
				Lcd_string(&lcd, "2.");
				Lcd_string(&lcd, profResponses[1]);
				Lcd_cursor(&lcd, 2, 0);
				Lcd_string(&lcd, "3.");
				Lcd_string(&lcd, profResponses[2]);
				//Lcd_cursor(&lcd, 3, 0);
				//Lcd_string(&lcd, "4.Dismiss");
				set = true;
			}

			returnKey = keypadRead();

			switch (returnKey)
			{
			case 0:
				strcpy(TxData, profResponses[0]);
				txData();
				break;

			case 1:
				strcpy(TxData, profResponses[1]);
				txData();
				break;

			case 2:
				strcpy(TxData, profResponses[2]);
				txData();
				break;

			default:
				//Lcd_string(&lcd, "Wrong");
				break;
			}
		}
		resetAll();
	}

	void rxData()
	{
		NRF24_startListening();

		if(NRF24_available())
		{
			memset(RxData, 0, 34);
			NRF24_read(RxData, 32);

			RxData[32] = '\r';
			RxData[32+1] = '\n';
			HAL_UART_Transmit(&huart1, (uint8_t *)RxData, 32+2, 10);

			resetAll();
			responseScreen();
		}
	}
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	Lcd_clear(&lcd);
	while (1) {

		memset(TxData, 0, 32);

		//Main menu
		if (menuState == 1)
		{
			if(!set){
				Lcd_string(&lcd, "1.Settings 2.Status");
				set = true;
			}

			//resetAll();
			//responseScreen();
			rxData();

			returnKey = keypadRead();

			switch (returnKey)
			{
			case 0:
				menuState = 11;
				resetAll();
				break;

			case 1:
				menuState = 12;
				resetAll();
				break;

			default:
				//Lcd_string(&lcd, "Wrong");
				break;
			}
		}

		//Settings Menu
		if (menuState == 11)
		{
			if(!set){
				Lcd_cursor(&lcd, 0, 4);
				Lcd_string(&lcd, "Customize...");
				Lcd_cursor(&lcd, 1, 0);
				Lcd_string(&lcd, "1.Buzzer 2.Vibration");
				Lcd_cursor(&lcd, 2, 0);
				Lcd_string(&lcd, "3.LED    4.Name");
				Lcd_cursor(&lcd, 3, 0);
				Lcd_string(&lcd, "5.Call Response");
				set = true;
			}

			returnKey = keypadRead();

			switch (returnKey)
			{
			case 0:
				menuState = 111;
				resetAll();
				break;

			case 1:
				menuState = 112;
				resetAll();
				break;

			case 2:
				menuState = 113;
				resetAll();
				break;

			case 4:
				menuState = 114;
				resetAll();
				break;

			case 5:
				menuState = 115;
				resetAll();
				break;

			case 7:
				menuState = 1;
				resetAll();
				break;

			default:
				//Lcd_string(&lcd, "Wrong");
				break;
			}
		}

		//Buzzer Menu
		if (menuState == 111)
		{
			Lcd_string(&lcd, "Buzzer");

			htim2.Instance->CCR2 = dutyCycles[0]; //% Duty Cycle
			//__HAL_TIM_SET_AUTORELOAD(&htim2, dutyCycles[]);
			//__HAL_TIM_SET_AUTORELOAD(&htim2, 7200);

			returnKey = keypadRead();

			switch (returnKey) {
			case 3:
				if(dutyCycles[0] < 50)
				{
					dutyCycles[0] += 10;
					Lcd_cursor(&lcd, 2, 8);
					Lcd_int(&lcd, dutyCycles[0]);
				}else
				{
					Lcd_cursor(&lcd, 1, 1);
					Lcd_string(&lcd, "Cannot go higher!");
					Lcd_cursor(&lcd, 2, 8);
					Lcd_int(&lcd, dutyCycles[0]);
				}
				break;

			case 15:
				if (dutyCycles[0] > 0) {
					dutyCycles[0] -= 10;
					Lcd_cursor(&lcd, 2, 8);
					Lcd_int(&lcd, dutyCycles[0]);
				}else
				{
					Lcd_cursor(&lcd, 1, 2);
					Lcd_string(&lcd, "Cannot go lower!");
					Lcd_cursor(&lcd, 2, 8);
					Lcd_int(&lcd, dutyCycles[0]);
				}
				break;

			case 7:
				htim2.Instance->CCR2 = 0; //% Duty Cycle
				menuState = 11;
				resetAll();
				break;

			default:
				break;
			}
		}

		//Vibration Menu
		if (menuState == 112)
		{
			Lcd_string(&lcd, "Vibration");

			htim3.Instance->CCR1 = dutyCycles[1]; //% Duty Cycle

			returnKey = keypadRead();

			switch (returnKey) {
			case 3:
				if(dutyCycles[1] < 90)
				{
					dutyCycles[1] += 10;
					Lcd_cursor(&lcd, 2, 8);
					Lcd_int(&lcd, dutyCycles[1]);
				}else
				{
					Lcd_cursor(&lcd, 1, 1);
					Lcd_string(&lcd, "Cannot go higher!");
					Lcd_cursor(&lcd, 2, 8);
					Lcd_int(&lcd, dutyCycles[1]);
				}
				break;

			case 15:
				if (dutyCycles[1] > 0) {
					dutyCycles[1] -= 10;
					Lcd_cursor(&lcd, 2, 8);
					Lcd_int(&lcd, dutyCycles[1]);
				}else
				{
					Lcd_cursor(&lcd, 1, 2);
					Lcd_string(&lcd, "Cannot go lower!");
					Lcd_cursor(&lcd, 2, 8);
					Lcd_int(&lcd, dutyCycles[1]);
				}
				break;

			case 7:
				htim3.Instance->CCR1 = 0; //% Duty Cycle
				menuState = 11;
				resetAll();
				break;

			default:
				break;
			}
		}

		//LED Menu
		if (menuState == 113)
		{
			Lcd_string(&lcd, "LED");

			htim4.Instance->CCR1 = dutyCycles[2]; //% Duty Cycle

			returnKey = keypadRead();

			switch (returnKey) {
			case 3:
				if(dutyCycles[2] < 100)
				{
					dutyCycles[2] += 10;
					Lcd_cursor(&lcd, 2, 8);
					Lcd_int(&lcd, dutyCycles[2]);
				}else
				{
					Lcd_cursor(&lcd, 1, 1);
					Lcd_string(&lcd, "Cannot go higher!");
					Lcd_cursor(&lcd, 2, 8);
					Lcd_int(&lcd, dutyCycles[2]);
				}
				break;

			case 15:
				if (dutyCycles[2] > 0) {
					dutyCycles[2] -= 10;
					Lcd_cursor(&lcd, 2, 8);
					Lcd_int(&lcd, dutyCycles[2]);
				}else
				{
					Lcd_cursor(&lcd, 1, 2);
					Lcd_string(&lcd, "Cannot go lower!");
					Lcd_cursor(&lcd, 2, 8);
					Lcd_int(&lcd, dutyCycles[2]);
				}
				break;

			case 7:
				htim4.Instance->CCR1 = 0; //% Duty Cycle
				menuState = 11;
				resetAll();
				break;

			default:
				break;
			}
		}

		//Name Menu
		if (menuState == 114)
		{
			int8_t alphaRow = 1;
			int8_t alphaCol = 0;
			int8_t namePos = 0;
			int8_t cursorCol = 1;

			Lcd_clear(&lcd);
			Lcd_string(&lcd, "Name");
			Lcd_cursor(&lcd, 2, 1);
			Lcd_char(&lcd, alphabets[alphaRow][alphaCol]);

			do{
				Lcd_cursor(&lcd, 3, 1);
				Lcd_string(&lcd, userName);

				returnKey = keypadRead();

				switch (returnKey) {
				//Alphabet Increase
				case 1:
					alphaCol++;
					if(alphaCol > 26)
					{
						alphaCol = 0;
					}

					Lcd_cursor(&lcd, 2, cursorCol);
					Lcd_char(&lcd, alphabets[alphaRow][alphaCol]);
					//HAL_Delay(300);
					break;

					//Alphabet Caps
				case 4:
					alphaRow++;
					if(alphaRow > 1)
					{
						alphaRow = 0;
					}

					Lcd_cursor(&lcd, 2, cursorCol);
					Lcd_char(&lcd, alphabets[alphaRow][alphaCol]);
					//HAL_Delay(300);
					break;

					//Char parse
				case 5:
					if(namePos <= 17)
					{
						//Store letter in name array
						userName[namePos] = alphabets[alphaRow][alphaCol];
						namePos++;

						//Dont let cursor exceed 18 chars
						if (cursorCol < 18)
						{
							cursorCol++;
						}
						alphaCol = 0;
						alphaRow = 0;

						//Move cursor over one and reset alphabet
						Lcd_cursor(&lcd, 2, cursorCol);
						Lcd_char(&lcd, alphabets[alphaRow][alphaCol]);
						//HAL_Delay(300);
					}else
					{
						//Dont let username exceed 18 chars
						Lcd_cursor(&lcd, 3, 0);
						Lcd_string(&lcd, "    Max letters!    ");
						HAL_Delay(750);
					}
					break;

					//Alphabet Caps
				case 6:
					alphaRow--;
					if(alphaRow < 0)
					{
						alphaRow = 1;
					}

					Lcd_cursor(&lcd, 2, cursorCol);
					Lcd_char(&lcd, alphabets[alphaRow][alphaCol]);
					//HAL_Delay(300);
					break;

					//Alphabet Decrease
				case 9:
					alphaCol--;
					if(alphaCol < 0)
					{
						alphaCol = 26;
					}

					Lcd_cursor(&lcd, 2, cursorCol);
					Lcd_char(&lcd, alphabets[alphaRow][alphaCol]);
					//HAL_Delay(300);
					break;

					//Clear Username
				case 11:
					for (int k = 0; k < sizeof(userName); k++)
					{
						userName[k] = ' ';
					}

					alphaRow = 1;
					alphaCol = 0;
					namePos = 0;
					cursorCol = 1;

					Lcd_cursor(&lcd, 3, 0);
					Lcd_string(&lcd, "                    ");

					Lcd_cursor(&lcd, 2, cursorCol);
					Lcd_string(&lcd, "                    ");
					Lcd_cursor(&lcd, 2, cursorCol);
					Lcd_char(&lcd, alphabets[alphaRow][alphaCol]);
					//HAL_Delay(300);
					break;

					//"Backspace"
				case 15:
					if(namePos > 0)
					{
						userName[namePos-1] = ' ';
						namePos -= 1;
						cursorCol -= 1;
						alphaRow = 1;
						alphaCol = 0;

						Lcd_cursor(&lcd, 2, cursorCol+1);
						Lcd_char(&lcd, ' ');
						Lcd_cursor(&lcd, 2, cursorCol);
						Lcd_char(&lcd, alphabets[alphaRow][alphaCol]);
						//HAL_Delay(300);
					}
					break;

					//Back
				case 7:
					menuState = 11;
					resetAll();
					break;

				default:
					break;
				}
			}while(menuState != 11);
		}

		//Call Response edit menu
		if(menuState == 115)
		{
			int8_t alphaRow = 1;
			int8_t alphaCol = 0;
			int8_t namePos = 0;
			int8_t cursorCol = 1;
			int8_t chosen = -1;

			if(!set)
			{
				Lcd_cursor(&lcd, 0, 1);
				Lcd_string(&lcd, "Choose to edit...");
				Lcd_cursor(&lcd, 1, 0);
				Lcd_string(&lcd, "1.");
				Lcd_string(&lcd, profResponses[0]);
				Lcd_cursor(&lcd, 2, 0);
				Lcd_string(&lcd, "2.");
				Lcd_string(&lcd, profResponses[1]);
				Lcd_cursor(&lcd, 3, 0);
				Lcd_string(&lcd, "3.");
				Lcd_string(&lcd, profResponses[2]);
				set = true;
			}

			returnKey = keypadRead();

			switch (returnKey) {

			//Response 1
			case 0:
				chosen = 0;
				resetAll();
				break;

				//Response 2
			case 1:
				chosen = 1;
				resetAll();
				break;

				//Response 3
			case 2:
				chosen = 2;
				resetAll();
				break;

				//Back
			case 7:
				menuState = 11;
				resetAll();
				break;

			}

			while(chosen != -1){

				if(!set){
					Lcd_cursor(&lcd, 0, 5);
					Lcd_string(&lcd, "Response ");
					Lcd_int(&lcd, chosen + 1);
					Lcd_cursor(&lcd, 2, 1);
					Lcd_char(&lcd, alphabets[alphaRow][alphaCol]);
					set = true;
				}


				returnKey = keypadRead();

				switch (returnKey) {
				//Alphabet Increase
				case 1:
					alphaCol++;
					if(alphaCol > 26)
					{
						alphaCol = 0;
					}

					Lcd_cursor(&lcd, 2, cursorCol);
					Lcd_char(&lcd, alphabets[alphaRow][alphaCol]);
					//HAL_Delay(300);
					break;

					//Alphabet Caps
				case 4:
					alphaRow++;
					if(alphaRow > 1)
					{
						alphaRow = 0;
					}

					Lcd_cursor(&lcd, 2, cursorCol);
					Lcd_char(&lcd, alphabets[alphaRow][alphaCol]);
					//HAL_Delay(300);
					break;

					//Char parse
				case 5:
					if(namePos <= 17)
					{
						//Store letter in name array
						profResponses[chosen][namePos] = alphabets[alphaRow][alphaCol];
						namePos++;

						//Dont let cursor exceed 18 chars
						if (cursorCol < 18)
						{
							cursorCol++;
						}
						alphaCol = 0;
						alphaRow = 0;

						//Move cursor over one and reset alphabet
						Lcd_cursor(&lcd, 2, cursorCol);
						Lcd_char(&lcd, alphabets[alphaRow][alphaCol]);
						//HAL_Delay(300);
					}else
					{
						//Dont let username exceed 18 chars
						Lcd_cursor(&lcd, 3, 0);
						Lcd_string(&lcd, "    Max letters!    ");
						HAL_Delay(750);
					}
					break;

					//Alphabet Caps
				case 6:
					alphaRow--;
					if(alphaRow < 0)
					{
						alphaRow = 1;
					}

					Lcd_cursor(&lcd, 2, cursorCol);
					Lcd_char(&lcd, alphabets[alphaRow][alphaCol]);
					//HAL_Delay(300);
					break;

					//Alphabet Decrease
				case 9:
					alphaCol--;
					if(alphaCol < 0)
					{
						alphaCol = 26;
					}

					Lcd_cursor(&lcd, 2, cursorCol);
					Lcd_char(&lcd, alphabets[alphaRow][alphaCol]);
					//HAL_Delay(300);
					break;

					//Clear
				case 11:

					memset(profResponses[chosen], 0, 19);

					alphaRow = 1;
					alphaCol = 0;
					namePos = 0;
					cursorCol = 1;

					Lcd_cursor(&lcd, 3, 0);
					Lcd_string(&lcd, "                    ");

					Lcd_cursor(&lcd, 2, cursorCol);
					Lcd_string(&lcd, "                    ");
					Lcd_cursor(&lcd, 2, cursorCol);
					Lcd_char(&lcd, alphabets[alphaRow][alphaCol]);
					//HAL_Delay(300);
					break;

					//"Backspace"
				case 15:
					if(namePos > 0)
					{
						profResponses[chosen][namePos-1] = ' ';
						namePos -= 1;
						cursorCol -= 1;
						alphaRow = 1;
						alphaCol = 0;

						Lcd_cursor(&lcd, 2, cursorCol+1);
						Lcd_char(&lcd, ' ');
						Lcd_cursor(&lcd, 2, cursorCol);
						Lcd_char(&lcd, alphabets[alphaRow][alphaCol]);
						//HAL_Delay(300);
					}
					break;

					//Back
				case 7:
					menuState = 115;
					chosen = -1;
					resetAll();
					break;

				default:
					break;
				}
			}
		}

		//Status Menu
		if(menuState == 12)
		{
			Lcd_cursor(&lcd, 0, 4);
			Lcd_string(&lcd, "Pick Status");
			Lcd_cursor(&lcd, 1, 0);
			Lcd_string(&lcd, "1.Available  2.Busy");
			Lcd_cursor(&lcd, 2, 0);
			Lcd_string(&lcd, "3.Away");
			Lcd_cursor(&lcd, 3, 0);
			Lcd_string(&lcd, "Current: ");
			Lcd_cursor(&lcd, 3, 9);
			Lcd_string(&lcd, userStatus);

			returnKey = keypadRead();

			switch (returnKey) {
			case 0:
				strcpy(userStatus, "Available");
				menuState = 1;
				resetAll();
				break;

			case 1:
				strcpy(userStatus, "Busy");
				menuState = 1;
				resetAll();
				break;

			case 2:
				strcpy(userStatus, "Away");
				menuState = 1;
				resetAll();
				break;

			case 4:

				break;

			case 7:
				menuState = 1;
				resetAll();
				break;

			default:
				break;
			}
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
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the CPU, AHB and APB busses clocks
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
	/** Initializes the CPU, AHB and APB busses clocks
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
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void)
{

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 0;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */
	HAL_TIM_MspPostInit(&htim2);

}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void)
{

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 0;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 0;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */
	HAL_TIM_MspPostInit(&htim3);

}

/**
 * @brief TIM4 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM4_Init(void)
{

	/* USER CODE BEGIN TIM4_Init 0 */

	/* USER CODE END TIM4_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	/* USER CODE BEGIN TIM4_Init 1 */

	/* USER CODE END TIM4_Init 1 */
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 0;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 0;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM4_Init 2 */

	/* USER CODE END TIM4_Init 2 */
	HAL_TIM_MspPostInit(&htim4);

}

/**
 * @brief TIM15 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM15_Init(void)
{

	/* USER CODE BEGIN TIM15_Init 0 */

	/* USER CODE END TIM15_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM15_Init 1 */

	/* USER CODE END TIM15_Init 1 */
	htim15.Instance = TIM15;
	htim15.Init.Prescaler = 72-1;
	htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim15.Init.Period = 0xffff-1;
	htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim15.Init.RepetitionCounter = 0;
	htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim15) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim15, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM15_Init 2 */

	/* USER CODE END TIM15_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 38400;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, CSpin_Pin|CEpin_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, RS_Pin|EN_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, D4_Pin|D5_Pin|D6_Pin|D7_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : CSpin_Pin CEpin_Pin */
	GPIO_InitStruct.Pin = CSpin_Pin|CEpin_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : C0_In_Pin C1_In_Pin C2_In_Pin C3_In_Pin */
	GPIO_InitStruct.Pin = C0_In_Pin|C1_In_Pin|C2_In_Pin|C3_In_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pins : PE11 PE12 PE13 PE14 */
	GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pins : RS_Pin EN_Pin */
	GPIO_InitStruct.Pin = RS_Pin|EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : D4_Pin D5_Pin D6_Pin D7_Pin */
	GPIO_InitStruct.Pin = D4_Pin|D5_Pin|D6_Pin|D7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

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
void assert_failed(char *file, uint32_t line)
{ 
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
