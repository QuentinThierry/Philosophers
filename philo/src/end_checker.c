/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 14:21:17 by qthierry          #+#    #+#             */
/*   Updated: 2023/07/29 14:23:21 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	check_nb_eat(t_philo *philos)
{
	pthread_mutex_lock(philos->mut_eat_end);
	if (has_all_eaten(philos))
	{
		pthread_mutex_unlock(philos->mut_eat_end);
		broadcast_death(philos, false);
		return (false);
	}
	pthread_mutex_unlock(philos->mut_eat_end);
	return (true);
}

static bool	end_checker_meat(t_philo *philos)
{
	pthread_mutex_lock(philos->mut_eat_end);
	if (philos->eat_to_end != -2)
	{
		pthread_mutex_unlock(philos->mut_eat_end);
		if (!check_nb_eat(philos))
			return (false);
	}
	else
		pthread_mutex_unlock(philos->mut_eat_end);
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
		if (!end_checker_meat(philos))
			break ;
		pthread_mutex_lock(philos->mut_last_meal);
		while (i < philos->nb_philos)
		{
			if (get_timestamp(*philos[i].start_time)
				- philos[i].last_meal >= philos->times[t_die])
			{
				broadcast_death(&philos[i], true);
				end = true;
				break ;
			}
			i++;
		}
		pthread_mutex_unlock(philos->mut_last_meal);
	}
}
