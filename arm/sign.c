#include "sign.h"

int keypair(
        unsigned char *pk,
        unsigned char *sk
        )
{
    /*
     * Generate a 64-byte pseudo-random string and a public
     * key point on the Kummer
     *
     * Input: 
     *      sk (64 bytes): 32 bytes of randomness (in lower bytes)
     *
     * Output: 
     *      pk (32 bytes): Public key
     *      sk (64 bytes): Pseudo-random secret
     */

    kp R;
    group_scalar rx;

    hash(sk, sk, 32);
    group_scalar_get32(rx.fe.l1.v, sk+32);
    ladder_base(&R, rx.fe.l1.b, 250);
    compress(&rx.fe.l1, &rx.fe.l2, &R);

    kpc_pack(pk, &rx);

    return 0;
}

int sign(
        unsigned char *sm, unsigned long long *smlen,
        const unsigned char *m, unsigned long long mlen,
        const unsigned char *pk, const unsigned char *sk
        )
{
    /*
     * Generate a signature consisting of a 32-byte 
     * compressed point on the Kummer and an integer 
     * modulo the curve order.
     * Append the message to the signature.
     *
     * Input: 
     *      m: Message
     *      mlen: Message length in bytes
     *      pk (32 bytes): Public key
     *      sk (64 bytes): Pseudo-random secret
     *
     * Output: 
     *      sm (64+mlen bytes): Signature + Message
     *      smlen: 64+mlen
     */

    unsigned long long i;
    kp R;
    kpc rx;
    group_scalar r;

    *smlen = mlen+64;
    for(i=0;i<mlen;i++) { sm[64+i] = m[i]; }
    for(i=0;i<32;i++) { sm[32+i] = sk[i]; }
    hash(sm, sm+32, mlen+32);
    group_scalar_get64(r.fe.l1.v, sm);

    ladder_base(&R, r.fe.l1.b, 250);
    compress(&rx.fe.l1, &rx.fe.l2, &R);

    for(i=0;i<32;i++) { sm[32+i] = pk[i]; }
    for(i=0;i<32;i++) { sm[i] = rx.b[i]; }
    hash(sm, sm, mlen+64);

    group_scalar_get64(R.X.v, sm);
    group_scalar_get32(R.Z.v, sk+32);
    group_scalar_ops(R.Z.v, &r, R.X.v, R.Z.v);

    kpc_pack(sm, &rx);
    for(i=0;i<16;i++) { sm[32+i] = R.Z.b[i]; }
    for(i=0;i<16;i++) { sm[48+i] = R.T.b[i]; }

    return 0;
}

int verify(
        unsigned char *m, long long mlen,
        unsigned char *sm, unsigned long long smlen,
        const unsigned char *pk
        )
{
    /*
     * Verify correctness of a signature with respect
     * to a public key. Return 0 if correct, 1 if
     * incorrect, and return the message.
     *
     * Input: 
     *      sm (64+mlen bytes): Signature + Message
     *      smlen: 64+mlen
     *      pk (32 bytes): Public key
     *
     * Output: 
     *      0 if correct, 1 if incorrect
     *      m: Message
     *      mlen: Message length (bytes)
     */

    unsigned long long i;
    kp sP, hQ, R, pxw;
    kpc rx;

    kpc_unpack(&rx, pk);
    if (decompress(&sP, &hQ, &rx)) { return 1; }

    kpc_unpack(&rx, sm);
    group_scalar_get32(R.X.v, sm+32);
    for(i=0;i<smlen-64;i++) { m[i] = sm[64+i]; }
    mlen = smlen-64;

    for(i=0;i<32;i++) { sm[32+i] = pk[i]; }
    hash(sm, sm, mlen+64);
    group_scalar_get64(R.Z.v, sm);

    xWRAP(&pxw, &sP);
    ladder(&hQ, &sP, &pxw, R.Z.b, 250);
    ladder_base(&sP, R.X.b, 250);

    return check(&sP, &hQ, &R, &pxw, &rx);
}
