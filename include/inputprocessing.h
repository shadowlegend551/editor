#ifndef INPUTPROCESSING_H
#define INPUTPROCESSING_H

#include <pthread.h>


extern pthread_mutex_t backend_lock;

typedef enum InstructionType
{
    NONE,
    CHARACTER,
    CURSOR,
    COMMAND

} InstructionType;


typedef struct Instruction Instruction;

typedef struct Instruction
{
    InstructionType type;
    void* context;
    Instruction* next_instruction;

} Instruction;



Instruction* init_instruction(InstructionType type, void* context);

void* backend_loop(void* argv);

Instruction* init_backend();

#endif
