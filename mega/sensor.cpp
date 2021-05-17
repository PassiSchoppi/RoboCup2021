#include "sensor.h"

void sensorInit()
{
	Serial1.begin(115200);
	pinMode(INTERUPT_PIN_A, OUTPUT);

	Serial3.begin(9600);
	pinMode(INTERUPT_PIN_B, OUTPUT);

	pinMode(7, OUTPUT);
}

void sensorRead()
{
	uint8_t bufferVar=0;
	uint8_t newBufferVar;
	uint8_t counter=0;

	// 														ALPHA
	// wenn noch Daten da sind kein Interrupt
	if(!Serial1.available())
	{
		digitalWrite(INTERUPT_PIN_A, HIGH);
		digitalWrite(INTERUPT_PIN_A, LOW);
	}
	
	// auf Daten warten
	while(Serial1.available()<6)
	{
		++counter;
		if(counter>100)
		{
			return(0);
		}
	}

	// lesen und in array schreiben
	for(uint8_t i=0; i<6; ++i)
	{
		sensorData[i] = ( sensorData[i]*(SMOOTHENSENSORDATA-1) + Serial1.read() )/SMOOTHENSENSORDATA ;
	}

	// Serial buffer leeren
	while(Serial1.available())
	{
		Serial1.read();
	}
	
	// 														GYRO
	//FIXME overwriting sensorData[4] and 5
	
	
	// 														BETA
	// wenn noch daten da sind kein Interrupt
	if(!Serial3.available())
	{
		digitalWrite(INTERUPT_PIN_B, HIGH);
		digitalWrite(INTERUPT_PIN_B, LOW);
	}
	
	// auf Daten warten
	counter = 0;
	while(Serial3.available()<5)
	{
		++counter;
		if(counter>100)
		{
			return(0);
		}
	}

	// lesen und in array schreiben
	for(uint8_t i=0; i<5; ++i)
	{
		sensorData[i+6] = ( sensorData[i+6]*(SMOOTHENSENSORDATA-1) + Serial3.read() )/SMOOTHENSENSORDATA ;
	}
	
	// Serial buffer leeren
	while(Serial3.available())
	{
		Serial3.read();
	}

	// 														MELEXIS
	melexisInterrupt();
	sensorData[11]=((int)melexisGetValue(0));
	sensorData[12]=((int)melexisGetValue(1));
	
	// 														LIGHT SENSOR
	if( DOBLACKTILEDETECTION ){
		if(digitalRead(7))
		{
			sensorData[13]=(analogRead(A6)>>2)-5;
			sensorData[14]=analogRead(7)>>2;
			digitalWrite(7, LOW);
		}
		else
		{
			digitalWrite(7, HIGH);
		}
	}

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
}
