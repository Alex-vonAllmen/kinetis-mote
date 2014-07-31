/*
 * uart0.c
 *
 *  Created on: Jul 31, 2014
 *      Author: alexanderwiniger
 */

#include <stdlib.h>

#include "contiki.h"
#include "dev/uart0.h"

static int (*uart0_input_handler)(unsigned char c);

static volatile uint8_t transmitting;

/*---------------------------------------------------------------------------*/
uint8_t
uart0_active(void)
{
  return transmitting;
}
/*---------------------------------------------------------------------------*/
void
uart0_set_input(int (*input)(unsigned char c))
{
  uart0_input_handler = input;
}
/*---------------------------------------------------------------------------*/
void
uart0_writeb(unsigned char c)
{

}
/*---------------------------------------------------------------------------*/
void
uart0_init(unsigned long ubr)
{

}
/*---------------------------------------------------------------------------*/
void uart0_isr(void)
{

}
