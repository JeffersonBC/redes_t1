#include <iostream>
#include <string.h>

#include <client-server.h>
#include <physical_sensors.h>


using namespace std;

// Envia medições dos sensores à central
void SendMeasures(int opt1, int sckt);

// Envia medições de um sensor à central
void SendMeasures(int opt1, int opt2, int sckt);

// Adiciona constantes às medições aleatórias para que elas mostrem resultados mais verossímeis
void InitializeBias();


// Sensores fixos
PressureSensor 		fixed_pressure[4];
TemperatureSensor 	fixed_temperature[4];
HumiditySensor 		fixed_humidity[4];
LightSensor			fixed_light[4];

// Sensores móveis
TemperatureSensor 	mobile_temperature[4];
HeartbeatSensor		mobile_heartbeat[4];
HeartpressureSensor mobile_heartpressure[4];
BreathSensor		mobile_breath[4];


int main(int argc, char const *argv[]){
	int sckt;
	InitServer(sckt);

	InitializeBias();

	while (true){
		int opt1, opt2;

		// Recebe requisição
		read(sckt, &opt1, sizeof(opt1));
		read(sckt, &opt2, sizeof(opt2));

		printf("Read: %d %d\n", opt1, opt2);


		// Responde requisição
		if (opt1 == 3){
			SendMeasures(1, sckt);
			SendMeasures(2, sckt);
		}

		else if (opt2 >= 1 && opt2 <= 4) SendMeasures(opt1, opt2, sckt);
		else if (opt2 == 5) 		SendMeasures(opt1, sckt);
	}

	return 0;
}

// Adiciona constantes às medições aleatórias para que elas mostrem resultados mais verossímeis
void InitializeBias(){
	fixed_temperature[1].bias = 4.0f;
	fixed_temperature[2].bias = -6.0f;

	fixed_humidity[2].bias = -10.0f;
	fixed_humidity[3].bias = 15.0f;

	fixed_light[0].bias = -600.0f;
	fixed_light[2].bias = -1000.0f;

	for (int i = 0; i < 4; i++)
		mobile_temperature[i].bias = 12.0f;
}

// Envia medições dos sensores à central
void SendMeasures(int opt1, int sckt){
	if (opt1 == 1){
		for (int i = 0; i < 4; i++){
			fixed_pressure	  [i].SendMeasure(sckt);
			fixed_temperature [i].SendMeasure(sckt);
			fixed_humidity	  [i].SendMeasure(sckt);
			fixed_light		  [i].SendMeasure(sckt);
		}
	}
	else if (opt1 == 2){
		for (int i = 0; i < 4; i++){
			mobile_temperature	 [i].SendMeasure(sckt);
			mobile_heartbeat	 [i].SendMeasure(sckt);
			mobile_heartpressure [i].SendMeasure(sckt);
			mobile_breath		 [i].SendMeasure(sckt);
		}
	}
}

// Envia medições de um sensor à central
void SendMeasures(int opt1, int opt2, int sckt){
	if (opt1 == 1){
		fixed_pressure	  [opt2 - 1].SendMeasure(sckt);
		fixed_temperature [opt2 - 1].SendMeasure(sckt);
		fixed_humidity	  [opt2 - 1].SendMeasure(sckt);
		fixed_light		  [opt2 - 1].SendMeasure(sckt);
	}
	else if (opt1 == 2){
		mobile_temperature	 [opt2 - 1].SendMeasure(sckt);
		mobile_heartbeat	 [opt2 - 1].SendMeasure(sckt);
		mobile_heartpressure [opt2 - 1].SendMeasure(sckt);
		mobile_breath		 [opt2 - 1].SendMeasure(sckt);
	}
}