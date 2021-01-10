#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "webServer.h"

#define ERROR -1
#define SUCCESS 0

IPAddress ip(192, 168, 0, 100); 
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

const char* ssid = "Quarto Escuro";
const char* password = "#";

String ConfigWifi() {

	int counter = 0;
	WiFi.config(ip, gateway, subnet);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print("..");

		// Timeout
		if (counter >= 10)
			return (String) ERROR;
		
		counter++;
	}

	return WiFi.localIP().toString();

}

void SetupUART() {
	Serial.begin(115200);
}

void setup() {

	String return_code;

	//Setup UART. Since Serial.begin returns nothing, error is not checked.
	SetupUART();
	Serial.println("***** Starting Code ******"); 
	Serial.print("Connecting to Wi-Fi -> ");

	//Setup & config Wifi
	return_code = ConfigWifi();
	if (return_code == (String) ERROR)
		exit(1);
	else 
		Serial.println(return_code);

	//Setup WebServer at 192.168.0.100:80 
	SetupServer();
	Serial.println("server is up");

}

void loop() {}
