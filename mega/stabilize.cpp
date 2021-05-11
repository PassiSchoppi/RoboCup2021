#include "stabilize.h"


void stabilize()
{
	motorBrake();
	
	for(int i=0; i<40; ++i)
	{
		if( wallExists(LEFT) )
		{
			motorSetSpeed(1, 50*(sensorData[0]-sensorData[1])/abs(sensorData[0]-sensorData[1]));
			motorSetSpeed(0, 50*(sensorData[0]-sensorData[1])/abs(sensorData[0]-sensorData[1]));
		}
		if( wallExists(RIGHT) )
		{
			motorSetSpeed(2, 50*(sensorData[2]-sensorData[3])/abs(sensorData[2]-sensorData[3]));
			motorSetSpeed(3, 50*(sensorData[3]-sensorData[3])/abs(sensorData[2]-sensorData[3]));
		}
	}

	// ausrichten an der hinteren Wand
	/*if(wallExists(BACK))
	{
		motorDriveTo(BACK, BASESPEED);
		while(motorStepsMade(0)<27)
		{
		}
		motorBrake();
		motorResetAllSteps();
		motorDriveTo(FRONT, BASESPEED);
		while(motorStepsMade(0)<27)
		{
		}
		motorBrake();
		motorResetAllSteps();
	}*/

	motorBrake();
	motorResetAllSteps();
}
