#ifndef STM32F103XX_GPIO_H
#define STM32F103XX_GPIO_H

#include <stdint.h>

typedef struct GPIO_t
{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_t;

#define GPIOA_BASE 0x40010800
#define GPIOA ((GPIO_t*)GPIOA_BASE)

#endif
