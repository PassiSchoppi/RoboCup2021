#ifndef MAP_H
#define MAP_H

#include <Arduino.h>
#include "config.h"
#include "sensor.h"
#include "led.h"
#include "wall.h"

struct Vector
{
	uint8_t X = 0;
	uint8_t Y = 0;
};

struct Field
{
	// uint8_t properties = 240; // 1111 0000
	bool directions[4] = {1, 1, 1, 1};
	bool visited = false;
	bool isBlack = false;
	int8_t numberOfKits = 69;
	uint8_t victimOnWall = 0;
	bool isRamp = false;
	uint8_t distanceToUnvisited = 0;

};



void mapPreMap();
void mapPutVitimAt(uint8_t field, uint8_t orientation, uint8_t kits);

uint8_t mapVictimAtWall();

// Utility functions
uint8_t mapDirectionToCompas(uint8_t directionI);
uint8_t mapCompasToDirection(uint8_t compasI);
uint8_t indexofSmallestElement(uint8_t array[4]);
bool mapFieldInSkip(Vector field, Vector *skip);
uint8_t mapSearchForUnvisited(Vector startPoint, Vector *skip);

void mapSilverField();
void mapSetBackToLastSilver();

void mapInit();
void mapClear();
void mapUpdateField();
void mapMoveTo(uint8_t directionToGo);

void mapOnlyMoveTo(uint8_t directionToGo);
void mapOnlyTurnTo(uint8_t directionToGo);

void mapBlackFieldFront();
void mapBlackFieldCurrent();
void mapVictimNewAtCurrentField();

void mapReturnToHome();

void mapRampAtCurrentField();
Vector mapStoriesThatConnectAt(Vector rampCoordinates);

void calcDistanceRecursively(uint8_t s, uint8_t x, uint8_t y, uint8_t num);

uint8_t mapVictimIsAtCurrentField();
uint8_t mapWhereToDrive();
void mapDisplay();

#endif
