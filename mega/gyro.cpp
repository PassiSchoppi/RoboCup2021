#include "gyro.h"
#include "Wire.h" // This library allows you to communicate with I2C devices.

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.

int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data

char tmp_str[7]; // temporary variable used in convert function

void gyroInit()
{
	Wire.begin();
  	Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  	Wire.write(0x6B); // PWR_MGMT_1 register
  	Wire.write(0); // set to zero (wakes up the MPU-6050)
  	Wire.endTransmission(true);
}

void gyroInterrupt()
{  
	Wire.beginTransmission(MPU_ADDR);
  	Wire.write(0x3B);
  	Wire.endTransmission(false);
  	Wire.requestFrom(MPU_ADDR, 7*2, true);
	
  	accelerometer_x = Wire.read()<<8 | Wire.read();
  	accelerometer_y = Wire.read()<<8 | Wire.read();
  	accelerometer_z = Wire.read()<<8 | Wire.read();
  	temperature = Wire.read()<<8 | Wire.read();
  	gyro_x = Wire.read()<<8 | Wire.read(); 
  	gyro_y = Wire.read()<<8 | Wire.read();
  	gyro_z = Wire.read()<<8 | Wire.read();
}

int16_t gyroX()
{
	return(accelerometer_x);
}

int16_t gyroY()
{
	return(accelerometer_y);
}

