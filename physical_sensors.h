#include <math.h>
#include <time.h>
#include <string>

using namespace std;

class PhysicalSensor {
	protected:
		string name;
		string unit;
	public:
		virtual double Measure() = 0;

		double DeltaTime(){
			time_t timer;
			struct tm y2k = {0};
			double seconds;

			y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
			y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

			time(&timer);

			seconds = difftime(timer, mktime(&y2k));

			return seconds;
		}

		// Envia valor, nome e unidade de medida do sensor
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
			name = "Pressao";
			unit = "Pa";
		}

		double Measure(){
			double t = DeltaTime();
			return (0.85f + 0.15f * sin(10000.0f * t)) * 101325.0f; // Pressão aleatória em Pascal
		}
};

class TemperatureSensor : public PhysicalSensor {
	public:
		TemperatureSensor (){
			name = "Temperatura";
			unit = "K";
		}
		double Measure(){
			double t = DeltaTime();
			return 297.15f + 2.0f * sin(100000.0f * t); // Temperatura aleatória em Kelvins
		}
};

class HumiditySensor : public PhysicalSensor {
	public:
		HumiditySensor(){
			name = "Humidade";
			unit = "\%";
		}
		double Measure(){
			double t = DeltaTime();
			return 25.0f + 15.0f * sin(100000.0f * t); // Humidade relativa aleatória em %
		}
};

class LightSensor : public PhysicalSensor {
	public:
		LightSensor(){
			name = "Luminosidade";
			unit = "lm";
		}

		double Measure(){
			double t = DeltaTime();
			return 1000.0f + 400.0f * sin(10000.0f * t); // Luminosidade aleatória em Lumens
		}
};