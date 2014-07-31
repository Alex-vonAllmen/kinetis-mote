/**
 * \file		Includes main(), initializes MC, starts Contiki
 * \author		Alexander Winiger <alexander.winiger@stud.hslu.ch>
 * \version		0.1
 * \since		06.03.2014
 */
/*---------------------------------------------------------------------------*/
#include "contiki.h"

#include "dev/serial-line.h"
#include "dev/slip.h"
#include "dev/watchdog.h"
#include "lib/random.h"

#if PLATFORM_CONF_HAS_LEDS
#include "dev/leds.h"
#endif

#if PLATFORM_CONF_HAS_RADIO
#include "dev/cc2520/cc2520.h"
#include "net/netstack.h"
#include "net/mac/frame802154.h"
#endif

#if WITH_UIP6
#include "net/ipv6/uip-ds6.h"
#endif /* WITH_UIP6 */

#include "net/rime/rime.h"

#include "sys/node-id.h"
#include "sys/autostart.h"

#if UIP_CONF_ROUTER

#ifndef UIP_ROUTER_MODULE
#ifdef UIP_CONF_ROUTER_MODULE
#define UIP_ROUTER_MODULE UIP_CONF_ROUTER_MODULE
#else /* UIP_CONF_ROUTER_MODULE */
#define UIP_ROUTER_MODULE rimeroute
#endif /* UIP_CONF_ROUTER_MODULE */
#endif /* UIP_ROUTER_MODULE */

extern const struct uip_router UIP_ROUTER_MODULE;
#endif /* UIP_CONF_ROUTER */

#ifndef WITH_UIP
#define WITH_UIP 0
#endif

#if WITH_UIP
#include "net/ip/uip.h"
#include "net/ipv4/uip-fw.h"
#include "net/uip-fw-drv.h"
#include "net/ipv4/uip-over-mesh.h"
static struct uip_fw_netif slipif =
  { UIP_FW_NETIF(192,168,1,2, 255,255,255,255, slip_send)};
static struct uip_fw_netif meshif =
  { UIP_FW_NETIF(172,16,0,0, 255,255,0,0, uip_over_mesh_send)};

#endif /* WITH_UIP */

#define UIP_OVER_MESH_CHANNEL 8
#if WITH_UIP
static uint8_t is_gateway;
#endif /* WITH_UIP */

void
ports_init(void);
void
platform_init(void);


/*---------------------------------------------------------------------------*/
#if !PROCESS_CONF_NO_PROCESS_NAMES && DEBUG
static void
print_processes(struct process * const processes[])
{
  /*  const struct process * const * p = processes;*/
  PRINTF("Starting");
  while (*processes != NULL )
    {
      PRINTF(" '%s'", (*processes)->name);
      processes++;
    }
  PRINTF('\n');
}
#endif /* !PROCESS_CONF_NO_PROCESS_NAMES */
/*--------------------------------------------------------------------------*/
int
main(void)
{
  /* Hardware initialization */
  clock_init();
  leds_init();
  
  rtimer_init();

  node_id_restore();

  process_init();
  process_start(&etimer_process, NULL );

  ctimer_init();
  platform_init();

  watchdog_start();
  /* Stop the watchdog */
  watchdog_stop();

  autostart_start(autostart_processes);

  /*
   * This is the scheduler loop.
   */
  while (1)
    {
      int r;
      do
        {
          /* Reset watchdog. */
          watchdog_periodic();
          r = process_run();
        }
      while (r > 0);
#if PLATFORM_CONF_ENABLE_LOW_POWER
      /*
       * Idle processing.
       */
      if (process_nevents() != 0 /* || uart0_active() */)
        {
        }
      else
        {
          static unsigned long irq_energest = 0;

          /* Re-enable interrupts and go to sleep atomically. */
          ENERGEST_OFF(ENERGEST_TYPE_CPU);
          ENERGEST_ON(ENERGEST_TYPE_LPM);
          /* We only want to measure the processing done in IRQs when we
           are asleep, so we discard the processing time done when we
           were awake. */
          energest_type_set(ENERGEST_TYPE_IRQ, irq_energest);
          watchdog_stop();
          Cpu_SetOperationMode(DOM_WAIT, NULL, NULL ); /* next interrupt will wake us up */
          /* We get the current processing time for interrupts that was
           done during the LPM and store it for next time around.  */
          __DI();
          irq_energest = energest_type_time(ENERGEST_TYPE_IRQ);
          __EI();
          watchdog_start();
          ENERGEST_OFF(ENERGEST_TYPE_LPM);
          ENERGEST_ON(ENERGEST_TYPE_CPU);
        }
#endif /* PLATFORM_CONF_ENABLE_LOW_POWER */
    }

  return 0;
}
