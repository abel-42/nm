/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abemorea <abemorea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:18:00 by abemorea          #+#    #+#             */
/*   Updated: 2024/12/27 21:22:56 by abemorea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_getopt.h"
#include "nm.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <stdio.h>

static const t_option	*long_options(void)
{
	static const t_option	options[] = {
	{"help", 0, NULL, 'h'},
	{"debug-syms", 0, NULL, 'a'},
	{"extern-only", 0, NULL, 'g'},
	{"undefined-only", 0, NULL, 'u'},
	{"reverse-sort", 0, NULL, 'r'},
	{"no-sort", 0, NULL, 'p'},
	{NULL, 0, 0, 0}
	};

	return (options);
}

void	usage(int fd, const char *progname)
{
	ft_putstr_fd("Usage: ", fd);
	ft_putstr_fd(progname, fd);
	ft_putstr_fd(" [option(s)] [file(s)]\n", fd);
	ft_putstr_fd(" List symbols in [file(s)] (a.out by default).\n", fd);
	ft_putstr_fd(" The options are:\n", fd);
	ft_putstr_fd("  -a, --debug-syms", fd);
	ft_putstr_fd("     Display debugger-only symbols\n", fd);
	ft_putstr_fd("  -g, --extern-only", fd);
	ft_putstr_fd("    Display only external symbols\n", fd);
	ft_putstr_fd("  -p, --no-sort", fd);
	ft_putstr_fd("        Do not sort the symbols\n", fd);
	ft_putstr_fd("  -r, --reverse-sort", fd);
	ft_putstr_fd("   Reverse the sense of the sort\n", fd);
	ft_putstr_fd("  -u, --undefined-only", fd);
	ft_putstr_fd(" Display only undefined symbols\n", fd);
	ft_putstr_fd("  -h, --help", fd);
	ft_putstr_fd("           Display this information\n", fd);
	ft_putstr_fd(progname, fd);
	ft_putstr_fd(": supported targets: elf64 elf32\n", fd);
}

int	main(int ac, char **av)
{
	t_ft_getopt_state	state;
	int					opt;
	char *const			default_files[] = {"a.out", NULL};
	char *const			*files;
	int					fd;
	struct stat			data;
	void				*ptr;
	t_sym				*symlst;
	int					parsing;

	ft_getopt_initialize_state(&state, "hagurp", long_options());
	opt = 0;
	while (opt > -1 && opt != '?')
	{
		opt = ft_getopt_long_r(&state, ac, av, NULL);
		if (opt > -1 && opt != '?')
		{
			printf("opt: %c\n", opt);
		}
		if (opt == 'h')
		{
			usage(1, av[0]);
			return (0);
		}
	}
	if (opt == '?')
	{
		usage(2, av[0]);
		return (1);
	}
	files = av + state.optind;
	if (!*files)
		files = default_files;
	symlst = NULL;
	while (*files)
	{
		fd = open(*files, O_RDONLY);
		if (fd >= 0)
		{
			if (fstat(fd, &data) == 0)
			{
				ptr = mmap(NULL, data.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
				if (ptr != MAP_FAILED)
				{
					parsing = 0;
					if (is_elf32(ptr))
					{
						ft_putstr_fd("ELF32\n", 1);
						parsing = parse_elf32(&symlst, ptr, data.st_size);
					}
					else if (is_elf64(ptr))
					{
						ft_putstr_fd("ELF64\n", 1);
						parsing = parse_elf64(&symlst, ptr, data.st_size);
					}
					if (parsing)
					{
						print_symbols(symlst);
					}
					else
					{
						ft_putstr_fd(av[0], 2);
						ft_putstr_fd(": ", 2);
						ft_putstr_fd(*files, 2);
						ft_putstr_fd(": file format not recognized\n", 2);
					}
					clear_symbols(&symlst);
					munmap(ptr, data.st_size);
					// files++;
					// continue ;
				}
			}
			close(fd);
		}
		ft_putstr_fd(av[0], 2);
		ft_putstr_fd(": ", 2);
		perror(*files);
		files++;
	}
	return (0);
}
