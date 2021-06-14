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
bool overHalfOfRamp = false;
bool mapIsFine = true;
bool frontIsBlack = false;
int numberOfStepsBeforBlack = 0;
char visVictim = ' ';
uint8_t numberOfKits = ' ';

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
			outOfField = false;
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
				// 																		RECHTSUMFAHRUNG
				
				/*uint8_t mostLeftFree;
				uint8_t mostLeftWall;
				mostLeftWall = 5;
				mostLeftFree = 5;
				if(mostLeftWall==5 && wallExists(RIGHT)){
					mostLeftWall = RIGHT;
				}
				if(mostLeftWall==5 && wallExists(FRONT) || frontIsBlack){
					mostLeftWall = FRONT;
				}
				if(mostLeftWall==5 && wallExists(LEFT)){
					mostLeftWall = LEFT;
				}
				if(mostLeftWall==5 && wallExists(BACK)){
					mostLeftWall = BACK;
				}
				if(mostLeftWall==5){
					state = 2;
				}
				switch (mostLeftWall)
				{
				case RIGHT:
					if(mostLeftFree==5 && (!wallExists(FRONT) || !frontIsBlack)){
						mostLeftFree = FRONT;
					}
					if(mostLeftFree==5 && !wallExists(LEFT)){
						mostLeftFree = LEFT;
					}
					if(mostLeftFree==5 && !wallExists(BACK)){
						mostLeftFree = BACK;
					}
					break;
				case FRONT:
					if(mostLeftFree==5 && !wallExists(LEFT)){
						mostLeftFree = LEFT;
					}
					if(mostLeftFree==5 && !wallExists(BACK)){
						mostLeftFree = BACK;
					}
					if(mostLeftFree==5 && !wallExists(RIGHT)){
						mostLeftFree = RIGHT;
					}
					break;
				case LEFT:
					if(mostLeftFree==5 && !wallExists(BACK)){
						mostLeftFree = BACK;
					}
					if(mostLeftFree==5 && !wallExists(RIGHT)){
						mostLeftFree = RIGHT;
					}
					if(mostLeftFree==5 && (!wallExists(FRONT) || !frontIsBlack)){
						mostLeftFree = FRONT;
					}
					break;
				case BACK:
					if(mostLeftFree==5 && !wallExists(RIGHT)){
						mostLeftFree = RIGHT;
					}
					if(mostLeftFree==5 && (!wallExists(FRONT) || !frontIsBlack)){
						mostLeftFree = FRONT;
					}
					if(mostLeftFree==5 && !wallExists(LEFT)){
						mostLeftFree = LEFT;
					}
					break;
				}
				switch (mostLeftFree)
				{
				case RIGHT:
					state = 2;
					break;
				case FRONT:
					state = 3;
					break;
				case LEFT:
					state = 4;
					break;
				case BACK:
					state = 5;
					break;
				case 5:
					state = 0;
				}*/
				
				if(!wallExists(RIGHT) && !frontIsBlack)
				{
					// rechts drehen dann gerade aus
					// Serial.println("Rechts abbiegen!");
					state = 2;
					// mapMoveTo(RIGHT);
					// Serial.println("checngin state to: RIGHT");
					return;
				}
				if(!wallExists(FRONT) && !frontIsBlack)
				{
					// gerade aus
					// Serial.println("Gerade aus!");
					state = 3;
					// mapMoveTo(FRONT);
					// Serial.println("checngin state to: FRONT");
					return;
				}
				if(!wallExists(LEFT))
				{
					// links drehen dann gerade aus
					// Serial.println("Links abbiegen!");
					state = 4;
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
				//                                                                             MAP
				motorResetAllSteps();
				mapUpdateField();

				visVictim = raspiRead();
				if(visVictim != 'e' && !mapVictimIsAtCurrentField())
				{
					lastState = 1;
					state = 6;
					return;
				}

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
				if( mapCompasToDirection( compasToGoTo ) == 5 )
				{
					// Serial.println("ARE WE HOME JET????");
					mapReturnToHome();
					// Serial.println("SOON!!!!");
					state = 1;
				}
				// Serial.println("letzzz gooo");
				mapDisplay();
				mapMoveTo( mapCompasToDirection( compasToGoTo ) );
			}
			break;
		}
		case 2:
		{
			// turn right und dann gerade aus
			LEDSetColor(GREEN);
			motorDriveTo(RIGHT, BASESPEED);
			// drive RIGHT until average > STEPFFORRIGHT
			average = 0;
			for(uint8_t i=0; i<4; ++i){
				average = average + abs(motorStepsMade(i));
			}
			average = average/4;
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
			if( sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)// || visVictim != ' ' )
			{
				lastState = state;
				state = 6;
			}
			break;
		}
		case 3:
		{
			// drive staight
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
			if( sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)// || visVictim != ' ' )
			{
				lastState = state;
				state = 6;
				break;
			}

			// if black tile
			if( sensorData[13]>MAXWHITE || sensorData[14]>MAXWHITE )
			{
				mapBlackFieldFront();
				frontIsBlack = true;
				state = 10;
				numberOfStepsBeforBlack = motorAverageSteps();
				break;
			}
			break;
		}
		case 4:
		{
			// drive left dann gerade aus
			LEDSetColor(TURQUOISE);
			motorDriveTo(LEFT, BASESPEED);
			// Left until average > STEPSFORLEFT
			average = 0;
			for(uint8_t i=0; i<4; ++i)
			{
					average = average + abs(motorStepsMade(i));
			}
			average = average/4;
			if(average > STEPSFORLEFT)
			{
				motorResetAllSteps();
				stabilize();
				motorBrake();
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
			if( sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)// || visVictim != ' ' )
			{
				lastState = state;
				state = 6;
			}
			break;
		}
		case 5:
		{
			// drive left then left and then straight
			LEDSetColor(TURQUOISE);
			motorDriveTo(LEFT, BASESPEED);
			// left bis average = STEPSFORLEFT
			average = 0;
			for(uint8_t i=0; i<4; ++i)
			{
				average = average + abs(motorStepsMade(i));
			}
			average = average/4;
			if(average > STEPSFORLEFT)
			{
				motorResetAllSteps();
				stabilize();
				motorBrake();
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
			if( sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)// || visVictim != ' ' )
			{
				lastState = state;
				state = 6;
			}
			break;
		}
		case 6:
		{
			// temp victim
			// try for 5 seconds and blink
			// wenn noch kein victim auf dem Feld ist
			
			// Serial.println();
			// Serial.print("FOUND");
			if (!outOfField){
				mapOnlyMoveTo(BACK);
			}

			bool victimIsLeftNotRight = false;
			if ( !mapVictimIsAtCurrentField() )
			{
				// Serial.println(" NEW VICTIM");
				// zweites mal testen und seite herausfinden
				if(visVictim == ' '){
					
					if ( sensorData[11]>VICTIMTEMP )
					{
						victimIsLeftNotRight = true;
					}else if (sensorData[12]>VICTIMTEMP)
					{
						victimIsLeftNotRight = false;
					}else{
						// Serial.println("DIDNT FOUND SECOND TEST");
						// Serial.println("RETURNING TO LAST STATE");
						state = lastState;
						if(!outOfField){
							mapOnlyMoveTo(FRONT);
						}
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
						// Serial.println("NONE OF THE LETTERS WERE GIVEN");
						// Serial.println("RETURNING TO LAST STATE");
						state = lastState;
						if(!outOfField){
							mapOnlyMoveTo(FRONT);
						}
						break;
					}
					
				}

				if((victimIsLeftNotRight && !wallExists(LEFT)) ||
				   (!victimIsLeftNotRight && !wallExists(RIGHT)))
				{
					// Serial.println("victim not on a wall");
					state = lastState;
					if(!outOfField){
						mapOnlyMoveTo(FRONT);
					}
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
					if(visVictim != ' '){
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
							// Serial.println("SOMETHING WENT WRONG");
							state = lastState;
							if(!outOfField){
								mapOnlyMoveTo(FRONT);
							}
							return;
						}
						
					}

					// mark current field as victim
					mapVictimNewAtCurrentField();

					for(uint8_t i = 0; i < numberOfKits; i++){

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
				// Serial.println(" OLD VICTIM");
			}
			
			if(!outOfField){
				mapOnlyMoveTo(FRONT);
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
			// stabilize und dann neu entscheiden
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
			//kurz zurück fahren
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
			//kurz zurück fahren
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
			// ramp down
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
			if( sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)// || visVictim != ' ' )
			{
				lastState = state;
				state = 6;
			}
			break;
		}
		case 12:
		{
			// ramp up
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
			if( sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)// || visVictim != ' ' )
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
