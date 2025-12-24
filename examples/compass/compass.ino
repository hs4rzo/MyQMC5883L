#include <QMC5883L.h>
#include <Wire.h>

QMC5883L compass;
bool compassready = false;
int heading  =0;
void setup()
{

compassready = compass.startInit();
	
	Serial.begin(9600);
	Serial.println("QMC5883L Compass Demo");
	Serial.println("Turn compass in all directions to calibrate....");
}

void loop()
{
	if (compassready)
     heading  = compass.readHeading360();//  compass.readHeading180()
	
		Serial.println(heading);
	
}
