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

// static void	*ignore_endian(void *ptr, size_t size)
// {
// 	(void)ptr;
// 	(void)size;
// 	return (ptr);
// }

char	get_symbol_type(uint16_t shndx, uint32_t sh_type, uint64_t sh_flags, unsigned char st_type, unsigned char st_bind, char *section)
{
	if (st_bind == STB_GNU_UNIQUE)
		return ('u');
	if (st_bind == STB_WEAK)
	{
		if (st_type == STT_OBJECT)
			return (shndx == SHN_UNDEF ? 'v' : 'V');
		else
			return (shndx == SHN_UNDEF ? 'w' : 'W');
	}
	if (shndx == SHN_ABS)
		return ('A');
	if (shndx == SHN_COMMON)
		return ('C');
	if (shndx == SHN_MIPS_SCOMMON || ft_strcmp(section, ".scommon") == 0)
		return ('c');
	if (shndx == SHN_UNDEF)
		return ('U');
	if (sh_type == SHT_NOBITS)
	{
		if (sh_flags == (SHF_WRITE | SHF_ALLOC))
			return (st_bind == STB_LOCAL ? 'b' : 'B');
	}
	// printf("%u == %u, %u %s\n", sh_type, SHT_PROGBITS, SHT_NOBITS, section);
	if (sh_type == SHT_PROGBITS || sh_type == SHT_DYNAMIC || sh_type == SHT_NOTE || sh_type == SHT_INIT_ARRAY || sh_type == SHT_FINI_ARRAY)
	{
		if (sh_flags & SHF_ALLOC)
		{
			if (sh_flags & SHF_WRITE)
				return (st_bind == STB_LOCAL ? 'd' : 'D');
			if (sh_flags & SHF_EXECINSTR)
				return (st_bind == STB_LOCAL ? 't' : 'T');
			return (st_bind == STB_LOCAL ? 'r' : 'R');
		}
	}
	if (st_type == STT_GNU_IFUNC)
		return ('i');
	if (ft_strncmp(section, ".debug", 6) == 0)
		return ('N');
	if ((sh_flags & SHF_WRITE) == 0)
		return ('n');
	return '?';
}

