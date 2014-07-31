/*
 * Copyright (c) 2011, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *			kl25z-specific rtimer code for Freescale KL25Z
 * \author
 *			Alexander Winiger <alexander.winiger@stud.hslu.ch>
 */

#include "contiki.h"
#include "sys/energest.h"
#include "sys/rtimer.h"
#include "dev/watchdog.h"
#include "dev/leds.h"

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#if RTIMER_CONF_USE_LPTMR
#define RT_ENABLE() do { LPTMR0_CSR |= LPTMR_CSR_TEN_MASK; } while(0)
#define RT_DISABLE() do { LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; } while(0)
#else
#define RT_MODE_COMPARE() do { TPM0_C0SC |= TPM_CnSC_MSA_MASK; } while(0)
#define RT_MODE_CAPTURE() do { TPM0_C0SC &= ~TPM_CnSC_MSA_MASK; } while(0)
#endif

/*---------------------------------------------------------------------------*/
#if RTIMER_CONF_USE_LPTMR
void TPM0_IRQHandler(void) {}
/*---------------------------------------------------------------------------*/
void
LPTMR_IRQHandler(void)
  {
    ENERGEST_ON(ENERGEST_TYPE_IRQ);

    watchdog_start();
    
    PRINTF("rtimer_arch_init compare event at 0x%4x.\n", rtimer_arch_now());

    LPTMR0_CMR = LPTMR_CMR_COMPARE(LPTMR_CMR_COMPARE_MASK);
    LPTMR0_CSR = (uint32_t) ((LPTMR0_CSR
        & (uint32_t) ~(uint32_t) (LPTMR_CSR_TIE_MASK))
    | (uint32_t) (LPTMR_CSR_TCF_MASK)); // Clear interrupt flag and disable interrupt

    rtimer_run_next();

    watchdog_stop();

    ENERGEST_OFF(ENERGEST_TYPE_IRQ);
  }
/*---------------------------------------------------------------------------*/
void Cpu_OnLLSWakeUpINT(void)
  {
    uint32_t tmp;

    tmp = Cpu_GetLLSWakeUpFlags();
    
    PRINTF("rtimer_arch_init wake up interrupt.\n");

    if (tmp & LLWU_INT_MODULE0)
      { /* LPTMR */
#if 1
        leds_off(LEDS_YELLOW);
        WAIT1_Waitms(1);
        leds_on(LEDS_YELLOW);
        WAIT1_Waitms(100);
#endif
        LPTMR_CSR_REG(LPTMR0_BASE_PTR) |= LPTMR_CSR_TCF_MASK; /* Clear interrupt flag */
      }
  }
#else
void LPTMR_IRQHandler(void) {}
void Cpu_OnLLSWakeUpINT(void) {}
#endif /* RTIMER_CONF_USE_LPTMR */
/*---------------------------------------------------------------------------*/
#if !RTIMER_CONF_USE_LPTMR
void
TPM0_IRQHandler(void)
{
  ENERGEST_ON(ENERGEST_TYPE_IRQ);

  if (TPM_STATUS_CH0F_MASK & TPM0_STATUS)
    {
      /* disable tmp0 channel 0 interrupts */
      TPM0_C0SC &= ~TPM_CnSC_CHIE_MASK;
    }

  /* Clear interrupt flags */
  TPM0_STATUS = (TPM_STATUS_CH0F_MASK | TPM_STATUS_CH1F_MASK
      | TPM_STATUS_CH2F_MASK | TPM_STATUS_CH3F_MASK | TPM_STATUS_CH4F_MASK
      | TPM_STATUS_CH5F_MASK | TPM_STATUS_TOF_MASK);
    
  rtimer_run_next();

  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}
