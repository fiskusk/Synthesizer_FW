/**
  * @file       usb.c
  * @author     Filip Klapil (klapil.filip@seznam.cz)
  * @brief      It provides processing of received data from USB VCP interface.
  * @version    0.1
  * @date       2020-01-18
  *
  * @copyright  Copyright (c) 2020
  * 
  */

#include "usb.h"
#include "string.h"
#include "format.h"
#include "max2871.h"
#include "flash.h"
#include "main.h"
#include "stdint.h"

/** @defgroup   Command_buffer_defines
  * @brief      Defines for receive buffers
  * @{
  */
#define CMD_BUFFER_LEN  96      /**< Size of each buffer in bytes */
#define CMD_BUFFER_CNT  2       /**< Total count of buffer        */
/**
  * @}
  */

/** 
  * @brief Struct for receive process buffers 
  */
typedef struct
{
    uint8_t length;                 /**< Length of process data             */
    char buffer[CMD_BUFFER_LEN];    /**< Process buffer                     */
    bool received;                  /**< Flag if buffer is ready to process */
} cmd_buffer_t;

cmd_buffer_t cmd_buffer[CMD_BUFFER_CNT]; /**< Command process buffer        */

/**< Stored registers data from USB commands */
uint32_t test_data[6] = {DEF_TEST_DATA_REG0, DEF_TEST_DATA_REG1,
                         DEF_TEST_DATA_REG2, DEF_TEST_DATA_REG3,
                         DEF_TEST_DATA_REG4, DEF_TEST_DATA_REG5};

/**
  * @brief   This function is called when receive buffer is full. The data is
  *          loaded character by character into process buffers. When all buffers 
  *          are full, the function return back. The data is recorded until the 
  *          '\n' or '\r' character arrives. At this point the received flag is 
  *          set and is switched to the next buffer.
  * @param   c: input charater
  */
void usb_data_available(uint8_t c)
{
    static uint8_t active_buff = 0;     /**< Handle actual active buffer    */
    uint8_t *pos = &cmd_buffer[active_buff].length;

    if (cmd_buffer[active_buff].received)
        return; // Buffer not free, cannot receive data...

    if (c == '\n' || c == '\r')
    {
        cmd_buffer[active_buff].buffer[*pos] = 0; // Add ending zero
        cmd_buffer[active_buff].received = 1;     // Mark data in buffer as received
        if (++active_buff >= CMD_BUFFER_CNT)      // Switch to next buffer
            active_buff = 0;                      // No more buffers, switch to first.
    }
    else
    {
        if (*pos < (CMD_BUFFER_LEN - 1)) // 1 Byte on the end is reserved for zero
        {
            cmd_buffer[active_buff].buffer[*pos] = c; // Save character to buffer
            *pos = *pos + 1;
        }
        else
        {
            //TODO: No more space in buffer, cannot store data.
            *pos = *pos; // Useless, just for filling in the "else" branch
        }
    }
}

/**
  * @brief   This function processes raw received data.
  * @param   command_data: Text string with received command.
  * @retval  Parsed 32-bits register value.
  */
