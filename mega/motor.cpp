#include "motor.h"
#include <Arduino.h>
#include "sensor.h"

Motor motor[4];

uint8_t localSensorData[15];

void motorInit() 
{
  	motor[0].pin1 = 29;
  	motor[0].pin2 = 27;
  	motor[0].enc = &PINC;
	motor[0].encbit = 4;
	// motor[0].enc2 = PINC;
	// motor[0].enc2bit = 6;
	motor[0].steps = 1;
	motor[0].pwm = 4;
  	motor[0].factor = 0.9968225247;

  	motor[1].pin1 = 23;
  	motor[1].pin2 = 25;
	motor[1].enc = &PINC;
	motor[1].encbit = 0;
	// motor[1].enc2 = PINC;
	// motor[1].enc2bit = 2;
	motor[1].steps = 1;
  	motor[1].pwm = 3;
  	motor[1].factor = 0.9708457497;

  	motor[2].pin1 = 49;
  	motor[2].pin2 = 47;
	motor[2].enc = &PINL;
	motor[2].encbit = 4;
	// motor[2].enc2 = PINL;
	// motor[2].enc2bit = 2;
	motor[2].steps = 1;
  	motor[2].pwm = 5;
  	motor[2].factor = 0.9565686154;

  	motor[3].pin1 = 51;
  	motor[3].pin2 = 53;
	motor[3].enc = &PING;
	motor[3].encbit = 0;
	// motor[3].enc2 = PING;
	// motor[3].enc2bit = 2;
	motor[3].steps = 1;
  	motor[3].pwm = 6;
  	motor[3].factor = 1.00000000;

  	for(uint8_t i=0; i<4; i++) 
	{
    	pinMode(motor[i].pin1, OUTPUT);
    	pinMode(motor[i].pin2, OUTPUT);
    	// pinMode(motor[i].enc, INPUT);
		// pinMode(motor[i].enc2, INPUT);
		pinMode(motor[i].pwm, OUTPUT);
  	}
}

void motorSetSpeed(uint8_t i, int speed) 
{
  	int pwm = min(255, round(abs(speed)*motor[i].factor));

  	analogWrite(motor[i].pwm, pwm);
  	digitalWrite(motor[i].pin1, speed>=0);
  	digitalWrite(motor[i].pin2, speed<=0);
}

unsigned int motorAverageSteps()
{
	unsigned int average = 0;
	for(uint8_t i=0; i<4; ++i)
	{
		average = average + motorStepsMade(i);
	}
	average = average/4;
	return(average);
}

void motorCheckForStepsMade(uint8_t i)
{
	// momentanes siganal auf enc1 des Motors i
	bool currentEncSignal;
	currentEncSignal = (*motor[i].enc >> motor[i].encbit & 1);

	// wenn ein rising edge auf current state ist
	if(motor[i].lastEncSignal != currentEncSignal && currentEncSignal == HIGH)
	{
			motor[i].steps = (motor[i].steps + 1);
	}
	// update lastEncSignal
	motor[i].lastEncSignal = currentEncSignal;
}

void motorResetAllSteps()
{
	for(uint8_t i=0; i<4; i++)
	{
		motor[i].steps = 1;
	}
}

unsigned int motorStepsMade(uint8_t i)
{
	return(motor[i].steps);
}