#endif /* !RTIMER_CONF_USE_LPTMR */
/*---------------------------------------------------------------------------*/
rtimer_clock_t
rtimer_arch_now(void)
{
  rtimer_clock_t t;
#if RTIMER_CONF_USE_LPTMR
  LPTMR0_CNR = LPTMR_CNR_COUNTER(0xFFFFu);
  t = (rtimer_clock_t) LPTMR_CNR_COUNTER(LPTMR0_CNR);
#else
  t = (rtimer_clock_t)TPM0_CNT;
#endif
  return t;
}
/*---------------------------------------------------------------------------*/
void
rtimer_arch_init(void)
{
#if RTIMER_CONF_USE_LPTMR
  /* SIM_SCGC5: LPTMR=1 */
  SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;
  
  LPTMR0_CSR = (LPTMR_CSR_TCF_MASK | LPTMR_CSR_TPS(0x00) | LPTMR_CSR_TFC_MASK); /* Clear control register */
  LPTMR0_CMR = LPTMR_CMR_COMPARE(LPTMR_CMR_COMPARE_MASK); /* Set up compare register */
  LPTMR0_PSR = LPTMR_PSR_PRESCALE(0x00) |
                 LPTMR_PSR_PBYP_MASK |
                 LPTMR_PSR_PCS(0x01);    /* Set up prescaler register */
  LPTMR0_CSR = (LPTMR_CSR_TPS(0x00) | LPTMR_CSR_TEN_MASK); /* Set up control register */
#else
  /* SIM_SCGC6: TPM0=1 */
  SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;                                   
  /* TPM0_SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,DMA=0,TOF=0,TOIE=0,CPWMS=0,CMOD=0,PS=0 */
  TPM0_SC = (TPM_SC_CMOD(0x00) | TPM_SC_PS(0x00)); /* Clear status and control register */
  /* TPM0_CNT: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,COUNT=0 */
  TPM0_CNT = TPM_CNT_COUNT(0x00);      /* Reset counter register */
  /* TPM0_C0SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CHF=0,CHIE=0,MSB=0,MSA=0,ELSB=0,ELSA=0,??=0,DMA=0 */
  TPM0_C0SC = 0x00U;                   /* Clear channel status and control register */
  /* TPM0_C1SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CHF=0,CHIE=0,MSB=0,MSA=0,ELSB=0,ELSA=0,??=0,DMA=0 */
  TPM0_C1SC = 0x00U;                   /* Clear channel status and control register */
  /* TPM0_C2SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CHF=0,CHIE=0,MSB=0,MSA=0,ELSB=0,ELSA=0,??=0,DMA=0 */
  TPM0_C2SC = 0x00U;                   /* Clear channel status and control register */
  /* TPM0_C3SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CHF=0,CHIE=0,MSB=0,MSA=0,ELSB=0,ELSA=0,??=0,DMA=0 */
  TPM0_C3SC = 0x00U;                   /* Clear channel status and control register */
  /* TPM0_C4SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CHF=0,CHIE=0,MSB=0,MSA=0,ELSB=0,ELSA=0,??=0,DMA=0 */
  TPM0_C4SC = 0x00U;                   /* Clear channel status and control register */
  /* TPM0_C5SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CHF=0,CHIE=0,MSB=0,MSA=0,ELSB=0,ELSA=0,??=0,DMA=0 */
  TPM0_C5SC = 0x00U;                   /* Clear channel status and control register */
  /* TPM0_MOD: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,MOD=0xFFFF */
  TPM0_MOD = TPM_MOD_MOD(0xFFFF);      /* Set up modulo register */
  /* TPM0_C0SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CHF=0,CHIE=1,MSB=0,MSA=1,ELSB=0,ELSA=0,??=0,DMA=0 */
  TPM0_C0SC = (TPM_CnSC_CHIE_MASK | TPM_CnSC_MSA_MASK); /* Set up channel status and control register */
  /* TPM0_C0V: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,VAL=0xF424 */
  TPM0_C0V = TPM_CnV_VAL(0x00);      /* Set up channel value register */
  /* TPM0_SC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,DMA=0,TOF=0,TOIE=0,CPWMS=0,CMOD=1,PS=5 */
  TPM0_SC = (TPM_SC_CMOD(0x01) | TPM_SC_PS(0x05)); /* Set up status and control register */
#endif /* RTIMER_CONF_USE_LPTMR */
  PRINTF("rtimer_arch_init done\n");
}
/*---------------------------------------------------------------------------*/
void
rtimer_arch_schedule(rtimer_clock_t t)
{
  PRINTF("rtimer_arch_schedule time 0x%4x\n",(unsigned int)t);
#if RTIMER_CONF_USE_LPTMR
  RT_DISABLE(); /* Disable LPTMR to write to CMR register */

  LPTMR0_CMR = LPTMR_CMR_COMPARE(t);

  /* enable LPTMR interrupts and clear interrupt flag */
  LPTMR0_CSR |= (LPTMR_CSR_TCF_MASK | LPTMR_CSR_TIE_MASK);

  RT_ENABLE(); /* Re-enable LPTMR */
#else
  RT_MODE_CAPTURE();
  
  TPM0_C0V = TPM_CnV_VAL(t);

  RT_MODE_COMPARE();

  /* enable tmp0 channel 0 interrupts */
  TPM0_C0SC |= TPM_CnSC_CHIE_MASK;
  TPM0_STATUS = (TPM_STATUS_CH0F_MASK | TPM_STATUS_CH1F_MASK
      | TPM_STATUS_CH2F_MASK | TPM_STATUS_CH3F_MASK | TPM_STATUS_CH4F_MASK
      | TPM_STATUS_CH5F_MASK | TPM_STATUS_TOF_MASK);
#endif /* RTIMER_CONF_USE_LPTMR */
}
/*---------------------------------------------------------------------------*/
