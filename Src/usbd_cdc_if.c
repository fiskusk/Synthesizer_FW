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
#include "flash.h"
#include "main.h"
#include "stdio.h"
#include "stdint.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define CMD_BUFFER_LEN        96        // Size of each buffer in bytes
#define CMD_BUFFER_CNT        2        // Total count of buffer

typedef struct
{
    uint8_t length;
    char buffer[CMD_BUFFER_LEN];
    bool received;
} cmd_buffer_t;


/* Private variables ---------------------------------------------------------*/
cmd_buffer_t cmd_buffer[CMD_BUFFER_CNT];

uint8_t buffer[7];
uint32_t test_data[6] = {DEF_TEST_DATA_REG0, DEF_TEST_DATA_REG1,
                         DEF_TEST_DATA_REG2, DEF_TEST_DATA_REG3,
                         DEF_TEST_DATA_REG4, DEF_TEST_DATA_REG5};
host_com_port_open_closed_t host_com_port_open_closed;
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
#define APP_RX_DATA_SIZE    64
#define APP_TX_DATA_SIZE    64
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
static void usb_data_available(uint8_t c);
uint32_t usb_process_command(char *command_data);
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
  USBD_SetupReqTypedef * req;
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
        req = (USBD_SetupReqTypedef *)pbuf;
        if((req->wValue & 0x0001) != 0)
        {
            host_com_port_open_closed = HOST_COM_PORT_OPEN;
        }
        else
        {
            apply_memory_select_changed();
            host_com_port_open_closed = HOST_COM_PORT_CLOSED;
        }
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
    
    for (uint8_t i = 0; i < *Len; i++)
    {
        usb_data_available(Buf[i]);
    }

    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);

    return (USBD_OK);
  /* USER CODE END 6 */
}

int _write(int file, char const *buf, int n)
{
    /* stdout redirection to USB */
    /*
    uint8_t sent    = 0;
    uint8_t try_cnt = 0;

    while(!sent)
    {
        if(((USBD_CDC_HandleTypeDef*)(hUsbDeviceFS.pClassData))->TxState == 0)
        {
            if(CDC_Transmit_FS((uint8_t*)(buf), n) == USBD_OK)
            {
                sent = 1;
            }
        }
        else if(try_cnt > 9)
        {
            sent = 1;
        }

        try_cnt++;
        if(sent == 0)
        {
            HAL_Delay(1);
        }
    }*/
    //if(((USBD_CDC_HandleTypeDef*)(hUsbDeviceFS.pClassData))->TxState==0){
    //    CDC_Transmit_FS((uint8_t*)(buf), n);
    //}
    
    while (((USBD_CDC_HandleTypeDef*)(hUsbDeviceFS.pClassData))->TxState != 0){}
    CDC_Transmit_FS((uint8_t*)(buf), n);
    return n;
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
  //uint64_t counter = 0;
  //USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  //if (hcdc->TxState != 0){
  //  return USBD_BUSY;
  //}
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  //while (hcdc->TxState != 0){
  //  counter++;
  //}
  /* USER CODE END 7 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
/****************************************************/
/****************************************************/
/****************************************************/
static void usb_data_available(uint8_t c)
{
    static uint8_t active_buff = 0;
    uint8_t *pos = &cmd_buffer[active_buff].length;

    if (cmd_buffer[active_buff].received)
        return;                                         // Buffer not free, cannot receive data...

    if (c == '\n' || c == '\r')
    {
        cmd_buffer[active_buff].buffer[*pos] = 0;       // Add ending zero
        cmd_buffer[active_buff].received = 1;           // Mark data in buffer as received
        if (++active_buff >= CMD_BUFFER_CNT)            // Switch to next buffer
            active_buff = 0;

    }
    else
    {
        if (*pos < (CMD_BUFFER_LEN - 1))                // 1 Byte on the end is reserved for zero
        {
            cmd_buffer[active_buff].buffer[*pos] = c;   // Save character to buffer
            *pos = *pos + 1;
        }
        else
        {
            //TODO: No more space in buffer, cannot store data. What to do with it?
            *pos = *pos;          // Useless, just for filling in the "else" branch
        }
    }
}

uint32_t usb_process_command(char *command_data)
{
    char *token;
    char *sub_token;
    char *value;
    char *value0;
    char *value1;
    char *value2;
    char *value3;
    char *value4;
    char *value5;
    char *value6;

    for (uint8_t i = 0; i < strlen(command_data); i++)
    {
        command_data[i] = (command_data[i] < 32 || command_data[i] > 126) ? '\0' : command_data[i];
    }

    token = strtok(command_data, " ");

    if (strcasecmp(token, "ref") == 0)
    {
        value = strtok(NULL, " ");
        if (strcasecmp(value, "ext") == 0) {
            PLO_MODULE_EXT_REF;
        }

        else if (strcasecmp(value, "int") == 0) {
            PLO_MODULE_INT_REF;
        }
        printf("OK\r");
        plo_new_data=PLO_DATA_SENDED;
    }

    else if (strcasecmp(token, "out") == 0)
    {
        sub_token = strtok(NULL, " ");
        value = strtok(NULL, " ");
        if (strcasecmp(sub_token, "1") == 0) {
            if (strcasecmp(value, "on") == 0)
                PLO_MODULE_OUT1_ON;
            else if (strcasecmp(value, "off") == 0)
                PLO_MODULE_OUT1_OFF;
        }
        else if (strcasecmp(sub_token, "2") == 0) {
            if (strcasecmp(value, "on") == 0)
                PLO_MODULE_OUT2_ON;
            else if (strcasecmp(value, "off") == 0)
                PLO_MODULE_OUT2_OFF;
        }
        printf("OK\r");
        plo_new_data=PLO_DATA_SENDED;
    }
    
    else if (strcasecmp(token, "plo") == 0)
    {
        sub_token = strtok(NULL, " ");
        value = strtok(NULL, " ");
        value0 =strtok(NULL, " ");
        value1 = strtok(NULL, " ");
        value2 = strtok(NULL, " ");
        value3 = strtok(NULL, " ");
        value4 = strtok(NULL, " ");
        value5 = strtok(NULL, " ");
        value6 = strtok(NULL, " ");

        if (strcasecmp(sub_token, "init") == 0)
        {
            plo_new_data = PLO_INIT;
        }

        else if (strcasecmp(sub_token, "set_register") == 0)
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

        else if (strcasecmp(sub_token, "data") == 0)
        {
            if (strcasecmp(value, "clean") == 0)
            {
                myFLASH_PageErase(0x08007800);
            }
            else if (strcasecmp(value, "1") == 0)
            {
                write_complete_data_to_flash(1, value0, value1, value2, value3, value4, value5, value6);
            }
            else if (strcasecmp(value, "2") == 0)
            {
                write_complete_data_to_flash(2, value0, value1, value2, value3, value4, value5, value6);
            }
            else if (strcasecmp(value, "3") == 0)
            {
                write_complete_data_to_flash(3, value0, value1, value2, value3, value4, value5, value6);
            }
            else if (strcasecmp(value, "4") == 0)
            {
                write_complete_data_to_flash(4, value0, value1, value2, value3, value4, value5, value6);
            }
            printf("OK\r");
            plo_new_data=PLO_DATA_SENDED;
        }
        else if (strcasecmp(sub_token, "locked?") == 0)
        {
            plo_check_lock_status();
            printf("OK\r");
            plo_new_data=PLO_DATA_SENDED;
        }
        else if (strcasecmp(sub_token, "storedData") == 0)
        {
            printf("OK\r");
            flash_send_stored_data();
            plo_new_data=PLO_DATA_SENDED;
        }
    }
    return 0;
}

void procesing_command_data()
{
    static uint8_t active_buff = 0;

    while (cmd_buffer[active_buff].received)        // Check if data in buffer was received
    {
        uint32_t new_register_value = usb_process_command(cmd_buffer[active_buff].buffer);        // Process command
        if (plo_new_data == PLO_INIT)
        {
            // toggle pin for trigger logic analyzer
            HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_RESET);
            plo_write_all(test_data, PLO_INIT);
            plo_write_all(test_data, PLO_INIT);
            plo_write_all(test_data, PLO_OUT_ENABLE);
            printf("OK\r");
            plo_new_data=PLO_DATA_SENDED;
        }
        else if (plo_new_data == PLO_CHANGED_REGISTER)
        {
            HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_RESET);
            plo_write_register(new_register_value);
            printf("OK\r");
            plo_new_data=PLO_DATA_SENDED;
        }

        cmd_buffer[active_buff].length = 0;            // Data in buffer processed, clear the length and
        cmd_buffer[active_buff].received = 0;        // mark it as free

        if (++active_buff >= CMD_BUFFER_CNT)        // Switch to next buffer
            active_buff = 0;
    }
}

