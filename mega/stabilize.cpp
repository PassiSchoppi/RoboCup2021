#include "stabilize.h"


void stabilize()
{
	if(SEND){Serial.println("stabalizing...");}
	motorBrake();
	motorResetAllSteps();

	int timerA = 500;

	int iterations;
	iterations = 1;

	/*// links an wand ausrichten
	if(wallExists(LEFT)){
		int setSpeed;
		setSpeed = 0;
		// minimum speed to move every motor
		for(int testSpeed = 0; testSpeed<timerA; testSpeed+=iterations){
			if(sensorData[0] > sensorData[1]){
				setSpeed = testSpeed;
			}else{
				setSpeed = -testSpeed;
			}
			if(motorStepsMade(0) < 2){
				motorSetSpeed(0, setSpeed);
			}
			if(motorStepsMade(1) < 2){
				motorSetSpeed(1, setSpeed);
			}
			if(motorStepsMade(2) < 2){
				motorSetSpeed(2, -setSpeed);
			}
			if(motorStepsMade(3) < 2){
				motorSetSpeed(3, -setSpeed);
			}
			delay(10);
		}
	}
	// rechts an wand ausrichten
	if(wallExists(RIGHT)){
		int setSpeed;
		setSpeed = 0;
		// minimum speed to move every motor
		for(int testSpeed = 0; testSpeed<timerA; testSpeed+=iterations){
			if(sensorData[2] > sensorData[3]){
				setSpeed = -testSpeed;
			}else{
				setSpeed = testSpeed;
			}
			if(motorStepsMade(0) < 2){
				motorSetSpeed(0, setSpeed);
			}
			if(motorStepsMade(1) < 2){
				motorSetSpeed(1, setSpeed);
			}
			if(motorStepsMade(2) < 2){
				motorSetSpeed(2, -setSpeed);
			}
			if(motorStepsMade(3) < 2){
				motorSetSpeed(3, -setSpeed);
			}
				delay(10);
		}	
	}

	// wall on back
	if(wallExists(BACK)){
		int setSpeed;
		setSpeed = 0;
		// minimum speed to move every motor
		for(int testSpeed = 0; testSpeed<timerA; testSpeed+=iterations){
			// Serial.print(sensorData[9]);Serial.println(" : ");Serial.println(sensorData[10]);
			if(sensorData[9] > sensorData[10]){
				setSpeed = testSpeed;
			}else{
				setSpeed = -testSpeed;
			}
			if(motorStepsMade(0) < 2){
				motorSetSpeed(0, setSpeed);
			}
			if(motorStepsMade(1) < 2){
				motorSetSpeed(1, setSpeed);
			}
			if(motorStepsMade(2) < 2){
				motorSetSpeed(2, -setSpeed);
			}
			if(motorStepsMade(3) < 2){
				motorSetSpeed(3, -setSpeed);
			}
			delay(10);
		}
	}

	// distance to back
	if(wallExists(BACK)){
		int setSpeed;
		setSpeed = 0;
		// minimum speed to move every motor
		for(int testSpeed = 0; testSpeed<timerA; testSpeed+=iterations){
			if((sensorData[9] + sensorData[10])/2 < PERFECTDISTTOW+13){
				setSpeed = testSpeed;
			}else{
				setSpeed = -testSpeed;
			}
			if(motorStepsMade(0) < 2){
				motorSetSpeed(0, setSpeed);
			}
			if(motorStepsMade(1) < 2){
				motorSetSpeed(1, setSpeed);
			}
			if(motorStepsMade(2) < 2){
				motorSetSpeed(2, setSpeed);
			}
			if(motorStepsMade(3) < 2){
				motorSetSpeed(3, setSpeed);
			}
			delay(10);
		}
	}*/

	return;


	
	for(int i=0; i<2000; ++i)
	{
		/*if( wallExists(LEFT) )
		{
			motorSetSpeed(1, -50);
			motorSetSpeed(0, -50);
			motorSetSpeed(2, 50);
			motorSetSpeed(3, 50);

			delay(500); // hoch: links vorne niedrig: links hinten

			motorSetSpeed(1, 50);
			motorSetSpeed(0, 50);
			motorSetSpeed(2, 50);
			motorSetSpeed(3, 50);

			delay(500); // hoch: hinten niedrig: vorne

			motorSetSpeed(1, 50);
			motorSetSpeed(0, 50);
			motorSetSpeed(2, -50);
			motorSetSpeed(3, -50);

			delay(1000); // hoch: 

			motorSetSpeed(1, -50);
			motorSetSpeed(0, -50);
			motorSetSpeed(2, -50);
			motorSetSpeed(3, -50);

			delay(500);
		}*/
		float factor;
		factor = 0.02;
		if( wallExists(LEFT) )
		{
			int speed = factor*(sensorData[0]-sensorData[1])*(sensorData[0]-sensorData[1])*(sensorData[0]-sensorData[1]);
			if(speed > BASESPEED){
				speed = BASESPEED;
			}else if (speed < -BASESPEED)
			{
				speed = -BASESPEED;
			}
			motorSetSpeed(1, speed);
			motorSetSpeed(0, speed);
		}
		if( wallExists(RIGHT) )
		{
			int speed = factor*(sensorData[2]-sensorData[3])*(sensorData[2]-sensorData[3])*(sensorData[2]-sensorData[3]);
			if(speed > BASESPEED){
				speed = BASESPEED;
			}else if (speed < -BASESPEED)
			{
				speed = -BASESPEED;
			}
			motorSetSpeed(2, speed);
			motorSetSpeed(3, speed);
		}
	}

	// ausrichten an der hinteren Wand
	if(wallExists(BACK))
	{
		float factor = 0.02;
		int i = 0;
		motorBrake();
		// links: 	      rechts:
		//sensorData[9]<sensorData[10] && 
		while(i<1000){
			int speed;
			speed = factor*(sensorData[9]-sensorData[10])*(sensorData[9]-sensorData[10])*(sensorData[9]-sensorData[10]);
			if(speed > BASESPEED){
				speed = BASESPEED;
			}else if (speed < -BASESPEED)
			{
				speed = -BASESPEED;
			}
			motorDriveTo(LEFT, -speed);
			motorDriveTo(RIGHT, speed);
			++i;
		}
		motorBrake();
	}

	motorBrake();
	motorResetAllSteps();
}
