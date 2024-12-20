#include <elf.h>
#include <fcntl.h>
#include "lib.h"

/**
 * return: -1 read error, 0 not an elf file, 1 elf32, 2 elf64
 */
int	read_elf_header(int fd, void *buf)
{
	ssize_t	ret;
	size_t	size;
	size_t	sig_size;
	const unsigned char elf32_sig[5] = {0x7f, 'E', 'L', 'F', 1};
	const unsigned char elf64_sig[5] = {0x7f, 'E', 'L', 'F', 2};

	sig_size = sizeof(elf64_sig);
	ret = read(fd, buf, sig_size);
	if (ret < 0)
		return (ret);
	if (ret < sig_size || (ft_memcmp(buf, elf32_sig, sig_size) != 0 && ft_memcmp(buf, elf64_sig, sig_size) != 0))
		return (0);
	if (((char*)buf)[4] == 1)
		size = sizeof(Elf32_Ehdr);
	else if (((char*)buf)[4] == 2)
		size = sizeof(Elf64_Ehdr);
	else
		return (0);
	ret = read(fd, buf + sig_size, size - sig_size);
	if (ret < 0)
		return (ret);
	return (((char*)buf)[4]);
}

int main(int ac, char **av)
{
	int fd;
	int elf_class;
	uint8_t buf[64];

	// const unsigned char buf[5];
	// const unsigned char elf32[5] = {0x7f, 'E', 'L', 'F', 1};
	// const unsigned char elf64[5] = {0x7f, 'E', 'L', 'F', 2};
	// const char hex[] = "0123456789ABCDEF";
	// int i;
	// size_t elf32_size;
	// size_t elf64_size;

	if (ac < 2)
	{
		ft_putstr_fd("ft_nm: requiert filename\n", 2);
		return (0);
	}
	fd = open(av[1], O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("ft_nm: couldn't open file\n", 2);
		return (0);
	}
	ft_memset(buf, 0, 64);
	elf_class = read_elf_header(fd, buf);
	if (elf_class < 0)
	{
		ft_putstr_fd("ft_nm: couldn't read file\n", 2);
		return (1);
	}
	else if (elf_class == 0)
	{
		ft_putstr_fd("ft_nm: not en ELF file\n", 2);
		return (1);
	}
	else if (elf_class == 1)
	{
		ft_putstr_fd("ELF32 FILE!\n", 1);
	}
	else if (elf_class == 2)
	{
		ft_putstr_fd("ELF64 FILE!\n", 1);
	}
	// elf32_size = sizeof(Elf32_Ehdr);
	// elf64_size = sizeof(Elf64_Ehdr);
	// ft_putnbr_base_fd(elf32_size, "0123456789", 1);
	// ft_putstr_fd("\n", 1);
	// ft_putnbr_base_fd(elf64_size, "0123456789", 1);
	// ft_putstr_fd("\n", 1);
	// read(fd, (void*)buf, 5);
	// i = 0;
	// while (i < 5)
	// {
	// 	ft_putnbr_base_fd((uint64_t)buf[i], hex, 1);
	// 	ft_putstr(" ");
	// 	i++;
	// }
	// ft_putstr("\n");
	// i = 0;
	// while (i < 5)
	// {
	// 	ft_putnbr_base_fd((uint64_t)elf64[i], hex, 1);
	// 	ft_putstr(" ");
	// 	i++;
	// }
	// ft_putstr("\n");
	// if (ft_memcmp(buf, elf32, 5) == 0)
	// {
	// 	ft_putstr("ELF32 FILE\n");
	// }
	// else if (ft_memcmp(buf, elf64, 5) == 0)
	// {
	// 	ft_putstr("ELF64 FILE\n");
	// }
	// else
	// {
	// 	ft_putstr("NOT AN ELF FILE\n");
	// }
	return (0);
}