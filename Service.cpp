#include "Service.h"

Service::Service(int command_sockfd){
	this->command_sockfd = command_sockfd;
	current_path = ".";
}

Service::~Service(){
	
}

//运行服务器
void Service::start(){
    bool continue_flag = true;
	//处理传来的命令
	while(continue_flag){
		if(recvCommand() < 0){
			HandleError::handleError("data", HandleError::receive_error);
            close(command_sockfd);
			break;
		}
		cout << "recv command: " << command_buffer << endl;
		continue_flag = handleCommand();
	}
}

//建立数据连接
bool Service::createDataSocket(){
    //建立数据连接
	int listen_data_sockfd = createSocket();
	if (listen_data_sockfd < 0){
		HandleError::handleError("data", HandleError::initial_socket_error);
		return false;
	}
	setAddr(data_sin);//设置本机地址与任意可用端口号
	if(bind(listen_data_sockfd, (struct sockaddr*)&data_sin, sizeof(data_sin)) != 0){
		HandleError::handleError("data", HandleError::bind_error);
		return false;
	}
	//获得系统给的可用端口号
	socklen_t data_sin_len = sizeof(data_sin);
	if(getsockname(listen_data_sockfd, (sockaddr*)&data_sin, &data_sin_len)){
		HandleError::handleError("data", HandleError::getsockname_error);
		return false;
	}
	
	if(listen(listen_data_sockfd, 1) != 0){
		HandleError::handleError("data", HandleError::listen_error);
		return false;
	}
    
	//发送端口号
    *((short*)command_buffer) = data_sin.sin_port;
	sendCommand();
    //cout  << "server port: " << ntohs(data_sin.sin_port) << ", ";
	
	//数据连接建立
	data_sockfd = accept(listen_data_sockfd, (struct sockaddr*)&data_sin, &data_sin_len);
	if(data_sockfd < 0){
		HandleError::handleError("data", HandleError::accept_error);
		return false;
	}
    //cout << "build data connection successfully!" << endl;
    return true;
}

//发送命令
inline int Service::sendCommand(){
	return write(command_sockfd, command_buffer, COMMAND_BUF_SIZE);
}

//接收命令
inline int Service::recvCommand(){
	return read(command_sockfd, command_buffer, COMMAND_BUF_SIZE);
}

//发送数据
inline int Service::sendData(){
	return write(data_sockfd, data_buffer, DATA_BUF_SIZE);
}

//接收数据
inline int Service::recvData(){
	return read(data_sockfd, data_buffer, DATA_BUF_SIZE);
}

//解析命令并处理命令，若命令为quit则返回false，否则返回true
bool Service::handleCommand(){
	split(command_buffer, ' ', command);
	if (command.empty())
		return true;
		
	if(command[0] == SERVER_RETR_COMMAND){
		RETR_command(command[1]);
	}
	else if(command[0] == SERVER_STOR_COMMAND){
		STOR_command(command[1]);
	}
	else if(command[0] == SERVER_PWD_COMMAND){
		PWD_command();
	}
	else if(command[0] == SERVER_LIST_COMMAND){
		LIST_command();
	}
	else if(command[0] == SERVER_CWD_COMMAND){
		CWD_command(command[1]);
	}
	else if(command[0] == SERVER_QUIT_COMMAND){
		QUIT_command();
        return false;
	}
	else{
		cout << "unknown command" << endl;
	}
    return true;
}

//处理RETR命令
void Service::RETR_command(string file_name){
    if(file_exist(current_path, file_name)){
        command_buffer[0] = FILE_EXIST;
        sendCommand();
        if(!createDataSocket()) return;
        send_file(data_sockfd, current_path + "/" + file_name, data_buffer, DATA_BUF_SIZE);
        close(data_sockfd);
    }
    else{
        command_buffer[0] = FILE_NOT_EXIST;
        sendCommand();
    }
}

//处理STOR命令
void Service::STOR_command(string file_name){
    if(!createDataSocket()) return;
	recv_file(data_sockfd, current_path + "/" + file_name, data_buffer, DATA_BUF_SIZE);
    close(data_sockfd);
}

//处理PWD命令
void Service::PWD_command(){
	strcpy(command_buffer, current_path.c_str());
    sendCommand();
}

//处理LIST命令
void Service::LIST_command(){
    list_file(current_path, command_buffer, COMMAND_BUF_SIZE);
    sendCommand();
}

//处理CWD命令
void Service::CWD_command(string path){
    bool path_exist;
    current_path = cwd(current_path, path, path_exist);
	if(!path_exist){
        strcpy(command_buffer, "file doesn't exist!\n");
    }
    else{
        strcpy(command_buffer, "");
    }
    sendCommand();
}

//处理QUIT命令
void Service::QUIT_command(){
	close(command_sockfd);
    cout << "client disconnect" << endl;
}
