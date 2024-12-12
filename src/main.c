#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "backend.h"
#include "controlstrings.h"


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
    char context_char;
    char* context_string;
    Instruction* old_instruction;
    Instruction* instruction;
    setvbuf(stdout, NULL, _IONBF, 0);

    instruction = init_backend();

    while(1)
    {
        pthread_mutex_lock(&backend_lock);

        switch(instruction->type)
        {
        case NONE_I:
            if(!instruction->next_instruction) break;
            old_instruction = instruction;
            instruction = instruction->next_instruction;
            free(old_instruction->context);
            free(old_instruction);
            break;

        case PRINTABLE_I:
            context_char = instruction->context.character;
            write(STDOUT_FILENO, context_char, 1);
            instruction->type = NONE_I;
            break;

        case CURSOR_I:
            context_string = instruction->context.string;
            printf("%s", context_string);
            instruction->type = NONE_I;
            break;

        case BACKSPACE_I:
            write(STDOUT_FILENO, BACKSPACE, 3);
            instruction->type = NONE_I;
            break;
        }

        pthread_mutex_unlock(&backend_lock);
    }

    return 0;
}

