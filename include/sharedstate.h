#ifndef SHARED_STATE
#define SHARED_STATE

#include <pthread.h>
#include <stdint.h>

typedef struct State
{
    unsigned int cursor_x;
    unsigned int cursor_y;

    Rope* text;

    uint64_t timestamp;

} State;

#endif

