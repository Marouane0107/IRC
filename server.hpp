#ifndef SERVER_HPP
#define SERVER_HPP

#include "client.hpp"

#include <cstdlib>
#include <iostream>
#include <csignal> 
#include <string>
#include <sstream>
#include <cstring>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <algorithm>

class client;

class Server
{
	private:
		std::string _port;
		std::string _pass;
		std::string _address;
		int socketfile;
		int acceptsocket;
		int epoll_fd;
		int in_port;
	public:
		int    get_socketfile() const;
		std::string const get_port() const;
		std::string const get_address() const;
		std::string const get_pass() const;
		Server(std::string port, std::string pass);
		void InitServer(Server &sev);
		void CreateSock();
		void BindSocket();
		void ListenSocket();
		void AcceptConnection();
		void HandleEvent(int fd, Server &sev, client &user);
};

#endif
