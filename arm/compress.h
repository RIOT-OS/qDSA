#ifndef COMP_H
#define COMP_H

#include "ladder.h"
#include "fe1271.h"

typedef union
{
    unsigned char b[32];
    struct { fe1271 l1; fe1271 l2; } fe;
}
kpc;

extern const unsigned char mu_1;
extern const unsigned char mu_2;
extern const unsigned char mu_3;
extern const unsigned char mu_4;

void compress(fe1271 *l1, fe1271 *l2, const kp *x);
int decompress(kp *r, kp *t, const kpc *x);
void kpc_pack(unsigned char r[32], const kpc *x);
void kpc_unpack(kpc *r, const unsigned char x[32]);
void set_const(fe1271 *r, const uint16 c);

#endif
