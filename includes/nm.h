/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abemorea <abemorea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 20:09:27 by abemorea          #+#    #+#             */
/*   Updated: 2024/12/27 20:10:57 by abemorea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H

# define NM_H

# include "lib.h"

# define ELF32 1
# define ELF64 2

// # define FLAG_SA 1
// # define FLAG_SG 2
// # define FLAG_SU 3
// # define FLAG_SR 4
// # define FLAG_SP 5

typedef struct s_sym
{
	char			*name;
	void			*sym;
	uint8_t			format;
	struct s_sym	*next;
}	t_sym;

int		is_elf32(void *ptr);
int		is_elf64(void *ptr);
int		parse_elf32(t_sym **symlst, void *ptr);
int		parse_elf64(t_sym **symlst, void *ptr);

t_sym	*add_symbol(t_sym **symlst, char *name, void *sym, int sorted);
void	clear_symbols(t_sym **symlst);
void	print_symbols(t_sym *symlst);

#endif