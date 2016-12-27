#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include <unistd.h>
#include "Service.h"


using namespace std;

void* service_thread(void* arg);

class Server{
public:
	Server();
	~Server();
	void start();
	
private:
	int listen_command_sockfd;
	sockaddr_in command_sin;
	
	void closeSocket();
};


#endif
