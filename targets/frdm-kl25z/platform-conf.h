/**
 * \file
 *         A brief description of what this file is.
 * \author
 *         Alexander Winiger <alexander.winiger@stud.hslu.ch>
 */

#ifndef PLATFORM_CONF_H_
#define PLATFORM_CONF_H_

#include "MKL25Z4.h"

/**
 * Port control register
 */
#define GPIO_BASE(port)		PT##port
#define PCR_BASE(port)		PORT##port
#define PORT_IRQ_VEC(port)	PORT##port_IRQn
#define PORT_ISR(port)		PORT##port_IRQHandler

/**
 * Definitions below are dictated by the hardware and not really
 * changeable!
 */
#define PLATFORM_HAS_LEDS          	1
#define PLATFORM_HAS_RADIO         	1
#define PLATFORM_HAS_TOUCH_SLIDER  	0
#define PLATFORM_HAS_ACCELEROMETER 	1
#define PLATFORM_HAS_USB           	1
#define PLATFORM_ENABLE_LOW_POWER  	0
#define PLATFORM_ENABLE_HEARTBEAT 	1

/**
 * CPU clock defintions
 */

/* CPU target speed in Hz */
extern uint32_t SystemCoreClock;
#define F_CPU SystemCoreClock

/* Our clock resolution, this is the same as Unix HZ. */
#define CLOCK_CONF_SECOND       256UL
#define RTIMER_CONF_USE_LPTMR   PLATFORM_CONF_ENABLE_LOW_POWER
#if RTIMER_CONF_USE_LPTMR
#define RTIMER_CONF_SECOND      1024U
#else
#define RTIMER_CONF_SECOND      (4096U*8)
#endif

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

/** 
 * UART Definitions
 */
/* Clock Source */
#define UART_IRCLK_SRC       0x03
#define UART_OSCER_SRC       0x02
#define UART_PLLFLL_SRC      0x01
#define UART_CLK_DIS         0x00

/* LPUART Pin definitions */
#ifdef LPUART_ENABLED
#define LPUART_TX_PIN_PORT		C
#define LPUART_TX_PIN_NUMBER	2
#define LPUART_RX_PIN_PORT		C
#define LPUART_RX_PIN_NUMBER	1
#endif

/* UART1 Pin definitions */
#ifdef UART1_ENABLED
#define UART1_TX_PIN_PORT
#define UART1_TX_PIN_NUMBER
#define UART1_RX_PIN_PORT
#define UART1_RX_PIN_NUMBER
#endif

/* UART2 Pin definitions */
#ifdef UART2_ENABLED
#define UART2_TX_PIN_PORT
#define UART2_TX_PIN_NUMBER
#define UART2_RX_PIN_PORT
#define UART2_RX_PIN_NUMBER
#endif

/* Console source configuration */
#define CONSOLE_ARCH_CONF_USB   (1 & PLATFORM_CONF_HAS_USB)
#define UART0_RX_WITH_DMA  		1
#define UART0_CLOCK_SRC    		UART_PLLFLL_SRC
#define SLIP_ARCH_CONF_USB      (0 & PLATFORM_CONF_HAS_USB)

/**
 * CC2520 configuration
 */

/* Peripheral configuration */
#define CC2520_SPI_IN_USE		SPI1	// Possibilities: SPI0/SPI1

/* Pin configuration */
#define CC2520_SPI_CLK_PORT		E
#define CC2520_SPI_CLK_PIN		2
#define CC2520_SPI_MISO_PORT	E
#define CC2520_SPI_MISO_PIN		3
#define CC2520_SPI_MOSI_PORT	D
#define CC2520_SPI_MOSI_PIN		6
#define CC2520_SPI_CSn_PORT		D
#define CC2520_SPI_CSn_PIN		7
#define CC2520_CLK_OUT_PORT		E
#define CC2520_CLK_OUT_PIN		4
#define CC2520_FIFO_PORT		E
#define CC2520_FIFO_PIN			5
#define CC2520_FIFOP_PORT		D
#define CC2520_FIFOP_PIN		0
#define CC2520_CCA_PORT			B
#define CC2520_CCA_PIN			9
#define CC2520_SFD_PORT			B
#define CC2520_SFD_PIN			10
#define CC2520_VREG_PORT		E
#define CC2520_VREG_PIN			0
#define CC2520_RESET_PORT		E
#define CC2520_RESET_PIN		1
/*
 * SPI bus configuration for the frdm-kl25z
 */

