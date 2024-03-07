// Temp Sensor
#include "OneWire.h"
#include "DallasTemperature.h"
// I2C screen
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

OneWire oneWire(13); // GPIO13
DallasTemperature sensors(&oneWire);

void setup()
{
	sensors.begin();
}

void loop()
{
}
