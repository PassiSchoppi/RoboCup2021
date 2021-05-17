#include "ramp.h"
#include <Arduino.h>
#include "sensor.h"
#include "config.h"

void rampInit()
{
	level = sensorData[4];
}
