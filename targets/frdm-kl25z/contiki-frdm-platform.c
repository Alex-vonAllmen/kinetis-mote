/*
 * contiki-frdm-platform.c
 *
 *  Created on: May 20, 2014
 *      Author: alexanderwiniger
 */

#include "contiki.h"

#if PLATFORM_CONF_HAS_ACCELEROMETER
#include "dev/acc-sensor.h"

SENSORS(&acc_sensor);
#endif

void
ports_init(void)
{
  /* PORTA_PCR1: ISF=0,MUX=2 */
  PORTA_PCR1 = (uint32_t) ((PORTA_PCR1
      & (uint32_t) ~(uint32_t) (PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x05)))
      | (uint32_t) (PORT_PCR_MUX(0x02)));
  /* PORTA_PCR2: ISF=0,MUX=2 */
  PORTA_PCR2 = (uint32_t) ((PORTA_PCR2
      & (uint32_t) ~(uint32_t) (PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x05)))
      | (uint32_t) (PORT_PCR_MUX(0x02)));
  /* PORTE_PCR3: ISF=0,MUX=2 */
  PORTE_PCR3 = (uint32_t) ((PORTE_PCR3
      & (uint32_t) ~(uint32_t) (PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x05)))
      | (uint32_t) (PORT_PCR_MUX(0x02)));
  /* PORTD_PCR6: ISF=0,MUX=2 */
  PORTD_PCR6 = (uint32_t) ((PORTD_PCR6
      & (uint32_t) ~(uint32_t) (PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x05)))
      | (uint32_t) (PORT_PCR_MUX(0x02)));
  /* PORTE_PCR2: ISF=0,MUX=2 */
  PORTE_PCR2 = (uint32_t) ((PORTE_PCR2
      & (uint32_t) ~(uint32_t) (PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x05)))
      | (uint32_t) (PORT_PCR_MUX(0x02)));
}


void
platform_init(void)
{
#if PLATFORM_CONF_HAS_ACCELEROMETER
  process_start(&sensors_process, NULL);
#endif
}
