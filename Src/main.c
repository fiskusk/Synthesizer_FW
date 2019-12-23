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
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "max2871.h"
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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim -> Instance == TIM3)
    {
        HAL_GPIO_TogglePin(RF_OUT2_GPIO_Port, RF_OUT2_Pin);
    }
}

uint32_t hex2int(char *hex) {
    uint32_t val = 0;
    while (*hex) {
        // get current character then increment
        uint8_t byte = *hex++; 
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;    
        // shift 4 to make space for new digit, and add the 4 bits of the new digit 
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}

uint32_t usb_process_command(uint32_t *test_data, char *command_data)
{
    char *token;
    char *sub_token;
    char *value;

    for (uint8_t i = 0; i < strlen(command_data); i++)
    {
        command_data[i] = (command_data[i] < 32 || command_data[i] > 126) ? '\0' : command_data[i];
    }

    token = strtok(command_data, " ");

    if (strcasecmp(token, "ref") == 0)
    {
        value = strtok(NULL, " ");
        if (strcasecmp(value, "ext") == 0) {
            HAL_GPIO_WritePin(INT_EXT_REF_GPIO_Port, INT_EXT_REF_Pin, GPIO_PIN_SET);
        }

        if (strcasecmp(value, "int") == 0) {
            HAL_GPIO_WritePin(INT_EXT_REF_GPIO_Port, INT_EXT_REF_Pin, GPIO_PIN_RESET);
        }
        plo_new_data=PLO_DATA_SENDED;
    }

    if (strcasecmp(token, "out") == 0)
    {
        sub_token = strtok(NULL, " ");
        value = strtok(NULL, " ");
        if (strcasecmp(sub_token, "1") == 0) {
            if (strcasecmp(value, "on") == 0)
                HAL_GPIO_WritePin(RF_OUT1_GPIO_Port, RF_OUT1_Pin, GPIO_PIN_RESET);
            else if (strcasecmp(value, "off") == 0)
                HAL_GPIO_WritePin(RF_OUT1_GPIO_Port, RF_OUT1_Pin, GPIO_PIN_SET);
            plo_new_data=PLO_DATA_SENDED;
        }

        if (strcasecmp(sub_token, "2") == 0) {
            if (strcasecmp(value, "on") == 0)
                HAL_GPIO_WritePin(RF_OUT2_GPIO_Port, RF_OUT2_Pin, GPIO_PIN_RESET);
            else if (strcasecmp(value, "off") == 0)
                HAL_GPIO_WritePin(RF_OUT2_GPIO_Port, RF_OUT2_Pin, GPIO_PIN_SET);
            plo_new_data=PLO_DATA_SENDED;
        }
    }
    
    if (strcasecmp(token, "plo") == 0)
    {
        sub_token = strtok(NULL, " ");
        value = strtok(NULL, " ");
        if (strcasecmp(sub_token, "init") == 0)
        {
            plo_new_data = PLO_INIT;
        }

        if (strcasecmp(sub_token, "set_register") == 0)
        {
            uint32_t new_data = hex2int(value);
            // save register into test_data variable
            if ((new_data & 0x07) == 0x00)
                test_data[0] = new_data;
            else if ((new_data & 0x07) == 0x01)
                test_data[1] = new_data;
            else if ((new_data & 0x07) == 0x02)
                test_data[2] = new_data;
            else if ((new_data & 0x07) == 0x03)
                test_data[3] = new_data;
            else if ((new_data & 0x07) == 0x04)
                test_data[4] = new_data;
            else if ((new_data & 0x07) == 0x05)
                test_data[5] = new_data;

            plo_new_data = PLO_CHANGED_REGISTER;

            return new_data;
        }

    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    /*
    uint32_t test_data[6] = {
        0x80C80000, 0x800103E9, 0x00005F42,
        0x00001F23, 0x63BE80E4, 0x00400005
    };
    */
    
    uint32_t* test_data = (uint32_t*)malloc(sizeof(uint32_t)*6);
    test_data[0] = 0x80C80000;
    test_data[1] = 0x800103E9;
    test_data[2] = 0x00005F42;
    test_data[3] = 0x00001F23;
    test_data[4] = 0x63BE80E4;
    test_data[5] = 0x00400005;
    
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
    proccesing_command_1 = false;
    proccesing_command_2 = false;
    proccesing_command_3 = false;
    proccesing_command_4 = false;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // TODO odeslat informaci o zaveseni

    if (proccesing_command_1 == true || proccesing_command_2 == true || proccesing_command_3 == true || proccesing_command_4 == true)
    {
        if (proccesing_command_1 == true)
        {
            uint32_t new_register_value1 = usb_process_command(test_data, command_data_1);
            if (plo_new_data == PLO_INIT)
            {
                // toggle pin for trigger logic analyzer
                HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_RESET);
                plo_write_all(test_data, PLO_INIT);
                plo_write_all(test_data, PLO_INIT);
                plo_write_all(test_data, PLO_OUT_ENABLE);
                plo_new_data=PLO_DATA_SENDED;
            }
            else if (plo_new_data == PLO_CHANGED_REGISTER)
            {
                HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_RESET);
                plo_write_register(new_register_value1);
                plo_new_data=PLO_DATA_SENDED;
            }
            proccesing_command_1 = false;
        }
        if (proccesing_command_2 == true)
        {
            uint32_t new_register_value2 = usb_process_command(test_data, command_data_2);
            if (plo_new_data == PLO_INIT)
            {
                // toggle pin for trigger logic analyzer
                HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_RESET);
                plo_write_all(test_data, PLO_INIT);
                plo_write_all(test_data, PLO_INIT);
                plo_write_all(test_data, PLO_OUT_ENABLE);
                plo_new_data=PLO_DATA_SENDED;
            }
            else if (plo_new_data == PLO_CHANGED_REGISTER)
            {
                HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_RESET);
                plo_write_register(new_register_value2);
                plo_new_data=PLO_DATA_SENDED;
            }
            proccesing_command_2 = false;
        }
        if (proccesing_command_3 == true)
        {
            uint32_t new_register_value3 = usb_process_command(test_data, command_data_3);
            if (plo_new_data == PLO_INIT)
            {
                // toggle pin for trigger logic analyzer
                HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_RESET);
                plo_write_all(test_data, PLO_INIT);
                plo_write_all(test_data, PLO_INIT);
                plo_write_all(test_data, PLO_OUT_ENABLE);
                plo_new_data=PLO_DATA_SENDED;
            }
            else if (plo_new_data == PLO_CHANGED_REGISTER)
            {
                HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_RESET);
                plo_write_register(new_register_value3);
                plo_new_data=PLO_DATA_SENDED;
            }
            proccesing_command_3 = false;
        }
        if (proccesing_command_4 == true)
        {
            uint32_t new_register_value4 = usb_process_command(test_data, command_data_4);
            if (plo_new_data == PLO_INIT)
            {
                // toggle pin for trigger logic analyzer
                HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_RESET);
                plo_write_all(test_data, PLO_INIT);
                plo_write_all(test_data, PLO_INIT);
                plo_write_all(test_data, PLO_OUT_ENABLE);
                plo_new_data=PLO_DATA_SENDED;
            }
            else if (plo_new_data == PLO_CHANGED_REGISTER)
            {
                HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_RESET);
                plo_write_register(new_register_value4);
                plo_new_data=PLO_DATA_SENDED;
            }
            proccesing_command_4 = false;
        }
    }
    
    

    /*
    HAL_GPIO_WritePin(MUX_OUT_GPIO_Port, MUX_OUT_Pin, GPIO_PIN_RESET);
    HAL_Delay(250);
    HAL_GPIO_WritePin(MUX_OUT_GPIO_Port, MUX_OUT_Pin, GPIO_PIN_SET);
    HAL_Delay(250);*/
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
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