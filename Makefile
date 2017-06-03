all: central sensores

central: central.cpp virtual_sensors.h client-server.h
	gcc central.cpp -o central -I. -lstdc++ -lm

sensores: sensores.cpp physical_sensors.h client-server.h
	gcc sensores.cpp -o sensores -I. -lstdc++ -lm
