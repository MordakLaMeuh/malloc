NAME = libft_malloc_
CC = gcc

### MAIN FLAGS ###

ifeq ($(DEBUG),yes)
	CFLAGS = -fpic -pedantic -Wextra -Wall -Werror -g -O0 -fsanitize=address
else
	CFLAGS = -fpic -pedantic -Wextra -Wall -Werror
endif

### LIBRAIRIES ###

LIB_DIR = .
_LIBFT = libft
LIBFT = $(addprefix $(LIB_DIR)/, $(_LIBFT))

### SOURCES ###

CORE = libft_alloc

SRC_LIST = $(CORE) 

VPATH = srcs/core

## HEADERS

MAIN_HEADER = libft_alloc.h
_HEADERS =

SRC = $(addsuffix .c, $(SRC_LIST))
OBJ_DIR = objs
__OBJ__ = $(basename $(notdir $(SRC)))
OBJ = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(__OBJ__)))
__H__ = $(basename $(notdir $(_HEADERS)))
HEADERS = $(addsuffix .h, $(__H__))

IFLAGS = -Isrcs -I$(LIBFT)/includes
LDFLAGS = -shared -L$(LIBFT) -lft

.PHONY: all clean fclean re help

all: create_lib_name $(OBJ) link
	make -C $(LIBFT)/ all DEBUG=$(DEBUG)
	$(CC) $(CFLAGS) -o $(NAME)$(HOSTTYPE).so $(OBJ) $(LDFLAGS)
	@echo "done"

$(OBJ_DIR)/%.o: %.c $(HEADERS) Makefile
	$(CC) -c $(CFLAGS) -o $@ $< $(IFLAGS)

clean:
	make -C $(LIBFT)/ clean
	rm -f $(OBJ)

fclean:
	make -C $(LIBFT)/ fclean
	rm -f $(OBJ)
	rm -f $(NAME)

re: fclean all

create_lib_name:
ifeq ($(HOSTTYPE),)
HOSTTYPE = $(shell uname -m)_$(shell uname -s)
endif

link:
	rm -f libft_malloc.so
	ln -s libft_malloc_$(HOSTTYPE).so libft_malloc.so

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
