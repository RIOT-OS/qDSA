#ifndef FE1271_H
#define FE1271_H

#include <inttypes.h>

typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

union state { unsigned char u8[200]; uint64 u64[25]; };

typedef union 
{
    unsigned char b[16];
    uint32 v[4];
}
fe1271;

void fe1271_mul(fe1271 *r, const fe1271 *x, const fe1271 *y);
void fe1271_square(fe1271 *r, const fe1271 *x);
void fe1271_invert(fe1271 *r, const fe1271 *x);
void fe1271_powminhalf(fe1271 *r, const fe1271 *x);
int fe1271_iszero(fe1271 *r);
void fe1271_setzero(fe1271 *r);
void fe1271_copy(fe1271 *r, const fe1271 *x);

extern void fe1271_mulconst(fe1271 *r, const fe1271 *x, const uint16 y);
extern void fe1271_add(fe1271 *r, const fe1271 *x, const fe1271 *y);
extern void fe1271_sub(fe1271 *r, const fe1271 *x, const fe1271 *y);
extern void fe1271_hdmrd(fe1271 *r, const fe1271 *x);
extern void fe1271_neg(fe1271 *r, const fe1271 *x);
extern void fe1271_freeze(fe1271 *r, const fe1271 *x);

extern void bigint_sqr(uint32 *r, const uint32 *x);
extern void bigint_mul(uint32 *r, const uint32 *x, const uint32 *y);
extern void bigint_red(uint32 *r, const uint32 *a);

#endif
