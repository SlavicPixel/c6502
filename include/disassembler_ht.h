#include "uthash.h"

typedef struct {
    uint16_t address;
    char line[64];
    UT_hash_handle hh;
} Line;
