#ifndef WALL_H_
#define WALL_H_

#include <Arduino.h>
#include "config.h"

// returns true if there is a wall in the given direction
bool wallExists(uint8_t where);

#endif
