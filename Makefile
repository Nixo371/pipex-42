# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nucieda- <nucieda-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/16 20:35:56 by nucieda-          #+#    #+#              #
#    Updated: 2022/02/19 02:53:58 by nucieda-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

SRC = libft/*.c \
	pipex.c \
	knife.c

BONUSSRC = 

RM = rm -f

CC = gcc

TAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME):
	$(CC) $(TAGS) -o $(NAME) $(SRC)

bonus: 
	make SRC="$(SRC) $(BONUSSRC)"

clean:
	

fclean: clean
	$(RM) $(NAME)

re: fclean all bonus
