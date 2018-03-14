/*
 * Based on crypto_scalarmult/try.c version 20090118 by D. J. Bernstein
 * Public domain.
 */

#include "avr.h"
#include "print.h"
#include "fail.h"
#include "../sign.h"
#include "../dh.h"

#include <stdlib.h>
#define max_length 64
#define num_tests 1

#define loop_min 0
#define loop_len 10
#define mes_len 1

int main(void)
{
    unsigned char ch;

    /* Full signature checks */

    unsigned long long i,j;
    unsigned char sk[64];
    unsigned long long mlen = mes_len;
    unsigned char m[mlen];
    unsigned char pk[32];
    unsigned char sm[64+mlen];
    unsigned long long smlen;

    unsigned long long ctr = 0;
    unsigned char error = 0;

    for(i=loop_min;i<=loop_min+loop_len-1;i++)
    {
        srand(i);

        for(j=0;j<64;j++) { sk[j] = rand() % 256; }
        for(j=0;j<mlen;j++) { m[j] = rand() % 256; }

        keypair(pk, sk);
        sign(sm, &smlen, m, mlen, pk, sk);
        ch = verify(m, mlen, sm, smlen, pk);

        if(ch) { error = i; i = loop_min+loop_len; ctr += 1; }
    }

    if(!ctr) 
    {
        print("\n");
        print("Signatures correct!");
        print("\n");
        unsigned char num = loop_len;
        print_bytes(&num, 1);
        print("iterations (in hex)");
        print("\n");
    }
    else
    {
        print("\n");
        print("Incorrect!");
        print("\n");
        print_bytes(&error, 1);
        print("error!");
        print("\n");
        print_bytes(sk, 32);
        print("\n");
        print_bytes(sk+32, 32);
        print("\n");
        print_bytes(m, mlen);
        print("\n");
    }

    /* End full signature checks */

    /* DH */

#if 0
    unsigned long long i;
    unsigned char sk[32]; 
    unsigned char pk[32];
    unsigned char ss[32];
    for(i=0;i<32;i++) { sk[i] = 0; }
    sk[0] = 0x1;
    sk[31] = 0xFF;

    dh_keygen(pk, sk);

    print("\n");
    print_bytes(pk, 16);
    print("\n");
    print_bytes(pk+16, 16);

    dh_exchange(ss, pk, sk);

    print("\n");
    print_bytes(ss, 16);
    print("\n");
    print_bytes(ss+16, 16);
#endif

    /* end DH */

    print("\n");
    print("\n");
    print_bytes(&ch, 1);
    print_bytes(&ch, 1);
    print_bytes(&ch, 1);
    print_bytes(&ch, 1);
    print_bytes(&ch, 1);
    print_bytes(&ch, 1);
    print("\n");
    print("\n");

  avr_end();
  return 0;
}
