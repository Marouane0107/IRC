#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "server.hpp"

#include <string>
#include <vector>

#include <iostream>
#include <chrono>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>


const int MAX_CLIENTS = 100; // Maximum number of clients
const int MAX_BUFFER_SIZE = 1024; // Maximum size of buffer for receiving messages

class Server;
class client
{
	private:
		int _fd[MAX_CLIENTS];
		std::string _username[MAX_CLIENTS];
		std::string _nickname[MAX_CLIENTS];
		std::string _realname[MAX_CLIENTS];
        std::string _port;
        std::string _address;
	public:
		client();
		~client();
		std::string	get_realname(int index);
		std::string	get_username(int index);
		std::string	get_nickname(int index);
        std::string	get_port();
        std::string	get_address();
		int		get_fd(int index);
		int		get_fd_by_nickname(std::string nickname);
		int		get_index_client(int fd);

		void	set_client(int fd);
		void	set_nickname(std::string nickname, int index);
		void	set_realname(std::string realname, int index);
		void	set_username(std::string username, int index);
		void	set_port(std::string port);
		void	set_address(std::string address);
	//	void	parse_cmd(int fd, Server &sev);
		int		check_input(std::string input, int fd, Server &sev);
		void	init_all();
		void	delete_client(int index);
		void	help(int fd);
		void	check_cmd(int fd, std::string input);
		int		check_if_aviable(std::string input, std::string *list);
};

std::string	get_str_no_space(std::string input);
int			get_len_no_space(std::string input, size_t start);
int			skep_space(std::string input, size_t start);
void		putstr_fd(int fd, std::string str);

#endif
