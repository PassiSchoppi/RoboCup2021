#ifndef MAP_H
#define MAP_H

#include <Arduino.h>
#include "config.h"
#include "sensor.h"
#include "wall.h"

struct Vector
{
	uint8_t X = 0;
	uint8_t Y = 0;
};

struct Field
{
	bool directions[4] = {1, 1, 1, 1};
	bool visited = false;
	bool isSilver = false;
	bool isBlack = false;
	bool hasVictim = false;
	bool isRamp = false;
	uint8_t distanceToUnvisited;
};

// Utility functions
uint8_t mapDirectionToCompas(uint8_t directionI);
uint8_t mapCompasToDirection(uint8_t compasI);
uint8_t indexofSmallestElement(uint8_t array[4]);
bool mapFieldInSkip(Vector field, Vector *skip);
uint8_t mapSearchForUnvisited(Vector startPoint, Vector *skip);

void mapInit();
void mapUpdateField();
void mapMoveTo(uint8_t directionToGo);
void mapBlackFieldFront();
void mapVictimNewAtCurrentField();

void calcDistanceRecursively(uint8_t x, uint8_t y, uint8_t num);

bool mapVictimIsAtCurrentField();
uint8_t mapWhereToDrive();
void mapDisplay();

#endif
