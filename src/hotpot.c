#include <stdio.h>
#include "include/raylib.h"
#include "include/hotpot.h"

void hotpot_hi(void) {
    printf("hello from hot\n");
}

Pot pot = {1, 0};

Pot hotpot_init(void) {
    return pot;
}

void hotpot_update(Pot new_pot) {
    pot.version = new_pot.version;
    pot.pause = !new_pot.pause;
}