int parse_elf32(t_sym **symlst, void *ptr, size_t size)
{
	Elf32_Ehdr		*eh;
	Elf32_Shdr		*sh;
	Elf32_Shdr		*link;
	char			*strtab;
	Elf32_Sym		*symtab;
	size_t			sym_count;
	size_t      	i;
	size_t			j;
	unsigned char	type;
	int				rev_endian;
	Elf32_Off      	shoff;
	uint16_t      	shnum;
	uint32_t		sh_type;
	uint32_t		sh_link;
	uint32_t		sh_size;
	uint32_t		sh_entsize;
	Elf32_Off		sh_offset;
	Elf32_Off		strtab_offset;
	uint32_t		strtab_size;
	// uint32_t		strtab_entsize;
	uint32_t		st_name;
	char			*name;
	t_sym			*symbol;

	eh = (Elf32_Ehdr *)ptr;
	rev_endian = 0;
	if (eh->e_ident[EI_DATA] == ELFDATA2LSB)
		rev_endian = 0;
	else if (eh->e_ident[EI_DATA] == ELFDATA2MSB)
		rev_endian = 1;
	else
		return (0);
	shoff = eh->e_shoff;
	if (rev_endian)
		reverse_endian(&shoff, sizeof(shoff));
	sh = (Elf32_Shdr*)(ptr + shoff);
	if (eh->e_shnum == 0)
		shnum = sh->sh_size;
	else
		shnum = eh->e_shnum;
	if (rev_endian)
		reverse_endian(&shnum, sizeof(shnum));
	if (shoff > 0 && shnum == 0)
		return (0);
	i = 0;
	while (i < shnum && is_ptr_valid(sh + i, sizeof (*sh), ptr, size))
	{
		sh_type = sh[i].sh_type;
		sh_link = sh[i].sh_link;
		sh_size = sh[i].sh_size;
		sh_entsize = sh[i].sh_entsize;
		sh_offset = sh[i].sh_offset;
		if (rev_endian)
		{
			reverse_endian(&sh_type, sizeof(sh_type));
			reverse_endian(&sh_link, sizeof(sh_link));
			reverse_endian(&sh_size, sizeof(sh_size));
			reverse_endian(&sh_entsize, sizeof(sh_entsize));
			reverse_endian(&sh_offset, sizeof(sh_offset));
		}
		if (sh_type == SHT_SYMTAB)
		{
			link = NULL;
			if (sh_link < shnum)
				link = sh + sh_link;
			if (!is_ptr_valid(link, sizeof (*link), ptr, size))
				return (0);
			strtab_offset = link->sh_offset;
			strtab_size = link->sh_size;
			// strtab_entsize = link->sh_entsize;
			if (rev_endian)
			{
				reverse_endian(&strtab_offset, sizeof(strtab_offset));
				reverse_endian(&strtab_size, sizeof(strtab_size));
				// reverse_endian(&strtab_entsize, sizeof(strtab_entsize));
			}
			strtab = (char *)(ptr + strtab_offset);
			if (!is_ptr_valid(strtab, strtab_size, ptr, size))
				return (0);
			symtab = (Elf32_Sym *)(ptr + sh_offset);
			sym_count = sh_size / sh_entsize;
			if (!is_ptr_valid(symtab, sizeof (*symtab) * sym_count, ptr, size))
				return (0);
			j = 0;
			while (j < sym_count)
			{
				st_name = symtab[j].st_name;
				if (rev_endian)
					reverse_endian(&st_name, sizeof(st_name));
				type = ELF32_ST_TYPE(symtab[j].st_info);
				if (st_name == 0 || st_name >= strtab_size || type == STT_FILE)
				{
					j++;
					continue ;
				}
				// printf("%u / %u\n", st_name, strtab_size);
				name = (char *)(strtab + st_name);
				// ft_putstr_fd(name, 1);
				// ft_putstr_fd("\n", 1);
				symbol = add_symbol(symlst, name, symtab + j, 1);
				if (!symbol)
					return (0);
				// symbol->type = get_symbol_type()
				j++;
			}
		}
		i++;
	}
	return (i >= shnum);
}

