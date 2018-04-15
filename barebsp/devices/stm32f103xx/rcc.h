#ifndef STM32F103XX_RCC_H
#define STM32F103XX_RCC_H

#include <stdint.h>

typedef struct RCC_t
{
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
} RCC_t;

#define RCC_BASE 0x40021000
#define RCC ((RCC_t*)RCC_BASE)

#endif
