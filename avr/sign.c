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

    hash(sk, sk, 32);
    group_scalar_get32(pk, sk+32);
    ladder_base(&R, pk, 250);
    compress(&R.Z, &R.T, &R);

    fe1271_pack(pk, &R.Z);
    fe1271_pack(pk+16, &R.T);

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
    unsigned char r[32];
    kp R;

    *smlen = mlen+64;
    for(i=0;i<mlen;i++) { sm[64+i] = m[i]; }
    for(i=0;i<32;i++) { sm[32+i] = sk[i]; }
    hash(sm, sm+32, mlen+32); 
    group_scalar_get64(r, sm);

    ladder_base(&R, r, 250);
    compress(&R.Z, &R.T, &R);

    fe1271_pack(sm, &R.Z);
    fe1271_pack(sm+16, &R.T);
    for(i=0;i<32;i++) { sm[32+i] = pk[i]; }
    hash(sm, sm, mlen+64);

    group_scalar_get64(R.X.b, sm);
    fe1271_pack(sm, &R.Z);
    fe1271_pack(sm+16, &R.T);

    group_scalar_set_pos(R.X.b, R.X.b);
    group_scalar_get32(sm+32, sk+32);
    group_scalar_ops(sm+32, r, R.X.b, sm+32);

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
    group_scalar_get32(R.X.b, sm+32);
    for(i=0;i<smlen-64;i++) { m[i] = sm[64+i]; }
    mlen = smlen-64;

    for(i=0;i<32;i++) { sm[32+i] = pk[i]; }
    hash(sm, sm, mlen+64);
    group_scalar_get64(R.Z.b, sm);

    xWRAP(&pxw, &sP);
    ladder(&hQ, &sP, &pxw, R.Z.b, 250);
    ladder_base(&sP, R.X.b, 250);

    return check(&sP, &hQ, &R, &pxw, &rx);
}
