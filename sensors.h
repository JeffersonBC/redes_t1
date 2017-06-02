#include <math.h>
#include <time.h>
#include <string>

using namespace std;

class VirtualSensor {
	public:
		string name;

		void ReadMeasure(int sckt){
			for (int j = 0; j < 4; j++){
				double m;
				int valread = read(sckt, &m, sizeof(m));
				printf("Received: %.2f\n", m);
			}
		}
};

class PhysicalSensor {
	private:
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

			time(&timer);  /* get current time; same as: timer = time(NULL)  */

			seconds = difftime(timer,mktime(&y2k));

			return seconds;
		}

		void SendMeasure(int sckt){
			printf("%f\n", Measure());

			double m = Measure();
			send(sckt, &m, sizeof(m), 0);
		}
};

class PressureSensor : public PhysicalSensor {
	public:
		double Measure(){
			double t = DeltaTime();
			return (0.85f + 0.15f * sin(10000.0f * t)) * 101325.0f; // Pressão aleatória em Pascal
		}
};

class TemperatureSensor : public PhysicalSensor {
	public:
		double Measure(){
			double t = DeltaTime();
			return 297.15f + 2.0f * sin(100000.0f * t); // Temperatura aleatória em Kelvins
		}
};

class HumiditySensor : public PhysicalSensor {
	public:
		double Measure(){
			double t = DeltaTime();
			return 25.0f + 15.0f * sin(100000.0f * t); // Humidade relativa aleatória em %
		}
};

class LightSensor : public PhysicalSensor {
	public:
		double Measure(){
			double t = DeltaTime();
			return 1000.0f + 400.0f * sin(10000.0f * t); // Luminosidade aleatória em Lumens
		}
};