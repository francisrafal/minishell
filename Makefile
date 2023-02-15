NAME			:= minishell
CC				:= cc
RM		    	:= rm -f
FLAGS			:= -Wall -Wextra -Werror
DEBUG			:= -g

HEADDIR			:= ./includes/
HEADLIST		:= minishell.h
HEADERS			:= $(addprefix ${HEADDIR}, ${HEADLIST})

SRCSDIR			:= ./src/
SRCSLIST		:= main.c \
					prompt.c \
					builtins.c \
					builtin_utils.c \
					list_utils.c \
					split_input.c \
					split_input_utils.c \
					error.c \
					env.c \
					init.c \
					exec.c \
					free.c \
					files_utils.c \
					signals.c
SRCS			:= $(addprefix ${SRCSDIR}, ${SRCSLIST})

OBJSDIR			:= ./obj/
OBJSLIST		:= ${SRCSLIST:.c=.o}
OBJS			:= $(addprefix ${OBJSDIR}, ${OBJSLIST})

LIBFTDIR		:= ./libft/
LIBFT			:= ${LIBFTDIR}libft.a

LIBS			:= -L${LIBFTDIR} -lft -lreadline
INCS			:= -I${HEADDIR} -I${LIBFTDIR} 

CYAN			:= \033[0;36m
RESET			:= \033[0m

${NAME}:		${LIBFT} ${OBJSDIR} ${OBJS}
				@echo ""
				@echo "${CYAN}Compiling ${NAME} ...${RESET}"
				${CC} ${FLAGS} ${DEBUG} ${OBJS} -o ${NAME} ${LIBS} ${INCS}
				@echo ""
				@echo "${CYAN}${NAME} Created${RESET}"

${LIBFT}:
				make -C ${LIBFTDIR}

${OBJSDIR}%.o:	${SRCSDIR}%.c ${HEADERS} compiling
				${CC} ${FLAGS} ${DEBUG} ${INCS} -c $< -o $@

${OBJSDIR}:
				mkdir -p ${OBJSDIR}

.PHONY:			all clean fclean re test 

.INTERMEDIATE:	compiling

compiling:
				@echo ""
				@echo "${CYAN}Compiling Objects ...${RESET}"

all:			${NAME}

clean:
				@echo ""
				@echo "${CYAN}Deleting ${NAME} Objects ...${RESET}"
				${RM} -r ${OBJSDIR}
				make -C ${LIBFTDIR} clean

fclean:			clean
				@echo ""
				@echo "${CYAN}Deleting ${NAME} Executable ...${RESET}"
				${RM} ${NAME}
				make -C ${LIBFTDIR} fclean

re:				fclean all

test:			all
				./${NAME} 

val:			all
				valgrind \
				--leak-check=full \
				--track-fds=yes \
				--show-leak-kinds=all \
				--suppressions=.ignore_readline_leaks \
				./${NAME}
