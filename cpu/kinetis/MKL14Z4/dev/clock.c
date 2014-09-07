/**
 * \file
 *			Clock.c
 * \author
 *			Alexander Winiger <alexander.winiger@stud.hslu.ch
 */

#include "contiki.h"
#include "sys/clock.h"
#include "sys/energest.h"
#include "sys/etimer.h"
#include "dev/watchdog.h"

#define CLK_WAIT_US(x)          WAIT1_Waitus(x)

/*--------------------------------------------------------------------------*/
static volatile unsigned long seconds;
static volatile clock_time_t count = 0;
/*---------------------------------------------------------------------------*/
void
SysTick_IRQHandler(void)
{
  ENERGEST_ON(ENERGEST_TYPE_IRQ);

  watchdog_start();

  ++count;

  if (count % CLOCK_CONF_SECOND == 0)
    {
      ++seconds;
      energest_flush();
    }

  if (etimer_pending())
    {
      etimer_request_poll();
    }

  watchdog_stop();

  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}
/*---------------------------------------------------------------------------*/
void
clock_init(void)
{

  SYST_RVR = 3906 * (F_CPU / 1000000); /* 7,8125ms time base */
  SYST_CVR = 0; // Reset the Current Value
  SYST_CSR = SysTick_CSR_ENABLE_MASK | SysTick_CSR_TICKINT_MASK
      | SysTick_CSR_CLKSOURCE_MASK;
}
/*---------------------------------------------------------------------------*/
clock_time_t
clock_time(void)
{
  return count;
}
/*---------------------------------------------------------------------------*/
unsigned long
clock_seconds(void)
{
  return seconds;
}
/*--------------------------------------------------------------------------*/
void
clock_delay(unsigned int i)
{
  CLK_WAIT_US(i * 2);
}
