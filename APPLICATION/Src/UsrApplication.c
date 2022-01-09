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


/*@Brief: CAN Port Initialization
 *
 */
void APPCanInitilization(void)
{
	static uint32_t TimeOut  = 0 ;
	Dev_CANInitialization();
	Dev_GPIOInitialization();
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
	memset(CanTxFrame, 0x00, 10 );
	Dev_GPIOReadOutToBuffer(CanTxFrame);
	/* Transmit CAN Frame */
	Dev_CANTransmitData(CANTxExID, CanTxFrame, 8);
}



void APPCanTransmitPeriodicTask(uint8_t TaskId)
{





}

