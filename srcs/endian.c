#include "nm.h"

void	*reverse_endian(void *ptr, size_t size)
{
	uint8_t	*bytes;
	uint8_t	b;
	size_t	lo;
	size_t	hi;

	bytes = (uint8_t *)ptr;
	lo = 0;
	hi = size - 1;
	while (lo < hi)
	{
		b = bytes[lo];
		bytes[lo] = bytes[hi];
		bytes[hi] = b;
		lo++;
		hi--;
	}
	return (ptr);
}

int	is_host_little_endian(void)
{
	int	x;

	x = 1;
	return (*(char*)(&x) == 1);
}

int	is_host_big_endian(void)
{
	return (!is_host_little_endian());
}