NAME = myOwnVector
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
CPP_EXTENSION = .cpp

#		눈_눈			SOURCES			눈_눈

FILES 	=	ArrayException main
SRCS_FILES	+=	$(FILES)

SRCS_FILES_EXT 		+= 	$(addsuffix $(CPP_EXTENSION), $(SRCS_FILES))
SRCS 				+= 	$(SRCS_FILES_EXT)
OBJS 				= 	$(SRCS_FILES_EXT:cpp=o)

#		(҂◡_◡)			UTILS			(҂◡_◡)

RM = rm -rf

#		(｡◕‿◕｡)			COLORS			(｡◕‿◕｡)

NONE = \033[0m
HIRED = \033[31m
HIGREEN = \033[92m
HIBLUE = \033[94m
CURSIVE = \033[3m

#		( ಠ ʖ̯ ಠ)		RULES			( ಠ ʖ̯ ಠ)

all: $(NAME)

$(NAME):  $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

.c.o:
	@$(CC) $(FLAGS) -c $< -o ${<:.cpp=.o} 


clean:
	@$(RM) -r $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.phony: all clean fclean re
