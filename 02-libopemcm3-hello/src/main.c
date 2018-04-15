#include <libopencm3/cm3/nvic.h>
#include <stdio.h>
#include "semihosting.h"

int main (void)
{
    initialise_monitor_handles();

    printf("Hello world!\n");

    while (1)
    {
	}

	return 0;
}
