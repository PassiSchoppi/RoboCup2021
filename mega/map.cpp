#include "map.h"


// ascii art at: https://textfancy.com/ascii-art/


// 2D Array of Fields
Field Map[NUMBEROFSTORIES][MAPSIZE][MAPSIZE];

// Variables of Robot
uint8_t robot_is_facing;
uint8_t robot_is_on_story;
Vector robot_is_at;
Vector lastVisitedSilver;

/*
 ▄▄▄▄▄▄  ▄▄▄ ▄▄▄▄▄▄   ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄ ▄▄▄▄▄▄▄ ▄▄    ▄    ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄    ▄ ▄▄   ▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄   ▄▄▄▄▄▄▄ ▄▄▄ ▄▄▄▄▄▄▄ ▄▄    ▄ 
█      ██   █   ▄  █ █       █       █       █   █       █  █  █ █  █       █       █  █  █ █  █ █  █       █   ▄  █ █       █   █       █  █  █ █
█  ▄    █   █  █ █ █ █    ▄▄▄█       █▄     ▄█   █   ▄   █   █▄█ █  █       █   ▄   █   █▄█ █  █▄█  █    ▄▄▄█  █ █ █ █  ▄▄▄▄▄█   █   ▄   █   █▄█ █
█ █ █   █   █   █▄▄█▄█   █▄▄▄█     ▄▄█ █   █ █   █  █ █  █       █  █     ▄▄█  █ █  █       █       █   █▄▄▄█   █▄▄█▄█ █▄▄▄▄▄█   █  █ █  █       █
█ █▄█   █   █    ▄▄  █    ▄▄▄█    █    █   █ █   █  █▄█  █  ▄    █  █    █  █  █▄█  █  ▄    █       █    ▄▄▄█    ▄▄  █▄▄▄▄▄  █   █  █▄█  █  ▄    █
█       █   █   █  █ █   █▄▄▄█    █▄▄  █   █ █   █       █ █ █   █  █    █▄▄█       █ █ █   ██     ██   █▄▄▄█   █  █ █▄▄▄▄▄█ █   █       █ █ █   █
█▄▄▄▄▄▄██▄▄▄█▄▄▄█  █▄█▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█ █▄▄▄█ █▄▄▄█▄▄▄▄▄▄▄█▄█  █▄▄█  █▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█▄█  █▄▄█ █▄▄▄█ █▄▄▄▄▄▄▄█▄▄▄█  █▄█▄▄▄▄▄▄▄█▄▄▄█▄▄▄▄▄▄▄█▄█  █▄▄█
*/
// translates compas to direction
uint8_t mapCompasToDirection(uint8_t compasI)
{
	switch(robot_is_facing)
	{
		// robot is facing NOTH
		case NOTH:
			return(compasI);
			break;
		
		// robot is facing EAST 
		case EAST:
			switch(compasI){
				case NOTH:
					return(LEFT);
					break;
				case EAST:
					return(FRONT);
					break;
				case SOUTH:
					return(RIGHT);
					break;
				case WEST:
					return(BACK);
					break;
			}
			break;
		
		// robot is facing SOUTH
		case SOUTH:
			switch(compasI){
				case NOTH:
					return(BACK);
					break;
				case EAST:
					return(LEFT);
					break;
				case SOUTH:
					return(FRONT);
					break;
				case WEST:
					return(RIGHT);
					break;
			}
			break;
				
		// robot is facing WEST
		case WEST:
			switch(compasI){
				case NOTH:
					return(RIGHT);
					break;
				case EAST:
					return(BACK);
					break;
				case SOUTH:
					return(LEFT);
					break;
				case WEST:
					return(FRONT);
					break;
			}
			break;
	}
}

// translates direction to compas
uint8_t mapDirectionToCompas(uint8_t directionI)
{
	switch(robot_is_facing)
	{
		// robot is facing NOTH
		case NOTH:
			return(directionI);
			break;
		
		// robot is facing EAST 
		case EAST:
			switch(directionI){
				case FRONT:
					return(EAST);
					break;
				case RIGHT:
					return(SOUTH);
					break;
				case BACK:
					return(WEST);
					break;
				case LEFT:
					return(NOTH);
					break;
			}
			break;
		
		// robot is facing SOUTH
		case SOUTH:
			switch(directionI){
				case FRONT:
					return(SOUTH);
					break;
				case RIGHT:
					return(WEST);
					break;
				case BACK:
					return(NOTH);
					break;
				case LEFT:
					return(EAST);
					break;
			}
			break;
				
		// robot is facing WEST
		case WEST:
			switch(directionI){
				case FRONT:
					return(WEST);
					break;
				case RIGHT:
					return(NOTH);
					break;
				case BACK:
					return(EAST);
					break;
				case LEFT:
					return(SOUTH);
					break;
			}
			break;
	}
}

