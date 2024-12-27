/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abemorea <abemorea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 19:03:06 by abemorea          #+#    #+#             */
/*   Updated: 2024/12/27 20:35:16 by abemorea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include <elf.h>

int	is_elf32(void *ptr)
{
	const uint8_t	sig[] = {0x7f, 'E', 'L', 'F', 1};
	size_t			sig_size;

	sig_size = sizeof(sig);
	return (ft_memcmp(ptr, sig, sig_size) == 0);
}

int	is_elf64(void *ptr)
{
	const uint8_t	sig[] = {0x7f, 'E', 'L', 'F', 2};
	size_t			sig_size;

	sig_size = sizeof(sig);
	return (ft_memcmp(ptr, sig, sig_size) == 0);
}

int parse_elf32(t_sym **symlst, void *ptr)
{
	Elf32_Ehdr		*eh;
	Elf32_Shdr		*sh;
	char			*strtab;
	Elf32_Sym		*symtab;
	size_t			sym_count;
	size_t      	i;
	size_t			j;
	unsigned char	type;

	eh = (Elf32_Ehdr *)ptr;
	sh = (Elf32_Shdr*)(ptr + eh->e_shoff);
	i = 0;
	while (i < eh->e_shnum)
	{
		if (sh[i].sh_type == SHT_SYMTAB)
		{
			strtab = (char *)(ptr + sh[sh[i].sh_link].sh_offset);
			symtab = (Elf32_Sym *)(ptr + sh[i].sh_offset);
			sym_count = sh[i].sh_size / sh[i].sh_entsize;
			j = 0;
			while (j < sym_count)
			{
				type = ELF64_ST_TYPE(symtab[j].st_info);
				if (type == STT_FILE)
				{
					j++;
					continue ;
				}
				add_symbol(symlst, (char *)(strtab + symtab[j].st_name),
					symtab + j, 1);
				j++;
			}
		}
		i++;
	}
	return (0);
}

int	parse_elf64(t_sym **symlst, void *ptr)
{
	Elf64_Ehdr		*eh;
	// Elf64_Phdr	*ph;
	Elf64_Shdr		*sh;
	// Elf64_Shdr	*shstrhdr;
	// Elf64_Shdr	*strhdr;
	// char		*shstrtab;
	char			*strtab;
	Elf64_Sym		*symtab;
	size_t			sym_count;
	size_t      	i;
	size_t			j;
	unsigned char	type;

	eh = (Elf64_Ehdr *)ptr;
	// ph = (Elf64_Phdr *)(ptr + eh->e_phoff);
	// i = 0;
	// while (i < eh->e_phnum)
	// {
	// 	i++;
	// }
	// shstrhdr = ((Elf64_Shdr*)(ptr + eh->e_shoff)) + eh->e_shstrndx;
	// shstrtab = (char*)(ptr + shstrhdr->sh_offset);
	sh = (Elf64_Shdr*)(ptr + eh->e_shoff);
	i = 0;
	while (i < eh->e_shnum)
	{
		if (sh[i].sh_type == SHT_SYMTAB)
		{
			strtab = (char *)(ptr + sh[sh[i].sh_link].sh_offset);
			symtab = (Elf64_Sym *)(ptr + sh[i].sh_offset);
			sym_count = sh[i].sh_size / sh[i].sh_entsize;
			j = 0;
			while (j < sym_count)
			{
				type = ELF64_ST_TYPE(symtab[j].st_info);
				if (type == STT_FILE)
				{
					j++;
					continue ;
				}
				// ft_putstr_fd((char *)(strtab + symtab[j].st_name), 1);
				// ft_putstr_fd("\n", 1);
				add_symbol(symlst, (char *)(strtab + symtab[j].st_name),
					symtab + j, 1);
				j++;
			}
		}
		i++;
	}
	return (0);
}