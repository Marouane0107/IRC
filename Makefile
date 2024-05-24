NAME = ircserv

SRC = src/server/main.cpp src/client/client.cpp src/server/server.cpp src/client/utils.cpp src/commands/bot.cpp \
	src/commands/check_cmd.cpp src/commands/cmd.cpp src/channels/channel.cpp src/commands/commands.cpp \
	src/commands/join.cpp src/commands/MODE.cpp src/channels/Global.cpp src/commands/update.cpp

OBJ = $(SRC:.cpp=.o)

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3 

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re