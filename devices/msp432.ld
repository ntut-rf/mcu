MEMORY
{
    FLASH   (rwx)   : ORIGIN = 0x00000000, LENGTH = 0x40000
    RAM     (rwx)   : ORIGIN = 0x20000000, LENGTH = 0x10000
}

INCLUDE common.ld
