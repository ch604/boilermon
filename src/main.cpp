// INCLUDES //
// temp sensors
#include "OneWire.h"
#include "DallasTemperature.h"

// screen
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// wifi
#include <WiFi.h>

// time
#include "time.h"

// DEFINITIONS //
// zone valve optocoupler
const int zone1Pin = 32;
const int zone2Pin = 33;
const int zone3Pin = 25;
const int zone4Pin = 26;
int zone1Val;
int zone2Val;
int zone3Val;
int zone4Val;

// temp sensors
OneWire oneWire(13); // GPIO13 for all temp sensor data pins
DallasTemperature sensors(&oneWire);
int total_tempsens;

// pump CTs
const int boilerPin = A0; // GPIO36
const int circPin = A3; // GPIO39
const int dhwPin = A6; // GPIO34
float boilerVal;
float circVal;
float dhwVal;


// screen
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16x2 display

// wifi
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASS";

// time
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -18000;
const int daylightOffset_sec = 3600;

// STARTUP //
void setup()
{
	Serial.begin(115200);
	delay(1000);

	// zone valve optocoupler
	pinMode(zone1Pin, INPUT);
	pinMode(zone2Pin, INPUT);
	pinMode(zone3Pin, INPUT);
	pinMode(zone4Pin, INPUT);

	// temp sensors
	sensors.begin();
	total_tempsens = sensors.getDeviceCount();

	// pump CTs
	pinMode(boilerPin, INPUT);
	pinMode(circPin, INPUT);
	pinMode(dhwPin, INPUT);

	// screen
	lcd.init();
	lcd.clear();
	lcd.backlight();
	lcd.setCursor(0,0);
	lcd.print("Initializing...");

	// wifi
	Serial.printf("Connecting to %s ", ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println(" CONNECTED");

	// time
	configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

// MAIN LOOP //
void loop()
{
	// temp sensors
	sensors.requestTemperatures();

	// zone valve optocoupler
	zone1Val = digitalRead(zone1Pin);
	zone2Val = digitalRead(zone2Pin);
	zone3Val = digitalRead(zone3Pin);
	zone4Val = digitalRead(zone4Pin);

	// pump CTs
	boilerVal = readCurrentVal(boilerPin);
	circVal = readCurrentVal(circPin);
	dhwVal = readCurrentVal(chwPin);

	// loop delay
	delay(1000);
}

// FUNCTIONS //
// read the amperage of a given pin
void readCurrentVal( char* x ) {
	float CTVal = 0;
	float peakVoltage = 0;
	float voltageVirtualValue = 0;  //Vrms
	for (int i = 0; i < 5; i++) {
		peakVoltage += analogRead(x);
		delay(1);
	}
	peakVoltage = peakVoltage / 5;
	voltageVirtualValue = peakVoltage * 0.707;
	voltageVirtualValue = (voltageVirtualValue / 1024 * 5 ) / 2;
	CTVal = voltageVirtualValue * 10; //10A CTs
	return CTVal;
}
