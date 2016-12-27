#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <vector>
#include <fstream>
#include <arpa/inet.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "HandleError.h"

using namespace std;

#define COMMAND_BUF_SIZE 4096
#define DATA_BUF_SIZE 4096
#define LENGTH_OF_LISTEN_QUEUE 20
#define FILE_NOT_EXIST 0
#define FILE_EXIST 1

extern const string CLIENT_RETR_COMMAND;
extern const string CLIENT_STOR_COMMAND;
extern const string CLIENT_PWD_COMMAND;
extern const string CLIENT_LIST_COMMAND;
extern const string CLIENT_CWD_COMMAND;
extern const string CLIENT_HELP_COMMAND;
extern const string CLIENT_QUIT_COMMAND;

extern const string SERVER_RETR_COMMAND;
extern const string SERVER_STOR_COMMAND;
extern const string SERVER_PWD_COMMAND;
extern const string SERVER_LIST_COMMAND;
extern const string SERVER_CWD_COMMAND;
extern const string SERVER_HELP_COMMAND;
extern const string SERVER_QUIT_COMMAND;

int createSocket();

void setAddr(string IPv4_addr, short port, struct sockaddr_in &sin);

void setAddr(int port, struct sockaddr_in &sin);

void setAddr(struct sockaddr_in &sin);

int connect2Target(int sockfd, struct sockaddr_in sin);

string trim(string s, char c);

void split(string s, char sp, vector<string> &ret);

void send_file(int sockfd, string file_name, char* buffer, int buffer_size);

void recv_file(int sockfd, string file_name, char* buffer, int buffer_size);

void list_file(string current_path, char* buffer, int buffer_size);

string cwd(string current_path, string cwd_path, bool &path_exist);

bool file_exist(string current_path, string file_name);


#endif
