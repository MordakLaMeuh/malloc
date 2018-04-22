
void begin (void) __attribute__((constructor));

#include <libft.h>

void       free(void *ptr) {
	ft_putstr("custom free called !\n");
	(void)ptr;
}

void       *malloc(size_t size) {
	ft_putstr("custom malloc called !\n");
	(void)size;
	return NULL;
}

void       *realloc(void *ptr, size_t size)
{
	ft_putstr("custom realoc called !\n");
	(void)ptr;
	(void)size;
	return NULL;
}

void       *calloc(size_t count, size_t size)
{
	ft_putstr("custom calloc called !\n");
	(void)size;
	(void)count;
	return NULL;
}

void begin(void)
{
	ft_putstr("custom constructor called !\n");

}
