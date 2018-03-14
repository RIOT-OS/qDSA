#ifndef QUAD_H
#define QUAD_H

#include "ladder.h"
#include "compress.h"

extern const unsigned char ehat_1[2];
extern const unsigned char ehat_2[2];
extern const unsigned char ehat_3[2];
extern const unsigned char ehat_4[2];

int check(kp *sP, kp *hQ, kp *R, kp *t, kpc *xr);

#endif
