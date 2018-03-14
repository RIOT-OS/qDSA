#include "quad.h"

static void fe1271_H(fe1271 *r, fe1271 *x);
static void dot(fe1271 *r,
        const fe1271 *x0, const fe1271 *x1, const fe1271 *x2, const fe1271 *x3,
        const fe1271 *y0, const fe1271 *y1, const fe1271 *y2, const fe1271 *y3);
static void dot_const(fe1271 *r,
        const fe1271 *x0, const fe1271 *x1, const fe1271 *x2, const fe1271 *x3,
        const unsigned char k1[2], const unsigned char k2[2],
        const unsigned char k3[2], const unsigned char k4[2]);

static void biiValues(kp *r, kp *t0, kp *t1, const kp *sP, const kp *hQ);

static void fe1271_sum(fe1271 *r, fe1271 *t, 
        const unsigned char *c1, const unsigned char *c2,
        const unsigned char *c3, const unsigned char *c4);
static void bijValue(fe1271 *r, kp *t, 
        const fe1271 *P1, const fe1271 *P2, const fe1271 *P3, const fe1271 *P4, 
        const fe1271 *Q1, const fe1271 *Q2, const fe1271 *Q3, const fe1271 *Q4,
        const unsigned char *c1, const unsigned char *c2, 
        const unsigned char *c3, const unsigned char *c4);

static int quad(fe1271 *Bij, kp *t, const fe1271 *Bjj, const fe1271 *Bii, 
        const fe1271 *R1, const fe1271 *R2);

static const unsigned char k1[2] = {0x59, 0x12};
static const unsigned char k2[2] = {0x3F, 0x17};
static const unsigned char k3[2] = {0x79, 0x16};
static const unsigned char k4[2] = {0xC7, 0x07};

static const unsigned char muhat_1[2] = {0x21, 0x00};
static const unsigned char muhat_2[2] = {0x0B, 0x00};
static const unsigned char muhat_3[2] = {0x11, 0x00};
static const unsigned char muhat_4[2] = {0x31, 0x00};

static const fe1271 C = { .b = { 0x43, 0xA8, 0xDD, 0xCD,
                                  0xD8, 0xE3, 0xF7, 0x46,
                                  0xDD, 0xA2, 0x20, 0xA3,
                                  0xEF, 0x0E, 0xF5, 0x40} };

static void fe1271_H(fe1271 *r, fe1271 *x)
{
    /*
     * Compute the Hadamard transform on four fe1271
     * elements
     *
     * Input: 
     *      (x,x+1,x+2,x+3): Four fe1271 elements
     *
     * Output: 
     *      r  : x + (x+1) + (x+2) + (x+3)
     *      r+1: x + (x+1) - (x+2) - (x+3)
     *      r+2: x - (x+1) + (x+2) - (x+3)
     *      r+3: x - (x+1) - (x+2) + (x+3)
     */

    fe1271_neg(x, x);
    fe1271_hdmrd(r, x);
    fe1271_neg(r+3, r+3);
}

static void dot(fe1271 *r, 
        const fe1271 *x0, const fe1271 *x1, const fe1271 *x2, const fe1271 *x3,
        const fe1271 *y0, const fe1271 *y1, const fe1271 *y2, const fe1271 *y3)
{
    /*
     * Compute the dot product of two tuples
     *
     * Input: 
     *      (x0,x1,x2,x3): Four fe1271 elements
     *      (y0,y1,y2,y3): Four fe1271 elements
     *
     * Output: 
     *      r: x0*y0 + x1*y1 + x2*y2 + x3*y3
     */

    fe1271 t;

    fe1271_mul(r, x0, y0);
    fe1271_mul(&t, x1, y1);
    fe1271_add(r, r, &t);
    fe1271_mul(&t, x2, y2);
    fe1271_add(r, r, &t);
    fe1271_mul(&t, x3, y3);
    fe1271_add(r, r, &t);
}

static void dot_const(fe1271 *r,
        const fe1271 *x0, const fe1271 *x1, const fe1271 *x2, const fe1271 *x3,
        const unsigned char k1[2], const unsigned char k2[2],
        const unsigned char k3[2], const unsigned char k4[2])
{
    /*
     * Compute the dot product of two tuples, where
     * the second tuple has small values and some 
     * are negated
     *
     * Input: 
     *      (x0,x1,x2,x3): Four fe1271 elements
     *      (k1,k2,k3,k4): Four small (< 16 bits) fe1271 elements
     *
     * Output: 
     *      r: x0*k1 - x1*k2 - x2*k3 + x3*k4
     */

    fe1271 t;

    fe1271_mulconst(r, x0, k1);
    fe1271_mulconst(&t, x1, k2);
    fe1271_sub(r, r, &t);
    fe1271_mulconst(&t, x2, k3);
    fe1271_sub(r, r, &t);
    fe1271_mulconst(&t, x3, k4);
    fe1271_add(r, r, &t);
}

