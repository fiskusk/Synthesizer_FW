/**
  * @file       format.h
  * @author     Filip Klapil (klapil.filip@seznam.cz)
  * @brief      Header file for formating functions
  * @version    0.1
  * @date       2020-01-18
  * 
  * @copyright  Copyright (c) 2020
  * 
  */

#ifndef _FORMAT_H
#define _FORMAT_H

#include "stdio.h"

extern uint32_t hex2int(char *hex);
extern uint32_t lsb_to_msb_bit_reversal(uint32_t input);

#endif