/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abemorea <abemorea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 20:09:27 by abemorea          #+#    #+#             */
/*   Updated: 2025/01/04 16:02:06 by abemorea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H

# define NM_H

# include "lib.h"

#include <errno.h>
#include <stdlib.h>

extern int	errno;

typedef struct s_sym
{
	char			*name;
	void			*sym;
	char			type;
	char			undef;
	char			debug_only;
	char			local;
	uint64_t		value;
	struct s_sym	*next;
}	t_sym;

typedef void *(*t_endian)(void *, size_t);

int		is_elf32(void *ptr);
int		is_elf64(void *ptr);
int		parse_elf32(t_sym **symlst, void *ptr, size_t size, char sort);
int		parse_elf64(t_sym **symlst, void *ptr, size_t size, char sort);

void	add_symbol(t_sym **symlst, t_sym *sym, int sorted);
void	clear_symbols(t_sym **symlst);
void	print_symbols(t_sym *symlst, size_t len, char option);

void	*reverse_endian(void *ptr, size_t size);
int		is_host_little_endian(void);
int		is_host_big_endian(void);

#endif