NAME = ft_containers
CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -g3
LIBLINKS	=	-I./includes
SRC_PATH	=	srcs/
OBJ_PATH	=	bin/
CPP_EXTENSION = .cpp

#		눈_눈			SOURCES			눈_눈

MAIN_PATH	=	
MAIN_FILES	=	stack_tests_ft #main chrono
SRCS_FILES	+=	$(addprefix $(MAIN_PATH), $(MAIN_FILES))

SRCS_FILES_EXT	+=	$(addsuffix $(CPP_EXTENSION), $(SRCS_FILES))
SRCS			+=	$(addprefix $(SRC_PATH), $(SRCS_FILES_EXT))
OBJS			=	$(addprefix $(OBJ_PATH), $(SRCS_FILES_EXT:cpp=o))

#		(҂◡_◡)			UTILS			(҂◡_◡)

RM	=	rm -rf

#		(｡◕‿◕｡)			COLORS			(｡◕‿◕｡)

NONE = \033[0m
HIRED = \033[31m
HIGREEN = \033[92m
HIBLUE = \033[94m
CURSIVE = \033[3m

#		( ಠ ʖ̯ ಠ)		RULES			( ಠ ʖ̯ ಠ)

all: $(NAME)

$(NAME):  $(OBJS)
	@$(CC) $(FLAGS) -o $(NAME) $(OBJS) -fsanitize=address

.c.o:
	@$(CC) $(FLAGS) $(LIBLINKS) -c $< -o ${<:.cpp=.o}

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@mkdir -p $(dir $@)
	${CC} ${FLAGS} ${LIBLINKS} -c $< -o $@

clean:
	@$(RM) -rf $(OBJ_PATH)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.phony: all clean fclean re
