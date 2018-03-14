#include "../sign.h"
#include "../dh.h"
#include "print.h"

#include <stdlib.h>
#include <time.h>

#define max_length 1
#define num_tests 10

int main(void)
{
    /* signatures API */

    unsigned long long i;
    int j;
    unsigned char sk[64]; 
    unsigned long long mlen = max_length;
    unsigned char m[mlen]; 
    unsigned char pk[32];
    unsigned char sm[64+mlen];
    unsigned long long smlen;
    unsigned char ch;

    printf("Checking %d signatures...\n", num_tests);
    for(j=0;j<num_tests;j++)
    {
        srand(j);

        for(i=0;i<64;i++) { sk[i] = rand(); }
        for(i=0;i<mlen;i++) { m[i] = rand(); }

        keypair(pk, sk);
        sign(sm, &smlen, m, mlen, pk, sk);
        ch = verify(sm+64, mlen, sm, smlen, pk);

        if ( ch != 0 ) 
        {
            printf("\n%lld\n", mlen);
            for(i=0;i<32;i++) { printf("%02X,", sk[i]); }
            printf("\n");
            for(i=0;i<mlen;i++) { printf("%02X,", m[i]); }
        }
    }
    printf("Finished");

    /* end signatures API */

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

    for(i=0;i<16;i++) { printf("%02X ", pk[i]); }
    printf("\n");
    for(i=16;i<32;i++) { printf("%02X ", pk[i]); }

    dh_exchange(ss, pk, sk);

    printf("\n");
    for(i=0;i<16;i++) { printf("%02X ", ss[i]); }
    printf("\n");
    for(i=16;i<32;i++) { printf("%02X ", ss[i]); }
#endif

    /* end DH */

    return 0;
}
