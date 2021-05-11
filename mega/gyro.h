#ifndef GYRO_H
#define GYRO_H

#include <Arduino.h>
#include "Wire.h"

void gyroInit();
void gyroInterrupt();
int16_t gyroX();
int16_t gyroY();

#endif
