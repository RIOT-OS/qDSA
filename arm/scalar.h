#ifndef SCALAR_H
#define SCALAR_H

#include "fe1271.h"
#include "compress.h"

typedef kpc group_scalar;

typedef union
{
    unsigned char b[64];
    uint32 v[16];
}
bigint;

void group_scalar_ops(uint32 *s, const group_scalar *r, 
        const uint32 *h, const uint32 *d);
void group_scalar_get32(uint32 *r, const unsigned char x[32]);
void group_scalar_get64(uint32 *r, const unsigned char x[64]);

#endif
