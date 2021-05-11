#include "wall.h"
#include "config.h"

bool wallExists(uint8_t where)
{
	switch(where) 
	{
		case FRONT :
			if(sensorData[6]>DISTANCETOWALL && sensorData[7]>DISTANCETOWALL && sensorData[8]>DISTANCETOWALL)
			{
			  	return true;
			}
			break;

		case RIGHT :
			if(sensorData[2]>DISTANCETOWALL && sensorData[3]>DISTANCETOWALL)
			{
			 	return true;
			}	
			break;

		case BACK :
			if(sensorData[9]>DISTANCETOWALL && sensorData[10]>DISTANCETOWALL)
			{
				return true;
			}
			break;
		
		case LEFT :
			if(sensorData[0]>DISTANCETOWALL && sensorData[1]>DISTANCETOWALL)
			{
				return true;
			}
			break;
	}
	return false;
}
