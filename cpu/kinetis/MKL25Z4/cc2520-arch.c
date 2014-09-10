/*
 * cc2520-arch.c
 *
 *  Created on: Mar 28, 2014
 *      Author: alexanderwiniger
 */

#include "contiki.h"
#include "contiki-net.h"

#include "dev/spi.h"
#include "dev/cc2520/cc2520.h"

#include "core_cm0plus.h"

#ifdef CC2520_CONF_SFD_TIMESTAMPS
#define CONF_SFD_TIMESTAMPS CC2520_CONF_SFD_TIMESTAMPS
#endif /* CC2520_CONF_SFD_TIMESTAMPS */

#ifndef CONF_SFD_TIMESTAMPS
#define CONF_SFD_TIMESTAMPS 0
#endif /* CONF_SFD_TIMESTAMPS */

#ifdef CONF_SFD_TIMESTAMPS
#include "cc2520-arch-sfd.h"
#endif

/*---------------------------------------------------------------------------*/
void
cc2520_arch_init(void)
{
  spi_init();
  
  /* Clear interrupt status flag */
  PORTD_ISFR = PORT_ISFR_ISF(0x01);
  /* Initialization of Port Control registers */
  /* PORTD_PCR0: ISF=0,IRQC=9,MUX=1 */
  PORTD_PCR0 = (uint32_t) ((PORTD_PCR0
      & (uint32_t) ~(uint32_t) (PORT_PCR_ISF_MASK | PORT_PCR_IRQC(0x06)
          | PORT_PCR_MUX(0x06)))
      | (uint32_t) (PORT_PCR_IRQC(0x09) | PORT_PCR_MUX(0x01)));

  /* Set interrupt priority */
  NVIC_SetPriority(PORTD_IRQn, 1);

  /* Enable Port D interrupt */
  NVIC_EnableIRQ(PORTD_IRQn);

#if CONF_SFD_TIMESTAMPS
  cc2520_arch_sfd_init();
#endif

  CC2520_SPI_DISABLE(); /* Unselect radio. */
}
/*---------------------------------------------------------------------------*/
void
PORTD_IRQHandler(void)
{
  ENERGEST_ON(ENERGEST_TYPE_IRQ);
  
  PORTD_PCR0 |= PORT_PCR_ISF_MASK;
  cc2520_interrupt();
  
  ENERGEST_ON(ENERGEST_TYPE_IRQ);
}
/*---------------------------------------------------------------------------*/

