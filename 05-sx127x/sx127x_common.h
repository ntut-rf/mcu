#ifndef SX127x_COMMON_H
#define SX127x_COMMON_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

void    SX127x_set_Mode (int mode);
uint8_t SX127x_get_Mode (void);
void    SX127x_set_LoRaMode (int loraMode);
uint8_t SX127x_get_LoRaMode (void);
void    SX127x_set_DioMapping (char pin, char mapping);
void    SX127x_set_PaDac (int on);
void    SX127x_set_Power (int power);
uint8_t SX127x_get_Power (void);
const char* SX127x_Mode_str (int mode);

#define    SX127x_MODE_SLEEP      0
#define    SX127x_MODE_STDBY      1
#define    SX127x_MODE_FSTX       2
#define    SX127x_MODE_TX         3
#define    SX127x_MODE_FSRX       4
#define    SX127x_MODE_RX         5
#define    SX127x_MODE_RXCONT     5
#define    SX127x_MODE_RXSINGLE   6
#define    SX127x_MODE_CAD        7

#ifdef __cplusplus
}
#endif

#endif
