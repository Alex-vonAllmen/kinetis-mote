/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
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
 * -----------------------------------------------------------------
 *
 * Author  : Adam Dunkels, Joakim Eriksson, Niclas Finne
 * Created : 2005-11-01
 */

#include "contiki.h"
#include "dev/acc-sensor.h"

const struct sensors_sensor acc_sensor;
static uint8_t active;

/*---------------------------------------------------------------------------*/
static void
activate(void)
{
  MMA1_Enable();
}
/*---------------------------------------------------------------------------*/
static void
deactivate(void)
{
  MMA1_Disable();
}
/*---------------------------------------------------------------------------*/
static int
value(int type)
{
  switch (type)
    {
  case ACCEL_X_AXIS:
    return (MMA1_GetX() >> 8);

  case ACCEL_Y_AXIS:
    return (MMA1_GetY() >> 8);

  case ACCEL_Z_AXIS:
    return (MMA1_GetZ() >> 8);

  default:
    return 0;
    }
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
configure(int type, int value)
{
  switch (type)
    {
  case SENSORS_HW_INIT:
    return MMA1_Init();
  case SENSORS_ACTIVE:
    if (value)
      {
        activate();
      }
    else
      {
        deactivate();
      }
    return 1;
  case ACC_FAST_MODE:
    MMA1_SetFastMode(value);
    return 1;
    }
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
status(int type)
{
  switch (type)
    {
  case SENSORS_ACTIVE:
  case SENSORS_READY:
    MMA1_isEnabled(&active);
    return active;
  default:
    return 0;
    }
}
/*---------------------------------------------------------------------------*/
SENSORS_SENSOR( acc_sensor, ACC_SENSOR, value, configure, status);
