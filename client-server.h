#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <string.h>

#define PORT 8080


using namespace std;

void InitClient(int &sckt) {
	struct sockaddr_in address;
	struct sockaddr_in serv_addr;

	int err;

	// Inicializa socket
	do {
		err = (sckt = socket(AF_INET, SOCK_STREAM, 0));
		if (err < 0){
			cout << "Erro na criação do socket. Tentando novamente.\n";
			sleep(2);
		}
	}
	while (err < 0);

	// Configura o socket para usar a porta 8080 no endereço local
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		perror("Endereço inválido/ não suportado.\n");
		exit(EXIT_FAILURE);
	}

	// Tenta conectar com o servidor
	do {
		err = connect(sckt, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
		if (err < 0){
			cout << "Conexão com o servidor falhou. Tentando novamente...\n";
			sleep(3);
		}
	}
	while(err < 0);

	cout << "Conexão com os sensores estabelecida!\n\n";
}

void InitServer(int &sckt) {
	int server_fd;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	// Descriptor do socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	cout << "Aguardando conexão da central...\n";
	if ((sckt = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
		perror("accept failed");
		exit(EXIT_FAILURE);
	}
	cout << "Conexão com central estabelecida com sucesso!\n\n";
}