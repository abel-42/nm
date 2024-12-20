#include "lib.h"

ssize_t	ft_putstr_fd(const char *s, int fd)
{
	return write(fd, s, ft_strlen(s));
}