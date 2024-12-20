# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/08 13:13:47 by sbouheni          #+#    #+#              #
#    Updated: 2024/10/31 13:46:11 by joakoeni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME					=	webserv

CXX						=	c++ -Wall -Wextra -Werror -std=c++98 -g

INCLUDE_DIR				=	include/
SRCS_DIR				=	source
OBJ_DIR					=	obj

SRCS					=	main.cpp											\
							parser/ConfigFile.cpp								\
							parser/ConfigExtractor.cpp							\
							server/Client.cpp									\
							server/HttpParsingRequest.cpp						\
							server/Server.cpp									\
							server/ServerHandler.cpp							\
							server/Location.cpp									\
							utils/utils.cpp										\
							block/Block.cpp										\
							block/BlockFactory.cpp								\
							block/MainBlock.cpp									\
							block/ServerBlock.cpp								\
							block/LocationBlock.cpp								\
							directive/DirectiveFactory.cpp						\
							directive/Directive.cpp								\
							directive/ListenDirective.cpp						\
							directive/ServerNameDirective.cpp					\
							directive/ClientMaxBodySizeDirective.cpp			\
							directive/ErrorPageDirective.cpp					\
							directive/RootDirective.cpp							\
							directive/IndexDirective.cpp						\
							directive/AutoIndexDirective.cpp					\
							directive/DenyDirective.cpp							\
							directive/ReturnDirective.cpp						\

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
