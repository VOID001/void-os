#ifndef _KBD_DRIVER_H
#define _KBD_DRIVER_H

#include "c_x86asm.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_COMMAND_ACK 0xFA
#define KEYBOARD_COMMAND_RESEND 0xFE
#define KEYBOARD_COMMAND_ECHO 0xEE
#define KEYBOARD_MAX_TRY 3

#define KEYBOARD_COMMAND_SCANCODE 0xF0
#define KEYBOARD_COMMAND_SCANCODE_GET 0

//char keycode_map[] = {};

int send_command(uint8_t command);
uint8_t get_rawdata(void);
uint8_t get_current_scanset(void);
void main_kbdroutine(void);

#endif
