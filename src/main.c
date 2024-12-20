#include <elf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "nm.h"


// void	symmerge(void *tab, void *first_1, void *first_2, void *last)

static char	*g_program_name;

void		set_program_name(char *name)
{
	g_program_name = name;
}

static t_sym	*add_symbol_sorted(t_sym **symlst, char *name, void *sym)
{
	t_sym	*new;
	t_sym	*it;
	t_sym	*prev;

	if (!(new = (t_sym*)malloc(sizeof(t_sym))))
		return NULL;
	ft_memset(new, 0, sizeof(t_sym));
	new->name = name;
	new->sym = sym;
	if (!*symlst || ft_strcmp(name, (*symlst)->name) < 0)
	{
		new->next = *symlst;
		*symlst = new;
		return new;
	}
	it = *symlst;
	prev = NULL;
	while (it->next)
	{
		if (ft_strcmp(name, it->next->name) < 0)
		{
			new->next = it->next;
			it->next = new;
			return new;
		}
		it = it->next;
	}
	it->next = new;
	return new;
}

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

// Elf64_Shdr	*search_section(Elf64_Shdr *sh, size_t sh_num, uint32_t sh_type)
// {
// 	size_t	i;

// 	i = 0;
// 	while (i < sh_num)
// 	{
// 		if (sh[i].sh_type == sh_type)
// 			return (sh + i);
// 		i++;
// 	}
// 	return (NULL);
// }

Elf64_Shdr	*search_section(Elf64_Shdr *sh, size_t *offset, size_t sh_num, uint32_t sh_type)
{
	while (*offset < sh_num)
	{
		if (sh[*offset].sh_type == sh_type)
			return (sh + *offset);
		(*offset)++;
	}
	return (NULL);
}

