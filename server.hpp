#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

class Server
{
    private:
        std::string _port;
        std::string _pass;
        std::string _address;
        int socketfile;
        int acceptsocket;
        int epoll_fd;
        int in_port;
        int cout;
    public:
        void    set_cout(int _cout);
        std::string const get_port() const;
        std::string const get_address() const;
        std::string const get_pass() const;
        Server(std::string port, std::string pass);
        void InitServer(Server &sev);
        void CreateSock();
        void BindSocket();
        void ListenSocket();
        void AcceptConnection();
        void HandleEvent(int fd, Server &sev);
};

#endif // SERVER_HPP
