#include <stdio.h>
#include <stm32f0xx.h>
#include "print.h"
#include "../fe1271.h"
#include "../ladder.h"
#include "../compress.h"
#include "../sign.h"
#include "../dh.h"

static kpc rc;
static kp r;
static unsigned char n[32];
static int len = 250;
static unsigned char sk[64];
static unsigned char pk[32];
static unsigned long long mlen = 64;
static unsigned char m[64];
static unsigned char sm[128];
static unsigned long long smlen;
static int ch;

int main(void)
{
  char out[500];

  SysTick_Config(12000000);
  SysTick->VAL = 0;
  //int ini, fin;

  int i;
  int num = 4;
  int ini_mul[num], fin_mul[num];

  // ---------------------
  for(i=0;i<=num-1;i++)
  {
            SysTick_Config(12000000);
      SysTick->VAL = 0;
      ini_mul[i]= SysTick->VAL;
            dh_keygen(pk, sk);
      fin_mul[i]= SysTick->VAL;
  
      fin_mul[i] = ini_mul[i]-fin_mul[i];
  }

  sprintf(out, "dh_keygen: %d, %d, %d, %d", fin_mul[0], fin_mul[1], fin_mul[2], fin_mul[3]);
  print(out);
  print("\n");

  // ---------------------
  for(i=0;i<=num-1;i++)
  {
            SysTick_Config(12000000);
      SysTick->VAL = 0;
      ini_mul[i]= SysTick->VAL;
            dh_exchange(pk, pk, sk);
      fin_mul[i]= SysTick->VAL;
  
      fin_mul[i] = ini_mul[i]-fin_mul[i];
  }

  sprintf(out, "dh_exchange: %d, %d, %d, %d", fin_mul[0], fin_mul[1], fin_mul[2], fin_mul[3]);
  print(out);
  print("\n");

  // ---------------------
  for(i=0;i<=num-1;i++)
  {
            SysTick_Config(12000000);
      SysTick->VAL = 0;
      ini_mul[i]= SysTick->VAL;
            keypair(pk, sk);
      fin_mul[i]= SysTick->VAL;
  
      fin_mul[i] = ini_mul[i]-fin_mul[i];
  }

  sprintf(out, "keypair: %d, %d, %d, %d", fin_mul[0], fin_mul[1], fin_mul[2], fin_mul[3]);
  print(out);
  print("\n");

  // ---------------------
  for(i=0;i<=num-1;i++)
  {
            SysTick_Config(12000000);
      SysTick->VAL = 0;
      ini_mul[i]= SysTick->VAL;
            sign(sm, &smlen, sm+64, mlen, pk, sk);
      fin_mul[i]= SysTick->VAL;
  
      fin_mul[i] = ini_mul[i]-fin_mul[i];
  }

  sprintf(out, "sign: %d, %d, %d, %d", fin_mul[0], fin_mul[1], fin_mul[2], fin_mul[3]);
  print(out);
  print("\n");

  for(i=0;i<=num-1;i++)
  {
            SysTick_Config(12000000);
      SysTick->VAL = 0;
      ini_mul[i]= SysTick->VAL;
            ch = verify(m, mlen, sm, smlen, pk);
      fin_mul[i]= SysTick->VAL;
  
      fin_mul[i] = ini_mul[i]-fin_mul[i];
  }

  sprintf(out, "verify: %d, %d, %d, %d", fin_mul[0], fin_mul[1], fin_mul[2], fin_mul[3]);
  print(out);
  print("\n");

  // ---------------------
  for(i=0;i<=num-1;i++)
  {
            SysTick_Config(12000000);
      SysTick->VAL = 0;
      ini_mul[i]= SysTick->VAL;
            ladder_base(&r, n, len);
      fin_mul[i]= SysTick->VAL;
  
      fin_mul[i] = ini_mul[i]-fin_mul[i];
  }

  sprintf(out, "ladder_base: %d, %d, %d, %d", fin_mul[0], fin_mul[1], fin_mul[2], fin_mul[3]);
  print(out);
  print("\n");

  for(i=0;i<=num-1;i++)
  {
            SysTick_Config(12000000);
      SysTick->VAL = 0;
      ini_mul[i]= SysTick->VAL;
            check(&r, &r, &r, &r, &rc);
      fin_mul[i]= SysTick->VAL;
  
      fin_mul[i] = ini_mul[i]-fin_mul[i];
  }

  sprintf(out, "check: %d, %d, %d, %d", fin_mul[0], fin_mul[1], fin_mul[2], fin_mul[3]);
  print(out);
  print("\n");

  for(i=0;i<=num-1;i++)
  {
            SysTick_Config(12000000);
      SysTick->VAL = 0;
      ini_mul[i]= SysTick->VAL;
            compress(&rc.fe.l1, &rc.fe.l2, &r);
      fin_mul[i]= SysTick->VAL;
  
      fin_mul[i] = ini_mul[i]-fin_mul[i];
  }

  sprintf(out, "compress: %d, %d, %d, %d", fin_mul[0], fin_mul[1], fin_mul[2], fin_mul[3]);
  print(out);
  print("\n");

  for(i=0;i<=num-1;i++)
  {
            SysTick_Config(12000000);
      SysTick->VAL = 0;
      ini_mul[i]= SysTick->VAL;
            decompress(&r, &r, &rc);
      fin_mul[i]= SysTick->VAL;
  
      fin_mul[i] = ini_mul[i]-fin_mul[i];
  }

  sprintf(out, "decompress: %d, %d, %d, %d", fin_mul[0], fin_mul[1], fin_mul[2], fin_mul[3]);
  print(out);
  print("\n");

    uint32 ch[1] = {0x12345678};
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

void SysTick_Handler(void)
{
  ;
}
