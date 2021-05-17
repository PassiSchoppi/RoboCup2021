#include "map.h"


// 2D Array of Fields
Field Map[NUMBEROFSTORIES][MAPSIZE][MAPSIZE];

// Variables of Robot
uint8_t robot_is_facing;
uint8_t robot_is_on_story = 0;
Vector robot_is_at;

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

void mapInit() 
{
	robot_is_facing = NOTH;
	// FIXME: half of MAPSIZE
	robot_is_at.X = 5;
	robot_is_at.Y = 5;
}


void mapUpdateField()
{
	Serial.println("updating field");

	// add walls to current field
	Map[robot_is_at.X][robot_is_at.Y].directions[ mapDirectionToCompas( FRONT ) ] = wallExists(FRONT);
	Map[robot_is_at.X][robot_is_at.Y].directions[ mapDirectionToCompas( RIGHT ) ] = wallExists(RIGHT);
	Map[robot_is_at.X][robot_is_at.Y].directions[ mapDirectionToCompas( BACK ) ] = 	wallExists(BACK);
	Map[robot_is_at.X][robot_is_at.Y].directions[ mapDirectionToCompas( LEFT ) ] = 	wallExists(LEFT);

	Serial.println("new walls:");
	for ( uint8_t i=0; i<4; ++i ){
		Serial.println(Map[robot_is_at.X][robot_is_at.Y].directions[i]);
	}
	
	// change walls of sourounding fields
	Map[robot_is_at.X][robot_is_at.Y-1].directions[ SOUTH ] = 	Map[robot_is_at.X][robot_is_at.Y].directions[NOTH];
	Map[robot_is_at.X+1][robot_is_at.Y].directions[ WEST ] = 	Map[robot_is_at.X][robot_is_at.Y].directions[EAST];
	Map[robot_is_at.X][robot_is_at.Y+1].directions[ NOTH ] = 	Map[robot_is_at.X][robot_is_at.Y].directions[SOUTH];
	Map[robot_is_at.X-1][robot_is_at.Y].directions[ EAST ] = 	Map[robot_is_at.X][robot_is_at.Y].directions[WEST];

	// mark current field as visited
	Map[robot_is_at.X][robot_is_at.Y].visited = true;

	// clear distances from the map
	for ( uint8_t i=0; i<MAPSIZE; ++i ){
		for ( uint8_t o=0; o<MAPSIZE; ++o ){
			if (Map[ i ][ o ].visited == false)
			{
				Map[ i ][ o ].distanceToUnvisited = 1;
			} else {
				Map[ i ][ o ].distanceToUnvisited = 0;
			}
		}
	}
	
	// update distance to nearest unvisited
	for ( uint8_t i=0; i<MAPSIZE; ++i ){
		for ( uint8_t o=0; o<MAPSIZE; ++o ){
			if (Map[ i ][ o ].visited == false)
			{
				calcDistanceRecursively(i, o, 1);
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


void mapMoveTo(uint8_t directionToGo)
{
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

void mapBlackFieldFront()
{
}

void mapVictimNewAtCurrentField()
{
	Map[ robot_is_at.X ][ robot_is_at.Y ].hasVictim = true;
}

bool mapVictimIsAtCurrentField()
{
	return( Map[robot_is_at.X][robot_is_at.Y].hasVictim );
}

void calcDistanceRecursively(uint8_t x, uint8_t y, uint8_t num)
{
	/*Serial.println("recursion call:");
	Serial.println(x);
	Serial.println(y);
	Serial.println(num);*/

	Map[ x ][ y ].distanceToUnvisited = num;

	// north field
	if(y < MAPSIZE - 1)
	{
		if(!Map[ x ][ y ].directions[NOTH])
		{
			if(!Map[ x ][ y + 1 ].isBlack)
			{
				if(Map[ x ][ y + 1 ].distanceToUnvisited == 0 or
				Map[ x ][ y + 1 ].distanceToUnvisited >= num + 1)
				{
					calcDistanceRecursively(x, y + 1, num + 1);
				}
			}
		}
	}

	// east field
	if(x < MAPSIZE - 1)
	{
		if(!Map[ x ][ y ].directions[EAST])
		{
			if(!Map[ x + 1 ][ y ].isBlack)
			{
				if(Map[ x + 1 ][ y ].distanceToUnvisited == 0 or
				Map[ x + 1 ][ y ].distanceToUnvisited >= num + 1)
				{
					calcDistanceRecursively(x + 1, y, num + 1);
				}
			}
		}
	}

	// south field
	if(y > 1)
	{
		if(!Map[ x ][ y ].directions[SOUTH])
		{
			if(!Map[ x ][ y - 1 ].isBlack)
			{
				if(Map[ x ][ y - 1 ].distanceToUnvisited == 0 or
				Map[ x ][ y - 1 ].distanceToUnvisited >= num + 1)
				{
					calcDistanceRecursively(x, y - 1, num + 1);
				}
			}
		}
	}

	// west field
	if(x > 1)
	{
		if(!Map[ x ][ y ].directions[WEST])
		{
			if(!Map[ x - 1 ][ y ].isBlack)
			{
				if(Map[ x - 1 ][ y ].distanceToUnvisited == 0 or
				Map[ x - 1 ][ y ].distanceToUnvisited >= num + 1)
				{
					calcDistanceRecursively(x - 1, y, num + 1);
				}
			}
		}
	}
	
	return(0);
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
	if( !Map[robot_is_at.X][robot_is_at.Y].directions[ NOTH ] )
		if( !Map[robot_is_at.X][robot_is_at.Y + 1].isBlack )
			distancesForCompas[ NOTH ] = Map[robot_is_at.X][robot_is_at.Y-1].distanceToUnvisited;
	if( !Map[robot_is_at.X][robot_is_at.Y].directions[ EAST ] )
		if( !Map[robot_is_at.X + 1][robot_is_at.Y].isBlack )
			distancesForCompas[ EAST ] = Map[robot_is_at.X+1][robot_is_at.Y].distanceToUnvisited;
	if( !Map[robot_is_at.X][robot_is_at.Y].directions[ SOUTH ] )
		if( !Map[robot_is_at.X][robot_is_at.Y - 1].isBlack )
			distancesForCompas[ SOUTH ]= Map[robot_is_at.X][robot_is_at.Y+1].distanceToUnvisited;
	if( !Map[robot_is_at.X][robot_is_at.Y].directions[ WEST ] )
		if( !Map[robot_is_at.X - 1][robot_is_at.Y].isBlack )
			distancesForCompas[ WEST ] = Map[robot_is_at.X-1][robot_is_at.Y].distanceToUnvisited;

	return( indexofSmallestElement( distancesForCompas ) );
}


void mapRampAtCurrentField()
{
	Map[robot_is_at.X][robot_is_at.Y].isRamp = true;
}


void mapDisplay()
{
	for( uint8_t i=0; i<MAPSIZE; ++i )
	{
		for( uint8_t o=0; o<MAPSIZE; ++o )
		{
			Serial.println("-");
			Serial.print("Field: ");
			Serial.print(i);
			Serial.print(" ");
			Serial.println(o);
			Serial.print("visited: ");
			Serial.println(Map[i][o].visited);
			Serial.print("distance: ");
			Serial.println(Map[i][o].distanceToUnvisited);
			Serial.print("walls: ");
			for( uint8_t w=0; w<4; ++w )
			{
				Serial.print(Map[i][o].directions[w]);
				Serial.print(" ");
			}
			Serial.println();
		}
	}




	/*
	Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ distances");
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

	return(0);





	
	for (uint8_t o=0; o<MAPSIZE; ++o)
	{
	
	// waende im NORDEN
	for( uint8_t i=0; i<MAPSIZE; ++i )
	{
		if ( Map[i][o].directions[NOTH] )
			Serial.print("@@@");
		else
			Serial.print("   ");
	}
	Serial.println();
	// waende im WESTEN und OSTEN
	for( uint8_t i=0; i<MAPSIZE; ++i )
	{
		// WESTEN
		if ( Map[i][o].directions[WEST] )
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
		// Serial.print( mapSearchForUnvisited( test, skip, false ) );
		Serial.print( Map[i][o].distanceToUnvisited );
		// if ( Map[i][o].visited )
			// Serial.print("V");
		// else
			// Serial.print("X");
		

		// OSTEN
		if ( Map[i][o].directions[EAST] )
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
		if ( Map[i][o].directions[SOUTH] )
			Serial.print("@@@");
		else
			Serial.print("   ");
	}
	Serial.println();
	delay(1);
	}
	*/
}
