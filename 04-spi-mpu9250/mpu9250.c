#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <stdint.h>
#include <stdio.h>
#include "mpu9250.h"

#define SPI1_CS     GPIO4
#define SPI1_SCK    GPIO5
#define SPI1_MISO   GPIO6
#define SPI1_MOSI   GPIO7

void MPU9250_setup_SPI (void)
{
    /* Enable SPI1 Periph and gpio clocks */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_SPI1);

    /* Configure GPIOs: CS=PA4, SCK=PA5, MISO=PA6 and MOSI=PA7 */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, SPI1_CS);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, SPI1_SCK | SPI1_MOSI );
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, SPI1_MISO);

    /* Reset SPI, SPI_CR1 register cleared, SPI is disabled */
    spi_reset(SPI1);

    /* Set up SPI in Master mode with:
     * Clock baud rate: 1/128 of peripheral clock frequency
     * Clock polarity: Idle high
     * Clock phase: Data valid on 2st clock pulse
     * Data frame format: 8-bit
     * Frame format: MSB First
     */
     spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_128, SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
                     SPI_CR1_CPHA_CLK_TRANSITION_2, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

    /* Set NSS management to software */
    spi_enable_software_slave_management(SPI1);
    spi_set_nss_high(SPI1);
    gpio_set(GPIOA, SPI1_CS);

    /* Enable SPI1 */
    spi_enable(SPI1);
}

uint8_t MPU9250_read (uint8_t addr)
{
    gpio_clear(GPIOA, SPI1_CS);
    spi_xfer(SPI1, 0x80 | addr);
    uint8_t rval = spi_xfer(SPI1, 0x00);
    gpio_set(GPIOA, SPI1_CS);
    return rval;
}

uint8_t MPU9250_readf (uint8_t addr, uint8_t mask)
{
    return (MPU9250_read(addr) & mask) >> __builtin_ctz(mask);
}

void MPU9250_write (uint8_t addr, uint8_t val)
{
    gpio_clear(GPIOA, SPI1_CS);
    spi_xfer(SPI1, addr);
    spi_xfer(SPI1, val);
    gpio_set(GPIOA, SPI1_CS);
    uint8_t rval = MPU9250_read(addr);
    if (rval == val)
        printf("Write 0x%02x = 0x%02x ok\n", addr, val);
    else
        printf("Write 0x%02x = 0x%02x FAIL = 0x%02x\n", addr, val, rval);
}

void MPU9250_writef (uint8_t addr, uint8_t mask, uint8_t val)
{
    uint8_t wval = MPU9250_read(addr);
    wval &= ~mask;
    wval |= ((val << __builtin_ctz(mask)) & mask);
    gpio_clear(GPIOA, SPI1_CS);
    spi_xfer(SPI1, addr);
    spi_xfer(SPI1, wval);
    gpio_set(GPIOA, SPI1_CS);
    uint8_t rval = MPU9250_read(addr);
    if ((rval & mask) == (wval & mask))
        printf("Write 0x%02x = 0x%02x ok\n", addr, wval);
    else
        printf("Write 0x%02x = 0x%02x FAIL = 0x%02x\n", addr, wval, rval);
}
