/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 14:21:17 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/01 19:35:54 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	end_with_nb_meat(t_philo *philos)
{
	size_t	i;

	i = 0;

	if (philos[i].nb_eat_max == -2)
	{
		pthread_mutex_unlock(philos->mut_eat_end);
		return (false);
	}
	while (i < philos->nb_philos)
	{
		if (philos[i].nb_eat < philos->nb_eat_max)
		{
			pthread_mutex_unlock(philos->mut_eat_end);
			return (false);
		}
		i++;
	}
	pthread_mutex_unlock(philos->mut_eat_end);
	broadcast_death(philos, false);
	return (true);
}

void	end_checker(t_philo *philos)
{
	bool	end;
	size_t	i;

	end = false;
	while (!end)
	{
		usleep(1000);
		i = 0;
		if (end_with_nb_meat(philos))
			break ;
		while (i < philos->nb_philos)
		{
			pthread_mutex_lock(philos[i].mut_last_meal);
			if (get_timestamp(*philos[i].origin_time)
				- philos[i].last_meal >= philos->times[t_die])
			{
				broadcast_death(&philos[i], true);
				end = true;
				pthread_mutex_unlock(philos[i].mut_last_meal);
				break ;
			}
			pthread_mutex_unlock(philos[i].mut_last_meal);
			i++;
		}
	}
}
