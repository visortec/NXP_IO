/*
 * Dev_TIM.h
 *
 *  Created on: 06-Jan-2022
 *      Author: Durga prasad
 */

#ifndef INC_DEV_TIM_H_
#define INC_DEV_TIM_H_

#include<stdint.h>


void ETimer_Init(void);
void Dev_PIT0_Init(uint32_t LDVAL);
uint32_t GetMilliSecCounter(void);


#endif /* INC_DEV_TIM_H_ */
