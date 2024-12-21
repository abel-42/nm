/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abemorea <abemorea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 17:05:12 by abemorea          #+#    #+#             */
/*   Updated: 2024/12/21 19:16:49 by abemorea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GETOPT_H

# define FT_GETOPT_H

# include <unistd.h>

# define ERR_SOPT_INVALID "invalid option"
# define ERR_SOPT_ARG_REQUIRED "option requires an argument"
# define ERR_LOPT_INVALID "unrecognized"
# define ERR_LOPT_ARG_REQUIRED "requires an argument"

typedef struct s_option {
	const char	*name;
	int			has_arg;
	int			*flag;
	int			val;
}	t_option;

typedef struct s_ft_getopt_state {
	char			*optarg;
	int				optind;
	int				opterr;
	int				optopt;
	char			*nextchar;
	int				argc;
	char *const		*argv;
	const char		*optstring;
	const t_option	*longopts;
	// int				*longind;
}	t_ft_getopt_state;

void	ft_getopt_initialize_state(
			t_ft_getopt_state *state,
			// int argc, char *const argv,
			const char *optstring,
			const t_option *longopts
);

int		ft_getopt_long_r(t_ft_getopt_state *state,
			int argc,
			char *const *argv,
			int *longind
);


#endif