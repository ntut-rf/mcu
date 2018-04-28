#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

void user_input(char c);
void prepare_packet (void);
void transmit_packet (void);
void read_packet (void);
void decode_packet (void);
void print_Info (void);
void print_Mode (void);
void print_Help (void);
void switch_mode (void);

#ifdef __cplusplus
}
#endif

#endif
