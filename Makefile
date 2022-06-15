NAME	= pipex
CC 		= gcc
FLAGS 	= -Wall -Wextra -Werror
LIB		= ./libft/libft.a
INC		= -I ./libft

HEAD_SRCS	=	pipex.h pipex_bonus.h
HEAD_DIR	=	include/
HEAD		=	$(addprefix $(HEAD_DIR), $(HEAD_SRCS))

SRCS 	= mandatory/pipex.c mandatory/utils.c
OBJS 	= ${SRCS:.c=.o}

B_SRCS 	= bonus/pipex_bonus.c bonus/utils_bonus.c bonus/get_next_line/get_next_line.c bonus/get_next_line/get_next_line_utils.c
B_OBJS 	= ${B_SRCS:.c=.o}

RM		= rm -rf

%.o:		%.c $(HEAD)
			@${CC} ${FLAGS} ${INC} -c $< -o $@

${NAME}:	${OBJS}
			@echo "\n\033[0;33mCompiling...\n...\n...\n"
			@make -s -C libft
			@${CC} ${FLAGS} -o ${NAME} $^ ${LIB}
			@echo "\n\033[1;32mPipex compiled! \(★ω★)/\n"

all: 		${NAME}

bonus:		${B_OBJS}
			@echo "\n\033[0;33mCompiling...\n...\n...\n"
			@make -s -C libft
			@${CC} ${FLAGS} -o ${NAME} $^ ${LIB}
			@echo "\n\033[1;32mPipex compiled! \(★ω★)/\n"

clean:
			@make clean -C libft
			@${RM} -f ${OBJS}
			@${RM} -f ${B_OBJS}

fclean: 	clean
			@make fclean -C libft
			@${RM} -f ${NAME}
			@echo "\n\033[1;32mDeleting DONE! (＃￣ω￣)\n"


re:			fclean all

.PHONY: all bonus clean fclean re