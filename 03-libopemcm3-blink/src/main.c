#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <stdio.h>
#include "semihosting.h"

void clock_setup(void);
void gpio_setup(void);

int main (void)
{
    initialise_monitor_handles();
    printf("Initilaized monitor handles\r\n");

    clock_setup();
    printf("Clock setup done\r\n");

    gpio_setup();
    printf("GPIO setup done\r\n");

    while (1)
    {
        gpio_toggle(GPIOA, GPIO5);	/* LED on/off */

        for (int i = 0; i < 800000; i++)	/* Wait a bit. */
            __asm__("nop");
	}

	return 0;
}

void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Enable clocks for GPIO port B (for GPIO_USART3_TX) and USART3. */
	rcc_periph_clock_enable(RCC_USART2);
}

void gpio_setup(void)
{
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO5);
}
