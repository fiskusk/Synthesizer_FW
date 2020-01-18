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
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */


/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/** @defgroup   PLO_module_controls
  * @brief      PLO module controls exported macros
  * @{
  */
 
/**< Activates the output amplifier in the frequency synthesizer baseband (23.5 MHz to 6 GHz)     */
#define PLO_MODULE_OUT1_ON      HAL_GPIO_WritePin(RF_OUT1_GPIO_Port, RF_OUT1_Pin, GPIO_PIN_RESET)

/**< Deactivates the output amplifier in the frequency synthesizer baseband (23.5 MHz to 6 GHz)   */
#define PLO_MODULE_OUT1_OFF     HAL_GPIO_WritePin(RF_OUT1_GPIO_Port, RF_OUT1_Pin, GPIO_PIN_SET)

/**<    Activates the second output which include frequency doubler 
 *      for output frequencies from 5 GHz to 12 GHz)     */
#define PLO_MODULE_OUT2_ON      HAL_GPIO_WritePin(RF_OUT2_GPIO_Port, RF_OUT2_Pin, GPIO_PIN_RESET)

/**<    Deactivates the second output which include frequency doubler 
 *      for output frequencies from 5 GHz to 12 GHz)     */
#define PLO_MODULE_OUT2_OFF     HAL_GPIO_WritePin(RF_OUT2_GPIO_Port, RF_OUT2_Pin, GPIO_PIN_SET)

/**< Activate the internal 10 MHz signal reference for the frequency synthesizer  */
#define PLO_MODULE_INT_REF      HAL_GPIO_WritePin(INT_EXT_REF_GPIO_Port, INT_EXT_REF_Pin, GPIO_PIN_RESET)

/**<    Activates the external signal reference for the frequency synthesizer 
 *      that is connected to the input reference signal SMA connector. */
#define PLO_MODULE_EXT_REF      HAL_GPIO_WritePin(INT_EXT_REF_GPIO_Port, INT_EXT_REF_Pin, GPIO_PIN_SET)

/**
  * @}
  */

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
#define JP1_EXTI_IRQn EXTI4_15_IRQn
#define JP2_Pin GPIO_PIN_7
#define JP2_GPIO_Port GPIOA
#define JP2_EXTI_IRQn EXTI4_15_IRQn
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
