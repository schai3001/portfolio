NAME	= cub3D


CC		= gcc
CFLAGS	= -Werror -Wextra -Wall -g #-fsanitize=address

MLX_PATH	= minilibx-linux/
MLX_NAME	= libmlx.a
MLX 		= $(MLX_PATH)$(MLX_NAME)


LIBFT_PATH	= libft/
LIBFT_NAME	= libft.a
LIBFT		= $(LIBFT_PATH)$(LIBFT_NAME)


SRC_PATH = ./src/
SRC		= 	main.c \
			init_data.c \
			check_args.c \
			create_map.c \
			free.c \
			parse_data.c \
			utils.c \
			get_file_data.c \
			get_color_textures.c \
			check_map.c \
			check_textures.c \
			init_mlx.c \
			init_textures.c \
			input.c	\
			raycast.c \
			texture.c \
			move_player.c \
			init_player.c \
			dda.c \
			frame.c \
			rotate.c \
			check_position.c \
			check_walls.c \
			minimap.c \
			minimap_frame.c \
			check_wall2.c



SRCS	= $(addprefix $(SRC_PATH), $(SRC))


OBJ_PATH	= ./objects/
OBJ			= $(SRC:.c=.o)
OBJS		= $(addprefix $(OBJ_PATH), $(OBJ))


INC			=	-I ./includes/\
				-I ./libft/\
				-I ./minilibx-linux/


all: $(OBJ_PATH) $(MLX) $(LIBFT) $(NAME)


$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)


$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)


$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(INC) $(LIBFT) $(MLX) -lXext -lX11 -lm


$(LIBFT):
	make -sC $(LIBFT_PATH)

$(MLX):
	make -sC $(MLX_PATH)


clean:
	rm -rf $(OBJ_PATH)
	make -C $(LIBFT_PATH) clean
	make -C $(MLX_PATH) clean


fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_PATH) fclean
	make -C $(MLX_PATH) clean


re: fclean all

.PHONY: all re clean fclean bonus