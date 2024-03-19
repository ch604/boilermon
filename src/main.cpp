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
#include <ArduinoJson.h>

// time
#include "time.h"

// DEFINITIONS //
// zone valve optocoupler
const int zone1Pin = 32;
const int zone2Pin = 33;
const int zone3Pin = 25;
const int zone4Pin = 26;
int zoneVal[4];

// temp sensors
OneWire oneWire(13); // GPIO13 for all temp sensor data pins
DallasTemperature sensors(&oneWire);
int totalTempSens;

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
	int tempVal[totalTempSens];

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
	Serial.printf("Connecting to %s\r\n", ssid);
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
	for (byte i = 0;  i < totalTempSens; i++) {
		tempVal[i]=DallasTemperature::toFahrenheit(sensors.getTempCByIndex(i));
		Serial.printf("Temp Sensor %i : %i", i+1, tempVal[i]);
		Serial.print((char)176);
		Serial.println("F");
	}

	// zone valve optocoupler
	zoneVal[0] = digitalRead(zone1Pin);
	zoneVal[1] = digitalRead(zone2Pin);
	zoneVal[2] = digitalRead(zone3Pin);
	zoneVal[3] = digitalRead(zone4Pin);
	for (byte i = 0; i < 4; i++) {
		Serial.printf("Zone %s is %s\r\n", i+1, zoneVal[i])
	}

	// pump CTs
	boilerVal = readCurrentVal(boilerPin);
	circVal = readCurrentVal(circPin);
	dhwVal = readCurrentVal(chwPin);

	// upload data
	postDataToServer();
	
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
void postDataToServer() {
	Serial.println("Posting JSON data to server...");
	HTTPClient http;

	http.begin(endpoint);
	http.addHeader("Content-Type", "application/json");

	StaticJsonDocument<200> message;
	for (byte i = 0; i < 4; i++) {
		message["zone"][i] = zoneVal[i];
	}
	message["boiler"] = boilerVal;
	message["circ"] = circVal;
	message["dhw"] = dhwVal;
	for (byte i = 0;  i < totalTempSens; i++) {
		message["temp"][i] = tempVal[i];
	}

	String requestBody;
	serializeJson(message, requestBody);

	int httpResponseCode = http.POST(requestBody);
}