static void biiValues(kp *r, kp *t0, kp *t1, const kp *sP, const kp *hQ)
{
    /*
     * Four quadratic forms B_{ii} on the Kummer, where
     * 1 <= i <= 4.
     *
     * Input: 
     *      sP: Uncompressed point on the Kummer
     *      hQ: Uncompressed point on the Kummer
     *
     * Output: 
     *      r: ( B_{11}, B_{22}, B_{33}, B_{44} )
     */

    sqr4(t0, sP);
    sqr4(r, hQ);

    mul4_const(t0, t0, ehat_1, ehat_2, ehat_3, ehat_4);
    mul4_const(r, r, ehat_1, ehat_2, ehat_3, ehat_4);

    fe1271_neg(&t0->X, &t0->X);
    fe1271_neg(&r->X, &r->X);

    dot(&t1->X, &t0->X, &t0->Y, &t0->Z, &t0->T, &r->X, &r->Y, &r->Z, &r->T);
    dot(&t1->Y, &t0->X, &t0->Y, &t0->Z, &t0->T, &r->Y, &r->X, &r->T, &r->Z);
    dot(&t1->Z, &t0->X, &t0->Z, &t0->Y, &t0->T, &r->Z, &r->X, &r->T, &r->Y);
    dot(&t1->T, &t0->X, &t0->T, &t0->Y, &t0->Z, &r->T, &r->X, &r->Z, &r->Y);

    dot_const(&r->X, &t1->X, &t1->Y, &t1->Z, &t1->T, k1, k2, k3, k4);
    dot_const(&r->Y, &t1->Y, &t1->X, &t1->T, &t1->Z, k1, k2, k3, k4);
    dot_const(&r->Z, &t1->Z, &t1->T, &t1->X, &t1->Y, k1, k2, k3, k4);
    dot_const(&r->T, &t1->T, &t1->Z, &t1->Y, &t1->X, k1, k2, k3, k4);

    mul4_const(r, r, muhat_1, muhat_2, muhat_3, muhat_4);
    fe1271_neg(&r->X, &r->X);
}

static void fe1271_sum(fe1271 *r, fe1271 *t, 
        const unsigned char *c1, const unsigned char *c2,
        const unsigned char *c3, const unsigned char *c4)
{
    set_const(t, c1);
    fe1271_mulconst(t, t, c2);
    set_const(r, c3);
    fe1271_mulconst(r, r, c4);
    fe1271_add(r, r, t);
}

static void bijValue(fe1271 *r, kp *t, 
        const fe1271 *P1, const fe1271 *P2, const fe1271 *P3, const fe1271 *P4, 
        const fe1271 *Q1, const fe1271 *Q2, const fe1271 *Q3, const fe1271 *Q4,
        const unsigned char *c1, const unsigned char *c2, 
        const unsigned char *c3, const unsigned char *c4)
{
    /*
     * Quadratic form B_{ij} on the Kummer, where
     * (P1,P2,P3,P4), (Q1,Q2,Q3,Q4) are some permutation of
     * the coordinates of two Kummer points P and Q,
     * and (c1,c2,c3,c4) is a permutation of curve constants
     * depending of the choice of {i,j}
     *
     * Input: 
     *      (P1,P2,P3,P4): Permutation of coordinates of P
     *      (Q1,Q2,Q3,Q4): Permutation of coordinates of Q
     *      (c1,c2,c3,c4): Permutation of curve constants
     *
     * Output: 
     *      r: B_{ij} 
     */

    fe1271_mul(r, P1, P2);
    fe1271_mul(&t->X, Q1, Q2);
    fe1271_mul(&t->Y, P3, P4);
    fe1271_sub(r, r, &t->Y);
    fe1271_mul(&t->Z, Q3, Q4);
    fe1271_sub(&t->X, &t->X, &t->Z);
    fe1271_mul(r, r, &t->X);
    fe1271_mul(&t->X, &t->Y, &t->Z);

    fe1271_mulconst(r, r, c3);
    fe1271_mulconst(r, r, c4);
    fe1271_sum(&t->Y, &t->Z, c3, c4, c1, c2);
    fe1271_mul(&t->X, &t->X, &t->Y);
    fe1271_sub(r, &t->X, r);

    fe1271_mulconst(r, r, c1);
    fe1271_mulconst(r, r, c2);
    fe1271_sum(&t->Y, &t->Z, c2, c4, c1, c3);
    fe1271_mul(r, r, &t->Y);
    fe1271_sum(&t->Y, &t->Z, c2, c3, c1, c4);
    fe1271_mul(r, r, &t->Y);
}

