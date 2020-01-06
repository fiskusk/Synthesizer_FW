/*************************************************************************
 *
 * $Id: ad9851.h 179 2007-09-16 15:02:06Z alpov $
 * 
 *************************************************************************/         

#ifndef _MAX2871_H
#define _MAX2871_H

#include <stdint.h>


typedef enum{
    PLO_NEW_DATA,
    PLO_INIT,
    PLO_OUT_ENABLE,
    PLO_CHANGED_REGISTER,
    PLO_DATA_SENDED
}plo_new_data_t;

extern volatile plo_new_data_t plo_new_data;

extern void plo_write_all(uint32_t *max2871, plo_new_data_t plo_write_type);
extern void plo_write_register(uint32_t register_data);

#endif