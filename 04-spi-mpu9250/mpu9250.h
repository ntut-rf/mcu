#ifndef MPU9250_H
#define MPU9250_H

void MPU9250_setup_SPI (void);
uint8_t MPU9250_read (uint8_t addr);
uint8_t MPU9250_readf (uint8_t addr, uint8_t mask);
void MPU9250_write (uint8_t addr, uint8_t val);
void MPU9250_writef (uint8_t addr, uint8_t mask, uint8_t val);

#endif
