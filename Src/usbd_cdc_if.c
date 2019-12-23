/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @version        : v2.0_Cube
  * @brief          : Usb device for Virtual Com Port.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"

/* USER CODE BEGIN INCLUDE */
#include "max2871.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t buffer[7];

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
/* Define size for the receive and transmit buffer over CDC */
/* It's up to user to redefine and/or remove those define */
#define APP_RX_DATA_SIZE    500
#define APP_TX_DATA_SIZE    500
#define CMD_BUFFER_LEN      30
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/** Received data over USB are stored in this buffer      */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
static void usb_data_avaible(uint8_t c);
void usb_process_command(char *data, uint8_t len);
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the CDC media low layer over the FS USB IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_FS(void)
{
  /* USER CODE BEGIN 3 */
  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
  switch(cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:

    break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

    break;

    case CDC_SET_COMM_FEATURE:

    break;

    case CDC_GET_COMM_FEATURE:

    break;

    case CDC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
    case CDC_SET_LINE_CODING:
        buffer[0] = pbuf[0];
        buffer[1] = pbuf[1];
        buffer[2] = pbuf[2];
        buffer[3] = pbuf[3];
        buffer[4] = pbuf[4];
        buffer[5] = pbuf[5];
        buffer[6] = pbuf[6];
    break;

    case CDC_GET_LINE_CODING:
        pbuf[0] = buffer[0];
        pbuf[1] = buffer[1];
        pbuf[2] = buffer[2];
        pbuf[3] = buffer[3];
        pbuf[4] = buffer[4];
        pbuf[5] = buffer[5];
        pbuf[6] = buffer[6];
    break;

    case CDC_SET_CONTROL_LINE_STATE:

    break;

    case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will block any OUT packet reception on USB endpoint
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result
  *         in receiving more data while previous ones are still not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */
    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);
    //usb_data_avaible(Buf[0]);    
    for (uint8_t i = 0; i < *Len; i++)
    {
        usb_data_avaible(Buf[i]);
    }

    return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  CDC_Transmit_FS
  *         Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  *         @note
  *
  *
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  if (hcdc->TxState != 0){
    return USBD_BUSY;
  }
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  /* USER CODE END 7 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
static void usb_data_avaible(uint8_t c)
{
    static uint16_t cnt1;
    static uint16_t cnt2;
    static uint16_t cnt3;
    static uint16_t cnt4;
    // prvni buffer je volny, pis do nej
    if (proccesing_command_1 == false)
    {
        // else indikuje chybu, ze doslo k preteceni vstupniho bufferu, nahodi ext ref signal input
        if (cnt1 < CMD_BUFFER_LEN) 
            command_data_1[cnt1++] = c; 
        else 
            HAL_GPIO_WritePin(INT_EXT_REF_GPIO_Port, INT_EXT_REF_Pin, GPIO_PIN_SET); 
        // over, jestli uz neprisel ukoncovaci znak, tedy mam cely command
        if (c == '\n' || c == '\r')
        {
            proccesing_command_1 = true;
            cnt1 = 0;
            plo_new_data = PLO_NEW_DATA;
            return;
        }
    }
    // kdyz je prvni zpracovavan, pis do druheho
    else if (proccesing_command_2 == false)
    {
        // else indikuje chybu, ze doslo k preteceni vstupniho bufferu, nahodi ext ref signal input
        if (cnt2 < CMD_BUFFER_LEN) 
            command_data_2[cnt2++] = c; 
        else 
            HAL_GPIO_WritePin(INT_EXT_REF_GPIO_Port, INT_EXT_REF_Pin, GPIO_PIN_SET);
        // over, jestli uz neprisel ukoncovaci znak, tedy mam cely command
        if (c == '\n' || c == '\r')
        {
            proccesing_command_2 = true;
            cnt2 = 0;
            plo_new_data = PLO_NEW_DATA;
            return;
        }
    }
    // treti buffer pro pripad, ze ani druhý se nestihl vycist
    else if (proccesing_command_3 == false)
    {
        // else indikuje chybu, ze doslo k preteceni vstupniho bufferu, nahodi ext ref signal input
        if (cnt3 < CMD_BUFFER_LEN) 
            command_data_3[cnt3++] = c; 
        else 
            HAL_GPIO_WritePin(INT_EXT_REF_GPIO_Port, INT_EXT_REF_Pin, GPIO_PIN_SET);
        // over, jestli uz neprisel ukoncovaci znak, tedy mam cely command
        if (c == '\n' || c == '\r')
        {
            proccesing_command_3 = true;
            cnt3 = 0;
            plo_new_data = PLO_NEW_DATA;
            return;
        }
    }
    // ctvrty buffer 
    else if (proccesing_command_4 == false)
    {
        // else indikuje chybu, ze doslo k preteceni vstupniho bufferu, nahodi ext ref signal input
        if (cnt4 < CMD_BUFFER_LEN) 
            command_data_4[cnt4++] = c; 
        else 
            HAL_GPIO_WritePin(INT_EXT_REF_GPIO_Port, INT_EXT_REF_Pin, GPIO_PIN_SET);
        // over, jestli uz neprisel ukoncovaci znak, tedy mam cely command
        if (c == '\n' || c == '\r')
        {
            proccesing_command_4 = true;
            cnt4 = 0;
            plo_new_data = PLO_NEW_DATA;
            return;
        }
    }
    else
    // kdyz je i ten plny, nahod chybu nastavenim externi signal reference
    {
        HAL_GPIO_WritePin(INT_EXT_REF_GPIO_Port, INT_EXT_REF_Pin, GPIO_PIN_SET);
    }
}


int _write(int file, char const *buf, int n)
{
    /* stdout redirection to USB */
    CDC_Transmit_FS((char*)(buf), n);
    return n;
}
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
