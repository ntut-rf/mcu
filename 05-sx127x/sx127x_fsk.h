#ifndef SX127x_FSK_H
#define SX127x_FSK_H

#ifdef __cplusplus
extern "C"
{
#endif

void     SX127x_FSK_init (void);
void     SX127x_FSK_set_Bitrate (int bitrate);
uint32_t SX127x_FSK_get_Bitrate (void);
void     SX127x_FSK_set_Deviation (int deviation);
uint32_t SX127x_FSK_get_Deviation (void);
void     SX127x_FSK_set_PayloadLength (int payloadLength);
uint32_t SX127x_FSK_get_PayloadLength (void);
void     SX127x_FSK_set_FifoThreshold (int threshold);
void     SX127x_FSK_TX_mode (void);
void     SX127x_FSK_RX_mode (void);
void     SX127x_FSK_wait_ModeReady (void);
void     SX127x_FSK_wait_TxReady (void);
void     SX127x_FSK_fifo_write (void);
void     SX127x_FSK_fifo_clear (void);
char     SX127x_FSK_flags1 (void);
char     SX127x_FSK_flags2 (void);
char     SX127x_FSK_get_PllLock (void);

#ifdef __cplusplus
}
#endif

#endif
