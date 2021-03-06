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


// ## GLOBAL VARIABLES (see config.h) ##
volatile uint8_t sensorData[18];
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

	Serial.begin(115200);
}


void loop() 
{
	//digitalWrite(13, !digitalRead(13));
	// motorDriveTo(BACK, BASESPEED);
	// motorDriveSafe(SIGNUM(STEPSFORONE)*(uint8_t)(BASESPEED), 2, 0, 0);
	// stabilize();
	// raspiRead();
	stateChange();
	
	/*kitdropperSetTo(POSMIDD);
	delay(1000);
	kitdropperSetTo(POSLEFT);
	delay(1000);
	kitdropperSetTo(POSMIDD);
	delay(1000);
	kitdropperSetTo(POSRIGHT);
	delay(1000);*/

	output();
	// mapDisplay();
	// gyroInterrupt();
	// Serial.print(gyroX());Serial.print(" ");
	// Serial.println(gyroY());
	// delay(50000);
}
