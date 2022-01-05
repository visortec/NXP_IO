/*
 * can.h
 *
 *  Created on: Mar 1, 2016
 *      Author: B55457
 */

#ifndef CAN_H_
#define CAN_H_

#include "derivative.h"
#include "project.h"

#define DEVKIT	1
#define LOOPBACK 0

void initCAN_2(void);
void initCAN_1(void);
void initCAN_0 (void);
void TransmitMsg(char* TxData, int Len );
void ReceiveMsg (void);
void delay(void);

#endif /* CAN_H_ */
