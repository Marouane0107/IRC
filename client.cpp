#include "client.hpp"

client::client()
{
	_nickname = "Mar_1";
	_username = "marouane";
}

client::client(std::string name, std::string nickname, std::string port, std::string address)
{
	this->_nickname = nickname;
	this->_username = name;
    this->_port = port;
    this->_address = address;
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

int client::connect_to_server(client cli)
{
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        return -1;
    }
    else
    {
        std::cout << "CLient accept\n";
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(std::stoi(cli._port));
    if (inet_pton(AF_INET, cli._address.c_str(), &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(client_fd);
        return -1;
    }
    else
        std::cout << "inet_pton work\n";

    if (connect(client_fd, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr)) == -1) {
        perror("connect");
        close(client_fd);
        return -1;
    }
    else
        std::cout << "client connect\n";

    return client_fd;
}
