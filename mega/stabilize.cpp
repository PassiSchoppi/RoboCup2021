#include "stabilize.h"


void stabilize()
{
	if(SEND){Serial.println("stabalizing...");}
	motorBrake();
	
	for(int i=0; i<1000; ++i)
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
		factor = 0.01;
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
		float factor = 0.01;
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
