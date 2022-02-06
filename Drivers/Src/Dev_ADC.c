/*
 * Dev_ADC.c
 *
 *  Created on: 05-Jan-2022
 *      Author: Durga prasad
 */


/*
 * adc.c
 *
 *  Created on: Mar 3, 2016
 *      Author: B55457
 */


#include "Dev_ADC.h"
#include "project.h"


#define ADC_VREF  3300 /* ADC ref voltage for both ADC modules. 3300mv or 5000mv */
static uint16_t Result[3];     /* ADC channel conversion results */
uint16_t ResultInMv[3]; /* ADC channel conversion results in mv */

void ADC_PadConfig_ChanSelect(void)
{ /* Config ADC pads & select scan chans */
     /* Note:  MSCR.SSS configuration  is not needed for inputs if there is  */
     /*        no SSS value is in signal spreadsheet */
     /* Note:  ADC1 Channel 6 on PE12 is connected to XDEVKIT-MPC5744P potentiometer. */

	SIUL2.MSCR[PB8].B.APC = 1; //Configure ADC1_AN6 to analog pad control function. Analog is default function so SSS says 0
	SIUL2.MSCR[PB12].B.APC = 1; //Configure ADC1_AN6 to analog pad control function. Analog is default function so SSS says 0
	SIUL2.MSCR[PB9].B.APC = 1; //Configure ADC1_AN6 to analog pad control function. Analog is default function so SSS says 0
	SIUL2.MSCR[PB10].B.APC = 1; //Configure ADC1_AN6 to analog pad control function. Analog is default function so SSS says 0
	SIUL2.MSCR[PB11].B.APC = 1; //Configure ADC1_AN6 to analog pad control function. Analog is default function so SSS says 0
	SIUL2.MSCR[PB7].B.APC = 1; //Configure ADC1_AN6 to analog pad control function. Analog is default function so SSS says 0
	SIUL2.MSCR[PB13].B.APC = 1; //Configure ADC1_AN6 to analog pad control function. Analog is default function so SSS says 0

	ADC_0.NCMR0.B.CH0 = 1; //Enable channel 0 for normal conversion on ADC0
	ADC_0.NCMR0.B.CH1 = 1; //Enable channel 1 for normal conversion on ADC0
	ADC_0.NCMR0.B.CH11 = 1;//Enable channel 11 for normal conversion on ADC0
	ADC_0.NCMR0.B.CH12 = 1;//Enable channel 12 for normal conversion on ADC0
	ADC_0.NCMR0.B.CH13 = 1;//Enable channel 13 for normal conversion on ADC0
	ADC_0.NCMR0.B.CH14 = 1;//Enable channel 14 for normal conversion on ADC0

	ADC_1.NCMR0.B.CH0 = 1; //Enable channel 0 for normal conversion on ADC1
	ADC_1.NCMR0.B.CH1 = 1; //Enable channel 1 for normal conversion on ADC1

}

void ADC1_Calibration(void) {       /* Steps below are from reference manual */
  uint32_t ADC1_Calibration_Failed = 1;    /* Calibration has not passed yet */

  ADC_1.MCR.B.PWDN = 1;     /* Power down for starting calibration process */
  ADC_1.MCR.B.ADCLKSEL = 0; /* ADC clock = bus clock/2 (80/2 MHz FS80) */
        /* Note: Calibration requires it to be set at max 40MHz for best results */
  ADC_1.CALBISTREG.B.TEST_EN = 1;       /* Enable calibration test */
  ADC_1.MCR.B.PWDN = 0;     /* Power back up for calibration test to start */
  while(ADC_1.CALBISTREG.B.C_T_BUSY);  /* Wait for calibration to finish */
  if(ADC_1.MSR.B.CALIBRTD) {            /* If calibration ran successfully */
    ADC1_Calibration_Failed = 0;        /* Calibration was successful */
  }
  else  {
    ADC1_Calibration_Failed = 1;        /* Calibration was not successful */
  }

  if(ADC1_Calibration_Failed){} //Functionally useless statement to make compiler happy

}