void usb_process_command(char *command_data)
{
    char *command;      /**< Recieved command               */
    char *sub_command;  /**< Sub-command                    */
    char *value;        /**< Carry command action value     */
    char *register0;       /**< Carry register 0 value         */
    char *register1;       /**< Carry register 1 value         */
    char *register2;       /**< Carry register 2 value         */
    char *register3;       /**< Carry register 3 value         */
    char *register4;       /**< Carry register 4 value         */
    char *register5;       /**< Carry register 5 value         */
    char *module_control;       /**< Carry module controls value    */

    /** Remove all possible command characters from text command string 
      * and replace them by NULL
      */
    for (uint8_t i = 0; i < strlen(command_data); i++)
    {
        command_data[i] = (command_data[i] < 32 || command_data[i] > 126) ? '\0' : command_data[i];
    }
    
    command = strtok(command_data, " ");    // Command part parse 
    // reference selection part
    if (strcasecmp(command, "ref") == 0)
    {
        value = strtok(NULL, " ");          // Action part parse
        if (strcasecmp(value, "ext") == 0)
        {
            PLO_MODULE_EXT_REF;
        }

        else if (strcasecmp(value, "int") == 0)
        {
            PLO_MODULE_INT_REF;
        }
        printf("OK\r");                     // Sends a confirmation text string
    }
    // outputs selection part
    else if (strcasecmp(command, "out") == 0)
    {
        sub_command = strtok(NULL, " ");
        value = strtok(NULL, " ");
        if (strcasecmp(sub_command, "1") == 0)
        {
            if (strcasecmp(value, "on") == 0)
                PLO_MODULE_OUT1_ON;
            else if (strcasecmp(value, "off") == 0)
                PLO_MODULE_OUT1_OFF;
        }
        else if (strcasecmp(sub_command, "2") == 0)
        {
            if (strcasecmp(value, "on") == 0)
                PLO_MODULE_OUT2_ON;
            else if (strcasecmp(value, "off") == 0)
                PLO_MODULE_OUT2_OFF;
        }
        printf("OK\r");                     // Sends a confirmation text string
    }
    // plo commands section
    else if (strcasecmp(command, "plo") == 0)
    {
        sub_command = strtok(NULL, " ");
        value = strtok(NULL, " ");
        // initialize PLO
        if (strcasecmp(sub_command, "init") == 0)
        {
            // sends test_data as initialization sequence to plo
            plo_write(test_data, PLO_INIT);  
            printf("OK\r");     // Sends a confirmation text string
        }
        // get new register value for PLO
        else if (strcasecmp(sub_command, "set_register") == 0)
        {
            // converted received register string into 32bit number
            uint32_t new_data = hex2int(value);
            // store received register into test_data variable
            switch (new_data & 0x07)
            {
                case 0:
                    test_data[0] = new_data;
                    // if PLO muxout pin hasn't set digital lock function
                    // set plo_lock_state to handle unknown lock state
                    if (((test_data[2] & ((1 << 28) | (1 << 27) | (1 << 26))) >> 26) != 0b110)
                        plo_lock_state = PLO_LOCK_STATE_UNKNOWN;
                    break;
                case 1:
                    test_data[1] = new_data;
                    break;
                case 2:
                    test_data[2] = new_data;
                    break;
                case 3:
                    test_data[3] = new_data;
                    break;
                case 4:
                    test_data[4] = new_data;
                    break;
                case 5:
                    test_data[5] = new_data;
                    break;
            }
            plo_write_register(new_data);   // Send new register value into PLO
            printf("OK\r");                 // Sends a confirmation text string
            if (plo_lock_state == PLO_LOCK_STATE_UNKNOWN)
            {
                printf("plo state is not known\r");
                plo_lock_state = PLO_LOCK_STATE_WAIT;
            }
        }
        // section related user stored data in flash memory 
        else if (strcasecmp(sub_command, "data") == 0)
        {
            register0 = strtok(NULL, " ");
            register1 = strtok(NULL, " ");
            register2 = strtok(NULL, " ");
            register3 = strtok(NULL, " ");
            register4 = strtok(NULL, " ");
            register5 = strtok(NULL, " ");
            module_control = strtok(NULL, " ");
            // User data page erase
            if (strcasecmp(value, "clean") == 0)
            {
                myFLASH_PageErase(USER_DATA_ADDRESS_SECTION);
            }
            // User data send back to serial port
            else if (strcasecmp(value, "stored?") == 0)
            {
                printf("OK\r");             // Send confirmation string
                flash_send_stored_data();   // Send back stored user reg data
            }
            // section for store new user data
            else if (strcasecmp(value, "1") == 0)   // store into memory 1
            {
                write_complete_data_to_flash(1, register0, register1, register2, register3, register4, register5, module_control);
            }
            else if (strcasecmp(value, "2") == 0)   // store into memory 2
            {
                write_complete_data_to_flash(2, register0, register1, register2, register3, register4, register5, module_control);
            }
            else if (strcasecmp(value, "3") == 0)   // store into memory 3
            {
                write_complete_data_to_flash(3, register0, register1, register2, register3, register4, register5, module_control);
            }
            else if (strcasecmp(value, "4") == 0)   // store into memory 4
            {
                write_complete_data_to_flash(4, register0, register1, register2, register3, register4, register5, module_control);
            }
            printf("OK\r");                     // Send confirmation string
        }
        // request to check suspension status
        else if (strcasecmp(sub_command, "locked?") == 0)
        {
            plo_check_lock_status();
            printf("OK\r");                     // Send confirmation string
        }
        // received string not recognized
        else
        {
            printf("unknown command!\r");
        }
    }
}

/**
  * @brief  This function going through received buffer if available and procces
  *         received data.
  */
void usb_procesing_command_data(void)
{
    static uint8_t active_buff = 0;

    while (cmd_buffer[active_buff].received) // Check if data in buffer was received
    {
        // Process command and get new register value if it is possible, otherwise get 0
        usb_process_command(cmd_buffer[active_buff].buffer);

        cmd_buffer[active_buff].length = 0;   // Data in buffer processed, clear the length and
        cmd_buffer[active_buff].received = 0; // mark it as free

        if (++active_buff >= CMD_BUFFER_CNT) // Switch to next buffer
            active_buff = 0;
    }
}