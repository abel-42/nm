/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abemorea <abemorea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 19:03:06 by abemorea          #+#    #+#             */
/*   Updated: 2024/12/27 21:44:31 by abemorea         ###   ########.fr       */
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

#include <stdio.h>

static int	is_ptr_valid(void *ptr, size_t size, void *memstart, size_t memsize)
{
	void	*limit;

	limit = (char *)(memstart + memsize);
	// printf("start: %p\n", start);
	// printf("size: %zu\n", size);
	// printf("ptr:%p\n", ptr);
	// printf("limit: %p\n", limit);
	return (ptr >= memstart && ptr + size <= limit);
}

int parse_elf32(t_sym **symlst, void *ptr, size_t size)
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
	if (eh->e_shoff > 0 && eh->e_shnum == 0)
		return (0);
	sh = (Elf32_Shdr*)(ptr + eh->e_shoff);
	i = 0;
	while (i < eh->e_shnum && is_ptr_valid(sh + i, sizeof (*sh), ptr, size))
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

int	parse_elf64(t_sym **symlst, void *ptr, size_t size)
{
	Elf64_Ehdr		*eh;
	// Elf64_Phdr	*ph;
	Elf64_Shdr		*sh;
	Elf64_Shdr		*link;
	// Elf64_Shdr	*shstrhdr;
	// Elf64_Shdr	*strhdr;
	// char		*shstrtab;
	char			*strtab;
	Elf64_Sym		*symtab;
	size_t			sym_count;
	size_t      	i;
	size_t			j;
	unsigned char	type;
	char			*name;

	eh = (Elf64_Ehdr *)ptr;
	if (eh->e_shoff > 0 && eh->e_shnum == 0)
		return (0);
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
	while (i < eh->e_shnum && is_ptr_valid(sh + i, sizeof (*sh), ptr, size))
	{
		if (sh[i].sh_type == SHT_SYMTAB)
		{
			link = NULL;
			if (sh[i].sh_link < eh->e_shnum)
				link = sh + sh[i].sh_link;
			if (!is_ptr_valid(link, sizeof (*link), ptr, size))
				return (0);
			strtab = (char *)(ptr + link->sh_offset);
			if (!is_ptr_valid(strtab, link->sh_size, ptr, size))
				return (0);
			symtab = (Elf64_Sym *)(ptr + sh[i].sh_offset);
			sym_count = sh[i].sh_size / sh[i].sh_entsize;
			if (!is_ptr_valid(symtab, sizeof (*symtab) * sym_count, ptr, size))
				return (0);
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
				name = (char *)(strtab + symtab[j].st_name);
				add_symbol(symlst, name, symtab + j, 1);
				j++;
			}
		}
		i++;
	}
	return (i >= eh->e_shnum);
}