/* SPI input/output registers. */
#define SPI_TXBUF 	SPI_D_REG(CC2520_SPI_IN_USE)
#define SPI_RXBUF 	SPI_D_REG(CC2520_SPI_IN_USE)

/* SPI bus - CC2520 pin configuration. */
#define CC2520_CONF_SYMBOL_LOOP_COUNT   2604
#define CC2520_CONF_AUTOACK             0

#define SPI_WAITFOREOTx() while (!(SPI_S_REG(CC2520_SPI_IN_USE) & SPI_S_SPTEF_MASK)){ __asm("NOP"); }         \
                          while (!(SPI_S_REG(CC2520_SPI_IN_USE) & SPI_S_SPRF_MASK)) { __asm("NOP"); }         \
                          (void)SPI_RXBUF;
#define SPI_WAITFOREORx() while (!(SPI_S_REG(CC2520_SPI_IN_USE) & SPI_S_SPTEF_MASK)){ __asm("NOP"); }         \
                          while (!(SPI_S_REG(CC2520_SPI_IN_USE) & SPI_S_SPRF_MASK)) { __asm("NOP"); }
#define SPI_WAITFORTxREADY() while (!(SPI_S_REG(CC2520_SPI_IN_USE) & SPI_S_SPTEF_MASK)){ __asm("NOP"); }

/**
 * Enables/disables CC2520 access to the SPI bus (not the bus).
 * (Chip Select)
 */
#define CC2520_SPI_ENABLE()		(GPIO_PCOR_REG(GPIO_BASE(CC2520_SPI_CSn_PORT)) |= (1 << CC2520_SPI_CSn_PIN))
#define CC2520_SPI_DISABLE()	(GPIO_PSOR_REG(GPIO_BASE(CC2520_SPI_CSn_PORT)) |= (1 << CC2520_SPI_CSn_PIN))

/* Pin status CC2520 */
#define CC2520_FIFO_IS_1		(GPIO_PDIR_REG(GPIO_BASE(CC2520_FIFO_PORT)))
#define CC2520_FIFOP_IS_1       ((GPIO_PDIR_REG(GPIO_BASE(CC2520_FIFOP_PORT)) & (1 << CC2520_FIFOP_PIN)) >> CC2520_FIFOP_PIN)
#define CC2520_CCA_IS_1			((GPIO_PDIR_REG(GPIO_BASE(CC2520_CCA_PORT)) & (1 << CC2520_CCA_PIN)) >> CC2520_CCA_PIN)
#define CC2520_SFD_IS_1			((GPIO_PDIR_REG(GPIO_BASE(CC2520_SFD_PORT)) & (1 << CC2520_SFD_PIN)) >> CC2520_SFD_PIN)

/* The CC2520 reset pin. */
#define SET_RESET_INACTIVE()	(GPIO_PSOR_REG(GPIO_BASE(CC2520_RESET_PORT)) |= (1 << CC2520_RESET_PIN))
#define SET_RESET_ACTIVE()		(GPIO_PCOR_REG(GPIO_BASE(CC2520_RESET_PORT)) |= (1 << CC2520_RESET_PIN))

/* CC2520 voltage regulator enable pin. */
#define SET_VREG_ACTIVE()		(GPIO_PSOR_REG(GPIO_BASE(CC2520_VREG_PORT)) |= (1 << CC2520_VREG_PIN))
#define SET_VREG_INACTIVE()		(GPIO_PCOR_REG(GPIO_BASE(CC2520_VREG_PORT)) |= (1 << CC2520_VREG_PIN))

/* CC2520 rising edge trigger for external interrupt 0 (FIFOP). */
#define CC2520_FIFOP_INT_INIT()

/* FIFOP on external interrupt C4. */
#define CC2520_ENABLE_FIFOP_INT()       (PORT_PCR_REG(PCR_BASE(CC2520_FIFOP_PORT), \
						  							CC2520_FIFOP_PIN) |= (uint32_t) \
													(PORT_PCR_IRQC(0x09) | PORT_PCR_ISF_MASK ))
#define CC2520_DISABLE_FIFOP_INT()      (PORT_PCR_REG(PCR_BASE(CC2520_FIFOP_PORT), \
													CC2520_FIFOP_PIN) = (uint32_t) \
													((PORTD_PCR0 & ~(0x0F)) | PORT_PCR_ISF_MASK))
#define CC2520_CLEAR_FIFOP_INT()		(PORT_PCR_REG(PCR_BASE(CC2520_FIFOP_PORT), \
													CC2520_FIFOP_PIN) |= PORT_PCR_ISF_MASK)

#endif /* PLATFORM_CONF_H_ */
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
						  
