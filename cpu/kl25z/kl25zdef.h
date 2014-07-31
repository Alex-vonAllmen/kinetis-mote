/*
 * kl25zdef.h
 *
 *  Created on: Jun 11, 2014
 *      Author: alexanderwiniger
 */

#ifndef KL25ZDEF_H_
#define KL25ZDEF_H_

#ifdef HAVE_STDINT_H
#include <stdint.h>
#else
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned long   uint32_t;
typedef   signed char   int8_t;
typedef          short  int16_t;
typedef          long   int32_t;
#endif /* !HAVE_STDINT_H */

#include "common.h"

extern unsigned char mkl25z_uid[10];

void mkl25z_cpu_init(void);

#endif /* KL25ZDEF_H_ */
