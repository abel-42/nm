/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt_r.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abemorea <abemorea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 17:05:12 by abemorea          #+#    #+#             */
/*   Updated: 2024/12/20 17:28:00 by abemorea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GETOPT_R_H

# define FT_GETOPT_R_H

typedef struct s_option {
	const char	*name;
	int			has_arg;
	int			*flag;
	int			val;
}	t_option;

typedef struct s_getopt_state {
	int			argc;
	char *const	*argv;
}	t_getopt_state;

#endif