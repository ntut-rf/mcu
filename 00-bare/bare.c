#include <stdint.h>

void main(void)
{
	while (1) ;
}

__attribute((section(".isr_vector"))) __attribute__((used))
uint32_t *isr_vectors[] =
{
	0,
	(uint32_t *) Reset_Handler,	/* code entry point */
};
