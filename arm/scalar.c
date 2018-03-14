#include "scalar.h"

static void big_neg(uint32 *r, const uint32 *x);
static void big_add(uint32 *r, const uint32 *x, const uint32 *y, unsigned char os);
static void big_mul(uint32 *r, const uint32 *x, const uint32 *y);
static void big_red(uint32 *res, const uint32 *x);

static const uint32 l[8] = {0x840C05BD, 0x47730B4B, 0xF9A154FF, 0xD2C27FC9, 0x20C75294, 0x334D698, 0x0, 0x0};
static const uint32 l6[8] = {0x3016F40, 0xDCC2D2E1, 0x68553FD1, 0xB09FF27E, 0x31D4A534, 0xCD35A608, 0x0, 0x0};
static const uint32 N[8] = {0x7BF3FA43, 0xB88CF4B4, 0x65EAB00, 0x2D3D8036, 0xDF38AD6B, 0xFCCB2967, 0xFFFFFFFF, 0x3FFFFFF};

void group_scalar_get32(uint32 *r, const unsigned char x[32])
{
    unsigned char i;
    bigint t;
    for(i=0;i<32;i++) { t.b[i] = x[i]; t.b[i+32] = 0; }
    big_red(r, t.v);
}

void group_scalar_get64(uint32 *r, const unsigned char x[64])
{
    unsigned char i;
    bigint t;
    for(i=0;i<64;i++) { t.b[i] = x[i]; }
    big_red(r, t.v);
}

void group_scalar_ops(uint32 *s, const group_scalar *r, 
        const uint32 *h, const uint32 *d)
{
    int i;
    uint32 t[16];

    big_mul(t, h, d);
    big_red(s, t);
    for(i=0;i<8;i++) t[i+8] = 0;
    big_neg(t, s);
    big_add(t, t, r->fe.l1.v, 0);
    big_red(s, t);
}

static void big_neg(uint32 *r, const uint32 *x)
{
    unsigned char i;
    uint64 t0, t1;
    uint32 carry = 0;

    for(i=0; i<=7; i++)
    {
        t0 = (uint64)N[i];
        t1 = (uint64)x[i];

        t1 += carry;
        t0 -= t1;
        carry = (t0 >> 32) & 1;
        r[i] = (uint32)t0;
    }
}

// 512x256 big integer addition, possibly starting at an offset
static void big_add(uint32 *r, const uint32 *x, const uint32 *y, unsigned char os)
{
    unsigned char i;
    uint64 t0,t1;
    uint32 carry = 0;

    for(i=0; i<=os-1;i++) { r[i] = x[i]; }
    for(i=0; i<=7; i++)
    {
        t0 = (uint64)x[i+os];
        t1 = (uint64)y[i];

        t0 += carry;
        t0 += t1;
        carry = (t0 >> 32) & 1;
        r[i+os] = (uint32)t0;
    }

    for(i=8+os; i<=15; i++)
    {
        t0 = (uint64)x[i];
        t0 += carry;
        carry = (t0 >> 32) & 1;
        r[i] = (uint32)t0;
    }
}

// 256x256 bit integer multiplication
static void big_mul(uint32 *r, const uint32 *x, const uint32 *y)
{
    unsigned char i;
    uint32 temp[8];

    for(i=8; i<=15; i++) { r[i] = 0; }
    bigint_mul(r, x, y);
    bigint_mul(temp, x, y+4);
    big_add(r, r, temp, 4);
    bigint_mul(temp, x+4, y);
    big_add(r, r, temp, 4);
    bigint_mul(temp, x+4, y+4);
    big_add(r, r, temp, 8);
}

// 512 -> 250-bit reduction modulo N
static void big_red(uint32 *res, const uint32 *x)
{
    unsigned char i,j;
    uint32 r[16], temp[16];

    for(i=0; i<=15; i++) { r[i] = x[i]; }

    for(i=0; i<=3; i++)
    {
        big_mul(temp, r+8, l6);
        for(j=8; j<=15; j++) { r[j] = temp[j]; }
        big_add(r, r, temp, 0);
    }

    r[8] = (r[8]<<6) | ((r[7]&0xfc000000)>>26);
    r[7] &= 0x03ffffff;
    big_mul(temp, r+8, l);
    for(i=8; i<=15; i++) { r[i] = temp[i]; }
    big_add(r, r, temp, 0);

    r[8] = (r[7]&0x04000000)>>26;
    r[7] &= 0x03ffffff;
    big_mul(temp, r+8, l);
    r[8] = 0;
    big_add(r, r, temp, 0);

    for(i=0; i<8; i++) { res[i] = r[i]; }
}
