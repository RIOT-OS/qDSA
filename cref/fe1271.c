#include "fe1271.h"
#include "test/print.h"

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
    unsigned char temp[32];

    bigint_mul(temp, x->b, y->b);
    bigint_red(r->b, temp);
}

void fe1271_square(fe1271 *r, const fe1271 *x)
{
    unsigned char temp[32];

    bigint_mul(temp, x->b, x->b);
    bigint_red(r->b, temp);
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
    int i;

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

void fe1271_mulconst(fe1271 *r, const fe1271 *x, const unsigned char cst[2])
{
    int i;
    fe1271 temp;

    temp.b[0] = cst[0];
    temp.b[1] = cst[1];
    for(i=2; i<=15; i++) { temp.b[i] = 0; }
    fe1271_mul(r, x, &temp);
}

void fe1271_hdmrd(fe1271 *r, const fe1271 *x)
{
  fe1271 a,b,c,d;

  fe1271_add(&c,x,  x+1);
  fe1271_sub(&a,x+1,x);
  fe1271_add(&b,x+2,x+3);
  fe1271_sub(&d,x+2,x+3);

  fe1271_add(r  ,&a,&b);
  fe1271_sub(r+1,&a,&b);
  fe1271_add(r+3,&c,&d);
  fe1271_sub(r+2,&d,&c);
}

void fe1271_neg(fe1271 *r, const fe1271 *x)
{
    fe1271 zero; 
    int i;

    for(i=0; i<=15; i++) { zero.b[i] = 0; }
    fe1271_sub(r, &zero, x);
}

void fe1271_freeze(fe1271 *r, const fe1271 *x)
{
    int i;
    uint16 c, t;

    c = (x->b[15] >> 7); // take top bit of x
    r->b[15] = x->b[15] & 0x7f; // set top bit to 0 (stored in c)

    for(i=0; i<=14; i++)
    {
        t = (uint16)(x->b[i]) + c; 
        c = (t >> 8) & 1; 
        r->b[i] = (unsigned char)t;
    }

    r->b[15] += (unsigned char)c;

    r->b[0] += (r->b[15] >> 7);
    r->b[15] &= 0x7f;
}

void fe1271_add(fe1271 *r, const fe1271 *x, const fe1271 *y)
{
    unsigned char i, c; 
    uint16 t0, t1, t2; 

    c = 0;

    // add
    for(i = 0; i<=15; i++)
    {
        t0 = (uint16)(x->b[i]); 
        t1 = (uint16)(y->b[i]); 

        t2 = t0 + t1;
        t2 += c;
        c = (t2 >> 8) & 1; 
        r->b[i] = (unsigned char)t2;
    }

    // reduce
    c = 2*c;

    for(i = 0; i<=15; i++)
    {
        t0 = (uint16)r->b[i]; 
        t0 += c;
        c = (t0 >> 8) & 1; 
        r->b[i] = (unsigned char)t0;
    }
}

void fe1271_sub(fe1271 *r, const fe1271 *x, const fe1271 *y)
{
    unsigned char i; 
    unsigned char c;
    uint16 t0, t1; 

    c = 0;

    // sub
    for(i = 0; i<=15; i++)
    {

        t0 = (uint16)(x->b[i]); // add carry
        t1 = (uint16)(y->b[i]); 

        t1 += c;
        t0 -= t1;
        c = (t0 >> 8) & 1;
        r->b[i] = (unsigned char)t0;
    }

    // reduce
    c = 2*c;

    for(i = 0; i<=15; i++)
    {
        t0 = (uint16)(r->b[i]); 
        t0 -= c;
        c = (t0 >> 8) & 1;
        r->b[i] = (unsigned char)t0;
    }
}

// multiply two 128 bit numbers into a 256 bit number
void bigint_mul(unsigned char *r, const unsigned char *x, const unsigned char *y)
{
    int i,j;
    uint16 t0,t1,mul;
    uint16 res[32];

    for(i=0; i<=31; i++)
    {
        res[i] = 0;
    }

    for(i=0; i<=15; i++)
    {
        for(j=0; j<=15; j++)
        {
            t0 = (uint16)(x[i]);   
            t1 = (uint16)(y[j]);   
            mul = t0*t1;

            res[i+j] += mul & 0xff; // add low part of mult to result
            res[i+j+1] += (mul >> 8) & 0xff; // add high part of mult to result
        }
    }

    // reduce the whole result to radix 2^8
    for(i=0; i<=30; i++)
    {
        res[i+1] += res[i] >> 8;
        r[i] = (uint16)res[i];
    }
    r[31] = (uint16)res[31];
}

// reduce a 256 bit number to a 128 bit one
void bigint_red(unsigned char *r, const unsigned char *x)
{
    int i;
    uint16 res[16];

    for(i=0; i<=15; i++)
    {
        res[i] = (uint16)x[i];
        res[i] += 2*x[i+16];
    }

    // reduce the whole thing to radix 2^8
    // might need two iterations
    for(i=0; i<=14; i++)
    {
        res[i+1] += (res[i] >> 8);
        res[i] &= 0x000000ff;
    }

    res[0] += 2*(res[15]>>8); // take top bits
    res[15] &= 0x000000ff; // set top bits to 0

    for(i=0; i<=14; i++)
    {
        res[i+1] += (res[i] >> 8);
        r[i] = (unsigned char)res[i];
    }

    r[15] = (unsigned char)res[15];
}
