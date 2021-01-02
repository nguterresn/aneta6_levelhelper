#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "index.h"

//ADC takes about 500us per analogRead
unsigned int adc_num = 1000;

unsigned int avg = 0;
unsigned int c = 0;
unsigned int adc_read = 0;

IPAddress ip(192, 168, 0, 100); 
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
AsyncWebServer server(80);

const char* ssid = "Quarto Escuro";
const char* password = "$%";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {

	Serial.begin(115200);
	Serial.println("***** Starting Code ******"); 

	Serial.println("Connecting to Wi-Fi");
	WiFi.config(ip, gateway, subnet);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.println("..");
	}
	Serial.println(WiFi.localIP());

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", index_html);
    });

    server.on("/measure_level", HTTP_GET, [](AsyncWebServerRequest *request){

		//Reset variables
		c = 0, avg = 0, adc_read = 0;

		//Measure ADC_NUM times the adc
		for (size_t i = 0; i < adc_num; i++) {
			adc_read = analogRead(A0);
			delay(1);
			Serial.println(adc_read);
			c += adc_read;
		}	

		//Calculate avg
		avg = c/adc_num;

        request->send_P(200, "text/plain", String(avg).c_str());
    });

	server.on("/change_adc", HTTP_GET, [](AsyncWebServerRequest *request){
		String received_precision;

		if (request->hasParam("precision")) {
			received_precision = request->getParam("precision")->value();
			adc_num = received_precision.toInt();
			request->send(200);
		}

	});

    server.onNotFound(notFound);
	server.begin();

	Serial.println("server is up");

}

void loop() {

}