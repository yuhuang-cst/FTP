#ifndef CLIENT_H
#define CLIENT_H

#include "Common.h"


class Client{
public:
	Client();
	~Client();
	void start();
	
private:
    //command
	char command_buffer[COMMAND_BUF_SIZE];
	struct sockaddr_in command_sin;
	int command_sockfd;
    vector<string> command;
	//data
	char data_buffer[DATA_BUF_SIZE];
	struct sockaddr_in data_sin;
	int data_sockfd;
	
    bool createCommandSocket();
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
	void HELP_command();
	void QUIT_command();
};



#endif