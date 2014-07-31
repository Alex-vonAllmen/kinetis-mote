/*
 * mkl25z.c
 *
 *  Created on: May 10, 2014
 *      Author: alexanderwiniger
 */

#include "contiki.h"
#include "dev/watchdog.h"

unsigned char mkl25z_uid[10];

void
mkl25z_cpu_init(void)
{
  /* initialize kinetis unique identifier */
  memcpy(&mkl25z_uid, &SIM_UIDL, sizeof(SIM_UIDL));
  memcpy(&mkl25z_uid[4], &SIM_UIDML, sizeof(SIM_UIDML));
  memcpy(&mkl25z_uid[8], &SIM_UIDMH, 2);
}
