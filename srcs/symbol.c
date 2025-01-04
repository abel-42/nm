/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abemorea <abemorea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 20:01:36 by abemorea          #+#    #+#             */
/*   Updated: 2025/01/04 16:05:39 by abemorea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static int	symbol_cmp(t_sym *sym1, t_sym *sym2)
{
	int		diff;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (1)
	{
		while (sym1->name[i] && !ft_isalnum(sym1->name[i]))
			i++;
		while (sym2->name[j] && !ft_isalnum(sym2->name[j]))
			j++;
		if (!(sym1->name[i] && sym2->name[j] && ft_tolower(sym1->name[i]) == ft_tolower(sym2->name[j])))
			break;
		i++;
		j++;
	}
	if ((diff = (unsigned char)ft_tolower(sym1->name[i]) - (unsigned char)ft_tolower(sym2->name[j])))
		return (diff);
	if ((diff = ft_strcmp(sym1->name, sym2->name)))
		return (diff);
	if (sym1->value < sym2->value)
		return (-1);
	if (sym2->value < sym1->value)
		return (1);
	return (0);
}

void	add_symbol(t_sym **symlst, t_sym *sym, int sorted)
{
	t_sym	*it;

	if (!*symlst || (sorted > 0 && symbol_cmp(sym, *symlst) < 0) || (sorted < 0 && symbol_cmp(sym, *symlst) > 0))
	{
		sym->next = *symlst;
		*symlst = sym;
		return ;
	}
	it = *symlst;
	while (it->next)
	{
		if ((sorted > 0 && symbol_cmp(sym, it->next) < 0) || (sorted < 0 && symbol_cmp(sym, it->next) > 0))
		{
			sym->next = it->next;
			it->next = sym;
			return ;
		}
		it = it->next;
	}
	it->next = sym;
	return ;
}

void		clear_symbols(t_sym **symlst)
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



