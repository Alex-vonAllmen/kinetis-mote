/*
 * uart0.h
 *
 *  Created on: Jul 31, 2014
 *      Author: alexanderwiniger
 */

#ifndef UART0_H_
#define UART0_H_

#include "contiki.h"

void uart0_set_input(int (*input)(unsigned char c));
void uart0_writeb(unsigned char c);
void uart0_init(unsigned long ubr);
uint8_t uart0_active(void);

#endif /* UART0_H_ */
