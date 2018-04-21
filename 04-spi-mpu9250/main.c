#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <stdint.h>
#include <stdio.h>
#include "uart-printf.h"
#include "mpu9250.h"

int main (void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    usart_setup();
    MPU9250_setup_SPI();
    for (int i = 0; i <= 0x7E; i++)
        printf("0x%02x: 0x%02x\r\n", i, MPU9250_read(i));
}
