#include <libopencm3/stm32/gpio.h>

int main (void)
{
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO5);
    while (1)
    {
        gpio_toggle(GPIOA, GPIO5);	/* LED on/off */

        for (int i = 0; i < 800000; i++)	/* Wait a bit. */
            __asm__("nop");
	}

	return 0;
}
