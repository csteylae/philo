NAME=philo

CC= cc
CFLAGS= -Wall -Wextra -Werror -pthread -g -fsanitize=address
DEBUG_FLAG= -fsanitize=thread

SRCS= src/main.c \
	  src/get_rules.c \
	  src/init_simulation.c \
	  src/utils.c \

OBJS = $(SRCS:.c=.o)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm  -rf $(NAME)

re: fclean
	make $(NAME)

debug: CFLAGS += $(DEBUG_FLAG)
debug: re
	  
VALGRIND : $(NAME)
	valgrind --tool=helgrind ./$(NAME)
valgrind_drd : $(NAME)
	valgrind --tool=drd ./$(NAME)
