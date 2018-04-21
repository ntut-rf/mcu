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
    printf("\r\n");
    printf("MPU9250 init\r\n");
    printf("MPU9250_WHO_AM_I: 0x%02x\r\n", MPU9250_read(MPU9250_WHO_AM_I));
    while (1)
    {
        int32_t sum;
        int32_t samples;

        sum += (int16_t) (MPU9250_read(MPU9250_ACCEL_ZOUT_H) << 8 | MPU9250_read(MPU9250_ACCEL_ZOUT_L));
        samples ++;

        if (samples >= 5000)
        {
            printf("MPU9250_ACCEL_ZOUT: %f g\r\n", (float) sum / samples / 16384);
            sum = 0;
            samples = 0;
        }
    }
}
