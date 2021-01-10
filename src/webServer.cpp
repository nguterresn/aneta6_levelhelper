#include "webServer.h"

//Creates server listening at port 80
AsyncWebServer server(80);

//ADC takes about 500us per analogRead
unsigned int adc_num = 1000;

//Since IR depends on the "color" of the oppositw surface 
//it is hard to create a function that displays a match between distance and digital values.
/*double calc_dist (int param) {
    double y = (0.000021)*param*param + 0.003349*param + 2.30578;
    return y;
}*/

void notFound(AsyncWebServerRequest *request);

void SetupServer() {

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", index_html);
    });

    server.on("/measure_level", HTTP_GET, [](AsyncWebServerRequest *request){

		//Reset variables
		unsigned int avg = 0;
		unsigned int c = 0;
		unsigned int adc_read = 0;

		//Measure ADC_NUM times the adc
		for (size_t i = 0; i < adc_num; i++) {

			//reads ADC data
			adc_read = analogRead(A0);

			//waits 1ms
			delay(1);

			//Writes serially the adc data measured
			Serial.println(adc_read);

			//Adds up 
			c += adc_read;
		}	

		//Calculates avg
		avg = c / adc_num;

        request->send_P(200, "text/plain", String(avg).c_str());
    });

    server.on("/set_distance", HTTP_GET, [](AsyncWebServerRequest *request){
       /* unsigned int adc_read = 0;
        adc_read = analogRead(A0);
        request->send_P(200, "text/plain", String(calc_dist(adc_read)).c_str());*/
        request->send_P(200, "text/plain", String("not working").c_str());
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

}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}
