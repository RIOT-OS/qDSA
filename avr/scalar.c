#include "scalar.h"

static void big_neg(unsigned char *r, const unsigned char *x);
static void big_add(unsigned char *r, const unsigned char *x, 
        const unsigned char *y, unsigned char os);
static void big_mul(unsigned char *r, const unsigned char *x, const unsigned char *y);
static void big_red(unsigned char *res, const unsigned char *x);

static const unsigned char l6[32] = {
    0x40, 0x6f, 0x01, 0x03, 0xe1, 0xd2, 0xc2, 0xdc, 
    0xd1, 0x3f, 0x55, 0x68, 0x7e, 0xf2, 0x9f, 0xb0, 
    0x34, 0xa5, 0xd4, 0x31, 0x08, 0xa6, 0x35, 0xcd, 
    0,0,0,0,0,0,0,0};

static const unsigned char l[32] = {
    0xbd, 0x05, 0x0c, 0x84, 0x4b, 0x0b, 0x73, 0x47, 
    0xff, 0x54, 0xa1, 0xf9, 0xc9, 0x7f, 0xc2, 0xd2, 
    0x94, 0x52, 0xc7, 0x20, 0x98, 0xd6, 0x34, 0x03, 
    0,0,0,0,0,0,0,0};

static const unsigned char N[32] = {
    0x43, 0xFA, 0xF3, 0x7B, 0xB4, 0xF4, 0x8C, 0xB8,
    0x0,  0xAB, 0x5E, 0x6,  0x36, 0x80, 0x3D, 0x2D,
    0x6B, 0xAD, 0x38, 0xDF, 0x67, 0x29, 0xCB, 0xFC,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3};

void group_scalar_get32(unsigned char *r, const unsigned char x[32])
{
    unsigned char i;
    unsigned char t[64];
    for(i=0;i<32;i++) { t[i] = x[i]; t[i+32] = 0; }
    big_red(r, t);
}

void group_scalar_get64(unsigned char *r, const unsigned char x[64])
{
    unsigned char i;
    unsigned char t[64];
    for(i=0;i<64;i++) { t[i] = x[i]; }
    big_red(r, t);
}

void group_scalar_set_pos(unsigned char *r, const unsigned char *x)
{
    if ( x[0] & 1 ) { big_neg(r, x); }
}

void group_scalar_ops(unsigned char *s, const unsigned char *r,
        const unsigned char *h, const unsigned char *d)
{
    unsigned char i;
    unsigned char t[64];

    big_mul(t, h, d);
    big_red(s, t);
    for(i=0;i<32;i++) { t[i+32] = 0; }
    big_neg(t, s);
    big_add(t, t, r, 0);
    big_red(s, t);
}

static void big_neg(unsigned char *r, const unsigned char *x)
{
    unsigned char i;
    uint16 t0,t1;
    uint16 carry = 0;

    for(i=0;i<32;i++)
    {
        t0 = (uint16)N[i];
        t1 = (uint16)x[i];

        t1 += carry;
        t0 -= t1;
        carry = (t0 >> 8) & 1;
        r[i] = (unsigned char)t0;
    }
}

static void big_add(unsigned char *r, const unsigned char *x, 
        const unsigned char *y, unsigned char os)
{
    unsigned char i;
    uint16 t0,t1;
    uint16 carry = 0;

    for(i=0;i<os;i++) { r[i] = x[i]; }
    for(i=0;i<32;i++)
    {
        t0 = (uint16)x[i+os];
        t1 = (uint16)y[i];

        t0 += carry;
        t0 += t1;
        carry = (t0 >> 8) & 1;
        r[i+os] = (unsigned char)t0;
    }
    for(i=32+os;i<64;i++)
    {
        t0 = (uint16)x[i];
        t0 += carry;
        carry = (t0 >> 8) & 1;
        r[i] = (unsigned char)t0;
    }
}

static void big_mul(unsigned char *r, const unsigned char *x, const unsigned char *y)
{
    unsigned char i;
    unsigned char temp[32];

    for(i=32;i<64;i++) { r[i] = 0; }
    bigint_mul(r, x, y);
    bigint_mul(temp, x, y+16);
    big_add(r, r, temp, 16);
    bigint_mul(temp, x+16, y);
    big_add(r, r, temp, 16);
    bigint_mul(temp, x+16, y+16);
    big_add(r, r, temp, 32);
}

static void big_red(unsigned char *res, const unsigned char *x)
{
    unsigned char i,j;
    unsigned char r[64], temp[64];

    for(i=0;i<64;i++) { r[i] = x[i]; }
    for(i=0;i<4;i++)
    {
        big_mul(temp, r+32, l6);
        for(j=32;j<64;j++) { r[j] = temp[j]; }
        big_add(r, r, temp, 0);
    }

    r[33] = (r[32] & 0x1c) >> 2;
    r[32] = r[32] << 6;
    r[32] |= (r[31] & 0xfc) >> 2;
    r[31] &= 0x03;
    big_mul(temp, r+32, l);
    for(i=32;i<64;i++) { r[i] = temp[i]; }
    big_add(r, r, temp, 0);

    r[33] = 0;
    r[32] = (r[31] & 0x04) >> 2;
    r[31] &= 0x03;
    big_mul(temp, r+32, l);
    r[32] = 0;
    big_add(r, r, temp, 0);

    for(i=0;i<32;i++) { res[i] = r[i]; }
}
