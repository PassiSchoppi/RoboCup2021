#include <Arduino.h>
#include "state.h"
#include "motor.h"
#include "sensor.h"
#include "wall.h"
#include "led.h"
#include "config.h"
#include "stabilize.h"
#include "map.h"
#include "ramp.h"
#include "raspi.h"

int average;
uint8_t lastState=0;
uint8_t nextState=0;
// bool seenVic = false;
uint8_t moveTo = 5;
bool outOfField = false;
bool overHalfOfTurn = false;
bool overHalfOfRamp = false;
bool mapIsFine = true;
bool frontIsBlack = false;
int numberOfStepsBeforBlack = 0;
char visVictim = 'e';
uint8_t numberOfKits = 0;

uint8_t nothing()
{
	return 0;
}

void stateChange()
{
	// Serial.print("state: ");
	// Serial.println(state);
	// set back to last silver field
	// mapIsFine = mapSetBackToLastSilver();
/*
 ▄▄▄▄▄▄▄ ▄     ▄ ▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄   ▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ 
█       █ █ ▄ █ █   █       █       █  █ █  █       █       █
█  ▄▄▄▄▄█ ██ ██ █   █▄     ▄█       █  █▄█  █    ▄▄▄█  ▄▄▄▄▄█
█ █▄▄▄▄▄█       █   █ █   █ █     ▄▄█       █   █▄▄▄█ █▄▄▄▄▄ 
█▄▄▄▄▄  █       █   █ █   █ █    █  █   ▄   █    ▄▄▄█▄▄▄▄▄  █
 ▄▄▄▄▄█ █   ▄   █   █ █   █ █    █▄▄█  █ █  █   █▄▄▄ ▄▄▄▄▄█ █
█▄▄▄▄▄▄▄█▄▄█ █▄▄█▄▄▄█ █▄▄▄█ █▄▄▄▄▄▄▄█▄▄█ █▄▄█▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█
*/
	if(sensorData[16] == 0) // PAUSE / LACK OF PROGRESS SWITCH
	{
		LEDSetColor(WHITE);
		// Serial.println("pausing");
		motorBrake();
		mapSetBackToLastSilver();
		state = 1;
		return;
	}
	if(sensorData[17] == 0)
	{
		motorBrake();
		motorResetAllSteps();
		// Serial.println("resetting");
		mapClear();
		state = 1;
		return;
	}

	switch(state) 
	{
		case 0:
		{
			// the end
			motorBrake();
			state = nothing();
			LEDSetColor(OFF);
			break;
		}
		case 1:
		{
			/*
			 ▄▄▄▄▄▄  ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄ ▄▄▄▄▄▄▄ ▄▄    ▄ 
			█      ██       █       █   █       █   █       █  █  █ █
			█  ▄    █    ▄▄▄█       █   █  ▄▄▄▄▄█   █   ▄   █   █▄█ █
			█ █ █   █   █▄▄▄█     ▄▄█   █ █▄▄▄▄▄█   █  █ █  █       █
			█ █▄█   █    ▄▄▄█    █  █   █▄▄▄▄▄  █   █  █▄█  █  ▄    █
			█       █   █▄▄▄█    █▄▄█   █▄▄▄▄▄█ █   █       █ █ █   █
			█▄▄▄▄▄▄██▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█▄▄▄█▄▄▄▄▄▄▄█▄▄▄█▄▄▄▄▄▄▄█▄█  █▄▄█
			*/
			outOfField = false;
			overHalfOfTurn = false;
			motorBrake();
			motorResetAllSteps();
			// Serial.println("new status");
			LEDSetColor(WHITE);
			
			// delay(250);

			if(mapIsFine)
			{
				mapIsFine = true; //mapIsMapFine();
			}

			if(mapIsFine)
			{
				
			}

			if( !DOMAP || !sensorData[15] )
			{
				// ## get direction to drive to ##
				/*   ▄     ▄ ▄▄▄▄▄▄ ▄▄▄     ▄▄▄        ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄     ▄▄▄     ▄▄▄▄▄▄▄ ▄     ▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄   
					█ █ ▄ █ █      █   █   █   █      █       █       █   █   █   █   █       █ █ ▄ █ █       █   ▄  █  
					█ ██ ██ █  ▄   █   █   █   █      █    ▄▄▄█   ▄   █   █   █   █   █   ▄   █ ██ ██ █    ▄▄▄█  █ █ █  
					█       █ █▄█  █   █   █   █      █   █▄▄▄█  █ █  █   █   █   █   █  █ █  █       █   █▄▄▄█   █▄▄█▄ 
					█       █      █   █▄▄▄█   █▄▄▄   █    ▄▄▄█  █▄█  █   █▄▄▄█   █▄▄▄█  █▄█  █       █    ▄▄▄█    ▄▄  █
					█   ▄   █  ▄   █       █       █  █   █   █       █       █       █       █   ▄   █   █▄▄▄█   █  █ █
					█▄▄█ █▄▄█▄█ █▄▄█▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█  █▄▄▄█   █▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█▄▄█ █▄▄█▄▄▄▄▄▄▄█▄▄▄█  █▄█
				*/
								
				if(!wallExists(RIGHT) && !frontIsBlack)
				{
					// rechts drehen dann gerade aus
					// Serial.println("Rechts abbiegen!");
					state = 2;
					
				visVictim = raspiRead();
				if(visVictim != 'e' && !mapVictimIsAtCurrentField())
				{
					lastState = state;
					state = 6;
					return;
				}
					// mapMoveTo(RIGHT);
					// Serial.println("checngin state to: RIGHT");
					return;
				}
				if(!wallExists(FRONT) && !frontIsBlack)
				{
					// gerade aus
					// Serial.println("Gerade aus!");
					state = 3;
					
				visVictim = raspiRead();
				if(visVictim != 'e' && !mapVictimIsAtCurrentField())
				{
					lastState = state;
					state = 6;
					return;
				}
					// mapMoveTo(FRONT);
					// Serial.println("checngin state to: FRONT");
					return;
				}
				if(!wallExists(LEFT))
				{
					// links drehen dann gerade aus
					// Serial.println("Links abbiegen!");
					state = 4;
					
				visVictim = raspiRead();
				if(visVictim != 'e' && !mapVictimIsAtCurrentField())
				{
					lastState = state;
					state = 6;
					return;
				}
					// mapMoveTo(LEFT);
					// Serial.println("checngin state to: LEFT");
					frontIsBlack = false;
					return;
				}
				// wenn rechts und forne und links eine wand ist aber hinten keine
				if(!wallExists(BACK))
				{
					// 2x links drehen dann gerade aus
					// Serial.println("Nach hinten!");
					state = 5;
					
				visVictim = raspiRead();
				if(visVictim != 'e' && !mapVictimIsAtCurrentField())
				{
					lastState = state;
					state = 6;
					return;
				}
					// mapMoveTo(BACK);
					// Serial.println("checngin state to: BACK");
					frontIsBlack = false;
					return;
				}

				// wenn überall Wände sind:::
				state = 1;
				frontIsBlack = false;
				LEDSetColor(OFF);
				break;
			}
			else
			{
				/* 	 ▄▄   ▄▄ ▄▄▄▄▄▄ ▄▄▄▄▄▄▄ 
					█  █▄█  █      █       █
					█       █  ▄   █    ▄  █
					█       █ █▄█  █   █▄█ █
					█       █      █    ▄▄▄█
					█ ██▄██ █  ▄   █   █    
					█▄█   █▄█▄█ █▄▄█▄▄▄█    
					*/
				motorResetAllSteps();
				mapUpdateField();

				uint8_t compasToGoTo = mapWhereToDrive();
				// Serial.print("going: ");
				// Serial.println(compasToGoTo);
				switch ( mapCompasToDirection( compasToGoTo ) ) {
					case FRONT:
						state = 3;
						break;
					case RIGHT:
						state = 2;
						break;
					case BACK:
						state = 5;
						break;
					case LEFT:
						state = 4;
						break;
					case 5:
						// Serial.println("ARE WE HOME JET????");
						mapReturnToHome();
						// Serial.println("SOON!!!!");
						state = 1;
						break;
				}

				visVictim = raspiRead();
				if(visVictim != 'e' && !mapVictimIsAtCurrentField())
				{
					lastState = state;
					state = 6;
					return;
				}

				if( mapCompasToDirection( compasToGoTo ) == 5 )
				{
					// Serial.println("ARE WE HOME JET????");
					mapReturnToHome();
					// Serial.println("SOON!!!!");
					state = 1;
				}
				// Serial.println("letzzz gooo");
				mapDisplay();
				// mapMoveTo( mapCompasToDirection( compasToGoTo ) );
			}
			break;
		}
		case 2:
		{
			/*
			 ▄▄▄▄▄▄   ▄▄▄ ▄▄▄▄▄▄▄ ▄▄   ▄▄ ▄▄▄▄▄▄▄    ▄▄▄▄▄▄▄ ▄▄   ▄▄ ▄▄▄▄▄▄▄ ▄▄    ▄    ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄   ▄▄▄▄▄▄ ▄▄▄ ▄▄▄▄▄▄▄ ▄▄   ▄▄ ▄▄▄▄▄▄▄ 
			█   ▄  █ █   █       █  █ █  █       █  █       █  █ █  █       █  █  █ █  █       █       █   ▄  █ █      █   █       █  █ █  █       █
			█  █ █ █ █   █   ▄▄▄▄█  █▄█  █▄     ▄█  █▄     ▄█  █▄█  █    ▄▄▄█   █▄█ █  █  ▄▄▄▄▄█▄     ▄█  █ █ █ █  ▄   █   █   ▄▄▄▄█  █▄█  █▄     ▄█
			█   █▄▄█▄█   █  █  ▄▄█       █ █   █      █   █ █       █   █▄▄▄█       █  █ █▄▄▄▄▄  █   █ █   █▄▄█▄█ █▄█  █   █  █  ▄▄█       █ █   █  
			█    ▄▄  █   █  █ █  █   ▄   █ █   █      █   █ █   ▄   █    ▄▄▄█  ▄    █  █▄▄▄▄▄  █ █   █ █    ▄▄  █      █   █  █ █  █   ▄   █ █   █  
			█   █  █ █   █  █▄▄█ █  █ █  █ █   █      █   █ █  █ █  █   █▄▄▄█ █ █   █   ▄▄▄▄▄█ █ █   █ █   █  █ █  ▄   █   █  █▄▄█ █  █ █  █ █   █  
			█▄▄▄█  █▄█▄▄▄█▄▄▄▄▄▄▄█▄▄█ █▄▄█ █▄▄▄█      █▄▄▄█ █▄▄█ █▄▄█▄▄▄▄▄▄▄█▄█  █▄▄█  █▄▄▄▄▄▄▄█ █▄▄▄█ █▄▄▄█  █▄█▄█ █▄▄█▄▄▄█▄▄▄▄▄▄▄█▄▄█ █▄▄█ █▄▄▄█  
			*/
			LEDSetColor(GREEN);
			motorDriveTo(RIGHT, BASESPEED);
			// drive RIGHT until average > STEPFFORRIGHT
			average = 0;
			for(uint8_t i=0; i<4; ++i){
				average = average + abs(motorStepsMade(i));
			}
			average = average/4;
			if(average>STEPFFORRIGHT*0.45 && !overHalfOfTurn){
				overHalfOfTurn = true;
				mapOnlyTurnTo(RIGHT);
			}
			if( average > STEPFFORRIGHT )
			{
				motorResetAllSteps();
				motorBrake();
				motorBrake();
				visVictim = raspiRead();
				if(visVictim != 'e' && !mapVictimIsAtCurrentField())
				{
					lastState = 3;
					state = 6;
					return;
				}
				stabilize();
				// gerade aus
				state = 3;
			}
			// wenn da ist ein Victim
			// visVictim = raspiRead();
			if( sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)// || visVictim != 'e' )
			{
				lastState = state;
				state = 6;
			}
			break;
		}
		case 3:
		{
			/*   ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄   ▄▄▄▄▄▄ ▄▄▄ ▄▄▄▄▄▄▄ ▄▄   ▄▄ ▄▄▄▄▄▄▄ 
				█       █       █   ▄  █ █      █   █       █  █ █  █       █
				█  ▄▄▄▄▄█▄     ▄█  █ █ █ █  ▄   █   █   ▄▄▄▄█  █▄█  █▄     ▄█
				█ █▄▄▄▄▄  █   █ █   █▄▄█▄█ █▄█  █   █  █  ▄▄█       █ █   █  
				█▄▄▄▄▄  █ █   █ █    ▄▄  █      █   █  █ █  █   ▄   █ █   █  
				 ▄▄▄▄▄█ █ █   █ █   █  █ █  ▄   █   █  █▄▄█ █  █ █  █ █   █  
				█▄▄▄▄▄▄▄█ █▄▄▄█ █▄▄▄█  █▄█▄█ █▄▄█▄▄▄█▄▄▄▄▄▄▄█▄▄█ █▄▄█ █▄▄▄█  
				*/
			// Serial.println("driving straigt");
			LEDSetColor(BLUE);
			motorDriveTo(FRONT, BASESPEED);
			// gerade aus bis average > STEPSFORONE
			average = 0;
			for(uint8_t i=0; i<4; ++i)
			{
				average = average + motorStepsMade(i);
			}
			average = average/4;
			if(average > STEPSFORONE*0.45 && !outOfField){
				LEDSetColor(RED);
				delay(100);
				mapOnlyMoveTo(FRONT);
				outOfField = true;
			}
			//6   FL,
			//7   FC,
			//8   FR,
			int plusToDis;
			plusToDis = 13;
			if( average>STEPSFORONE || (sensorData[6]>PERFECTDISTTOW-plusToDis && sensorData[7]>PERFECTDISTTOW-plusToDis && sensorData[8]>PERFECTDISTTOW-plusToDis) )
			{
				motorBrake();
				motorResetAllSteps();
				// stabilize und dann neue entscheidung
				state = 8;
				break;
			}
						
			// wenn zu nah an einer Wand oder obstacle
			if( sensorData[7]>120 )
			{
				// kurz zurück und dann neu entscheiden
				motorBrake();
				state = 9;
				break;
			}

			if(DORAMPDETECTION)
			{

				// wenn on ramp down
				if( sensorData[4] < level - RAMP_THRESHOLD)
				{
					// motorResetAllSteps();
					overHalfOfRamp = false;

					// ramp down
					state = 11;
					break;
				}

				// wenn on ramp up
				if( sensorData[4] > level + RAMP_THRESHOLD)
				{
					// motorResetAllSteps();
					overHalfOfRamp = false;

					// ramp up
					state = 12;
					break;
				}
			}
			
			// wenn Victim
			// visVictim = raspiRead();
			if( (sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)&&!mapVictimIsAtCurrentField() )// || visVictim != 'e' )
			{
				Serial.println("maybe victim ?");
				lastState = state;
				state = 6;
				break;
			}

			// if black tile
			if( sensorData[13]>MAXWHITE || sensorData[14]>MAXWHITE )
			{
				// THIS \/ is the right order!
				if(outOfField){
					mapBlackFieldCurrent();
				}else{
					mapBlackFieldFront();
				}
				frontIsBlack = true;
				state = 10;
				numberOfStepsBeforBlack = motorAverageSteps();
				break;
			}
			break;
		}
		case 4:
		{
			/*
			 ▄▄▄     ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄    ▄▄▄▄▄▄▄ ▄▄   ▄▄ ▄▄▄▄▄▄▄ ▄▄    ▄    ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄   ▄▄▄▄▄▄ ▄▄▄ ▄▄▄▄▄▄▄ ▄▄   ▄▄ ▄▄▄▄▄▄▄ 
			█   █   █       █       █       █  █       █  █ █  █       █  █  █ █  █       █       █   ▄  █ █      █   █       █  █ █  █       █
			█   █   █    ▄▄▄█    ▄▄▄█▄     ▄█  █▄     ▄█  █▄█  █    ▄▄▄█   █▄█ █  █  ▄▄▄▄▄█▄     ▄█  █ █ █ █  ▄   █   █   ▄▄▄▄█  █▄█  █▄     ▄█
			█   █   █   █▄▄▄█   █▄▄▄  █   █      █   █ █       █   █▄▄▄█       █  █ █▄▄▄▄▄  █   █ █   █▄▄█▄█ █▄█  █   █  █  ▄▄█       █ █   █  
			█   █▄▄▄█    ▄▄▄█    ▄▄▄█ █   █      █   █ █   ▄   █    ▄▄▄█  ▄    █  █▄▄▄▄▄  █ █   █ █    ▄▄  █      █   █  █ █  █   ▄   █ █   █  
			█       █   █▄▄▄█   █     █   █      █   █ █  █ █  █   █▄▄▄█ █ █   █   ▄▄▄▄▄█ █ █   █ █   █  █ █  ▄   █   █  █▄▄█ █  █ █  █ █   █  
			█▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█▄▄▄█     █▄▄▄█      █▄▄▄█ █▄▄█ █▄▄█▄▄▄▄▄▄▄█▄█  █▄▄█  █▄▄▄▄▄▄▄█ █▄▄▄█ █▄▄▄█  █▄█▄█ █▄▄█▄▄▄█▄▄▄▄▄▄▄█▄▄█ █▄▄█ █▄▄▄█  
			*/
			LEDSetColor(TURQUOISE);
			motorDriveTo(LEFT, BASESPEED);
			// Left until average > STEPSFORLEFT
			average = 0;
			for(uint8_t i=0; i<4; ++i)
			{
					average = average + abs(motorStepsMade(i));
			}
			average = average/4;
			if(average>STEPSFORLEFT*0.45 && !overHalfOfTurn){
				overHalfOfTurn = true;
				mapOnlyTurnTo(LEFT);
			}
			if(average > STEPSFORLEFT)
			{
				motorResetAllSteps();
				stabilize();
				motorBrake();
				overHalfOfTurn = false;
				visVictim = raspiRead();
				if(visVictim != 'e' && !mapVictimIsAtCurrentField())
				{
					lastState = 3;
					state = 6;
					return;
				}
				// gerade aus
				state = 3;
			}
			// wen victim
			// visVictim = raspiRead();
			if( sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)// || visVictim != 'e' )
			{
				lastState = state;
				state = 6;
			}
			break;
		}
		case 5:
		{
		/*	 ▄▄▄▄▄▄▄ ▄▄   ▄▄    ▄▄▄     ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄    ▄▄▄▄▄▄▄ ▄▄   ▄▄ ▄▄▄▄▄▄▄ ▄▄    ▄    ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄   ▄▄▄▄▄▄ ▄▄▄ ▄▄▄▄▄▄▄ ▄▄   ▄▄ ▄▄▄▄▄▄▄ 
			█       █  █▄█  █  █   █   █       █       █       █  █       █  █ █  █       █  █  █ █  █       █       █   ▄  █ █      █   █       █  █ █  █       █
			█▄▄▄▄   █       █  █   █   █    ▄▄▄█    ▄▄▄█▄     ▄█  █▄     ▄█  █▄█  █    ▄▄▄█   █▄█ █  █  ▄▄▄▄▄█▄     ▄█  █ █ █ █  ▄   █   █   ▄▄▄▄█  █▄█  █▄     ▄█
			 ▄▄▄▄█  █       █  █   █   █   █▄▄▄█   █▄▄▄  █   █      █   █ █       █   █▄▄▄█       █  █ █▄▄▄▄▄  █   █ █   █▄▄█▄█ █▄█  █   █  █  ▄▄█       █ █   █  
			█ ▄▄▄▄▄▄██     █   █   █▄▄▄█    ▄▄▄█    ▄▄▄█ █   █      █   █ █   ▄   █    ▄▄▄█  ▄    █  █▄▄▄▄▄  █ █   █ █    ▄▄  █      █   █  █ █  █   ▄   █ █   █  
			█ █▄▄▄▄▄█   ▄   █  █       █   █▄▄▄█   █     █   █      █   █ █  █ █  █   █▄▄▄█ █ █   █   ▄▄▄▄▄█ █ █   █ █   █  █ █  ▄   █   █  █▄▄█ █  █ █  █ █   █  
			█▄▄▄▄▄▄▄█▄▄█ █▄▄█  █▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█▄▄▄█     █▄▄▄█      █▄▄▄█ █▄▄█ █▄▄█▄▄▄▄▄▄▄█▄█  █▄▄█  █▄▄▄▄▄▄▄█ █▄▄▄█ █▄▄▄█  █▄█▄█ █▄▄█▄▄▄█▄▄▄▄▄▄▄█▄▄█ █▄▄█ █▄▄▄█  
			*/
			LEDSetColor(TURQUOISE);
			motorDriveTo(LEFT, BASESPEED);
			// left bis average = STEPSFORLEFT
			average = 0;
			for(uint8_t i=0; i<4; ++i)
			{
				average = average + abs(motorStepsMade(i));
			}
			average = average/4;
			if(average>STEPSFORLEFT*0.45 && !overHalfOfTurn){
				overHalfOfTurn = true;
				mapOnlyTurnTo(LEFT);
			}
			if(average > STEPSFORLEFT)
			{
				motorResetAllSteps();
				stabilize();
				motorBrake();
				overHalfOfTurn = false;
				visVictim = raspiRead();
				if(visVictim != 'e' && !mapVictimIsAtCurrentField())
				{
					lastState = 4;
					state = 6;
					return;
				}
				// dann left und dann gerade aus
				state = 4;
			}
			// wenn victim
			//visVictim = raspiRead();
			if( sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)// || visVictim != 'e' )
			{
				lastState = state;
				state = 6;
			}
			break;
		}
		case 6:
		{
			/*
			 ▄▄   ▄▄ ▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄ ▄▄   ▄▄ 
			█  █ █  █   █       █       █   █  █▄█  █
			█  █▄█  █   █       █▄     ▄█   █       █
			█       █   █     ▄▄█ █   █ █   █       █
			█       █   █    █    █   █ █   █       █
			 █     ██   █    █▄▄  █   █ █   █ ██▄██ █
			  █▄▄▄█ █▄▄▄█▄▄▄▄▄▄▄█ █▄▄▄█ █▄▄▄█▄█   █▄█
			*/
			// try for 5 seconds and blink
			// wenn noch kein victim auf dem Feld ist
			
			// Serial.println();
			// Serial.print("FOUND");

			bool victimIsLeftNotRight = false;
			if ( !mapVictimIsAtCurrentField() )
			{
				Serial.println(" NEW VICTIM");
				// zweites mal testen und seite herausfinden
				if(visVictim == 'e'){
					
					if ( sensorData[11]>VICTIMTEMP )
					{
						victimIsLeftNotRight = true;
					}else if (sensorData[12]>VICTIMTEMP)
					{
						victimIsLeftNotRight = false;
					}else{
						Serial.println("DIDNT FOUND SECOND TEST");
						Serial.println("RETURNING TO LAST STATE");
						state = lastState;
						break;
					}
				}else{
					if(visVictim == 'H' || visVictim == 'U' || visVictim == 'S' || 
					   visVictim == 'G' || visVictim == 'Y' || visVictim == 'R'){
						   victimIsLeftNotRight = false;
					}else if (visVictim == 'h' || visVictim == 'u' || visVictim == 's' || 
					   		  visVictim == 'g' || visVictim == 'y' || visVictim == 'r')
					{
						victimIsLeftNotRight = true;
					}else{
						Serial.println("NONE OF THE LETTERS WERE GIVEN");
						Serial.println("RETURNING TO LAST STATE");
						state = lastState;
						break;
					}
					
				}

				if((victimIsLeftNotRight && !wallExists(LEFT)) ||
				   (!victimIsLeftNotRight && !wallExists(RIGHT)))
				{
					Serial.println("victim not on a wall");
					state = lastState;
					return;
				}
				
				// sekunde 1
				motorBrake();
				LEDSetColor(RED);
				delay(1000);
				// sekunde 2
				LEDSetColor(OFF);
				delay(1000);
				// senkunde 3
				LEDSetColor(RED);
				delay(1000);
				// sekunde 4
				LEDSetColor(OFF);
				delay(1000);
				// sekunde 5
				LEDSetColor(RED);
				delay(1000);
				
				// abwurf only if switch to run program is ON
				if(sensorData[16]){
					numberOfKits = 1;
					if(visVictim != 'e'){
						if(visVictim == 'h' || visVictim == 'H'){
							numberOfKits = 3;
						}else if (visVictim == 's' || visVictim == 'S')
						{
							numberOfKits = 2;
						}else if (visVictim == 'y' || visVictim == 'Y' ||
						          visVictim == 'r' || visVictim == 'R')
						{
							numberOfKits = 1;
						}else if (visVictim == 'u' || visVictim == 'U' ||
						          visVictim == 'g' || visVictim == 'G')
						{
							numberOfKits = 0;
						}else{
							Serial.println("SOMETHING WENT WRONG");
							state = lastState;
							return;
						}
						
					}

					// mark current field as victim
					mapVictimNewAtCurrentField();

					for(uint8_t i = 0; i < numberOfKits; i++){
						Serial.println("KIT");

						kitdropperSetTo(POSMIDD);
						delay(1000);
						
						if( victimIsLeftNotRight )
						{
							kitdropperSetTo(POSLEFT);
						}
						else
						{
							kitdropperSetTo(POSRIGHT);
						}
						// 2 sekunden warten und dann zurück zu mitte
						delay(2000);
						kitdropperSetTo(POSMIDD);
					}
				}
			}else{
				Serial.println(" OLD VICTIM");
				Serial.println("returning to last state:");
				Serial.println(lastState);
				state = lastState;
			}
			
			// Serial.println("RETURNING TO LAST STATE");
			// zurück zu dem was er gerade gemacht hat
			state = lastState;
			break;
		}
		case 7:
		{
			// freier State
			break;
		}
		case 8:
		{
			/*   ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄ ▄▄▄     ▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ 
				█       █       █      █  ▄    █   █   █   █   █       █       █
				█  ▄▄▄▄▄█▄     ▄█  ▄   █ █▄█   █   █   █   █   █▄▄▄▄   █    ▄▄▄█
				█ █▄▄▄▄▄  █   █ █ █▄█  █       █   █   █   █   █▄▄▄▄█  █   █▄▄▄ 
				█▄▄▄▄▄  █ █   █ █      █  ▄   ██   █   █▄▄▄█   █ ▄▄▄▄▄▄█    ▄▄▄█
				 ▄▄▄▄▄█ █ █   █ █  ▄   █ █▄█   █   █       █   █ █▄▄▄▄▄█   █▄▄▄ 
				█▄▄▄▄▄▄▄█ █▄▄▄█ █▄█ █▄▄█▄▄▄▄▄▄▄█▄▄▄█▄▄▄▄▄▄▄█▄▄▄█▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█
				*/
			// Serial.println("stabalizing");
			LEDSetColor(PINK);
			motorBrake();
			stabilize();
			motorBrake();
			motorResetAllSteps();
			state = 1;
			// Serial.print("state: ");
			// Serial.println(*state);
			// seenVic = false;
			break;
		}
		case 9:
		{
			/*
			 ▄▄▄▄▄▄▄ ▄▄   ▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄   ▄▄▄▄▄▄▄    ▄▄▄▄▄▄▄ ▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄   ▄ 
			█       █  █ █  █       █   ▄  █ █       █  █  ▄    █      █       █   █ █ █
			█  ▄▄▄▄▄█  █▄█  █   ▄   █  █ █ █ █▄     ▄█  █ █▄█   █  ▄   █       █   █▄█ █
			█ █▄▄▄▄▄█       █  █ █  █   █▄▄█▄  █   █    █       █ █▄█  █     ▄▄█      ▄█
			█▄▄▄▄▄  █   ▄   █  █▄█  █    ▄▄  █ █   █    █  ▄   ██      █    █  █     █▄ 
			 ▄▄▄▄▄█ █  █ █  █       █   █  █ █ █   █    █ █▄█   █  ▄   █    █▄▄█    ▄  █
			█▄▄▄▄▄▄▄█▄▄█ █▄▄█▄▄▄▄▄▄▄█▄▄▄█  █▄█ █▄▄▄█    █▄▄▄▄▄▄▄█▄█ █▄▄█▄▄▄▄▄▄▄█▄▄▄█ █▄█
			*/
			motorBrake();
			motorResetAllSteps();
			motorDriveTo(BACK, BASESPEED);
			while(motorStepsMade(0)<15){}
			motorResetAllSteps();
			motorBrake();
			// stabilize und dann neu entscheiden
			state = 8;
			break;
		}
		case 10:
		{
			/*   ▄▄▄▄▄▄▄ ▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄   ▄    ▄▄▄▄▄▄▄ ▄▄▄▄▄▄   ▄▄▄▄▄▄▄ ▄▄   ▄▄    ▄▄▄▄▄▄▄ ▄▄▄     ▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄   ▄ 
				█  ▄    █      █       █   █ █ █  █       █   ▄  █ █       █  █▄█  █  █  ▄    █   █   █      █       █   █ █ █
				█ █▄█   █  ▄   █       █   █▄█ █  █    ▄▄▄█  █ █ █ █   ▄   █       █  █ █▄█   █   █   █  ▄   █       █   █▄█ █
				█       █ █▄█  █     ▄▄█      ▄█  █   █▄▄▄█   █▄▄█▄█  █ █  █       █  █       █   █   █ █▄█  █     ▄▄█      ▄█
				█  ▄   ██      █    █  █     █▄   █    ▄▄▄█    ▄▄  █  █▄█  █       █  █  ▄   ██   █▄▄▄█      █    █  █     █▄ 
				█ █▄█   █  ▄   █    █▄▄█    ▄  █  █   █   █   █  █ █       █ ██▄██ █  █ █▄█   █       █  ▄   █    █▄▄█    ▄  █
				█▄▄▄▄▄▄▄█▄█ █▄▄█▄▄▄▄▄▄▄█▄▄▄█ █▄█  █▄▄▄█   █▄▄▄█  █▄█▄▄▄▄▄▄▄█▄█   █▄█  █▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█▄█ █▄▄█▄▄▄▄▄▄▄█▄▄▄█ █▄█
				*/
			motorBrake();
			motorDriveTo(BACK, BASESPEED);
			while(motorStepsMade(0) < (numberOfStepsBeforBlack * 2)){}
			numberOfStepsBeforBlack = 0;
			motorResetAllSteps();
			motorBrake();
			// stabilize und dann neu entscheiden
			state = 8;
			break;
		}
		case 11:
		{
			/*
			 ▄▄▄▄▄▄   ▄▄▄▄▄▄ ▄▄   ▄▄ ▄▄▄▄▄▄▄    ▄▄▄▄▄▄  ▄▄▄▄▄▄▄ ▄     ▄ ▄▄    ▄ 
			█   ▄  █ █      █  █▄█  █       █  █      ██       █ █ ▄ █ █  █  █ █
			█  █ █ █ █  ▄   █       █    ▄  █  █  ▄    █   ▄   █ ██ ██ █   █▄█ █
			█   █▄▄█▄█ █▄█  █       █   █▄█ █  █ █ █   █  █ █  █       █       █
			█    ▄▄  █      █       █    ▄▄▄█  █ █▄█   █  █▄█  █       █  ▄    █
			█   █  █ █  ▄   █ ██▄██ █   █      █       █       █   ▄   █ █ █   █
			█▄▄▄█  █▄█▄█ █▄▄█▄█   █▄█▄▄▄█      █▄▄▄▄▄▄██▄▄▄▄▄▄▄█▄▄█ █▄▄█▄█  █▄▄█
			*/
			if(overHalfOfRamp){
				LEDSetColor(WHITE);
			}else
				LEDSetColor(YELLOW);
			motorDriveTo(FRONT, BASESPEED*0.5);

			if ( motorAverageSteps() > STEPSFORHALFARAMP && !overHalfOfRamp )
			{
				// next story
				if(DORAMPONMAP){
				mapRampAtCurrentField();
				}
				overHalfOfRamp = true;
			}

			// when level
			if( sensorData[4] > level - RAMP_THRESHOLD )
			{
				// kurz vorwärts fahren dann neu entscheiden
				state = 13;
				if(!overHalfOfRamp){
					state = 3;
				}
			}

			// wenn victim
			//visVictim = raspiRead();
			if( sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)// || visVictim != 'e' )
			{
				lastState = state;
				state = 6;
			}
			break;
		}
		case 12:
		{
			/*
			 ▄▄▄▄▄▄   ▄▄▄▄▄▄ ▄▄   ▄▄ ▄▄▄▄▄▄▄    ▄▄   ▄▄ ▄▄▄▄▄▄▄ 
			█   ▄  █ █      █  █▄█  █       █  █  █ █  █       █
			█  █ █ █ █  ▄   █       █    ▄  █  █  █ █  █    ▄  █
			█   █▄▄█▄█ █▄█  █       █   █▄█ █  █  █▄█  █   █▄█ █
			█    ▄▄  █      █       █    ▄▄▄█  █       █    ▄▄▄█
			█   █  █ █  ▄   █ ██▄██ █   █      █       █   █    
			█▄▄▄█  █▄█▄█ █▄▄█▄█   █▄█▄▄▄█      █▄▄▄▄▄▄▄█▄▄▄█    
			*/
			if(overHalfOfRamp){
				LEDSetColor(WHITE);
				motorDriveTo(RAMPSTATE, BASESPEED*1.5);
			}else
			{
				LEDSetColor(YELLOW);
				motorDriveTo(RAMPSTATE, BASESPEED*1.5);
			}
			/*motorSetSpeed(0, BASESPEED);
			motorSetSpeed(1, BASESPEED*3);
			motorSetSpeed(2, 60);
			motorSetSpeed(3, BASESPEED*2);*/

			if ( motorAverageSteps() > STEPSFORHALFARAMP && !overHalfOfRamp )
			{
				// next story
				if(DORAMPONMAP){
				mapRampAtCurrentField();
				}
				overHalfOfRamp = true;
			}
			
			// when level
			if( sensorData[4] < level + RAMP_THRESHOLD )
			{
				// kurz vorwärts fahren dann neu entscheiden
				state = 13;
				if(!overHalfOfRamp){
					state = 3;
				}
			}

			// wenn victim
			// visVictim = raspiRead();
			if( sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)// || visVictim != 'e' )
			{
				lastState = state;
				state = 6;
			}
			break;
		}
		case 13:
		{
			// kurz vorwärts fahren
			motorBrake();
			motorResetAllSteps();
			motorDriveTo(FRONT, BASESPEED);
			while(motorStepsMade(0)<REST_OF_RAMP){}
			motorResetAllSteps();
			motorBrake();
			// stabilize und dann neu entscheiden
			state = 8;
			break;
		}
	}
}
