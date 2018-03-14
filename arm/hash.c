/* Based on the public domain implementation in
 * crypto_hash/keccakc512/simple/ from http://bench.cr.yp.to/supercop.html
 * by Ronny Van Keer 
 * and the public domain "TweetFips202" implementation
 * from https://twitter.com/tweetfips202
 * by Gilles Van Assche, Daniel J. Bernstein, and Peter Schwabe 
 * and the public domain "A new hope on ARM Cortex-M"
 * from https://github.com/newhopearm/newhopearm/tree/master/cm0
 * by Erdem Alkim, Philipp Jakubeit and Peter Schwabe
 */

#include "hash.h"
#include "fe1271.h"

static void absorb_final(unsigned char *s,
                          const unsigned char *m, unsigned long long int mlen)
{
  KeccakF1600_StateXORBytes(s,m,0,mlen);
	s[8 * (mlen / 8) + (mlen & 7) / 2] ^= ( 0x07 << ( 4 * (mlen & 1) ) );
	s[8 * (mlen / 8) + (mlen & 7) / 2 + 4] ^= ( 0x03 << ( 4 * (mlen & 1) ) );
	s[167] ^= 128;
}

void hash(unsigned char *output, const unsigned char *input, unsigned int inputByteLen)
{
	union state s;

	KeccakF1600_StateInitialize(s.u64);

	for ( ; inputByteLen >= 168; inputByteLen -= 168, input += 168 )
	{
		KeccakF1600_StateXORBytes(s.u64, input, 0, 168);
		KeccakF1600_StatePermute(s.u64);
	}
	absorb_final(s.u8, input, inputByteLen);
	KeccakF1600_StatePermute(s.u64);

	KeccakF1600_StateExtractBytes(s.u64, output, 0, 64);
}
