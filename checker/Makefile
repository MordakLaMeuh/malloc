NAME = sodo
CC = gcc

### MAIN FLAGS ###

ifeq ($(DEBUG),yes)
	CFLAGS = -std=c99 -Wextra -Wall -Werror -g -O0 -fsanitize=address
else
	CFLAGS = -std=c99 -Wextra -Wall -Werror -Ofast
endif

### LIBRAIRIES ###

LIB_DIR = ..
_LIBFT = libft
LIBFT = $(addprefix $(LIB_DIR)/, $(_LIBFT))

### SOURCES ###

MAIN = sodo deep tool

SRC_LIST = $(MAIN)

VPATH = ./

## HEADERS

_HEADERS = sodo.h

SRC = $(addsuffix .c, $(SRC_LIST))
OBJ_DIR = .
__OBJ__ = $(basename $(notdir $(SRC)))
OBJ = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(__OBJ__)))
__H__ = $(basename $(notdir $(_HEADERS)))
HEADERS = $(addsuffix .h, $(__H__))

IFLAGS = -I$(LIBFT)/includes -I$(LIBFT)/srcs
LDFLAGS = -L ../ -lft_malloc -L$(LIBFT) -lft

.PHONY: all clean fclean re help

all: top_level_rebuild_libft $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c $(HEADERS) Makefile
	$(CC) -c $(CFLAGS) -o $@ $< $(IFLAGS)

clean:
	rm -f $(OBJ)

fclean:
	rm -f $(OBJ)
	rm -f $(NAME)

re: fclean all

top_level_rebuild_libft:
	make -C $(LIBFT)/ all DEBUG=$(DEBUG)

help:
	@echo
	@echo "Programm $(NAME)"
	@echo
	@echo "--------------------------------------------------------------------------"
	@echo " Disp rules."
	@echo
	@echo " all     : Compile the program $(NAME) into $(BINDIR) directory."
	@echo " re      : Recompile all objets of the programm."
	@echo " clean   : Remove objects."
	@echo " fclean  : Remove objects and programm."
	@echo " help    : Display this."
	@echo "--------------------------------------------------------------------------"