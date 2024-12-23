#include <unistd.h>
#include <stdio.h>
#include <getopt.h>


extern char	*optarg;

int	main(int ac, char **av)
{
	static struct option options[] = {
		{"foo", 2, NULL, 't'},
	};
	int	opt;

	while ((opt = getopt_long(ac, av, "c", options, NULL)) != -1)
	{
		printf("option: %c\n", opt);
		printf("argument: %s\n", optarg);
	}
	return (0);
}