/*
 * pit.h
 *
 *  Created on: Feb 25, 2016
 *      Author: B55457
 */

#ifndef PIT_H_
#define PIT_H_

#include "project.h"

//Enable the Peri clock for PIT
void peri_clock_gating(void);

void PIT0_init(uint32_t);

void PIT1_init(uint32_t);

void PIT2_init(uint32_t);

#endif /* PIT_H_ */
