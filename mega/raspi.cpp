#include "raspi.h"

void raspiInit()
{
	Serial2.begin(9600);
}

void raspiRead()
{
	while(Serial2.available())
	{
		Serial.print("Data: ");
		Serial.println( Serial2.read() );
	}
}
