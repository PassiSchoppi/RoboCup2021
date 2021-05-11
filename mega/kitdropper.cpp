#include "kitdropper.h"

void kitdropperInit(){
	kitdropperSetTo(POSMIDD);
}

void kitdropperSetTo(uint8_t deg)
{
	Serial1.write(deg);
}
