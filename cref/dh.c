#include "dh.h"

int dh_keygen(
        unsigned char *pk,
        unsigned char *sk
        )
{
    /*
     * Generate a public key point on the Kummer
     *
     * Input: 
     *      sk (32 bytes): 32 bytes of randomness
     *
     * Output:
     *      pk (32 bytes): Public key
     */

    kp R;

    group_scalar_get32(pk, sk);
    ladder_base(&R, pk, 250);
    compress(&R.Z, &R.T, &R);

    fe1271_pack(pk, &R.Z);
    fe1271_pack(pk+16, &R.T);

    return 0;
}

int dh_exchange(
        unsigned char *ss,
        unsigned char *pk,
        unsigned char *sk
        )
{
    /*
     * Generate a shared secret
     *
     * Input: 
     *      sk (32 bytes): 32 bytes of randomness
     *      pk (32 bytes): Public key
     *
     * Output:
     *      ss (32 bytes): Shared secret
     */

    unsigned char i;
    kp SS, PK, pkw;
    kpc pkc;

    for(i=0;i<32;i++) { pkc.b[i] = pk[i]; }
    decompress(&PK, &SS, &pkc);
    xWRAP(&pkw, &PK);

    group_scalar_get32(ss, sk);
    ladder(&SS, &PK, &pkw, ss, 250);
    compress(&SS.Z, &SS.T, &SS);

    fe1271_pack(ss, &SS.Z);
    fe1271_pack(ss+16, &SS.T);

    return 0;
}
