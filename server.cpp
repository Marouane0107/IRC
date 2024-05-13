#include "server.hpp"

void    Server::set_port(std::string const port)
{
    this->_port = port;
}

void    Server::set_address(std::string const address)
{
    this->_address = address;
}

std::string const Server::get_port()
{
    return (this->_port);
}

std::string const Server::get_address()
{
    return (this->_address);
}

Server::Server(std::string port, std::string pass)
{
    _port = port;
    _pass = pass;
    _address = "127.0.0.1";
    socketfile = -1;
    in_port = atoi(_port.c_str());
}

void    Server::CreateSock(Server &sev)
{
    sev.socketfile = socket(AF_INET, SOCK_STREAM, 0);
    if (sev.socketfile == -1)
    {
        std::cerr << "error in creating socket" << std::endl;
        exit (1);
    }
    else
        std::cout << "GOOD :)" << std::endl;
}

void    Server::BindSocket(Server &sev)
{
    struct sockaddr_in add;
    int en = 1;
    memset(&add, 0, sizeof(add));
    add.sin_family = AF_INET;
    add.sin_port = htons(sev.in_port);
    add.sin_addr.s_addr = inet_addr(sev._address.c_str());
    if(setsockopt(sev.socketfile, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
    {
        std::cout << "error in bind" << std::endl;
        exit (-1);
    }
    if (fcntl(sev.socketfile, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cout << "error in bind" << std::endl;
        exit (-2);
    }
    sev.bindsocket = bind(sev.socketfile, (struct sockaddr *) &add, sizeof(add));
    std::cout << sev.socketfile << std::endl;
    if (sev.bindsocket == -1)
    {
        perror("bind");
        exit (2);
    }
    else
        std::cout << "that good" << std::endl;
}

void    Server::lisenSocket(Server &sev)
{
    sev.lisensocket = listen(sev.socketfile, 1);
    if (sev.lisensocket == -1)
    {
        std::cout << "Error in listen" << std::endl;
        exit (3);
    }
    else
        std::cout << "That Good" << std::endl;
}

void    Server::a_new(Server &sev)
{
    struct sockaddr_in add;

    socklen_t len = sizeof(add);
    sev.acceptsocket = accept(sev.socketfile, (struct sockaddr *) &add, &len);
    if (sev.acceptsocket == -1)
    {
        std::cout << "error accepte" << std::endl;
        exit (5);
    }
    else
        std::cout << "accept is work" << std::endl;
    if (fcntl(sev.acceptsocket, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cout << "fcntl() failed" << std::endl;
        exit (7);
    }
    else
        std::cout << "fcntl() work" << std::endl;
}

void    Server::o_file(int fd)
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
        std::cout << "recv work" << std::endl;

}

void Server::initsever(Server &sev)
{
    CreateSock(sev);
    BindSocket(sev);
    lisenSocket(sev);

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(4);
    }
    else
        std::cout << "epoll is created" << std::endl;
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = sev.socketfile;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sev.socketfile, &event) == -1)
    {
        perror("epoll_ctl");
        exit(5);
    }
    while(1)
    {
        struct epoll_event events[10];
        int num_events = epoll_wait(epoll_fd, events, 10, -1);
        if (num_events == -1)
        {
            perror("epoll_wait");
            exit(6);
        }
        else
        {
            std::cout << "that good wait poll " << num_events << std::endl;
        }
        for (int i = 0; i < num_events; ++i)
        {
            if (events[i].events & EPOLLIN) { 
                if (events[i].data.fd == sev.socketfile)
                    a_new(sev);
                else
                    o_file(events[i].data.fd);
            }
        }
    }   
}

// void    Server::initsever(Server &sev)
// {
//     CreateSock(sev);
//     BindSocket(sev);
//     lisenSocket(sev);
//     fd_set rfds;
//     while(1)
//     {
//         FD_ZERO(&rfds);
//         FD_SET(sev.socketfile, &rfds);
//         int ret = select(sev.socketfile + 1, &rfds, NULL, NULL, NULL);
//         if (ret == -1)
//         {
//             std::cout << "error in select" << std::endl;
//             exit (4);
//         }
//         for (int i = 0; i <= 1024; ++i)
//         {
//             if (FD_ISSET(i, &rfds))
//             { 
//                 if (i == sev.socketfile)
//                     a_new(sev);
//             }
//         }
//     }   
// }