void process_lock_status(bool data)
{
    if (plo_lock_state != PLO_LOCK_STATE_UNKNOWN)
    {
        if (data)
        {   
            printf("plo locked\r");
        }
        else
        {
            printf("plo isn't locked\r");
        }
    }
    else
    {
        printf("plo state is not known\r");
    }
    plo_lock_state = PLO_LOCK_STATE_WAIT;

}

void flash_send_stored_data(void)
{
    printf( "stored_data_1 %08x %08x %08x %08x %08x %08x %08x\r", 
        (unsigned int)(saved_data_1[0]), (unsigned int)(saved_data_1[1]),
        (unsigned int)(saved_data_1[2]), (unsigned int)(saved_data_1[3]), 
        (unsigned int)(saved_data_1[4]), (unsigned int)(saved_data_1[5]),
        (unsigned int)(saved_data_1[6]));
    printf("stored_data_2 %08x %08x %08x %08x %08x %08x %08x\r", 
        (unsigned int)(saved_data_2[0]), (unsigned int)(saved_data_2[1]),
        (unsigned int)(saved_data_2[2]), (unsigned int)(saved_data_2[3]),
        (unsigned int)(saved_data_2[4]), (unsigned int)(saved_data_2[5]),
        (unsigned int)(saved_data_2[6]));
    printf("stored_data_3 %08x %08x %08x %08x %08x %08x %08x\r", 
        (unsigned int)(saved_data_3[0]), (unsigned int)(saved_data_3[1]),
        (unsigned int)(saved_data_3[2]), (unsigned int)(saved_data_3[3]),
        (unsigned int)(saved_data_3[4]), (unsigned int)(saved_data_3[5]),
        (unsigned int)(saved_data_3[6]));
    printf("stored_data_4 %08x %08x %08x %08x %08x %08x %08x\r",
        (unsigned int)(saved_data_4[0]), (unsigned int)(saved_data_4[1]),
        (unsigned int)(saved_data_4[2]), (unsigned int)(saved_data_4[3]),
        (unsigned int)(saved_data_4[4]), (unsigned int)(saved_data_4[5]),
        (unsigned int)(saved_data_4[6]));
}
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
