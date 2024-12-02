#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include "backend.h"


#define ANSI_CODE_BUFFER_LEN 17
#define ANSI_CODE_MAX_LEN 15
#define ANSI_ESCAPE_CHAR 0x1b

char BACKSPACE_CHAR[3] = {0x08, 0x20, 0x08};


pthread_mutex_t backend_lock;

Instruction* init_instruction(InstructionType type, char* context)
{
    Instruction* instruction = malloc(sizeof(Instruction));
    instruction->type = type;
    instruction->context = context;
    instruction->next_instruction = NULL;

    return instruction;
}


void* backend_loop(void* argv)
{
    char* context;
    Instruction* new_instruction;
    Instruction* head_instruction = argv;
    char ch = 0;
    char ansi_code[ANSI_CODE_BUFFER_LEN];
    ansi_code[0] = ANSI_ESCAPE_CHAR;
    int read_characters = 0;

    while(1)
    {
        if(read(STDIN_FILENO, &ch, 1))
        {
            if(ch == ANSI_ESCAPE_CHAR)
            {
                printf("here\n");
                read_characters = read(STDIN_FILENO, ansi_code+1, ANSI_CODE_MAX_LEN);

                // If no escape code was found.
                if(!read_characters)
                {
                    init_instruction(ERROR, NULL);
                    goto append_instruction;
                }

                // Null terminate and copy the string.
                ansi_code[read_characters+1] = '\0';
                context = malloc(read_characters+2);
                strcpy(context, ansi_code);

                new_instruction = init_instruction(CURSOR, context);
            }

            else if(0x20 <= ch && ch <= 0x7e)
            {
                context = malloc(1);
                *context = ch;
                new_instruction = init_instruction(PRINTABLE_CHAR, context);
            }

            else if(ch == 0x7f)
            {
                new_instruction = init_instruction(BACKSPACE, NULL);
            }

        append_instruction:
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
    if(!base_instruction) return NULL;
    base_instruction->next_instruction = NULL;
    base_instruction->context = NULL;
    base_instruction->type = NONE;

    // Initialize mutex and thread.
    pthread_t backend_thread;
    pthread_mutex_init(&backend_lock, NULL);
    pthread_create(&backend_thread, NULL, backend_loop, base_instruction);

    return base_instruction;
}