int	parse_elf64(t_sym **symlst, void *ptr, size_t size)
{
	Elf64_Ehdr		*eh;
	Elf64_Shdr		*sh;
	Elf64_Shdr		*shstrhdr;
	Elf64_Shdr		*link;
	char			*shstrtab;
	char			*section;
	char			*strtab;
	Elf64_Sym		*symtab;
	size_t			sym_count;
	size_t      	i;
	size_t			j;
	unsigned char	type;
	int				rev_endian;
	Elf64_Off      	shoff;
	uint16_t      	shnum;
	uint32_t		sh_type;
	uint32_t		sh_link;
	uint64_t   		sh_flags;
	uint64_t		sh_size;
	uint64_t		sh_entsize;
	Elf64_Off		sh_offset;
	Elf64_Off		strtab_offset;
	uint64_t		strtab_size;
	uint32_t		st_name;
	uint16_t		st_shndx;
	Elf64_Addr		st_value;
	char			*name;
	t_sym			*symbol;

	eh = (Elf64_Ehdr *)ptr;
	rev_endian = 0;
	if (is_host_big_endian() && eh->e_ident[EI_DATA] == ELFDATA2LSB)
		rev_endian = 1;
	else if (is_host_little_endian() && eh->e_ident[EI_DATA] == ELFDATA2MSB)
		rev_endian = 1;
	else
		return (0);
	shoff = eh->e_shoff;
	if (rev_endian)
		reverse_endian(&shoff, sizeof(shoff));
	sh = (Elf64_Shdr*)(ptr + shoff);
	if (eh->e_shnum == 0)
		shnum = sh->sh_size;
	else
		shnum = eh->e_shnum;
	if (rev_endian)
		reverse_endian(&shnum, sizeof(shnum));
	if (shoff > 0 && shnum == 0)
		return (0);
	shstrhdr = ((Elf64_Shdr*)(ptr + eh->e_shoff)) + eh->e_shstrndx;
	shstrtab = (char*)(ptr + shstrhdr->sh_offset);
	i = 0;
	while (i < shnum && is_ptr_valid(sh + i, sizeof (*sh), ptr, size))
	{
		// section = (char *)(shstrtab + sh[i].sh_name);
		// ft_putstr_fd(sec_name, 1);
		// ft_putstr_fd("\n", 1);
		sh_type = sh[i].sh_type;
		sh_link = sh[i].sh_link;
		// sh_flags = sh[i].sh_flags;
		sh_size = sh[i].sh_size;
		sh_entsize = sh[i].sh_entsize;
		sh_offset = sh[i].sh_offset;
		if (rev_endian)
		{
			// reverse_endian(&sh_type, sizeof(sh_type));
			reverse_endian(&sh_link, sizeof(sh_link));
			// reverse_endian(&sh_flags, sizeof(sh_flags));
			reverse_endian(&sh_size, sizeof(sh_size));
			reverse_endian(&sh_entsize, sizeof(sh_entsize));
			reverse_endian(&sh_offset, sizeof(sh_offset));
		}
		if (sh_type == SHT_SYMTAB)
		{
			link = NULL;
			if (sh_link < shnum)
				link = sh + sh_link;
			if (!is_ptr_valid(link, sizeof (*link), ptr, size))
				return (0);
			strtab_offset = link->sh_offset;
			strtab_size = link->sh_size;
			// strtab_entsize = link->sh_entsize;
			if (rev_endian)
			{
				reverse_endian(&strtab_offset, sizeof(strtab_offset));
				reverse_endian(&strtab_size, sizeof(strtab_size));
				// reverse_endian(&strtab_entsize, sizeof(strtab_entsize));
			}
			strtab = (char *)(ptr + strtab_offset);
			if (!is_ptr_valid(strtab, strtab_size, ptr, size))
				return (0);
			symtab = (Elf64_Sym *)(ptr + sh_offset);
			sym_count = sh_size / sh_entsize;
			if (!is_ptr_valid(symtab, sizeof (*symtab) * sym_count, ptr, size))
				return (0);
			j = 0;
			while (j < sym_count)
			{
				st_name = symtab[j].st_name;
				st_shndx = symtab[j].st_shndx;
				st_value = symtab[j].st_value;
				if (rev_endian)
				{
					reverse_endian(&st_name, sizeof(st_name));
					reverse_endian(&st_shndx, sizeof(st_shndx));
					reverse_endian(&st_value, sizeof(st_value));
				}
				type = ELF64_ST_TYPE(symtab[j].st_info);
				if (st_name == 0 || st_name >= strtab_size || type == STT_FILE)
				{
					j++;
					continue ;
				}
				// printf("%u / %u\n", st_name, strtab_size);
				name = (char *)(strtab + st_name);
				// ft_putstr_fd(name, 1);
				// ft_putstr_fd("\n", 1);
				symbol = add_symbol(symlst, name, symtab + j, 1);
				if (!symbol)
					return (0);
				sh_type = 0;
				sh_flags = 0;
				section = "";
				if (st_shndx > 0 && st_shndx < shnum)
				{
					sh_type = sh[st_shndx].sh_type;
					sh_flags = sh[st_shndx].sh_flags;
					section = (char *)(shstrtab + sh[st_shndx].sh_name);
				}
				symbol->type = get_symbol_type(st_shndx, sh_type, sh_flags, ELF64_ST_TYPE(symtab[j].st_info), ELF64_ST_BIND(symtab[j].st_info), section);
				symbol->value = (uint64_t)st_value;
				j++;
			}
		}
		i++;
	}
	return (i >= shnum);
}