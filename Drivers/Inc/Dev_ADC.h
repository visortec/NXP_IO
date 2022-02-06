/*
 * Dev_ADC.h
 *
 *  Created on: 05-Jan-2022
 *      Author: Durga prasad
 */

#ifndef INC_DEV_ADC_H_
#define INC_DEV_ADC_H_

void ADC_PadConfig_ChanSelect(void);
void ADC0_Calibration(void);
void ADC1_Calibration(void);
void ADC0_Init(void);
void ADC1_Init (void);
void ADC_Read_Channels (void);
unsigned short int * ADC_GetBuffer();
#endif /* INC_DEV_ADC_H_ */
