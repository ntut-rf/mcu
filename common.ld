SECTIONS
{
	.text :
	{
        __isr_vector_start__ = .;
		KEEP(*(.isr_vector))
		KEEP(*(.text))
        ASSERT(. != __isr_vector_start__, "The .isr_vector section is empty");
	} >FLASH

    .rodata : { *(.rodata*) } >FLASH
    .bss : { *(.bss*) } >RAM
}