void	handle_elf64(void *ptr)
{
	Elf64_Ehdr	*eh;
	Elf64_Phdr	*ph;
	Elf64_Shdr	*sh;
	Elf64_Shdr	*sec;
	Elf64_Shdr	*shstrhdr;
	Elf64_Shdr	*strhdr;
	char		*shstrtab;
	char		*strtab;
	Elf64_Sym	*symtab;
	size_t		i;
	size_t		j;
	size_t		sh_num;
	size_t		sym_num;
	// size_t		index;
	size_t		offset;
	unsigned char	type;
	t_sym 		*symlst;

	eh = (Elf64_Ehdr *)ptr;
	ft_putstr_fd("size: ", 1);
	ft_putnbr_base_fd(eh->e_shentsize, DEC, 1);
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("num: ", 1);
	ft_putnbr_base_fd(eh->e_shnum, DEC, 1);
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("offset: ", 1);
	ft_putnbr_base_fd(eh->e_phoff, DEC, 1);
	ft_putstr_fd("\n", 1);
	i = 0;
	// ph = (Elf64_Phdr*)ptr + eh->e_phoff;
	ph = (Elf64_Phdr*)(ptr + eh->e_phoff);
	ft_putstr_fd("== PROGRAM HEADERS ==\n", 1);
	while (i < eh->e_phnum)
	{
		ft_putstr_fd("[", 1);
		ft_putnbr_base_fd(i, DEC, 1);
		ft_putstr_fd("] type: ", 1);
		if (ph[i].p_type == PT_NULL)
			ft_putstr_fd("PT_NULL", 1);
		else if (ph[i].p_type == PT_LOAD)
			ft_putstr_fd("PT_LOAD", 1);
		else if (ph[i].p_type == PT_DYNAMIC)
			ft_putstr_fd("PT_DYNAMIC", 1);
		else if (ph[i].p_type == PT_INTERP)
			ft_putstr_fd("PT_INTERP", 1);
		else if (ph[i].p_type == PT_NOTE)
			ft_putstr_fd("PT_NOTE", 1);
		else if (ph[i].p_type == PT_SHLIB)
			ft_putstr_fd("PT_SHLIB", 1);
		else if (ph[i].p_type == PT_PHDR)
			ft_putstr_fd("PT_PHDR", 1);
		else if (ph[i].p_type == PT_LOPROC)
			ft_putstr_fd("PT_LOPROC", 1);
		else if (ph[i].p_type == PT_HIPROC)
			ft_putstr_fd("PT_HIPROC", 1);
		else if (ph[i].p_type == PT_GNU_STACK)
			ft_putstr_fd("PT_GNU_STACK", 1);
		else
			ft_putstr_fd("UNKNOWN", 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	ft_putstr_fd("\n", 1);
	sh = (Elf64_Shdr*)(ptr + eh->e_shoff);
	// shstrtab = (Elf64_Shdr*)(ptr + eh->e_shoff + eh->e_shstrndx);
	shstrhdr = ((Elf64_Shdr*)(ptr + eh->e_shoff)) + eh->e_shstrndx;
	shstrtab = (char*)(ptr + shstrhdr->sh_offset);
	// ft_putstr_fd((const char *)(shstrtab + shstrhdr->sh_name), 1);
	// ft_putstr_fd("\n", 1);
	// if (shstrtab->sh_type == SHT_STRTAB)
	// {
	// 	ft_putstr_fd("shstrtab found!\n", 1);
	// }
	// else
	// {
	// 	ft_putstr_fd("Failed!\n", 1);
	// }
	// if (eh->e_shnum >= SHN_LORESERVE && eh->e_shnum <= SHN_HIRESERVE)
	// {

	// }
	// else
	// {
	// 	sh_num = eh->e_shnum;
	// }
	//SHT_SYMTAB
	// while ((sec = search_section(sh, sh_num, SHT_STRTAB)))
	// {
	// 	index = (sec - sh) / sizeof(Elf64_Shdr);
	// 	ft_putstr_fd("SRTTAB FOUND\n", 1);
	// 	sh_num -= index;
	// 	sh = sec + 1;
	// }
	// offset = 0;
	// while ((sec = search_section(sh, &offset, sh_num, SHT_STRTAB)))
	// {
	// 	ft_putstr_fd("[STRTAB] offset: ", 1);
	// 	ft_putnbr_base_fd(offset, DEC, 1);
	// 	ft_putstr_fd("\n", 1);
	// 	offset++;
	// }
	symlst = NULL;
	i = 1;
	ft_putstr_fd("== SECTION HEADERS ==\n", 1);
	while (i < sh_num)
	{
		ft_putstr_fd("[", 1);
		ft_putnbr_base_fd(i, DEC, 1);
		ft_putstr_fd("] type: ", 1);
		if (sh[i].sh_type == SHT_NULL)
			ft_putstr_fd("SHT_NULL", 1);
		else if (sh[i].sh_type == SHT_PROGBITS)
			ft_putstr_fd("SHT_PROGBITS", 1);
		else if (sh[i].sh_type == SHT_SYMTAB)
			ft_putstr_fd("SHT_SYMTAB", 1);
		else if (sh[i].sh_type == SHT_STRTAB)
			ft_putstr_fd("SHT_STRTAB", 1);
		else if (sh[i].sh_type == SHT_RELA)
			ft_putstr_fd("SHT_RELA", 1);
		else if (sh[i].sh_type == SHT_HASH)
			ft_putstr_fd("SHT_HASH", 1);
		else if (sh[i].sh_type == SHT_DYNAMIC)
			ft_putstr_fd("SHT_DYNAMIC", 1);
		else if (sh[i].sh_type == SHT_NOTE)
			ft_putstr_fd("SHT_NOTE", 1);
		else if (sh[i].sh_type == SHT_NOBITS)
			ft_putstr_fd("SHT_NOBITS", 1);
		else if (sh[i].sh_type == SHT_REL)
			ft_putstr_fd("SHT_REL", 1);
		else if (sh[i].sh_type == SHT_SHLIB)
			ft_putstr_fd("SHT_SHLIB", 1);
		else if (sh[i].sh_type == SHT_DYNSYM)
			ft_putstr_fd("SHT_DYNSYM", 1);
		else if (sh[i].sh_type == SHT_LOPROC)
			ft_putstr_fd("SHT_LOPROC", 1);
		else if (sh[i].sh_type == SHT_HIPROC)
			ft_putstr_fd("SHT_HIPROC", 1);
		else if (sh[i].sh_type == SHT_LOUSER)
			ft_putstr_fd("SHT_LOUSER", 1);
		else if (sh[i].sh_type == SHT_HIUSER)
			ft_putstr_fd("SHT_HIUSER", 1);
		else if (sh[i].sh_type == SHT_NUM)
			ft_putstr_fd("SHT_NUM", 1);
		else
			ft_putstr_fd("UNKNOWN", 1);
		ft_putstr_fd(" (", 1);
		ft_putnbr_base_fd(sh[i].sh_type, DEC, 1);
		ft_putstr_fd("), name: ", 1);
		ft_putstr_fd((const char *)(shstrtab + sh[i].sh_name), 1);
		ft_putstr_fd("\n", 1);
		if (sh[i].sh_type == SHT_STRTAB || sh[i].sh_type == SHT_NULL || sh[i].sh_type == SHT_PROGBITS || sh[i].sh_type == SHT_NOTE || sh[i].sh_type > 15 || sh[i].sh_type == SHT_DYNAMIC || sh[i].sh_type == SHT_NOBITS)
		{
			i++;
			continue;
		}
		// if (sh[i].sh_type == SHT_SYMTAB || sh[i].sh_type == SHT_DYNSYM)
		if (sh[i].sh_type == SHT_SYMTAB)
		// if (1)
		{
			strtab = (char*)(ptr + sh[sh[i].sh_link].sh_offset);
			symtab = (Elf64_Sym*)(ptr + sh[i].sh_offset);
			sym_num = sh[i].sh_size/sh[i].sh_entsize;
			j = 0;
			while (j < sym_num)
			{
				type = ELF64_ST_TYPE(symtab[j].st_info);
				if (type == STT_FILE)
				{
					j++;
					continue;
				}
				add_symbol_sorted(&symlst, (char*)(strtab + symtab[j].st_name), symtab + j);
				ft_putstr_fd("  -> name: ", 1);
				ft_putstr_fd((const char *)(strtab + symtab[j].st_name), 1);
				ft_putstr_fd(", type: ", 1);
				if (type == STT_NOTYPE)
					ft_putstr_fd("STT_NOTYPE", 1);
				else if (type == STT_OBJECT)
					ft_putstr_fd("STT_OBJECT", 1);
				else if (type == STT_FUNC)
					ft_putstr_fd("STT_FUNC", 1);
				else if (type == STT_SECTION)
					ft_putstr_fd("STT_SECTION", 1);
				else if (type == STT_FILE)
					ft_putstr_fd("STT_FILE", 1);
				else if (type == STT_LOPROC)
					ft_putstr_fd("STT_LOPROC", 1);
				else if (type == STT_HIPROC)
					ft_putstr_fd("STT_HIPROC", 1);
				else
					ft_putstr_fd("UNKNOWN", 1);
				ft_putstr_fd("\n", 1);
				j++;
			}
		}
		i++;
	}
	// i = 0;
	// while (i < eh->e_shnum)
	// {
	// 	if (sh[i].sh_type == SHT_SYMTAB || sh[i].sh_type == SHT_DYNSYM)
	// 	{
	// 		ft_putstr_fd("[", 1);
	// 		ft_putnbr_base_fd(i, DEC, 1);
	// 		ft_putstr_fd("] name: ", 1);
	// 		if (sh[i].sh_type == SHT_NULL)
	// 			ft_putstr_fd("SHT_NULL", 1);
	// 		else
	// 			ft_putstr_fd("UNKNOWN", 1);
	// 		ft_putstr_fd(", ", 1);
	// 		ft_putstr_fd((const char *)(shstrtab + sh[i].sh_name), 1);
	// 		ft_putstr_fd("\n", 1);
	// 	}
	// 	i++;
	// }
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("== Symbols ==\n", 1);
	// types: A Bb Cc Dd Gg i I N n p Rr Ss Tt U u  Vv Ww - ?
	// print: {value} {type} {name}@{ver}
	while (symlst)
	{
		ft_putstr_fd(symlst->name, 1);
		ft_putstr_fd("\n", 1);
		symlst = symlst->next;
	}
}

int main(int ac, char **av)
{
	int 		fd;
	int			ret;
	struct stat	data;
	size_t		size;
	const char	*default_files[] = {"a.out", NULL};
	char		**files;
	void		*ptr;
	int			i;

	const char *optstr = ":abcf:";
	int	opt;

	opt = ft_getopt(ac, av, optstr);
	printf("getopt: %d, optind: %d, arg: %s\n", opt, g_optind, g_optarg);
	opt = ft_getopt(ac, av, optstr);
	printf("getopt: %d, optind: %d, arg: %s\n", opt, g_optind, g_optarg);
	opt = ft_getopt(ac, av, optstr);
	printf("getopt: %d, optind: %d, arg: %s\n", opt, g_optind, g_optarg);
	// return (0);
	if (ac < 2)
		files = (char**)default_files;
	else
		files = av + 1;	
	while (*files)
	{
		fd = open(*files, O_RDONLY);
		if (fd < 0)
		{
			ft_putstr_fd(av[0], 2);
			ft_putstr_fd(": ", 2);
			perror(*files);
			files++;
			continue ;
		}
		ret = fstat(fd, &data);
		if (ret != 0)
		{
			perror(av[0]);
			files++;
			continue ;
		}
		size = data.st_size;
		// ft_putstr_fd(*files, 1);
		// ft_putstr_fd(": file size: ", 1);
		// ft_putnbr_base_fd(size, "0123456789", 1);
		// ft_putstr_fd("\n", 1);
		ptr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
		if (ptr == MAP_FAILED)
		{
			ft_putstr_fd(av[0], 2);
			ft_putstr_fd(": ", 2);
			perror(*files);
			files++;
			continue ;
		}
		if (is_elf32(ptr))
		{
			ft_putstr_fd(*files, 1);
			ft_putstr_fd(": file format: ELF32\n", 1);
		}
		else if (is_elf64(ptr))
		{
			ft_putstr_fd(*files, 1);
			ft_putstr_fd(": file format: ELF64\n", 1);
			handle_elf64(ptr);
		}
		else
		{
			ft_putstr_fd(av[0], 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(*files, 2);
			ft_putstr_fd(": file format not recognized\n", 2);
		}
		files++;
	}
	return (0);
}