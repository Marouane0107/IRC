NAME = ircserv

SRC = main.cpp client.cpp server.cpp utils.cpp bot.cpp cmd.cpp oelboukh_irc_files/channel.cpp oelboukh_irc_files/commands.cpp \
	oelboukh_irc_files/join.cpp oelboukh_irc_files/MODE.cpp oelboukh_irc_files/Global.cpp \
	oelboukh_irc_files/update.cpp 

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