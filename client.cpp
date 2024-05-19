#include "server.hpp"
#include "client.hpp"


client::client()
{
	_port = "0";
	_address = "127.0.0.1";
}

client::~client(){	
}

std::string client::get_nickname(int index)
{
	return (_nickname[index]);
}

int client::get_fd(int index)
{
	return (_fd[index]);
}

std::string client::get_username(int index)
{
	return (_username[index]);
}

std::string client::get_realname(int index)
{
	return (_realname[index]);
}

std::string client::get_address()
{
	return (_address);
}

std::string client::get_port()
{
	return (_port);
}

void	client::set_username(std::string username, int index)
{
	_username[index] = username;
}

void	client::set_nickname(std::string nickname, int index)
{
	_nickname[index] = nickname;
}


void	client::set_realname(std::string realname, int index)
{
	_realname[index] = realname;
}

void	client::set_address(std::string address)
{
	_address = address;
}

void	client::set_port(std::string port)
{
	_port = port;
}

void	client::set_client(int fd)
{
	int index = 0;
	while (index < MAX_CLIENTS)
	{
		if (_fd[index] <= 0 )
		{
			_fd[index] = fd;
			_username[index] = "user_";
			_nickname[index] = "usernick_";
			_realname[index] = "Unknown";
			return ;
		}
		index++;
	}
}

int check_ip_port(int fd, std::string input, Server &sev)
{
	size_t i = 0;
	size_t start = 0;
	size_t end = 0;
	std::string ip = sev.get_address();
	std::string port = sev.get_port();
	std::string pass = sev.get_pass();
	while (end < input.length() && input[end] != '\0' && input[end] != '\n')
	{
		start = skep_space(input, start);
		end = start;
		while(input[end] != '\0' && input[end] != ' ' && input[end] != '\n' && end < input.length())
			end++;
		if (i == 0)
		{
			if(input.substr(start, ip.length()) != ip)
			{
				send(fd, "Invalid ip / \n", 15, 0);
				return (1);
			}
		}
		if (i == 1)
		{
			if(input.substr(start, port.length()) != port)
			{
				send(fd, "Invalid port / \n", 17, 0);
				return (1);
			}
		}
		if (i == 2)
		{
			if (input.substr(start, pass.length()) != pass)
			{
				send(fd, "Invalid password / \n", 21, 0);
				return (1);
			}
		}
		start = end;
		i++;
	}
	start = skep_space(input, start);
	if (i != 3 && start < input.length())
		return (1);
	return (0);
}

int client::get_index_client(int fd)
{
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (fd == _fd[i])
			return (i);
	}
	return (-1);
}

int client::get_fd_by_nickname(std::string nickname)
{
	nickname = get_str_no_space(nickname);
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (nickname == _nickname[i])
			return (_fd[i]);
	}
	return (-1);
}

