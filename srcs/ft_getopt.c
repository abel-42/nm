// #include <unistd.h>
#include "nm.h"

char	*g_optarg = NULL;
int		g_optind = 1;
int		g_optopt = 0;

int		ft_getopt(int ac, char * const *av, const char *optstring)
{
	static char *nextchar = NULL;
	// static int	optind = 1;
	size_t		len;
	int			c;
	char		*opt;

	// if (optind != g_optind)
	// {
	// 	optind = g_optind;
	// 	nextchar = NULL;
	// }
	g_optarg = NULL;
	if (ac > g_optind)
	{
		// if (nextchar)
		// {
		// 	len = ft_strlen(av[g_optind]);
		// 	// if (av[g_optind] > nextchar && av[g_optind] + len < nextchar)
		// 	// 	nextchar = NULL;
		// 	if (nextchar < av[g_optind] && nextchar >= av[g_optind] + len)
		// 		nextchar = NULL;

		// }
		if (av[optind] && av[g_optind][0] == '-' && av[g_optind][1] != '\0')
		{
			if (av[g_optind][1] == '-' && av[g_optind][2] == '\0')
			{
				g_optind++;
				return (-1);
			}
			len = ft_strlen(av[g_optind]);
			if (!nextchar || nextchar < av[g_optind] && nextchar >= av[g_optind] + len)
				nextchar = av[g_optind] + 1;
			opt = ft_strchr(optstring, *nextchar);
			if (*nextchar != ':' && opt)
			{
				c = *nextchar;
				nextchar++;
				// ft_putstr_fd("TEST OK.\n", 1);
				if (opt[1] == ':')
				{
					// ft_putstr_fd("OPT requires ARG\n", 1);
					g_optind++;
					if (!*nextchar)
					{
						if (g_optind >= ac)
						{
							if (*optstring == ':')
								return (':');
							//PRINT ERROR
							return ('?');
						}
						g_optarg = av[g_optind];
						g_optind++;
					}
					else
						g_optarg = nextchar + 1;
				}
				else if (!*nextchar)
					g_optind++;
				return (c);
			}
			g_optopt = c;
			//PRINT ERRROR
			return ('?');
		}
	}
	return (-1);
}