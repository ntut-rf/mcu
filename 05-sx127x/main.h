#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

void user_input(char c);
void prepare_packet (char* data);
void transmit_packet (void);
void receive_packet (void);
void print_Info (void);
void print_Mode (void);
void print_Help (void);
void update_mode (void);

#ifdef __cplusplus
}
#endif

#endif
