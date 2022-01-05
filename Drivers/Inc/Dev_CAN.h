/*
 * Dev_CAN.h
 *
 *  Created on: 05-Jan-2022
 *      Author: Durga prasad
 */

#ifndef INC_DEV_CAN_H_
#define INC_DEV_CAN_H_

#include<stdint.h>

void Dev_CANInitialization(void);
void Dev_CANTransmitData(uint32_t ExID, char *pBuf, char MsgLen);
uint8_t Dev_CANFrameReceive(char *pBuf);


#endif /* INC_DEV_CAN_H_ */
