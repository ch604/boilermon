// INCLUDES //
// temp sensors
#include "OneWire.h"
#include "DallasTemperature.h"

// screen
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// network
#include <WiFi.h>
#include <HTTPClient.h>

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
int totalTempSens;
float tempC;

// pump CTs
const int boilerPin = A0; // GPIO36
const int circPin = A3; // GPIO39
const int dhwPin = A6; // GPIO34
float boilerVal;
float circVal;
float dhwVal;


// screen
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16x2 display

// network
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASS";
const char* endpoint = "YOUR_POST_ENDPOINT";

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
	totalTempSens = sensors.getDeviceCount();

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

	// network
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
	for (int i = 0;  i < totalTempSens; i++) {
		Serial.print("Temp Sensor ");
		Serial.print(i+1);
		Serial.print(" : ");
		Serial.print(DallasTemperature::toFahrenheit(sensors.getTempCByIndex(i)));
		Serial.print((char)176);
		Serial.println("F");
	}

	// zone valve optocoupler
	zone1Val = digitalRead(zone1Pin);
	String result = zone1Val = 1 ? "Open" : "Closed";
	Serial.print("Zone 1 is ");
	Serial.println(result);
	zone2Val = digitalRead(zone2Pin);
	String result = zone2Val = 1 ? "Open" : "Closed";
	Serial.print("Zone 2 is ");
	Serial.println(result);
	zone3Val = digitalRead(zone3Pin);
	String result = zone3Val = 1 ? "Open" : "Closed";
	Serial.print("Zone 3 is ");
	Serial.println(result);
	zone4Val = digitalRead(zone4Pin);
	String result = zone4Val = 1 ? "Open" : "Closed";
	Serial.print("Zone 4 is ");
	Serial.println(result);

	// pump CTs
	boilerVal = readCurrentVal(boilerPin);
	circVal = readCurrentVal(circPin);
	dhwVal = readCurrentVal(chwPin);

	// upload data
	HTTPClient http;
	http.begin(endpoint);
	http.addHeader("Content-Type", "application/json");
	String message="{\"temp1\":" + ;
	message
	int httpResponseCode = http.POST(message);
	
	// loop delay 5s
	delay(5000);
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
