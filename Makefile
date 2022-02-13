# Directories #
HEADERS			= -I headers/

OBJ_DIR			= objs/
SRC_DIR			= classes/

SRCS			= Connector.cpp Listener.cpp Poller.cpp Request.cpp Response.cpp Cgi.cpp Config.cpp Context_t.cpp Server_t.cpp Location_t.cpp
# Files #
SRCS			= Connector.cpp \
                  Listener.cpp \
                  Poller.cpp \
                  Request.cpp \
                  Response.cpp \
                  Cgi.cpp \
                  Config.cpp \
                  Context_t.cpp \
                  Server_t.cpp \
                  Location_t.cpp

CFIND			= $(SRCS:%=$(SRC_DIR)%)
OFILE			= $(SRCS:%.cpp=%.o)
OBJS			= $(addprefix $(OBJ_DIR), $(OFILE))

MAIN            = main.cpp
OMAIN           = $(addprefix $(OBJ_DIR), $(MAIN:%.cpp=%.o))



# Compiler #
CC				= g++
RM				= rm -f
CFLAGS			= -g -Wall -Wextra -Werror -std=c++98

NAME			= webserv


# Rules #
all:			$(OBJ_DIR) $(NAME)

$(OBJ_DIR):
				mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o:  $(SRC_DIR)%.cpp 
				$(CC) $(HEADERS) $(CFLAGS) -c $< -o $@

$(OMAIN): $(MAIN)
				$(CC) $(HEADERS) $(CFLAGS) -c $< -o $@
		

$(NAME):		$(OBJ_DIR) $(OBJS) $(OMAIN)
				$(CC) $(HEADERS) $(CFLAGS) -o $(NAME)  $(OBJS) $(OMAIN)

clean:
				$(RM) -r $(OBJ_DIR)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re
