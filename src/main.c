#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "sharedstate.h"


void raw_mode_enable()
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}


void raw_mode_disable()
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag |= (ICANON|ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}


int main()
{
    raw_mode_enable();
    char ch;
    int read_characters = 0;
    int buffer_index = 0;
    char buffer[8];
    while(1)
    {
        read_characters = read(STDIN_FILENO, &ch, 1);
        if(read_characters)
        {
            buffer[buffer_index] = ch;
            buffer_index++;
            if(buffer_index > 7)
            {
                buffer[buffer_index] = '\0';
                printf("%s\n", buffer);
                buffer_index = 0;
            }
        }
    }

    return 0;
}

