#include "Server.h"

Server::Server(){
	
}
Server::~Server(){
	
}
//运行服务器
void Server::start(){
    //监听指定端口
	listen_command_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_command_sockfd < 0){
		HandleError::handleError("command", HandleError::initial_socket_error);
		return;
	}
	setAddr(8001, command_sin);
	if (bind(listen_command_sockfd, (struct sockaddr*)&command_sin, sizeof(command_sin)) != 0){
		HandleError::handleError("command", HandleError::bind_error);
		return;
	}
	if(listen(listen_command_sockfd, LENGTH_OF_LISTEN_QUEUE) != 0){
		HandleError::handleError("command", HandleError::listen_error);
		return;
	}
	
    //处理连接服务器请求
	while(true){
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		int data_sockfd = accept(listen_command_sockfd, (struct sockaddr*)&client_addr, &client_addr_len);
        cout << "client connect" << endl;
		if(data_sockfd < 0){
			HandleError::handleError("command", HandleError::accept_error);
			break;
		}
        //为每个新请求创建一个线程进行处理
		pthread_t pthread_temp;
		if(pthread_create(&pthread_temp, NULL, service_thread, (void*)&data_sockfd) != 0){
			HandleError::handleError("data", HandleError::create_thread_error);
		}
	}
	closeSocket();
}
//新线程
void* service_thread(void* arg){
	int data_sockfd = *(int*)arg;
	Service service(data_sockfd);
	service.start();
}
//关闭连接
void Server::closeSocket(){
	close(listen_command_sockfd);
}