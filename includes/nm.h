#ifndef NM_H

# define NM_H

# include "lib.h"

# define ELF32 1
# define ELF64 2

# define FLAG_SA 1
# define FLAG_SG 2
# define FLAG_SU 3
# define FLAG_SR 4
# define FLAG_SP 5

typedef struct	s_sym
{
	char			*name;
	void			*sym;
	uint8_t			format;
	struct s_sym	*next;
} 			t_sym;

extern char	*g_optarg;
extern int	g_optind;
extern int	g_optopt;


int		ft_getopt(int ac, char * const *av, const char *optstring);

#endif