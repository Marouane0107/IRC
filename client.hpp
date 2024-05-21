#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "server.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <errno.h>

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
		std::string _save[MAX_CLIENTS];
		int		_flag[MAX_CLIENTS];
		int			socketfile;
        std::string _port;
        std::string _address;
	public:
		client();
		~client();
		std::string	get_realname(int index);
		std::string	get_username(int index);
		std::string	get_nickname(int index);
		std::string	get_save(int index);
        std::string	get_port();
        std::string	get_address();
		int		get_socketfile();
		int		get_fd(int index);
		int		get_flag(int index);
		int		get_fd_by_nickname(std::string nickname);
		int		get_index_client(int fd);


		void	set_client(int fd);
		void	set_save(std::string save, int index);
		void	set_nickname(std::string nickname, int index);
		void	set_realname(std::string realname, int index);
		void	set_username(std::string username, int index);
		void	set_port(std::string port);
		void	set_flag(int flag, int index);
		void	set_address(std::string address);
		void	init_all(Server &sev);
		void	delete_client(int index);
		int		check_input(std::string input, int fd, Server &sev);
		void	check_cmd(int fd, std::string input);
		int		check_if_aviable(std::string input, std::string *list);
		int		send_file(int fd, int fd_recv, std::string file);
};

//-------------------------------------------------------------+ bot
void	help_to_connect(int fd);
void	welcome_message(int fd);
void    welcome_user_channel(int fd, std::string channel);
void    goodbye_user_channel(int fd, std::string channel);
void	help(int fd);
void	goodbye_message(int fd);
//-------------------------------------------------------------+

int check_ip_port(int fd, std::string input, Server &sev);
std::string	ft_itos(int value); // Convert int to string
std::string	get_str_no_space(std::string input);
std::string	one_by_one(std::string input, size_t *start);
std::string	get_param(std::string input, int param_number, int flag);
int			param_count(std::string input);
int			get_len_no_space(std::string input, size_t start);
int			skep_space(std::string input, size_t start);
void		putstr_fd(int fd, std::string str);

#endif
