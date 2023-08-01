/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 00:38:45 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/01 18:36:54 by qthierry         ###   ########.fr       */
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

bool	create_threads(t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < philos->nb_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]))
			return (printf("Error on creating thread\n"), 1);
		i++;
	}
	return (true);
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
	if (!create_threads(philos))
		return (free_philos(philos), 1);
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
