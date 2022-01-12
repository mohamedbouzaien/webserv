HEADERS			= -I headers/

OBJ_DIR			= objs/
SRC_DIR			= classes/


SRCS			= Connector.cpp Listener.cpp Request.cpp
CFIND			= $(SRCS:%=$(SRC_DIR)%)
OFILE			= $(SRCS:%.cpp=%.o)
OBJS			= $(addprefix $(OBJ_DIR), $(OFILE))



CC				= g++
RM				= rm -f
CFLAGS			= -g -Wall -Wextra -Werror -o

NAME			= webserv


all:			$(OBJ_DIR) $(NAME)

$(OBJ_DIR):
				mkdir -p $(OBJ_DIR)

$(NAME):		$(OBJS)
				$(CC) $(HEADERS) -c $(CFLAGS) $(OBJ_DIR)main.o main.cpp
				$(CC) $(CFLAGS) $(NAME) $(HEADERS) $(OBJS) $(OBJ_DIR)main.o


$(OBJS): $(CFIND)
			make $(OFILE)

$(OFILE):
			$(CC) $(HEADERS) -c $(CFLAGS) $(OBJ_DIR)$@ $(SRC_DIR)$(@:%.o=%.cpp)

clean:
				$(RM) -r $(OBJ_DIR)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re
