#include "Client.h"


Client::Client(){
}

Client::~Client(){
	
}
//运行客户端
void Client::start(){
    //建立控制连接
	if(!createCommandSocket()) return;
    
    //循环处理输入的命令
    bool continue_flag = true;
	while(continue_flag){
		cout << "ftp> ";
		cin.getline(command_buffer, COMMAND_BUF_SIZE);
		continue_flag = handleCommand();
	}
}

//建立控制连接
bool Client::createCommandSocket(){
	command_sockfd = createSocket();
	if(command_sockfd < 0){
		HandleError::handleError("command", HandleError::initial_socket_error);
		return false;
	}
	
	setAddr("127.0.0.1", 8001, command_sin);
	
	if(connect2Target(command_sockfd, command_sin) == -1){
		HandleError::handleError("command", HandleError::connect_error);
		return false;
	}
    cout << "build command connection successfully!" << endl;
    return true;
}

//建立数据连接
bool Client::createDataSocket(){
    //获得服务器数据连接的端口号
	if (recvCommand() < 0){
		HandleError::handleError("command", HandleError::receive_error);
		return false;
	}
	
    //建立数据连接
	data_sockfd = createSocket();
	if(data_sockfd < 0){
		HandleError::handleError("data", HandleError::initial_socket_error);
		return false;
	}
    setAddr("127.0.0.1", ntohs(*(short*)command_buffer), data_sin);
	
	if(connect2Target(data_sockfd, data_sin) == -1){
		HandleError::handleError("data", HandleError::connect_error);
		return false;
	}
    //cout << "server port: " << ntohs(*(short*)command_buffer) << ", " << "build data connection successfully!" << endl;
    return true;
}

//接收命令
inline int Client::sendCommand(){
	return write(command_sockfd, command_buffer, COMMAND_BUF_SIZE);
}

//发送命令
inline int Client::recvCommand(){
	return read(command_sockfd, command_buffer, COMMAND_BUF_SIZE);
}

//接收数据
inline int Client::sendData(){
	return write(data_sockfd, data_buffer, DATA_BUF_SIZE);
}

//发送数据
inline int Client::recvData(){
	return read(data_sockfd, data_buffer, DATA_BUF_SIZE);
}

//解析命令并处理命令，若命令为quit则返回false，否则返回true
bool Client::handleCommand(){
	split(command_buffer, ' ', command);
	if (command.empty())
		return true;
	
	if(command[0] == CLIENT_RETR_COMMAND){
		RETR_command(command[1]);
	}
	else if(command[0] == CLIENT_STOR_COMMAND){
        STOR_command(command[1]);
	}
	else if(command[0] == CLIENT_PWD_COMMAND){
        PWD_command();
	}
	else if(command[0] == CLIENT_LIST_COMMAND){
        LIST_command();
	}
	else if(command[0] == CLIENT_CWD_COMMAND){
		CWD_command(command[1]);
	}
	else if(command[0] == CLIENT_HELP_COMMAND){
		HELP_command();
	}
	else if(command[0] == CLIENT_QUIT_COMMAND){
		QUIT_command();
        return false;
	}
	else{
		cout << "unknown command" << endl;
	}
	return true;
}

//处理RETR命令
void Client::RETR_command(string file_name){
	strcpy(command_buffer, (SERVER_RETR_COMMAND + " " + file_name).c_str());
	sendCommand();
    recvCommand();//接收文件是否存在标记
    if (command_buffer[0] == FILE_EXIST){
        if(!createDataSocket()) return;
	    recv_file(data_sockfd, file_name, data_buffer, DATA_BUF_SIZE);
        close(data_sockfd);
    }
    else{
        HandleError::handleError("", HandleError::file_not_found_error);
    }
}

//处理STOR命令
void Client::STOR_command(string file_name){
    if(!file_exist("./", file_name)) {
        HandleError::handleError("", HandleError::file_not_found_error);
        return;
    }
	strcpy(command_buffer, (SERVER_STOR_COMMAND + " " + file_name).c_str());
	sendCommand();
    if(!createDataSocket()) return;
    send_file(data_sockfd, "./" + file_name, data_buffer, DATA_BUF_SIZE);
    close(data_sockfd);
}

//处理PWD命令
void Client::PWD_command(){
	strcpy(command_buffer, SERVER_PWD_COMMAND.c_str());
	sendCommand();
    recvCommand();
    cout << command_buffer << endl;
}

//处理LIST命令
void Client::LIST_command(){
	strcpy(command_buffer, SERVER_LIST_COMMAND.c_str());
	sendCommand();
    recvCommand();
    cout << command_buffer;
}

//处理CWD命令
void Client::CWD_command(string path){
	strcpy(command_buffer, (SERVER_CWD_COMMAND + " " + path).c_str());
    sendCommand();
    recvCommand();
    cout << command_buffer;
}

//处理HELP命令
void Client::HELP_command(){
	cout << CLIENT_RETR_COMMAND << endl;
    cout << CLIENT_STOR_COMMAND << endl;
    cout << CLIENT_PWD_COMMAND << endl;
    cout << CLIENT_LIST_COMMAND << endl;
    cout << CLIENT_CWD_COMMAND << endl;
    cout << CLIENT_HELP_COMMAND << endl;
    cout << CLIENT_QUIT_COMMAND << endl;
}

//处理QUIT命令
void Client::QUIT_command(){
    strcpy(command_buffer, SERVER_QUIT_COMMAND.c_str());
    sendCommand();
	close(command_sockfd);
}
