#ifndef STATE_H_
#define STATE_H_

#include <Arduino.h>
#include "config.h"
#include "kitdropper.h"
#include "map.h"
#include "map.h"

// extern uint8_t nextState = 0;

uint8_t nothing();

// gets next step of state machine
void stateChange();

#endif
