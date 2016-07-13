#ifndef _KBD_DRIVER_H
#define _KBD_DRIVER_H

#include "c_x86asm.h"
#include <stdbool.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_COMMAND_PORT 0x64
#define KEYBOARD_COMMAND_ACK 0xFA
#define KEYBOARD_COMMAND_RESEND 0xFE
#define KEYBOARD_COMMAND_ECHO 0xEE
#define KEYBOARD_MAX_TRY 3

#define KEYBOARD_COMMAND_SCANCODE 0xF0
#define KEYBOARD_COMMAND_SCANCODE_GET 0

#define KEYBOARD_SCANCODE_LSHIFT_PRESSED 0x2A
#define KEYBOARD_SCANCODE_RSHIFT_PRESSED 0x36

#define KEYBOARD_SCANCODE_LSHIFT_RELEASED 0xAA
#define KEYBOARD_SCANCODE_RSHIFT_RELEASED 0xB6

#define KEYBOARD_SCANCODE_CAPSLOCK_PRESSED 0x3A
#define KEYBOARD_SCANCODE_CAPSLOCK_RELEASED 0xBA

uint8_t keycode_map_shift[256];

uint8_t keycode_map[256];

bool capslock_on;
bool lshift_pressed;
bool rshift_pressed;

int send_command(uint8_t command);
uint8_t get_rawdata(void);
uint8_t get_current_scanset(void);
void main_kbdroutine(void);
uint8_t translate_scancode(void);
void init_keyboard(void);

#endif
