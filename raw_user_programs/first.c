
#include <stdlib.h>

#include "libft_alloc.h"

int main(void) {
	char *s = malloc(10);
	free(s);
	return 0;
}
