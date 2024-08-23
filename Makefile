NAME = philo

SRC = philo.c utils.c philo_actions.c
SRC_OBJS = $(SRC:.c=.o)

FLAGS = -Wall -Wextra 
INCLUDES = -I ./includes

CORE_N = -D $(shell nproc -all)

$(NAME):
	gcc $(FLAGS) $(SRC) $(INCLUDES) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(SRC_OBJS)

fclean: clean
	rm -f $(NAME)

re : fclean all

.PHONY: clean all
