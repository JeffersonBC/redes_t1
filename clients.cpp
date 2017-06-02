#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sensors.h>

#define PORT 8080

using namespace std;

int init_socket(int &sckt);


int main(int argc, char const *argv[]){
	int sckt;
	init_socket(sckt);

	// Sensores fixos
	PressureSensor 		fixed_pressure[3];
	TemperatureSensor 	fixed_temperature[3];
	HumiditySensor 		fixed_humidity[3];
	LightSensor			fixed_light[3];

	for (int i = 0; i < 3; i++){
		fixed_pressure[i].SendMeasure(sckt);
		fixed_temperature[i].SendMeasure(sckt);
		fixed_humidity[i].SendMeasure(sckt);
		fixed_light[i].SendMeasure(sckt);
	}

	return 0;
}


int init_socket(int &sckt) {
	struct sockaddr_in address;
	struct sockaddr_in serv_addr;

	if ((sckt = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Erro na criação do socket.\n");
		return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("Endereço inválido/ não suportado.\n");
		return -1;
	}

	if (connect(sckt, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("Conexão com o servidor falhou.\n");
		return -1;
	}

}