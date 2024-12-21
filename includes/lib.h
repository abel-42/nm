#ifndef LIB_H

# define LIB_H

# include <unistd.h>
# include <stdint.h>

# define BIN "01"
# define OCT "01234567"
# define DEC "0123456789"
# define HEX "0123456789ABCDEF"

size_t	ft_strlen(const char *s);
ssize_t	ft_putstr_fd(const char *s, int fd);
int		ft_memcmp(const void *buf1, const void *buf2, size_t n);
void	*ft_memset(void *b, int c, size_t len);
void	ft_putnbr_base_fd(uint64_t nbr, const char *base, int fd);
// void	*ft_insertion_sort(void *first, void *last, int (*cmp)(), void (*swap)());
void	ft_insertion_sort(void **data, int a, int b, int (*cmp)());
int		ft_strcmp(const char *s1, const char *s2);
int	    ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strchr(const char *s, int c);

#endif