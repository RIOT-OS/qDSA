#ifndef LADDER_H
#define LADDER_H

#include "fe1271.h"

typedef struct
{
    fe1271 X;
    fe1271 Y;
    fe1271 Z;
    fe1271 T;
}
kp;

void mul4_const(kp *xq, const kp *xp,
        const uint16 a, const uint16 b, const uint16 c, const uint16 d);
void sqr4(kp *xq, const kp *xp);

void cswap(fe1271 *x, fe1271 *y, int b);
void kpcswap(kp *x, kp *y, int b);
void ladder(kp *xp, kp *xq, const kp *xd, const unsigned char *n, const int l);
void ladder_base(kp *xp, const unsigned char *n, const int l);
void xDBLADD(kp *xp, kp *xq, const kp *xd);

void xUNWRAP(kp *xp, const kp *xpw);
void xWRAP(kp *xpw, const kp *xp);

#endif