/*
 ▄▄▄ ▄▄    ▄ ▄▄▄ ▄▄▄▄▄▄▄ 
█   █  █  █ █   █       █
█   █   █▄█ █   █▄     ▄█
█   █       █   █ █   █  
█   █  ▄    █   █ █   █  
█   █ █ █   █   █ █   █  
█▄▄▄█▄█  █▄▄█▄▄▄█ █▄▄▄█  
*/
void mapInit() 
{
	robot_is_facing = NOTH;
	robot_is_at.X = STARTX;
	robot_is_at.Y = STARTY;
	lastVisitedSilver.X = robot_is_at.X;
	lastVisitedSilver.Y = robot_is_at.Y;
	robot_is_on_story = 0;
}

/*
 ▄▄▄▄▄▄▄ ▄▄▄     ▄▄▄▄▄▄▄ ▄▄▄▄▄▄ ▄▄▄▄▄▄   
█       █   █   █       █      █   ▄  █  
█       █   █   █    ▄▄▄█  ▄   █  █ █ █  
█     ▄▄█   █   █   █▄▄▄█ █▄█  █   █▄▄█▄ 
█    █  █   █▄▄▄█    ▄▄▄█      █    ▄▄  █
█    █▄▄█       █   █▄▄▄█  ▄   █   █  █ █
█▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█▄█ █▄▄█▄▄▄█  █▄█
*/
// clears every information of the map
// and puts robot back to start
// FIXME needs to be testet
void mapClear()
{
	lastVisitedSilver.X = STARTX;
	lastVisitedSilver.Y = STARTY;
	robot_is_at.X = STARTX;
	robot_is_at.Y = STARTY;
	robot_is_facing = NOTH;
	for ( uint8_t s=0; s<NUMBEROFSTORIES; ++s ){
		for ( uint8_t i=0; i<MAPSIZE; ++i ){
			for ( uint8_t o=0; o<MAPSIZE; ++o ){
				Map[ s ][ i ][ o ].visited = false;
				Map[ s ][ i ][ o ].isBlack = false;
				Map[ s ][ i ][ o ].hasVictim = false;
				Map[ s ][ i ][ o ].isRamp = false;
				Map[ s ][ i ][ o ].distanceToUnvisited = 0;
				Map[ s ][ i ][ o ].directions[0] = 1;
				Map[ s ][ i ][ o ].directions[1] = 1;
				Map[ s ][ i ][ o ].directions[2] = 1;
				Map[ s ][ i ][ o ].directions[3] = 1;
			}
		}
	}
}

/*
▄▄▄▄▄▄▄ ▄▄▄ ▄▄▄     ▄▄   ▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄      ▄▄▄▄▄▄▄ ▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄     ▄▄▄▄▄▄  
█       █   █   █   █  █ █  █       █   ▄  █    █       █   █       █   █   █      █ 
█  ▄▄▄▄▄█   █   █   █  █▄█  █    ▄▄▄█  █ █ █    █    ▄▄▄█   █    ▄▄▄█   █   █  ▄    █
█ █▄▄▄▄▄█   █   █   █       █   █▄▄▄█   █▄▄█▄   █   █▄▄▄█   █   █▄▄▄█   █   █ █ █   █
█▄▄▄▄▄  █   █   █▄▄▄█       █    ▄▄▄█    ▄▄  █  █    ▄▄▄█   █    ▄▄▄█   █▄▄▄█ █▄█   █
▄▄▄▄▄█  █   █       ██     ██   █▄▄▄█   █  █ █  █   █   █   █   █▄▄▄█       █       █
█▄▄▄▄▄▄▄█▄▄▄█▄▄▄▄▄▄▄█ █▄▄▄█ █▄▄▄▄▄▄▄█▄▄▄█  █▄█  █▄▄▄█   █▄▄▄█▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█▄▄▄▄▄▄█ 
*/
// sets current field as the last visited silver field
void mapSilverField()
{
	lastVisitedSilver.X = robot_is_at.X;
	lastVisitedSilver.Y = robot_is_at.Y;
}

// puts the robot back to last visited silver
void mapSetBackToLastSilver()
{
	robot_is_at.X = lastVisitedSilver.X;
	robot_is_at.Y = lastVisitedSilver.Y;
	robot_is_facing = NOTH;
}

