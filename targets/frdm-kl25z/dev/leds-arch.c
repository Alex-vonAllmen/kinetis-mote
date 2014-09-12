/**
 * \file
 *         A brief description of what this file is.
 * \author
 *         Alexander Winiger <alexander.winiger@stud.hslu.ch>
 */

#include "contiki.h"
#include "dev/leds.h"

/*---------------------------------------------------------------------------*/
void
leds_arch_init(void)
{
	/**
	 * Initialize blue led 
	*/
	/* Configure pin as output */
    GPIOD_PDDR |= GPIO_PDDR_PDD(0x02);                                   
    /* Set initialization value */
    GPIOD_PDOR &= (uint32_t)~(uint32_t)(GPIO_PDOR_PDO(0x02));                                   
    /* Initialization of Port Control register */
    PORTD_PCR1 = (uint32_t)((PORTD_PCR1 & (uint32_t)~(uint32_t)(
                  PORT_PCR_ISF_MASK |
                  PORT_PCR_MUX(0x06)
                 )) | (uint32_t)(
                  PORT_PCR_MUX(0x01)
                 ));
	
	/**
	 * Initialize green led 
	*/
	/* Configure pin as output */
  	GPIOB_PDDR |= GPIO_PDDR_PDD(0x00080000);                                   
  	/* Set initialization value */
  	GPIOB_PDOR &= (uint32_t)~(uint32_t)(GPIO_PDOR_PDO(0x00080000));                                   
  	/* Initialization of Port Control register */
	PORTB_PCR19 = (uint32_t)((PORTB_PCR19 & (uint32_t)~(uint32_t)(
                 PORT_PCR_ISF_MASK |
                 PORT_PCR_MUX(0x06)
                )) | (uint32_t)(
                 PORT_PCR_MUX(0x01)
                ));
	   
  	/**
  	 * Initialize red led 
  	*/
  	/* Configure pin as output */
	GPIOB_PDDR |= GPIO_PDDR_PDD(0x00040000);                                   
  	/* Set initialization value */
	GPIOB_PDOR &= (uint32_t)~(uint32_t)(GPIO_PDOR_PDO(0x00040000));                                   
  	/* Initialization of Port Control register */
	PORTB_PCR18 = (uint32_t)((PORTB_PCR18 & (uint32_t)~(uint32_t)(
                 PORT_PCR_ISF_MASK |
                 PORT_PCR_MUX(0x06)
                )) | (uint32_t)(
                 PORT_PCR_MUX(0x01)
                ));         
}
/*---------------------------------------------------------------------------*/
unsigned char
leds_arch_get(void)
{
  return (
	  	 ((GPIOB_PDIR & 0x00080000) != 0U) ? 0 : LEDS_GREEN) | 
		 ((GPIOB_PDIR & 0x00040000) != 0U) ? 0 : LEDS_RED) | 
	     ((GPIOD_PDIR & 0x02) != 0U) ? 0 : LEDS_BLUE);
}
/*---------------------------------------------------------------------------*/
void
leds_arch_set(unsigned char leds)
{
  if (leds & LEDS_GREEN)
    {
      GPIOB_PSOR = GPIO_PSOR_PTSO(0x00080000);
    }
  else
    {
      GPIOB_PCOR |= GPIO_PCOR_PTCO(0x00080000);
    }
  if (leds & LEDS_RED)
    {
      GPIOB_PSOR = GPIO_PSOR_PTSO(0x00040000);
    }
  else
    {
      GPIOB_PCOR |= GPIO_PCOR_PTCO(0x00040000);
    }
  if (leds & LEDS_BLUE)
    {
      GPIOD_PSOR = GPIO_PSOR_PTSO(0x02);
    }
  else
    {
      GPIOD_PCOR |= GPIO_PCOR_PTCO(0x02);
    }
}
/*---------------------------------------------------------------------------*/
