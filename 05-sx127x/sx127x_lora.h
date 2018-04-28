#ifndef SX127x_LORA_H
#define SX127x_LORA_H

#ifdef __cplusplus
extern "C"
{
#endif

void     SX127x_LoRa_init (void);
void     SX127x_LoRa_TX_mode (void);
void     SX127x_LoRa_RX_mode (void);
void     SX127x_LoRa_set_SpreadingFactor (int spreadingFactor);
uint32_t SX127x_LoRa_getSpreadingFactor (void);
void     SX127x_LoRa_set_CodingRate (int rate);
void     SX127x_LoRa_set_Bandwidth (int bw);
void     SX127x_LoRa_set_PayloadLength (int payloadLength);
uint32_t SX127x_LoRa_getPayloadLength(void);
void     SX127x_LoRa_transmit (int length, uint8_t* buffer);
void     SX127x_LoRa_read (int length, uint8_t* buffer);
void     SX127x_LoRa_fifo_reset (void);
void     SX127x_LoRa_fifo_write (uint8_t c);
void     SX127x_LoRa_fifo_transmit (void);
char     SX127x_LoRa_flags_read (void);
void     SX127x_LoRa_flags_clear (void);

#ifdef __cplusplus
}
#endif

#endif
