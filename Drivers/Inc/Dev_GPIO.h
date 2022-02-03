/*
 * Dev_GPIO.h
 *
 *  Created on: 05-Jan-2022
 *      Author: Durga prasad
 */

#ifndef INC_DEV_GPIO_H_
#define INC_DEV_GPIO_H_


void Dev_GPIOInitialization(void);
_Bool Dev_GPIOInputReadOut(int PinNumber);
void Dev_GPIOReadOutToBuffer(char *pBuf);
_Bool GPIO_InputReadOut(int PinNumber);
void Dev_GPIOSetClearPin(int PinNumber , _Bool State );
void Dev_GPIOOutputReadOutBuffer(char *pBuf);


#endif /* INC_DEV_GPIO_H_ */
