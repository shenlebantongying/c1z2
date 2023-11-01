#pragma once

#include <cstdint>

extern "C" {

struct sab {
    int32_t a;
    int32_t b;
};

int plus_sab(struct sab* s);

}