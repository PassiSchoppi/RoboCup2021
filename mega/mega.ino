#include "Arduino.h"
#include "config.h"
#include "motor.h"
#include "sensor.h"
#include "map.h"
#include "state.h"
#include "led.h"
#include "wall.h"
#include "i2cmaster.h"
#include "melexis.h"
#include "stabilize.h"
#include "timer.h"
#include "kitdropper.h"
#include "raspi.h"
#include "output.h"
#include "ramp.h"
#include "gyro.h"


// uint8_t sensorData[15];
// ## GLOBAL VARIABLES (see config.h) ##
volatile uint8_t sensorData[15];
uint8_t state;


void setup() 
{
	// stabilize und dann entscheiden
	state = 8;
	
	// ## INIT ##
  	motorInit();
	sensorInit(); // 1 second delay
	melexisInit();
	mapInit();
	timerInit();
	kitdropperInit();
	raspiInit();
	// gyroInit();

	Serial.begin(1000000);
}


void loop() 
{
	//digitalWrite(13, !digitalRead(13));
	stateChange();
	output();
	// mapDisplay();
	// gyroInterrupt();
	// Serial.print(gyroX());Serial.print(" ");
	// Serial.println(gyroY());
	// delay(5);
}
