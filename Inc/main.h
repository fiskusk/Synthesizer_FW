/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define CMD_BUFFER_LEN 100
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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RF_OUT1_Pin GPIO_PIN_0
#define RF_OUT1_GPIO_Port GPIOF
#define PLO_LE_Pin GPIO_PIN_0
#define PLO_LE_GPIO_Port GPIOA
#define PLO_DATA_Pin GPIO_PIN_2
#define PLO_DATA_GPIO_Port GPIOA
#define PLO_CLK_Pin GPIO_PIN_3
#define PLO_CLK_GPIO_Port GPIOA
#define INT_EXT_REF_Pin GPIO_PIN_4
#define INT_EXT_REF_GPIO_Port GPIOA
#define RF_OUT2_Pin GPIO_PIN_5
#define RF_OUT2_GPIO_Port GPIOA
#define JP1_Pin GPIO_PIN_6
#define JP1_GPIO_Port GPIOA
#define JP2_Pin GPIO_PIN_7
#define JP2_GPIO_Port GPIOA
#define PLO_MUXOUT_Pin GPIO_PIN_1
#define PLO_MUXOUT_GPIO_Port GPIOB
#define PLO_MUXOUT_EXTI_IRQn EXTI0_1_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