/*
 ▄▄   ▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄  ▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄    ▄▄▄▄▄▄▄ ▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄     ▄▄▄▄▄▄  
█  █ █  █       █      ██      █       █       █  █       █   █       █   █   █      █ 
█  █ █  █    ▄  █  ▄    █  ▄   █▄     ▄█    ▄▄▄█  █    ▄▄▄█   █    ▄▄▄█   █   █  ▄    █
█  █▄█  █   █▄█ █ █ █   █ █▄█  █ █   █ █   █▄▄▄   █   █▄▄▄█   █   █▄▄▄█   █   █ █ █   █
█       █    ▄▄▄█ █▄█   █      █ █   █ █    ▄▄▄█  █    ▄▄▄█   █    ▄▄▄█   █▄▄▄█ █▄█   █
█       █   █   █       █  ▄   █ █   █ █   █▄▄▄   █   █   █   █   █▄▄▄█       █       █
█▄▄▄▄▄▄▄█▄▄▄█   █▄▄▄▄▄▄██▄█ █▄▄█ █▄▄▄█ █▄▄▄▄▄▄▄█  █▄▄▄█   █▄▄▄█▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█▄▄▄▄▄▄█ 
*/
void mapUpdateField()
{
	// Serial.println("updating field");

	// add walls to current field
	//  etage              X              Y                          compas                            wand
	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[ mapDirectionToCompas( FRONT ) ] = wallExists(FRONT);
	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[ mapDirectionToCompas( RIGHT ) ] = wallExists(RIGHT);
	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[ mapDirectionToCompas( BACK ) ] = 	wallExists(BACK);
	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[ mapDirectionToCompas( LEFT ) ] = 	wallExists(LEFT);

	/*Serial.println("new walls:");
	for ( uint8_t i=0; i<4; ++i ){
		Serial.println(Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[i]);
	}*/
	
	// change walls of sourounding fields
	//					 wand eines anliegenden Feldes                        =                     wand welche an dieses Feld angrenztz
	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y-1].directions[ SOUTH ] = 	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[NOTH];
	Map[robot_is_on_story][robot_is_at.X+1][robot_is_at.Y].directions[ WEST ] = 	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[EAST];
	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y+1].directions[ NOTH ] = 	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[SOUTH];
	Map[robot_is_on_story][robot_is_at.X-1][robot_is_at.Y].directions[ EAST ] = 	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[WEST];

	// silver field
	if( sensorData[13]<MINWHITE || sensorData[14]<MINWHITE )
	{
		if(SEND){Serial.print("detected silver tile... robot is at: ");
		Serial.print(robot_is_at.X);Serial.print(" | ");
		Serial.println(robot_is_at.Y);}

		// zeigt an ob dieses Felb bereits als Siber markiert wurde
		if((robot_is_at.X != lastVisitedSilver.X) || (robot_is_at.Y != lastVisitedSilver.Y)){
			LEDSetColor(WHITE);
			delay(100);
			LEDSetColor(OFF);
			delay(100);
			LEDSetColor(WHITE);
			delay(100);
			LEDSetColor(OFF);
			delay(100);
			LEDSetColor(WHITE);
			delay(100);
			LEDSetColor(OFF);
			delay(100);
			LEDSetColor(WHITE);
			delay(100);
			LEDSetColor(OFF);
			delay(100);
			mapSilverField();
		}
		else
		{
			LEDSetColor(BLUE);
			delay(100);
			LEDSetColor(OFF);
			delay(100);
			LEDSetColor(BLUE);
			delay(100);
			LEDSetColor(OFF);
			delay(100);
			LEDSetColor(BLUE);
			delay(100);
			LEDSetColor(OFF);
			delay(100);
			LEDSetColor(BLUE);
			delay(100);
			LEDSetColor(OFF);
			delay(100);
		}
	}

	// mark current field as visited
	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].visited = true;

	// clear distances from the map
	for ( uint8_t s=0; s<NUMBEROFSTORIES; ++s ){
		for ( uint8_t i=0; i<MAPSIZE; ++i ){
			for ( uint8_t o=0; o<MAPSIZE; ++o ){
				if (Map[ s ][ i ][ o ].visited == false)
				{
					Map[ s ][ i ][ o ].distanceToUnvisited = 1;
				} else {
					Map[ s ][ i ][ o ].distanceToUnvisited = 0;
				}
			}
		}
	}

	// update distance to nearest unvisited
	for ( uint8_t s=0; s<NUMBEROFSTORIES; ++s ){
		for ( uint8_t i=0; i<MAPSIZE; ++i ){
			for ( uint8_t o=0; o<MAPSIZE; ++o ){
				if (Map[s][ i ][ o ].visited == false)
				{
					calcDistanceRecursively(s, i, o, 1);
				}

				/*Vector test;
				test.X = i;
				test.Y = o;
				Vector skipMe[LENGTHOFSKIP];
				Serial.print("Field: ");Serial.print(i);Serial.print(", ");Serial.println(o);
				Map[i][o].distanceToUnvisited = mapSearchForUnvisited( test, skipMe);*/
			}
		}
	}
}

/*
 ▄▄   ▄▄ ▄▄▄▄▄▄▄ ▄▄   ▄▄ ▄▄▄ ▄▄    ▄ ▄▄▄▄▄▄▄ 
█  █▄█  █       █  █ █  █   █  █  █ █       █
█       █   ▄   █  █▄█  █   █   █▄█ █   ▄▄▄▄█
█       █  █ █  █       █   █       █  █  ▄▄ 
█       █  █▄█  █       █   █  ▄    █  █ █  █
█ ██▄██ █       ██     ██   █ █ █   █  █▄▄█ █
█▄█   █▄█▄▄▄▄▄▄▄█ █▄▄▄█ █▄▄▄█▄█  █▄▄█▄▄▄▄▄▄▄█
*/
void mapMoveTo(uint8_t directionToGo)
{
	if(SEND){Serial.print("Moving to compas: ");
	Serial.println(mapDirectionToCompas( directionToGo ));}
	switch( mapDirectionToCompas( directionToGo ) ) {
		case NOTH:
			robot_is_at.Y -= 1;
			robot_is_facing = NOTH;
			break;
		case EAST:
			robot_is_at.X += 1;
			robot_is_facing = EAST;
			break;
		case SOUTH:
			robot_is_at.Y += 1;
			robot_is_facing = SOUTH;
			break;
		case WEST:
			robot_is_at.X -= 1;
			robot_is_facing = WEST;
			break;
	}
}


