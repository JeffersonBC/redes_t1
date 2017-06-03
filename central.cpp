#include <iostream>
#include <iomanip>
#include <string>

#include <client-server.h>
#include <virtual_sensors.h>


using namespace std;


void menu(int sckt);
void ReadMeasure(int sckt);

VirtualSensor fixed_sensors[4];


int main(int argc, char const *argv[]){
	int sckt;
	init_client(sckt);

	cout << fixed;
    cout << setprecision(2);

	fixed_sensors[0].name = "Cabine de piloto";
	fixed_sensors[1].name = "Cabine de passageiros";
	fixed_sensors[2].name = "Compartimento de bagagem";
	fixed_sensors[3].name = "Banheiro";

	menu(sckt);

	return 0;
}


void menu(int sckt){
	int menu1, menu2;

	do {
		cout << "Selecione uma das opções abaixo:\n1. Sensores fixos\n2. Sensores moveis\n\n0. Sair\n\n";

		cin >> menu1;

		if (menu1 == 1){
			cout << "1.1. Cabine de piloto\n1.2. Cabine de passageiros\n"
				<< "1.3. Compartimento de bagagem\n1.4. Banheiro\n\n";

			cin >> menu2;

			if (menu2 >= 1 && menu2 <= 4){
				// Envia requisição
				send(sckt, &menu1, sizeof(menu1), 0);
				send(sckt, &menu2, sizeof(menu2), 0);

				// Recebe resposta dos sensores
				cout << fixed_sensors[menu2 - 1].name << '\n';
				ReadMeasure(sckt);
				cout << '\n';
			}
			else {
				cout << "\nOpcao invalida.\n";
			}
		}

		else if (menu1 == 2){

		}

		else if (menu1 == 0){
			cout << "Desligando o servidor...\n";
			break;
		}

		else {
			cout << "\nOpcao invalida.\n";
		}
	}
	while (menu != 0);
}


void ReadMeasure(int sckt){
	for (int j = 0; j < 4; j++){
		char name[128] = {0};
		char unit[8] = {0};

		double m;

		read(sckt, &m, sizeof(m));
		read(sckt, &name, 128);
		read(sckt, &unit, 8);

		cout << setw(12) << name << ":  " << m << " " << unit << '\n';
	}
}