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
		int _index;
		int _fd[MAX_CLIENTS];
		std::string _username;
		std::string _nickname[MAX_CLIENTS];
		std::string _realname;
        std::string _port;
        std::string _address;
	public:
		client();
		~client();
		std::string	get_realname();
		std::string	get_username();
		std::string	get_nickname(int index);
        std::string	get_port();
        std::string	get_address();
		int		get_index();
		void	set_index(int index);
		void	set_fd(int fd, int index);
		int		get_fd(int index);
		void	set_nickname(std::string nickname, int index);
		void	set_realname(std::string realname);
		void	set_username(std::string username);
		void	set_port(std::string port);
		void	set_address(std::string address);
		void	parse_cmd(int fd, Server &sev);
		int		check_input(std::string input, int fd, Server &sev);
		void	help(int fd);
		void	check_cmd(int fd, std::string input);
};

int		skep_space(std::string input, size_t start);
void	putstr_fd(int fd, std::string str);

#endif
