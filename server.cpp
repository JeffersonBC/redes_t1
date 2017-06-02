#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include <string>
//#include <stdio.h>
//#include <stdlib.h>

#include <sensors.h>

#define PORT 8080

using namespace std;

int init_server(int &new_socket);

int main(int argc, char const *argv[]){
	int valread;
	int new_socket;

	init_server(new_socket);

	VirtualSensor sensors[3];
	sensors[0].name = "Cabine de piloto";
	sensors[1].name = "Cabine de passageiros";
	sensors[2].name = "Compartimento de bagagem";

	double m;
	for (int i = 0; i < 3; i++){
		cout << sensors[i].name << "\n";
		sensors[i].ReadMeasure(new_socket);
	}

	return 0;
}

int init_server(int &new_socket){
	int server_fd;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	// Socket descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt failed");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	return 0;
}