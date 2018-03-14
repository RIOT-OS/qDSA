#include "compress.h"

static void T_row(fe1271 *r, const fe1271 *X1,
        const fe1271 *X2, const fe1271 *X3, const fe1271 *X4);
static void T(kp *r, const kp *x);

static void T_inv_row(fe1271 *r, const fe1271 *X1,
        const fe1271 *X2, const fe1271 *X3, const fe1271 *X4);
static void T_inv(kp *r, const kp *x);

static void get_k2(fe1271 *r, fe1271 *t, const fe1271 *l1, const fe1271 *l2, unsigned char tau);
static void get_k3(fe1271 *r, fe1271 *t0, fe1271 *t1, 
        const fe1271 *l1, const fe1271 *l2, unsigned char tau);
static void get_k4(fe1271 *r, fe1271 *t, const fe1271 *l1, const fe1271 *l2, unsigned char tau);
static int fe1271_hasSqrt(fe1271 *R, fe1271 *t, const fe1271 *Delta, unsigned char sigma);

static const unsigned char khat_1[2] = {0xC1, 0x03};
static const unsigned char khat_2[2] = {0x80, 0x00};
static const unsigned char khat_3[2] = {0x39, 0x02};
static const unsigned char khat_4[2] = {0x49, 0x04};

static const unsigned char q0[2] = {0xF7, 0x0D};
static const unsigned char q1[2] = {0x99, 0x25};
static const unsigned char q2[2] = {0x11, 0x12};
static const unsigned char q3[2] = {0xE3, 0x2F};
static const unsigned char q4[2] = {0x0B, 0x2C};
static const unsigned char q5[2] = {0x33, 0x1D};
static const unsigned char q6[2] = {0x79, 0x17};
static const unsigned char q7[2] = {0xD7, 0xAB};

/*
 * get k2, k3, k4
 */

void kpc_unpack(kpc *r, const unsigned char x[32])
{
    int i;
    for(i=0;i<32;i++) { r->b[i] = x[i]; }
}

void set_const(fe1271 *r, const unsigned char c[2])
{
    int i;
    r->b[0] = c[0];
    r->b[1] = c[1];
    for(i=2;i<16;i++) r->b[i] = 0;
}

static void get_k2(fe1271 *r, fe1271 *t, const fe1271 *l1, const fe1271 *l2, unsigned char tau)
{
    /*
     * Compute K_2(l1,l2,tau)
     *
     * Input: 
     *      l1: Element of fe1271
     *      l2: Element of fe1271
     *      tau: Either 0 or 1
     *
     * Output: 
     *      r: K_2(l1,l2,tau)
     */

    fe1271_mulconst(r, l1, q2);
    fe1271_mul(r, l2, r);

    if (tau) 
    {
        fe1271_mulconst(t, l1, q0);
        fe1271_add(r, r, t);
        fe1271_mulconst(t, l2, q1);
        fe1271_sub(r, r, t);
    }

    fe1271_mulconst(r, r, q3);
    fe1271_add(r, r, r);

    fe1271_mulconst(t, l1, q5);
    fe1271_square(t, t);
    fe1271_sub(r, t, r);

    fe1271_mulconst(t, l2, q3);
    fe1271_square(t, t);
    fe1271_add(r, t, r);

    if (tau)
    {
        set_const(t, q4);
        fe1271_square(t, t);
        fe1271_add(r, t, r);
    }
}

static void get_k3(fe1271 *r, fe1271 *t0, fe1271 *t1, 
        const fe1271 *l1, const fe1271 *l2, unsigned char tau)
{
    /*
     * Compute K_3(l1,l2,tau)
     *
     * Input: 
     *      l1: Element of fe1271
     *      l2: Element of fe1271
     *      tau: Either 0 or 1
     *
     * Output: 
     *      r: K_3(l1,l2,tau)
     */

    const unsigned char one[2] = {0x01, 0x00};

    fe1271_square(r, l1);
    fe1271_square(t0, l2);

    if (tau)
    {
        set_const(t1, one);
        fe1271_add(r, r, t1);
        fe1271_add(t0, t0, t1);
        fe1271_add(t1, r, t0);
    }

    fe1271_mul(r, r, l2);
    fe1271_mulconst(r, r, q0);
    fe1271_mul(t0, t0, l1);
    fe1271_mulconst(t0, t0, q1);
    fe1271_sub(r, r, t0);

    if (tau)
    {
        set_const(t0, one);
        fe1271_sub(t1, t1, t0);
        fe1271_sub(t1, t1, t0);
        fe1271_mulconst(t1, t1, q2);
        fe1271_add(r, r, t1);
    }

    fe1271_mulconst(r, r, q3);

    if (tau)
    {
        fe1271_mul(t0, l1, l2);
        fe1271_mulconst(t0, t0, q6);
        fe1271_mulconst(t0, t0, q7);
        fe1271_sub(r, r, t0);
    }
}

