#ifndef GFE_H
#define GFE_H

#include <inttypes.h>

typedef uint16_t uint16;
typedef struct
{
    unsigned char b[16];
}
fe1271;

void fe1271_mul(fe1271 *r, const fe1271 *x, const fe1271 *y);
void fe1271_square(fe1271 *r, const fe1271 *x);
void fe1271_invert(fe1271 *r, const fe1271 *x);
void fe1271_powminhalf(fe1271 *r, const fe1271 *x);
int fe1271_iszero(fe1271 *r);
void fe1271_setzero(fe1271 *r);
void fe1271_pack(unsigned char *r, const fe1271 *x);
void fe1271_copy(fe1271 *r, const fe1271 *x);

void fe1271_mulconst(fe1271 *r, const fe1271 *x, const unsigned char cst[2]);
void fe1271_add(fe1271 *r, const fe1271 *x, const fe1271 *y);
void fe1271_sub(fe1271 *r, const fe1271 *x, const fe1271 *y);
void fe1271_hdmrd(fe1271 *r, const fe1271 *x);
void fe1271_neg(fe1271 *r, const fe1271 *x);
void fe1271_freeze(fe1271 *r, const fe1271 *x);

void bigint_mul(unsigned char *r, const unsigned char *x, const unsigned char *y);
void bigint_red(unsigned char *r, const unsigned char *x);

#endif