void	client::check_cmd(int fd, std::string input)
{
	int index = get_index_client(fd);
	if (index == -1)
	{
		putstr_fd(fd, "You are not connected to the server, use /help for more information\n");
		return ;
	}
	char buffer[MAX_BUFFER_SIZE];
	bzero(buffer, sizeof(buffer));
	size_t start = skep_space(input, 0);
	size_t end = start;
	while(input.length() > 0 && input[end] != '\0' && input[end] != ' ' && input[end] != '\n' && end < input.length())
		end++;
	if (input.substr(start, end) == "/nick" && !is_one_param(input)) ///// ----------- /nick <nickname> ------------------------
	{
		putstr_fd(fd, "Plase enter you'r nickname\n");
		ft_recv(fd, buffer);
		while (check_if_aviable(buffer, _nickname))
		{
			if (!*buffer)
				putstr_fd(fd, "Invalid nickname, please enter a valid one\n");
			else
				putstr_fd(fd, "This nickname is already taken, please choose another one\n");
			ft_recv(fd, buffer);
		}
		set_nickname(get_str_no_space(buffer), index);
		putstr_fd(fd, "Your nickname has been set successfully to: ");
		putstr_fd(fd, get_nickname(index));
	}
	else if (input.substr(start, end) == "/user" && !is_one_param(input)) ////// --------- /user <username> -------------------
	{
		putstr_fd(fd, "Plase enter you'r new username\n");
		ft_recv(fd, buffer);
		while (check_if_aviable(buffer, _username))
		{
			if (!*buffer)
				putstr_fd(fd, "Invalid username, please enter a valid one\n");
			else
				putstr_fd(fd, "This username is already taken, please choose another one\n");
			ft_recv(fd, buffer);
		}
		set_username(buffer, index);
		putstr_fd(fd, "Your username has been changed successfully to: ");
		putstr_fd(fd, get_username(index));
	}
	else if (input.substr(start, end) == "/realname" && !is_one_param(input)) ////// --------- /realname <realname> --------------------recv need to be fixed !!
	{
		putstr_fd(fd, "Plase enter you'r new realname\n");
		ft_recv(fd, buffer);
		while (check_if_aviable(buffer, _realname))
		{
			if (!*buffer)
				putstr_fd(fd, "Invalid realname, please enter a valid one\n");
			else
				putstr_fd(fd, "This realname is already taken, please choose another one\n");
			ft_recv(fd, buffer);
		}
		set_realname(buffer, index);
		putstr_fd(fd, "Your realname has been changed successfully to: ");
		putstr_fd(fd, get_realname(index));
	}
	else if (input.substr(start, end) == "/quit" && !is_one_param(input)) ////// --------- /quit --------------------
	{
		putstr_fd(fd, "You have been disconnected from the server successfully\n");
		std::cout << "A client has been disconnected" << std::endl;
		close(fd);
	}
	else if (input.substr(start, end) == "/help" && !is_one_param(input)) ////// --------- /help --------------------
		help(fd);
	else if (input.substr(start, end) == "/connect")
		putstr_fd(fd, "You are already connected to the server\n");
	else if (input.substr(start, end) == "PRIVMSG") ////// --------- PRIVMSG <nickname> <message> --------------------recv need to be fixed !!
	{
		putstr_fd(fd, "Please enter the nickname of the user you want to send a message to\n");
		ft_recv(fd, buffer);
		while (get_fd_by_nickname(buffer) == -1)
		{
			putstr_fd(fd, "This user is not exist, please enter a valid nickname\n");
			ft_recv(fd, buffer);
		}
		memset(buffer, 0, sizeof(buffer));
		putstr_fd(fd, "Please enter the message you want to send\n");
		ft_recv(fd, buffer);
		send(get_fd_by_nickname(buffer), buffer, strlen(buffer), 0);
	}
	else
		putstr_fd(fd, "Invalid command, use /help for more information\n");
}

int client::check_input(std::string input, int fd, Server &sev)
{
	// parse input and check if it is a valid command to connecte to the server-------------------------------------------------- connect
	size_t start = skep_space(input, 0);
	size_t end = start;
	while(input.length() > 0 && input[end] != '\0' && input[end] != ' ' && input[end] != '\n' && end < input.length())
		end++;
	if (input.substr(start, end) == "/help" && !is_one_param(input))
	{
		help(fd);
		return (1);
	}
	if (input.substr(start, end) == "/connect" && check_ip_port(fd , input.substr(end, input.length() - end -1), sev) == 0)
	{
		send(fd, "You have been connected to the server successfully :) \n", 56, 0);
		std::cout << "A new client is connected" << std::endl;
		set_client(fd);
		send(fd, "Please enter a command to continue...\n", 39, 0);
		return (0);
	}
	else
	{
		putstr_fd(fd, "Connect to the server first, use /help for more information \n");
		return (1);
	}
	return (0);
}

void	client::help(int fd)
{
	putstr_fd(fd, "+----------------| List of available commands: |---------------+\n");
	putstr_fd(fd, "| /connect <address> <port> <password> - Connect to the server |\n");
	putstr_fd(fd, "| /nick <nickname> - Set your nickname                         |\n");
	putstr_fd(fd, "| /user <username> - Set your username                         |\n");
	putstr_fd(fd, "| /realname <realname> - Set your realname                     |\n");
	putstr_fd(fd, "| /quit - Disconnect from the server                           |\n");
	putstr_fd(fd, "| PRIVMSG <nickname> <message> - Send a private message        |\n");
	putstr_fd(fd, "| /help - Display this help message                            |\n");
	putstr_fd(fd, "+--------------------------------------------------------------+\n");
}

void	client::delete_client(int index)
{
	_fd[index] = -1;
	_nickname[index] = "";
	_username[index] = "";
	_realname[index] = "";
}

void	client::init_all()
{
	memset(_fd, -2, sizeof(_fd));
	memset(_nickname, 0, sizeof(_nickname));
	memset(_username, 0, sizeof(_username));
	memset(_realname, 0, sizeof(_realname));
}

int		client::check_if_aviable(std::string input, std::string* list)
{
	size_t i = 0;
	std::string name;
	name = get_str_no_space(input);
	while (i < MAX_CLIENTS)
	{
		if (name == list[i] && _fd[i] > 0)
			return (1);
		i++;
	}
	return (0);
}
