client: client.cpp server
	g++  client.cpp -o client

server: server.cpp
	g++ server.cpp -o server
clean: 
	rm *.o a.out
