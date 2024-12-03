/*
 * software_timer.h
 *
 *  Created on: Sep 24, 2023
 *      Author: HaHuyen
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "tim.h"

extern uint16_t flag_timer2;

void timer_init();
void setTimer2(uint16_t duration);
void setFlagModifyTimer(uint16_t timer);
uint16_t isFlagModify();

#endif /* INC_SOFTWARE_TIMER_H_ */
