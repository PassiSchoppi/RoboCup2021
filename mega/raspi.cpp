#include "raspi.h"

void raspiInit()
{
	// Serial2.begin(9600);
}

bool raspiRead()
{
	bool vic;
	vic = false;
	while(Serial.available()){
		LEDSetColor(GREEN);
		if(Serial.read() == 69){
			vic = true;
			uint8_t value[3] = {0, 0, 0};
			value[0] = Serial.read()-48;
			value[1] = Serial.read()-48;
			value[2] = Serial.read()-48;
			mapPutVitimAt(value[0], value[1], value[2]);

			int timeWait = 250;

			for(int i = 0; i<value[0];i ++){
				LEDSetColor(RED);
				delay(timeWait);
				LEDSetColor(OFF);
				delay(timeWait);
			}
			for(int i = 0; i<value[1];i ++){
				LEDSetColor(BLUE);
				delay(timeWait);
				LEDSetColor(OFF);
				delay(timeWait);
			}
			for(int i = 0; i<value[2];i ++){
				LEDSetColor(GREEN);
				delay(timeWait);
				LEDSetColor(OFF);
				delay(timeWait);
			}
		}
		//if(Serial.read() == 69){
	}
	return(vic);
	return;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////7


	if(	Serial.available()>3 )//&& counter<(WAITINGTIME*100))
	{
		uint8_t value[3] = {0, 0, 0};
		LEDSetColor(GREEN);
		
		if(Serial.read() == 69){
			value[0] = Serial.read()-48;
			value[1] = Serial.read()-48;
			value[2] = Serial.read()-48;
			
			// delay(100);

			int timeWait = 250;

			for(int i = 0; i<value[0];i ++){
				LEDSetColor(RED);
				delay(timeWait);
				LEDSetColor(OFF);
				delay(timeWait);
			}
			for(int i = 0; i<value[1];i ++){
				LEDSetColor(BLUE);
				delay(timeWait);
				LEDSetColor(OFF);
				delay(timeWait);
			}
			for(int i = 0; i<value[2];i ++){
				LEDSetColor(GREEN);
				delay(timeWait);
				LEDSetColor(OFF);
				delay(timeWait);
			}
			
			
			mapPutVitimAt(value[0], value[1], value[2]);
		}else{
			for(int i = 0; i<50;i ++){
				LEDSetColor(RED);
				delay(10);
				LEDSetColor(OFF);
				delay(10);
			}
			while(Serial.available() != 0){
				Serial.read();
			}
		}
		

		
	}

}
