#include <iostream>
#include <iomanip>
#include <string>
#include <curses.h>

#include <client-server.h>
#include <virtual_sensors.h>


using namespace std;


void Menu(int sckt);
void Live(int sckt);

void ReadMeasures(int sckt);
void ReadMeasuresCompact(int sckt);

VirtualSensor fixed_sensors[4];
VirtualSensor mobile_sensors[4];


int main(int argc, char const *argv[]){
	int sckt;
	InitClient(sckt);

	cout << fixed;
    cout << setprecision(2);

	fixed_sensors[0].name = "Cabine de piloto";
	fixed_sensors[1].name = "Cabine de passageiros";
	fixed_sensors[2].name = "Compartimento de bagagem";
	fixed_sensors[3].name = "Banheiro";

	mobile_sensors[0].name = "Piloto";
	mobile_sensors[1].name = "Co-piloto";
	mobile_sensors[2].name = "Aeromoça 1";
	mobile_sensors[3].name = "Aeromoça 2";

	//Menu(sckt);
	Live(sckt);

	return 0;
}


void Menu(int sckt){
	int menu1, menu2;

	do {
		cout << "Selecione uma das opções abaixo:\n1. Sensores fixos\n2. Sensores moveis\n0. Sair\n:";

		cin >> menu1;

		// Sensores fixos
		if (menu1 == 1){
			cout << "1.1. Cabine de piloto\n1.2. Cabine de passageiros\n"
				<< "1.3. Compartimento de bagagem\n1.4. Banheiro\n1.5. Todos\n:";

			cin >> menu2;

			if (menu2 >= 1 && menu2 <= 4){
				// Envia requisição
				send(sckt, &menu1, sizeof(menu1), 0);
				send(sckt, &menu2, sizeof(menu2), 0);

				// Recebe resposta dos sensores
				cout << fixed_sensors[menu2 - 1].name << '\n';
				ReadMeasures(sckt);
				cout << '\n';
			}
			else if (menu2 == 5){
				// Envia requisição
				send(sckt, &menu1, sizeof(menu1), 0);
				send(sckt, &menu2, sizeof(menu2), 0);

				// Recebe resposta dos sensores
				for (int i = 0; i < 4; i++){
					cout << fixed_sensors[i].name << '\n';
					ReadMeasures(sckt);
					cout << '\n';
				}
			}
			else {
				cout << "Opção inválida. ";
			}
		}

		// Sensores móveis
		else if (menu1 == 2){
			cout << "2.1. Piloto\n2.2. Co-piloto\n2.3. Aeromoça 1\n2.4. Aeromoça 2\n2.5. Todos\n:";

			cin >> menu2;

			// Um sensor virtual móvel
			if (menu2 >= 1 && menu2 <= 4){
				// Envia requisição
				send(sckt, &menu1, sizeof(menu1), 0);
				send(sckt, &menu2, sizeof(menu2), 0);

				// Recebe resposta dos sensores
				cout << mobile_sensors[menu2 - 1].name << '\n';
				ReadMeasures(sckt);
				cout << '\n';
			}
			// Todos os sensores virtuais móveis
			else if (menu2 == 5){
				// Envia requisição
				send(sckt, &menu1, sizeof(menu1), 0);
				send(sckt, &menu2, sizeof(menu2), 0);

				// Recebe resposta dos sensores
				for (int i = 0; i < 4; i++){
					cout << mobile_sensors[i].name << '\n';
					ReadMeasures(sckt);
					cout << '\n';
				}
			}
			else {
				cout << "Opção inválida. ";
			}
		}

		else if (menu1 == 0){
			cout << "Desligando a central...\n";
			break;
		}

		else {
			cout << "Opção inválida. ";
		}
	}
	while (menu1 != 0);
}

void Live(int sckt){
	while(true){
		cout << "\n\n\n";

		int menu1 = 3, menu2 = 0;

		// Envia requisição
		send(sckt, &menu1, sizeof(menu1), 0);
		send(sckt, &menu2, sizeof(menu2), 0);

		// Recebe resposta dos sensores
		for (int i = 0; i < 4; i++){
			cout << fixed_sensors[i].name << '\n';
			ReadMeasuresCompact(sckt);
			cout << "\n\n";
		}

		cout << "\n";

		for (int i = 0; i < 4; i++){
			cout << mobile_sensors[i].name << '\n';
			ReadMeasuresCompact(sckt);

			if (i < 3) cout << "\n\n";
			else cout << "\n";
		}

		sleep(3);
	}
}

// Lê valores, nomes e unidades de medida de um sensor virtual
void ReadMeasures(int sckt){
	for (int j = 0; j < 4; j++){
		char name[128] = {0};
		char unit[8] = {0};

		double m;

		read(sckt, &m, sizeof(m));
		read(sckt, &name, 128);
		read(sckt, &unit, 8);

		cout << setw(12) << name << ": " << m << " " << unit << '\n';
	}
}

void ReadMeasuresCompact(int sckt){
	for (int j = 0; j < 4; j++){
		char name[128] = {0};
		char unit[8] = {0};

		double m;

		read(sckt, &m, sizeof(m));
		read(sckt, &name, 128);
		read(sckt, &unit, 8);

		cout << name << ": " << m << " " << unit << "   ";
	}
}
