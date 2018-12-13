# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aollivie <aollivie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/11/14 17:07:06 by aollivie          #+#    #+#              #
#    Updated: 2018/12/11 07:44:17 by alex             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


DEBUG=yes
CC=clang

ifeq ($(DEBUG), yes)
				CFLAGS=  -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -O1 -g #-fsanitize=address -fno-omit-frame-pointer # -g -fsanitize=address,undefined # -g -ansi -pedantic
else
		CFLAGS=  -Wall -Wextra -Werror
endif

NAME_PING = ft_ping

# NAME_CLIENT = client

LIBFT =libft/libft.a



I_DIR= -I inc/  -I libft/inc

ifeq ($(LANG),fr)
	I_DIR= -I inc/ -I./libft/inc
endif

O_DIR= obj

MKDIR = mkdir

NEW_SRC = main.c packet.c check.c display.c send.c receve.c init.c tool.c
# PING_SRC = create_client.c main.c get_addr.c

# VPATH= .:src:src/ping
VPATH= .:src:src/new

# OBJS= $(PING_SRC:%.c=$(O_DIR)/%.o)
OBJS= $(NEW_SRC:%.c=$(O_DIR)/%.o)

.PHONY : all clean fclean re

all :
	make -C libft
	make -j $(NAME_PING)


ifeq ($(DEBUG),yes)
				@echo "Generation mode debug"
else
				@echo "Generation mode release"
endif

$(NAME_PING):$(OBJS)
				$(CC) $(CFLAGS) $(I_DIR) $^ $(LIBFT) -o $@

$(O_DIR)/%.o: %.c
				$(CC) $(CFLAGS) $(I_DIR) -c $< -o $@

$(OBJS): | $(O_DIR)

$(O_DIR):
				$(MKDIR) $(O_DIR)
clean :
		rm -rf $(O_DIR)
		make clean -C libft

fclean : clean
		@rm -rf $(NAME_PING)
		make fclean -C libft

re : fclean all
