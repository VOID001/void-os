#include "kbd_driver.h"

uint8_t get_rawdata()
{
    uint8_t data;
    while(!(data = inb(KEYBOARD_DATA_PORT)));
    return data;
}

uint8_t get_current_scanset()
{
    int ret;
    ret = send_command(KEYBOARD_COMMAND_SCANCODE);
    if(ret == 0)
        get_rawdata();
    else
        return 0xFF;
    ret = send_command(KEYBOARD_COMMAND_SCANCODE_GET);
    if(ret == 0)
        return get_rawdata();
    return 0xFF;
}

int send_command(uint8_t command)
{
    int tried = 0;
    uint8_t recv;
    while(tried < KEYBOARD_MAX_TRY)
    {
        /* Send the command */
        outb(KEYBOARD_DATA_PORT, command);
        /* Wait for the keyboard answer */
        while(!(recv = inb(KEYBOARD_DATA_PORT)));
        if(recv == KEYBOARD_COMMAND_ACK)
            return 0;
        /* We should resend it */
        else if(recv == KEYBOARD_COMMAND_RESEND)
            tried++;
        /* Other code means error */
        else 
            return 1;
    }
    return 1;
}
