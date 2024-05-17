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
        int in_port;
        int socketfile;
        int bindsocket;
        int lisensocket;
        int acceptsocket;
    public:
        void    set_password(std::string const pass);
        void    set_port(std::string const port);
        void    set_address(std::string const address);
        std::string const get_port();
        std::string const get_password();
        std::string const get_address();
        Server();
        Server(std::string port, std::string pass);
        void    initsever(Server &sev);
        void    CreateSock(Server &sev);
        void    lisenSocket(Server &sev);
        void    BindSocket(Server &sev);
        void    a_new(Server &sev);
        void    o_file(int fd);
};

#endif