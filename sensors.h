#include <math.h>
#include <time.h>

class PhysicalSensor {
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
};

class PressureSensor : public PhysicalSensor {
	public:
		double Measure(){
			double t = DeltaTime();
			return 0.75f + 0.1f * sin(1000 * t);
		}
};