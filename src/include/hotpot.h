#ifndef HOTPOT_H
#define HOTPOT_H
#include <stdlib.h>

typedef struct {
    size_t version;
    bool pause;
} Pot;

typedef void (*hotpot_t)(void);
typedef Pot (*hotpot_init_t)(void);
typedef void (*hotpot_update_t)(Pot pot);

#endif //HOTPOT_H
