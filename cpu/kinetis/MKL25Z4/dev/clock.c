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

#define CLK_WAIT_US(x)

/*--------------------------------------------------------------------------*/
static volatile unsigned long seconds;
static volatile clock_time_t count = 0;
/*---------------------------------------------------------------------------*/
void
SysTick_Handler(void)
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
	SysTick_Config(3906 * (F_CPU / 1000000)); /* 7,8125ms time base */
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
clock_delay_usec(uint16_t dt)
{

}
