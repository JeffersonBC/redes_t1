#include <iostream>
#include <string.h>

#include <client-server.h>
#include <physical_sensors.h>


using namespace std;

void SendMeasures(int sckt);
void SendMeasures(int s, int sckt);


// Sensores fixos
PressureSensor 		fixed_pressure[4];
TemperatureSensor 	fixed_temperature[4];
HumiditySensor 		fixed_humidity[4];
LightSensor			fixed_light[4];


int main(int argc, char const *argv[]){
	int sckt;
	init_server(sckt);

	while (true){
		int opt1, opt2;
		read(sckt, &opt1, sizeof(opt1));
		read(sckt, &opt2, sizeof(opt2));

		if (opt1 == 1){
			SendMeasures(opt2, sckt);
		}
	}

	return 0;
}


void SendMeasures(int sckt){
	for (int i = 0; i < 4; i++){
		fixed_pressure[i].SendMeasure(sckt);
		fixed_temperature[i].SendMeasure(sckt);
		fixed_humidity[i].SendMeasure(sckt);
		fixed_light[i].SendMeasure(sckt);
	}
}

void SendMeasures(int s, int sckt){
	fixed_pressure[s - 1].SendMeasure(sckt);
	fixed_temperature[s - 1].SendMeasure(sckt);
	fixed_humidity[s - 1].SendMeasure(sckt);
	fixed_light[s - 1].SendMeasure(sckt);
}