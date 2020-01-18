/**
  * @file       usb.h
  * @author     Filip Klapil (klapil.filip@seznam.cz)
  * @brief      Header file for processing of received data from USB VCP interface.
  * @version    0.1
  * @date       2020-01-18
  * 
  * @copyright  Copyright (c) 2020
  * 
  */

#ifndef _USB_H
#define _USB_H

#include "stdio.h"

/** @defgroup DEFAULT_TEST_DATA
  * @brief Define default test data for frequency synthesizer
  * @{
  */
#define DEF_TEST_DATA_REG0 0x80C80008   /**< Default register 0 test value  */
#define DEF_TEST_DATA_REG1 0x800003E9   /**< Default register 1 test value  */
#define DEF_TEST_DATA_REG2 0x18004142   /**< Default register 2 test value  */
#define DEF_TEST_DATA_REG3 0x00001F23   /**< Default register 3 test value  */
#define DEF_TEST_DATA_REG4 0x63BE80FC   /**< Default register 4 test value  */
#define DEF_TEST_DATA_REG5 0x00400005   /**< Default register 5 test value  */
/**
  * @}
  */

extern uint32_t test_data[6];

extern void usb_data_available(uint8_t c);
extern void usb_procesing_command_data(void);

#endif