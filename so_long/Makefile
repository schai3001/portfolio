NAME			= so_long
CC				= gcc
FLAGS			= -Wall -Wextra -Werror
RM				= rm -rf
MLX				= minilibx-linux/Makefile.gen
LFT				= ft_printf/libftprintf.a
INCLUDE			= -I ./inc -I ./ft_printf/include -I ./mlx
LIB				= ft_printf/libftprintf.a -L ./mlx -lmlx -lXext -lX11
OBJ				= $(patsubst src%, obj%, $(SRC:.c=.o))
SRC				=	src/animation.c \
					src/check_map.c \
					src/check_map2.c \
					src/check_path_collect.c \
					src/check_path_collect2.c \
					src/check_path_exit.c \
					src/check_path_exit2.c \
					src/count_in_map.c \
					src/defeat_screen.c \
					src/find_player.c \
					src/free.c \
					src/init_game.c \
					src/init_game2.c \
					src/load_player_image.c \
					src/load_wall_img.c \
					src/put_img.c \
					src/put_img2.c \
					src/put_img3.c \
					src/put.c \
					src/read_file.c \
					src/so_long.c \
					src/victory_screen.c


all:			$(MLX) $(LFT) obj $(NAME)


$(NAME):		$(OBJ)
					$(CC) $(FLAGS) -o $@ $^ $(LIB)
				

$(MLX):
					@echo " [ .. ] | Compilling minilibx.."
					@make -s -C mlx
					@echo " [ OK ] | Minilibx ready !"

$(LFT):
					@echo " [ .. ] | Compilling libft.."
					@make -s -C ft_printf
					@echo " [ OK ] | Libft ready !"

obj:
					@mkdir -p obj

obj/%.o:		src/%.c
					$(CC) $(FLAGS) $(INCLUDE) -o $@ -c $<

clean:
					@make -s $@ -C ft_printf
					$(RM) $(OBJ) obj
					@echo "object files removed"

fclean:			clean
					@make -s $@ -C ft_printf
					$(RM) $(NAME)
					@echo "binary files removed"

re:				fclean all


.PHONY:			all clean fclean re
