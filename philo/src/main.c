/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 00:38:45 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/24 22:53:10 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static inline bool	begin_sleep(t_philo *philo,
		t_timeval *act_time, double *timestamps)
{
	if (get_is_dead(philo))
		return (false);
	pthread_mutex_lock(philo->mut_eat_end);
	if (philo->eat_to_end > -1)
		philo->eat_to_end--;
	pthread_mutex_unlock(philo->mut_eat_end);
	print_event(philo, act_time, timestamps, "is sleeping");
	philo->state = get_next_state(philo->state);
	release_forks(philo);
	timestamps[last_meal] = 0;
	timestamps[delay] = 0;
	return (true);
}

static inline bool	begin_think(t_philo *philo,
		t_timeval *act_time, double *timestamps)
{
	if (get_is_dead(philo))
		return (false);
	print_event(philo, act_time, timestamps, "is thinking");
	philo->state = get_next_state(philo->state);
	timestamps[delay] = 0;
	return (true);
}

static inline bool	philo_routine2(t_philo *philo,
	t_timeval *act_time, double *timestamps)
{
	if (philo->state == thinking)
	{
		if (get_is_dead(philo))
			return (NULL);
		try_to_eat(philo, act_time, timestamps);
		timestamps[delay] = 0;
	}
	else if (philo->state == eating
		&& timestamps[delay] >= philo->times[t_eat])
	{
		if (!begin_sleep(philo, act_time, timestamps))
			return (NULL);
	}
	else if (philo->state == sleeping
		&& timestamps[delay] >= philo->times[t_sleep])
	{
		if (!begin_think(philo, act_time, timestamps))
			return (false);
	}
	return (true);
}

void	*philo_routine(void *arg)
{
	t_timeval	act_time;
	t_philo		*philo;
	double		*timestamps;

	timestamps = (double [3]){0, 0, 0};
	philo = arg;
	act_time = philo->start_time;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (true)
	{
		usleep(1);
		refresh_time(&act_time, timestamps);
		if (get_is_dead(philo))
			return (NULL);
		if ((long)timestamps[last_meal] >= philo->times[t_die])
			return (broadcast_death(philo, &act_time, timestamps, 1), NULL);
		if (philo->eat_to_end > -2 && has_all_eaten(philo))
			return (broadcast_death(philo, &act_time, timestamps, 0), NULL);
		else if (!philo_routine2(philo, &act_time, timestamps))
			return (NULL);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_philo		*philos;
	size_t		i;

	if (!parsing(argc, argv, &philos))
		return (false);
	if (!(fill_philo_1(philos) && fill_philo_2(philos)))
		return (free(philos), printf("Memory exhausted\n"), 1);
	i = 0;
	while (i < philos->nb_philos)
	{
		pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]);
		i++;
	}
	i = 0;
	while (i < philos->nb_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	free_philos(philos);
	return (0);
}
