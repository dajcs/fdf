# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/15 13:40:53 by anemet            #+#    #+#              #
#    Updated: 2025/07/20 23:26:23 by anemet           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ----------- Variables --------------- #

# Executable name
NAME = fdf

# Compiler and general flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Source files for fdf
SRCS = fdf.c \
       fdf_map.c \
	   fdf_alt.c \
       fdf_view.c \
       fdf_color.c \
       fdf_utils.c \
       fdf_handle.c \
       fdf_mouse.c

# Object files for fdf
OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))

# Cleanup command
RM = rm -f


# ------------------- Directories ---------------- #

# Libft directory
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

# MiniLibX directory
MLX_DIR = ./minilibx-linux
MLX_LIB = $(MLX_DIR)/libmlx.a


# ------------------ Compiler/Linker Flags ---------- #

# Include paths for headers (-I)
INCLUDES = -I. -I$(LIBFT_DIR) -I$(MLX_DIR)

# Library paths (-L) and library names (-l)
LDFLAGS = -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -lXext -lX11 -lm


# ------------------- Rules ------------------------ #

# Default rule
all: $(NAME)

# Rule to create the final executable
$(NAME): $(OBJS) $(LIBFT) $(MLX_LIB)
	@echo "Linking $@..."
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)
	@echo "$@ compiled successfully!"

# Rule to build the libft library
$(LIBFT):
	@echo "Compiling libft..."
	@$(MAKE) -s -C $(LIBFT_DIR)

# Rule to build the MiniLibX library
$(MLX_LIB):
	@echo "Compiling MiniLibX..."
	@$(MAKE) -s -C $(MLX_DIR)

# Rule to compile the fdf source files into object files
# %.o: %.c fdf.h
# 	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# $@ - inside a recipe line it expands to the exact filename of the current
#                                             target that the rule is building
# $< - the first prerequisite (handy in single-source compile rules,
#                                                            e.g. the .c file)
# $^ - the full list of prerequisites (deduplicated).
# $? - only the prerequisites that are newer than the target.

# pattern rule - target path + order-only dep on $(OBJ_DIR)
$(OBJ_DIR)/%.o: %.c fdf.h | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# make sure obj/ exists before compiling
# `| $(OBJ_DIR)` order-only prerequisite, `mkdir -p obj` once, no needless rbld
$(OBJ_DIR):
	@mkdir -p $@

# Rule to remove object files
clean:
	@echo "Cleaning libft..."
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@echo "Cleaning MiniLibX..."
	@$(MAKE) -s -C $(MLX_DIR) clean
	@echo "Removing $(OBJ_DIR) directory..."
	@$(RM) -r $(OBJ_DIR)

# Rule to remove object files and the final executable
fclean: clean
	@echo "Fcleaning libft..."
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@echo "Cleaning MiniLibX..."
	@$(MAKE) -s -C $(MLX_DIR) clean
	@echo "Removing executable..."
	@$(RM) $(NAME)

# Rule to re-compile the project from scratch
re: fclean all

# Phony targets are rules that do not produce a file
.PHONY: all clean fclean re
