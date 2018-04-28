#include <stdint.h>
#include <stdbool.h>
#include "sx127x.h"
#include "sx127x_reg.h"
#include "sx127x_common.h"
#include "sx127x_fsk.h"

#define SX127x_FXOSC 32000000
#define SX127x_FSTEP 61

static uint32_t FSK_Bitrate;
static uint32_t FSK_Deviation;
static uint32_t FSK_PayloadLength;

void SX127x_FSK_init (void)
{
    SX127x_set_LoRaMode(false);
    SX127x_FSK_set_Bitrate(250000);
    SX127x_FSK_set_Deviation(62500);
    SX127x_writef(SX127x_FSK_PacketConfig2, 1 << 6, 1); // Packet mode
    SX127x_writef(SX127x_FSK_PacketConfig1, 0x80, 0); // Fixed length packet
    SX127x_FSK_set_PayloadLength(64);
    SX127x_FSK_set_FifoThreshold(64);

    SX127x_write(SX127x_FSK_PreambleDetect, (0b1 << 7) | (0b01 << 5) | 0x0A);
    SX127x_writef(SX127x_FSK_SyncConfig, 0xC0, 0x1);
    SX127x_writef(SX127x_FSK_SyncConfig, 0x04, 0x1); // FIFO filling condition
    SX127x_write(SX127x_FSK_RxBw,  0x01);
    SX127x_write(SX127x_FSK_AfcBw, 0x01);
    //SX127x_writef(SX127x_FSK_RxConfig, 0x80, 0x1);
    SX127x_writef(SX127x_FSK_RxConfig, 1 << 3, 0x1);
    //SX127x_writef(SX127x_FSK_RxConfig, 0x06, 0x06);

    SX127x_write(SX127x_FSK_SyncValue1, 0x01);
    SX127x_write(SX127x_FSK_SyncValue2, 0x01);
    SX127x_write(SX127x_FSK_SyncValue3, 0x01);
    SX127x_write(SX127x_FSK_SyncValue4, 0x01);
    SX127x_write(SX127x_FSK_SyncValue5, 0x01);
    SX127x_write(SX127x_FSK_SyncValue6, 0x01);
    SX127x_write(SX127x_FSK_SyncValue7, 0x01);
    SX127x_write(SX127x_FSK_SyncValue8, 0x01);
    SX127x_writef(SX127x_FSK_SyncConfig, 0x07, 0x7);
    //SX127x_write(SX127x_FSK_PreambleLsb, 8);

    SX127x_writef(SX127x_FSK_PacketConfig1, 1 << 3, 0x1); // CRC auto clear off
    //SX127x_writef(SX127x_FSK_PacketConfig1, 1 << 0, 0x1); // IBM whitening
    SX127x_writef(SX127x_FSK_PacketConfig1, 0x60, 0b10); // Data whitening

    SX127x_write(SX127x_FSK_SeqConfig1, 0b01 << 3);
    SX127x_write(SX127x_FSK_SeqConfig2, (0b001 << 5) | (0b00 << 3) | 0b100);

    //SX127x_writef(SX127x_Lna, 0xE0, 0b110);
}

void SX127x_FSK_set_Bitrate (int bitrate)
{
    FSK_Bitrate = bitrate;
    if (bitrate <= 299065)
    {
        SX127x_write(SX127x_FSK_BitrateMsb, (SX127x_FXOSC / FSK_Bitrate) >> 8);
        SX127x_write(SX127x_FSK_BitrateLsb, SX127x_FXOSC / FSK_Bitrate);
        SX127x_write(SX127x_FSK_BitRateFrac, (SX127x_FXOSC % FSK_Bitrate) * 16 / FSK_Bitrate);
    }
    else
    {
        SX127x_write(SX127x_FSK_BitrateMsb, 0x00);
        SX127x_write(SX127x_FSK_BitrateLsb, 0x6B);
        SX127x_write(SX127x_FSK_BitRateFrac, 0x00);
    }
}

uint32_t SX127x_FSK_get_Bitrate (void)
{
    return FSK_Bitrate;
}

void SX127x_FSK_set_Deviation (int deviation)
{
    FSK_Deviation = deviation;
    SX127x_write(SX127x_FSK_FdevMsb, (FSK_Deviation / SX127x_FSTEP) >> 8);
    SX127x_write(SX127x_FSK_FdevLsb, FSK_Deviation / SX127x_FSTEP);
}

uint32_t SX127x_FSK_get_Deviation (void)
{
    return FSK_Deviation;
}

void SX127x_FSK_set_PayloadLength (int payloadLength)
{
    FSK_PayloadLength = payloadLength;
    SX127x_writef(SX127x_FSK_PacketConfig2, 0x7, (FSK_PayloadLength >> 8) & 0b111);
    SX127x_write(SX127x_FSK_PayloadLength , FSK_PayloadLength);
}

uint32_t SX127x_FSK_get_PayloadLength (void)
{
    return FSK_PayloadLength;
}

void SX127x_FSK_set_FifoThreshold (int threshold)
{
    if (threshold == 0) SX127x_write(SX127x_FSK_FifoThresh, 0x80);
    else SX127x_write(SX127x_FSK_FifoThresh, (threshold - 1) & 0x7F);
}

void SX127x_FSK_wait_TxReady (void)
{
    while (SX127x_read(SX127x_FSK_IrqFlags1) & 0x20) ;
}

void SX127x_FSK_TX_mode (void)
{
    SX127x_set_LoRaMode(false);
    SX127x_set_Mode(SX127x_MODE_STDBY);
    SX127x_set_DioMapping(0, 0x00);
    SX127x_set_DioMapping(3, 0x01);
    SX127x_set_Mode(SX127x_MODE_FSTX);
    while (!(SX127x_read(SX127x_FSK_IrqFlags1) & 0x80)) ;
}

void SX127x_FSK_RX_mode (void)
{
    SX127x_set_LoRaMode(false);
    SX127x_set_Mode(SX127x_MODE_STDBY);
    SX127x_set_DioMapping(0, 0x00);
    SX127x_set_DioMapping(1, 0x00);
    SX127x_set_Mode(SX127x_MODE_RX);
}

void SX127x_FSK_fifo_clear (void)
{
    SX127x_writef(SX127x_FSK_IrqFlags2, 0x10, 1);
}

void SX127x_FSK_fifo_write (void)
{
    //for (int i = 0; i < 64; i++) SX127x_write(SX127x_Fifo, i);
}

void SX127x_FSK_wait_ModeReady (void)
{
    while (!(SX127x_read(SX127x_FSK_IrqFlags1) & 0x80)) ;
}

char SX127x_FSK_flags1 (void)
{
    return SX127x_read(SX127x_FSK_IrqFlags1);
}

char SX127x_FSK_flags2 (void)
{
    return SX127x_read(SX127x_FSK_IrqFlags2);
}

char SX127x_FSK_get_PllLock (void)
{
    return (SX127x_read(SX127x_FSK_IrqFlags1) & 0x10) > 0;
}
