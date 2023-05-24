/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:19:58 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/24 18:20:28 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_atoi(const char *nptr, bool *has_error)
{
	long	res;
	int		sign;

	res = 0;
	sign = 1;
	while (*nptr == ' ' || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	if (!ft_isdigit(*nptr))
	{
		if (*nptr == '-')
			sign = -1;
		else if (*nptr != '+')
			return (*has_error = true, 0);
		nptr++;
	}
	while (ft_isdigit(*nptr))
	{
		res = res * 10 + *nptr++ - '0';
		if (res > __INT_MAX__)
			return (*has_error = true, 0);
	}
	return (res * sign);
}
