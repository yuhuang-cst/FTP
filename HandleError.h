#ifndef HANDLEERROR_H
#define HANDLEERROR_H

#include <iostream>
#include <cstring>
using namespace std;

//用于报告错误
class HandleError{
public:
	static const int initial_socket_error;
	static const int connect_error;
	static const int receive_error;
	static const int bind_error;
	static const int getsockname_error;
	static const int listen_error;
	static const int accept_error;
	static const int create_thread_error;
    static const int file_not_found_error;
	
	static void handleError(string flag, int error_num);
private:
	
};


#endif
