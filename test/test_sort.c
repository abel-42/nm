#include "lib.h"
#include <stdio.h>

int main(int ac, char **av)
{
	int	i;

	if (ac > 1)
	{
		ft_insertion_sort((void**)av + 1, 0, ac - 1, &ft_strcmp);
		i = 1;
		while (i < ac)
		{
			printf("[%d] %s\n", i, av[i]);
			i++;
		}
		
	}
	return (0);
}