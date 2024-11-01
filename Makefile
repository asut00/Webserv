# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asuteau <asuteau@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/02 12:33:11 by thelaine          #+#    #+#              #
#    Updated: 2024/10/02 18:15:44 by asuteau          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ---------------------------------- VARS ------------------------------------ #

NAME			=	webserv

SRC	=	Webserv.cpp \
					Utils.cpp \
					RequestSendUtils.cpp\
					ManageSessionCookie.cpp\
					InitHttpStatusCodes.cpp \
					Request_read.cpp \
					Request_handle.cpp \
					Request_send.cpp \
					ErrResponse.cpp \
					PostData.cpp \
					Server.cpp \
					Server_utils.cpp \
					WebservManager.cpp \
					CGIhandler.cpp

VPATH			=	src/ src/Utils src/InitHttpStatusCodes src/Requests \
					src/Server src/WebservManager src/CGIhandler

INC				=	-I inc/

OBJECTS			=	$(SRC:%.cpp=$(OBJ_DIR)/%.o)

OBJ_DIR			=	build

CC				=	c++

CFLAGS			=	-Wall -Wextra -Werror -std=c++98 -g3

# ---------------------------------- RULES ----------------------------------- #

.cpp.o:
	$(CC) $(CFLAGS) -I $(INCL_DIR) -c $< -o $(<:.cpp=.o)

$(NAME): $(OBJ_DIR) $(OBJECTS)
	$(CC) $(CFLAGS) $(INC) $(OBJECTS) -o $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o:		%.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

all: $(NAME)

clean:
	rm -f $(OBJECTS)
	rm -f $(OBJECTS_BONUS)
	rm -f -r $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)

re: fclean all

.PHONY: all clean fclean re minilibx bonus
