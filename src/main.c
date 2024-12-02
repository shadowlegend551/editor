#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "backend.h"


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
    Instruction* old_instruction;
    Instruction* instruction;
    setvbuf(stdout, NULL, _IONBF, 0);

    instruction = init_backend();

    while(1)
    {
        pthread_mutex_lock(&backend_lock);
        if(instruction->type == NONE && instruction->next_instruction)
        {
            old_instruction = instruction;
            instruction = instruction->next_instruction;
            free(old_instruction->context);
            free(old_instruction);
        }
        else if(instruction->type == PRINTABLE_CHAR)
        {
            write(STDOUT_FILENO, instruction->context, 1);
            instruction->type = NONE;
        }
        else if(instruction->type == CURSOR)
        {
            printf("%s", instruction->context);
            instruction->type = NONE;
        }
        else if(instruction->type == BACKSPACE)
        {
            write(STDOUT_FILENO, BACKSPACE_CHAR, 3);
            instruction->type = NONE;
        }
        pthread_mutex_unlock(&backend_lock);
    }

    return 0;
}

