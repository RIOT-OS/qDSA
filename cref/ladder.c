#include "ladder.h"

static void mul4(kp *xq, const kp *xp);

// Wrapped base point
static const kp bpw = {  
    .Y = { .b = { 0x48, 0x1a, 0x93, 0x4e, 0xa6, 0x51, 0xb3, 0xae, 
                  0xe7, 0xc2, 0x49, 0x20, 0xdc, 0xc3, 0xe0, 0x1b } },
    .Z = { .b = { 0xdf, 0x36, 0x7e, 0xe0, 0x18, 0x98, 0x65, 0x64, 
                  0x30, 0xa6, 0xab, 0x8e, 0xcd, 0x16, 0xb4, 0x23 } },
    .T = { .b = { 0x1e, 0x44, 0x15, 0x72, 0x05, 0x3d, 0xae, 0xc7, 
                  0x4d, 0xa2, 0x47, 0x44, 0x38, 0x5c, 0xb3, 0x5d } }
};

static const unsigned char e_1[2] = {0x72, 0x00};
static const unsigned char e_2[2] = {0x39, 0x00};
static const unsigned char e_3[2] = {0x42, 0x00};
static const unsigned char e_4[2] = {0xA2, 0x01};

const unsigned char ehat_1[2] = {0x41, 0x03};
const unsigned char ehat_2[2] = {0xC3, 0x09};
const unsigned char ehat_3[2] = {0x51, 0x06};
const unsigned char ehat_4[2] = {0x31, 0x02};

const unsigned char mu_1[2] = {0x0b, 0x00};
const unsigned char mu_2[2] = {0x16, 0x00};
const unsigned char mu_3[2] = {0x13, 0x00};
const unsigned char mu_4[2] = {0x03, 0x00};

void cswap(fe1271 *x, fe1271 *y, int b)
{
    unsigned char i;
    unsigned char db = -b;
    unsigned char t;
    for(i=0;i<16;i++)
    {
        t = x->b[i] ^ y->b[i];
        t &= db;
        x->b[i] ^= t;
        y->b[i] ^= t;
    }
}

void kpcswap(kp *x, kp *y, int b)
{
    cswap(&x->X, &y->X, b);
    cswap(&x->Y, &y->Y, b);
    cswap(&x->Z, &y->Z, b);
    cswap(&x->T, &y->T, b);
}

void ladder(kp *xp, kp *xq, const kp *xd, const unsigned char *n, const int l)
{
    /*
     * Montgomery ladder computing n*xq via repeated
     * differential additions and constant-time
     * conditional swaps.
     *
     * Input: 
     *      xq: Uncompressed Kummer point
     *      xd: Wrapped Kummer point xq
     *      n: Scalar
     *      l: Maximum scalar bit-length
     *
     * Output: 
     *      xp: n*xq
     *      xq: (n+1)*xq
     */

    int i, swap;
    int bit = 0;
    int prevbit = 0;

    fe1271_setzero(&xp->X);
    fe1271_setzero(&xp->Y);
    fe1271_setzero(&xp->Z);
    fe1271_setzero(&xp->T);
    xp->X.b[0] = mu_1[0];
    xp->Y.b[0] = mu_2[0];
    xp->Z.b[0] = mu_3[0];
    xp->T.b[0] = mu_4[0];

    for(i=l;i>=0;i--)
    {
        bit = (n[i>>3]>>(i&0x07)) & 1;
        swap = bit ^ prevbit;
        prevbit = bit;

        fe1271_neg(&xq->X, &xq->X);
        kpcswap(xp, xq, swap);
        xDBLADD(xp, xq, xd);
    }
    fe1271_neg(&xp->X, &xp->X);
    kpcswap(xp, xq, bit);
}

void ladder_base(kp *xp, const unsigned char *n, const int l)
{
    kp xq;

    xUNWRAP(&xq, &bpw);
    ladder(xp, &xq, &bpw, n, l);
}

void mul4_const(kp *xq, const kp *xp,
    const unsigned char a[2], const unsigned char b[2],
    const unsigned char c[2], const unsigned char d[2])
{
    /*
     * Pairwise multiply two tuples, where
     * the second tuple has small values
     *
     * Input: 
     *      xp: Four fe1271 elements (X,Y,Z,T)
     *      (a,b,c,d): Four small (< 16 bits) fe1271 elements
     *
     * Output: 
     *      xq: (a*X, b*Y, c*Z, d*T)
     */

    fe1271_mulconst(&xq->X, &xp->X, a);
    fe1271_mulconst(&xq->Y, &xp->Y, b);
    fe1271_mulconst(&xq->Z, &xp->Z, c);
    fe1271_mulconst(&xq->T, &xp->T, d);
}

