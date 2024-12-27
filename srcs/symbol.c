/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abemorea <abemorea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 20:01:36 by abemorea          #+#    #+#             */
/*   Updated: 2024/12/27 20:22:47 by abemorea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include <stdlib.h>

t_sym	*add_symbol(t_sym **symlst, char *name, void *sym, int sorted)
{
	t_sym	*new;
	t_sym	*it;

	new = (t_sym *)malloc(sizeof (t_sym));
	if (!new)
		return (NULL);
	ft_memset(new, 0, sizeof(t_sym));
	new->name = name;
	new->sym = sym;
	if (!*symlst || (sorted && ft_strcmp(name, (*symlst)->name) < 0))
	{
		new->next = *symlst;
		*symlst = new;
		return (new);
	}
	it = *symlst;
	while (it->next)
	{
		if (sorted && ft_strcmp(name, it->next->name) < 0)
		{
			new->next = it->next;
			it->next = new;
			return (new);
		}
		it = it->next;
	}
	it->next = new;
	return (new);
}

void	clear_symbols(t_sym **symlst)
{
	t_sym	*it;
	t_sym	*next;

	it = *symlst;
	while (it)
	{
		next = it->next;
		free(it);
		it = next;
	}
	*symlst = NULL;
	return ;
}

void	print_symbols(t_sym *symlst)
{
	while (symlst)
	{
		ft_putstr_fd(symlst->name, 1);
		ft_putstr_fd("\n", 1);
		symlst = symlst->next;
	}
	return ;
}