int speedFromEnc(unsigned int encA, unsigned int encB, unsigned int encC, unsigned int encME, int speedME, bool stickToWall, bool leftMotor)
{
	int correctedSpeed = ((float)((( ((float)(encA+encB+encC+encME)/4) *speedME) / (encME))-speedME)*5)+speedME;
	
	// wenn er sich an waenden orientieren soll
	if( stickToWall )
	{
		// wenn gerade linker motor kontrolliert wird
		if( leftMotor )
		{
			// wenn auf der linken seite eine Wand ist
			if(wallExists(LEFT))
			{
				// wenn er zu nah an der wand ist
				/*if ( sensorData[0]>MINDISTANCETOW || sensorData[1]>MINDISTANCETOW )
				{
					// linken motor schneller drehen lassen
					correctedSpeed += 20*CORECTIONAGGRESSIVE;
				}
				else*/

				// differenz der sensoren auf einer seite auf motor speed addieren
				correctedSpeed = correctedSpeed + (( sensorData[0] - sensorData[1]) * CORECTIONAGGRESSIVE );
				
				if ( wallExists(RIGHT) ) // wenn auf beiden seiten waende sind
				{
					// differenz der Sensoren auf verschiedenen seiten addiert zu morot speed
					correctedSpeed = correctedSpeed + ((sensorData[3] - sensorData[1]) * CORECTIONAGGRESSIVE );
				}
			}
		}

		else // wenn es der rechte Morot ist
		{
			// wenn rechts eine wand ist
			if(wallExists(RIGHT))
			{
				// wenn zu nah an einer wand ist
				/*if ( sensorData[2]>MINDISTANCETOW || sensorData[3]>MINDISTANCETOW )
				{
					// rechten motor schneller drehen
					correctedSpeed += 20*CORECTIONAGGRESSIVE;
				}
				else*/
				// differenz der sensoren auf ener seite auf motor speed addieren
				correctedSpeed = correctedSpeed + (( sensorData[2] - sensorData[3] ) * CORECTIONAGGRESSIVE );
				
				if ( wallExists(LEFT) ) // wenn auf beiden seiten waende sind
				{
					// differenz der Sensoren auf verschiedenen seiten addiert zu morot speed
					correctedSpeed = correctedSpeed + ((sensorData[1] - sensorData[3]) * CORECTIONAGGRESSIVE );
				}
			}
		}

	}
	return (correctedSpeed);
}

void motorDriveTo(uint8_t direction, int speed)
{
	int kP;
	int errorP;
	int sum;
	int targetWallDistance = PERFECTDISTTOW;

	// Serial.println("hi");
	// Serial.println(direction);
	switch( direction )
	{
		case FRONT:
			kP = 1;
			targetWallDistance = PERFECTDISTTOW;
			int avoidObstacleBy;
			avoidObstacleBy = 0.7;
			int moreDistanceToWall;
			moreDistanceToWall = 10;
			
			//6   FL,
			//7   FC,
			//8   FR,
			//0   [LF,
			//1   LB,
			//2   RF,
			//3   RB,			
			if(wallExists(RIGHT))
			{
				errorP = (int)(sensorData[2]) - targetWallDistance;
				sum = (int)(errorP * kP);
				if( sum > speed )
				{
					sum = speed;
				}else if( sum < - speed )
				{
					sum = - speed;
				}
				motorSetSpeed(0, speed - sum);
				motorSetSpeed(1, speed - sum);
				motorSetSpeed(2, speed + sum);
				motorSetSpeed(3, speed + sum);
			}
			else if (wallExists(LEFT))
			{
				errorP = (int)(sensorData[0]) - targetWallDistance;
				sum = (int)(errorP * kP);
				if( sum > speed )
				{
					sum = speed;
				}else if( sum < - speed )
				{
					sum = - speed;
				}
				motorSetSpeed(0, speed + sum);
				motorSetSpeed(1, speed + sum);
				motorSetSpeed(2, speed - sum);
				motorSetSpeed(3, speed - sum);
			} else {
				motorSetSpeed(0, speedFromEnc(motor[1].steps, motor[2].steps, motor[3].steps, motor[0].steps, speed, true, true));
				motorSetSpeed(1, speedFromEnc(motor[0].steps, motor[2].steps, motor[3].steps, motor[1].steps, speed, true, true));
				motorSetSpeed(2, speedFromEnc(motor[0].steps, motor[1].steps, motor[3].steps, motor[2].steps, speed, true, false));
				motorSetSpeed(3, speedFromEnc(motor[0].steps, motor[2].steps, motor[1].steps, motor[3].steps, speed, true, false));
			}
			break;

		case RIGHT:
			motorSetSpeed(0, speedFromEnc(motor[1].steps, motor[2].steps, motor[3].steps, motor[0].steps, speed, false, true));
			motorSetSpeed(1, speedFromEnc(motor[0].steps, motor[2].steps, motor[3].steps, motor[1].steps, speed, false, true));
			motorSetSpeed(2, speedFromEnc(motor[0].steps, motor[1].steps, motor[3].steps, motor[2].steps, -speed, false, false));
			motorSetSpeed(3, speedFromEnc(motor[0].steps, motor[2].steps, motor[1].steps, motor[3].steps, -speed, false, false));
			break;
		
		case LEFT:
			// Serial.println("hello");
			motorSetSpeed(0, speedFromEnc(motor[1].steps, motor[2].steps, motor[3].steps, motor[0].steps, -speed, false, true));
			motorSetSpeed(1, speedFromEnc(motor[0].steps, motor[2].steps, motor[3].steps, motor[1].steps, -speed, false, true));
			motorSetSpeed(2, speedFromEnc(motor[0].steps, motor[1].steps, motor[3].steps, motor[2].steps, speed, false, false));
			motorSetSpeed(3, speedFromEnc(motor[0].steps, motor[2].steps, motor[1].steps, motor[3].steps, speed, false, false));
			break;
		
		case BACK:
			// FIXME
			motorSetSpeed(0, -speed);
			motorSetSpeed(1, -speed);
			motorSetSpeed(2, -speed);
			motorSetSpeed(3, -speed);
			break;
		case RAMPSTATE:
			kP = 1;
			targetWallDistance = PERFECTDISTTOW - 20;
			errorP = -((int)(sensorData[0]) - targetWallDistance);
			sum = (int)(errorP * kP);
			if( sum > speed )
			{
				sum = speed;
			}else if( sum < - speed )
			{
				sum = - speed;
			}
			motorSetSpeed(0, speed - sum);
			motorSetSpeed(1, speed - sum);
			motorSetSpeed(2, speed + sum);
			motorSetSpeed(3, speed + sum);
			break;
	}
	// Serial.println("done");
}

