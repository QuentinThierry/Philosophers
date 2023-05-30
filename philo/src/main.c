/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 00:38:45 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/28 22:40:56 by qthierry         ###   ########.fr       */
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

static void	end_checker(t_philo *philos)
{
	bool	end;
	size_t	i;

	end = false;
	while (!end)
	{
		usleep(1000);
		i = 0;
		pthread_mutex_lock(philos->mut_eat_end);
		if (philos->eat_to_end != -2)
		{
			pthread_mutex_unlock(philos->mut_eat_end);
			if (!check_nb_eat(philos))
				break ;
		}
		else
			pthread_mutex_unlock(philos->mut_eat_end);
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

static void	start_philos(t_philo *philos, t_timeval *start_time)
{
	size_t	i;

	i = 0;
	gettimeofday(start_time, NULL);
	while (i < philos->nb_philos)
	{
		philos[i].start_time = start_time;
		philos[i].last_meal = get_timestamp(*start_time);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_philo		*philos;
	size_t		i;
	t_timeval	start_time;

	if (!parsing(argc, argv, &philos))
		return (false);
	if (!(fill_philo_1(philos) && fill_philo_2(philos)))
		return (free(philos), printf("Memory exhausted\n"), 1);
	i = 0;
	start_philos(philos, &start_time);
	while (i < philos->nb_philos)
	{
		pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]);
		i++;
	}
	end_checker(philos);
	i = 0;
	while (i < philos->nb_philos)
		pthread_join(philos[i++].thread, NULL);
	free_philos(philos);
	return (0);
}
