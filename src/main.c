#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "inputprocessing.h"


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
    Instruction* instruction;
    setvbuf(stdout, NULL, _IONBF, 0);
    pthread_mutex_init(&backend_lock, NULL);

    instruction = init_backend();

    while(1)
    {
        pthread_mutex_lock(&backend_lock);
        if(instruction->type == NONE && instruction->next_instruction)
        {
            instruction = instruction->next_instruction;
        }
        else if(instruction->type == CHARACTER)
        {
            printf("%c", *(char*)(instruction->context));
            instruction->type = NONE;
        }
        pthread_mutex_unlock(&backend_lock);
    }

    return 0;
}

