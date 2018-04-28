#ifndef SX127x_H
#define SX127x_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

void    SX127x_init (void);
uint8_t SX127x_read   (uint8_t addr);
void    SX127x_write  (uint8_t addr, uint8_t val);
uint8_t SX127x_readf  (uint8_t addr, uint8_t mask);
void    SX127x_writef (uint8_t addr, uint8_t mask, uint8_t val);
void    SX127x_readn  (uint8_t addr, int n, uint8_t* buffer);
void    SX127x_writen (uint8_t addr, int n, uint8_t* data);

#ifdef __cplusplus
}
#endif

#endif
