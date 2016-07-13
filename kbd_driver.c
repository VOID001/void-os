#include "kbd_driver.h"
#include "stdio.h"
#include "string.h"

uint8_t get_rawdata()
{
    uint8_t data;
    while(!(data = inb(KEYBOARD_DATA_PORT)));
    return data;
}

uint8_t get_current_scanset()
{
    outb(KEYBOARD_DATA_PORT, KEYBOARD_COMMAND_SCANCODE);
    printf("The first data %x\n",get_rawdata());
    outb(KEYBOARD_DATA_PORT, KEYBOARD_COMMAND_SCANCODE_GET);
    printf("The second data %x\n",get_rawdata());
    return get_rawdata();
}

int send_command(uint8_t command)
{
    int tried = 0;
    uint8_t recv;
    while(tried < KEYBOARD_MAX_TRY)
    {
        /* Send the command */
        outb(KEYBOARD_COMMAND_PORT, command);
        /* Wait for the keyboard answer */
        while(!(recv = inb(KEYBOARD_COMMAND_PORT)));
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

void init_keyboard()
{

    capslock_on = false;
    lshift_pressed = false;
    rshift_pressed = false;

    const char first_series[] = "1234567890-=";
    const char first_series_shift[] = "!@#$%^&*()_+";
    const char second_series[] = "qwertyuiop[]";
    const char second_series_shift[] = "QWERTYUIOP{}";
    const char third_series[] = "asdfghjkl;'";
    const char third_series_shift[] = "ASDFGHJKL:\"";
    const char fourth_series[] = "zxcvbnm,./";
    const char fourth_series_shift[] = "ZXCVBNM<>?";

    for(int i = 0x02, cnt = 0; i <= 0x0d; i++, cnt++)
    {
        keycode_map[i] = first_series[cnt];
        keycode_map_shift[i] = first_series_shift[cnt];
    }

    for(int i = 0x10, cnt =0; i <=0x1b; i++, cnt++)
    {
        keycode_map[i] = second_series[cnt];
        keycode_map_shift[i] = second_series_shift[cnt];
    }

    keycode_map[0x1c] = '\n';
    keycode_map_shift[0x1c] = '\n';

    for(int i = 0x1e, cnt = 0; i <= 0x28; i++, cnt++)
    {
        keycode_map[i] = third_series[cnt];
        keycode_map_shift[i] = third_series_shift[cnt];
    }

    for(int i = 0x2c, cnt = 0; i <= 0x35; i++, cnt++)
    {
        keycode_map[i] = fourth_series[cnt];
        keycode_map_shift[i] = fourth_series_shift[cnt];
    }

    keycode_map[0x39] = ' ';
    keycode_map_shift[0x39] = ' ';

    keycode_map[0x0E] = '\b';
    keycode_map_shift[0x0E] = '\b';

}

uint8_t translate_scancode()
{
    uint8_t scancode = get_rawdata();
    //printf("Scancode is **%x**\n", scancode);
    if(keycode_map[scancode] != 0)
    {
        if(lshift_pressed || rshift_pressed)
        {
            printf("%c", keycode_map_shift[scancode]);
            write_to_bufio(keycode_map_shift[scancode]);
        }
        else if(capslock_on)
        {
            printf("%c", toupper(keycode_map[scancode]));
            write_to_bufio(toupper(keycode_map[scancode]));
        }
        else
        {
            printf("%c", keycode_map[scancode]);
            write_to_bufio(keycode_map[scancode]);
        }
            
    }

    else if(scancode == KEYBOARD_SCANCODE_LSHIFT_PRESSED)
        lshift_pressed = true;
    else if(scancode == KEYBOARD_SCANCODE_RSHIFT_PRESSED)
        rshift_pressed = true;
    else if(scancode == KEYBOARD_SCANCODE_LSHIFT_RELEASED)
        lshift_pressed = false;
    else if(scancode == KEYBOARD_SCANCODE_RSHIFT_RELEASED)
        rshift_pressed = false;
    else if(scancode == KEYBOARD_SCANCODE_CAPSLOCK_RELEASED)
        capslock_on = ! capslock_on;
    return scancode;

}
