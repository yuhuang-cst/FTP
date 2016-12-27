#include "HandleError.h"

const int HandleError::initial_socket_error = 1;
const int HandleError::connect_error = initial_socket_error + 1;
const int HandleError::receive_error = connect_error + 1;
const int HandleError::bind_error = receive_error + 1;
const int HandleError::getsockname_error = bind_error + 1;
const int HandleError::listen_error = getsockname_error + 1;
const int HandleError::accept_error = listen_error + 1;
const int HandleError::create_thread_error = accept_error + 1;
const int HandleError::file_not_found_error = create_thread_error + 1;

void HandleError::handleError(string flag, int error_num){
        if (flag != "")
		    cout << flag << ": ";
		switch(error_num){
			case initial_socket_error: 
				cout << "failed to initial socket!" << endl; 
				break;
			case connect_error: 
				cout << "failed to connect server" << endl; 
				break;
			case receive_error:
				cout << "failed to receive data" << endl;
				break;
			case bind_error: 
				cout << "failed to bind socket with address" << endl;
				break;
			case getsockname_error:
				cout << "failed to get socket name" << endl;
				break;
			case listen_error:
				cout << "failed to listen port" << endl;
				break;
			case accept_error:
				cout << "failed to accept client" << endl;
				break;
			case create_thread_error:
				cout << "failed to create thread" << endl;
				break;
            case file_not_found_error:
                cout << "file does not exist!" << endl;
                break;
			default: 
				cout << "unknown error" << endl; 
				break;
		}
	}
