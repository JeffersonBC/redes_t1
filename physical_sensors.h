#include <cstdlib>
#include <cmath>
#include <ctime>
#include <string>

using namespace std;

class PhysicalSensor {
	protected:
		string name;
		string unit;
	public:
		float bias;

		// Retorna uma medição aleatória
		virtual double Measure() = 0;

		// Envia valor, nome e unidade de medida do sensor físico
		void SendMeasure(int sckt){
			const char* local_name = name.c_str();
			const char* local_unit = unit.c_str();
			double m = Measure();

			send(sckt, &m, sizeof(m), 0);
			send(sckt, local_name, 128, 0);
			send(sckt, local_unit, 8, 0);

			printf("Sent: %f %s %s\n", Measure(), local_name, local_unit);
		}

		string SensorName(){return name;}
		string SensorUnit(){return unit;}
};

class PressureSensor : public PhysicalSensor {
	public:
		PressureSensor (){
			name = "Pressão";
			unit = "Pa";
			bias = 0;
		}

		double Measure(){
			float r = rand() % 1000;
			return (bias + 1.0f + 0.0001f * sin(r)) * 101325.0f; // Pressão aleatória em Pascal
		}
};

class TemperatureSensor : public PhysicalSensor {
	public:
		TemperatureSensor (){
			name = "Temperatura";
			unit = "ºC";
			bias = 0;
		}
		double Measure(){
			float r = rand() % 1000;
			return bias + 24.0f + 2.0f * sin(r); // Temperatura aleatória em graus celcius
		}
};

class HumiditySensor : public PhysicalSensor {
	public:
		HumiditySensor(){
			name = "Humidade";
			unit = "\%";
			bias = 0;
		}
		double Measure(){
			float r = rand() % 1000;
			return bias + 30.0f + 10.0f * sin(r); // Humidade relativa aleatória em %
		}
};

class LightSensor : public PhysicalSensor {
	public:
		LightSensor(){
			name = "Luminosidade";
			unit = "lm";
			bias = 0;
		}

		double Measure(){
			float r = rand() % 1000;
			return bias + 1200.0f + 100.0f * sin(r); // Luminosidade aleatória em Lumens
		}
};

class HeartbeatSensor : public PhysicalSensor {
	public:
		HeartbeatSensor(){
			name = "Batimentos";
			unit = "bpm";
			bias = 0;
		}

		double Measure(){
			float r = rand() % 1000;
			return bias + 90.0f + 10.0f * sin(r); // Frequência cardiaca aleatória em bpm
		}
};

class BreathSensor : public PhysicalSensor {
	public:
		BreathSensor(){
			name = "Respiracao";
			unit = "rpm";
			bias = 0;
		}

		double Measure(){
			float r = rand() % 1000;
			return bias + 18.0f + 2.0f * sin(r); // Frequência respiratória aleatória em rpm
		}
};

class HeartpressureSensor : public PhysicalSensor {
	public:
		HeartpressureSensor(){
			name = "Pressao Card";
			unit = "mm hg";
			bias = 0;
		}

		double Measure(){
			float r = rand() % 1000;
			return bias + 127.5f + 2.5f * sin(r); // Pressão diastólica aleatória em mm hg
		}
};