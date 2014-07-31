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
  /* part of PE_low_power_init() */
}
/*---------------------------------------------------------------------------*/
unsigned char
leds_arch_get(void)
{
  return (Led_Green_Get() ? 0 : LEDS_GREEN) | (Led_Red_Get() ? 0 : LEDS_RED)
      | (Led_Blue_Get() ? 0 : LEDS_BLUE);
}
/*---------------------------------------------------------------------------*/
void
leds_arch_set(unsigned char leds)
{
  if (leds & LEDS_GREEN)
    {
      Led_Green_On();
    }
  else
    {
      Led_Green_Off();
    }
  if (leds & LEDS_RED)
    {
      Led_Red_On();
    }
  else
    {
      Led_Red_Off();
    }
  if (leds & LEDS_BLUE)
    {
      Led_Blue_On();
    }
  else
    {
      Led_Blue_Off();
    }
}
/*---------------------------------------------------------------------------*/
