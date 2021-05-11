#ifndef KITDROPPER_H
#define KITDROPPER_H

#include <Arduino.h>
#include "config.h"

void kitdropperInit();

// sends serial signal to alpha to
// rotate servor to deg
// 70 is max left
// 120 is middle
// 150 is max right
void kitdropperSetTo(uint8_t deg);

#endif
