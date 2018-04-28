NAME = libft_malloc
CC = gcc

### MAIN FLAGS ###

ifeq ($(DEBUG),yes)
	CFLAGS = -std=c99 -fPIC -pedantic -Wextra -Wall -Werror -g -O0 -fsanitize=address
else
	CFLAGS = -Ofast -std=c99 -fPIC -pedantic -Wextra -Wall -Werror
endif

### LIBRAIRIES ###

LIB_DIR = .
_LIBFT = libft
LIBFT = $(addprefix $(LIB_DIR)/, $(_LIBFT))

### SOURCES ###

CORE = ctor mem_syscall record index_add index_del dyn_allocator core_base core_realloc debug sectors size_conversion merge

SRC_LIST = $(CORE)

VPATH = srcs/core

## HEADERS

MAIN_HEADER = libft_alloc.h
_HEADERS = dyn_allocator

ifeq ($(HOSTTYPE),)
HOSTTYPE = $(shell uname -m)_$(shell uname -s)
endif

SRC = $(addsuffix .c, $(SRC_LIST))
OBJ_DIR = objs
__OBJ__ = $(basename $(notdir $(SRC)))
OBJ = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(__OBJ__)))
__H__ = $(basename $(notdir $(_HEADERS)))
HEADERS = $(addsuffix .h, $(__H__))

IFLAGS = -Isrcs -I$(LIBFT)/includes
LDFLAGS = -shared -L$(LIBFT) -lft

.PHONY: all clean fclean re help

all: top_level_rebuild_libft $(NAME).so

$(NAME).so: $(OBJ) libft/libft.a
	$(CC) $(CFLAGS) -o $(NAME)_$(HOSTTYPE).so $(OBJ) $(LDFLAGS)
	rm -f $(NAME).so
	ln -s libft_malloc_$(HOSTTYPE).so $(NAME).so

$(OBJ_DIR)/%.o: %.c $(HEADERS) Makefile
	$(CC) -c $(CFLAGS) -o $@ $< $(IFLAGS)

clean:
	make -C $(LIBFT)/ clean
	rm -f $(OBJ)

fclean:
	make -C $(LIBFT)/ fclean
	rm -f $(OBJ)
	rm -f $(NAME)_$(HOSTTYPE).so
	rm -f $(NAME).so

top_level_rebuild_libft:
	make -C $(LIBFT)/ all DEBUG=$(DEBUG)

re: fclean all

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
