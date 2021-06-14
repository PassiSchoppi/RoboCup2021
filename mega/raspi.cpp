#include "raspi.h"

void raspiInit()
{
	// Serial2.begin(9600);
}

char raspiRead()
{
	char value;
	value = ' ';
	while(Serial.available())
	{
		value = Serial.readStringUntil('\n')[0];
	}
	// return('h');
	return(value);
}
