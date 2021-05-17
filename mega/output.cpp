#include <Arduino.h>
#include "output.h"
#include "motor.h"
#include "sensor.h"
#include "config.h"
#include "map.h"


void output()
{
/*	Serial.println("########################################");
	// # STATE #
	Serial.print("State: ");Serial.println(state);
	// # MOTOREN #
	Serial.print("Motor:");
	Serial.print(" VL: ");Serial.print(motorStepsMade(0));
	Serial.print(" HL: ");Serial.print(motorStepsMade(1));
	Serial.print(" VR: ");Serial.print(motorStepsMade(2));
	Serial.print(" HR: ");Serial.println(motorStepsMade(3));
	// # SIDE SHARPS #
	Serial.print("Sharps:");
	Serial.print(" LV: ");Serial.print(sensorData[0]);
	Serial.print(" LH: ");Serial.print(sensorData[1]);
	Serial.print(" RV: ");Serial.print(sensorData[2]);
	Serial.print(" RH: ");Serial.println(sensorData[3]);
	Serial.print(" FL: ");Serial.print(sensorData[6]);
	Serial.print(" FC: ");Serial.print(sensorData[7]);
	Serial.print(" FR: ");Serial.print(sensorData[8]);
	Serial.print(" BL: ");Serial.print(sensorData[9]);
	Serial.print(" BR: ");Serial.println(sensorData[10]);
	// # ACC #
	Serial.print("Accelerometer:");
	Serial.print(" Y: ");Serial.print(sensorData[4]);
	Serial.print(" Z: ");Serial.println(sensorData[5]);
	// # MELEXIS #
	Serial.print("Melexis:");
	Serial.print(" L: ");Serial.print(sensorData[11]);
	Serial.print(" R: ");Serial.println(sensorData[12]);
	// # LIGHT #
	Serial.print("Light:");
	Serial.print(" R: ");Serial.print(sensorData[13]);
	Serial.print(" L: ");Serial.println(sensorData[14]);
	// # ISWALL #
	Serial.print("Walls:");
	Serial.print(" F: ");Serial.print(wallExists(FRONT));
	Serial.print(" R: ");Serial.print(wallExists(RIGHT));
	Serial.print(" L: ");Serial.print(wallExists(LEFT));
	Serial.print(" B: ");Serial.println(wallExists(BACK));*/
	// # RAMP #
	// Serial.print("Level:");Serial.print(level);Serial.print(" | ");Serial.println(sensorData[4]);
}
