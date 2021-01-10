#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "index.h"

#define SERVERUP 1
#define MEASURE_ADC 2
#define NUM_READINGS 3

void SetupServer();

#endif