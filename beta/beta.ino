#include "Arduino.h"
#include "config.h"

uint8_t bufferVar;
uint8_t sensorData[8];


void setup() 
{
	pinMode(INTERUPT_PIN, INPUT_PULLUP);
	pinMode(SHP_FL, INPUT);
	pinMode(SHP_FC, INPUT);
	pinMode(SHP_FR, INPUT);
	pinMode(SHP_BL, INPUT);
	pinMode(SHP_BR, INPUT);
	
	pinMode(LED_R, OUTPUT);
	pinMode(LED_G, OUTPUT);
	pinMode(LED_B, OUTPUT);
	Serial.begin(9600);
	attachInterrupt(digitalPinToInterrupt(INTERUPT_PIN), interupt, RISING);
}

void loop() 
{
	//              EDIT THIS \/ \/ \/

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
	while(Serial.available())
	{
		bufferVar = Serial.read();
		switch(bufferVar) 
		{
			case 0:
				digitalWrite(LED_R, LOW);
				digitalWrite(LED_G, LOW);
				digitalWrite(LED_B, LOW);
				break;
		
			case 1:
				digitalWrite(LED_R, HIGH);
				digitalWrite(LED_G, HIGH);
				digitalWrite(LED_B, HIGH);
				break;
			
			case 2:
				digitalWrite(LED_R, HIGH);
				digitalWrite(LED_G, LOW);
				digitalWrite(LED_B, LOW);
				break;
			
			case 3:
				digitalWrite(LED_R, LOW);
				digitalWrite(LED_G, HIGH);
				digitalWrite(LED_B, LOW);
				break;
			
			case 4:
				digitalWrite(LED_R, LOW);
				digitalWrite(LED_G, LOW);
				digitalWrite(LED_B, HIGH);
				break;
			
			case 5:
				digitalWrite(LED_R, HIGH);
				digitalWrite(LED_G, LOW);
				digitalWrite(LED_B, HIGH);
				break;
			
			case 6:
				digitalWrite(LED_R, HIGH);
				digitalWrite(LED_G, HIGH);
				digitalWrite(LED_B, LOW);
				break;
			
			case 7:
				digitalWrite(LED_R, LOW);
				digitalWrite(LED_G, HIGH);
				digitalWrite(LED_B, HIGH);
				break;
		}
	}

	sensorData[0] = analogRead(SHP_FL) >> 2;
	sensorData[1] = analogRead(SHP_FC) >> 2;
	sensorData[2] = analogRead(SHP_FR) >> 2;
	sensorData[3] = analogRead(SHP_BL) >> 2;
	sensorData[4] = analogRead(SHP_BR) >> 2;
	sensorData[5] = digitalRead(9);
	sensorData[6] = digitalRead(10);
	sensorData[7] = digitalRead(11);

	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void interupt()
{
	for(uint8_t i=0; i<8; ++i)
	{
		Serial.write(sensorData[i]);
	}
	
	/*Serial.write(sensorData[0]);
	Serial.write(sensorData[1]);
	Serial.write(sensorData[2]);
	Serial.write(sensorData[3]);
	Serial.write(sensorData[4]);*/
}
