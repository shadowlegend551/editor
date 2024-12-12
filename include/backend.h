#ifndef BACKEND_H
#define BACKEND_H

#include <pthread.h>


extern pthread_mutex_t backend_lock;

typedef enum InstructionType
{
    ERROR_I,
    NONE_I,
    PRINTABLE_I,
    BACKSPACE_I,
    CURSOR_I,
    COMMAND_I,
    ESCAPE_I

} InstructionType;


typedef union Context
{
    char    character;
    char*   string;

} Context;


typedef struct Instruction Instruction;
typedef struct Instruction
{
    InstructionType type;
    Context         context;
    Instruction*    next_instruction;

} Instruction;


Instruction* init_instruction(InstructionType type, char* context);

void* backend_loop(void* argv);

Instruction* init_backend();

#endif

