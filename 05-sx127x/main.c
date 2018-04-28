#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>
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
int Continuous_mode = false;
uint32_t TX_count = 0;
int TX_started = false;

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
    update_mode();
    print_Info();
    print_Help();
    prepare_packet("Hello world!");

    while (1)
    {
        if (USART2_SR & USART_SR_RXNE) user_input(usart_recv(USART2));
        if (!LoRa_mode && TX_mode && Continuous_mode && TX_started)
        {
            SX127x_FSK_wait_ModeReady();
            transmit_packet();
        }
    }
}

void update_mode (void)
{
    SX127x_set_LoRaMode(LoRa_mode);
    LoRa_mode ?
        (TX_mode ? SX127x_LoRa_TX_mode() : SX127x_LoRa_RX_mode()):
        (TX_mode ? SX127x_FSK_TX_mode() : SX127x_FSK_RX_mode());
    if (TX_started || !Continuous_mode) transmit_packet();
    print_Mode();
}

void user_input (char c)
{
    switch (c)
    {
        case 'l':
        case 'L':
        {
            LoRa_mode = !LoRa_mode;
            update_mode();
        } break;

        case 't':
        case 'T':
        {
            TX_mode = !TX_mode;
            update_mode();
        } break;

        case 'c':
        case 'C':
        {
            Continuous_mode = !Continuous_mode;
            if (!Continuous_mode) TX_started = false;
            print_Mode();
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
                if (Continuous_mode) TX_started = !TX_started;
                update_mode();
            }
        }
    }
}

void print_Info (void)
{
    printf("Power: %u dBm\r\n", SX127x_get_Power());
    printf("LoRa Spreading factor: %lu\r\n", SX127x_LoRa_getSpreadingFactor());
    printf("LoRa Payload length: %lu\r\n", SX127x_LoRa_getPayloadLength());
    printf("FSK Bitrate: %lu\r\n", SX127x_FSK_get_Bitrate());
    printf("FSK Deviation: %lu\r\n", SX127x_FSK_get_Deviation());
    printf("\r\n");
}

void print_Help (void)
{
    printf(
        "Commands:\r\n"
        "  - L: toggle LoRa/FSK mode\r\n"
        "  - T: switch to TX/RX mode\r\n"
        "  - C: toggle continuous mode\r\n"
        "  - Space: send single packet in cont. mode / toggle TX in cont. mode\r\n"
        "  - I: print status info\r\n"
        "  - H: display this help\r\n"
        "\r\n"
    );
}

void print_Mode (void)
{
    printf("Current mode: %s %s %s\r\n", LoRa_mode ? "LoRa" : "FSK", TX_mode ? "TX" : "RX", TX_mode ? (Continuous_mode ? "Continuous" : "Single") : "");
}

void prepare_packet (char* data)
{
    memcpy(buffer + 2, data, strlen(data));
}

void transmit_packet (void)
{
    buffer[0] = TX_count;
    buffer[1] = TX_count >> 8;
    if (LoRa_mode)
    {
        SX127x_LoRa_transmit(SX127x_LoRa_getPayloadLength(), buffer);
        printf("LoRa send packet, count %lu, length %lu: ", TX_count, SX127x_LoRa_getPayloadLength());
    }
    else
    {
        SX127x_FSK_fifo_clear();
        SX127x_writen(SX127x_Fifo, 64, buffer); //SX127x_FSK_fifo_write();
        //SX127x_set_Mode(RFM_MODE_FSTX);
        SX127x_set_Mode(SX127x_MODE_TX);
        printf("FSK send packet, count %lu: ", TX_count);
    }
    printf("%s\r\n", (char*)buffer + 2);
    TX_count++;
}

void receive_packet (void)
{
    if (LoRa_mode)
    {
        SX127x_LoRa_read(SX127x_LoRa_getPayloadLength(), buffer);
        uint32_t RX_count = buffer[0] | (buffer[1] << 8);
        printf("LoRa receive packet, count %lu ", RX_count);
    }
    else
    {
        SX127x_readn(SX127x_Fifo, 64, buffer);
        SX127x_FSK_fifo_clear();
        uint32_t RX_count = buffer[0] | (buffer[1] << 8);
        printf("FSK receive packet, count %lu:", RX_count);
    }
    printf("%s\r\n", (char*)buffer + 2);
}

void exti0_isr(void)
{
    exti_reset_request(EXTI0);
    if (LoRa_mode)
    {
        if (TX_mode)
        {
            if (Continuous_mode && TX_started) transmit_packet();
        }
        else
        {
            receive_packet();
        }
        SX127x_LoRa_flags_clear();
    }
    else
    {
        if (TX_mode)
        {
            SX127x_set_Mode(SX127x_MODE_STDBY);
        }
        else
        {
            SX127x_set_Mode(SX127x_MODE_RX);
            receive_packet();
        }
    }
}
