/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:24:11 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/03 17:48:04 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	fill_philos_value(int arg_pos, t_philo *philo, int val)
{
	if (arg_pos == 1)
		philo->nb_philos = val;
	else if (arg_pos > 1 && arg_pos < 5)
		philo->times[arg_pos - 2] = val;
	else if (arg_pos == 5)
		philo->eat_to_end = val;
}

bool	parsing(int argc, char **argv, t_philo *philo)
{
	int		i;
	int		val;
	bool	has_error;

	if (argc > 6 || argc < 5)
		return (printf("Incorrect number of arguments.\n"), false);
	has_error = false;
	i = 1;
	while (i < argc)
	{
		val = ft_atoi(argv[i], &has_error);
		if (has_error || val <= 0)
			return (printf("Invalid argument.\n"), false);
		fill_philos_value(i, philo, val);
		i++;
	}
	if (i == 5)
		fill_philos_value(5, philo, -2);
	philo->state = thinking;
	return (true);
}
