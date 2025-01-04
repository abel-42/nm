#ifndef LIB_H

# define LIB_H

# include <unistd.h>
# include <stdint.h>

# define BIN "01"
# define OCT "01234567"
# define DEC "0123456789"
# define HEX "0123456789ABCDEF"
# define LOHEX "0123456789abcdef"

size_t	ft_strlen(const char *s);
ssize_t	ft_putstr_fd(const char *s, int fd);
int		ft_memcmp(const void *buf1, const void *buf2, size_t n);
void	*ft_memset(void *b, int c, size_t len);
void	ft_putnbr_base_fd(uint64_t nbr, const char *base, int fd);
int		ft_strcmp(const char *s1, const char *s2);
int	    ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strchr(const char *s, int c);
int	    ft_isalnum(int c);
int	    ft_tolower(int c);

#endif