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
    group_scalar rx;

    group_scalar_get32(rx.fe.l1.v, sk);
    ladder_base(&R, rx.fe.l1.b, 250);
    compress(&rx.fe.l1, &rx.fe.l2, &R);

    kpc_pack(pk, &rx);

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

    group_scalar_get32(pkc.fe.l1.v, sk);
    ladder(&SS, &PK, &pkw, pkc.fe.l1.b, 250);
    compress(&pkc.fe.l1, &pkc.fe.l2, &SS);

    kpc_pack(ss, &pkc);

    return 0;
}
