/*
 * UsrApplication.c
 *
 *  Created on: 05-Jan-2022
 *      Author: Durga prasad
 */


#include "Dev_GPIO.h"
#include "Dev_CAN.h"
#include "UsrApplication.h"
#include "Dev_TIM.h"
#include<string.h>
#include "project.h"

static char 	CanTxFrame[10] = {0};
const uint32_t  CANTxExID[] = { 0x06FF0807 , 0x06FF0907, 0x06FF0A07, 0x06FF0B07 };


#define _IR(X) GPIO_InputReadOut(X)

#define  DIP1   PE0
#define  DIP2   PE2
#define  DIP3   PE4
#define  DIP4   PE5
#define  DIP5   PE6
#define  DIP6   PE7
#define  DIP7   PE9
#define  DIP8   PE10
#define  DIP9   PE11
#define  DIP10  PE13

void APPInputsStatusChecking(void);
void APP_ADCFrameFotmation(uint8_t FrameType,uint8_t *Buf);
void APPTransmitCANExIdFrames(void);


/*******************************************************************************************************************/

/*@Brief: CAN Port Initialization
 *
 */
void APPCanInitilization(void)
{
	/*-- GPIO PIN Init --*/
	Dev_GPIOInitialization();
	/* Analog Channels Init */
	ADC_PadConfig_ChanSelect();
	ADC0_Calibration();
	ADC0_Init();

	ADC1_Calibration();
	ADC1_Init();

	/*CAN Port Init */
	Dev_CANInitialization();

	Dev_PIT0_Init(0x00);
}


/*
 *
 */
void APPCanTransmitPeriodicTask(uint8_t TaskId)
{
	/* Checking Input Status */
	APPInputsStatusChecking();

	/* Read Analog Channles */
	ADC_Read_Channels();

	/* -- CAN Transmit -- */
	APPTransmitCANExIdFrames();
}

/*@ Brief:
 * @Note : DOP3= Not( DIP3)  AND (DIP2) AND  DIP1 AND DIP6 OR (DIP9 OR DIP10)
 *         DOP1  =[Not( DIP3)  AND DIP1 AND DIP6 AND (DIP9 )] OR    Not ( DIP3)  AND (DIP2)
 *         DOP2 =Not( DIP3)  AND (Not DIP2) AND DIP1 AND DIP6 AND (DIP10 ) OR    Not ( DIP3)  AND (DIP2)
 *         DOP5  =Not( DIP3)  AND (Not DIP2) AND DIP1 AND DIP6 AND (DIP9 ) OR    Not ( DIP3)  AND (DIP2)
 */
void APPInputsStatusChecking(void)
{
	if( (!_IR(DIP3)) && _IR(DIP2) && (_IR(DIP1)) && _IR(DIP6) || (_IR(DIP9) || _IR(DIP10)))
	{
		Dev_GPIOSetClearPin(PA2, 1);
	}else{
		Dev_GPIOSetClearPin(PA2, 0);
	}

	if(( (!_IR(DIP3)) && _IR(DIP1) && (_IR(DIP6)) && _IR(DIP9)) || (!(_IR(DIP3)) && _IR(DIP2)))
	{
		Dev_GPIOSetClearPin(PA0, 1);
	}else{
		Dev_GPIOSetClearPin(PA0, 0);
	}

	if((!_IR(DIP3)) && (!_IR(DIP2)) && _IR(DIP1) && (_IR(DIP6)) && _IR(DIP10) || (!(_IR(DIP3)) && _IR(DIP2)))
	{
		Dev_GPIOSetClearPin(PA1, 1);
	}else{
		Dev_GPIOSetClearPin(PA1, 0);
	}

	if((!_IR(DIP3)) && (!_IR(DIP2)) && _IR(DIP1) && (_IR(DIP6)) && _IR(DIP9) || (!(_IR(DIP3)) && _IR(DIP2)))
	{
		Dev_GPIOSetClearPin(PA4, 1);
	}else{
		Dev_GPIOSetClearPin(PA4, 0);
	}
}

/*@Brief: Transmit the CAN Frame
 *
 */
void APPTransmitCANExIdFrames(void)
{
	static uint32_t sSecTimeOut_l = 0;   static uint8_t FrmeIndex = 0;
    /* Transmit 4-CAN Frames For Every 2 Sec */
	if( GetMilliSecCounter() -sSecTimeOut_l >= 4 )
	{
		memset(CanTxFrame, 0x00, 8 );
		switch(FrmeIndex)
		{
			case 0:
				APP_ADCFrameFotmation(1,CanTxFrame);
				/* Transmit CAN Frame */
				Dev_CANTransmitData(CANTxExID[0], CanTxFrame, 8);
				FrmeIndex = 1;
				break;
			case 1:
				APP_ADCFrameFotmation(1,CanTxFrame);
				/* Transmit CAN Frame */
				Dev_CANTransmitData(CANTxExID[1], CanTxFrame, 8);
				FrmeIndex = 2;
				break;
			case 2:
				APP_ADCFrameFotmation(1,CanTxFrame);
				/* Transmit CAN Frame */
				Dev_CANTransmitData(CANTxExID[2], CanTxFrame, 8);
				FrmeIndex = 3;
				break;
			case 3:
				FrmeIndex = 0;
				APP_ADCFrameFotmation(1,CanTxFrame);
				/* Transmit CAN Frame */
				Dev_CANTransmitData(CANTxExID[3], CanTxFrame, 8);
				sSecTimeOut_l = GetMilliSecCounter();
				break;
		}
	}
}


void APP_ADCFrameFotmation(uint8_t FrameType,uint8_t *Buf)
{
	uint16_t *TempBuff = 0;

	/* -- Fill the Buffer -- */
	switch(FrameType)
	{
		case 1:
			Dev_GPIOReadOutDinToBuffer(Buf); 			break;
		case 2:
			Dev_GPIOReadOutDoutToBuffer(Buf);			break;
		case 3:
			TempBuff = ADC_GetBuffer();
			Buf[0] = (uint8_t)TempBuff[0] ;  Buf[1] = (uint8_t)TempBuff[0] >> 8 ;
			Buf[2] = (uint8_t)TempBuff[1] ;  Buf[3] = (uint8_t)TempBuff[1] >> 8 ;
			Buf[4] = (uint8_t)TempBuff[2] ;  Buf[5] = (uint8_t)TempBuff[2] >> 8 ;
			Buf[6] = (uint8_t)TempBuff[3] ;  Buf[7] = (uint8_t)TempBuff[3] >> 8 ;
			break;
		case 4:
			TempBuff = ADC_GetBuffer();
			Buf[0] = (uint8_t)TempBuff[4] ;  Buf[1] = (uint8_t)TempBuff[4] >> 8 ;
			Buf[2] = (uint8_t)TempBuff[5] ;  Buf[3] = (uint8_t)TempBuff[5] >> 8 ;
			Buf[4] = (uint8_t)TempBuff[6] ;  Buf[5] = (uint8_t)TempBuff[6] >> 8 ;
			Buf[6] = (uint8_t)TempBuff[7] ;  Buf[7] = (uint8_t)TempBuff[7] >> 8 ;
			break;
	}
}