void motorBrake() 
{
  	for(uint8_t i=0; i<4; i++) 
	{
    	motorSetSpeed(i, 0);
  	}
}


int errorI=0;

void motorDriveSafe(int16_t baseSpeed, float kP, float kI, float kD) {
    int errorP = 0;
    int errorD = 0;

    int targetWallDistance = 0;
    // int obstacle = obstacleInFront();

    // if((obstacle == 1) && entireWall(LEFT, 180) && !entireWall(RIGHT, 200))
        // targetWallDistance = 500;
    // else if((obstacle == -1) && entireWall(RIGHT, 180) && !entireWall(LEFT, 200))
        // targetWallDistance = 500;
    // else
    targetWallDistance = PERFECTDISTTOW;
//0   [LF,
	//1   LB,
	//2   RF,
	//3   RB,
	//4   ACC_X,
	//5   ACC_Z,
	//6   FL,
	//7   FC,
	//8   FR,
	//9   BL, ???
	//10  BR,
	//11  TEMP_L,
	//12  TEMP_R
	//13  LIGHT_R
	//14  LIGHT_L]
    if(wallExists(RIGHT)) {
		errorP = (int)(sensorData[2]) - targetWallDistance;
		errorI = errorI + errorP;
		errorD = sensorData[3] - sensorData[2];
		// Serial.println();
		// Serial.print("P: ");Serial.print(errorP);Serial.print(" I: ");Serial.print(errorI);Serial.print(" D: ");Serial.print(errorD);Serial.print(" SUM: ");Serial.println(errorD+errorI+errorP);
    } else if(wallExists(LEFT)) {
        errorP = -((int)(sensorData[0]) - targetWallDistance);
        errorI = errorI + errorP;
        errorD = -(sensorData[1] - sensorData[0]);
    } else {
        errorI = 0;
    }

	int sum = (int)(errorP * kP) - (int)(errorI * kI) - (int)(errorD * kD);
	if( sum > BASESPEED*1.5 )
	{
		sum = BASESPEED*1.5;
	}else if( sum < BASESPEED/2 )
	{
		sum = BASESPEED/2;
	}

	motorSetSpeed(0, baseSpeed - (int)(errorP * kP) - (int)(errorI * kI) - (int)(errorD * kD));
	motorSetSpeed(1, baseSpeed - (int)(errorP * kP) - (int)(errorI * kI) - (int)(errorD * kD));
    motorSetSpeed(2, baseSpeed + (int)(errorP * kP) + (int)(errorI * kI) + (int)(errorD * kD));
    motorSetSpeed(3, baseSpeed + (int)(errorP * kP) + (int)(errorI * kI) + (int)(errorD * kD));

    /*motorSetLeftSpeed(baseSpeed + (int)(errorP * kP) + (int)(errorI * kI) + (int)(errorD * kD));
    motorSetRightSpeed(baseSpeed - (int)(errorP * kP) - (int)(errorI * kI) - (int)(errorD * kD));*/
}
