/*
 * Dev_TIM.c
 *
 *  Created on: 06-Jan-2022
 *      Author: Durga prasad
 */

#include "MPC5744P.h"
#include "Dev_TIM.h"

void ETimer_ISR(void);
void PIT0_Sec_ISR(void);

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

/*
 *  Initalize PIT channel 0 for desired SYSCLK counts
 *  timeout= 40M  PITclks x 4 sysclks/1 PITclk x 1 sec/160Msysck
 *   = 40M x 4 / 160M = 160/160 = 1 sec.
 */
void Dev_PIT0_Init(uint32_t LDVAL)
{
    PIT_0.MCR.B.MDIS = 0; /* Enable PIT module. NOTE: PIT module must be       */
                        /* enabled BEFORE writing to it's registers.         */
                        /* Other cores will write to PIT registers so the    */
                        /* PIT is enabled here before starting other cores.  */
    PIT_0.MCR.B.FRZ = 1;  /* Freeze PIT timers in debug mode */

    PIT0_init(20000000);

    PIT_0.MCR.B.FRZ = 0; //Unfreeze timers

}


uint32_t GetMilliSecCounter()
{
	return sMilliSecTimer;
}


void PIT0_init(uint32_t LDVAL)
{
	PIT_0.TIMER[0].LDVAL.R = LDVAL; /* Load # PIT clocks to count */
	PIT_0.TIMER[0].TCTRL.B.TIE = 1; /* Enable interrupt */
	INTC_0.PSR[226].R = 0x800A;
	PIT_0.TIMER[0].TCTRL.B.TEN = 1; /* enable channel */

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

void PIT0_Sec_ISR(void)
{
	/* Increment ISR counter */
	sMilliSecTimer++;
    PIT_0.TIMER[0].TFLG.R |= 1;  /* Clear interrupt flag. w1c */
}

