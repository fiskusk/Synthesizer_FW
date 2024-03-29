/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "max2871.h"
#include "flash.h"
#include "timer.h"
#include "usb.h"
#include "usbd_cdc_if.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_it.h"
#include "stm32f0xx_ll_adc.h"
#include "stdio.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    temp = HAL_ADC_GetValue(hadc);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (host_com_port_open_closed == HOST_COM_PORT_OPENED && GPIO_Pin == GPIO_PIN_1)
        plo_check_lock_status(); // only if serial port opened
}

/**
  * @brief  Function that call in while endless loop. Check, if serial port was 
  *         open or was close.
  *
  *         @note
  *         When serial port is open, process circle buffer for lock state 
  *         events and buffered usb command data.
  *         When ser. port is close, check if jumper select position is changed. 
  *         Than apply appropriate changes.
  */
void running_routine(void)
{
    switch (host_com_port_open_closed)
    {
        case HOST_COM_PORT_OPENED:
            //PLO_MODULE_OUT2_ON; // TODO for test purpose only

            if (tick_handle == TICK_OCCUR) {
                HAL_ADC_Start_IT(&hadc);
            }
            // check if new plo lock status occur and eventually process them
            usb_procesing_command_data();
            
            uint8_t data;
            while (plo_buff_pop(&data))
            {
                plo_process_lock_status((bool)data);
            }
            // check if new usb command is ready and eventually process them.
            break;
        case HOST_COM_PORT_CLOSED:
            //PLO_MODULE_OUT2_OFF; // TODO for test purpose only
            // if jumper possition has changed, load new setting into plo
            if (memory_select_event == MEMORY_SELECT_CHANGED)
                apply_memory_select_changed(PLO_NEW_DATA);
            break;
        default:
            break;
    }
}

/**
  * @brief This function run after startup and if stored data isn't write to data
  *        memory, load default values into memory. After that apply changes and
  *        sellect plo setting according to jumper position.
  */
void init_routine(void)
{
    if (saved_data_1[0] == 0x0)
    {
        load_default_memory_register_values();
    }
    apply_memory_select_changed(PLO_INIT);
}

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
  MX_USB_DEVICE_Init();
  MX_TIM3_Init();
  MX_ADC_Init();
  /* USER CODE BEGIN 2 */
    PLO_MODULE_OUT2_OFF;
    PLO_MODULE_OUT1_OFF;

    HAL_TIM_Base_Start_IT(&htim3);  // start timer 3
    HAL_ADCEx_Calibration_Start(&hadc);
    setbuf(stdout, NULL);           // it is necessary to redirect to usb

    HAL_Delay(500);
    init_routine();     // initialize user settings

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        running_routine();
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI14|RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
