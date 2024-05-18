#include "server.hpp"
#include "client.hpp"

void    Server::set_cout(int _cout)
{
	cout = _cout;
}

std::string const Server::get_port() const
{
	return (this->_port);
}

std::string const Server::get_address() const
{
	return (this->_address);
}

std::string const Server::get_pass() const
{
	return (this->_pass);
}

Server::Server(std::string port, std::string pass)
{
	_port = port;
	_pass = pass;
	_address = "127.0.0.1";
	socketfile = -1;
	in_port = atoi(_port.c_str());
}

void Server::CreateSock()
{
	socketfile = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfile == -1)
	{
		std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
		exit(1);
	}
	std::cout << "Socket created: " << socketfile << std::endl;
}

void Server::BindSocket()
{
	struct sockaddr_in add;
	int en = 1;
	memset(&add, 0, sizeof(add));
	add.sin_family = AF_INET;
	add.sin_port = htons(in_port);
	add.sin_addr.s_addr = inet_addr(_address.c_str());
	if (setsockopt(socketfile, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
	{
		std::cerr << "Error in setsockopt: " << strerror(errno) << std::endl;
		exit(-1);
	}

	if (fcntl(socketfile, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Error in fcntl: " << strerror(errno) << std::endl;
		exit(-2);
	}

	if (bind(socketfile, (struct sockaddr*)&add, sizeof(add)) == -1)
	{
		std::cerr << "Error in bind: " << strerror(errno) << std::endl;
		exit(2);
	}
	std::cout << "Socket bound successfully" << std::endl;
}

void Server::ListenSocket()
{
	if (listen(socketfile, SOMAXCONN) == -1)
	{
		std::cerr << "Error in listen: " << strerror(errno) << std::endl;
		exit(3);
	}
	std::cout << "Listening on socket" << std::endl;
}

void Server::AcceptConnection()
{
	struct sockaddr_in add;
	socklen_t len = sizeof(add);
	acceptsocket = accept(socketfile, (struct sockaddr*)&add, &len);
	if (acceptsocket == -1)
	{
		std::cerr << "Error in accept: " << strerror(errno) << std::endl;
		return;
	}
	std::cout << "Accepted connection: " << acceptsocket << std::endl;
	if (fcntl(acceptsocket, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Error in fcntl: " << strerror(errno) << std::endl;
		return;
	}

	epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = acceptsocket;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, acceptsocket, &ev) == -1)
		std::cerr << "Error in epoll_ctl: " << strerror(errno) << std::endl;
}

void Server::HandleEvent(int fd, Server &sev, client &user)
{

	char buff[1024];
	memset(buff, 0, sizeof(buff));
	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0);
	if (bytes <= 0)
	{
		std::cout << "Client <" << fd << "> Disconnected" << std::endl;
		close(fd);
	}
	else
	{
		std::cout << "Received data: " << sizeof(buff) << "      " << fd << "    "<< buff << std::endl;
		if(sev.cout == 0)
		{
			if (user.check_input(buff, fd, sev) == 1)
				return ;
			else
				memset(buff, 0, sizeof(buff));
		}
		else
			user.check_cmd(fd, buff);
	}
	
}

void Server::InitServer(Server &sev)
{
	//int index = 0;
	CreateSock();
	BindSocket();
	ListenSocket();
	
	client user;
	user.init_all(); /// intialize all parameters to 0


	epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
	{
		std::cerr << "Error in epoll_create1: " << strerror(errno) << std::endl;
		exit(4);
	}
	std::cout << "Epoll created" << std::endl;

	epoll_event event;
	sev.cout = 0;
	event.events = EPOLLIN;
	event.data.fd = socketfile;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socketfile, &event) == -1)
	{
		std::cerr << "Error in epoll_ctl: " << strerror(errno) << std::endl;
		exit(5);
	}

	while (1)
	{
		epoll_event events[10];
		int num_events = epoll_wait(epoll_fd, events, 10, -1);
		if (num_events == -1)
		{
			std::cerr << "Error in epoll_wait: " << strerror(errno) << std::endl;
			exit(6);
		}

		for (int i = 0; i < num_events; ++i)
		{
			if (events[i].data.fd == socketfile)
			{
				AcceptConnection();
				// if (index < MAX_CLIENTS)
				// 	index++;
			}
			else if (events[i].events & EPOLLIN)
				HandleEvent(events[i].data.fd, sev, user);
		}
	}
}
