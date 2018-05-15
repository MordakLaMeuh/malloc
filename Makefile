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

MAIN = ctor mem_syscall main_prototypes node_custom_allocator node_custom_deallocator free_record cmp_range cmp_fixe \
		allocator deallocator size_fn index debug_show_alloc debug_display_nodes

SRC_LIST = $(MAIN)

VPATH = srcs

## HEADERS

MAIN_HEADER = libft_alloc.h
_HEADERS = main_headers.h ctor.h

ifeq ($(HOSTTYPE),)
HOSTTYPE = $(shell uname -m)_$(shell uname -s)
endif

SRC = $(addsuffix .c, $(SRC_LIST))
OBJ_DIR = objs
__OBJ__ = $(basename $(notdir $(SRC)))
OBJ = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(__OBJ__)))
__H__ = $(basename $(notdir $(_HEADERS)))
HEADERS = $(addsuffix .h, $(__H__))

IFLAGS = -Isrcs -I$(LIBFT)/includes -I$(LIBFT)/srcs

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
LDFLAGS = -shared -fPIC -L$(LIBFT) -lft -exported_symbols_list symbol_list
endif

ifeq ($(UNAME_S),Linux)
LDFLAGS = -shared -fPIC -L$(LIBFT) -lft -Wl,--version-script=exportmap
endif

.PHONY: all clean fclean re help

all: top_level_rebuild_libft $(NAME).so

$(NAME).so: $(OBJ) libft/libft.a symbol_list
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
