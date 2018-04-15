MEMORY
{
    FLASH   (rwx)   : ORIGIN = 0x00000000, LENGTH = 0x40000
    RAM     (rwx)   : ORIGIN = 0x20000000, LENGTH = 0x10000
}

/*
 * Reserve memory for heap and stack. The linker will issue an error if there
 * is not enough memory.
 *
 * NOTE: The reserved heap and stack will be added to the bss column of the
 * binutils size command.
 */
_Min_Heap_Size  = 0x200;    /* required amount of heap */
_Min_Stack_Size = 0x400;    /* required amount of stack */

INCLUDE barebsp/common.ld
