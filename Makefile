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
					builtin_cd.c \
					builtin_export.c \
					builtin_exit.c \
					builtin_utils.c \
					cmd_lst_utils.c \
					check_input.c \
					check_input2.c \
					split_input.c \
					split_input_utils.c \
					files_utils.c \
					get_outfile.c \
					get_infile.c \
					prun_str.c \
					get_command.c \
					cut_char.c \
					cut_char_utils.c\
					error.c \
					env.c \
					env_lst_utils.c\
					env_lst_utils2.c\
					env_arr.c\
					init.c \
					exec.c \
					exec_pipeline.c \
					exec_single_cmd.c \
					exits.c \
					free.c \
					signals.c \
					debug_helpers.c \
					syscall_wrappers.c \
					wait_for_children.c
					
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
				--track-origins=yes \
				./${NAME}
