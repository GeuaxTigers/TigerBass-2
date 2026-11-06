/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void send_midi_off(uint8_t j);
void send_midi_on(uint8_t j);
void disable_frets_ahead(uint8_t j);
void print_packet(uint8_t* packet);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define input_5_Pin GPIO_PIN_2
#define input_5_GPIO_Port GPIOE
#define input_37_Pin GPIO_PIN_3
#define input_37_GPIO_Port GPIOE
#define input_4_Pin GPIO_PIN_4
#define input_4_GPIO_Port GPIOE
#define input_38_Pin GPIO_PIN_5
#define input_38_GPIO_Port GPIOE
#define input_26_Pin GPIO_PIN_6
#define input_26_GPIO_Port GPIOE
#define input_14_Pin GPIO_PIN_13
#define input_14_GPIO_Port GPIOC
#define input_27_Pin GPIO_PIN_14
#define input_27_GPIO_Port GPIOC
#define input_3_Pin GPIO_PIN_15
#define input_3_GPIO_Port GPIOC
#define input_39_Pin GPIO_PIN_0
#define input_39_GPIO_Port GPIOF
#define input_13_Pin GPIO_PIN_1
#define input_13_GPIO_Port GPIOF
#define input_2_Pin GPIO_PIN_2
#define input_2_GPIO_Port GPIOF
#define input_40_Pin GPIO_PIN_3
#define input_40_GPIO_Port GPIOF
#define input_1_Pin GPIO_PIN_4
#define input_1_GPIO_Port GPIOF
#define input_28_Pin GPIO_PIN_5
#define input_28_GPIO_Port GPIOF
#define input_12_Pin GPIO_PIN_6
#define input_12_GPIO_Port GPIOF
#define input_30_Pin GPIO_PIN_7
#define input_30_GPIO_Port GPIOF
#define input_29_Pin GPIO_PIN_8
#define input_29_GPIO_Port GPIOF
#define input_11_Pin GPIO_PIN_9
#define input_11_GPIO_Port GPIOF
#define input_10_Pin GPIO_PIN_10
#define input_10_GPIO_Port GPIOF
#define HSE_IN_Pin GPIO_PIN_0
#define HSE_IN_GPIO_Port GPIOH
#define HSE_OUT_Pin GPIO_PIN_1
#define HSE_OUT_GPIO_Port GPIOH
#define input_31_Pin GPIO_PIN_1
#define input_31_GPIO_Port GPIOC
#define input_9_Pin GPIO_PIN_2
#define input_9_GPIO_Port GPIOC
#define input_32_Pin GPIO_PIN_3
#define input_32_GPIO_Port GPIOC
#define ADC_String_1_Pin GPIO_PIN_0
#define ADC_String_1_GPIO_Port GPIOA
#define ADC_String_2_Pin GPIO_PIN_1
#define ADC_String_2_GPIO_Port GPIOA
#define ADC_String_3_Pin GPIO_PIN_2
#define ADC_String_3_GPIO_Port GPIOA
#define ADC_String_4_Pin GPIO_PIN_3
#define ADC_String_4_GPIO_Port GPIOA
#define ADC_Octave_Pin GPIO_PIN_4
#define ADC_Octave_GPIO_Port GPIOA
#define ADC_Mode_Pin GPIO_PIN_5
#define ADC_Mode_GPIO_Port GPIOA
#define ADC_Bend_Pin GPIO_PIN_6
#define ADC_Bend_GPIO_Port GPIOA
#define ADC_MIDI_Ch_Pin GPIO_PIN_7
#define ADC_MIDI_Ch_GPIO_Port GPIOA
#define input_8_Pin GPIO_PIN_4
#define input_8_GPIO_Port GPIOC
#define input_33_Pin GPIO_PIN_5
#define input_33_GPIO_Port GPIOC
#define ADC_Slider_Pin GPIO_PIN_0
#define ADC_Slider_GPIO_Port GPIOB
#define input_7_Pin GPIO_PIN_11
#define input_7_GPIO_Port GPIOF
#define input_34_Pin GPIO_PIN_12
#define input_34_GPIO_Port GPIOF
#define input_6_Pin GPIO_PIN_13
#define input_6_GPIO_Port GPIOF
#define input_35_Pin GPIO_PIN_14
#define input_35_GPIO_Port GPIOF
#define input_60_Pin GPIO_PIN_15
#define input_60_GPIO_Port GPIOF
#define input_36_Pin GPIO_PIN_0
#define input_36_GPIO_Port GPIOG
#define input_61_Pin GPIO_PIN_1
#define input_61_GPIO_Port GPIOG
#define input_59_Pin GPIO_PIN_7
#define input_59_GPIO_Port GPIOE
#define input_62_Pin GPIO_PIN_8
#define input_62_GPIO_Port GPIOE
#define input_58_Pin GPIO_PIN_9
#define input_58_GPIO_Port GPIOE
#define input_63_Pin GPIO_PIN_10
#define input_63_GPIO_Port GPIOE
#define input_57_Pin GPIO_PIN_11
#define input_57_GPIO_Port GPIOE
#define input_65_Pin GPIO_PIN_12
#define input_65_GPIO_Port GPIOE
#define input_64_Pin GPIO_PIN_13
#define input_64_GPIO_Port GPIOE
#define input_55_Pin GPIO_PIN_14
#define input_55_GPIO_Port GPIOE
#define input_56_Pin GPIO_PIN_15
#define input_56_GPIO_Port GPIOE
#define input_66_Pin GPIO_PIN_10
#define input_66_GPIO_Port GPIOB
#define input_54_Pin GPIO_PIN_11
#define input_54_GPIO_Port GPIOB
#define input_67_Pin GPIO_PIN_12
#define input_67_GPIO_Port GPIOB
#define input_53_Pin GPIO_PIN_14
#define input_53_GPIO_Port GPIOB
#define input_68_Pin GPIO_PIN_15
#define input_68_GPIO_Port GPIOB
#define UART_TX_Pin GPIO_PIN_8
#define UART_TX_GPIO_Port GPIOD
#define input_52_Pin GPIO_PIN_9
#define input_52_GPIO_Port GPIOD
#define input_69_Pin GPIO_PIN_13
#define input_69_GPIO_Port GPIOD
#define input_51_Pin GPIO_PIN_14
#define input_51_GPIO_Port GPIOD
#define input_70_Pin GPIO_PIN_15
#define input_70_GPIO_Port GPIOD
#define input_50_Pin GPIO_PIN_3
#define input_50_GPIO_Port GPIOG
#define input_71_Pin GPIO_PIN_5
#define input_71_GPIO_Port GPIOG
#define input_49_Pin GPIO_PIN_7
#define input_49_GPIO_Port GPIOG
#define input_72_Pin GPIO_PIN_8
#define input_72_GPIO_Port GPIOG
#define input_48_Pin GPIO_PIN_6
#define input_48_GPIO_Port GPIOC
#define input_73_Pin GPIO_PIN_8
#define input_73_GPIO_Port GPIOC
#define input_47_Pin GPIO_PIN_9
#define input_47_GPIO_Port GPIOC
#define input_74_Pin GPIO_PIN_8
#define input_74_GPIO_Port GPIOA
#define VBUS_Sense_Pin GPIO_PIN_9
#define VBUS_Sense_GPIO_Port GPIOA
#define VBUS_Sense_EXTI_IRQn EXTI9_5_IRQn
#define input_46_Pin GPIO_PIN_10
#define input_46_GPIO_Port GPIOA
#define USB_M_Pin GPIO_PIN_11
#define USB_M_GPIO_Port GPIOA
#define USB_P_Pin GPIO_PIN_12
#define USB_P_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define input_75_Pin GPIO_PIN_15
#define input_75_GPIO_Port GPIOA
#define input_45_Pin GPIO_PIN_10
#define input_45_GPIO_Port GPIOC
#define input_76_Pin GPIO_PIN_11
#define input_76_GPIO_Port GPIOC
#define input_44_Pin GPIO_PIN_12
#define input_44_GPIO_Port GPIOC
#define input_77_Pin GPIO_PIN_0
#define input_77_GPIO_Port GPIOD
#define input_43_Pin GPIO_PIN_1
#define input_43_GPIO_Port GPIOD
#define input_78_Pin GPIO_PIN_2
#define input_78_GPIO_Port GPIOD
#define input_42_Pin GPIO_PIN_3
#define input_42_GPIO_Port GPIOD
#define input_79_Pin GPIO_PIN_4
#define input_79_GPIO_Port GPIOD
#define input_41_Pin GPIO_PIN_5
#define input_41_GPIO_Port GPIOD
#define input_80_Pin GPIO_PIN_6
#define input_80_GPIO_Port GPIOD
#define input_20_Pin GPIO_PIN_7
#define input_20_GPIO_Port GPIOD
#define input_21_Pin GPIO_PIN_9
#define input_21_GPIO_Port GPIOG
#define String_1_out_Pin GPIO_PIN_10
#define String_1_out_GPIO_Port GPIOG
#define String_2_out_Pin GPIO_PIN_11
#define String_2_out_GPIO_Port GPIOG
#define String_3_out_Pin GPIO_PIN_12
#define String_3_out_GPIO_Port GPIOG
#define String_4_out_Pin GPIO_PIN_13
#define String_4_out_GPIO_Port GPIOG
#define input_19_Pin GPIO_PIN_14
#define input_19_GPIO_Port GPIOG
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define input_22_Pin GPIO_PIN_4
#define input_22_GPIO_Port GPIOB
#define input_18_Pin GPIO_PIN_5
#define input_18_GPIO_Port GPIOB
#define input_23_Pin GPIO_PIN_6
#define input_23_GPIO_Port GPIOB
#define input_17_Pin GPIO_PIN_7
#define input_17_GPIO_Port GPIOB
#define input_16_Pin GPIO_PIN_8
#define input_16_GPIO_Port GPIOB
#define input_24_Pin GPIO_PIN_9
#define input_24_GPIO_Port GPIOB
#define input_25_Pin GPIO_PIN_0
#define input_25_GPIO_Port GPIOE
#define input_15_Pin GPIO_PIN_1
#define input_15_GPIO_Port GPIOE
#define LED_OUT_Pin GPIO_PIN_7
#define LED_OUT_GPIO_Port GPIOI

/* USER CODE BEGIN Private defines */
typedef struct{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
} fret_port_pin;

typedef struct{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
} string_port_pin;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
