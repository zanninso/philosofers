CC = gcc
NAME = philosophers
CFLAGS = -Wall -Wextra -Werror
FILES = src/forks.o src/main.o src/philo_actions.o src/tools.o

all:$(NAME)

$(NAME): $(FILES)
	$(CC) -pthread -g $(FILES) -o   $(NAME)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(FILES)

fclean: clean
	rm -rf $(NAME)

re: fclean all 