/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abemorea <abemorea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:18:00 by abemorea          #+#    #+#             */
/*   Updated: 2024/12/23 20:20:28 by abemorea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_getopt.h"
#include <stdio.h>

static const t_option long_options[] = {
	{"foo", 2, NULL, 'f'},
	{"version", 0, NULL, 'v'},
	{"target", 1, NULL, 't'},
	{NULL, 0, 0, 0}
};

int	main(int ac, char **av)
{
	t_ft_getopt_state	state;
	int					opt;
	int	longind;

	ft_getopt_initialize_state(&state, "laf::z:", long_options);
	longind = -1;
	opt = 0;
	while (opt > -1)
	{
		opt = ft_getopt_long_r(&state, ac, av, &longind);
		if (opt > -1 && opt != '?')
		{
			printf("opt: %c\n", opt);
			if (state.optarg)
				printf("arg: %s\n", state.optarg);
			printf("longind: %d\n", longind);
			longind = -1;
		}
	}
	return (0);
}
