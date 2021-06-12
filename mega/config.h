#ifndef CONFIG_H
#define CONFIG_H

// ## SETTINGS ##
#define DOBLACKTILEDETECTION 	true
#define DOMAP 					true
#define DODRIVINGCORECTION 		true
#define CORECTIONAGGRESSIVE 	1
#define DOVICTIMEDETECTION 		false
#define DORAMPDETECTION         true
#define DORAMPONMAP             false
#define SMOOTHENSENSORDATA 		5 // 15
#define MAPSIZE 				10
#define NUMBEROFSTORIES         2
#define LENGTHOFSKIP 			10

// ## GLOBAL VARIABLES ##
extern volatile uint8_t sensorData[18];
extern uint8_t state;
extern uint8_t level;  // ebene fläche

#define INTERUPT_PIN_A 	8
#define INTERUPT_PIN_B 	9

#define NOTH 			0
#define EAST 			1
#define SOUTH 			2
#define WEST 			3

#define STOP 			0

#define FRONT 			0
#define RIGHT 			1
#define BACK 			2
#define LEFT 			3

#define RAMPSTATE       4

#define DISTANCETOWALL 	36
#define PERFECTDISTTOW  70
#define MINDISTANCETOW 	100
#define CUTDATAVAL 		0

#define RAMP_THRESHOLD  15
#define REST_OF_RAMP    30
#define STEPSFORHALFARAMP   250

#define VICTIMTEMP 		28

#define POSLEFT 		70
#define POSMIDD 		120
#define POSRIGHT 		150

#define BASESPEED 		100

#define P 				30

#define STEPSFORONE 	135
#define STEPSFORHALF 	90
/*#define STEPFFORRIGHT 	70
#define STEPSFORLEFT 	70*/
#define STEPFFORRIGHT 	73
#define STEPSFORLEFT 	73


// #define MAXWHITE 		220
// #define MINWHITE 		150

#define MAXWHITE 		240
#define MINWHITE 		50

#define OFF 			0
#define WHITE 			1
#define RED 			2
#define GREEN 			3
#define BLUE 			4
#define PINK 			5
#define YELLOW 			6
#define TURQUOISE 		7

#define SIGNUM(x) ((x==0) ? 0 : x/abs(x))

#endif
