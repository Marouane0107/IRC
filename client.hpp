#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "server.hpp"

#include <iostream>
#include <string>
#include <vector>
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
		~client();
		std::string	get_username();
		std::string	get_nickname();
        std::string get_port();
        std::string get_address();
		void	set_username(std::string username);
		void	set_nickname(std::string nickname);
		void	set_port(std::string port);
		void	set_address(std::string address);
		void	parse_cmd(int fd);
		int		check_input(std::string input, int fd);
};


#endif
