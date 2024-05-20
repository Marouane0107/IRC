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

int client::get_socketfile()
{
	return (socketfile);
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
			_username[index] = "User_" + ft_itos(index);
			_nickname[index] = "Nickname" + ft_itos(index);
			_realname[index] = "Unknown_" + ft_itos(index);
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
				putstr_fd(fd, "Invalid ip / \n");
				return (1);
			}
		}
		if (i == 1)
		{
			if(input.substr(start, port.length()) != port)
			{
				putstr_fd(fd, "Invalid port / \n");
				return (1);
			}
		}
		if (i == 2)
		{
			if (input.substr(start, pass.length()) != pass)
			{
				putstr_fd(fd, "Invalid password / \n");
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

void	client::delete_client(int index)
{
	_fd[index] = -1;
	_nickname[index] = "";
	_username[index] = "";
	_realname[index] = "";
}

void	client::init_all(Server &sev)
{
	socketfile = sev.get_socketfile();
	bzero(_fd, sizeof(_fd));
	_nickname->clear();
	_username->clear();
	_realname->clear();
}

int		client::check_if_aviable(std::string input, std::string* list)
{
	size_t i = 0;
	std::string name;
	size_t start = skep_space(input, 0);
	size_t end = start;
	while(input.length() > 0 && std::isalpha(input[end]) && end < input.length())
		end++;
	if (input[end] != '\0' && input[end] != ' ' && input[end] != '\n')
		return (2);
	name = get_str_no_space(input);
	if (name.empty())
		return (2);
	while (i < MAX_CLIENTS)
	{
		if (name == list[i] && _fd[i] > 0)
			return (1);
		i++;
	}
	return (0);
}
