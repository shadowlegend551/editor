#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "inputprocessing.h"

pthread_mutex_t backend_lock;

Instruction* init_instruction(InstructionType type, void* context)
{
    Instruction* instruction = malloc(sizeof(Instruction));
    instruction->type = type;
    instruction->context = context;
    instruction->next_instruction = NULL;

    return instruction;
}


void* backend_loop(void* argv)
{
    void* context;
    Instruction* new_instruction;
    Instruction* head_instruction = argv;
    char ch = 0;

    while(1)
    {
        if(read(STDIN_FILENO, &ch, 1))
        {
            if(ch != 0x1b)
            {
                context = malloc(1);
                *(char*)context = ch;
                new_instruction = init_instruction(CHARACTER, context);
            }

            pthread_mutex_lock(&backend_lock);
            head_instruction->next_instruction = new_instruction;
            pthread_mutex_unlock(&backend_lock);

            head_instruction = new_instruction;
        }
    }

}


Instruction* init_backend()
{
    // Set terminal to raw mode.
    struct termios input;
    tcgetattr(STDIN_FILENO, &input);
    input.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &input);

    // Create the initial instruction.
    Instruction* base_instruction = malloc(sizeof(Instruction));
    base_instruction->next_instruction = NULL;
    base_instruction->context = NULL;
    base_instruction->type = NONE;

    pthread_t backend_thread;
    pthread_create(&backend_thread, NULL, backend_loop, base_instruction);

    return base_instruction;
}