void ADC0_Calibration(void)
{       /* Steps below are from reference manual */
  uint32_t ADC0_Calibration_Failed = 1;    /* Calibration has not passed yet */

  ADC_0.MCR.B.PWDN = 1;     /* Power down for starting calibration process */
  ADC_0.MCR.B.ADCLKSEL = 0; /* ADC clock = bus clock/2 (80/2 MHz FS80) */
        /* Note: Calibration requires it to be set at max 40MHz for best results */
  ADC_0.CALBISTREG.B.TEST_EN = 1;       /* Enable calibration test */
  ADC_0.MCR.B.PWDN = 0;     /* Power back up for calibration test to start */
  while(ADC_0.CALBISTREG.B.C_T_BUSY);  /* Wait for calibration to finish */
  if(ADC_0.MSR.B.CALIBRTD) {            /* If calibration ran successfully */
    ADC0_Calibration_Failed = 0;        /* Calibration was successful */
  }
  else  {
    ADC0_Calibration_Failed = 1;        /* Calibration was not successful */
  }

  if(ADC0_Calibration_Failed){} //Functionally useless statement to make compiler happy

}


void ADC1_Init(void) {
  ADC_1.MCR.B.PWDN     = 1;  /* Power down for starting module initialization */
  ADC_1.MCR.B.OWREN    = 1;  /* Enable overwriting older conversion results */
  ADC_1.MCR.B.MODE     = 1;  /* Scan mode (1) used instead of one shot mode */
  ADC_1.MCR.B.ADCLKSEL = 1;  /* ADC clock = FS80 bus clock (80 MHz here) */
  ADC_1.MCR.B.PWDN   = 0;    /* ADC_1 ready to receive converstion triggers */
  ADC_1.MCR.B.NSTART = 1;    /* Initiate trigger for normal scan */
}

void ADC0_Init(void) {
  ADC_0.MCR.B.PWDN     = 1;  /* Power down for starting module initialization */
  ADC_0.MCR.B.OWREN    = 1;  /* Enable overwriting older conversion results */
  ADC_0.MCR.B.MODE     = 1;  /* Scan mode (1) used instead of one shot mode */
  ADC_0.MCR.B.ADCLKSEL = 1;  /* ADC clock = FS80 bus clock (80 MHz here) */
  ADC_0.MCR.B.PWDN   = 0;    /* ADC_1 ready to receive converstion triggers */
  ADC_0.MCR.B.NSTART = 1;    /* Initiate trigger for normal scan */
}



void ADC_Read_Channels (void)
{
	if ( ADC_0.ISR.B.ECH ) {
		Result[0] = ADC_0.CDR[0].B.CDATA; //Read ADC1_AN6 conversion result data
		Result[1] = ADC_0.CDR[1].B.CDATA; //Read ADC1_AN6 conversion result data
		Result[2] = ADC_0.CDR[11].B.CDATA; //Read ADC1_AN6 conversion result data
		Result[3] = ADC_0.CDR[12].B.CDATA; //Read ADC1_AN6 conversion result data
		Result[4] = ADC_0.CDR[13].B.CDATA; //Read ADC1_AN6 conversion result data
		Result[5] = ADC_0.CDR[14].B.CDATA; //Read ADC1_AN6 conversion result data

		ResultInMv[0] = (uint16_t) (ADC_VREF*Result[0]/0xFFF); /* Conversion in mv */
		ADC_0.ISR.B.EOC = 1;
	}

	if ( ADC_1.ISR.B.ECH ) {
		Result[6] = ADC_1.CDR[0].B.CDATA; //Read ADC1_AN6 conversion result data
		Result[7] = ADC_1.CDR[1].B.CDATA; //Read ADC1_AN6 conversion result data
		ADC_1.ISR.B.EOC = 0x01;
    }
}

/*
 *
 */
unsigned short int *ADC_GetBuffer(void)
{
	return Result;
}
