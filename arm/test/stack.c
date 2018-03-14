#include <stdlib.h>
#include "print.h"
#include "fail.h"
#include "../fe1271.h"
#include "../ladder.h"
#include "../compress.h"
#include "../sign.h"
#include "../dh.h"

#define MAXSTACK 5000

unsigned int ctr;
unsigned char canary;
volatile unsigned char *p;
extern unsigned char _end; 

static unsigned int stack_count(unsigned char canary,volatile unsigned char *a)
{
  volatile unsigned char *p = (a-MAXSTACK);
  unsigned int c = 0;
  while(*p == canary && p < a)
  {
    p++;
    c++;
  }
  return c;
} 

#define WRITE_CANARY(X) {p=X;while(p>= (X-MAXSTACK)) *(p--) = canary;}

unsigned char pair_sk[64];
unsigned char pair_pk[32];

unsigned char sk[64];
unsigned char pk[32];
unsigned long long mlen = 1;
unsigned char m[1];
unsigned char sm[65];
unsigned long long smlen = 65;

uint32 ch[1];
unsigned long long ver_mlen = 1;
unsigned char ver_m[1];
unsigned char ver_sm[65];
unsigned long long ver_smlen = 65;
unsigned char ver_pk[32];
 
int main(void)
{
  //randombytes(&canary,1);
    volatile unsigned char c;
  canary = 42;

#if 0
  volatile unsigned char a; /* Mark the beginning of the stack */
  WRITE_CANARY(&a);
    dh_keygen(pk, sk);
  ctr = MAXSTACK - stack_count(canary,&a);
  print_stack("dh_keygen",-1,ctr);

  volatile unsigned char b; /* Mark the beginning of the stack */
  WRITE_CANARY(&b);
    dh_exchange(pk, pk, sk);
  ctr = MAXSTACK - stack_count(canary,&b);
  print_stack("dh_exchange",-1,ctr);
#endif

#if 0
  WRITE_CANARY(&c);
    keypair(pair_pk, pair_sk);
  ctr = MAXSTACK - stack_count(canary,&c);
  print_stack("keypair",-1,ctr);
#endif

#if 0
  WRITE_CANARY(&c);
    sign(sm, &smlen, m, mlen, pk, sk);
  ctr = MAXSTACK - stack_count(canary,&c);
  print_stack("sign",-1,ctr);
#endif

  WRITE_CANARY(&c);
    verify(ver_m, ver_mlen, ver_sm, ver_smlen, ver_pk);
  ctr = MAXSTACK - stack_count(canary,&c);
  print_stack("verify",-1,ctr);

    /*
  WRITE_CANARY(&a);
    ladder(&r, &xq, &bpw, n, len);
  ctr = MAXSTACK - stack_count(canary,&a);
  print_stack("ladder",-1,ctr);

  WRITE_CANARY(&a);
    ladder_base(&r, n, len);
  ctr = MAXSTACK - stack_count(canary,&a);
  print_stack("ladder_base",-1,ctr);

  WRITE_CANARY(&a);
    compress(&rc, &r);
  ctr = MAXSTACK - stack_count(canary,&a);
  print_stack("compress",-1,ctr);

  WRITE_CANARY(&a);
    decompress(&r, &rc);
  ctr = MAXSTACK - stack_count(canary,&a);
  print_stack("decompress",-1,ctr);
    */

    /*
    print("\n");
    print("\n");
    print("\n");
  print32(ch,1);
    print("\n");
  print32(ch,1);
    print("\n");
    print("\n");
    print("\n");
    */

  write_byte(4);
  while(1);
}
