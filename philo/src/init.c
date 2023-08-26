/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:25:20 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/26 18:42:25 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	init_mutexs(t_philo *philo)
{
	int	error;

	error = 0;
	philo->mut_end = ft_calloc(1, sizeof(pthread_mutex_t));
	philo->mut_print = ft_calloc(1, sizeof(pthread_mutex_t));
	philo->mut_eat_end = ft_calloc(1, sizeof(pthread_mutex_t));
	philo->mut_last_meal = ft_calloc(1, sizeof(pthread_mutex_t));
	philo->mut_begin_sim = ft_calloc(1, sizeof(pthread_mutex_t));
	if (!philo->mut_end || !philo->mut_print || !philo->mut_eat_end
		|| !philo->mut_last_meal || !philo->mut_begin_sim)
		return (free_mutexs(philo), false);
	error |= pthread_mutex_init(philo->mut_end, NULL);
	error |= pthread_mutex_init(philo->mut_print, NULL);
	error |= pthread_mutex_init(philo->mut_eat_end, NULL);
	error |= pthread_mutex_init(philo->mut_last_meal, NULL);
	error |= pthread_mutex_init(philo->mut_begin_sim, NULL);
	if (error != 0)
		return (free_mutexs(philo), false);
	return (true);
}

static bool	init_callocs(t_fork **forks, bool **is_end, bool **is_start,
	size_t nb_philos)
{
	*forks = ft_calloc(nb_philos, sizeof(t_fork));
	*is_end = ft_calloc(1, sizeof(bool));
	*is_start = ft_calloc(1, sizeof(bool));
	if (!*forks || !*is_end || !*is_start)
		return (free(*forks), free(*is_end), free(*is_start), false);
	return (true);
}

bool	fill_philo_1(t_philo *philos)
{
	size_t			i;
	t_philo			tmp_philo;

	tmp_philo = (t_philo){0};
	if (!init_mutexs(&tmp_philo))
		return (false);
	i = 0;
	while (i < philos->nb_philos)
	{
		philos[i].id = i + 1;
		philos[i].state = thinking;
		philos[i].mut_end = tmp_philo.mut_end;
		philos[i].mut_eat_end = tmp_philo.mut_eat_end;
		philos[i].mut_print = tmp_philo.mut_print;
		philos[i].mut_last_meal = tmp_philo.mut_last_meal;
		philos[i].mut_begin_sim = tmp_philo.mut_begin_sim;
		i++;
	}
	return (true);
}

bool	fill_philo_2(t_philo *philos)
{
	t_fork			*forks;
	bool			*is_end;
	bool			*is_start;
	size_t			i;

	if (!init_callocs(&forks, &is_end, &is_start, philos->nb_philos))
	{
		free_mutexs(philos);
		return (false);
	}
	i = 0;
	while (i < philos->nb_philos)
	{
		philos[i].is_end = is_end;
		philos[i].is_start = is_start;
		philos[i].forks = forks;
		if (pthread_mutex_init(&forks[i].mut, NULL) != 0)
			return (free_mutexs(philos),
				free(forks), free(is_end), free(is_start), false);
		i++;
	}
	return (true);
}
