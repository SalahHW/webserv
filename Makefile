NAME					=	webserv

CXX						=	c++ -Wall -Wextra -Werror -std=c++98 -g

INCLUDE_DIR				=	include/
SRCS_DIR				=	source
OBJ_DIR					=	obj

SRCS					=	main.cpp											\
							parser/ConfigFile.cpp								\
							parser/ConfigExtractor.cpp							\
							parser/ConfigFinalizer.cpp							\
							server/Server.cpp									\
							server/ServerManager.cpp							\
							server/EventReporter.cpp							\
							server/Location.cpp									\
							server/Port.cpp										\
							server/VirtualHost.cpp								\
							server/Client.cpp									\
							server/ParseRequest.cpp								\
							utils/utils.cpp										\
							block/Block.cpp										\
							block/BlockFactory.cpp								\
							block/MainBlock.cpp									\
							block/ServerBlock.cpp								\
							block/LocationBlock.cpp								\
							block/LimitExceptBlock.cpp							\
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
							directive/DefaultServerDirective.cpp				\

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
