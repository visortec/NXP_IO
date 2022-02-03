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

static char 	CanTxFrame[10] = {0};
const uint32_t  CANTxExID = 0x0FEE000;


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



/*@Brief: CAN Port Initialization
 *
 */
void APPCanInitilization(void)
{
	static uint32_t TimeOut  = 0 ;
	/*-- GPIO PIN Init --*/
	Dev_GPIOInitialization();

	/*CAN Port Init */
	Dev_CANInitialization();

//	ETimer_Init();
	Dev_PIT0_Init(0x00);
	TimeOut = GetMilliSecCounter();
	while(1)
	{
		if( GetMilliSecCounter() - TimeOut >= 5 )
		{
			break;
		}
	}
}

void APPTransmitCanExIdFrame(void)
{
	static uint32_t sSecTimeOut_l = 0;

	if(GetMilliSecCounter() -sSecTimeOut_l >= 4 )
	{
		memset(CanTxFrame, 0x00, 10 );
		Dev_GPIOReadOutToBuffer(CanTxFrame);
		/* Transmit CAN Frame */
		Dev_CANTransmitData(CANTxExID, CanTxFrame, 8);
		sSecTimeOut_l = GetMilliSecCounter();
	}
}



void APPCanTransmitPeriodicTask(uint8_t TaskId)
{





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

