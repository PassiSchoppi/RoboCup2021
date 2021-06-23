#include "raspi.h"

void raspiInit()
{
	// Serial2.begin(9600);
}

char raspiRead()
{
	char value;
	int counter;
	counter = 0;
	value = ' ';


	return('e');



	if(wallExists(RIGHT) && !wallExists(LEFT)){
		Serial.print('J');
	}else if (wallExists(LEFT) && !wallExists(RIGHT))
	{
		Serial.print('j');
	}else if (wallExists(LEFT) && wallExists(RIGHT))
	{
		Serial.print('x');
	}else{
		return('e');
	}
	
	// return('h');
	while(!Serial.available() && counter<(WAITINGTIME*100))
	{
		++counter;
		LEDSetColor(YELLOW);
		delay(10);
	}
	if(!(counter<600)){
		LEDSetColor(OFF);
		delay(100);
		return('e');  // (not test case)
	}else{
		LEDSetColor(GREEN);
		while(Serial.available())
		{
			value = Serial.readStringUntil('\n')[0];
		}
		return(value);
	}
}