void mapOnlyMoveTo(uint8_t directionToGo)
{
	if(SEND){Serial.print("Force Moving to compas: ");
	Serial.println(mapDirectionToCompas( directionToGo ));}
	switch( mapDirectionToCompas( directionToGo ) ) {
		case NOTH:
			robot_is_at.Y -= 1;
			break;
		case EAST:
			robot_is_at.X += 1;
			break;
		case SOUTH:
			robot_is_at.Y += 1;
			break;
		case WEST:
			robot_is_at.X -= 1;
			break;
	}
}


void mapOnlyTurnTo(uint8_t directionToGo)
{
	if(SEND){Serial.print("Force Turning to compas: ");
	Serial.println(mapDirectionToCompas( directionToGo ));}
	robot_is_facing = mapDirectionToCompas( directionToGo );
}

/*
 ▄▄▄▄▄▄▄ ▄▄▄     ▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄   ▄    ▄▄▄▄▄▄▄ ▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄     ▄▄▄▄▄▄  
█  ▄    █   █   █      █       █   █ █ █  █       █   █       █   █   █      █ 
█ █▄█   █   █   █  ▄   █       █   █▄█ █  █    ▄▄▄█   █    ▄▄▄█   █   █  ▄    █
█       █   █   █ █▄█  █     ▄▄█      ▄█  █   █▄▄▄█   █   █▄▄▄█   █   █ █ █   █
█  ▄   ██   █▄▄▄█      █    █  █     █▄   █    ▄▄▄█   █    ▄▄▄█   █▄▄▄█ █▄█   █
█ █▄█   █       █  ▄   █    █▄▄█    ▄  █  █   █   █   █   █▄▄▄█       █       █
█▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█▄█ █▄▄█▄▄▄▄▄▄▄█▄▄▄█ █▄█  █▄▄▄█   █▄▄▄█▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█▄▄▄▄▄▄█ 
*/
void mapBlackFieldFront()
{
	if(SEND){Serial.println("found black while at:");
	Serial.println( robot_is_at.X);
	Serial.println( robot_is_at.Y);}
	mapOnlyMoveTo(FRONT);
	if(SEND){Serial.println("the black field is at:");
	Serial.println( robot_is_at.X);
	Serial.println( robot_is_at.Y);}
	Map[robot_is_on_story][ robot_is_at.X ][ robot_is_at.Y ].visited = true;
	Map[robot_is_on_story][ robot_is_at.X ][ robot_is_at.Y ].isBlack = true;
	mapOnlyMoveTo(BACK);
	if(SEND){Serial.println("done");}
}

void mapBlackFieldCurrent()
{
	if(SEND){Serial.println("found black at:");
	Serial.println( robot_is_at.X);
	Serial.println( robot_is_at.Y);
	Serial.println( robot_is_facing);}
	Map[robot_is_on_story][ robot_is_at.X ][ robot_is_at.Y ].visited = true;
	Map[robot_is_on_story][ robot_is_at.X ][ robot_is_at.Y ].isBlack = true;
	
	mapOnlyMoveTo(BACK);

	/*switch( robot_is_facing ) {
		case NOTH:
			Serial.println("Black at North");
			robot_is_at.Y += 1;
			break;
		case EAST:
			Serial.println("Black at East");
			robot_is_at.X -= 1;
			break;
		case SOUTH:
			Serial.println("Black at South");
			robot_is_at.Y -= 1;
			break;
		case WEST:
			Serial.println("Black at West");
			robot_is_at.X += 1;
			break;
	}*/
	if(SEND){Serial.println("moving...now at:");
	Serial.println( robot_is_at.X);
	Serial.println( robot_is_at.Y);
	Serial.println( robot_is_facing);
	Serial.println("done");}
}

/*
 ▄▄   ▄▄ ▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄ ▄▄   ▄▄ 
█  █ █  █   █       █       █   █  █▄█  █
█  █▄█  █   █       █▄     ▄█   █       █
█       █   █     ▄▄█ █   █ █   █       █
█       █   █    █    █   █ █   █       █
 █     ██   █    █▄▄  █   █ █   █ ██▄██ █
  █▄▄▄█ █▄▄▄█▄▄▄▄▄▄▄█ █▄▄▄█ █▄▄▄█▄█   █▄█
*/
void mapVictimNewAtCurrentField()
{
	if(SEND){Serial.println("new victim at:");
	Serial.println(robot_is_at.X);
	Serial.println(robot_is_at.Y);}
	Map[robot_is_on_story][ robot_is_at.X ][ robot_is_at.Y ].hasVictim = true;
}