static int quad(fe1271 *Bij, kp *t, const fe1271 *Bjj, const fe1271 *Bii, 
        const fe1271 *R1, const fe1271 *R2)
{
    /*
     * Verify whether  BjjR1^2 - 2*C*BijR1R2 + BiiR2^2 = 0
     *
     * Input: 
     *      Bij: Biquadratic form Bij
     *      Bjj: Biquadratic form Bjj
     *      Bii: Biquadratic form Bii
     *      R1: Coordinate of Kummer point R
     *      R2: Coordinate of Kummer point R
     *
     * Output: 
     *      0 if BjjR1^2 - 2*C*BijR1R2 + BiiR2^2 = 0, 
     *      1 otherwise
     */

    fe1271_square(&t->X, R1);
    fe1271_mul(&t->X, Bjj, &t->X);
    fe1271_mul(&t->Y, R1, R2);
    fe1271_mul(&t->Y, Bij, &t->Y);
    fe1271_mul(&t->Y, &C, &t->Y);
    fe1271_add(&t->Y, &t->Y, &t->Y);
    fe1271_sub(&t->X, &t->X, &t->Y);
    fe1271_square(&t->Y, R2);
    fe1271_mul(&t->Y, Bii, &t->Y);
    fe1271_add(&t->X, &t->X, &t->Y);

    return fe1271_iszero(&t->X);
}

int check(kp *sP, kp *hQ, kp *R, kp *t, kpc *xr)
{
    /*
     * Verify whether R = \pm ( sP \pm hQ ) on
     * the Kummer
     *
     * Input: 
     *      sP: Uncompressed point on the Kummer
     *      hQ: Uncompressed point on the Kummer
     *      xr: Compression of Kummer point R
     *
     * Output: 
     *      0 if R = \pm ( sP \pm hQ ), 1 otherwise
     */

    kp Bii;
    fe1271 Bij;
    int v = 0;

    fe1271_H(&sP->X, &sP->X);
    fe1271_H(&hQ->X, &hQ->X);

    biiValues(&Bii, t, R, sP, hQ);
    if (decompress(R, t, xr)) { return 1; }
    fe1271_H(&R->X, &R->X);

    // B12
    bijValue(&Bij, t, 
            &sP->X, &sP->Y, &sP->Z, &sP->T,
            &hQ->X, &hQ->Y, &hQ->Z, &hQ->T,
            muhat_1, muhat_2, muhat_3, muhat_4);
    v |= quad(&Bij, t, &Bii.Y, &Bii.X, &R->X, &R->Y);

    // B13
    bijValue(&Bij, t, 
            &sP->X, &sP->Z, &sP->Y, &sP->T,
            &hQ->X, &hQ->Z, &hQ->Y, &hQ->T,
            muhat_1, muhat_3, muhat_2, muhat_4);
    v |= quad(&Bij, t, &Bii.Z, &Bii.X, &R->X, &R->Z);

    // B14
    bijValue(&Bij, t, 
            &sP->X, &sP->T, &sP->Y, &sP->Z,
            &hQ->X, &hQ->T, &hQ->Y, &hQ->Z,
            muhat_1, muhat_4, muhat_2, muhat_3);
    v |= quad(&Bij, t, &Bii.T, &Bii.X, &R->X, &R->T);

    // B23
    bijValue(&Bij, t, 
            &sP->Y, &sP->Z, &sP->X, &sP->T,
            &hQ->Y, &hQ->Z, &hQ->X, &hQ->T,
            muhat_2, muhat_3, muhat_1, muhat_4);
    fe1271_neg(&Bij, &Bij);
    v |= quad(&Bij, t, &Bii.Z, &Bii.Y, &R->Y, &R->Z);

    // B24
    bijValue(&Bij, t, 
            &sP->Y, &sP->T, &sP->X, &sP->Z,
            &hQ->Y, &hQ->T, &hQ->X, &hQ->Z,
            muhat_2, muhat_4, muhat_1, muhat_3);
    fe1271_neg(&Bij, &Bij);
    v |= quad(&Bij, t, &Bii.T, &Bii.Y, &R->Y, &R->T);

    // B34
    bijValue(&Bij, t, 
            &sP->Z, &sP->T, &sP->X, &sP->Y,
            &hQ->Z, &hQ->T, &hQ->X, &hQ->Y,
            muhat_3, muhat_4, muhat_1, muhat_2);
    fe1271_neg(&Bij, &Bij);
    v |= quad(&Bij, t, &Bii.T, &Bii.Z, &R->Z, &R->T);

    return v;
}
