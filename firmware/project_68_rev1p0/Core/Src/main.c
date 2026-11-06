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
#include "main.h" //Header File
#include "usb_device.h" //For USB connection

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h" //Needed for standard functions
#include "usbd_hid.h" //For Sending MIDI through USB

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
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim8;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
uint8_t midiNoteOn[4];
uint8_t midiNoteOff[4];

uint8_t midi_msg_off[4]; //  = {0x08,0x80,0x0,0x7F}; // Only modify midi_msg_on[2] and midi_msg_on[3] field (note and velocity)

uint8_t midi_msg_on[4];  // Only modify midi_msg_off[2] field (note)

uint8_t NOTE_ON_Flag[80] = {0}; //this is going to be modified to hold for all segments (notes)

uint16_t ADC_Strings[4];
uint16_t ADC_Data[9];

	// MIDI NOTE TABLE

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){

	ADC_Strings[0] = (uint16_t)ADC_Data[0];
	ADC_Strings[1] = (uint16_t)ADC_Data[1];
	ADC_Strings[2] = (uint16_t)ADC_Data[2];
	ADC_Strings[3] = (uint16_t)ADC_Data[3];

//	printf("in complete callback\n\r");

}

uint8_t notes[80] = {

	// String 1 //

	0x2F, // Fret 20, B2
	0x2E, // Fret 19, A2b
	0x2D, // Fret 18, A2
	0x2C, // Fret 17, G2b
	0x2B, // Fret 16, G2
	0x2A, // Fret 15, F2b
	0x29, // Fret 14, F2
	0x28, // Fret 13, E2
	0x27, // Fret 12, D2b
	0x26, // Fret 11, D2
	0x25, // Fret 10, C2b
	0x24, // Fret 9,  C2
	0x23, // Fret 8,  B1
	0x22, // Fret 7,  A1b
	0x21, // Fret 6,  A1
	0x20, // Fret 5,  G1b
	0x1F, // Fret 4,  G1
	0x1E, // Fret 3,  F1b
	0x1D, // Fret 2,  F1
	0x1C, // Fret 1,  E1

	// String 2 //

	0x34, // Fret 20, E3
	0x33, // Fret 19, D3b
	0x32, // Fret 18, D3
	0x31, // Fret 17, C3b
	0x30, // Fret 16, C3
	0x2F, // Fret 15, B2
	0x2E, // Fret 14, A2b
	0x2D, // Fret 13, A2
	0x2C, // Fret 12, G2b
	0x2B, // Fret 11, G2
	0x2A, // Fret 10, F2b
	0x29, // Fret 9,  F2
	0x28, // Fret 8,  E2
	0x27, // Fret 7,  D2b
	0x26, // Fret 6,  D2
	0x25, // Fret 5,  C2b
	0x24, // Fret 4,  C2
	0x23, // Fret 3,  B1
	0x22, // Fret 2,  A1b
	0x21, // Fret 1,  A1

	// String 3 //

    0x39, // Fret 20, A3
	0x38, // Fret 19, G3b
	0x37, // Fret 18, G3
	0x36, // Fret 17, F3b
	0x35, // Fret 16, F3
	0x34, // Fret 15, E3
	0x33, // Fret 14, D3b
	0x32, // Fret 13, D3
	0x31, // Fret 12, C3b
	0x30, // Fret 11, C3
	0x2F, // Fret 10, B2
	0x2E, // Fret 9,  A2b
	0x2D, // Fret 8,  A2
	0x2C, // Fret 7,  G2b
	0x2B, // Fret 6,  G2
	0x2A, // Fret 5,  F2b
	0x29, // Fret 4,  F2
	0x28, // Fret 3,  E2
	0x27, // Fret 2,  D2b
	0x26, // Fret 1,  D2

	// String 4 //

	0x3E, // Fret 20, D4
	0x3D, // Fret 19, C4b
	0x3C, // Fret 18, C4
	0x3B, // Fret 17, B3
	0x3A, // Fret 16, A3b
	0x39, // Fret 15, A3
	0x38, // Fret 14, G3b
	0x37, // Fret 13, G3
	0x36, // Fret 12, F3b
	0x35, // Fret 11, F3
	0x34, // Fret 10, E3
	0x33, // Fret 9,  D3b
	0x32, // Fret 8,  D3
	0x31, // Fret 7,  C3b
	0x30, // Fret 6,  C3
	0x2F, // Fret 5,  B2
	0x2E, // Fret 4,  A2b
	0x2D, // Fret 3,  A2
	0x2C, // Fret 2,  G2b
	0x2B, // Fret 1,  G2
	};

