#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>
#include "motor.h"

const uint16_t t1_load = 0;
const uint16_t t1_comp = 250;

void timerInit();

// there is a ISR in timer.cpp
// to motorCheckForStepsMade()

#endif 
