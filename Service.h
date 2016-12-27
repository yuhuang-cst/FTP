#ifndef SERVICE_H
#define SERVICE_H

#include "Common.h"

using namespace std;

class Service{
public:
	Service(int command_sockfd);
	~Service();
	void start();
	
private:
    //command
	char command_buffer[COMMAND_BUF_SIZE];
	int command_sockfd;
    vector<string> command;
	//data
	char data_buffer[DATA_BUF_SIZE];
	struct sockaddr_in data_sin;
	int data_sockfd;
	
	string current_path;
	
    bool createDataSocket();
	int sendCommand();
	int recvCommand();
	int sendData();
	int recvData();
    
	bool handleCommand();
	void RETR_command(string file_name);
	void STOR_command(string file_name);
	void PWD_command();
	void LIST_command();
	void CWD_command(string path);
	void QUIT_command();
};





#endif
