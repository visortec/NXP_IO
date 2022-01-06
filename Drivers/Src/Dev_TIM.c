/*
 * Dev_TIM.c
 *
 *  Created on: 06-Jan-2022
 *      Author: Durga prasad
 */

#include "MPC5744P.h"
#include "Dev_TIM.h"

void ETimer_ISR(void);

static uint32_t sMilliSecTimer =  0 ;



/*******************************************************************************
Function Name : ETimer_Init
Engineer      : Martin Kovar
Date          : Jan-12-2016
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : ETimer0 channel 0, count mode for delay, Primary source is MOTC_CLK divided by 128
Issues        : NONE
*******************************************************************************/
void ETimer_Init(void)
{
	    ETIMER_0.ENBL.R = 0x0;			// disable Timer0 channels
	    ETIMER_0.CH[0].CTRL1.R = 0x3F40;	 // Counts only rising edge of the MC_CLK (5MHz in RUN0), divide by 128, count up, count repeatedly, count until compare, then reinitialized
	    ETIMER_0.CH[0].COMP1.R = 0x9896;     // Compare value for 1 second delay (5MHz/128 = 39.063KHz)
	    ETIMER_0.CH[0].INTDMA.R = 0x0002;    // enable interrupt for COMP1
	    ETIMER_0.CH[0].CCCTRL.R = 0x0240;	 // compare on COMP1 when counting up, CAPT1 on rising edge,
	    ETIMER_0.ENBL.R = 0x0003;			// Enable Timer0 channel
}

uint32_t GetMilliSecCounter()
{
	return sMilliSecTimer;
}



/*******************************************************************************
Function Name : ETimer_ISR
Engineer      : Martin Kovar
Date          : Jan-12-2016
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : ETimer interrupt routine
Issues        : NONE
*******************************************************************************/
void ETimer_ISR(void)
{
	ETIMER_0.CH[0].STS.R = 0x0002;                     //clear interrupt flag
	/* millisec timer */
	sMilliSecTimer++;
}

