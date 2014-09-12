/**
 * \file
 *         A brief description of what this file is.
 * \author
 *         Alexander Winiger <alexander.winiger@stud.hslu.ch>
 */

#ifndef PLATFORM_CONF_H_
#define PLATFORM_CONF_H_

#include "MKL25Z4.h"

/*
 * Definitions below are dictated by the hardware and not really
 * changeable!
 */
#define PLATFORM_CONF_HAS_LEDS          1
#define PLATFORM_CONF_HAS_RADIO         1
#define PLATFORM_CONF_HAS_EEPROM        0
#define PLATFORM_CONF_HAS_ACCELEROMETER 1
#define PLATFORM_CONF_HAS_USB           1
#define PLATFORM_CONF_ENABLE_LOW_POWER  0
#define PLATFORM_CONF_ENABLE_HEARTBEAT  1

#ifdef __CYGWIN__
int strcasecmp(const char*, const char*);
int strncasecmp(const char*, const char*, size_t);
char* strdup(const char*);
#else /* __CYGWIN__ */
#define HAVE_SNPRINTF
#define snprintf    _snprintf
#define strcasecmp  _stricmp
#define strncasecmp _strnicmp
#define strdup      _strdup
#endif /* __CYGWIN__ */

/* Compiler configurations */
#define CCIF
#define CLIF

#ifndef BV
#define BV(x) (1<<(x))
#endif

// needed for CC2520 to work
#define splhigh() 0
#define splx(arg)

/* Platform typedefs */
typedef uint32_t clock_time_t;
typedef uint32_t uip_stats_t;

extern uint32_t SystemCoreClock;

/* CPU target speed in Hz */
#define F_CPU SystemCoreClock

/* Our clock resolution, this is the same as Unix HZ. */
#define CLOCK_CONF_SECOND       256UL
#define RTIMER_CONF_USE_LPTMR   PLATFORM_CONF_ENABLE_LOW_POWER
#if RTIMER_CONF_USE_LPTMR
#define RTIMER_CONF_SECOND      1024U
#else
#define RTIMER_CONF_SECOND      (4096U*8)
#endif

// UART Definitions
// Clock Source
#define UART0_IRCLK_SRC       0x03
#define UART0_OSCER_SRC       0x02
#define UART0_PLLFLL_SRC      0x01
#define UART0_CLK_DIS         0x00

/* Console source configuration */
#define CONSOLE_ARCH_CONF_USB   (1 & PLATFORM_CONF_HAS_USB)
#define UART0_CONF_RX_WITH_DMA  1
#define UART0_CONF_CLOCK_SRC    UART0_PLLFLL_SRC
#define SLIP_ARCH_CONF_USB      (0 & PLATFORM_CONF_HAS_USB)

/*
 * SPI bus configuration for the frdm-kl25z
 */
/* SPI input/output registers. */
#define SPI_TXBUF SPI_D_REG(SPI1_BASE_PTR)
#define SPI_RXBUF SPI_D_REG(SPI1_BASE_PTR)

/* SPI bus - CC2520 pin configuration. */
#define CC2520_CONF_SYMBOL_LOOP_COUNT   2604
#define CC2520_CONF_AUTOACK             0

#define SPI_WAITFOREOTx() while (!(SPI1_S & SPI_S_SPTEF_MASK)){ __asm("NOP"); }         \
                          while (!(SPI1_S & SPI_S_SPRF_MASK)) { __asm("NOP"); }         \
                          (void)SPI_RXBUF;
#define SPI_WAITFOREORx() while (!(SPI1_S & SPI_S_SPTEF_MASK)){ __asm("NOP"); }         \
                          while (!(SPI1_S & SPI_S_SPRF_MASK)) { __asm("NOP"); }
#define SPI_WAITFORTxREADY() while (!(SPI1_S & SPI_S_SPTEF_MASK)){ __asm("NOP"); }

/*
 * Enables/disables CC2520 access to the SPI bus (not the bus).
 * (Chip Select)
 */
#define CC2520_SPI_ENABLE()
#define CC2520_SPI_DISABLE()

/* Pin status.CC2520 */
#define CC2520_FIFO_IS_1
#define CC2520_FIFOP_IS_1       (GPIO_PDIR_REG(FPTD_BASE_PTR) & 0x01)
#define CC2520_CCA_IS_1
#define CC2520_SFD_IS_1

/* The CC2520 reset pin. */
#define SET_RESET_INACTIVE()
#define SET_RESET_ACTIVE()

/* CC2520 voltage regulator enable pin. */
#define SET_VREG_ACTIVE()
#define SET_VREG_INACTIVE()

/* CC2520 rising edge trigger for external interrupt 0 (FIFOP). */
#define CC2520_FIFOP_INT_INIT()

/* FIFOP on external interrupt C4. */
#define CC2520_ENABLE_FIFOP_INT()       (PORTD_PCR0 |= (uint32_t) (PORT_PCR_IRQC(0x09) | PORT_PCR_ISF_MASK ))
#define CC2520_DISABLE_FIFOP_INT()      (PORTD_PCR0 = (uint32_t) ((PORTD_PCR0 & ~(0x0F)) | PORT_PCR_ISF_MASK))
#define CC2520_CLEAR_FIFOP_INT()

#endif /* PLATFORM_CONF_H_ */
