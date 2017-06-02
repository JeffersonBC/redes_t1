all: server clients

server: server.cpp sensors.h
	gcc server.cpp -o server -I. -lm -lstdc++

clients: clients.cpp sensors.h
	gcc clients.cpp -o clients -I. -lm -lstdc++
