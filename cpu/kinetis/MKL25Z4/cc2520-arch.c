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
  PORT_ISFR_REG(CC2520_FIFOP_PORT) = PORT_ISFR_ISF(0x01);
  /* Initialization of Port Control registers */
  PORT_PCR_REG(PCR_BASE(CC2520_FIFOP_PORT), CC2520_FIFOP_PIN) = (uint32_t) ((PORTD_PCR0
      & (uint32_t) ~(uint32_t) (PORT_PCR_ISF_MASK | PORT_PCR_IRQC(0x06)
          | PORT_PCR_MUX(0x06)))
      | (uint32_t) (PORT_PCR_IRQC(0x09) | PORT_PCR_MUX(0x01)));

  /* Set interrupt priority */
  NVIC_SetPriority(PORT_IRQ_VEC(CC2520_FIFOP_PORT), 1);

  /* Enable Port D interrupt */
  NVIC_EnableIRQ(PORT_IRQ_VEC(CC2520_FIFOP_PORT));

#if CONF_SFD_TIMESTAMPS
  cc2520_arch_sfd_init();
#endif

  CC2520_SPI_DISABLE(); /* Unselect radio. */
}
/*---------------------------------------------------------------------------*/
void
PORT_ISR(CC2520_FIFOP_PORT)(void)
{
  ENERGEST_ON(ENERGEST_TYPE_IRQ);
  
  PORT_PCR_REG(PCR_BASE(CC2520_FIFOP_PORT), CC2520_FIFOP_PIN) |= PORT_PCR_ISF_MASK;
  cc2520_interrupt();
  
  ENERGEST_ON(ENERGEST_TYPE_IRQ);
}
/*---------------------------------------------------------------------------*/

