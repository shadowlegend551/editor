#ifndef BACKEND_H
#define BACKEND_H

#include <pthread.h>


extern pthread_mutex_t backend_lock;

extern char BACKSPACE_CHAR[3];

typedef enum InstructionType
{
    ERROR,
    NONE,
    PRINTABLE_CHAR,
    BACKSPACE,
    CURSOR,
    COMMAND

} InstructionType;


typedef struct Instruction Instruction;

typedef struct Instruction
{
    InstructionType type;
    char* context;
    Instruction* next_instruction;

} Instruction;



Instruction* init_instruction(InstructionType type, char* context);

void* backend_loop(void* argv);

Instruction* init_backend();

#endif

