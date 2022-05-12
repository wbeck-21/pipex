NAME	= pipex
CC 		= gcc
FLAGS 	= -Wall -Wextra -Werror
HEAD	= pipex.h
LIB		= ./libft/libft.a
INC		= -I ./libft

SRCS 	= pipex.c
OBJS 	= ${SRCS:.c=.o}

RM		= rm -rf

all: 		${NAME}

${NAME}:	${OBJS}
			@echo "\n\033[0;33mCompiling...\n...\n...\n"
			@make -s -C libft
			@${CC} ${FLAGS} -o ${NAME} $^ ${LIB}
			@echo "\n\033[1;32mPipex compiled! \(★ω★)/\n"

%.o:		%.c $(HEAD)
			@${CC} ${FLAGS} ${INC} -c $< -o $@

clean:
			@make clean -C libft
			@${RM} -f ${OBJS}

fclean: 	clean
			@make fclean -C libft
			@${RM} -f ${NAME}
			@echo "\n\033[1;32mDeleting DONE! (＃￣ω￣)\n"


re:			fclean all

.PHONY: all clean fclean re