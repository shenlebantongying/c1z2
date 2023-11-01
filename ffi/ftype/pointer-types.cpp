#include "pointer-types.h"
#include <stdio.h>

int plus_sab(struct sab* s)
{
    printf("Address -> %p from C\n", s);
    return s->a + s->b;
}
