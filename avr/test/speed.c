#include <stdlib.h>
#include "print.h"
#include "cpucycles.h"
#include "fail.h"
#include "avr.h"
#include "../sign.h"

#define NRUNS 5

#define nlen 32
#define plen 32

static unsigned char *n;
static unsigned char *p;

static unsigned char sk[64];
static unsigned char pk[32];
static kpc rc;
static kp r;
static unsigned char s[32];
static unsigned long long mlen = 64;
static unsigned long long smlen;
static unsigned char m[64];
static unsigned char sm[128];
static int ch;

/*
    unsigned char sk[64];
    unsigned long long mlen = 64;
    unsigned char pk[32];
    unsigned char sm[64+mlen];
    unsigned long long smlen;
    ecp r;
    group_scalar s;
*/

int main(void)
{
  unsigned int i;
  unsigned long long t[NRUNS];

  n = calloc(nlen,1);
  if(!n) fail("allocation of n failed");
  p = calloc(plen,1);
  if(!p) fail("allocation of p failed");

  for (i = 0;i < nlen;++i) n[i] = i + 1;
  for (i = 0;i < plen;++i) p[i] = i + 2;

  for(i=0;i<NRUNS;i++)
  {
    t[i] = cpucycles();
    ladder_base(&r, s, 250);
  }
  print_speed("ladder_base",-1,t,NRUNS);

  for(i=0;i<NRUNS;i++)
  {
    t[i] = cpucycles();
    keypair(pk, sk);
  }
  print_speed("keypair",-1,t,NRUNS);

  for(i=0;i<NRUNS;i++)
  {
    t[i] = cpucycles();
        sign(sm, &smlen, m, mlen, pk, sk);
  }
  print_speed("sign",-1,t,NRUNS);

  for(i=0;i<NRUNS;i++)
  {
    t[i] = cpucycles();
        ch = verify(m, mlen, sm, smlen, pk);
  }
  print_speed("verify",-1,t,NRUNS);

  for(i=0;i<NRUNS;i++)
  {
    t[i] = cpucycles();
    ladder_base(&r, s, 250);
  }
  print_speed("ladder_base",-1,t,NRUNS);

  for(i=0;i<NRUNS;i++)
  {
    t[i] = cpucycles();
    check(&r, &r, &r, &r, &rc);
  }
  print_speed("check",-1,t,NRUNS);

  for(i=0;i<NRUNS;i++)
  {
    t[i] = cpucycles();
        compress(&rc.fe.l1, &rc.fe.l2, &r);
  }
  print_speed("compress",-1,t,NRUNS);

  for(i=0;i<NRUNS;i++)
  {
    t[i] = cpucycles();
        decompress(&r, &r, &rc);
  }
  print_speed("decompress",-1,t,NRUNS);

  free(n);
  free(p);

  avr_end();
  return 0;
}
