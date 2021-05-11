#include "melexis.h"


Melexis melexis[2];

uint8_t nextMelexis;

void melexisInit() 
{
    pinMode(SDA, INPUT_PULLUP);
    pinMode(SCL, INPUT_PULLUP);
    i2c_init();
	
    melexis[0].address = 0xA0;
	melexis[1].address = 0xA6;
    nextMelexis = 0;
}

float melexisGetValue(uint8_t i)
{
	return melexis[i].value;
}

void melexisInterrupt() 
{
	// Serial.println("interrupt");
    float value = melexis[nextMelexis].value;

    melexis[nextMelexis].value = melexisTemperature(melexis[nextMelexis].address);

    if(melexis[nextMelexis].value>=50)
        melexis[nextMelexis].value = value;

    nextMelexis = (nextMelexis == 1 ? 0 : nextMelexis+1);
}

float melexisVerify(uint8_t i) 
{
    return melexisTemperature(melexis[nextMelexis].address);
}

float melexisTemperature(uint8_t address)
{
    int data_low = 0;
    int data_high = 0;
    int pec = 0;

    // Start reading linearized temperature (Application Note SMBus communication with MLX90614 p. 14)
    i2c_start_wait(address+I2C_WRITE);
    i2c_write(0x07);

    //Read temperature
    i2c_rep_start(address+I2C_READ);
    data_low = i2c_readAck();      				// Read 1 byte and then send ack.
    data_high = i2c_readAck();          		// Read 1 byte and then send ack.
    pec = i2c_readNak();
    i2c_stop();

    // This converts high and low bytes together and processes temperature,
    // MSB is a error bit and is ignored for temps.
    double tempFactor = 0.02;      				// 0.02 degrees per LSB (measurement
								                // resolution of the MLX90614).
    double tempData = 0x0000;      				// Zero out the data

    // This masks off the error bit of the high byte, then moves it left
    // 8 bits and adds the low byte.
    tempData = (double)(((data_high & 0x007F) << 8) + data_low);
    tempData = (tempData * tempFactor)-0.01;
    float celcius = tempData - 273.15;

    // Returns temperature in celcius.
    return celcius;
}

int melexisChangeAddress(uint8_t newAddress)
{
	Serial.print("new address: ");Serial.println(newAddress);
	i2c_start_wait(0x00 + I2C_WRITE);   		// Send start condition and write bit.
    i2c_write(0x2E);    			            // Send command for device to return address (0x2E).
    i2c_write(0x00);                			// Send low byte zero to erase.
    i2c_write(0x00);    		            	// Send high byte zero to erase.
    if (i2c_write(0x6F) == 0) 
    {
        i2c_stop();	                    		// Release bus, end transaction.
        // Address erase confirmation message.
    }
    else 
    {
        i2c_stop();                             // Release bus, end transaction.
        return -1;
    }   
    delay(5);                                   //wait 5 ms to erase

    int a = 0;
    for(a; a!=256; a++) 
    {
        i2c_start_wait(0x00 + I2C_WRITE);       // Send start condition and write bit.
        i2c_write(0x2E);            			// Send command for device to return address (0x2E).
        i2c_write(newAddress>>1);       		// Send low byte zero to assign new address.
        i2c_write(0x00);            			// Send high byte zero to assign new address.
        if (i2c_write(a) == 0)                  // Send PEC
        {
            i2c_stop();              			// Release bus, end transaction.
            delay(100);             			// Wait 10ms.
            return a;
        }
    }
    i2c_stop();  		                    	// Release bus, end transaction.
    return -1;
}