bool mapVictimIsAtCurrentField()
{
	if(SEND){Serial.println("robot is at:");
	Serial.println(robot_is_at.X);
	Serial.println(robot_is_at.Y);
	Serial.print("Field has victim: ");
	Serial.println(Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].hasVictim);}
	return( Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].hasVictim );
}

/*
 ▄▄▄▄▄▄  ▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄ ▄▄    ▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ 
█      ██   █       █       █      █  █  █ █       █       █       █
█  ▄    █   █  ▄▄▄▄▄█▄     ▄█  ▄   █   █▄█ █       █    ▄▄▄█  ▄▄▄▄▄█
█ █ █   █   █ █▄▄▄▄▄  █   █ █ █▄█  █       █     ▄▄█   █▄▄▄█ █▄▄▄▄▄ 
█ █▄█   █   █▄▄▄▄▄  █ █   █ █      █  ▄    █    █  █    ▄▄▄█▄▄▄▄▄  █
█       █   █▄▄▄▄▄█ █ █   █ █  ▄   █ █ █   █    █▄▄█   █▄▄▄ ▄▄▄▄▄█ █
█▄▄▄▄▄▄██▄▄▄█▄▄▄▄▄▄▄█ █▄▄▄█ █▄█ █▄▄█▄█  █▄▄█▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█
*/
void calcDistanceRecursively(uint8_t s, uint8_t x, uint8_t y, uint8_t num)
{
	/*Serial.print("recursion call:");
	Serial.print(s);
	Serial.print(x);
	Serial.print(y);
	Serial.println(num);*/

	// declarations
	Vector thisRamp;
	Vector storiesThatMatch;

	Map[ s ][ x ][ y ].distanceToUnvisited = num;

	// north field
	if(y < MAPSIZE - 1)
	{
		if(!Map[ s ][ x ][ y ].directions[SOUTH])
		{
			if(!Map[ s ][ x ][ y + 1 ].isBlack)
			{
				if(Map[ s ][ x ][ y + 1 ].distanceToUnvisited == 0 or
				Map[ s ][ x ][ y + 1 ].distanceToUnvisited >= num + 1)
				{
					if(Map[ s ][ x ][ y + 1 ].isRamp)
					{
						// Serial.println("ramp stuff");
						thisRamp.X = x;
						thisRamp.Y = y + 1;
						storiesThatMatch = mapStoriesThatConnectAt(thisRamp);

						if( storiesThatMatch.X == s && storiesThatMatch.Y != s )
						{
							Map[ storiesThatMatch.X ][ x ][ y + 1 ].distanceToUnvisited = num + 1;
							calcDistanceRecursively(storiesThatMatch.Y, x, y + 1, num + 2);
						}
						else if ( storiesThatMatch.Y == s && storiesThatMatch.X != s )
						{
							Map[ storiesThatMatch.Y ][ x ][ y + 1 ].distanceToUnvisited = num + 1;
							calcDistanceRecursively(storiesThatMatch.X, x, y + 1, num + 2);
						}else{
							if(SEND){Serial.println("something went very wrong...");}
						}
					}else{
						//Serial.println("distance is not 0 and not bigger than num+1 ::: north");
						calcDistanceRecursively(s, x, y + 1, num + 1);
					}
				}else{
					/*Serial.println(x);
					Serial.println(y+1);
					Serial.println("should work... north");
					Serial.println(Map[ s ][ x ][ y + 1 ].distanceToUnvisited);*/
				}
			}else{
				// Serial.println("tile north is black");
			}
		}else{
			// Serial.println("there is a wall north");
		}
	}

	// east field
	if(x < MAPSIZE - 1)
	{
		if(!Map[ s ][ x ][ y ].directions[EAST])
		{
			if(!Map[ s ][ x + 1 ][ y ].isBlack)
			{
				if(Map[ s ][ x + 1 ][ y ].distanceToUnvisited == 0 or
				Map[ s ][ x + 1 ][ y ].distanceToUnvisited >= num + 1)
				{
					if(Map[ s ][ x + 1 ][ y ].isRamp)
					{
						thisRamp.X = x + 1;
						thisRamp.Y = y;
						storiesThatMatch = mapStoriesThatConnectAt(thisRamp);

						if( storiesThatMatch.X == s && storiesThatMatch.Y != s )
						{
							Map[ storiesThatMatch.X ][ x + 1 ][ y ].distanceToUnvisited = num + 1;
							calcDistanceRecursively(storiesThatMatch.Y, x + 1, y, num + 2);
						}
						else if ( storiesThatMatch.Y == s && storiesThatMatch.X != s )
						{
							Map[ storiesThatMatch.Y ][ x + 1 ][ y ].distanceToUnvisited = num + 1;
							calcDistanceRecursively(storiesThatMatch.X, x + 1, y, num + 2);
						}else{
							if(SEND){Serial.println("something went very wrong...");}
						}
					}else{
						calcDistanceRecursively(s, x + 1, y, num + 1);
					}
				}
			}
		}
	}

	// south field
	if(y > 1)
	{
		if(!Map[ s ][ x ][ y ].directions[NOTH])
		{
			if(!Map[ s ][ x ][ y - 1 ].isBlack)
			{
				if(Map[ s ][ x ][ y - 1 ].distanceToUnvisited == 0 or
				Map[ s ][ x ][ y - 1 ].distanceToUnvisited >= num + 1)
				{
					if(Map[ s ][ x ][ y - 1 ].isRamp)
					{
						// Serial.print("ramp stuff");
						thisRamp.X = x;
						thisRamp.Y = y - 1;
						storiesThatMatch = mapStoriesThatConnectAt(thisRamp);

						if( storiesThatMatch.X == s && storiesThatMatch.Y != s )
						{
							Map[ storiesThatMatch.X ][ x ][ y - 1 ].distanceToUnvisited = num + 1;
							calcDistanceRecursively(storiesThatMatch.Y, x, y - 1, num + 2);
						}
						else if ( storiesThatMatch.Y == s && storiesThatMatch.X != s )
						{
							Map[ storiesThatMatch.Y ][ x ][ y - 1 ].distanceToUnvisited = num + 1;
							calcDistanceRecursively(storiesThatMatch.X, x, y - 1, num + 2);
						}else{
							if(SEND){Serial.println("something went very wrong...");}
						}
					}else{
						// Serial.println("should work...");
						calcDistanceRecursively(s, x, y - 1, num + 1);
					}
				}else{
					/*Serial.println("distance is not 0 and not bigger than num+1 :::");
					Serial.println(x);
					Serial.println(y-1);
					Serial.println(Map[ s ][ x ][ y - 1 ].distanceToUnvisited);*/
				}
			}else{
				// Serial.println("tile is black");
			}
		}else{
			// Serial.println("there is a wall");
		}
	}

	// west field
	if(x > 1)
	{
		if(!Map[ s ][ x ][ y ].directions[WEST])
		{
			if(!Map[ s ][ x - 1 ][ y ].isBlack)
			{
				if(Map[ s ][ x - 1 ][ y ].distanceToUnvisited == 0 or
				Map[ s ][ x - 1 ][ y ].distanceToUnvisited >= num + 1)
				{
					if(Map[ s ][ x - 1 ][ y ].isRamp)
					{
						thisRamp.X = x - 1;
						thisRamp.Y = y;
						storiesThatMatch = mapStoriesThatConnectAt(thisRamp);

						if( storiesThatMatch.X == s && storiesThatMatch.Y != s )
						{
							Map[ storiesThatMatch.X ][ x - 1 ][ y ].distanceToUnvisited = num + 1;
							calcDistanceRecursively(storiesThatMatch.Y, x - 1, y, num + 2);
						}
						else if ( storiesThatMatch.Y == s && storiesThatMatch.X != s )
						{
							Map[ storiesThatMatch.Y ][ x - 1 ][ y ].distanceToUnvisited = num + 1;
							calcDistanceRecursively(storiesThatMatch.X, x - 1, y, num + 2);
						}else{
							if(SEND){Serial.println("something went very wrong...");}
						}
					}else{
						calcDistanceRecursively(s, x - 1, y, num + 1);
					}
				}
			}
		}
	}
	
	return;
}


