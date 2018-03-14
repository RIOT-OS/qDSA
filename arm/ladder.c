#include "ladder.h"

static void mul4(kp *xq, const kp *xp);

// Wrapped base point
static const kp bpw = {
    .Y = { .v = {0x4e931a48, 0xaeb351a6, 0x2049c2e7, 0x1be0c3dc} },
    .Z = { .v = {0xe07e36df, 0x64659818, 0x8eaba630, 0x23b416cd} },
    .T = { .v = {0x7215441e, 0xc7ae3d05, 0x4447a24d, 0x5db35c38} }
};

static const uint16 e_1 = 0x72;
static const uint16 e_2 = 0x39;
static const uint16 e_3 = 0x42;
static const uint16 e_4 = 0x1A2;

const uint16 ehat_1 = 0x341;
const uint16 ehat_2 = 0x9C3;
const uint16 ehat_3 = 0x651;
const uint16 ehat_4 = 0x231;

const unsigned char mu_1 = 0x0b;
const unsigned char mu_2 = 0x16;
const unsigned char mu_3 = 0x13;
const unsigned char mu_4 = 0x03;

void cswap(fe1271 *x, fe1271 *y, int b)
{
    int i;
    uint32 db = -b;
    uint32 t;
    for(i=0;i<4;i++)
    {
        t = x->v[i] ^ y->v[i];
        t &= db;
        x->v[i] ^= t;
        y->v[i] ^= t;
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
    xp->X.b[0] = mu_1;
    xp->Y.b[0] = mu_2;
    xp->Z.b[0] = mu_3;
    xp->T.b[0] = mu_4;

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
        const uint16 a, const uint16 b, const uint16 c, const uint16 d)
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

static void mul4(kp *xq, const kp *xp)
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
