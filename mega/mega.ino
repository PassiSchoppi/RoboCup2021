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
uint8_t level;


void setup() 
{
	// stabilize und dann entscheiden
	state = 8;
	level = 0;
	
	// ## INIT ##
  	motorInit();
	sensorInit();
	sensorRead();
	melexisInit();
	mapInit();
	timerInit(); // 2 second delay
	kitdropperInit();
	raspiInit();
	rampInit();
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
