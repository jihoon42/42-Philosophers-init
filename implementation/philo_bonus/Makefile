NAME = philo_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = actions.c child_sems.c cleanup.c child.c init.c main.c monitor.c \
	parent.c parse.c print.c processes.c schedule.c sems.c sleep.c time.c \
	utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c philo_bonus.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
