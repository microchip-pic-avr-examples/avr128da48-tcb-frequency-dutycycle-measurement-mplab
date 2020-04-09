/*
 * File:   application_isr.c
 * Author: I51005
 *
 * Created on March 31, 2020, 11:55 AM
 */


#include <xc.h>
#include "mcc_generated_files/include/rtc.h"
#include "application_isr.h"
#include "application.h"
#include "mcc_generated_files/include/tcb2.h"



extern uint8_t  rtcFlag;
extern uint32_t periodSum;
extern uint32_t pulseWidthSum;
extern uint8_t  samplescount;

void (*TCB2_CAPT_isr_cb)(void) = NULL;
void (*RTC_PIT_isr_cb)(void) = NULL;

/**
 * **********************************************************
 *  ISR

    For every one second periodic interrupt is generated
 * **********************************************************
 **/
ISR(RTC_PIT_vect)
{
    rtcFlag=1;
   if (RTC_PIT_isr_cb != NULL) 
   {
    (*RTC_PIT_isr_cb)();
   } 
   RTC.PITINTFLAGS = RTC_PI_bm;
}

/**
 * *********************************************************************************
 *  ISR

    Capture Interrupt is generated for every one period measurement of the signal
 * *********************************************************************************
 **/

ISR(TCB2_INT_vect)
{
	/* Insert your TCB interrupt handling code */

    if (samplescount < MAXSAMPLES_COUNT) 
    {
		pulseWidthSum += TCB2.CCMP;

		periodSum += TCB2.CNT;

		samplescount = samplescount + 1;
	} 
    else 
    {
		// no operation
	}
	/**
	 * The interrupt flag is cleared by writing 1 to it, or when the Capture register
	 * is read in Capture mode
	 */
	 if(TCB2.INTFLAGS & TCB_CAPT_bm)
        {
            if (TCB2_CAPT_isr_cb != NULL)
            {
                (*TCB2_CAPT_isr_cb)();
            }

            TCB2.INTFLAGS = TCB_CAPT_bm;
        }
	 
}


