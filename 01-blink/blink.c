#include <stdint.h>
#include "devices/stm32f103xx/rcc.h"
#include "devices/stm32f103xx/gpio.h"

/* For STM32F103 Technical Reference, see http://www.st.com/resource/en/reference_manual/cd00171190.pdf */

void Reset_Handler (void)
{
    /* Set bit 2 of the RCC_APB2ENR register. This enables the GPIOA peripheral clock, required for functions of the GPIOA port.
       See page 113 of the Reference manual for more information. */
    RCC->APB2ENR |= 1 << 2;

    /* Configure the 5th pin of the GPIOA port to be "Output, max speed 10 MHz".
       This pin corresponds to the on-board LED of the STM32F103 Nucleo.
       See page 170 of the Reference manual for more information on the GPIOA_CRL register. */
    GPIOA->CRL = 0x00100000;

	while (1)
    {
        /* Toggle the 5th pin output of GPIOA port. This will toggle the on-board LED */
        GPIOA->ODR ^= 1 << 5;

        /* Delay some time using a simple for loop */
        for (int i = 0; i < 1000000; i++) ;
    }
}

/* This is the vector table, which lists the functions pointers that will be called on reset and on interrupts */
__attribute((section(".isr_vector"))) __attribute__((used))
uint32_t *isr_vectors[] =
{
	0,
	(uint32_t *) Reset_Handler,	/* code entry point on CPU reset */
};
