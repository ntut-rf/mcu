#include <stdint.h>
#include "sx127x.h"
#include "sx127x_reg.h"
#include "sx127x_common.h"
#include "sx127x_lora.h"

void SX127x_LoRa_TX_mode (void)
{
    SX127x_set_Mode(SX127x_MODE_STDBY);
    SX127x_set_DioMapping(0, 0b01);
    SX127x_write(SX127x_LoRa_FifoRxBaseAddr, 0xFF);
    SX127x_write(SX127x_LoRa_FifoTxBaseAddr, 0x00);
    SX127x_write(SX127x_LoRa_FifoAddrPtr   , 0x00);
    SX127x_write(SX127x_LoRa_IrqFlagsMask  , 0xF7);     // TxDone
}

void SX127x_LoRa_RX_mode (void)
{
    SX127x_set_Mode(SX127x_MODE_STDBY);
    SX127x_set_DioMapping(0, 0b00);
    SX127x_write(SX127x_LoRa_FifoRxBaseAddr, 0x00);
    SX127x_write(SX127x_LoRa_FifoTxBaseAddr, 0xFF);
    SX127x_write(SX127x_LoRa_FifoAddrPtr   , 0x00);
    SX127x_write(SX127x_LoRa_IrqFlagsMask  , 0xBF);     // RxDone
    SX127x_set_Mode(SX127x_MODE_RXCONT); // Continuous RX mode
}

void SX127x_LoRa_set_SpreadingFactor (int spreadingFactor)
{
    int mode = SX127x_get_Mode();
    if (mode != SX127x_MODE_STDBY) SX127x_set_Mode(SX127x_MODE_STDBY);
    if (spreadingFactor > 12)   spreadingFactor = 12;
    if (spreadingFactor < 6)    spreadingFactor = 6;
    SX127x_writef(SX127x_LoRa_ModemConfig2, 0xF0, spreadingFactor);
    if (spreadingFactor == 0x06)
    {
        SX127x_write(SX127x_LoRa_DetectOptimize,     0x05);
        SX127x_write(SX127x_LoRa_DetectionThreshold, 0x0C);
    }
    else
    {
        SX127x_write(SX127x_LoRa_DetectOptimize,     0x03);
        SX127x_write(SX127x_LoRa_DetectionThreshold, 0x0A);
    }
    if (mode != SX127x_MODE_STDBY) SX127x_set_Mode(mode);
}

void SX127x_LoRa_set_Power (int power)
{
    int mode = SX127x_get_Mode();
    if (mode != SX127x_MODE_STDBY) SX127x_set_Mode(SX127x_MODE_STDBY);
    if (power < 2) power = 2;
    if (power > 17) power = 17;
    SX127x_writef(SX127x_PaConfig, 0x0F, power - 2);
    if (mode != SX127x_MODE_STDBY) SX127x_set_Mode(mode);
}

void SX127x_LoRa_set_PayloadLength (int payloadLength)
{
    int mode = SX127x_get_Mode();
    if (mode != SX127x_MODE_STDBY) SX127x_set_Mode(SX127x_MODE_STDBY);
    if (payloadLength < 1)      payloadLength = 1;
    if (payloadLength > 255)    payloadLength = 255;
    SX127x_write(SX127x_LoRa_PayloadLength, payloadLength);
    if (mode != SX127x_MODE_STDBY) SX127x_set_Mode(mode);
}

void SX127x_LoRa_set_CodingRate (int rate)
{
    SX127x_writef(SX127x_LoRa_ModemConfig, 0x0E, rate);
}

void SX127x_LoRa_set_Bandwidth (int bw)
{
    SX127x_writef(SX127x_LoRa_ModemConfig, 0xF0, bw);
}

void SX127x_LoRa_transmit (int length, char* buffer)
{
    SX127x_write(SX127x_LoRa_FifoAddrPtr, 0x00);
    for (int i = 0; i < length; i++) SX127x_write(SX127x_Fifo, buffer[i]);
    SX127x_set_Mode(SX127x_MODE_TX);
}

void SX127x_LoRa_read (int length, char* buffer)
{
    SX127x_write(SX127x_LoRa_FifoAddrPtr, SX127x_LoRa_FifoRxCurrentAddr);
    for (int i = 0; i < length; i++) buffer[i] = SX127x_read(SX127x_Fifo);
}

char SX127x_LoRa_flags_read (void)
{
    return SX127x_read(SX127x_LoRa_IrqFlags);
}

void SX127x_LoRa_flags_clear (void)
{
    SX127x_write(SX127x_LoRa_IrqFlags, 0xFF);
}

void SX127x_LoRa_init (void)
{
    SX127x_set_LoRaMode(1);
    SX127x_write(SX127x_PaConfig, 0xF0); // PA_BOOST pin
    SX127x_write(SX127x_PaOcp, 0x3F); // Overcurrent protection I_max = 240mA
    SX127x_writef(SX127x_LoRa_ModemConfig,  0x01, 1); // Implicit Header mode
    SX127x_writef(SX127x_LoRa_ModemConfig2, 0x04, 1); // RX payload CRC on

    /* Default settings */
    SX127x_LoRa_set_CodingRate(0b001);
    SX127x_LoRa_set_Bandwidth(0b1001);
    SX127x_LoRa_set_Power(17);
    SX127x_LoRa_set_SpreadingFactor(6);
    SX127x_LoRa_set_PayloadLength(255);
}

void SX127x_LoRa_fifo_reset (void)
{
    SX127x_write(SX127x_LoRa_FifoAddrPtr, 0x00);
}

void SX127x_LoRa_fifo_write (char c)
{
    SX127x_write(SX127x_Fifo, c);
}

void SX127x_LoRa_fifo_transmit (void)
{
    SX127x_set_Mode(SX127x_MODE_TX);
}