void mul4(kp *xq, const kp *xp)
{
    /*
     * Pairwise multiply two tuples
     *
     * Input: 
     *      xq: Four fe1271 elements (X1,Y1,Z1,T1)
     *      xp: Four fe1271 elements (X2,Y2,Z2,T2)
     *
     * Output: 
     *      xq: (X1*X2, Y1*Y2, Z1*Z2, T1*T2)
     */

    fe1271_mul(&xq->X, &xq->X, &xp->X);
    fe1271_mul(&xq->Y, &xq->Y, &xp->Y);
    fe1271_mul(&xq->Z, &xq->Z, &xp->Z);
    fe1271_mul(&xq->T, &xq->T, &xp->T);
}

void sqr4(kp *xq, const kp *xp)
{
    /*
     * Pairwise square a tuple
     *
     * Input: 
     *      xp: Four fe1271 elements (X,Y,Z,T)
     *
     * Output: 
     *      xq: (X^2,Y^2,Z^2,T^2)
     */

    fe1271_square(&xq->X, &xp->X);
    fe1271_square(&xq->Y, &xp->Y);
    fe1271_square(&xq->Z, &xp->Z);
    fe1271_square(&xq->T, &xp->T);
}

void xDBLADD(kp *xp, kp *xq, const kp *xd)
{
    /*
     * Simultaneous xDBL and xADD operation on
     * the Kummer. To deal with negated constants, 
     * it assume the first coordinates of xp, xq 
     * are negated. The first output coordinate 
     * of xp will be negated.
     *
     * Input: 
     *      xp: Uncompressed Kummer point
     *      xq: Uncompressed Kummer point
     *      xd: Wrapped difference Kummer point (xp-xq)
     *
     * Output: 
     *      xp: Uncompressed Kummer point 2*xp
     *      xq: Uncompressed Kummer point xp+xq
     */

    fe1271_hdmrd(&xq->X, &xq->X);
    fe1271_hdmrd(&xp->X, &xp->X);

    mul4(xq, xp);
    sqr4(xp, xp);
    mul4_const(xq, xq, ehat_1, ehat_2, ehat_3, ehat_4);
    mul4_const(xp, xp, ehat_1, ehat_2, ehat_3, ehat_4);

    fe1271_hdmrd(&xq->X, &xq->X);
    fe1271_hdmrd(&xp->X, &xp->X);

    sqr4(xq, xq);
    sqr4(xp, xp);

    fe1271_mul(&xq->Y, &xq->Y, &xd->Y);
    fe1271_mul(&xq->Z, &xq->Z, &xd->Z);
    fe1271_mul(&xq->T, &xq->T, &xd->T);

    mul4_const(xp, xp, e_1, e_2, e_3, e_4);
}

void xUNWRAP(kp *xp, const kp *xpw)
{
    /*
     * Unwrap a wrapped Kummer point
     *
     * Input: 
     *      xpw: Wrapped Kummer point (X/Y,X/Z,X/T)
     *
     * Output: 
     *      xp: Uncompressed Kummer point (X:Y:Z:T)
     */

    fe1271_mul(&xp->T, &xpw->Y, &xpw->Z);
    fe1271_mul(&xp->Z, &xpw->Y, &xpw->T);
    fe1271_mul(&xp->Y, &xpw->Z, &xpw->T);
    fe1271_mul(&xp->X, &xp->T, &xpw->T);
}

void xWRAP(kp *xpw, const kp *xp)
{
    /*
     * Wrap an uncompressed Kummer point
     *
     * Input: 
     *      xp: Uncompressed Kummer point (X:Y:Z:T)
     *
     * Output: 
     *      xpw: Wrapped Kummer point (X/Y,X/Z,X/T)
     */

    fe1271 w0,w1,w2,w3;

    fe1271_mul(&w0, &xp->Y, &xp->Z);
    fe1271_mul(&w1, &w0, &xp->T);
    fe1271_invert(&w2, &w1);
    fe1271_mul(&w2, &w2, &xp->X);
    fe1271_mul(&w3, &w2, &xp->T);
    fe1271_mul(&xpw->Y, &w3, &xp->Z);
    fe1271_mul(&xpw->Z, &w3, &xp->Y);
    fe1271_mul(&xpw->T, &w0, &w2);
}
