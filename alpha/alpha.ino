#include "Arduino.h"
#include "config.h"

uint8_t sensorData[6];

uint8_t kitdropperDeg = 120;
int 	pwm;

void setup() 
{
	pinMode(INTERUPT_PIN, INPUT_PULLUP);
	pinMode(3, OUTPUT);
	// pinMode(0, OUTPUT);
	Serial.begin(115200);
	attachInterrupt(digitalPinToInterrupt(INTERUPT_PIN), interupt, RISING);
}


void loop() 
{
/*
 *
 *           Sensordata in ARRAYINDEX
 *
 *             +-------------------+
 *             |                   |
 *      SHP_LF |  0   ACC_X 4   2  | SHP_RF
 *             |        /|\        |
 *             |         |         |
 *             |         |         |
 *             |         *         |
 *      SHP_LB |  1   ACC_Z 5   3  | SHP_RB
 *             |                   |
 *             +-------------------+ 
 *
 * 
 */
	sensorData[0] = analogRead(SHP_LF) >> 2;
	sensorData[1] = analogRead(SHP_LB) >> 2;
	sensorData[2] = analogRead(SHP_RF) >> 2;
	sensorData[3] = analogRead(SHP_RB) >> 2;
	sensorData[4] = (analogRead(ACC_X)-30) >> 2;
	sensorData[5] = (analogRead(ACC_Z)-30) >> 2;

	if(Serial.available())
	{
		kitdropperDeg = Serial.read();
	}

	servoPulse(3, kitdropperDeg);
}

void servoPulse (int servo, int angle)
{
 	pwm = (angle*11) + 500;      // Convert angle to microseconds
 	digitalWrite(servo, HIGH);
 	delayMicroseconds(pwm);
 	digitalWrite(servo, LOW);
 	delay(50);                   // Refresh cycle of servo
}

void interupt()
{
	for(uint8_t i=0; i<6; ++i)
	{
		Serial.write(sensorData[i]);
	}
}
