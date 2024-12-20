/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abemorea <abemorea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 16:22:30 by abemorea          #+#    #+#             */
/*   Updated: 2024/12/20 16:28:41 by abemorea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

void	ft_putnbr_base_fd(uint64_t nbr, const char *base, int fd)
{
	size_t	base_len;

	base_len = ft_strlen(base);
	if (nbr >= base_len)
		ft_putnbr_base_fd(nbr / base_len, base, fd);
	write(fd, &base[nbr % base_len], 1);
	return ;
}