uint8_t indexofSmallestElement(uint8_t array[4])
{
	uint8_t index = 0;

	if(array[index]!=0){
		index = 0;
	}else if(array[1]!=0){
		index = 1;
	}else if(array[2]!=0){
		index = 2;
	}else if(array[3]!=0){
		index = 3;
	}else{
		return(5);
	}
	
	// das muss so \/
	// null mit sich selbst vergleichen ist nicht notwendig...
	// also i = 1 bis i = 3
	// I call this optimized
    for(int i = 1; i < 4; i++)
    {
        if((array[i] < array[index]) && (array[i]!=0))
            index = i;              
    }

	if(array[robot_is_facing] != 0)
	{
		if(array[robot_is_facing] <= array[index])
		{
			return(robot_is_facing);
		}
	}

    return index;
}


uint8_t mapWhereToDrive() 
{
	uint8_t distancesForCompas[4] = {0, 0, 0, 0};
	// wenn in der Richtung keine Wand ist guck wie weit von dort das näheste unbesuchte Feld ist
	if( !Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[ NOTH ] )
		if( !Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y - 1].isBlack )
			distancesForCompas[ NOTH ] = Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y-1].distanceToUnvisited;
	if( !Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[ EAST ] )
		if( !Map[robot_is_on_story][robot_is_at.X + 1][robot_is_at.Y].isBlack )
			distancesForCompas[ EAST ] = Map[robot_is_on_story][robot_is_at.X+1][robot_is_at.Y].distanceToUnvisited;
	if( !Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[ SOUTH ] )
		if( !Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y + 1].isBlack )
			distancesForCompas[ SOUTH ]= Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y+1].distanceToUnvisited;
	if( !Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[ WEST ] )
		if( !Map[robot_is_on_story][robot_is_at.X - 1][robot_is_at.Y].isBlack )
			distancesForCompas[ WEST ] = Map[robot_is_on_story][robot_is_at.X-1][robot_is_at.Y].distanceToUnvisited;
	
	if(SEND){Serial.println("distances of sourounding fields:");
	for(int i=0; i<4; i++)
	{
		Serial.print(i);
		Serial.print(" : ");
		Serial.println(distancesForCompas[ i ]);
	}
	Serial.println("going to:");
	Serial.println(indexofSmallestElement( distancesForCompas ));}

	return( indexofSmallestElement( distancesForCompas ) );
}


