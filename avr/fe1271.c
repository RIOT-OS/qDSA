#include "fe1271.h"

// add 1 because rep of 0 is not unique, while of 1 is
int fe1271_iszero(fe1271 *r)
{
    unsigned char i;
    unsigned char t = 0;
    fe1271 one;

    one.b[0] = 1;
    for(i=1;i<16;i++) { one.b[i] = 0; }
    fe1271_add(&one, &one, r);

    fe1271_freeze(&one, &one);
    t |= (one.b[0] ^ 1);
    for(i=1;i<16;i++) { t |= one.b[i]; }

    return !( t == 0 );
}

void fe1271_pack(unsigned char *r, const fe1271 *x)
{
    unsigned char i;

    for(i=0;i<16;i++) { r[i] = x->b[i]; }
}

void fe1271_copy(fe1271 *r, const fe1271 *x)
{
    unsigned char i;
    
    for(i=0;i<16;i++) { r->b[i] = x->b[i]; }
}

void fe1271_setzero(fe1271 *r)
{
    unsigned char i;
    
    for(i=0;i<16;i++) { r->b[i] = 0; }
}

void fe1271_mul(fe1271 *r, const fe1271 *x, const fe1271 *y)
{
    unsigned char t[32];

    bigint_mul(t, x->b, y->b);
    bigint_red(r->b, t);
}

void fe1271_square(fe1271 *r, const fe1271 *x)
{
    unsigned char t[32];

    bigint_sqr(t, x->b);
    bigint_red(r->b, t);
}

void fe1271_invert(fe1271 *r, const fe1271 *x)
{
    fe1271 t;

    fe1271_square(r, x);
    fe1271_powminhalf(r, r);
    fe1271_mul(&t, r, x);
    fe1271_mul(r, r, &t);
}

void fe1271_powminhalf(fe1271 *r, const fe1271 *x)
{
   unsigned char i;

     fe1271 x2, x3, x6;

   fe1271_square(&x2, x); // 2
   fe1271_mul(&x3, &x2, x); // 3
   fe1271_square(&x6, &x3); // 6
   fe1271_square(&x6, &x6); // 12
   fe1271_mul(&x3, &x6, &x3); // 2^4-1
   fe1271_square(&x6, &x3); // 30
   fe1271_mul(&x6, &x6, x); // 2^5-1

   fe1271_square(r, &x6); // 2^6-2
   for(i=0;i<=3;i++) fe1271_square(r, r); // 2^10-2^5
   fe1271_mul(&x6, r, &x6); // 2^10-1

   fe1271_square(r, &x6); // 2^11-2
   for(i=0;i<=8;i++) fe1271_square(r, r); // 2^20-2^10
   fe1271_mul(&x6, r, &x6); // 2^20-1

   fe1271_square(r, &x6); // 2^21-2
   for(i=0;i<=18;i++) fe1271_square(r, r); // 2^40-2^20
   fe1271_mul(&x6, r, &x6); // 2^40-1

   fe1271_square(r, &x6); // 2^41-2
   for(i=0;i<=38;i++) fe1271_square(r, r); // 2^80-2^40
   fe1271_mul(r, r, &x6); // 2^80-1

   for(i=0;i<=39;i++) fe1271_square(r, r); // 2^120-2^40
   fe1271_mul(r, r, &x6); // 2^120-1

   for(i=0;i<=3;i++) fe1271_square(r, r); // 2^124-2^4

   fe1271_mul(r, r, &x3); // 2^124-1
   fe1271_square(r, r); // 2^125-2
   fe1271_mul(&x6, r, &x2); // 2^125
   fe1271_square(&x6, &x6); // 2^126
   fe1271_mul(r, r, &x6);
}
