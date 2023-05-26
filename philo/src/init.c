/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:25:20 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/26 19:30:54 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	init_mutexs(pthread_mutex_t **mut_end, pthread_mutex_t **mut_print,
		pthread_mutex_t **mut_eat_end, pthread_mutex_t **mut_last_meal)
{
	*mut_end = ft_calloc(1, sizeof(pthread_mutex_t));
	if (!*mut_end)
		return (false);
	*mut_print = ft_calloc(1, sizeof(pthread_mutex_t));
	if (!*mut_print)
		return (free(*mut_end), false);
	*mut_eat_end = ft_calloc(1, sizeof(pthread_mutex_t));
	if (!*mut_eat_end)
		return (free(*mut_end), free(*mut_print), false);
	*mut_last_meal = ft_calloc(1, sizeof(pthread_mutex_t));
	if (!*mut_last_meal)
		return (free(*mut_end), free(*mut_print), free(mut_eat_end), false);
	**mut_end = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	**mut_print = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	**mut_eat_end = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	**mut_last_meal = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	return (true);
}

static bool	init_callocs(t_fork **forks, bool **is_end, size_t nb_philos)
{
	*forks = ft_calloc(nb_philos, sizeof(t_fork));
	*is_end = ft_calloc(1, sizeof(bool));
	if (!*forks || !*is_end)
		return (free(*forks), free(*is_end), false);
	return (true);
}

bool	fill_philo_1(t_philo *philos)
{
	size_t			i;
	pthread_mutex_t	*mut_end;
	pthread_mutex_t	*mut_print;
	pthread_mutex_t	*mut_eat_end;
	pthread_mutex_t	*mut_last_meal;

	if (!init_mutexs(&mut_end, &mut_print, &mut_eat_end, &mut_last_meal))
		return (false);
	i = 0;
	while (i < philos->nb_philos)
	{
		philos[i].id = i + 1;
		philos[i].state = thinking;
		philos[i].mut_end = mut_end;
		philos[i].mut_eat_end = mut_eat_end;
		philos[i].mut_print = mut_print;
		philos[i].mut_last_meal = mut_last_meal;
		i++;
	}
	return (true);
}

bool	fill_philo_2(t_philo *philos)
{
	t_fork			*forks;
	bool			*is_end;
	size_t			i;

	if (!init_callocs(&forks, &is_end, philos->nb_philos))
	{
		pthread_mutex_destroy(philos->mut_end);
		free(philos->mut_end);
		pthread_mutex_destroy(philos->mut_print);
		free(philos->mut_print);
		pthread_mutex_destroy(philos->mut_eat_end);
		free(philos->mut_eat_end);
		pthread_mutex_destroy(philos->mut_last_meal);
		free(philos->mut_last_meal);
		return (false);
	}
	i = 0;
	while (i < philos->nb_philos)
	{
		philos[i].is_end = is_end;
		philos[i].forks = forks;
		forks[i++].mut = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	}
	return (true);
}
