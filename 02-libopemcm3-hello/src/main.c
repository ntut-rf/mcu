#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <stdio.h>
#include "semihosting.h"

int main (void)
{
    initialise_monitor_handles();
    printf("Initialized monitor handles\n");

    rcc_clock_setup_in_hsi_out_64mhz();
    printf("Clock setup done\n");

    printf("Hello world!\n");

    while (1)
    {
	}

	return 0;
}