string_port_pin spp[4] = {//This holds the ports and pins for the string outputs
		{String_1_out_GPIO_Port, String_1_out_Pin},
		{String_2_out_GPIO_Port, String_2_out_Pin},
		{String_3_out_GPIO_Port, String_3_out_Pin},
		{String_4_out_GPIO_Port, String_4_out_Pin},
};
fret_port_pin fpp[80] = { //This holds EVERY port and pin for each of the fret segments
		{input_1_GPIO_Port, input_1_Pin},
		{input_2_GPIO_Port, input_2_Pin},
		{input_3_GPIO_Port, input_3_Pin},
		{input_4_GPIO_Port, input_4_Pin},
		{input_5_GPIO_Port, input_5_Pin},
		{input_6_GPIO_Port, input_6_Pin},
		{input_7_GPIO_Port, input_7_Pin},
		{input_8_GPIO_Port, input_8_Pin},
		{input_9_GPIO_Port, input_9_Pin},
		{input_10_GPIO_Port, input_10_Pin},
		{input_11_GPIO_Port, input_11_Pin},
		{input_12_GPIO_Port, input_12_Pin},
		{input_13_GPIO_Port, input_13_Pin},
		{input_14_GPIO_Port, input_14_Pin},
		{input_15_GPIO_Port, input_15_Pin},
		{input_16_GPIO_Port, input_16_Pin},
		{input_17_GPIO_Port, input_17_Pin},
		{input_18_GPIO_Port, input_18_Pin},
		{input_19_GPIO_Port, input_19_Pin},
		{input_20_GPIO_Port, input_20_Pin},

		{input_21_GPIO_Port, input_21_Pin},
		{input_22_GPIO_Port, input_22_Pin},
		{input_23_GPIO_Port, input_23_Pin},
		{input_24_GPIO_Port, input_24_Pin},
		{input_25_GPIO_Port, input_25_Pin},
		{input_26_GPIO_Port, input_26_Pin},
		{input_27_GPIO_Port, input_27_Pin},
		{input_28_GPIO_Port, input_28_Pin},
		{input_29_GPIO_Port, input_29_Pin},
		{input_30_GPIO_Port, input_30_Pin},
		{input_31_GPIO_Port, input_31_Pin},
		{input_32_GPIO_Port, input_32_Pin},
		{input_33_GPIO_Port, input_33_Pin},
		{input_34_GPIO_Port, input_34_Pin},
		{input_35_GPIO_Port, input_35_Pin},
		{input_36_GPIO_Port, input_36_Pin},
		{input_37_GPIO_Port, input_37_Pin},
		{input_38_GPIO_Port, input_38_Pin},
		{input_39_GPIO_Port, input_39_Pin},
		{input_40_GPIO_Port, input_40_Pin},

		{input_41_GPIO_Port, input_41_Pin},
		{input_42_GPIO_Port, input_42_Pin},
		{input_43_GPIO_Port, input_43_Pin},
		{input_44_GPIO_Port, input_44_Pin},
		{input_45_GPIO_Port, input_45_Pin},
		{input_46_GPIO_Port, input_46_Pin},
		{input_47_GPIO_Port, input_47_Pin},
		{input_48_GPIO_Port, input_48_Pin},
		{input_49_GPIO_Port, input_49_Pin},
		{input_50_GPIO_Port, input_50_Pin},
		{input_51_GPIO_Port, input_51_Pin},
		{input_52_GPIO_Port, input_52_Pin},
		{input_53_GPIO_Port, input_53_Pin},
		{input_54_GPIO_Port, input_54_Pin},
		{input_55_GPIO_Port, input_55_Pin},
		{input_56_GPIO_Port, input_56_Pin},
		{input_57_GPIO_Port, input_57_Pin},
		{input_58_GPIO_Port, input_58_Pin},
		{input_59_GPIO_Port, input_59_Pin},
		{input_60_GPIO_Port, input_60_Pin},

		{input_61_GPIO_Port, input_61_Pin},
		{input_62_GPIO_Port, input_62_Pin},
		{input_63_GPIO_Port, input_63_Pin},
		{input_64_GPIO_Port, input_64_Pin},
		{input_65_GPIO_Port, input_65_Pin},
		{input_66_GPIO_Port, input_66_Pin},
		{input_67_GPIO_Port, input_67_Pin},
		{input_68_GPIO_Port, input_68_Pin},
		{input_69_GPIO_Port, input_69_Pin},
		{input_70_GPIO_Port, input_70_Pin},
		{input_71_GPIO_Port, input_71_Pin},
		{input_72_GPIO_Port, input_72_Pin},
		{input_73_GPIO_Port, input_73_Pin},
		{input_74_GPIO_Port, input_74_Pin},
		{input_75_GPIO_Port, input_75_Pin},
		{input_76_GPIO_Port, input_76_Pin},
		{input_77_GPIO_Port, input_77_Pin},
		{input_78_GPIO_Port, input_78_Pin},
		{input_79_GPIO_Port, input_79_Pin},
		{input_80_GPIO_Port, input_80_Pin},
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM8_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

	midi_msg_off[0] = 0x08; //For USB MIDI, need this byte which describes wire number(0, similar to channel), and function(8, OFF msg)
	midi_msg_off[1] = 0x80; //Actual status byte (8 is OFF msg, 0 is channel)
	midi_msg_off[2] = 0x00; //Will be filled with correct note using while(1) and associated functions called below
	midi_msg_off[3] = 0x7F; //Velocity value, set to max for now (0x7F = 127) since ADC is not functional.


	midi_msg_on[0] = 0x09; //Same as above, but ON message = 9
	midi_msg_on[1] = 0x90;
	midi_msg_on[2] = 0x00;
	midi_msg_on[3] = 0x7F;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */









int debug_flag = 0; //This is some weird shite connor did for debugging idk how it works
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init(); //!!!!!!!!! DMA and ADC init, but NOT used due to ADC issue
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  MX_USB_DEVICE_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
  printf("Program Starting\n\r");
//  HAL_ADC_Start_DMA(&hadc1, (uint32_t *) ADC_Data, 9);
//
//  HAL_TIM_Base_Start(&htim8);

  printf("adc and tim8 started\n\r");
//  HAL_GPIO_WritePin(String_1_out_GPIO_Port, String_1_out_Pin, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) //The first bit of code below commented out is experimenting with ADC, did not work. Also some debugging below it
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	  HAL_Delay(1000);
//	  printf("in while loop\n\r");
//	  printf("String 1 = %hu\n\r", ADC_Strings[0]);
//	  printf("String 2 = %hu\n\r", ADC_Strings[1]);
//	  printf("String 3 = %hu\n\r", ADC_Strings[2]);
//	  printf("String 4 = %hu\n\r", ADC_Strings[3]);


//	  HAL_GPIO_WritePin(LED_OUT_GPIO_Port, LED_OUT_Pin, 0);
//	  while( ((USBD_HID_HandleTypeDef *) hUsbDeviceFS.pClassDataCmsit[hUsbDeviceFS.classId])->state == USBD_HID_BUSY ) {}
//	  USBD_HID_SendReport(&hUsbDeviceFS, midi_msg_on, 4); // Send MIDI OFF Packet
//	  HAL_Delay(1000);
//	  HAL_GPIO_WritePin(LED_OUT_GPIO_Port, LED_OUT_Pin, 1);
//	  while( ((USBD_HID_HandleTypeDef *) hUsbDeviceFS.pClassDataCmsit[hUsbDeviceFS.classId])->state == USBD_HID_BUSY ) {}
//	  USBD_HID_SendReport(&hUsbDeviceFS, midi_msg_off, 4); // Send MIDI OFF Packet
//	  HAL_Delay(1000);



  if(!debug_flag){ //Again connor doing weird shite
	  for (int i = 0; i < 4; i++){ //Outer loop for string iterations
		  HAL_GPIO_WritePin(spp[i].GPIOx, spp[i].GPIO_Pin, 1); //Activate string i
		  for (int j = (i*20); j < ((i*20) + 20); j++){ //Inner loop for fret iterations/scans, makes sure to start at 0, 20, 40, 60 and only scans 20 at a time.
			  if ( HAL_GPIO_ReadPin(fpp[j].GPIOx, fpp[j].GPIO_Pin) ){ //If fret j is active...
				if(NOTE_ON_Flag[j] == 1){ //If note is already on, do not send another and break.
					break; //Once active note is found and sent, break to avoid scanning others.
					// can either continue here or send another NOTE_ON (sends at max velocity currently)
				}
				else{ //If not on, set flag to on and send midi note.

					NOTE_ON_Flag[j] = 1;
					send_midi_on(j); //This is an external function below that appends the correct note value into the message and sends it
					// Make sure the USB functions are not BUSY before sending the MIDI Message
					if(j != 19 && j != 39 && j != 59 && j != 79){ //If current note is not an end note (has a playable fret behind it, will be 19, 39, 59, 79 for segments)
						disable_frets_ahead(j); //External function that kills all notes behind targeted fret so it acts like a guitar would
					}
				}
				break; //Once active note is found and sent, break to avoid scanning others.

			  }
			  else{// ** Fret is INACTIVE
				  if(NOTE_ON_Flag[j] == 1){ //If note is already sent/on...
					  // Note was previously on. Turn it off now.
					  send_midi_off(j);
					  HAL_Delay(5); //Delay is here to minimize ghost note behind targeted fret when pulling off/hammering on
				  }
			  }
		  }
		  HAL_GPIO_WritePin(spp[i].GPIOx, spp[i].GPIO_Pin, 0); //Deactivate current string and iterate to next
		}
//	  HAL_Delay(25); //This was used for testing can be deleted

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 192;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T8_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 9;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Rank = 6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Rank = 7;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Rank = 8;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Rank = 9;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM8_Init(void)
{

  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM8_Init 1 */

  /* USER CODE END TIM8_Init 1 */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 9600-1;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 10-1;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */

  /* USER CODE END TIM8_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_HalfDuplex_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, String_1_out_Pin|String_2_out_Pin|String_3_out_Pin|String_4_out_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_OUT_GPIO_Port, LED_OUT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : input_5_Pin input_37_Pin input_4_Pin input_38_Pin
                           input_26_Pin input_59_Pin input_62_Pin input_58_Pin
                           input_63_Pin input_57_Pin input_65_Pin input_64_Pin
                           input_55_Pin input_56_Pin input_25_Pin input_15_Pin */
  GPIO_InitStruct.Pin = input_5_Pin|input_37_Pin|input_4_Pin|input_38_Pin
                          |input_26_Pin|input_59_Pin|input_62_Pin|input_58_Pin
                          |input_63_Pin|input_57_Pin|input_65_Pin|input_64_Pin
                          |input_55_Pin|input_56_Pin|input_25_Pin|input_15_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : input_14_Pin input_27_Pin input_3_Pin input_31_Pin
                           input_9_Pin input_32_Pin input_8_Pin input_33_Pin
                           input_48_Pin input_73_Pin input_47_Pin input_45_Pin
                           input_76_Pin input_44_Pin */
  GPIO_InitStruct.Pin = input_14_Pin|input_27_Pin|input_3_Pin|input_31_Pin
                          |input_9_Pin|input_32_Pin|input_8_Pin|input_33_Pin
                          |input_48_Pin|input_73_Pin|input_47_Pin|input_45_Pin
                          |input_76_Pin|input_44_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : input_39_Pin input_13_Pin input_2_Pin input_40_Pin
                           input_1_Pin input_28_Pin input_12_Pin input_30_Pin
                           input_29_Pin input_11_Pin input_10_Pin input_7_Pin
                           input_34_Pin input_6_Pin input_35_Pin input_60_Pin */
  GPIO_InitStruct.Pin = input_39_Pin|input_13_Pin|input_2_Pin|input_40_Pin
                          |input_1_Pin|input_28_Pin|input_12_Pin|input_30_Pin
                          |input_29_Pin|input_11_Pin|input_10_Pin|input_7_Pin
                          |input_34_Pin|input_6_Pin|input_35_Pin|input_60_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : input_36_Pin input_61_Pin input_50_Pin input_71_Pin
                           input_49_Pin input_72_Pin input_21_Pin input_19_Pin */
  GPIO_InitStruct.Pin = input_36_Pin|input_61_Pin|input_50_Pin|input_71_Pin
                          |input_49_Pin|input_72_Pin|input_21_Pin|input_19_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : input_66_Pin input_54_Pin input_67_Pin input_53_Pin
                           input_68_Pin input_22_Pin input_18_Pin input_23_Pin
                           input_17_Pin input_16_Pin input_24_Pin */
  GPIO_InitStruct.Pin = input_66_Pin|input_54_Pin|input_67_Pin|input_53_Pin
                          |input_68_Pin|input_22_Pin|input_18_Pin|input_23_Pin
                          |input_17_Pin|input_16_Pin|input_24_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : input_52_Pin input_69_Pin input_51_Pin input_70_Pin
                           input_77_Pin input_43_Pin input_78_Pin input_42_Pin
                           input_79_Pin input_41_Pin input_80_Pin input_20_Pin */
  GPIO_InitStruct.Pin = input_52_Pin|input_69_Pin|input_51_Pin|input_70_Pin
                          |input_77_Pin|input_43_Pin|input_78_Pin|input_42_Pin
                          |input_79_Pin|input_41_Pin|input_80_Pin|input_20_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : input_74_Pin input_46_Pin input_75_Pin */
  GPIO_InitStruct.Pin = input_74_Pin|input_46_Pin|input_75_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : VBUS_Sense_Pin */
  GPIO_InitStruct.Pin = VBUS_Sense_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VBUS_Sense_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : String_1_out_Pin String_2_out_Pin String_3_out_Pin String_4_out_Pin */
  GPIO_InitStruct.Pin = String_1_out_Pin|String_2_out_Pin|String_3_out_Pin|String_4_out_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_OUT_Pin */
  GPIO_InitStruct.Pin = LED_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LED_OUT_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
int __io_putchar(int ch)
{
 // Write character to ITM ch.0
 ITM_SendChar(ch);
 return(ch);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	printf("Entered EXTI Callback Routine\n\r");
  if (GPIO_Pin == VBUS_Sense_Pin)
  {
    if (HAL_GPIO_ReadPin(VBUS_Sense_GPIO_Port, VBUS_Sense_Pin) == GPIO_PIN_SET)
    {
       MX_USB_DEVICE_Init(); // reinitialize USB
    }
    else{
    	MX_USB_DEVICE_DeInit();
    }
  }
	printf("Left EXTI Callback Routine\n\r");
}

void send_midi_off(uint8_t j){ // Ahhhh finally, the externals. This is the send function that appends the correct note and sends the packet

	  while( ((USBD_HID_HandleTypeDef *) hUsbDeviceFS.pClassDataCmsit[hUsbDeviceFS.classId])->state == USBD_HID_BUSY ) {} //Must check if USB is busy first
	  midi_msg_off[2] = notes[j]; //Append correct note using value of j and note array
//	  print_packet(midi_msg_off);
	  USBD_HID_SendReport(&hUsbDeviceFS, midi_msg_off, 4); // Send MIDI OFF Packet
	  NOTE_ON_Flag[j] = 0; //When note is off reset its flag to 0
}

void send_midi_on(uint8_t j){ //Same as above
	while( ((USBD_HID_HandleTypeDef *) hUsbDeviceFS.pClassDataCmsit[hUsbDeviceFS.classId])->state == USBD_HID_BUSY ) {}
	midi_msg_on[2] = notes[j];
//	print_packet(midi_msg_on);
	USBD_HID_SendReport(&hUsbDeviceFS, midi_msg_on, 4); // Send MIDI ON Packet

}

void disable_frets_ahead(uint8_t j){ //Disable frets function, must only disable on the string that targeted note was found on

	while((j+1) % 20 != 0){
		while( ((USBD_HID_HandleTypeDef *) hUsbDeviceFS.pClassDataCmsit[hUsbDeviceFS.classId])->state == USBD_HID_BUSY ) {}
		midi_msg_off[2] = notes[j+1];
		USBD_HID_SendReport(&hUsbDeviceFS, midi_msg_off, 4); //Turn off fret behind it
		NOTE_ON_Flag[j+1] = 0; //set fret behind current one's flag to 0 since it is now off
		j++;
	}

}

void print_packet(uint8_t* packet){ //For debugging purposes
	for(uint8_t i = 0; i < 4; i++){
		printf("packet[%i] = %02X ",i,packet[i]);
	}
	printf("\n\r");
}
// AYEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE!!!!!!!!!!!! I believe this marks the end of the code, good luck to whoever screws with this
//Rest of the stuff in the middle of the externals, the while/main, and below are generated by controller for inits and whatnot dont worry bout it brah.
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
