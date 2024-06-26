#include "../../includes/client.hpp"


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

std::string client::get_save(int index)
{
	return (_save[index]);
}

void	client::set_save(std::string save, int index)
{
	_save[index] = save;
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

void	client::set_flag(int flag, int index)
{
	_flag[index] = flag;
}

int client::get_flag(int index)
{
	return (_flag[index]);
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
	if(index == -1)
		return ;
	_fd[index] = -1;
	_nickname[index].clear();
	_username[index].clear();
	_realname[index].clear();
	_save[index].clear();
}

void	client::set_client(int fd)
{
	int index = 0;
	while (index < MAX_CLIENTS)
	{
		if (_fd[index] <= 0 )
		{
			_fd[index] = fd;
			_flag[index] = 0;
			_username[index] = "User" + ft_itos(index);
			_nickname[index] = "Nickname" + ft_itos(index);
			_realname[index] = "Unknown";
			return ;
		}
		index++;
	}
}

void	client::init_all()
{
	bzero(_flag, sizeof(_fd));
	bzero(_fd, sizeof(_fd));
	_nickname->clear();
	_username->clear();
	_realname->clear();
	_save->clear();
}

int		client::check_if_aviable(std::string input, std::string* list)
{
	size_t i = 0;
	std::string name;
	size_t start = skep_space(input, 0);
	size_t end = start;
	while((input.length() > 0 && end < input.length()) && (std::isalpha(input[end]) || std::isdigit(input[end])))
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
