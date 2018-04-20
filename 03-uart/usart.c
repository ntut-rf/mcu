#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <stdio.h>
#include <errno.h>

int main (void)
{
    /* Enable peripheral clocks. */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART2);

    /* Setup GPIO pin 2 on GPIO port A for transmit. */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                       GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO2);

    /* Setup UART parameters. */
    usart_set_baudrate(USART2, 9600);
    usart_set_databits(USART2, 8);
    usart_set_parity(USART2, USART_PARITY_NONE);
    usart_set_stopbits(USART2, USART_STOPBITS_1);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
    usart_set_mode(USART2, USART_MODE_TX);

    /* Finally enable the USART. */
    usart_enable(USART2);

    printf("Hello World!");

    while (1)
    {
    }

    return 0;
}

int _write(int file, char *ptr, int len)
{
    if (file == 1)
    {
        int i;
	for (i = 0; i < len; i++) usart_send_blocking(USART2, ptr[i]);
	return i;
    }
    errno = EIO;
    return -1;
}
