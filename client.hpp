#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "server.hpp"

#include <iostream>
#include <string>

const int MAX_CLIENTS = 100; // Maximum number of clients
const int MAX_BUFFER_SIZE = 1024; // Maximum size of buffer for receiving messages

class client
{
	private:
		std::string _username;
		std::string _nickname;
        std::string _port;
        std::string _address;
	public:
		client();
		client(std::string name, std::string nickname, std::string port, std::string address);
		~client();
		std::string	get_username();
		std::string	get_nickname();
        std::string get_port();
        std::string get_address();
		int	connect_to_server(client cli);
};


#endif
