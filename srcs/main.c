/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abemorea <abemorea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:18:00 by abemorea          #+#    #+#             */
/*   Updated: 2024/12/21 21:13:22 by abemorea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_getopt.h"
#include <stdio.h>

static const t_option long_options[] = {
	{"foo", 0, 0, 'f'},
	{NULL, 0, 0, 0}
};

int	main(int ac, char **av)
{
	t_ft_getopt_state	state;
	int					opt;

	ft_getopt_initialize_state(&state, "laf:z::", long_options);
	opt = 0;
	while (opt > -1 && opt != '?')
	{
		opt = ft_getopt_long_r(&state, ac, av, NULL);
		if (opt > -1 && opt != '?')
		{
			printf("opt: %c\n", opt);
			if (state.optarg)
				printf("arg: %s\n", state.optarg);
		}
	}
	return (0);
}