/*
 ▄▄   ▄▄ ▄▄▄▄▄▄▄ ▄▄   ▄▄ ▄▄▄▄▄▄▄ 
█  █ █  █       █  █▄█  █       █
█  █▄█  █   ▄   █       █    ▄▄▄█
█       █  █ █  █       █   █▄▄▄ 
█   ▄   █  █▄█  █       █    ▄▄▄█
█  █ █  █       █ ██▄██ █   █▄▄▄ 
█▄▄█ █▄▄█▄▄▄▄▄▄▄█▄█   █▄█▄▄▄▄▄▄▄█
*/
void mapReturnToHome()
{
	Map[0][STARTX][STARTY].visited = false;
	Map[0][STARTX][STARTY].isBlack = false;
	Map[0][STARTX][STARTY].isRamp = false;
}

/*
 ▄▄▄▄▄▄   ▄▄▄▄▄▄ ▄▄   ▄▄ ▄▄▄▄▄▄▄ 
█   ▄  █ █      █  █▄█  █       █
█  █ █ █ █  ▄   █       █    ▄  █
█   █▄▄█▄█ █▄█  █       █   █▄█ █
█    ▄▄  █      █       █    ▄▄▄█
█   █  █ █  ▄   █ ██▄██ █   █    
█▄▄▄█  █▄█▄█ █▄▄█▄█   █▄█▄▄▄█    
*/
void mapRampAtCurrentField()
{
	Vector possibleRamp = mapStoriesThatConnectAt(robot_is_at);
	if( possibleRamp.X == 69 )
	{
		// mark as visited
		Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].visited = true;
		// mark as ramp
		Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].isRamp = true;
		// set walls acordingly
		Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[ mapDirectionToCompas( FRONT ) ] = 	true;
		Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[ mapDirectionToCompas( RIGHT ) ] = 	true;
		Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[ mapDirectionToCompas( BACK ) ] = 	false;
		Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[ mapDirectionToCompas( LEFT ) ] = 	true;
		Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y-1].directions[ SOUTH ] = 	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[NOTH];
		Map[robot_is_on_story][robot_is_at.X+1][robot_is_at.Y].directions[ WEST ] = 	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[EAST];
		Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y+1].directions[ NOTH ] = 	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[SOUTH];
		Map[robot_is_on_story][robot_is_at.X-1][robot_is_at.Y].directions[ EAST ] = 	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[WEST];

		// put virtual robot on new story
		robot_is_on_story = 1;
		// NO NO NO NO KEEP THE COORDINATES
		// robot_is_at.X = STARTX;
		// robot_is_at.Y = STARTY;

		Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].isRamp = true;
		// set walls acordingly
		Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[ mapDirectionToCompas( FRONT ) ] = 	false;
		Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[ mapDirectionToCompas( RIGHT ) ] = 	true;
		Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[ mapDirectionToCompas( BACK ) ] = 	true;
		Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[ mapDirectionToCompas( LEFT ) ] = 	true;
		Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y-1].directions[ SOUTH ] = 	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[NOTH];
		Map[robot_is_on_story][robot_is_at.X+1][robot_is_at.Y].directions[ WEST ] = 	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[EAST];
		Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y+1].directions[ NOTH ] = 	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[SOUTH];
		Map[robot_is_on_story][robot_is_at.X-1][robot_is_at.Y].directions[ EAST ] = 	Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].directions[WEST];
		// mark current field as visited
		Map[robot_is_on_story][robot_is_at.X][robot_is_at.Y].visited = true;
		// move on field to front
	}else{
		if( possibleRamp.X == robot_is_on_story && possibleRamp.Y != robot_is_on_story )
		{
			robot_is_on_story = possibleRamp.Y;
		}else if ( possibleRamp.Y == robot_is_on_story && possibleRamp.X != robot_is_on_story )
		{
			robot_is_on_story = possibleRamp.X;
		}
	}
	mapMoveTo(FRONT);
}


