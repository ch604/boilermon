// INCLUDES //
// temp sensors
#include "OneWire.h"
#include "DallasTemperature.h"
// screen
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// DEFINITIONS //
// temp sensors
OneWire oneWire(13); // GPIO13 for all temp sensor data pins
DallasTemperature sensors(&oneWire);
int total_tempsens

// screen
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16x2 display

// STARTUP //
void setup()
{
	Serial.begin(115200);

	// temp sensors
	sensors.begin();
	total_tempsens = sensors.getDeviceCount();

	// screen
	lcd.init();
	lcd.backlight();
}

// MAIN LOOP //
void loop()
{
	sensors.requestTemperatures();
}
