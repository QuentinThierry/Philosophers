/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:24:11 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/11 19:34:56 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	fill_philos_value(int arg_pos, t_philo *philos, int val)
{
	size_t	j;

	j = 0;
	if (arg_pos == 1)
	{
		while (j < (size_t)val)
			philos[j++].nb_philos = val;
	}
	else if (arg_pos > 1 && arg_pos < 5)
	{
		while (j < philos->nb_philos)
			philos[j++].times[arg_pos - 2] = val;
	}
	else if (arg_pos == 5)
	{
		while (j < philos->nb_philos)
			philos[j++].nb_eat_max = val;
	}
}

bool	parsing(int argc, char **argv, t_philo **philos)
{
	int		i;
	int		val;
	bool	has_error;

	if (argc > 6 || argc < 5)
		return (printf("Incorrect number of arguments.\n"), false);
	has_error = false;
	i = 1;
	*philos = NULL;
	while (i < argc)
	{
		val = ft_atoi(argv[i], &has_error);
		if (has_error || val <= 0)
			return (free(*philos), printf("Invalid argument.\n"), false);
		if (i == 1)
			*philos = ft_calloc(val, sizeof(t_philo));
		if (!*philos)
			return (printf("Memory exhausted.\n"), false);
		fill_philos_value(i, *philos, val);
		i++;
	}
	if (i == 5)
		fill_philos_value(5, *philos, -2);
	return (true);
}
