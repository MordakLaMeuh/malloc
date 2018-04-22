#include <stdio.h>
#include <stdlib.h>

int main(void) {
/* XXX OSX TINY start at 1 */
	char *a = malloc(1);
	char *b = malloc(2);
	char *c = malloc(1);
	char *d = malloc(11);
	char *e = malloc(1);
	printf("ptr_a = %p, ptr_b = %p, diff = %lu\n", a, b, (size_t)b - (size_t)a);
	printf("ptr_b = %p, ptr_c = %p, diff = %lu\n", b, c, (size_t)c - (size_t)b);
	printf("ptr_c = %p, ptr_d = %p, diff = %lu\n", c, d, (size_t)d - (size_t)c);
	printf("ptr_d = %p, ptr_e = %p, diff = %lu\n", d, e, (size_t)e - (size_t)d);
	printf("ptr_a = %p, ptr_e = %p, diff = %lu\n", a, e, (size_t)e - (size_t)a);
	printf("---------------------------------------------------------------\n");
	free(e); free(d); free(c); free(b); free(a);

	a = malloc(16);
	b = malloc(16);
	c = malloc(16);
	d = malloc(16);
	e = malloc(16);
	printf("ptr_a = %p, ptr_b = %p, diff = %lu\n", a, b, (size_t)b - (size_t)a);
	printf("ptr_b = %p, ptr_c = %p, diff = %lu\n", b, c, (size_t)c - (size_t)b);
	printf("ptr_c = %p, ptr_d = %p, diff = %lu\n", c, d, (size_t)d - (size_t)c);
	printf("ptr_d = %p, ptr_e = %p, diff = %lu\n", d, e, (size_t)e - (size_t)d);
	printf("ptr_a = %p, ptr_e = %p, diff = %lu\n", a, e, (size_t)e - (size_t)a);
	printf("---------------------------------------------------------------\n");
	free(e); free(d); free(c); free(b); free(a);

	a = malloc(17);
	b = malloc(31);
	c = malloc(32);
	d = malloc(19);
	e = malloc(29);
	printf("ptr_a = %p, ptr_b = %p, diff = %lu\n", a, b, (size_t)b - (size_t)a);
	printf("ptr_b = %p, ptr_c = %p, diff = %lu\n", b, c, (size_t)c - (size_t)b);
	printf("ptr_c = %p, ptr_d = %p, diff = %lu\n", c, d, (size_t)d - (size_t)c);
	printf("ptr_d = %p, ptr_e = %p, diff = %lu\n", d, e, (size_t)e - (size_t)d);
	printf("ptr_a = %p, ptr_e = %p, diff = %lu\n", a, e, (size_t)e - (size_t)a);
	printf("---------------------------------------------------------------\n");
	free(e); free(d); free(c); free(b); free(a);

	a = malloc(56);
	b = malloc(55);
	c = malloc(51);
	d = malloc(59);
	e = malloc(60);
	printf("ptr_a = %p, ptr_b = %p, diff = %lu\n", a, b, (size_t)b - (size_t)a);
	printf("ptr_b = %p, ptr_c = %p, diff = %lu\n", b, c, (size_t)c - (size_t)b);
	printf("ptr_c = %p, ptr_d = %p, diff = %lu\n", c, d, (size_t)d - (size_t)c);
	printf("ptr_d = %p, ptr_e = %p, diff = %lu\n", d, e, (size_t)e - (size_t)d);
	printf("ptr_a = %p, ptr_e = %p, diff = %lu\n", a, e, (size_t)e - (size_t)a);
	printf("---------------------------------------------------------------\n");
	free(e); free(d); free(c); free(b); free(a);

	a = malloc(70);
	b = malloc(84);
	c = malloc(90);
	d = malloc(100);
	e = malloc(80);
	printf("ptr_a = %p, ptr_b = %p, diff = %lu\n", a, b, (size_t)b - (size_t)a);
	printf("ptr_b = %p, ptr_c = %p, diff = %lu\n", b, c, (size_t)c - (size_t)b);
	printf("ptr_c = %p, ptr_d = %p, diff = %lu\n", c, d, (size_t)d - (size_t)c);
	printf("ptr_d = %p, ptr_e = %p, diff = %lu\n", d, e, (size_t)e - (size_t)d);
	printf("ptr_a = %p, ptr_e = %p, diff = %lu\n", a, e, (size_t)e - (size_t)a);
	printf("---------------------------------------------------------------\n");
	free(e); free(d); free(c); free(b); free(a);

	a = malloc(200);
	b = malloc(352);
	c = malloc(354);
	d = malloc(435);
	e = malloc(333);
	printf("ptr_a = %p, ptr_b = %p, diff = %lu\n", a, b, (size_t)b - (size_t)a);
	printf("ptr_b = %p, ptr_c = %p, diff = %lu\n", b, c, (size_t)c - (size_t)b);
	printf("ptr_c = %p, ptr_d = %p, diff = %lu\n", c, d, (size_t)d - (size_t)c);
	printf("ptr_d = %p, ptr_e = %p, diff = %lu\n", d, e, (size_t)e - (size_t)d);
	printf("ptr_a = %p, ptr_e = %p, diff = %lu\n", a, e, (size_t)e - (size_t)a);
	printf("---------------------------------------------------------------\n");
	free(e); free(d); free(c); free(b); free(a);

/* XXX OSX MEDIUM start at 1024 - 16 => 1008 */
	a = malloc(1025);
	b = malloc(1030);
	c = malloc(1010);
	d = malloc(1248);
	e = malloc(1530);
	printf("ptr_a = %p, ptr_b = %p, diff = %lu, mod 512 = %s\n", a, b, (size_t)b - (size_t)a, (((size_t)b - (size_t)a) % 512 == 0) ? "TRUE" : "FALSE");
	printf("ptr_b = %p, ptr_c = %p, diff = %lu, mod 512 = %s\n", b, c, (size_t)c - (size_t)b, (((size_t)b - (size_t)a) % 512 == 0) ? "TRUE" : "FALSE");
	printf("ptr_c = %p, ptr_d = %p, diff = %lu, mod 512 = %s\n", c, d, (size_t)d - (size_t)c, (((size_t)b - (size_t)a) % 512 == 0) ? "TRUE" : "FALSE");
	printf("ptr_d = %p, ptr_e = %p, diff = %lu, mod 512 = %s\n", d, e, (size_t)e - (size_t)d, (((size_t)b - (size_t)a) % 512 == 0) ? "TRUE" : "FALSE");
	printf("ptr_a = %p, ptr_e = %p, diff = %lu, mod 512 = %s\n", a, e, (size_t)e - (size_t)a, (((size_t)b - (size_t)a) % 512 == 0) ? "TRUE" : "FALSE");
	printf("---------------------------------------------------------------\n");
	free(e); free(d); free(c); free(b); free(a);

	a = malloc(1745);
	b = malloc(1875);
	c = malloc(1324);
	d = malloc(1966);
	e = malloc(1800);
	printf("ptr_a = %p, ptr_b = %p, diff = %lu, mod 512 = %s\n", a, b, (size_t)b - (size_t)a, (((size_t)b - (size_t)a) % 512 == 0) ? "TRUE" : "FALSE");
	printf("ptr_b = %p, ptr_c = %p, diff = %lu, mod 512 = %s\n", b, c, (size_t)c - (size_t)b, (((size_t)b - (size_t)a) % 512 == 0) ? "TRUE" : "FALSE");
	printf("ptr_c = %p, ptr_d = %p, diff = %lu, mod 512 = %s\n", c, d, (size_t)d - (size_t)c, (((size_t)b - (size_t)a) % 512 == 0) ? "TRUE" : "FALSE");
	printf("ptr_d = %p, ptr_e = %p, diff = %lu, mod 512 = %s\n", d, e, (size_t)e - (size_t)d, (((size_t)b - (size_t)a) % 512 == 0) ? "TRUE" : "FALSE");
	printf("ptr_a = %p, ptr_e = %p, diff = %lu, mod 512 = %s\n", a, e, (size_t)e - (size_t)a, (((size_t)b - (size_t)a) % 512 == 0) ? "TRUE" : "FALSE");
	printf("---------------------------------------------------------------\n");
	free(e); free(d); free(c); free(b); free(a);

	a = malloc(11245);
	b = malloc(13625);
	c = malloc(14521);
	d = malloc(10245);
	e = malloc(15425);
	printf("ptr_a = %p, ptr_b = %p, diff = %lu, mod 512 = %s\n", a, b, (size_t)b - (size_t)a, (((size_t)b - (size_t)a) % 512 == 0) ? "TRUE" : "FALSE");
	printf("ptr_b = %p, ptr_c = %p, diff = %lu, mod 512 = %s\n", b, c, (size_t)c - (size_t)b, (((size_t)b - (size_t)a) % 512 == 0) ? "TRUE" : "FALSE");
	printf("ptr_c = %p, ptr_d = %p, diff = %lu, mod 512 = %s\n", c, d, (size_t)d - (size_t)c, (((size_t)b - (size_t)a) % 512 == 0) ? "TRUE" : "FALSE");
	printf("ptr_d = %p, ptr_e = %p, diff = %lu, mod 512 = %s\n", d, e, (size_t)e - (size_t)d, (((size_t)b - (size_t)a) % 512 == 0) ? "TRUE" : "FALSE");
	printf("ptr_a = %p, ptr_e = %p, diff = %lu, mod 512 = %s\n", a, e, (size_t)e - (size_t)a, (((size_t)b - (size_t)a) % 512 == 0) ? "TRUE" : "FALSE");
	printf("---------------------------------------------------------------\n");
	free(e); free(d); free(c); free(b); free(a);

	a = malloc(55000);
	b = malloc(48578);
	c = malloc(49666);
	d = malloc(65985);
	e = malloc(60000);
	printf("ptr_a = %p, ptr_b = %p, diff = %lu, mod 4096 = %s\n", a, b, (size_t)b - (size_t)a, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("ptr_b = %p, ptr_c = %p, diff = %lu, mod 4096 = %s\n", b, c, (size_t)c - (size_t)b, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("ptr_c = %p, ptr_d = %p, diff = %lu, mod 4096 = %s\n", c, d, (size_t)d - (size_t)c, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("ptr_d = %p, ptr_e = %p, diff = %lu, mod 4096 = %s\n", d, e, (size_t)e - (size_t)d, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("ptr_a = %p, ptr_e = %p, diff = %lu, mod 4096 = %s\n", a, e, (size_t)e - (size_t)a, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("---------------------------------------------------------------\n");
	free(e); free(d); free(c); free(b); free(a);

/* XXX OSX LARGE start at 131072 - 512 => 130560 */
	a = malloc(140000);
	b = malloc(141000);
	c = malloc(129500);
	d = malloc(143000);
	e = malloc(144000);
	printf("ptr_a = %p, ptr_b = %p, diff = %lu, mod 4096 = %s\n", a, b, (size_t)b - (size_t)a, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("ptr_b = %p, ptr_c = %p, diff = %lu, mod 4096 = %s\n", b, c, (size_t)c - (size_t)b, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("ptr_c = %p, ptr_d = %p, diff = %lu, mod 4096 = %s\n", c, d, (size_t)d - (size_t)c, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("ptr_d = %p, ptr_e = %p, diff = %lu, mod 4096 = %s\n", d, e, (size_t)e - (size_t)d, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("ptr_a = %p, ptr_e = %p, diff = %lu, mod 4096 = %s\n", a, e, (size_t)e - (size_t)a, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("---------------------------------------------------------------\n");
	free(e); free(d); free(c); free(b); free(a);

	a = malloc(195642);
	b = malloc(256874);
	c = malloc(198552);
	d = malloc(200000);
	e = malloc(333333);
	printf("ptr_a = %p, ptr_b = %p, diff = %lu, mod 4096 = %s\n", a, b, (size_t)b - (size_t)a, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("ptr_b = %p, ptr_c = %p, diff = %lu, mod 4096 = %s\n", b, c, (size_t)c - (size_t)b, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("ptr_c = %p, ptr_d = %p, diff = %lu, mod 4096 = %s\n", c, d, (size_t)d - (size_t)c, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("ptr_d = %p, ptr_e = %p, diff = %lu, mod 4096 = %s\n", d, e, (size_t)e - (size_t)d, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("ptr_a = %p, ptr_e = %p, diff = %lu, mod 4096 = %s\n", a, e, (size_t)e - (size_t)a, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("---------------------------------------------------------------\n");
	free(e); free(d); free(c); free(b); free(a);

	a = malloc(111111);
	b = malloc(112111);
	c = malloc(99999);
	d = malloc(100000);
	e = malloc(130000);
	printf("ptr_a = %p, ptr_b = %p, diff = %lu, mod 4096 = %s\n", a, b, (size_t)b - (size_t)a, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("ptr_b = %p, ptr_c = %p, diff = %lu, mod 4096 = %s\n", b, c, (size_t)c - (size_t)b, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("ptr_c = %p, ptr_d = %p, diff = %lu, mod 4096 = %s\n", c, d, (size_t)d - (size_t)c, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("ptr_d = %p, ptr_e = %p, diff = %lu, mod 4096 = %s\n", d, e, (size_t)e - (size_t)d, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("ptr_a = %p, ptr_e = %p, diff = %lu, mod 4096 = %s\n", a, e, (size_t)e - (size_t)a, (((size_t)b - (size_t)a) % 4096 == 0) ? "TRUE" : "FALSE");
	printf("---------------------------------------------------------------\n");
	free(e); free(d); free(c); free(b); free(a);
	return 0;
}
