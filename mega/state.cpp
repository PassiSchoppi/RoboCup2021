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
int timeSpentInOneStateB = 0;
int timeSpentInOneState = 0;
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
bool obstacleSum = false;

uint8_t nothing()
{
	return 0;
}

void stateChange()
{
	/*Serial.print("A: ");
	Serial.print(timeSpentInOneState);
	Serial.print(" B: ");
	Serial.println(timeSpentInOneStateB);*/
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
		if(SEND){Serial.println("pausing");}
		motorBrake();
		mapSetBackToLastSilver();
		state = 8;
		timeSpentInOneState = 0;
		timeSpentInOneStateB = 0;
		return;
	}
	if(sensorData[17] == 0)
	{
		motorBrake();
		motorResetAllSteps();
		if(SEND){Serial.println("resetting");}
		mapClear();
		state = 8;
		timeSpentInOneState = 0;
		timeSpentInOneStateB = 0;
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
			timeSpentInOneState = 0;
			timeSpentInOneStateB = 0;
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
			++timeSpentInOneState;
			if(timeSpentInOneState > 32750){
				timeSpentInOneState = 0;
				++timeSpentInOneStateB;
			}
			outOfField = false;
			visVictim = 'e';
			obstacleSum = false;
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
					timeSpentInOneState = 0;
					timeSpentInOneStateB = 0;
					state = 2;
					// mapMoveTo(RIGHT);
					// Serial.println("checngin state to: RIGHT");
					return;
				}
				if(!wallExists(FRONT) && !frontIsBlack)
				{
					// gerade aus
					// Serial.println("Gerade aus!");
					timeSpentInOneState = 0;
					timeSpentInOneStateB = 0;
					state = 3;
					// mapMoveTo(FRONT);
					// Serial.println("checngin state to: FRONT");
					return;
				}
				if(!wallExists(LEFT))
				{
					// links drehen dann gerade aus
					// Serial.println("Links abbiegen!");
					timeSpentInOneState = 0;
					timeSpentInOneStateB = 0;
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
					timeSpentInOneState = 0;
					timeSpentInOneStateB = 0;
					state = 5;
					// mapMoveTo(BACK);
					// Serial.println("checngin state to: BACK");
					frontIsBlack = false;
					return;
				}

				// wenn überall Wände sind:::
				state = 1;
				timeSpentInOneState = 0;
				timeSpentInOneStateB = 0;
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
				if(SEND){Serial.print("!!! going to: ");
				Serial.println(compasToGoTo);}
				switch ( mapCompasToDirection( compasToGoTo ) ) {
					case FRONT:
					{
						state = 3;
						timeSpentInOneState = 0;
						timeSpentInOneStateB = 0;
						break;
					}
					case RIGHT:
					{
						state = 2;
						timeSpentInOneState = 0;
						timeSpentInOneStateB = 0;
						break;
					}
					case BACK:
					{
						state = 5;
						timeSpentInOneState = 0;
						timeSpentInOneStateB = 0;
						break;
					}
					case LEFT:
					{
						state = 4;
						timeSpentInOneState = 0;
						timeSpentInOneStateB = 0;
						break;
					}
					case 5:
					{
						if(SEND){Serial.println("ARE WE HOME JET????");}
						mapReturnToHome();
						if(SEND){Serial.println("SOON!!!!");}
						state = 1;
						timeSpentInOneState = 0;
						timeSpentInOneStateB = 0;
						break;
					}
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
			++timeSpentInOneState;
			if(timeSpentInOneState > 32750){
				timeSpentInOneState = 0;
				++timeSpentInOneStateB;
			}
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
				LEDSetColor(RED);
				delay(100);
				mapOnlyTurnTo(RIGHT);
			}
			if( average > STEPFFORRIGHT || timeSpentInOneState > MAXTIMEINSTATE )
			{
				motorResetAllSteps();
				motorBrake();
				motorBrake();
				stabilize();
				if(wallExists(LEFT) || wallExists(RIGHT)){
					visVictim = raspiRead();
					if(visVictim != 'e' && !mapVictimIsAtCurrentField())
					{
						lastState = 3;
						state = 6;
						timeSpentInOneState = 0;
						timeSpentInOneStateB = 0;
						return;
					}
				}
				// gerade aus
				timeSpentInOneState = 0;
				timeSpentInOneStateB = 0;
				state = 3;
			}
			// wenn da ist ein Victim
			if( sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)
			{
				lastState = state;
				state = 6;
				timeSpentInOneState = 0;
				timeSpentInOneStateB = 0;
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
			++timeSpentInOneState;
			if(timeSpentInOneState > 32750){
				timeSpentInOneState = 0;
				++timeSpentInOneStateB;
			}
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
			if(obstacleSum){
				average -= 25;
			}
			if( average>STEPSFORONE || (sensorData[6]>PERFECTDISTTOW-plusToDis && sensorData[7]>PERFECTDISTTOW-plusToDis && sensorData[8]>PERFECTDISTTOW-plusToDis) )
			{
				motorBrake();
				motorResetAllSteps();
				// stabilize und dann neue entscheidung
				state = 8;
				timeSpentInOneState = 0;
				timeSpentInOneStateB = 0;
				break;
			}

			int moreDistanceToWall;
			moreDistanceToWall = 0;

			if(!obstacleSum && sensorData[6]<DISTANCETOWALL-moreDistanceToWall && sensorData[7]<DISTANCETOWALL-moreDistanceToWall && sensorData[8]>DISTANCETOWALL+moreDistanceToWall)
			{
				obstacleSum = true;
			}
			// wenn nur vorne links ein obstacle
			if(!obstacleSum && sensorData[6]>DISTANCETOWALL+moreDistanceToWall && sensorData[7]<DISTANCETOWALL-moreDistanceToWall && sensorData[8]<DISTANCETOWALL-moreDistanceToWall)
			{
				obstacleSum = true;
			}
						
			// wenn zu nah an einer Wand oder obstacle
			if( sensorData[7]>120 || timeSpentInOneState > MAXTIMEINSTATE )
			{
				// kurz zurück und dann neu entscheiden
				motorBrake();
				state = 9;
				timeSpentInOneState = 0;
				timeSpentInOneStateB = 0;
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
					timeSpentInOneState = 0;
					timeSpentInOneStateB = 0;
					break;
				}

				// wenn on ramp up
				if( sensorData[4] > level + RAMP_THRESHOLD)
				{
					// motorResetAllSteps();
					overHalfOfRamp = false;

					// ramp up
					state = 12;
					timeSpentInOneState = 0;
					timeSpentInOneStateB = 0;
					break;
				}
			}
			
			// wenn Victim
			if(sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)
			{
				if(SEND){Serial.println("maybe victim ?");}
				lastState = state;
				state = 6;
				timeSpentInOneState = 0;
				timeSpentInOneStateB = 0;
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
				timeSpentInOneState = 0;
				timeSpentInOneStateB = 0;
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
			++timeSpentInOneState;
			if(timeSpentInOneState > 32750){
				timeSpentInOneState = 0;
				++timeSpentInOneStateB;
			}
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
				LEDSetColor(RED);
				delay(100);
				mapOnlyTurnTo(LEFT);
			}
			if(average > STEPSFORLEFT || timeSpentInOneState > MAXTIMEINSTATE )
			{
				motorResetAllSteps();
				stabilize();
				motorBrake();
				overHalfOfTurn = false;
				if(wallExists(LEFT) || wallExists(RIGHT)){
					visVictim = raspiRead();
					if(visVictim != 'e' && !mapVictimIsAtCurrentField())
					{
						lastState = 3;
						state = 6;
						timeSpentInOneState = 0;
						timeSpentInOneStateB = 0;
						return;
					}
				}
				// gerade aus
				timeSpentInOneState = 0;
				timeSpentInOneStateB = 0;
				state = 3;
			}
			// wen victim
			if( sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)
			{
				lastState = state;
				state = 6;
				timeSpentInOneState = 0;
				timeSpentInOneStateB = 0;
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
			++timeSpentInOneState;
			if(timeSpentInOneState > 32750){
				timeSpentInOneState = 0;
				++timeSpentInOneStateB;
			}
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
				LEDSetColor(RED);
				delay(100);
				mapOnlyTurnTo(LEFT);
			}
			if(average > STEPSFORLEFT || timeSpentInOneState > MAXTIMEINSTATE )
			{
				motorResetAllSteps();
				stabilize();
				motorBrake();
				overHalfOfTurn = false;
				if(wallExists(LEFT) || wallExists(RIGHT)){
					visVictim = raspiRead();
					if(visVictim != 'e' && !mapVictimIsAtCurrentField())
					{
						lastState = 4;
						state = 6;
						timeSpentInOneState = 0;
						timeSpentInOneStateB = 0;
						return;
					}
				}
				// dann left und dann gerade aus
				state = 4;
				timeSpentInOneState = 0;
				timeSpentInOneStateB = 0;
			}
			// wenn victim
			if( sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)
			{
				lastState = state;
				state = 6;
				timeSpentInOneState = 0;
				timeSpentInOneStateB = 0;
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
				if(SEND){Serial.println(" NEW VICTIM");}
				// zweites mal testen und seite herausfinden
				if(visVictim == 'e'){
					
					if ( sensorData[11]>VICTIMTEMP )
					{
						victimIsLeftNotRight = true;
					}else if (sensorData[12]>VICTIMTEMP)
					{
						victimIsLeftNotRight = false;
					}else{
						if(SEND){Serial.println("DIDNT FOUND SECOND TEST");
						Serial.println("RETURNING TO LAST STATE");}
						state = lastState;
						timeSpentInOneState = 0;
						timeSpentInOneStateB = 0;
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
						if(SEND){Serial.println("NONE OF THE LETTERS WERE GIVEN");
						Serial.println("RETURNING TO LAST STATE. The letter was:");
						Serial.println(visVictim);}
						state = lastState;
						timeSpentInOneState = 0;

						timeSpentInOneStateB = 0;
						break;
					}
					
				}

				if((victimIsLeftNotRight && !wallExists(LEFT)) ||
				   (!victimIsLeftNotRight && !wallExists(RIGHT)))
				{
					if(SEND){Serial.println("victim not on a wall");}
					state = lastState;
					timeSpentInOneState = 0;

					timeSpentInOneStateB = 0;
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
							if(SEND){Serial.println("SOMETHING WENT WRONG");}
							state = lastState;
							timeSpentInOneState = 0;

							timeSpentInOneStateB = 0;
							return;
						}
						
					}

					// mark current field as victim
					mapVictimNewAtCurrentField();

					for(uint8_t i = 0; i < numberOfKits; i++){
						if(SEND){Serial.println("KIT");}

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
				if(SEND){Serial.println(" OLD VICTIM");
				Serial.println("returning to last state:");
				Serial.println(lastState);}
				state = lastState;
				timeSpentInOneState = 0;

				timeSpentInOneStateB = 0;
			}
			
			if(SEND){Serial.print("RETURNING TO LAST STATE: ");Serial.println(lastState);}
			// zurück zu dem was er gerade gemacht hat
			state = lastState;
			timeSpentInOneState = 0;

			timeSpentInOneStateB = 0;
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
			++timeSpentInOneState;
			if(timeSpentInOneState > 32750){
				timeSpentInOneState = 0;

				++timeSpentInOneStateB;
			}
			// Serial.println("stabalizing");
			LEDSetColor(PINK);
			motorBrake();
			stabilize();
			motorBrake();
			motorResetAllSteps();
			if(wallExists(LEFT) || wallExists(RIGHT)){
				visVictim = raspiRead();
				if(visVictim != 'e' && !mapVictimIsAtCurrentField())
				{
					lastState = 1;
					state = 6;
					timeSpentInOneState = 0;

					timeSpentInOneStateB = 0;
					return;
				}
			}
			timeSpentInOneState = 0;

			timeSpentInOneStateB = 0;
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
			while(motorStepsMade(0)<15 && timeSpentInOneState < MAXTIMEINSTATE ){
				++timeSpentInOneState;
				if(timeSpentInOneState > 32750){
					timeSpentInOneState = 0;

					++timeSpentInOneStateB;
				}
			}
			motorResetAllSteps();
			motorBrake();
			// stabilize und dann neu entscheiden
			state = 8;
			timeSpentInOneState = 0;

			timeSpentInOneStateB = 0;
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
			while(motorStepsMade(0) < (numberOfStepsBeforBlack * 2) && timeSpentInOneStateB < MAXTIMEINSTATE ){
				++timeSpentInOneState;
				if(timeSpentInOneState > 32750){
					timeSpentInOneState = 0;

					++timeSpentInOneStateB;
				}
			}
			numberOfStepsBeforBlack = 0;
			motorResetAllSteps();
			motorBrake();
			// stabilize und dann neu entscheiden
			state = 8;
			timeSpentInOneState = 0;
			timeSpentInOneStateB = 0;
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
			++timeSpentInOneState;
			if(timeSpentInOneState > 32750){
				timeSpentInOneState = 0;

				++timeSpentInOneStateB;
			}
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
				timeSpentInOneState = 0;

				timeSpentInOneStateB = 0;
				// kurz vorwärts fahren dann neu entscheiden
				state = 13;
				if(!overHalfOfRamp){
					state = 3;
				}
			}

			// wenn victim
			if( sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)
			{
				lastState = state;
				state = 6;
				timeSpentInOneState = 0;

				timeSpentInOneStateB = 0;
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
			++timeSpentInOneState;
			if(timeSpentInOneState > 32750){
				timeSpentInOneState = 0;

				++timeSpentInOneStateB;
			}
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
				timeSpentInOneState = 0;

				timeSpentInOneStateB = 0;
				// kurz vorwärts fahren dann neu entscheiden
				state = 13;
				if(!overHalfOfRamp){
					state = 3;
				}
			}

			// wenn victim
			if( sensorData[11]>VICTIMTEMP || sensorData[12]>VICTIMTEMP)
			{
				timeSpentInOneState = 0;

				timeSpentInOneStateB = 0;
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
			while(motorStepsMade(0)<REST_OF_RAMP && timeSpentInOneState < MAXTIMEINSTATE ){
				++timeSpentInOneState;
				if(timeSpentInOneState > 32750){
					timeSpentInOneState = 0;
					++timeSpentInOneStateB;
				}
			}
			motorResetAllSteps();
			motorBrake();
			timeSpentInOneState = 0;

			timeSpentInOneStateB = 0;
			// stabilize und dann neu entscheiden
			state = 8;
			break;
		}
	}
}
