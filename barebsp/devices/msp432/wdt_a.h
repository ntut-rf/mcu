#ifndef MSP432_WDT_A_H
#define MSP432_WDT_A_H

#include <stdint.h>

typedef struct WDT_A_t
{
    volatile uint32_t Reserved[3];
    volatile uint32_t WDTCTL;
} WDT_A_t;

#define WDT_A_BASE 0x40004800
#define WDT_A ((WDT_A_t*)WDT_A_BASE)

#endif
