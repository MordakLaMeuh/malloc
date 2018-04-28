NAME = libft.a
CC = gcc

LIBDIR	= ./
SRCDIR	= srcs
INCDIR	= includes
OBJDIR	= objs
HEADER	= libft.h

ifeq ($(DEBUG),yes)
	CFLAGS = -std=c99 -fPIC -Wextra -Wall -Werror -g -O0 -fsanitize=address -I./$(INCDIR)
else
	CFLAGS = -Ofast -std=c99 -fPIC -Wextra -Wall -Werror -I./$(INCDIR)
endif

SRC_USUALS	= ft_memset ft_bzero ft_memcpy ft_memccpy ft_memmove ft_memchr ft_memcmp \
	ft_strlen ft_strcpy ft_strncpy ft_strcat ft_strncat ft_strlcat \
	ft_strchr ft_strrchr ft_strstr ft_strnstr ft_strcmp ft_strncmp \
	ft_atoi ft_isalpha ft_isdigit ft_isalnum ft_isascii ft_isprint ft_toupper ft_tolower \
	ft_strclr ft_striter ft_striteri \
	ft_strequ ft_strnequ \
	ft_putchar ft_putstr ft_putendl ft_putnbr ft_putchar_fd ft_putstr_fd \
	ft_putendl_fd ft_putnbr_fd ft_secure_atoi \
	ft_aligned_memcpy ft_aligned_bzero

SRC_PRINTF	= beacon op_main modifiers extract_args \
	s_string s_char s_numeric_u s_numeric_i \
	s_pointer s_logical_b s_logical_o s_logical_xmin s_logical_xmaj	\
	cast buffer norme

USUALS_DIR	= usuals_tools
PRINTF_DIR  = ft_printf

SRC = 	$(addprefix $(USUALS_DIR)/, $(addsuffix .c, $(SRC_USUALS))) \
		$(addprefix $(PRINTF_DIR)/, $(addsuffix .c, $(SRC_PRINTF)))
TMP = $(basename $(notdir $(SRC)))
OBJ = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(TMP)))

.PHONY: all re clean fclean help

all: $(NAME)

$(NAME): $(OBJ) $(INCDIR)/$(HEADER)
	ar rc $(NAME) $(OBJ)
	ranlib $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/$(USUALS_DIR)/%.c $(INCDIR)/$(HEADER)
	$(CC) -c $(CFLAGS) $(LIBFT_HEADER)-o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/$(PRINTF_DIR)/%.c $(INCDIR)/$(HEADER)
	$(CC) -c $(CFLAGS) $(LIBFT_HEADER)-o $@ $<

clean:
	rm -vf $(OBJ)

fclean:
	rm -vf $(OBJ)
	rm -vf $(NAME)

re: fclean all

help:
	@echo
	@echo "Librairy $(NAME)"
	@echo
	@echo "--------------------------------------------------------------------------"
	@echo " Disp rules."
	@echo
	@echo " all     : Compile the librairy if a file has changed, stored in $(LIBDIR)."
	@echo " re      : Recompile all objets of the librairy."
	@echo " clean   : Remove objects."
	@echo " fclean  : Remove objects and executable."
	@echo " help    : Display this."
	@echo "--------------------------------------------------------------------------"
	@echo
