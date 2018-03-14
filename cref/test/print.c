#include "print.h"

void fe1271_print(const unsigned char *x, const int len)
{
  int i;

	for(i=0;i<len;i++)
	{
		printf("%02x ",x[i]);
	}
}
