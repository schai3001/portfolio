NAME		= libftprintf.a
INCLUDE 	= include
LIBFT 		= libft
SRC_DIR		= src/
OBJ_DIR 	= obj/
CC			= gcc
FLAGS 		= -Wall -Wextra -Werror -I
RM			= rm -f
AR			= ar rcs


SRC_FILES	= ft_printfmain ft_print_cs ft_print_di ft_print_p ft_print_u ft_print_x


SRC			= $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))
OBJ			= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))


OBJF	= .cache_exists


all:	$(NAME)

$(NAME):	$(OBJ)
			@make -C $(LIBFT)
			@cp libft/libft.a .
			@mv libft.a $(NAME)
			@$(AR) $(NAME) $(OBJ)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJF)
			@$(CC) $(FLAGS) $(INCLUDE) -c $< -o $@

$(OBJF):
			@mkdir -p $(OBJ_DIR)

clean:
			@$(RM) -rf $(OBJ_DIR)
			@make clean -C $(LIBFT)

fclean:		clean
			@$(RM) -f $(NAME)
			@$(RM) -f $(LIBFT)/libft.a

re:			fclean all

.PHONY:		all clean fclean re
