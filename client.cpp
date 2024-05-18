#include "server.hpp"
#include "client.hpp"

client::client()
{
	_username = "user_" + std::to_string(rand() % 10000);
	_nickname[_index] = "nick_" + std::to_string(rand() % 10000);
	_realname = "Unknown";
	_port = "0";
	_address = "127.0.0.1";
}

client::~client(){	
}

std::string client::get_nickname(int index)
{
	return (_nickname[index]);
}

int client::get_index()
{
	return (_index);
}

void client::set_index(int index)
{
	_index = index;
}

void client::set_fd(int fd, int index)
{
	_fd[index] = fd;
}

int client::get_fd(int index)
{
	return (_fd[index]);
}

std::string client::get_username()
{
	return (_username);
}

std::string client::get_realname()
{
	return (_realname);
}

std::string client::get_address()
{
	return (_address);
}

std::string client::get_port()
{
	return (_port);
}

void	client::set_username(std::string username)
{
	_username = username;
}

void	client::set_nickname(std::string nickname, int index)
{
	_nickname[index] = nickname;
}


void	client::set_realname(std::string realname)
{
	_realname = realname;
}

void	client::set_address(std::string address)
{
	_address = address;
}

void	client::set_port(std::string port)
{
	_port = port;
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

void	client::check_cmd(int fd, std::string input)
{
	char buffer[MAX_BUFFER_SIZE];
	memset(buffer, 0, sizeof(buffer));
	size_t start = skep_space(input, 0);
	size_t end = start;
	while(input.length() > 0 && input[end] != '\0' && input[end] != ' ' && input[end] != '\n' && end < input.length())
		end++;
	if (input.substr(start, end) == "/nick")
	{
		putstr_fd(fd, "Plase enter you'r new nickname\n");
		recv(fd, buffer, MAX_BUFFER_SIZE, 0);
		set_nickname(buffer, _index);
		putstr_fd(fd, "Your nickname has been changed successfully to: ");
		putstr_fd(fd, get_nickname(_index));
		//putstr_fd(fd, "\n");
	}
	else if (input.substr(start, end) == "/user")
	{
		putstr_fd(fd, "Plase enter you'r new username\n");
		recv(fd, buffer, MAX_BUFFER_SIZE, 0);
		set_username(buffer);
		putstr_fd(fd, "Your username has been changed successfully to: ");
		putstr_fd(fd, get_username());
		//putstr_fd(fd, "\n");
	}
	else if (input.substr(start, end) == "/realname")
	{
		putstr_fd(fd, "Plase enter you'r new realname\n");
		recv(fd, buffer, MAX_BUFFER_SIZE, 0);
		set_realname(buffer);
		putstr_fd(fd, "Your realname has been changed successfully to: ");
		putstr_fd(fd, get_realname());
		//putstr_fd(fd, "\n");
	}
	else if (input.substr(start, end) == "/quit")
	{
		putstr_fd(fd, "You have been disconnected from the server successfully\n");
		std::cout << "A client has been disconnected" << std::endl;
		close(fd);
	}
	else if (input.substr(start, end) == "/help")
		help(fd);
	else if (input.substr(start, end) == "/connect")
		putstr_fd(fd, "You are already connected to the server\n");
	else if (input.substr(start, end) == "PRIVMSG") ///----- still need to be implemented--------------------
	{
		putstr_fd(fd, "Please enter the nickname of the user you want to send a message to\n");
		recv(fd, buffer, MAX_BUFFER_SIZE, 0);
		putstr_fd(fd, "Please enter the message you want to send\n");
		recv(fd, buffer, MAX_BUFFER_SIZE, 0);
	}
	else
		putstr_fd(fd, "Invalid command, use /help for more information\n");
}

int client::check_input(std::string input, int fd, Server &sev)
{
	// parse input and check if it is a valid command
	size_t start = skep_space(input, 0);
	size_t end = start;
	while(input.length() > 0 && input[end] != '\0' && input[end] != ' ' && input[end] != '\n' && end < input.length())
		end++;
	if (input.substr(start, end) == "/help")
	{
		help(fd);
		return (1);
	}
	if (input.substr(start, end) == "/connect" && check_ip_port(fd , input.substr(end, input.length() - end -1), sev) == 0)
	{
		sev.set_cout(1);
		send(fd, "You have been connected to the server successfully :) \n", 56, 0);
		std::cout << "A new client is connected" << std::endl;
		send(fd, "Please enter a command to continue...\n", 39, 0);
		return (0);
	}
	else
	{
		send(fd, "Connect to the server first, use /help for more information \n", 62, 0);
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

// void	client::parse_cmd(int fd, Server &sev)
// {
// 	char buffer[MAX_BUFFER_SIZE];
// 	memset(buffer, 0, sizeof(buffer));
// 	recv(fd, buffer, MAX_BUFFER_SIZE, 0);
// 	while (check_input(buffer, fd, sev) == 1)
// 	{
// 		memset(buffer, 0, sizeof(buffer));
// 		recv(fd, buffer, MAX_BUFFER_SIZE, 0);
// 	}
// 	set_index(fd - 3);
// 	memset(buffer, 0, sizeof(buffer));
// 	recv(fd, buffer, MAX_BUFFER_SIZE, 0);
// 	check_cmd(fd, buffer);
// }

void	client::parse_cmd(int fd, Server &sev)
{
	char buffer[MAX_BUFFER_SIZE];
	memset(buffer, 0, sizeof(buffer));
	recv(fd, buffer, MAX_BUFFER_SIZE, 0);
	while (check_input(buffer, fd, sev) == 1)
	{
		memset(buffer, 0, sizeof(buffer));
		recv(fd, buffer, MAX_BUFFER_SIZE, 0);
	}
	set_index(fd - 3);
	memset(buffer, 0, sizeof(buffer));
	recv(fd, buffer, MAX_BUFFER_SIZE, 0);
	check_cmd(fd, buffer);
}

// clients need to be listed in a vector and the server should be able to send messages to all clients