static void get_k4(fe1271 *r, fe1271 *t, const fe1271 *l1, const fe1271 *l2, unsigned char tau)
{
    /*
     * Compute K_4(l1,l2,tau)
     *
     * Input: 
     *      l1: Element of fe1271
     *      l2: Element of fe1271
     *      tau: Either 0 or 1
     *
     * Output: 
     *      r: K_4(l1,l2,tau)
     */

    if (tau)
    {
        fe1271_mulconst(t, l2, q0);
        fe1271_mulconst(r, l1, q1);
        fe1271_sub(t, t, r);
        set_const(r, q2);
        fe1271_add(t, t, r);

        fe1271_mul(t, t, l1);
        fe1271_mul(t, t, l2);
        fe1271_mulconst(t, t, q3);
        fe1271_add(t, t, t);

        fe1271_mulconst(r, l1, q3);
        fe1271_square(r, r);
        fe1271_sub(t, r, t);
        fe1271_mulconst(r, l2, q5);
        fe1271_square(r, r);
        fe1271_add(t, r, t);
    }

    fe1271_mulconst(r, l1, q4);
    fe1271_mul(r, r, l2);
    fe1271_square(r, r);

    if (tau)
    {
        fe1271_add(r, r, t);
    }
}

/*
 * Compression
 */

static void T_row(fe1271 *r, const fe1271 *X1,
        const fe1271 *X2, const fe1271 *X3, const fe1271 *X4)
{

    fe1271 t;

    fe1271_mulconst(r, X2, khat_2);
    fe1271_mulconst(&t, X3, khat_3);
    fe1271_add(r, r, &t);
    fe1271_mulconst(&t, X4, khat_4);
    fe1271_add(r, r, &t);
    fe1271_mulconst(&t, X1, khat_1);
    fe1271_sub(r, r, &t);
}

static void T(kp *r, const kp *x)
{
    /*
     * Matrix multiplication by T = ( khat_{} )
     *
     * Input: 
     *      (X1,X2,X3,X4): Four fe1271 elements
     *
     * Output: 
     *      r  : X1*khat_4+X2*khat_3+X3*khat_2+X4*khat_1
     *      r+1: X1*khat_3+X2*khat_4+X3*khat_1+X4*khat_2
     *      r+2: X1*khat_2+X2*khat_1+X3*khat_4+X4*khat_3
     *      r+3: X1*khat_1+X2*khat_2+X3*khat_3+X4*khat_4
     */

    T_row(&r->X, &x->T, &x->Z, &x->Y, &x->X);
    T_row(&r->Y, &x->Z, &x->T, &x->X, &x->Y);
    T_row(&r->Z, &x->Y, &x->X, &x->T, &x->Z);
    T_row(&r->T, &x->X, &x->Y, &x->Z, &x->T);
}

void compress(fe1271 *l1, fe1271 *l2, const kp *x)
{
    /*
     * Compress a Kummer point to two field elements,
     * and two sign bits
     *
     * Input: 
     *      x: Uncompressed Kummer point
     *
     * Output: 
     *      l1: Element of fe1271
     *      l2: Element of fe1271
     *      tau: Either 0 or 1 (top bit of l1)
     *      sigma: Either 0 or 1 (top bit of l2)
     */

    kp t;
    unsigned char tau;

    T(&t, x);

    tau = fe1271_iszero(&t.Z); // 0 if L_3 = 0, 1 if L_3 \neq 0
    if (tau) { fe1271_invert(l2, &t.Z); }
    else if ( fe1271_iszero(&t.Y) ) { fe1271_invert(l2, &t.Y); }
    else if ( fe1271_iszero(&t.X) ) { fe1271_invert(l2, &t.X); }
    else { fe1271_invert(l2, &t.T); }

    // Normalize
    fe1271_mul(&t.T, &t.T, l2);
    fe1271_mul(l1, &t.X, l2);
    fe1271_mul(l2, &t.Y, l2);

    // k2*l4 - k3
    get_k2(&t.Z, &t.X, l1, l2, tau);
    fe1271_mul(&t.Z, &t.Z, &t.T);
    get_k3(&t.T, &t.X, &t.Y, l1, l2, tau);
    fe1271_sub(&t.Z, &t.Z, &t.T);

    fe1271_freeze(l1, l1);
    fe1271_freeze(l2, l2);
    fe1271_freeze(&t.Z, &t.Z);
    l1->b[15] |= ( (tau & 1) << 7);
    l2->b[15] |= ( (t.Z.b[0] & 1) << 7 );
}

/*
 * Decompression
 */

static void T_inv_row(fe1271 *r, const fe1271 *X1,
        const fe1271 *X2, const fe1271 *X3, const fe1271 *X4)
{
    fe1271 t;

    fe1271_add(r, X2, X2);
    fe1271_sub(r, r, X1);
    fe1271_mulconst(r, r, mu_1);
    fe1271_mulconst(&t, X3, mu_3);
    fe1271_add(r, r, &t);
    fe1271_mulconst(&t, X4, mu_4);
    fe1271_add(r, r, &t);
}

