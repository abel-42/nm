#include "ft_getopt.h"
#include "lib.h"

static void	_print_error_sopt(const char *progname, const char *err, int opt)
{
	ft_putstr_fd(progname, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(err, 2);
	ft_putstr_fd(" -- ", 2);
	write(2, &opt, 1);
	ft_putstr_fd("\n", 2);
	return ;
}

static void	_print_error_lopt(
				const char *progname,
				const char *err,
				const char *opt)
{
	ft_putstr_fd(progname, 2);
	ft_putstr_fd(": option '", 2);
	ft_putstr_fd(opt, 2);
	ft_putstr_fd("' ", 2);
	ft_putstr_fd(err, 2);
	ft_putstr_fd("\n", 2);
	return ;
}

static int	_handle_sopt_error(
			t_ft_getopt_state *state,
			const char *progname,
			char *opt,
			int c
)
{
	if (state->optstring[0] == ':')
	{
		if (c == ':' || !opt)
			return ('?');
		return (':');
	}
	if (!state->opterr)
		return ('?');
	if (c == ':' || !opt)
		_print_error_sopt(progname, ERR_SOPT_INVALID, c);
	else
		_print_error_sopt(progname, ERR_SOPT_ARG_REQUIRED, c);
	return ('?');
}

static int	_handle_lopt_error(
			t_ft_getopt_state *state,
			const char *progname,
			const t_option *opt,
			char *s
)
{
	if (state->optstring[0] == ':')
	{
		if (!opt)
			return ('?');
		return (':');
	}
	if (!state->opterr)
		return ('?');
	if (!opt)
		_print_error_lopt(progname, ERR_LOPT_INVALID, s);
	else if (opt->has_arg == OPT_NO_ARG && state->optarg)
		_print_error_lopt(progname, ERR_LOPT_ARG_NOT_ALLOWED, s);
	else
		_print_error_lopt(progname, ERR_LOPT_ARG_REQUIRED, s);
	return ('?');
}

static const t_option	*_get_longopt(t_ft_getopt_state *state)
{
	size_t			len;
	const t_option	*opts;

	opts = state->longopts;
	if (!opts)
		return (NULL);
	if (state->optarg)
		len = state->optarg - state->nextchar;
	else
		len = ft_strlen(state->nextchar);
	while (opts->name)
	{
		if (ft_strncmp(opts->name, state->nextchar, len) == 0
			&& ft_strlen(opts->name) == len)
			return (opts);
		opts++;
	}
	return (NULL);
}

static int	_handle_long_opt(
	t_ft_getopt_state *state,
	int argc,
	char *const *argv,
	int *longind
)
{
	const t_option	*opt;
	char			*s;

	state->nextchar++;
	state->optarg = ft_strchr(state->nextchar, '=');
	opt = _get_longopt(state);
	s = state->nextchar;
	state->nextchar = NULL;
	state->optind++;
	if (!opt)
		return (_handle_lopt_error(state, argv[0], opt, s));
	if (state->optarg)
	{
		state->optarg++;
		if (opt->has_arg == OPT_NO_ARG)
			return (_handle_lopt_error(state, argv[0], opt, s));
	}
	else
	{
		if (opt->has_arg == OPT_REQ_ARG)
		{
			if (state->optind < argc)
			{
				state->optarg = argv[state->optind];
				state->optind++;
			}
			else
				return (_handle_lopt_error(state, argv[0], opt, s));
		}	
	}
	if (longind)
		*longind = opt - state->longopts;
	if (opt->flag)
	{
		*opt->flag = opt->val;
		return (0);
	}
	return (opt->val);
}

static int	_handle_short_opt(
	t_ft_getopt_state *state,
	int argc,
	char *const *argv
)
{
	int		c;
	char	*opt;

	c = *state->nextchar;
	state->nextchar++;
	if (*state->nextchar == '\0')
	{
		state->nextchar = NULL;
		state->optind++;
	}
	opt = ft_strchr(state->optstring, c);
	if (c == ':' || !opt)
		return (_handle_sopt_error(state, argv[0], opt, c));
	if (opt[1] == ':')
	{
		if (state->nextchar)
		{
			state->optarg = state->nextchar;
			state->nextchar = NULL;
			state->optind++;
		}
		else if (state->optind < argc)
		{
			state->optarg = argv[state->optind];
			state->optind++;
		}
		if (!state->optarg && opt[2] != ':')
		{
			return (_handle_sopt_error(state, argv[0], opt, c));
		}
	}
	return (c);
}

void	ft_getopt_initialize_state(
			t_ft_getopt_state *state,
			const char *optstring,
			const t_option *longopts
)
{
	state->opterr = 1;
	state->optind = 1;
	state->nextchar = NULL;
	state->optstring = optstring;
	state->longopts = longopts;
	state->optarg = NULL;
	return ;
}

int		ft_getopt_long_r(
	t_ft_getopt_state *state,
	int argc,
	char *const *argv,
	int *longind
)
{
	state->optarg = NULL;
	if (state->optind >= argc || argv[state->optind][0] != '-' ||
		(argv[state->optind][0] == '-' && argv[state->optind][1] == '\0'))
		return (-1);
	if (argv[state->optind][0] == '-' && argv[state->optind][1] == '-' &&
		argv[state->optind][2] == '\0')
	{
		state->optind++;
		return (-1);
	}
	if (!state->nextchar)
		state->nextchar = argv[state->optind] + 1;
	if (*state->nextchar == '-')
		return (_handle_long_opt(state, argc, argv, longind));
	return (_handle_short_opt(state, argc, argv));
}
