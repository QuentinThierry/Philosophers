/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 00:38:45 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/11 19:23:10 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	start_philos(t_philo *philos, t_timeval *start_time)
{
	size_t	i;

	i = 0;
	gettimeofday(start_time, NULL);
	while (i < philos->nb_philos)
	{
		philos[i].origin_time = start_time;
		philos[i].last_meal = get_timestamp(*start_time);
		i++;
	}
}

void	create_threads(t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < philos->nb_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, (void *)philo_routine,
				&philos[i]))
			return ((void)printf("Error on creating thread\n"));
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
	start_philos(philos, &start_time);
	create_threads(philos);
	pthread_mutex_lock(philos->mut_begin_sim);
	*philos->is_start = true;
	pthread_mutex_unlock(philos->mut_begin_sim);
	end_checker(philos);
	i = 0;
	while (i < philos->nb_philos)
	{
		if (pthread_join(philos[i++].thread, NULL))
			printf("Error on join thread number %ld\n", i);
	}
	free_philos(philos);
	return (0);
}
