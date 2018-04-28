#include <stdint.h>
#include "sx127x.h"
#include "sx127x_reg.h"
#include "sx127x_common.h"

void SX127x_set_Mode (int mode)
{
    SX127x_writef(SX127x_OpMode, SX127x_OpMode_Mode, mode);
}

uint8_t SX127x_get_Mode (void)
{
    return SX127x_readf(SX127x_OpMode, SX127x_OpMode_Mode);
}

void SX127x_set_LoRaMode (int loraMode)
{
    SX127x_set_Mode(SX127x_MODE_SLEEP);
    SX127x_writef(SX127x_OpMode, SX127x_OpMode_LoRaMode, loraMode);
    SX127x_set_Mode(SX127x_MODE_STDBY);
}

uint8_t SX127x_get_LoRaMode (void)
{
    return SX127x_readf(SX127x_OpMode, SX127x_OpMode_LoRaMode);
}

void SX127x_set_DioMapping (char pin, char mapping)
{
    int loraMode = SX127x_get_LoRaMode();
    if (loraMode) SX127x_set_LoRaMode(0);
    switch (pin)
    {
        case 0: SX127x_writef(SX127x_DioMapping1, 0xC0, mapping & 0b11); break;
        case 1: SX127x_writef(SX127x_DioMapping1, 0x30, mapping & 0b11); break;
        case 2: SX127x_writef(SX127x_DioMapping1, 0x0C, mapping & 0b11); break;
        case 3: SX127x_writef(SX127x_DioMapping1, 0x03, mapping & 0b11); break;
        case 4: SX127x_writef(SX127x_DioMapping2, 0xC0, mapping & 0b11); break;
        case 5: SX127x_writef(SX127x_DioMapping2, 0x30, mapping & 0b11); break;
    }
    if (loraMode) SX127x_set_LoRaMode(1);
}

void SX127x_set_PaDac (int on)
{
    SX127x_writef(SX127x_PaDac, 0x07, on ? 0x7 : 0x4);
}
