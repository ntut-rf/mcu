#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "uart-printf.h"
#include "sx127x.h"
#include "sx127x_reg.h"
#include "sx127x_common.h"
#include "sx127x_fsk.h"
#include "sx127x_lora.h"
#include "main.h"

uint8_t buffer[256];

int TX_mode     = true;
int LoRa_mode   = true;

int main (void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    usart_setup();
    SX127x_init();
    printf("\r\n"
           "***********************************************\r\n"
           "************* SX127x Test Program *************\r\n"
       );
    printf("SX127x init\r\n");
    uint8_t version = SX127x_read(SX127x_Version);
    printf("SX127x Version: 0x%02x %s\r\n", version, version == 0x12 ? "OK" : "ERROR");
    SX127x_FSK_init();
    SX127x_LoRa_init();
    print_Mode();
    print_Info();
    print_Help();

    while (1)
    {
        if (USART2_SR & USART_SR_RXNE) user_input(usart_recv(USART2));
    }
}

void switch_mode (void)
{
    SX127x_set_LoRaMode(LoRa_mode);
    LoRa_mode ?
        (TX_mode ? SX127x_LoRa_TX_mode() : SX127x_LoRa_RX_mode()):
        (TX_mode ? SX127x_FSK_TX_mode() : SX127x_FSK_RX_mode());
    print_Mode();
}

void user_input (char c)
{
    switch (c)
    {
        case 'l':
        case 'L':
        {
            LoRa_mode = true;
            switch_mode();
        } break;

        case 'f':
        case 'F':
        {
            LoRa_mode = false;
            switch_mode();
        } break;

        case 't':
        case 'T':
        {
            TX_mode = true;
            switch_mode();
        } break;

        case 'r':
        case 'R':
        {
            TX_mode = false;
            switch_mode();
        } break;

        case 'h':
        case 'H':
        {
            print_Help();
        } break;

        case 'i':
        case 'I':
        {
            print_Mode();
            print_Info();
        } break;

        case ' ':
        {
            if (TX_mode)
            {
                prepare_packet();
                transmit_packet();
            }
        }
    }
}

void print_Info (void)
{
    printf("Power: %u dBm\r\n", SX127x_get_Power());
    if (LoRa_mode)
    {
        printf("LoRa Spreading factor: %lu\r\n", SX127x_LoRa_getSpreadingFactor());
        printf("LoRa Payload length: %lu\r\n", SX127x_LoRa_getPayloadLength());
    }
    printf("\r\n");
}

void print_Help (void)
{
    printf(
        "Commands:\r\n"
        "  - L: switch to LoRa mode\r\n"
        "  - F: switch to FSK mode\r\n"
        "  - T: switch to TX mode\r\n"
        "  - R: switch to RX mode\r\n"
        "  - Space: send packet\r\n"
        "  - I: print status info\r\n"
        "  - H: display this help\r\n"
        "\r\n"
    );
}

void print_Mode (void)
{
    printf("Current mode: %s %s\r\n", LoRa_mode ? "LoRa" : "FSK", TX_mode ? "TX" : "RX");
}

void prepare_packet (void)
{
}

void transmit_packet (void)
{
    if (LoRa_mode)
    {
        SX127x_LoRa_transmit(SX127x_LoRa_getPayloadLength(), buffer);
        printf("LoRa send packet: length %lu\r\n", SX127x_LoRa_getPayloadLength());
    }
    else
    {
        SX127x_FSK_fifo_clear();
        SX127x_writen(SX127x_Fifo, 64, buffer); //SX127x_FSK_fifo_write();
        SX127x_set_Mode(SX127x_MODE_TX);
        printf("FSK send packet\r\n");
    }
}

void read_packet (void)
{
    SX127x_LoRa_read(SX127x_LoRa_getPayloadLength(), buffer);
}

void decode_packet (void)
{
}
