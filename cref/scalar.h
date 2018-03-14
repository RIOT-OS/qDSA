#ifndef SCALAR_H
#define SCALAR_H

#include "fe1271.h"
#include "compress.h"

void group_scalar_ops(unsigned char *s, const unsigned char *r,
        const unsigned char *h, const unsigned char *d);
void group_scalar_get32(unsigned char *r, const unsigned char x[32]);
void group_scalar_get64(unsigned char *r, const unsigned char x[64]);
void group_scalar_set_pos(unsigned char *r, const unsigned char *x);

#endif
