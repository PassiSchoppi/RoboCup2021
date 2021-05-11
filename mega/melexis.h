#ifndef MELEXIS_H_
#define MELEXIS_H_

#include "i2cmaster.h"
#include <Arduino.h>

/*
 *
 *      ---------------------
 *      |                   |
 *      |  A2           A4  |
 *      |        /|\        |
 *      |       / | \       |
 *      |         |         |
 *      |         |         |
 *      |  A6           A0  |
 *      |                   |
 *      --------------------- 
 *        MELEXIS POSITIONS
 * 
 */

struct Melexis {
    uint8_t address;
    float value;
};

// returns heat value in celcius
float melexisGetValue(uint8_t i);

void melexisInit();

// updates the value
// ! may take some time if not properly connected
void melexisInterrupt();

float melexisVerify(uint8_t i);

float melexisTemperature(uint8_t address);

// changes the hardware addr of all connected melexis
int melexisChangeAddress(uint8_t newAddress);


#endif
