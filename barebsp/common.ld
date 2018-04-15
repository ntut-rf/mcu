/*
 * GCC linker script for STM32 microcontrollers (ARM Cortex-M).
 *
 * It exports the symbols needed for the CMSIS assembler startup script for GCC
 * ARM toolchains (_sidata, _sdata, _edata, _sbss, _ebss) and sets the entry
 * point to Reset_Handler.
 *
 * Adapt rom/ram size for your particular device below.
 *
 * @author      Bjørn Forsman
 * edited by    En Shih
 */

/* Entry Point */
ENTRY(Reset_Handler)

/*
 * The stack starts at the end of ram and grows downwards. Full-descending
 * stack; decrement first, then store.
 */
_estack = ORIGIN(ram) + LENGTH(ram);

SECTIONS
{
    /* Reset and ISR vectors */
    .isr_vector :
    {
        . = ALIGN(4);
        __isr_vector_start__ = .;
        KEEP(*(.isr_vector)) /* without 'KEEP' the garbage collector discards this section */
        . = ALIGN(4);
        __isr_vector_end__ = .;
        ASSERT(. != __isr_vector_start__, "The .isr_vector section is empty");
    } >ram AT>rom /* load vector table into ram */

    /* LMA address of data section */
    _siisr = LOADADDR(.isr_vector);

    /* Text section (code and read-only data) */
    .text :
    {
        . = ALIGN(4);
        _stext = .;
        *(.text)           /* .text sections (code) */
        *(.text*)          /* .text* sections (code) */

        /*
         * NOTE: .glue_7 and .glue_7t sections are not needed because Cortex-M
         * only supports Thumb instructions, no ARM/Thumb interworking.
         */
        *(.glue_7)         /* glue arm to thumb code */
        *(.glue_7t)        /* glue thumb to arm code */
        *(.eh_frame)

        /* Static constructors and destructors */
        KEEP(*(.init))
        KEEP(*(.fini))

        . = ALIGN(4);
        _etext = .;
    } >rom

    /* Constant data goes into rom */
    .rodata :
    {
      . = ALIGN(4);
      *(.rodata)         /* .rodata sections (constants, strings, etc.) */
      *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
      . = ALIGN(4);
    } >rom

    /*
     * Stack unwinding and exception handling sections.
     *
     * ARM compilers emit object files with .ARM.extab and .ARM.exidx sections
     * when using C++ exceptions. Also, at least GCC emits those sections when
     * dividing large numbers (64-bit) in C. So we have to handle them.
     *
     * (ARM uses .ARM.extab and .ARM.exidx instead of the .eh_frame section
     * used on x86.)
     */
    .ARM.extab : /* exception unwinding information */
    {
        *(.ARM.extab*)
    } >rom
    .ARM.exidx : /* index entries for section unwinding */
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >rom

    /*
     * Newlib and Eglibc (at least) need these for C++ support.
     *
     * (Copied from Sourcery CodeBench Lite: arm-none-eabi-gcc -V)
     */
    .preinit_array :
    {
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP(*(.preinit_array*))
        PROVIDE_HIDDEN(__preinit_array_end = .);
    } >rom
    .init_array :
    {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array*))
        PROVIDE_HIDDEN(__init_array_end = .);
    } >rom
    .fini_array :
    {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array*))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } >rom

    /* LMA address of data section; used by the startup to initialize data */
    _sidata = LOADADDR(.data);

    /*
     * Initialized data section. This section is programmed into rom (LMA
     * address) and copied to ram (VMA address) in startup code.
     */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;        /* data section VMA address */
        *(.data)           /* .data sections */
        *(.data*)          /* .data* sections */
        . = ALIGN(4);
        _edata = .;
    } >ram AT>rom

    /* LMA address of ccmram section */
    _siccmram = LOADADDR(.ccmram);

    /* CCM-ram section
    *
    * IMPORTANT NOTE!
    * If initialized variables will be placed in this section,
    * the startup code needs to be modified to copy the init-values.
    */
    .ccmram :
    {
      . = ALIGN(4);
      _sccmram = .;       /* create a global symbol at ccmram start */
      *(.ccmram)
      *(.ccmram*)

      . = ALIGN(4);
      _eccmram = .;       /* create a global symbol at ccmram end */
    } >CCMRAM AT> rom

    /* Persistent variables saved in rom */
    .romvar :
    {
      . = ALIGN(2048);
      _sromvar = .;
      *(.romvar)         /* .romvar sections */
      *(.romvar*)        /* .romvar* sections */
      KEEP(*(.romvar))
      KEEP(*(.romvar*))
      _eromvar = .;
      . = ALIGN(2048);
    } >rom

    /* Uninitialized data section (zeroed out by startup code) */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } >ram

    /* User_heap_stack section, used to check that there is enough ram left */
    ._user_heap_stack :
    {
      . = ALIGN(8);
      PROVIDE ( end = . );
      PROVIDE ( _end = . );
      . = . + _Min_Heap_Size;
      . = . + _Min_Stack_Size;
      . = ALIGN(8);
    } >ram

    /* Remove information from the standard libraries */
    /DISCARD/ :
    {
      libc.a ( * )
      libm.a ( * )
      libgcc.a ( * )
    }

    .ARM.attributes 0 : { *(.ARM.attributes) }
}

/* Nice to have */
__isr_vector_size__ = SIZEOF(.isr_vector);
__text_size__ = SIZEOF(.text);
__data_size__ = SIZEOF(.data);
__bss_size__ = SIZEOF(.bss);