Vector mapStoriesThatConnectAt(Vector rampCoordinates)
{
	Vector stories;
	int first = 69;
	int second = 69;
	for(uint8_t i = 0; i < NUMBEROFSTORIES; i++)
    {
		if( Map[ i ][ rampCoordinates.X ][ rampCoordinates.Y ].isRamp )
		{
			if( first == 69 )
			{
				first = i;
			}else{
				second = i;
			}
		}
	}
	stories.X = first;
	stories.Y = second;
	/*Serial.println("Theres a ramp connection between:");
	Serial.println(first);
	Serial.println(second);
	Serial.println("at:");
	Serial.println(rampCoordinates.X);
	Serial.println(rampCoordinates.Y);*/
	return(stories);
}

/*
 ▄▄▄▄▄▄  ▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄     ▄▄▄▄▄▄ ▄▄   ▄▄ 
█      ██   █       █       █   █   █      █  █ █  █
█  ▄    █   █  ▄▄▄▄▄█    ▄  █   █   █  ▄   █  █▄█  █
█ █ █   █   █ █▄▄▄▄▄█   █▄█ █   █   █ █▄█  █       █
█ █▄█   █   █▄▄▄▄▄  █    ▄▄▄█   █▄▄▄█      █▄     ▄█
█       █   █▄▄▄▄▄█ █   █   █       █  ▄   █ █   █  
█▄▄▄▄▄▄██▄▄▄█▄▄▄▄▄▄▄█▄▄▄█   █▄▄▄▄▄▄▄█▄█ █▄▄█ █▄▄▄█  
*/
void mapDisplay()
{
	if(SEND){
	/*for( uint8_t s=0; s<NUMBEROFSTORIES; ++s )
	{
		for( uint8_t i=0; i<MAPSIZE; ++i )
		{
			for( uint8_t o=0; o<MAPSIZE; ++o )
			{
				Serial.println("-");
				Serial.print("Field: ");
				Serial.print(s);
				Serial.print(" ");
				Serial.print(i);
				Serial.print(" ");
				Serial.println(o);
				Serial.print("visited: ");
				Serial.println(Map[ s ][i][o].visited);
				Serial.print("ramp: ");
				Serial.println(Map[ s ][i][o].isRamp);
				Serial.print("distance: ");
				Serial.println(Map[ s ][i][o].distanceToUnvisited);
				Serial.print("walls: ");
				for( uint8_t w=0; w<4; ++w )
				{
					Serial.print(Map[ s ][i][o].directions[w]);
					Serial.print(" ");
				}
				Serial.println();
			}
		}
	}*/



	
	/*Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ distances");
	for( uint8_t i=0; i<MAPSIZE; ++i )
	{
		for( uint8_t o=0; o<MAPSIZE; ++o )
		{
			Serial.print(Map[i][o].distanceToUnvisited);
		}
		Serial.println();
	}
	
	Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ visited");
	for( uint8_t i=0; i<MAPSIZE; ++i )
	{
		for( uint8_t o=0; o<MAPSIZE; ++o )
		{
			if(Map[i][o].visited)
			{
				Serial.print("1");
			} else {
				Serial.print("0");
			}
		}
		Serial.println();
	}

	return(0);*/




	Serial.println("Robot is at:");
	Serial.println( robot_is_at.X);
	Serial.println( robot_is_at.Y);
	Serial.println("Robot is facing:");
	Serial.println( robot_is_facing);

	for (uint8_t s=0; s<NUMBEROFSTORIES; ++s)
	{
		Serial.println();
		Serial.print("Story:");
		Serial.println(s);
	
	for (uint8_t o=0; o<MAPSIZE; ++o)
	{
	
	// waende im NORDEN
	for( uint8_t i=0; i<MAPSIZE; ++i )
	{
		if ( Map[s][i][o].directions[NOTH] )
			Serial.print("@@@@@@");
		else
			Serial.print("      ");
	}
	Serial.println();
	// waende im WESTEN und OSTEN
	for( uint8_t i=0; i<MAPSIZE; ++i )
	{
		// WESTEN
		if ( Map[s][i][o].directions[WEST] )
		{	
			Serial.print("@");
		}else{
			Serial.print(" ");
		}

		// STATUS
		Vector skip[LENGTHOFSKIP];
		Vector test;
		test.X = i;
		test.Y = o;
		// skip[0] = *robot_is_at;
		Serial.print( Map[s][i][o].distanceToUnvisited );
		if ( Map[s][i][o].visited )
			Serial.print("V");
		else
			Serial.print("_");

		if ( Map[s][i][o].isBlack )
			Serial.print("B");
		else{
			if ( i == lastVisitedSilver.X && o == lastVisitedSilver.Y ){
				Serial.print("S");
			}else{
				Serial.print("_");
			}
		}

		if ( Map[s][i][o].hasVictim )
			Serial.print("M");
		else
			Serial.print("_");
		

		// OSTEN
		if ( Map[s][i][o].directions[EAST] )
		{	
			Serial.print("@");
		}else{
			Serial.print(" ");
		}
	}
	Serial.println();
	// waende im SUEDEN
	for( uint8_t i=0; i<MAPSIZE; ++i )
	{
		if ( Map[s][i][o].directions[SOUTH] )
			Serial.print("@@@@@@");
		else
			Serial.print("      ");
	}
	Serial.println();
	}
	}
	}
}
