#include "usb.h"
#include "string.h"
#include "format.h"
#include "max2871.h"
#include "flash.h"
#include "main.h"
#include "stdint.h"

#define CMD_BUFFER_LEN  96 // Size of each buffer in bytes
#define CMD_BUFFER_CNT  2  // Total count of buffer

typedef struct
{
    uint8_t length;
    char buffer[CMD_BUFFER_LEN];
    bool received;
} cmd_buffer_t;

cmd_buffer_t cmd_buffer[CMD_BUFFER_CNT];

uint32_t test_data[6] = {DEF_TEST_DATA_REG0, DEF_TEST_DATA_REG1,
                         DEF_TEST_DATA_REG2, DEF_TEST_DATA_REG3,
                         DEF_TEST_DATA_REG4, DEF_TEST_DATA_REG5};

void usb_data_available(uint8_t c)
{
    static uint8_t active_buff = 0;
    uint8_t *pos = &cmd_buffer[active_buff].length;

    if (cmd_buffer[active_buff].received)
        return; // Buffer not free, cannot receive data...

    if (c == '\n' || c == '\r')
    {
        cmd_buffer[active_buff].buffer[*pos] = 0; // Add ending zero
        cmd_buffer[active_buff].received = 1;     // Mark data in buffer as received
        if (++active_buff >= CMD_BUFFER_CNT)      // Switch to next buffer
            active_buff = 0;
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
            //TODO: No more space in buffer, cannot store data. What to do with it?
            *pos = *pos; // Useless, just for filling in the "else" branch
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
        if (strcasecmp(value, "ext") == 0)
        {
            PLO_MODULE_EXT_REF;
        }

        else if (strcasecmp(value, "int") == 0)
        {
            PLO_MODULE_INT_REF;
        }
        printf("OK\r");
        plo_new_data = PLO_DATA_SENDED;
    }

    else if (strcasecmp(token, "out") == 0)
    {
        sub_token = strtok(NULL, " ");
        value = strtok(NULL, " ");
        if (strcasecmp(sub_token, "1") == 0)
        {
            if (strcasecmp(value, "on") == 0)
                PLO_MODULE_OUT1_ON;
            else if (strcasecmp(value, "off") == 0)
                PLO_MODULE_OUT1_OFF;
        }
        else if (strcasecmp(sub_token, "2") == 0)
        {
            if (strcasecmp(value, "on") == 0)
                PLO_MODULE_OUT2_ON;
            else if (strcasecmp(value, "off") == 0)
                PLO_MODULE_OUT2_OFF;
        }
        printf("OK\r");
        plo_new_data = PLO_DATA_SENDED;
    }

    else if (strcasecmp(token, "plo") == 0)
    {
        sub_token = strtok(NULL, " ");
        value = strtok(NULL, " ");
        value0 = strtok(NULL, " ");
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
            switch (new_data & 0x07)
            {
                case 0:
                    test_data[0] = new_data;
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
            plo_new_data = PLO_DATA_SENDED;
        }
        else if (strcasecmp(sub_token, "locked?") == 0)
        {
            plo_check_lock_status();
            printf("OK\r");
            plo_new_data = PLO_DATA_SENDED;
        }
        else if (strcasecmp(sub_token, "storedData") == 0)
        {
            printf("OK\r");
            flash_send_stored_data();
            plo_new_data = PLO_DATA_SENDED;
        }
    }
    return 0;
}

void usb_procesing_command_data(void)
{
    static uint8_t active_buff = 0;

    while (cmd_buffer[active_buff].received) // Check if data in buffer was received
    {
        uint32_t new_register_value = usb_process_command(cmd_buffer[active_buff].buffer); // Process command
        if (plo_new_data == PLO_INIT)
        {
            plo_write(test_data, plo_new_data);
            printf("OK\r");
            plo_new_data = PLO_DATA_SENDED;
        }
        else if (plo_new_data == PLO_CHANGED_REGISTER)
        {
            plo_write_register(new_register_value);
            printf("OK\r");
            plo_new_data = PLO_DATA_SENDED;
        }

        cmd_buffer[active_buff].length = 0;   // Data in buffer processed, clear the length and
        cmd_buffer[active_buff].received = 0; // mark it as free

        if (++active_buff >= CMD_BUFFER_CNT) // Switch to next buffer
            active_buff = 0;
    }

    if (plo_lock_state == PLO_LOCK_STATE_UNKNOWN)
    {
        printf("plo state is not known\r");
        plo_lock_state = PLO_LOCK_STATE_WAIT;
    }
}