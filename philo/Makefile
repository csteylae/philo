NAME=philo

CC= cc
CFLAGS= -Wall -Wextra -Werror -g -pthread -fsanitize=thread
DEBUG_FLAG= -fsanitize=thread

SRCS= src/main.c \
	  src/get_rules.c \
	  src/init_simulation.c \
	  src/utils.c \
	  src/philo_cycle.c \
	  src/monitoring.c \
	  src/terminate_sim.c \

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
	  
valgrind : $(NAME)
	valgrind --tool=helgrind ./$(NAME)
valgrind_drd : $(NAME)
	valgrind --tool=drd ./$(NAME)
