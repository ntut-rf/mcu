#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>
#include <stdint.h>
#include <stdio.h>
#include "sx127x.h"

#define RESET       GPIO1
#define SPI1_CS     GPIO4
#define SPI1_SCK    GPIO5
#define SPI1_MISO   GPIO6
#define SPI1_MOSI   GPIO7

static void SX127x_setup_SPI (void);
static void SX127x_setup_EXTI (void);

void SX127x_init (void)
{
    /* RESET pin */
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, RESET);
    gpio_clear(GPIOA, RESET);
    for (int i = 0; i < 1000000; i++) ;
    gpio_set(GPIOA, RESET);

    SX127x_setup_SPI();
    SX127x_setup_EXTI();
}

static void SX127x_setup_SPI (void)
{
    /* Enable SPI1 Periph and gpio clocks */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_SPI1);

    /* Configure GPIOs: CS=PA4, SCK=PA5, MISO=PA6 and MOSI=PA7 */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, SPI1_CS);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, SPI1_SCK | SPI1_MOSI);
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, SPI1_MISO);

    /* Reset SPI, SPI_CR1 register cleared, SPI is disabled */
    spi_reset(SPI1);

    /* Set up SPI in Master mode with:
     * Clock baud rate: 1/4 of peripheral clock frequency
     * Clock polarity: Idle low
     * Clock phase: Data valid on 1st clock pulse
     * Data frame format: 8-bit
     * Frame format: MSB First
     */
     spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_4, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                     SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

    /* Set NSS management to software */
    spi_enable_software_slave_management(SPI1);
    spi_set_nss_high(SPI1);
    gpio_set(GPIOA, SPI1_CS);

    /* Enable SPI1 */
    spi_enable(SPI1);
}

void SX127x_setup_EXTI (void)
{
    /* Enable GPIOA clock. */
    rcc_periph_clock_enable(RCC_GPIOA);

	/* Enable AFIO clock. */
	rcc_periph_clock_enable(RCC_AFIO);

	/* Enable EXTI0 interrupt. */
	nvic_enable_irq(NVIC_EXTI0_IRQ);

	/* Set GPIO0 (in GPIO port A) to 'input open-drain'. */
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO0);

	/* Configure the EXTI subsystem. */
	exti_select_source(EXTI0, GPIOA);
	exti_set_trigger(EXTI0, EXTI_TRIGGER_RISING);
	exti_enable_request(EXTI0);
}



uint8_t SX127x_read (uint8_t addr)
{
    gpio_clear(GPIOA, SPI1_CS);
    spi_xfer(SPI1, addr & 0x7F);
    uint8_t rval = spi_xfer(SPI1, 0x00);
    gpio_set(GPIOA, SPI1_CS);
    return rval;
}

void SX127x_write (uint8_t addr, uint8_t val)
{
    gpio_clear(GPIOA, SPI1_CS);
    spi_xfer(SPI1, addr | 0x80);
    spi_xfer(SPI1, val);
    gpio_set(GPIOA, SPI1_CS);
}

uint8_t SX127x_readf (uint8_t addr, uint8_t mask)
{
    return (SX127x_read(addr) & mask) >> __builtin_ctz(mask);
}

void SX127x_writef (uint8_t addr, uint8_t mask, uint8_t val)
{
    uint8_t wval = SX127x_read(addr);
    wval &= ~mask;
    wval |= ((val << __builtin_ctz(mask)) & mask);
    gpio_clear(GPIOA, SPI1_CS);
    spi_xfer(SPI1, addr | 0x80);
    spi_xfer(SPI1, wval);
    gpio_set(GPIOA, SPI1_CS);
}

void SX127x_readn (uint8_t addr, int n, uint8_t* buffer)
{
    gpio_clear(GPIOA, SPI1_CS);
    spi_xfer(SPI1, addr & 0x7F);
    for (int i = 0; i < n; i++) buffer[i] = spi_xfer(SPI1, 0x00);
    gpio_set(GPIOA, SPI1_CS);
}

void SX127x_writen (uint8_t addr, int n, uint8_t* data)
{
    gpio_clear(GPIOA, SPI1_CS);
    spi_xfer(SPI1, addr | 0x80);
    for (int i = 0; i < n; i++) spi_xfer(SPI1, data[i]);
    gpio_set(GPIOA, SPI1_CS);
}
