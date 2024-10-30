#include "endian.h"

int endian() {
    // Optimization *should* simplify this in dead code removal
    int i = 1;
    return (int) *((unsigned char*) &i) == 1;
}