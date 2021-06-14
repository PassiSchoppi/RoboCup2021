#include "raspi.h"

void raspiInit()
{
	// Serial2.begin(9600);
}

char raspiRead()
{
	char value;
	value = ' ';
	Serial.print('j');
//	while(!Serial.available()){LEDSetColor(YELLOW);}
	LEDSetColor(GREEN);
	while(Serial.available())
	{
		value = Serial.readStringUntil('\n')[0];
	}
	return('e');
	return(value);
}
