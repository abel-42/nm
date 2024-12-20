#include <unistd.h>
#include <stdio.h>

extern int optind;

int main(int ac, char **av)
{
	const char *optstr = ":abcf:";
	// char *argv[] = {"-b", "-a", "-d"};
	int	opt;

	// av = argv;
	// ac = 3;
	optind = 0;
	opt = getopt(ac, av, optstr);
	printf("getopt: %d, optind: %d\n", opt, optind);
	opt = getopt(ac, av, optstr);
	printf("getopt: %d, optind: %d\n\n", opt, optind);
	opt = getopt(ac, av, optstr);
	printf("getopt: %d, optind: %d\n\n", opt, optind);
	return (0);
}