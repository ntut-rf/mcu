#include <stdint.h>
#include <stdbool.h>
#include "sx127x.h"
#include "sx127x_reg.h"
#include "sx127x_common.h"

static uint8_t Power;

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
    while(SX127x_get_Mode() != SX127x_MODE_STDBY);
}

uint8_t SX127x_get_LoRaMode (void)
{
    return SX127x_readf(SX127x_OpMode, SX127x_OpMode_LoRaMode);
}

void SX127x_set_DioMapping (char pin, char mapping)
{
    int loraMode = SX127x_get_LoRaMode();
    if (loraMode) SX127x_set_LoRaMode(false);
    switch (pin)
    {
        case 0: SX127x_writef(SX127x_DioMapping1, 0xC0, mapping & 0b11); break;
        case 1: SX127x_writef(SX127x_DioMapping1, 0x30, mapping & 0b11); break;
        case 2: SX127x_writef(SX127x_DioMapping1, 0x0C, mapping & 0b11); break;
        case 3: SX127x_writef(SX127x_DioMapping1, 0x03, mapping & 0b11); break;
        case 4: SX127x_writef(SX127x_DioMapping2, 0xC0, mapping & 0b11); break;
        case 5: SX127x_writef(SX127x_DioMapping2, 0x30, mapping & 0b11); break;
    }
    if (loraMode) SX127x_set_LoRaMode(true);
}

void SX127x_set_PaDac (int on)
{
    SX127x_writef(SX127x_PaDac, 0x07, on ? 0x7 : 0x4);
}

void SX127x_set_Power (int power)
{
    int mode = SX127x_get_Mode();
    if (mode != SX127x_MODE_STDBY) SX127x_set_Mode(SX127x_MODE_STDBY);
    if (power < 2) power = 2;
    if (power > 17) power = 17;
    SX127x_writef(SX127x_PaConfig, 0x0F, power - 2);
    if (mode != SX127x_MODE_STDBY) SX127x_set_Mode(mode);
    Power = power;
}

uint8_t SX127x_get_Power (void)
{
    return Power;
}

const char* SX127x_Mode_str (int mode)
{
    switch (mode)
    {
        case SX127x_MODE_SLEEP    : return "SLEEP   ";
        case SX127x_MODE_STDBY    : return "STDBY   ";
        case SX127x_MODE_FSTX     : return "FSTX    ";
        case SX127x_MODE_TX       : return "TX      ";
        case SX127x_MODE_FSRX     : return "FSRX    ";
        case SX127x_MODE_RX       : return "RX      ";
        case SX127x_MODE_RXSINGLE : return "RXSINGLE";
        case SX127x_MODE_CAD      : return "CAD     ";
    }
    return "";
}
