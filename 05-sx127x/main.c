#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <stdint.h>
#include <stdio.h>
#include "uart-printf.h"
#include "sx127x.h"
#include "sx127x_reg.h"
#include "sx127x_fsk.h"
#include "sx127x_lora.h"

int main (void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    usart_setup();
    SX127x_init();
    printf("SX127x init\r\n");
    printf("SX127x Version: 0x%02x\r\n", SX127x_read(SX127x_Version));
    SX127x_FSK_init();
    SX127x_LoRa_init();

    while (1)
    {
    }
}
