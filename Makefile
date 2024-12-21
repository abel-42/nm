
NAME = ft_nm

# LDFLAGS = -Llibft

# LDLIBS = -lft -lmlx -framework OpenGL -framework AppKit

CPPFLAGS = -Iincludes

# LFT = libft/libft.a

# LFT_DIR = libft

OBJS_DIR = .objs

SRCS_DIR = srcs

CC = gcc

CFLAGS = -Wall -Wextra -Werror

DFLAGS = -g3 -fsanitize=address

LSFLAG = -fsanitize=leak

SRCS = main.c \
       ft_getopt_r.c \
	   lib/ft_memcmp.c \
	   lib/ft_memset.c \
	   lib/ft_putnbr_base.c \
	   lib/ft_putstr.c \
	   lib/ft_sort.c \
	   lib/ft_strchr.c \
	   lib/ft_strcmp.c \
	   lib/ft_strlen.c

OBJS = $(SRCS:.c=.o)

OBJS_PATH = $(addprefix $(OBJS_DIR)/, $(OBJS))

SRCS_PATH = $(addprefix $(SRCS_DIR)/, $(SRCS))


.PHONY: all clean re fclean compile

DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CFLAGS += $(DFLAGS)
	LDFLAGS += $(DFLAGS)
endif

LEAKS ?= 0
ifeq ($(LEAKS), 1)
	CFLAGS += $(LSFLAG)
	LDFLAGS += $(LSFLAG)
endif

all: compile

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(NAME): $(MLX_OSX) $(LFT) $(OBJS_PATH)
	@$(CC) $(LDFLAGS) $(LDLIBS) -o $(NAME) $(OBJS_PATH)
	@echo $(NAME) "done"

$(LFT):
	make -C $(LFT_DIR)

$(MLX_OSX):
	make -C $(MLX_OSX_DIR)

clean:
	@echo "Removing" $(NAME) "object files"
	@/bin/rm -f $(OBJS_PATH)
	@rmdir $(OBJS_DIR) 2> /dev/null || true
	@make -C $(LFT_DIR) clean

fclean:
	@echo "Removing" $(NAME) "object files"
	@/bin/rm -f $(OBJS_PATH)
	@rmdir $(OBJS_DIR) 2> /dev/null || true
	@echo "Removing" $(NAME)
	@/bin/rm -f $(NAME)
	@make -C $(LFT_DIR) fclean
	@make -C $(MLX_OSX_DIR) clean

re: fclean all

run: all
	./$(NAME)

compile:
	@echo "Compiling" $(NAME)
	@make $(NAME)
