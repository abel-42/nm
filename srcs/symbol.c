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
	if (!*symlst || (sorted > 0 && ft_strcmp(name, (*symlst)->name) < 0) || (sorted < 0 && ft_strcmp(name, (*symlst)->name) > 0))
	{
		new->next = *symlst;
		*symlst = new;
		return (new);
	}
	it = *symlst;
	while (it->next)
	{
		if ((sorted > 0 && ft_strcmp(name, it->next->name) < 0) || (sorted < 0 && ft_strcmp(name, it->next->name) > 0))
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

static size_t	count_digit(uint64_t n, unsigned int base)
{
	size_t	count;

	if (n == 0)
		return (1);
	count = 0;
	while (n > 0)
	{
		n /= base;
		count++;
	}
	return (count);
}

// static void	print_padding(char c, size_t len)
// {
// 	while (len > 0)
// 	{
// 		write(1, &c, 1);
// 		len--;
// 	}
// }

void	print_symbols(t_sym *symlst, size_t len, char option)
{
	int padding;

	while (symlst)
	{
		if ((option != 'a' && symlst->debug_only)
			|| (option == 'u' && !symlst->undef)
			|| (option == 'g' && symlst->local))
		{
			symlst = symlst->next;
			continue;
		}
		if (symlst->undef)
			write(1, "                ", len * 2);
		else
		{
			padding = len * 2 - count_digit(symlst->value, 16);
			while (padding > 0)
			{
				write(1, "0", 1);
				padding--;
			}
			ft_putnbr_base_fd(symlst->value, LOHEX, 1);
		}
		ft_putstr_fd(" ", 1);
		write(1, &symlst->type, 1);
		ft_putstr_fd(" ", 1);
		ft_putstr_fd(symlst->name, 1);
		ft_putstr_fd("\n", 1);
		symlst = symlst->next;
	}
	return ;
}



