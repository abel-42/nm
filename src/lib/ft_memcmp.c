#include "lib.h"

int	ft_memcmp(const void *buf1, const void *buf2, size_t n)
{
	size_t	i;
	const unsigned char	*s1;
	const unsigned char	*s2;

	s1 = (const unsigned char *)buf1;
	s2 = (const unsigned char *)buf2;
	i = 0;
	while (i < n && s1[i] == s2[i])
		i++;
	if (i == n)
		return (0);
	return (s1[i] - s2[i]);
}