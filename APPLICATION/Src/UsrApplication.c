/*
 * UsrApplication.c
 *
 *  Created on: 05-Jan-2022
 *      Author: Durga prasad
 */


#include "Dev_GPIO.h"
#include "Dev_CAN.h"
#include "UsrApplication.h"

static char 	CanTxFrame[10] = {0};
const uint32_t  CANTxExID = 0x0FEE000;


/*@Brief: CAN Port Initialization
 *
 */
void APPCanInitilization(void)
{
	Dev_CANInitialization();
	Dev_GPIOInitialization();
}

void APPTransmitCanExIdFrame(void)
{
	memset(CanTxFrame, 0x00, 10 );
	Dev_GPIOInputReadOutBuffer(CanTxFrame);
	/* Transmit CAN Frame */
	Dev_CANTransmitData(CANTxExID, CanTxFrame, 8);



}



void APPCanTransmitPeriodicTask(uint8_t TaskId)
{





}

