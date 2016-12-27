all: Client_main.out Server_main.out clear

Common.o: Common.h Common.cpp
	g++ -c Common.cpp -O2
	
HandleError.o: HandleError.h HandleError.cpp
	g++ -c HandleError.cpp -O2
	
Client.o: Client.h Client.cpp
	g++ -c Client.cpp -O2

Service.o: Service.h Service.cpp
	g++ -c Service.cpp -O2

Server.o: Server.h Server.cpp
	g++ -c Server.cpp -O2

Client_main.out: Client_main.cpp Common.o HandleError.o Client.o
	g++ Client_main.cpp Common.o HandleError.o Client.o -o Client_main.out -O2

Server_main.out: Server_main.cpp Common.o HandleError.o Service.o Server.o
	g++ Server_main.cpp Common.o HandleError.o Service.o Server.o -o Server_main.out -O2 -lpthread

clear:
	rm -f *.o
