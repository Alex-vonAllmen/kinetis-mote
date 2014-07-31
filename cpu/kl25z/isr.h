/******************************************************************************
* File:    isr.h
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H 1

/******* UARTO Interrupt Service Routine ******/
#undef  VECTOR_028
#define VECTOR_028 uart0_isr

extern void uart0_isr(void);

#endif  //__ISR_H

/* End of "isr.h" */
