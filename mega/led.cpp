#include "led.h"
#include "config.h"

void LEDSetColor(uint8_t color)
{
	Serial3.write(color);
}