static void T_inv(kp *r, const kp *x)
{
    /*
     * Matrix multiplication by T_inv = ( mu_{} )
     *
     * Input: 
     *      (X1,X2,X3,X4): Four fe1271 elements
     *
     * Output: 
     *      r  : X1*mu_1+X2*mu_2+X3*mu_3+X4*mu_4
     *      r+1: X1*mu_2+X2*mu_1+X3*mu_4+X4*mu_3
     *      r+2: X1*mu_3+X2*mu_4+X3*mu_1+X4*mu_2
     *      r+3: X1*mu_4+X2*mu_3+X3*mu_2+X4*mu_1
     */

    T_inv_row(&r->X, &x->T, &x->Z, &x->Y, &x->X);
    T_inv_row(&r->Y, &x->Z, &x->T, &x->X, &x->Y);
    T_inv_row(&r->Z, &x->Y, &x->X, &x->T, &x->Z);
    T_inv_row(&r->T, &x->X, &x->Y, &x->Z, &x->T);
}

static int fe1271_hasSqrt(fe1271 *R, fe1271 *t, const fe1271 *Delta, unsigned char sigma)
{
    /*
     * Return the square-root of Delta, with sign
     * of sigma, if it exists
     *
     * Input: 
     *      Delta: Element of fe1271
     *      sigma: Either 0 or 1
     *
     * Output: 
     *      0 if Delta has a Sqrt, 1 otherwise
     *      R: Sqrt(Delta), if it exists
     */

    fe1271_powminhalf(R, Delta);
    fe1271_mul(R, R, Delta);

    fe1271_square(t, R);
    fe1271_sub(t, t, Delta);

    if ( fe1271_iszero(t) ) { return 1; }
    fe1271_freeze(R, R);
    if ( (R->b[0] & 1) ^ sigma ) 
    {
        fe1271_neg(R, R);
    }

    return 0;
}

int decompress(kp *r, kp *t, const kpc *x)
{
    /*
     * Decompress two field elements and two
     * sign bits to a Kummer point.
     * If valid decompression is possible,
     * return 0. Otherwise, return 1.
     *
     * Input: 
     *      x: Compressed Kummer point (l1,l2,tau,sigma)
     *
     * Output: 
     *      r: Uncompressed Kummer point
     */

    unsigned char tau, sigma;

    fe1271_copy(&r->X, &x->fe.l1);
    fe1271_copy(&r->Y, &x->fe.l2);

    tau = (r->X.b[15] & 0x80) >> 7;
    sigma = (r->Y.b[15] & 0x80) >> 7;
    r->X.b[15] &= 0x7f;
    r->Y.b[15] &= 0x7f;

    get_k2(&t->Y, &r->Z, &r->X, &r->Y, tau);
    get_k3(&t->Z, &r->Z, &r->T, &r->X, &r->Y, tau);
    get_k4(&t->T, &r->Z, &r->X, &r->Y, tau);

    if ( !fe1271_iszero(&t->Y) ) // k2 = 0
    {
        fe1271_freeze(&t->Z, &t->Z);
        if ( !fe1271_iszero(&t->Z) ) // k3 = 0
        {
            if ( fe1271_iszero(&r->X) | fe1271_iszero(&r->Y) | tau | sigma )
            {
                return 1; 
            }
            else 
            { 
                fe1271_setzero(&t->X);
                fe1271_setzero(&t->Y);
                fe1271_setzero(&t->Z);
                fe1271_setzero(&t->T);
                t->T.b[0] = 1;
            }
        }
        else if ( sigma ^ t->Z.b[0] )
        {
            fe1271_mul(&t->X, &t->Z, &r->X);
            fe1271_add(&t->X, &t->X, &t->X);
            fe1271_mul(&t->Y, &t->Z, &r->Y);
            fe1271_add(&t->Y, &t->Y, &t->Y);
            if (tau) { fe1271_add(&t->Z, &t->Z, &t->Z); }
            else { fe1271_setzero(&t->Z); }
        }
        else { return 1; }
    }
    else
    {
        fe1271_square(&r->Z, &t->Z);
        fe1271_mul(&r->T, &t->Y, &t->T);
        fe1271_sub(&r->Z, &r->Z, &r->T);
        if ( fe1271_hasSqrt(&r->T, &t->X, &r->Z, sigma) ) { return 1; }

        fe1271_add(&t->T, &t->Z, &r->T);
        if (tau) { fe1271_copy(&t->Z, &t->Y); }
        else { fe1271_setzero(&t->Z); }
        fe1271_mul(&t->X, &t->Y, &r->X);
        fe1271_mul(&t->Y, &t->Y, &r->Y);
    }

    T_inv(r, t);

    return 0;
}
