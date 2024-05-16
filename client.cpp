#include "server.hpp"
#include "client.hpp"

client::client()
{
	_username = "user_" + std::to_string(rand() % 10000);
	_nickname = "nick_" + std::to_string(rand() % 10000);
}

client::~client(){	
}

std::string client::get_nickname()
{
	return (_nickname);
}

std::string client::get_username()
{
	return (_username);
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

void	client::set_nickname(std::string nickname)
{
	_nickname = nickname;
}

void	client::set_address(std::string address)
{
	_address = address;
}

void	client::set_port(std::string port)
{
	_port = port;
}

int check_ip_port(std::string input)
{
	size_t i = 0;
	size_t start = 0;
	size_t end = 0;
	std::string ip;
	std::string port;
	while (end < input.length() && input[end] != '\0' && input[end] != '\n')
	{
		while (input.length() > 0 && input[start] == ' ' && start < input.length())
			start++;
		end = start;
		while(input.length() > 0 && input[end] != '\0' && input[end] != ' ' && input[end] != '\n' && end < input.length())
			end++;
		if (i == 0)
			ip = input.substr(start, end);
		if (i == 1)
			port = input.substr(start, end);
		start = end;
		i++;
	}
	if (i != 2)
		return (1);
	// need to check if ip and port are valid
	// ------------------------------------------------ //
	return (0);
}

int client::check_input(std::string input, int fd)
{
	// parse input and check if it is a valid command
	size_t start = 0;
	while (input.length() > 0 && input[start] == ' ' && start < input.length())
	{
		start++;
	}
	size_t end = start;
	while(input.length() > 0 && input[end] != '\0' && input[end] != ' ' && input[end] != '\n' && end < input.length())
	{
		end++;
	}
	if ( strcmp(input.substr(start, end ).c_str(), "/connect") == 0 && check_ip_port(input.substr(end, input.length())) == 0)
	{
		send(fd, "you have been connected to the server successfully :) \n", 56, 0);
		send(fd, "Please enter a command to continue...\n", 39, 0);
		return (0);
	}
	else
	{
		send(fd, "Invalid command. Please use : /connect <address> <port> - Connect to a server first :( \n", 89, 0);
		return (1);
	}
	return (0);
}

void	client::parse_cmd(int fd)
{
	char buffer[MAX_BUFFER_SIZE];
	recv(fd, buffer, MAX_BUFFER_SIZE, 0);
	check_input(buffer, fd);
}