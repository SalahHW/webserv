# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/08 13:13:47 by sbouheni          #+#    #+#              #
#    Updated: 2024/10/02 10:45:34 by sbouheni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME					=	webserv

CXX						=	c++ -Wall -Wextra -Werror -std=c++98 -g

INCLUDE_DIR				=	include/
SRCS_DIR				=	source
OBJ_DIR					=	obj

SRCS					=	main.cpp											\
							parser/ConfigFile.cpp								\
							parser/ConfigValidator.cpp							\
							parser/DirectiveValidator.cpp						\
							parser/Block.cpp									\
							parser/Directive.cpp								\
							utils/utils.cpp

OBJS					=	$(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

all						:	$(NAME)

$(NAME)					:	$(OBJS)
							$(CXX) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o			:	$(SRCS_DIR)/%.cpp
							@mkdir -p $(dir $@)
							$(CXX) -I$(INCLUDE_DIR) -c $< -o $@

clean					:
							rm -rf $(OBJ_DIR)

fclean					:	clean
							rm -rf $(NAME)

re						:	fclean all

.PHONY					:	all clean fclean re
