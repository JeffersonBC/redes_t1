all: server clients

server: server.cpp
	gcc server.cpp -o server

clients: clients.cpp sensors.h
	gcc clients.cpp -o clients -I. -lm -lstdc++
