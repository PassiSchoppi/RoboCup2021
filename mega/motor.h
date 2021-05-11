#ifndef MOTOR_H_
#define MOTOR_H_

#include <Arduino.h>
#include "config.h"
#include "sensor.h"
#include "wall.h"

/*
 *
 *                 pin1/pin2:pwm
 *
 *             +---------------------+
 *             |                     |
 *     29/27:4 |  0 A           D 2  | 49/47:5
 *             |          /|\        |
 *             |         / | \       |
 *             |           |         |
 *             |           |         |
 *     23/25:3 |  1 B           C 3  | 51/53:6
 *             |                     |
 *             +---------------------+ 
 *                MOTOR POSITIONS
 * 
 */

struct Motor 
{
    // pins to controll direction and speed
    uint8_t pin1;
    uint8_t pin2;
    // value of current encoder signal (addr)
	volatile uint8_t* enc;
	uint8_t encbit;
	// value of last encoder signal (value)
	bool 	lastEncSignal;
	// steps this motor made
	volatile unsigned int 	steps;
	// pwm pin on arduino
    uint8_t pwm;
    // motor factor caused by manufacturing differences
    double  factor;
};

void motorInit();

// set speed of individual motor
void motorSetSpeed(uint8_t motor, int16_t speed);

// checks encoder steps
// should be called every milli second
void motorCheckForStepsMade(uint8_t i);

// set all motor steps to 0
void motorResetAllSteps();

// returns steps an individual motor made
unsigned int motorStepsMade(uint8_t i);

// returns regulated speed so the robot moves straight
int speedFromEnc(unsigned int encA, unsigned int encB, unsigned int encC, unsigned int encME, int speedME);

// let the robot drive in specific direction
void motorDriveTo(uint8_t direction, int speed);

// stop all motors
void motorBrake();


#endif
