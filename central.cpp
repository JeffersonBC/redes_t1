#include <iostream>
#include <iomanip>
#include <string>

#include <client-server.h>
#include <virtual_sensors.h>


using namespace std;

void InitializeSensors(VirtualSensor (&fixed_sensors)[4], VirtualSensor (&mobile_sensors)[4]);

void Menu(int sckt, VirtualSensor (&fixed_sensors)[4], VirtualSensor (&mobile_sensors)[4]);
void Live(int sckt, VirtualSensor (&fixed_sensors)[4], VirtualSensor (&mobile_sensors)[4]);

void ReadMeasures(int sckt);
void ReadMeasuresCompact(int sckt);


int main(int argc, char const *argv[]){
	// Inicializa descriptor do socket de cliente
	int sckt;
	InitClient(sckt);

	// Formata a saída de números
	cout << fixed;
	cout << setprecision(2);

	// Inicializa os sensores virtuais
	VirtualSensor fixed_sensors[4];
	VirtualSensor mobile_sensors[4];
	InitializeSensors(fixed_sensors, mobile_sensors);


	// Se o primeiro argumento na execução da central for "live", usa interface automática
	string live_flag;
	if (argc > 1){
		live_flag = argv[1];

		if (live_flag == "live")
			Live(sckt, fixed_sensors, mobile_sensors); // Interface automática
		else
			Menu(sckt, fixed_sensors, mobile_sensors); // Interface interativa
	}
	else
		Menu(sckt, fixed_sensors, mobile_sensors); // Interface interativa


	return 0;
}

// Interface para o usuário digitar qual sensor ele quer checar
void Menu(int sckt, VirtualSensor (&fixed_sensors)[4], VirtualSensor (&mobile_sensors)[4]){
	int menu1, menu2;

	do {
		cout << "Selecione uma das opcoes abaixo:\n1. Sensores fixos\n2. Sensores moveis\n0. Sair\n:";

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
				cout << "Opcao invalida. ";
			}
		}

		// Sensores móveis
		else if (menu1 == 2){
			cout << "2.1. Piloto\n2.2. Co-piloto\n2.3. Aeromoca 1\n2.4. Aeromoca 2\n2.5. Todos\n:";

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
				cout << "Opcao invalida. ";
			}
		}

		else if (menu1 == 0){
			cout << "Desligando a central...\n";
			break;
		}

		else {
			cout << "Opcao invalida. ";
		}
	}
	while (menu1 != 0);

	// Envia requisição para encerrar servidor
	menu1 = -1;
	menu2 = 0;

	send(sckt, &menu1, sizeof(menu1), 0);
	send(sckt, &menu2, sizeof(menu2), 0);
}

// Interface na qual todos os sensores são mostrados e atualizados automaticamente até
// encerrar o programa
void Live(int sckt, VirtualSensor (&fixed_sensors)[4], VirtualSensor (&mobile_sensors)[4]){
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

// Inicializa os sensores virtuais
void InitializeSensors(VirtualSensor (&fixed_sensors)[4], VirtualSensor (&mobile_sensors)[4]){
	fixed_sensors[0].name = "Cabine de piloto";
	fixed_sensors[1].name = "Cabine de passageiros";
	fixed_sensors[2].name = "Compartimento de bagagem";
	fixed_sensors[3].name = "Banheiro";

	mobile_sensors[0].name = "Piloto";
	mobile_sensors[1].name = "Co-piloto";
	mobile_sensors[2].name = "Aeromoca 1";
	mobile_sensors[3].name = "Aeromoca 2";
}

// Lê valores, nomes e unidades de medida de um sensor virtual e imprime na tela
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

// Lê valores, nomes e unidades de medida de um sensor virtual, e imprime de maneira compacta
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
