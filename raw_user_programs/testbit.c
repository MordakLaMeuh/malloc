
#include <stdio.h>
#include <stdint.h>

#define BLOC_MASK 64

int		jump(uint64_t r, uint32_t required_sectors)
{
	static int itab[16] = {4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0};
	uint32_t tmp;

	while (1)
	{
		tmp = r & 0xf,
		required_sectors -= itab[tmp];
		if (tmp != 0)
			break;
		r >>= 4;
	}
	return (required_sectors);
}

int		fill(uint64_t *field, uint32_t required_sectors)
{
	int initial_offset;
	uint64_t mask;
	uint64_t r;
	int offset;

	if (required_sectors == BLOC_MASK && *field != 0)
		return (-1);
	initial_offset = BLOC_MASK - required_sectors;
	mask = (((uint64_t)1 << required_sectors) - 1) << initial_offset;
	while (mask >> initial_offset != 0)
	{
		r = *field & mask;
		if (r == 0)
		{
			*field |= mask;
			return (BLOC_MASK - initial_offset - required_sectors);
		}
		offset = jump(r >> initial_offset, required_sectors);
		mask >>= offset;
		initial_offset -= offset;
	}
	return (-1);
}

#define SIZE_TEST 32

int 	main(void) {
	uint32_t a = 0x0FFEFF80;
	printf("initial value: %#x\n", a);
	int size = SIZE_TEST;
	int sector = size >> 4;
	if (size != sector << 4)
		sector += 1;
	printf("size = %i, sector wanted = %i\n", size, sector);
	int mask = (1 << sector) - 1;
	printf("mask = %#x\n", mask);


	int count = 32 - sector;
	uint32_t v_test = mask << count;
	printf("mask up = %#x\n", v_test);

	uint32_t b;

	//int shl_v;

	while (count != -1) {
		b = a & v_test;
		if (b == 0) {
			printf("matched: %#.8x - %#.8x\n", a, v_test);
		}
		else {
			printf("NOT matched: %#.8x - %#.8x, reste = %#.x, jump = %i\n", a, v_test, b >> count, jump(b >> count, sector));


		}
		v_test = v_test >> 1;
		count--;
	}

	int res;
	uint64_t field = 0x0FFEFF80;
	int sectors;

	sectors = 36;
	res = fill(&field, sectors);
	if (res != -1) {
		printf("filling success at sector %i for %i sectors, new field = %lx\n", res, sectors, field);
	} else {
		printf("Cannot fill for a sector %i\n", sectors);
	}

	sectors = 1;
	res = fill(&field, sectors);
	if (res != -1) {
		printf("filling success at sector %i for %i sectors, new field = %lx\n", res, sectors, field);
	} else {
		printf("Cannot fill for a sector %i\n", sectors);
	}

	sectors = 4;
	res = fill(&field, sectors);
	if (res != -1) {
		printf("filling success at sector %i for %i sectors, new field = %lx\n", res, sectors, field);
	} else {
		printf("Cannot fill for a sector %i\n", sectors);
	}

	sectors = 4;
	res = fill(&field, sectors);
	if (res != -1) {
		printf("filling success at sector %i for %i sectors, new field = %lx\n", res, sectors, field);
	} else {
		printf("Cannot fill for a sector %i\n", sectors);
	}

	sectors = 2;
	res = fill(&field, sectors);
	if (res != -1) {
		printf("filling success at sector %i for %i sectors, new field = %lx\n", res, sectors, field);
	} else {
		printf("Cannot fill for a sector %i\n", sectors);
	}

	sectors = 1;
	res = fill(&field, sectors);
	if (res != -1) {
		printf("filling success at sector %i for %i sectors, new field = %lx\n", res, sectors, field);
	} else {
		printf("Cannot fill for a sector %i\n", sectors);
	}

	return 0;
}
