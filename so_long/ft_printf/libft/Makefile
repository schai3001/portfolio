NAME	= libft.a

SRCS	= ft_isalpha.c \
		ft_isdigit.c \
		ft_isalnum.c \
		ft_isascii.c \
		ft_isprint.c \
		ft_strlen.c \
		ft_memset.c \
		ft_bzero.c \
		ft_memcpy.c \
		ft_memmove.c \
		ft_strlcpy.c \
		ft_strlcat.c \
		ft_toupper.c \
		ft_tolower.c \
		ft_strchr.c \
		ft_strrchr.c \
		ft_strncmp.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_strnstr.c \
		ft_atoi.c \
		ft_calloc.c \
		ft_strdup.c \
		ft_substr.c \
		ft_strjoin.c \
		ft_strtrim.c \
		ft_split.c \
		ft_itoa.c \
		ft_strmapi.c \
		ft_striteri.c \
		ft_putchar_fd.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		get_next_line.c \
		get_next_line_utils.c


BONUS_SRCS	= ft_lstnew.c \
			ft_lstadd_front.c \
			ft_lstsize.c \
			ft_lstlast.c \
			ft_lstadd_back.c \
			ft_lstclear.c \
			ft_lstdelone.c \
			ft_lstiter.c \
			ft_lstmap.c


OBJS	= ${SRCS:%.c=%.o}

BONUS_OBJS	= ${BONUS_SRCS:%.c=%.o}

FLAGS	= -Wall -Wextra -Werror

ifdef FLAG_BONUS
 TOTAL_OBJS = $(OBJS) $(BONUS_OBJS)
else
 TOTAL_OBJS = $(OBJS)
endif

RM		= rm -f

${NAME}		: 	$(TOTAL_OBJS)
			ar rc $@ $^

%.o : %.c
	$(CC) $(FLAGS) -c $< -o $@ -I .

all:		${NAME}

bonus:		$(BONUS_OBJS)
			ar rcs $(NAME) $^

clean:
			${RM} ${OBJS} ${BONUS_OBJS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY	: all clean fclean re
