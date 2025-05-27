# ft_irc - IRC Server Implementation

![Language](https://img.shields.io/badge/Language-C%2B%2B98-blue)
![Build](https://img.shields.io/badge/Build-Makefile-green)
![School](https://img.shields.io/badge/School-1337-red)

## 📖 Overview

**ft_irc** is a comprehensive IRC (Internet Relay Chat) server implementation developed as part of the 1337 School curriculum. This project provides hands-on experience with network programming, socket communication, and the IRC protocol, built entirely from scratch using C++98.

The server supports multiple clients, channels, user authentication, and implements core IRC commands following RFC standards.

## ✨ Features

### Core Functionality
- **Multi-client Support**: Handle multiple simultaneous client connections
- **Channel Management**: Create, join, and manage IRC channels
- **User Authentication**: Secure user registration and nickname management
- **Real-time Messaging**: Instant message delivery between users and channels
- **Bot Integration**: Built-in bot functionality for enhanced user experience

### IRC Commands Implemented
- **JOIN**: Join channels
- **PART**: Leave channels  
- **PRIVMSG**: Send private messages
- **MODE**: Channel and user mode management
- **NICK**: Change nickname
- **USER**: User registration
- **And more...**

### Technical Features
- **Socket Programming**: Custom socket implementation for network communication
- **Non-blocking I/O**: Efficient handling of multiple connections
- **Protocol Compliance**: Follows IRC RFC specifications
- **Error Handling**: Robust error management and validation

## 🛠️ Technical Stack

- **Language**: C++98
- **Build System**: Makefile
- **Compiler**: g++ with strict flags (-Wall -Wextra -Werror)
- **Memory Safety**: Address sanitizer enabled for debugging
- **Networking**: BSD sockets

## 🚀 Getting Started

### Prerequisites

- C++98 compatible compiler (g++, clang++)
- Make utility
- Unix-based system (Linux, macOS)

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/Marouane0107/IRC.git
   cd IRC
   ```

2. **Build the project**
   ```bash
   make
   ```

3. **Clean build files** (optional)
   ```bash
   make clean    # Remove object files
   make fclean   # Remove object files and executable
   make re       # Clean rebuild
   ```

## 📋 Usage

### Starting the Server

```bash
./ircserv <port> <password>
```

**Parameters:**
- `<port>`: Port number (1024-65535, ports below 1024 are restricted)
- `<password>`: Server password for client authentication

**Example:**
```bash
./ircserv 6667 mypassword
```

### Connecting with IRC Clients

You can connect using any standard IRC client:

**Using netcat:**
```bash
nc localhost 6667
```

**Using popular IRC clients:**
- **irssi**: `irssi -c localhost -p 6667`
- **HexChat**: Connect to `localhost:6667`
- **WeeChat**: `/connect localhost/6667`

### Basic IRC Commands

Once connected, authenticate and start chatting:

```irc
PASS mypassword
NICK yournickname
USER yourusername 0 * :Your Real Name
JOIN #general
PRIVMSG #general :Hello, World!
```

## 📁 Project Structure

```
IRC/
├── src/
│   ├── server/           # Server core implementation
│   │   ├── main.cpp      # Entry point
│   │   └── server.cpp    # Server class
│   ├── client/           # Client handling
│   │   ├── client.cpp    # Client class
│   │   └── utils.cpp     # Utility functions
│   ├── commands/         # IRC command implementations
│   │   ├── commands.cpp  # Command dispatcher
│   │   ├── join.cpp      # JOIN command
│   │   ├── MODE.cpp      # MODE command
│   │   ├── bot.cpp       # Bot functionality
│   │   └── ...
│   └── channels/         # Channel management
│       ├── channel.cpp   # Channel class
│       └── Global.cpp    # Global channel operations
├── includes/             # Header files
│   └── server.hpp        # Main server header
├── Makefile             # Build configuration
└── README.md            # This file
```

## 🎯 Learning Objectives

This project demonstrates proficiency in:

- **Network Programming**: Socket creation, binding, listening, and connection management
- **Concurrent Programming**: Handling multiple clients simultaneously
- **Protocol Implementation**: Understanding and implementing IRC protocol standards
- **C++ Development**: Object-oriented design patterns and C++98 standard compliance
- **System Programming**: Low-level networking and system calls

## 🧪 Testing

Test the server with various scenarios:

1. **Multiple Clients**: Connect several clients simultaneously
2. **Channel Operations**: Create channels, join/leave, send messages
3. **Error Handling**: Test with invalid commands and edge cases
4. **Load Testing**: Stress test with many concurrent connections

## 🤝 Contributing

This project is part of the 1337 School curriculum. If you're a fellow student or have suggestions:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/improvement`)
3. Commit your changes (`git commit -am 'Add improvement'`)
4. Push to the branch (`git push origin feature/improvement`)
5. Open a Pull Request

## 📚 Resources

- [IRC Protocol RFC 1459](https://tools.ietf.org/html/rfc1459)
- [IRC Protocol RFC 2812](https://tools.ietf.org/html/rfc2812)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)

## 📝 License

This project is developed for educational purposes as part of the 1337 School curriculum.

## 👨‍💻 Author

**Marouane** - [Marouane0107](https://github.com/Marouane0107)

---

*Part of the 1337 School curriculum - Building the future, one project at a time! 🚀*
