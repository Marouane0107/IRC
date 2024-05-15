#include "client.hpp"

client::client()
{
	_nickname = "Mar_1";
	_username = "marouane";
}

client::client(std::string name, std::string nickname)
{
	this->_nickname = nickname;
	this->_username = name;
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

int	client::connect_to_server()
{
	int	client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_fd == -1){
		perror("socket");
		return (-1);
	}

	if (connect(client_fd, reinterpret_cast<struct sockaddr*>(&add), sizeof(add)) == -1)
	{
		perror("connect");
		return (-1);
	}
	return (client_fd);
}
