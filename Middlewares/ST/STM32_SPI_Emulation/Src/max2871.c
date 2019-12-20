/*************************************************************************
 *
 * communication with AD9851 DDS
 * 
 * (c) Ales Povalac, alpov@alpov.net, http://www.alpov.net/
 * 
 *************************************************************************
 *
 * $Id: ad9851.c 186 2007-10-18 14:15:12Z alpov $
 * 
 *************************************************************************/         

#include "max2871.h"
#include "gpio.h"

volatile plo_new_data_t plo_new_data = PLO_DATA_SENDED;
// multiplier constant, 2^64 / 180e6
// const unsigned long long dds_multiplier EEMEM = 102481911521;
//const unsigned long long dds_multiplier EEMEM =    102482050000;


/*************************************************************************
void dds_init(void)
{
    DDS_DDR |= _BV(DDS_D7) | _BV(DDS_W_CLK) | _BV(DDS_FQ_UD) | _BV(DDS_RESET);
    
    // generate master reset
    DDS_PORT |= _BV(DDS_RESET);
    DDS_PORT &= ~_BV(DDS_RESET);
    
    // generate update pulse
    DDS_PORT |= _BV(DDS_FQ_UD);
    DDS_PORT &= ~_BV(DDS_FQ_UD);
    
    // clear internal registers
    dds_write(0);   
}*/
uint32_t lsb_to_msb_bit_reversal(uint32_t input)
{
    input = (((input & 0xaaaaaaaa) >> 1) | ((input & 0x55555555) << 1));
    input = (((input & 0xcccccccc) >> 2) | ((input & 0x33333333) << 2));
    input = (((input & 0xf0f0f0f0) >> 4) | ((input & 0x0f0f0f0f) << 4));
    input = (((input & 0xff00ff00) >> 8) | ((input & 0x00ff00ff) << 8));
    return ((input >> 16) | (input << 16));
}

/*************************************************************************/
void plo_write(unsigned int *max2871, plo_new_data_t plo_write_type)
{
    for (uint8_t i = 0; i < 6; i++) {
        if (plo_write_type == PLO_OUT_ENABLE)
        {
            i=1;
        }

        uint32_t c = max2871[i];
        if ((plo_write_type == PLO_INIT) && (i==1))
        {
            c &= ~((1UL << 5) | (1UL << 9 ));
        }
        c = lsb_to_msb_bit_reversal(c);

            

        for (uint8_t j = 0; j < 32; j++) {
            if (c & 0x01)
                HAL_GPIO_WritePin(PLO_DATA_GPIO_Port, PLO_DATA_Pin, GPIO_PIN_SET);
            else
                HAL_GPIO_WritePin(PLO_DATA_GPIO_Port, PLO_DATA_Pin, GPIO_PIN_RESET);
            
            // generate clock pulse and shift next
            HAL_GPIO_WritePin(PLO_CLK_GPIO_Port, PLO_CLK_Pin, GPIO_PIN_SET);
            c >>= 1;
            HAL_GPIO_WritePin(PLO_CLK_GPIO_Port, PLO_CLK_Pin, GPIO_PIN_RESET);
        }
        // generate update pulse
        HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_RESET);
        if ((plo_write_type == PLO_INIT) && (i==0))
            HAL_Delay(20);

        if (plo_write_type == PLO_OUT_ENABLE)
            i=6;
    }  
}
    /*union {
        struct {
            unsigned long freq;
            unsigned char config;
        } regs;
        unsigned char max2871[6];
    } plo;
    
    plo.regs.freq = freq_reg;
    plo.regs.config = DDS_CONFIG;*/



/*************************************************************************
void dds_set_freq(unsigned long new_freq)
{
    unsigned long long mult;
    union {
        unsigned long long ll;
        struct {
            unsigned long low;
            unsigned long high;
        } sl;
    } cfg;
    
    eeprom_read_block(&mult, &dds_multiplier, sizeof(mult));
    
    cfg.sl.low = new_freq;
    cfg.sl.high = 0;
    
    cfg.ll *= mult;
    
    dds_write(cfg.sl.high);
}*/
