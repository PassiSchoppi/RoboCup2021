#include "stabilize.h"


void stabilize()
{
	motorBrake();
	
	for(int i=0; i<40; ++i)
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
	if(wallExists(BACK))
	{
		int i = 0;
		motorBrake();
		while(sensorData[9]<sensorData[10] && i<1000){
			motorDriveTo(LEFT, BASESPEED/3);
			++i;
		}
		motorBrake();
		i = 1000;
		while(sensorData[9]>sensorData[10] && i>0){
			motorDriveTo(RIGHT, BASESPEED/3);
			--i;
		}
		motorBrake();
	}

	motorBrake();
	motorResetAllSteps();
}
