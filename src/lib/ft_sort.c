#include "lib.h"

void	ft_insertion_sort(void **data, int a, int b, int (*cmp)())
{
	int		i;
	int		j;
	void	*tmp;

	i = a + 1;
	while (i < b)
	{
		j = i;
		while (j > a && cmp(data[j], data[j - 1]) < 0)
		{
			tmp = data[j];
			data[j] = data[j - 1];
			data[j - 1] = tmp;
			j--;
		}
		i++;
	}
}

// void	*ft_insertion_sort(void *first, void *last, int (*cmp)(), void (*swap)())
// {
// 	void	*it;
// 	void	*ti;

// 	it = first + 1;
// 	while (it < last)
// 	{
// 		ti = it;
// 		while (ti > first && cmp(ti, ti - 1) < 0)
// 		{
// 			swap(ti, ti - 1);
// 			ti--;
// 		}
// 		it++;
// 	}
// }

// void	*ft_stable_sort(void *first, void *last, int (*cmp)())
// {
// 	void	*it;

// 	it = first + 1;
// 	while (it < last)
// 	{

// 		it++;
// 	}
// }