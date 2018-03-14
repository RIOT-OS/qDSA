/*
 * Based on crypto_scalarmult/try.c version 20090118 by D. J. Bernstein
 * Public domain.
 */

#include "../sign.h"
#include "print.h"
#include "fail.h"

#include <stdlib.h>
#define max_length 64
#define num_tests 1

#define loop_min 0
#define loop_len 1
#define mes_len 1

int main(void)
{
  unsigned long long i,j;
    uint32 ch[1] = {0x12345678};

    /* Full signature chs */

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
        print("\n");
        print8(sk, 32);
        print("\n");
        print8(sk+32, 32);
        print("\n");
        sign(sm, &smlen, m, mlen, pk, sk);
        ch[0] = verify(m, mlen, sm, smlen, pk);

        if(ch[0]) { error = i; i = loop_min+loop_len; ctr += 1; }
    }

    if(!ctr) 
    {
        print("\n");
        print("Signatures correct!");
        print("\n");
        unsigned char num = loop_len;
        print8(&num, 1);
        print("iterations (in hex)");
        print("\n");
    }
    else
    {
        print("\n");
        print("Incorrect!");
        print("\n");
        print8(&error, 1);
        print("error!");
        print("\n");
        print8(sk, 32);
        print("\n");
        print8(sk+32, 32);
        print("\n");
        print8(m, mlen);
        print("\n");
    }

    /* End full signature chs */

    /* crypto_scalarmult */

    /*
  static const uint32 xpw[12] = { 0x4e931a48, 0xaeb351a6, 0x2049c2e7, 0x1be0c3dc,
                                  0xe07e36df, 0x64659818, 0x8eaba630, 0x23b416cd,
                                  0x7215441e, 0xc7ae3d05, 0x4447a24d, 0x5db35c38 };

    uint32 rw[16], r[16];
    static const uint32 n0[8] = {0,0,0,0,0,0,0,0};
    static const uint32 n1[8] = {1,0,0,0,0,0,0,0};
    static const uint32 n2[8] = {-1,-1,-1,-1,-1,-1,-1,-1};

    crypto_scalarmult(r,n0,xpw,255);
    xWRAP(rw,r);
  print("\n");
  print("n = n0");
  print("\n");
  print32(rw,12);
  print("\n");

    crypto_scalarmult(r,n1,xpw,255);
    xWRAP(rw,r);
  print("\n");
  print("n = n1");
  print("\n");
  print32(rw,12);
  print("\n");

    crypto_scalarmult(r,n2,xpw,255);
    xWRAP(rw,r);
  print("\n");
  print("n = n2");
  print("\n");
  print32(rw,12);
  print("\n");
    */

    /*
  static const uint32 xpw[12] = { 0x4e931a48, 0xaeb351a6, 0x2049c2e7, 0x1be0c3dc,
                                  0xe07e36df, 0x64659818, 0x8eaba630, 0x23b416cd,
                                  0x7215441e, 0xc7ae3d05, 0x4447a24d, 0x5db35c38 };
    static const uint32 n0[8] = {0,0,0,0,0,0,0,0};
    static const uint32 n1[8] = {1,0,0,0,0,0,0,0};
    static const uint32 n2[8] = {-1,-1,-1,-1,-1,-1,-1,-1};

    kp xpn, xqn;
    kpw xdn, xpnw;

    for(i=0;i<4;i++)
    {
        xdn.y.v[i] = xpw[i];
        xdn.z.v[i] = xpw[i+4];
        xdn.t.v[i] = xpw[i+8];
    }

    ladder_base(&xpn, n0, 255);
    xWRAP(&xpnw,&xpn);
  print("\n");
  print("n = n0");
  print("\n");
  print32(xpnw.y.v,4);
  print("\n");
  print32(xpnw.z.v,4);
  print("\n");
  print32(xpnw.t.v,4);
  print("\n");

    ladder_base(&xpn, n1, 255);
    xWRAP(&xpnw,&xpn);
  print("\n");
  print("n = n1");
  print("\n");
  print32(xpnw.y.v,4);
  print("\n");
  print32(xpnw.z.v,4);
  print("\n");
  print32(xpnw.t.v,4);
  print("\n");

    ladder_base(&xpn, n2, 255);
    xWRAP(&xpnw,&xpn);
  print("\n");
  print("n = n2");
  print("\n");
  print32(xpnw.y.v,4);
  print("\n");
  print32(xpnw.z.v,4);
  print("\n");
  print32(xpnw.t.v,4);
  print("\n");
    */

    /* end crypto_scalarmult */

    /* modN */

    /*
    uint32 r_add[16];
    static const uint32 x_add[16] = {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0};
    static const uint32 y_add[8] = {-1,-1,-1,-1,-1,-1,-1,-1};

    add_N(r_add, x_add, y_add, 8);

  print("\n");
  print("add_N");
  print("\n");
  print32(r_add,16);
  print("\n");

    uint32 r_mul[16];
    static const uint32 x_mul[8] = {0,0,0,0,0,0,0,0x80000000};
    static const uint32 y_mul[8] = {0,0,0,0,2,0,0,0};

    mul_N(r_mul, x_mul, y_mul);

  print("\n");
  print("mul_N");
  print("\n");
  print32(r_mul,16);
  print("\n");

    uint32 r_red[8];
    group_scalar gr;
    static const uint32 x1_red[16] = {0x477F0C74, 0x608C414, 0xF3A557C1, 0x858F0D49, 
                                                                        0x3859642B, 0x6B735BDE, 0x6D90A3CC, 0x4072FCE5, 
                                                                        0x93FCFBB5, 0x508280C9, 0x41CD1247, 0xB51B84E0, 
                                                                        0x2E760ADE, 0xD9CBE35E, 0x16E6B02F, 0xE3AD27F0};
    static const uint32 x2_red[16] = {0xF4B8CDEC, 0xCE552A0B, 0xA63E1101, 0x98D14C69, 
                                                                        0x49D73543, 0xC58966C6, 0xD856CBA7, 0xC094D286,
                                                                        0x79B79C87, 0xA7415FAB, 0x9E2FFDD7, 0xA8718784, 
                                                                        0xAA587448, 0xED6FFC3C, 0xC8A90019, 0x843F5677};
    static const uint32 x3_red[16] = {0x78757701, 0x6BCE2D24, 0xEA75D5B6, 0xF7DCD882, 
                                                                        0x4BD0A8C8, 0xBD544DA, 0x3F668602, 0x31EE9C1F, 
                                                                        0xB39E4C49, 0xB0880C92, 0x7F20EE8F, 0xD04F1B4F, 
                                                                        0x86E2F9D, 0x4BA2C8A4, 0x25784076, 0xEE1ED30B};

    red_N(r_red, x1_red);

  print("\n");
  print("x1_red");
  print("\n");
  print32(r_red,8);
  print("\n");

    red_N(r_red, x2_red);

  print("\n");
  print("x2_red");
  print("\n");
  print32(r_red,8);
  print("\n");

    red_N(r_red, x3_red);

  print("\n");
  print("x3_red");
  print("\n");
  print32(r_red,8);
  print("\n");

    big_red(&gr, x1_red);

  print("\n");
  print("x1_red new");
  print("\n");
  print32(gr.fe.l1.v,4);
  print("\n");
  print32(gr.fe.l2.v,4);
  print("\n");

    big_red(&gr, x2_red);

  print("\n");
  print("x2_red new");
  print("\n");
  print32(gr.fe.l1.v,4);
  print("\n");
  print32(gr.fe.l2.v,4);
  print("\n");

    big_red(&gr, x3_red);

  print("\n");
  print("x3_red new");
  print("\n");
  print32(gr.fe.l1.v,4);
  print("\n");
  print32(gr.fe.l2.v,4);
  print("\n");

    uint32 r_mul16[8];
    static const uint32 x_mul16[8] = {1,2,3,4,5,6,7,8};

    mul16_N(r_mul16, x_mul16);

  print("\n");
  print("mul16_N");
  print("\n");
  print32(r_mul16,8);
  print("\n");

    uint32 r_neg[8];
    static const uint32 x_neg[8] = {0xFC6CBF62, 0x1575F3FD, 0x148A11B8, 0xD7579D59, 0x58998015, 0xE647AE8D, 0xF6CA6774, 0x4D9934};

    neg_N(r_neg, x_neg);

  print("\n");
  print("neg_N");
  print("\n");
  print32(r_neg,8);
  print("\n");

    uint32 s_ops[8];
    static const uint32 r_ops[8] = {0xCF9F6DF, 0xCAAA3F10, 0xFE4E7671, 0x1B7A6723, 0x212B273, 0xB06A5D02, 0xF8FE9200, 0x2FC4390F};
    static const uint32 h_ops[16] = {0x9E74CEBA, 0x7F2465D0, 0xB7A4246F, 0x4DAA3BFC, 
                                                                        0x43E64466, 0x43C92824, 0x68C0C3FF, 0x7F4987E9, 
                                                                        0xDE4F7482, 0x56D1CE4D, 0xE1D373C, 0xD517DAC5, 
                                                                        0xFC7542C, 0x6B1C3FD2, 0x462B2A41, 0x359BECBC};
    static const uint32 d_ops[8] = {0x3E7E00B1, 0x9978472E, 0x5B12F992, 0x3E51649B, 0x71728E65, 0x1BA6731B, 0x94308B3E, 0x7D245263};

    group_ops(s_ops, r_ops, h_ops, d_ops);

  print("\n");
  print("groups_ops");
  print("\n");
  print32(s_ops,8);
  print("\n");
    */

    /* end modN */

    /* hash */

    /*
    union data512 digest;
    unsigned long long inlen = 1;
    unsigned char message[inlen];

    for (i=0; i<inlen; i++)
    {
        message[i] = 0;
    }
    //message[0] = 0x01;
    //message[1] = 0x02;

    shake(digest.u8, message, inlen);

    print("\n");
    print("Hash output");
    print("\n");
    print32(digest.u32,16);
    print("\n");
    */

    /* end hash */

    /* gfe_sqrtinv */

        /*
    char b = 0;
    gfe r, s;
    static const gfe x = {0xff,0xff,0xff,0xff};
    static const gfe y = {0xff,0xff,0xff,0xff};

    gfe_sqrtinv(r, s, x, y, b);

    print("\n");
    print("sqrtinv");
    print("\n");
    print32(r,4);
    print("\n");
    print32(s,4);
    print("\n");

    fe1271 rn, sn;
    fe1271 xn, yn;

    for(i=0;i<4;i++) xn.v[i] = x[i];
    for(i=0;i<4;i++) yn.v[i] = y[i];

    fe1271_sqrtinv(&rn, &sn, &xn, &yn, b);

    print("\n");
    print("sqrtinv new");
    print("\n");
    print32(rn.v,4);
    print("\n");
    print32(sn.v,4);
    print("\n");
    */

    /* end gfe_invert */

    /* xDBLADD */

        /*
    uint32 r[16], s[16];
    uint32 xp[16] = {
        0x521BD580, 0xBCAF6FB3, 0xE21691D2, 0x6F9070B5,
        0xB91A3EB4, 0xF0D22507, 0x150CF313, 0x35F6D849,
        0x209067FD, 0xCB160E42, 0xDBA8B4E8, 0x93A653A,
        0xBAF48052, 0x4DC2FCD1, 0x6A432EED, 0x30B0510E
    };
    uint32 xq[16] = {
        0xD94FEB1B, 0x483CF770, 0xE2614471, 0x71BE2EEE,
        0x6F2D3C80, 0xD0651100, 0x650CDAE1, 0x1D11B2D8,
        0x7919303C, 0xCB001F5B, 0xE0038948, 0x4478711,
        0x50CBAB6E, 0x33653A67, 0xFA93AFCB, 0x5BC7AD40
    };
    uint32 xd[12] = {
        0xA3101962, 0x429284A2, 0xB4E21F84, 0x1740149D,
        0x3BEB08D3, 0xC8380366, 0x9D13BD1F, 0xF712A8E,
        0xD94F749C, 0xB5F1A9C7, 0x692A2236, 0x3DB6559D
    };

    xDBLADD(r,s,xp,xq,xd);

    print("\n");
    print("old");
    print("\n");
    print("r");
    print("\n");
  print32(r,4);
    print("\n");
    print("\n");
  print32(r+4,4);
    print("\n");
    print("\n");
  print32(r+8,4);
    print("\n");
    print("\n");
  print32(r+12,4);
    print("\n");
    print("\n");
    print("s");
    print("\n");
  print32(s,4);
    print("\n");
    print("\n");
  print32(s+4,4);
    print("\n");
    print("\n");
  print32(s+8,4);
    print("\n");
    print("\n");
  print32(s+12,4);
    print("\n");

    kp xpn, xqn;
    kpw xdn;

    for(i=0;i<4;i++)
    {
        xpn.X.v[i] = xp[i]; xqn.X.v[i] = xq[i];
        xpn.Y.v[i] = xp[i+4]; xqn.Y.v[i] = xq[i+4];
        xpn.Z.v[i] = xp[i+8]; xqn.Z.v[i] = xq[i+8];
        xpn.T.v[i] = xp[i+12]; xqn.T.v[i] = xq[i+12];

        xdn.y.v[i] = xd[i];
        xdn.z.v[i] = xd[i+4];
        xdn.t.v[i] = xd[i+8];
    }

    xDBLADD(&xpn, &xqn, &xdn);

    print("\n");
    print("new");
    print("\n");
    print("r");
    print("\n");
  print32(xpn.X.v,4);
    print("\n");
  print32(xpn.Y.v,4);
    print("\n");
  print32(xpn.Z.v,4);
    print("\n");
  print32(xpn.T.v,4);
    print("\n");
    print("\n");
    print("s");
    print("\n");
  print32(xqn.X.v,4);
    print("\n");
  print32(xqn.Y.v,4);
    print("\n");
  print32(xqn.Z.v,4);
    print("\n");
  print32(xqn.T.v,4);
    print("\n");
    */

    /* end xDBLADD */

    /* tmat */

        /*
    fe1271 r;
    kp xr;
    kp xp = {
        .X = { .v = {0x5717DE3, 0x55791D66, 0x47CCFBBA, 0x650AA43B} },
        .Y = { .v = {0x3FA49659, 0x7F5DA10, 0xA60B3489, 0x30D44EBC} },
        .Z = { .v = {0xEA71AD2D, 0xF841991F, 0x72C0D71A, 0x37EA498} },
        .T = { .v = {0xAF5633A2, 0xEB751675, 0x1AA535EC, 0x38FE6452} }
    };

    tmat_coord(&r, &xp.X, &xp.Y, &xp.Z, &xp.T);

    print("\n");
    print("r");
    print("\n");
  print32(r.v,4);
    print("\n");
  print32(r.v,4);
    print("\n");

    tmat(&xr, &xp);

    fe1271_freeze(&xr.X, &xr.X);
    fe1271_freeze(&xr.Y, &xr.Y);
    fe1271_freeze(&xr.Z, &xr.Z);
    fe1271_freeze(&xr.T, &xr.T);

    print("\n");
    print("rx");
    print("\n");
  print32(xr.X.v,4);
    print("\n");
  print32(xr.Y.v,4);
    print("\n");
  print32(xr.Z.v,4);
    print("\n");
  print32(xr.T.v,4);
    print("\n");
    */

    /* end tmat */

    /* compress */

        /*
    kp xrr;
    kpc xr;
    kp xp = {
        .X = { .v = {0x5717DE3, 0x55791D66, 0x47CCFBBA, 0x650AA43B} },
        .Y = { .v = {0x3FA49659, 0x7F5DA10, 0xA60B3489, 0x30D44EBC} },
        .Z = { .v = {0xEA71AD2D, 0xF841991F, 0x72C0D71A, 0x37EA498} },
        .T = { .v = {0xAF5633A2, 0xEB751675, 0x1AA535EC, 0x38FE6452} }
    };

    compress(&xr, &xp);

    print("\n");
    print("rx");
    print("\n");
  print32(xr.l1.v,4);
    print("\n");
  print32(xr.l2.v,4);
    print("\n");

    print("\n");
    print("rx");
    print("\n");
  print32(xr.l1.v,4);
    print("\n");
  print32(xr.l2.v,4);
    print("\n");

    decompress(&xrr, &xr);

    print("\n");
    print("xrr");
    print("\n");
  print32(xrr.X.v,4);
    print("\n");
  print32(xrr.Y.v,4);
    print("\n");
  print32(xrr.Z.v,4);
    print("\n");
  print32(xrr.T.v,4);
    print("\n");
    */

    /* end compress */

#if 0
    /* keypair & sign & verify */

    unsigned char sk[64];
    unsigned char pk[32];
    unsigned long long mlen = 1;
    unsigned char m[mlen];
    unsigned char sm[64+mlen];
    unsigned long long smlen;

    for(i=0;i<64;i++) sk[i] = 0;
    sk[0] = 0x2;

    keypair(pk, sk);

    print("\n");
    print("sk");
    print("\n");
    print8(sk, 64);
    print("\n");
    print("\n");
    print8(pk, 16);
    print("\n");
    print8(pk+16, 16);
    print("\n");

    m[0] = 0;
    sign(sm, &smlen, m, mlen, pk, sk);

    print("\n");
    print("sm");
    print("\n");
    print8(sm, 16);
    print("\n");
    print8(sm+16, 16);
    print("\n");
    print("\n");
    print8(sm+32, 32);
    print("\n");
    print("\n");

    ch[0] = verify(m, mlen, sm, smlen, pk);

    /* end keypair & sign & verify */
#endif

    print("\n");
    print("\n");
    print("\n");
  print32(ch,1);
    print("\n");
  print32(ch,1);
    print("\n");
    print("\n");
    print("\n");

  write_byte(4);

  while(1);
}
